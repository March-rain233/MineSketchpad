#include "ChangeRGB.h"
#include "Image.h"

void ColorBalance(MyImage::Image& buffer, MyImage::Image& res, int cR, int cG, int cB) {
	int size = res.GetWidth() * res.GetHeight();
	auto check = [](double v) {
		return v < 0 ? 0 : (v > 255) ? 255 : v;
	};
	auto t = buffer.GetBits();
	auto r = res.GetBits();
	for (int i = 0; i < size; ++i) {
		t[i].rgbRed = check(r[i].rgbRed + cR);
		t[i].rgbGreen = check(r[i].rgbGreen + cG);
		t[i].rgbBlue = check(r[i].rgbBlue + cB);
		t[i].rgbReserved = 255;
	}
}

ChangeRGB::ChangeRGB(DrawCanvas* device, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	auto layer = device->GetLayers();
	auto selected = device->GetSelected();
	for (int i = 0; i < selected.size(); ++i) {
		layer[selected[i]]->BeginDraw();
	}
	connect(ui.R, &QAbstractSlider::valueChanged, [this, device](int v) {
		double r, g, b;
		r = ui.R->value();
		g = ui.G->value();
		b = ui.B->value();
		r -= 255;
		g -= 255;
		b -= 255;
		auto layer = device->GetLayers();
		auto selected = device->GetSelected();
		for (int i = 0; i < selected.size(); ++i) {
			ColorBalance(layer[selected[i]]->GetBuffer(), layer[selected[i]]->GetImage(), r, g, b);
		}
		ui.RT->setText(QString::number(r));
		ui.GT->setText(QString::number(g));
		ui.BT->setText(QString::number(b));
		device->ReDraw();
		device->update();
		});
	connect(ui.G, &QAbstractSlider::valueChanged, [this, device](int v) {
		double r, g, b;
		r = ui.R->value();
		g = ui.G->value();
		b = ui.B->value();
		r -= 255;
		g -= 255;
		b -= 255;
		auto layer = device->GetLayers();
		auto selected = device->GetSelected();
		for (int i = 0; i < selected.size(); ++i) {
			ColorBalance(layer[selected[i]]->GetBuffer(), layer[selected[i]]->GetImage(), r, g, b);
		}
		ui.RT->setText(QString::number(r));
		ui.GT->setText(QString::number(g));
		ui.BT->setText(QString::number(b));
		device->ReDraw();
		device->update();
		});
	connect(ui.B, &QAbstractSlider::valueChanged, [this, device](int v) {
		double r, g, b;
		r = ui.R->value();
		g = ui.G->value();
		b = ui.B->value();
		r -= 255;
		g -= 255;
		b -= 255;
		auto layer = device->GetLayers();
		auto selected = device->GetSelected();
		for (int i = 0; i < selected.size(); ++i) {
			ColorBalance(layer[selected[i]]->GetBuffer(), layer[selected[i]]->GetImage(), r, g, b);
		}
		ui.RT->setText(QString::number(r));
		ui.GT->setText(QString::number(g));
		ui.BT->setText(QString::number(b));
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

ChangeRGB::~ChangeRGB()
{
}
