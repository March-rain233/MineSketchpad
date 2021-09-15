#include "SpectroscopeUI.h"
#include <qcolordialog.h>
#include "MyPaletteModel.h"

SpectroscopeUI::SpectroscopeUI(QWidget *parent)
	: DrawToolsUI(parent)
{
	ui.setupUi(this);
	_target = MyImage::RGBQUAD{ 255,255,255,255 };
	ui.color->setIcon(createColorIcon(QColor(_target.rgbRed, _target.rgbGreen, _target.rgbBlue), ui.color->iconSize()));
	connect(ui.change, &QPushButton::clicked, [this]() {
		auto layer = GetTool().GetDevice().GetLayers();
		auto selected = GetTool().GetDevice().GetSelected();
		GroupCommand* group = new GroupCommand();
		for (int i = 0; i < selected.size(); ++i) {
			PaintCommand* com = new PaintCommand(*layer[selected[i]]);
			ChangeColor(layer[selected[i]], _target,
				MyPaletteModel::GetInstance().GetFrontColor(), ui.t->value(), com);
			group->PushBackCommand(com);
		}
		GetTool().GetDevice().PushCommand(group);
		GetTool().GetDevice().ReDraw();
		GetTool().GetDevice().update();
		});
	connect(ui.color, &QPushButton::clicked, [this]() {
		auto temp = QColorDialog::getColor(Qt::black, this);
		_target.rgbRed = temp.red();
		_target.rgbGreen = temp.green();
		_target.rgbBlue = temp.blue();
		_target.rgbReserved = 255;
		ui.color->setIcon(createColorIcon(QColor(_target.rgbRed, _target.rgbGreen, _target.rgbBlue), ui.color->iconSize()));
		});
}

SpectroscopeUI::~SpectroscopeUI()
{
}

void SpectroscopeUI::ChangeColor(LayerModel* layer, MyImage::RGBQUAD target, MyImage::RGBQUAD old, int t, PaintCommand* c) {
	int total = layer->GetImage().GetWidth() * layer->GetImage().GetHeight();
	auto image = layer->GetImage().GetBits();
	layer->BeginDraw();
	int w = layer->GetImage().GetWidth();
	auto comp = [t](unsigned char v, unsigned char old)->bool {
		if (v <= old + t && v >= old - t) {
			return true;
		}
		return false;
	};
	for (int i = 0; i < total; ++i) {
		if (comp(image[i].rgbRed, old.rgbRed) && 
			comp(image[i].rgbGreen, old.rgbGreen) && 
			comp(image[i].rgbBlue, old.rgbBlue)) {
			c->SetPixel(i % w, i / w, target);
		}
	}
	layer->EndDraw();
}
