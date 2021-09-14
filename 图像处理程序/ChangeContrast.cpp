#include "ChangeContrast.h"
#include "Image.h"
void Handler(MyImage::Image* buffer, MyImage::Image* res, double c, double b) {
	int total = res->GetWidth() * res->GetHeight();
	float k = tan((45 + 44 * c) / 180.0 * 3.1415926535);
	auto cal = [b, c, k](unsigned char& v) {
		v = ((v * 255.0 - 127.5 * (1.0 - b)) * k + 127.5 * (1.0 + b)) / 255.0;
	};
	for (int i = 0; i < total; ++i) {
		auto t = res->GetPixel(i);
		cal(t.rgbRed);
		cal(t.rgbGreen);
		cal(t.rgbBlue);
		buffer->SetPixel(i, t);
	}
}

ChangeContrast::ChangeContrast(DrawCanvas* device, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	auto layer = device->GetLayers();
	auto selected = device->GetSelected();
	for (int i = 0; i < selected.size(); ++i) {
		layer[selected[i]]->BeginDraw();
	}
	connect(ui.C, &QAbstractSlider::valueChanged, [this, device] {
		double b = (ui.L->value()-255) / 255.0;
		double c = (ui.C->value()-255) / 255.0;
		ui.LT->setText(QString::number(b * 255));
		ui.CT->setText(QString::number(c * 255));
		auto layer = device->GetLayers();
		auto select = device->GetSelected();

		for (int i = 0; i < select.size(); ++i) {
			layer[select[i]]->PixelChanged.BlockSign(true);
			Handler(&layer[select[i]]->GetBuffer(), &layer[select[i]]->GetImage(), c, b);
			layer[select[i]]->PixelChanged.BlockSign(false);
		}
		device->ReDraw();
		device->update();
		});
	connect(ui.L, &QAbstractSlider::valueChanged, [this, device] {
		double b = (ui.L->value() - 255) / 255.0;
		double c = (ui.C->value() - 255) / 255.0;
		ui.LT->setText(QString::number(b * 255));
		ui.CT->setText(QString::number(c * 255));
		auto layer = device->GetLayers();
		auto select = device->GetSelected();
		for (int i = 0; i < select.size(); ++i) {
			layer[select[i]]->PixelChanged.BlockSign(true);
			Handler(&layer[select[i]]->GetBuffer(), &layer[select[i]]->GetImage(), c, b);
			layer[select[i]]->PixelChanged.BlockSign(false);
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
			group->PushBackCommand(copy);
			layer[selected[i]]->EndDraw();
		}
		device->PushCommand(group);
		device->update();
		accept();
		});
}

ChangeContrast::~ChangeContrast()
{
}
