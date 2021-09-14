#include "ChangeHSL.h"

ChangeHSL::ChangeHSL(DrawCanvas* device, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	auto layer = device->GetLayers();
	auto selected = device->GetSelected();
	for (int i = 0; i < selected.size(); ++i) {
		layer[selected[i]]->BeginDraw();
	}
	connect(ui.H, &QAbstractSlider::valueChanged, [this, device](int v) {
		double h, s, l;
		h = ui.H->value();
		s = ui.S->value();
		l = ui.L->value();
		h -= 180;
		s -= 100;
		l -= 100;
		auto layer = device->GetLayers();
		auto selected = device->GetSelected();
		for (int i = 0; i < selected.size(); ++i) {
			ChangeH(layer[selected[i]], h, layer[selected[i]]->GetImage());
			ChangeS(layer[selected[i]], s, layer[selected[i]]->GetBuffer());
			ChangeL(layer[selected[i]], l, layer[selected[i]]->GetBuffer());
		}
		ui.HT->setText(QString::number(v));
		device->ReDraw();
		device->update();
		});
	connect(ui.L, &QAbstractSlider::valueChanged, [this, device](int v) {
		double h, s, l;
		h = ui.H->value();
		s = ui.S->value();
		l = ui.L->value();
		h -= 180;
		s -= 100;
		l -= 100;
		auto layer = device->GetLayers();
		auto selected = device->GetSelected();
		for (int i = 0; i < selected.size(); ++i) {
			ChangeH(layer[selected[i]], h, layer[selected[i]]->GetImage());
			ChangeS(layer[selected[i]], s, layer[selected[i]]->GetBuffer());
			ChangeL(layer[selected[i]], l, layer[selected[i]]->GetBuffer());
		}
		ui.LT->setText(QString::number(v));
		device->ReDraw();
		device->update();
		});
	connect(ui.S, &QAbstractSlider::valueChanged, [this, device](int v) {
		double h, s, l;
		h = ui.H->value();
		s = ui.S->value();
		l = ui.L->value();
		h -= 180;
		s -= 100;
		l -= 100;
		auto layer = device->GetLayers();
		auto selected = device->GetSelected();
		for (int i = 0; i < selected.size(); ++i) {
			ChangeH(layer[selected[i]], h, layer[selected[i]]->GetImage());
			ChangeS(layer[selected[i]], s, layer[selected[i]]->GetBuffer());
			ChangeL(layer[selected[i]], l, layer[selected[i]]->GetBuffer());
		}
		ui.ST->setText(QString::number(v));
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

ChangeHSL::~ChangeHSL()
{
}

void ChangeHSL::ChangeH(LayerModel* layer, int val, MyImage::Image& res) {
	int w = layer->GetImage().GetWidth();
	int total = w * layer->GetImage().GetHeight();
	layer->GetBuffer().PixelChanged.BlockSign(true);
	for (int i = 0; i < total; ++i) {
		auto t = res.GetPixel(i);
		double h, s, l;
		RGBToHSL(t, h, s, l);
		h *= 255;
		h = h + val;
		if (h >= 360) h -= 360;
		else if (h < 0) h += 360;
		layer->GetBuffer().SetPixel(i, HSLToRGB(h/=255, s, l));
	}
	layer->GetBuffer().PixelChanged.BlockSign(false);
}

void ChangeHSL::ChangeS(LayerModel* layer, int value, MyImage::Image& res) {
	int w = layer->GetImage().GetWidth();
	int total = w * layer->GetImage().GetHeight();
	using namespace std;
	double percent = value / 100.0;
	layer->GetBuffer().PixelChanged.BlockSign(true);
	for (int i = 0; i < total; ++i) {
		auto color = res.GetPixel(i);
		double h, s, l;
		RGBToHSL(color, h, s, l);
		double alp;
		if (percent >= 0) {
			if (percent + s >= 1) {
				alp = s;
			}
			else {
				alp = 1 - percent;
			}
			alp = 1 / alp - 1;
			auto cal = [l, alp](unsigned char& v) {
				v = v + (v - l * 255) * alp;
			};
			cal(color.rgbRed);
			cal(color.rgbGreen);
			cal(color.rgbBlue);
			layer->GetBuffer().SetPixel(i % w, i / w, color);
		}
		else {
			alp = percent;
			auto cal = [l, alp](unsigned char& v) {
				v = l * 255 + (v - l * 255) * (1 + alp);
			};
			cal(color.rgbRed);
			cal(color.rgbGreen);
			cal(color.rgbBlue);
			layer->GetBuffer().SetPixel(i % w, i / w, color);
		}
	}
	layer->GetBuffer().PixelChanged.BlockSign(false);
}

void ChangeHSL::ChangeL(LayerModel* layer, int value, MyImage::Image& res) {
	double para = value / 100.0;
	int w = layer->GetImage().GetWidth();
	int total = w * layer->GetImage().GetHeight();
	std::function<void(unsigned char& v, double a)> cal;
	if (para > 0) {
		cal = [](unsigned char& v, double a) {
			v = v * (1 - a) + 255 * a;
		};
	}
	else {
		cal = [](unsigned char& v, double a) {
			v = v + v * a;
		};
	}
	layer->GetBuffer().PixelChanged.BlockSign(true);
	for (int i = 0; i < total; ++i) {
		auto color = res.GetPixel(i);
		cal(color.rgbRed, para);
		cal(color.rgbGreen, para);
		cal(color.rgbBlue, para);
		layer->GetBuffer().SetPixel(i % w, i / w, color);
	}
	layer->GetBuffer().PixelChanged.BlockSign(true);
}
