#include "Binaryzation.h"

Binaryzation::Binaryzation(DrawCanvas* device,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	auto layer = device->GetLayers();
	auto selected = device->GetSelected();
	for (int i = 0; i < selected.size(); ++i) {
		layer[selected[i]]->BeginDraw();
	}
	connect(ui.t, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this, device](int v) {
		auto layer = device->GetLayers();
		auto selected = device->GetSelected();
		for (int i = 0; i < selected.size(); ++i) {
			MyImage::Image& res = layer[selected[i]]->GetImage();
			int total = res.GetWidth() * res.GetHeight();
			layer[selected[i]]->GetBuffer().PixelChanged.BlockSign(true);
			for (int j = 0; j < total; ++j) {
				auto color = res.GetPixel(j);
				if ((int)color.rgbBlue + color.rgbGreen + color.rgbRed < 3 * v) {
					layer[selected[i]]->GetBuffer().SetPixel(j, MyImage::RGBQUAD{ 0,0,0,255 });
				}
				else {
					layer[selected[i]]->GetBuffer().SetPixel(j, MyImage::RGBQUAD{ 255,255,255,255 });
				}
			}
			layer[selected[i]]->GetBuffer().PixelChanged.BlockSign(false);
		}
		device->ReDraw();
		device->update();
		});
	connect(ui.buttonBox, &QDialogButtonBox::rejected, [this, device] {
		auto layer = device->GetLayers();
		auto selected = device->GetSelected();
		for (int i = 0; i < selected.size(); ++i) {
			layer[selected[i]]->CancelDraw();
		}
		device->ReDraw();
		device->update();
		reject();
		});
	connect(ui.buttonBox, &QDialogButtonBox::accepted, [this, device] {
		auto layer = device->GetLayers();
		auto selected = device->GetSelected();
		GroupCommand* group = new GroupCommand();
		for (int i = 0; i < selected.size(); ++i) {
			CopyCommand* copy = new CopyCommand();
			copy->Last = layer[selected[i]]->GetImage().Clone();
			copy->After = layer[selected[i]]->GetBuffer().Clone();
			copy->Target = layer[selected[i]];
			group->PushBackCommand(copy);
			layer[selected[i]]->EndDraw();
		}
		device->PushCommand(group);
		device->update();
		accept();
		});
}

Binaryzation::~Binaryzation()
{
}
