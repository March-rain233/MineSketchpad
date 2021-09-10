#include "MyPalette.h"
#include "MyPaletteModel.h"
#include "Image.h"
#include "qdebug.h"
#include <qbuttongroup.h>

void RGBToHSL(MyImage::RGBQUAD color, double& h, double& s, double& l) {
	double M = std::max(color.rgbBlue, std::max(color.rgbGreen, color.rgbRed));
	double m = std::min(color.rgbBlue, std::min(color.rgbGreen, color.rgbRed));
	double c = M - m;
	l = (M + m) / 2;
	if (c == 0) {
		h = s = 0;
	}
	else {
		s = (l >= 127.5 ? c / (510 - M - m) : c / (M + m)) * 255;
		if (M == color.rgbBlue) {
			h = 240.0 + (color.rgbRed - color.rgbGreen) / c;
		}
		else if (M == color.rgbGreen) {
			h = 120.0 + (color.rgbBlue - color.rgbRed) / c;
		}
		else {
			h = (color.rgbGreen - color.rgbBlue) / c;
		}
	}
	if (h < 0) {
		h += 360;
	}
	if (h > 360) {
		h -= 360;
	}
	s = s * 100 / 255;
	l = l * 100 / 255;
}
MyImage::RGBQUAD HSLToRGB(double h, double s, double l) {
	MyImage::RGBQUAD res;
	res.rgbReserved = 255;
	l = l * 255 / 100;
	s = s * 255 / 100;
	double c1o60 = 1.0 / 60.0;
	double c1o255 = 1.0 / 255.0;
	double v1, v2, v3, h1;
	if (s == 0) {
		res.rgbBlue = res.rgbGreen = res.rgbRed= l;
	}
	else {
		if (l < 127.5) {
			v2 = c1o255 * l * (255 + s);
		}
		else {
			v2 = l + s - c1o255 * s * l;
		}
		v1 = 2 * l - v2;
		v3 = v2 - v1;
		h1 = h + 120.0;
		if (h1 >= 360.0)
			h1 -= 360.0;
		if (h1 < 60.0) {
			res.rgbRed = (int)(v1 + v3 * h1 * c1o60);
		}
		else if (h1 < 180.0) {
			res.rgbRed = (int)v2;
		}
		else if (h1 < 240.0) {
			res.rgbRed = (int)(v1 + v3 * (4 - h1 * c1o60));
		}
		else {
			res.rgbRed = (int)v1;
		}

		h1 = h;
		if (h1 < 60.0) {
			res.rgbGreen = (int)(v1 + v3 * h1 * c1o60);
		}
		else if (h1 < 180.0) {
			res.rgbGreen = (int)v2;
		}
		else if (h1 < 240.0) {
			res.rgbGreen = (int)(v1 + v3 * (4 - h1 * c1o60));
		}
		else {
			res.rgbGreen = (int)v1;
		}

		h1 = h - 120.0;
		if (h1 < 0.0) {
			h1 += 360.0;
		}
		if (h1 < 60.0) {
			res.rgbBlue = (int)(v1 + v3 * h1 * c1o60);
		}
		else if (h1 < 180.0) {
			res.rgbBlue = (int)v2;
		}
		else if (h1 < 240.0) {
			res.rgbBlue = (int)(v1 + v3 * (4 - h1 * c1o60));
		}
		else {
			res.rgbBlue = (int)v1;
		}
	}
	return res;
}
MyPalette::MyPalette(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	MyPaletteModel& instance = MyPaletteModel::GetInstance();
	ui.R->setRange(0, 255);
	ui.G->setRange(0, 255);
	ui.B->setRange(0, 255);
	ui.H->setRange(0, 360);
	ui.S->setRange(0, 100);
	ui.L->setRange(0, 100);
	instance.FrontColorChange += [this](MyImage::RGBQUAD v)->void {
		ui.R->setValue(v.rgbRed);
		ui.G->setValue(v.rgbGreen);
		ui.B->setValue(v.rgbBlue);
		double h, l, s;
		RGBToHSL(v, h, s, l);
		ui.H->setValue(h);
		ui.S->setValue(s);
		ui.L->setValue(l);
		ui.RV->setText(QString::number(v.rgbRed));
		ui.GV->setText(QString::number(v.rgbGreen));
		ui.BV->setText(QString::number(v.rgbBlue));
		ui.HV->setText(QString::number((int)h));
		ui.SV->setText(QString::number((int)s));
		ui.LV->setText(QString::number((int)l));
		qDebug() << "color";
	};
	connect(ui.R, &QSlider::sliderMoved, [&instance](int v) {
		auto t = instance.GetFrontColor();
		if (t.rgbRed == v) { return; }
		t.rgbRed = v;
		instance.SetFrontColor(t);
		});
	connect(ui.G, &QSlider::sliderMoved, [&instance](int v) {
		auto t = instance.GetFrontColor();
		if (t.rgbGreen == v) { return; }
		t.rgbGreen = v;
		instance.SetFrontColor(t);
		});
	connect(ui.B, &QSlider::sliderMoved, [&instance](int v) {
		auto t = instance.GetFrontColor();
		if (t.rgbBlue == v) { return; }
		t.rgbBlue = v;
		instance.SetFrontColor(t);
		});
	connect(ui.H, &QSlider::sliderMoved, [&instance, this](int v) {
		double h, s, l;
		RGBToHSL(instance.GetFrontColor(), h, s, l);
		h = v;
		instance.SetFrontColor(HSLToRGB(h, s, l));
		});
	connect(ui.S, &QSlider::sliderMoved, [&instance, this](int v) {
		double h, s, l;
		RGBToHSL(instance.GetFrontColor(), h, s, l);
		s = v;
		instance.SetFrontColor(HSLToRGB(h, s, l));
		});
	connect(ui.L, &QSlider::sliderMoved, [&instance, this](int v) {
		double h, s, l;
		RGBToHSL(instance.GetFrontColor(), h, s, l);
		l = v;
		instance.SetFrontColor(HSLToRGB(h, s, l));
		});
	
	connect(ui.RGB, &QAbstractButton::toggled, [this](bool v) {
		if (v) {
			ui.RGBmenu->show();
		}
		else {
			ui.RGBmenu->hide();
		}
		});
	connect(ui.HSL, &QAbstractButton::toggled, [this](bool v) {
		if (v) {
			ui.HSLmenu->show();
		}
		else {
			ui.HSLmenu->hide();
		}
		});
}

MyPalette::~MyPalette()
{
}
