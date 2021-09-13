#include "MyPalette.h"
#include "MyPaletteModel.h"
#include "Image.h"
#include "qdebug.h"
#include <qbuttongroup.h>
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
	_isHsl = false;
	instance.FrontColorChange += [this](MyImage::RGBQUAD v)->void {
		auto sliderSetter = [](QAbstractSlider* slider, unsigned char v) {
			if (slider->value() == v) {
				return;
			}
			slider->blockSignals(true);
			slider->setValue(v);
			slider->blockSignals(false);
		};
		sliderSetter(ui.R, v.rgbRed);
		sliderSetter(ui.G, v.rgbGreen);
		sliderSetter(ui.B, v.rgbBlue);
		ui.RV->setText(QString::number(v.rgbRed));
		ui.GV->setText(QString::number(v.rgbGreen));
		ui.BV->setText(QString::number(v.rgbBlue));
		double h, l, s;
		RGBToHSL(v, h, s, l);
		h *= 360;
		s *= 100;
		l *= 100;
		if (!_isHsl) {
			sliderSetter(ui.H, h);
			sliderSetter(ui.S, s);
			sliderSetter(ui.L, l);
			_isHsl = false;
		}
		ui.HV->setText(QString::number(ui.H->value()));
		ui.SV->setText(QString::number(ui.S->value()));
		ui.LV->setText(QString::number(ui.L->value()));
	};
	connect(ui.R, &QSlider::valueChanged, [&instance, this](int v) {
		_isHsl = false;
		auto t = instance.GetFrontColor();
		t.rgbRed = v;
		instance.SetFrontColor(t);
		});
	connect(ui.G, &QSlider::valueChanged, [&instance, this](int v) {
		_isHsl = false;
		auto t = instance.GetFrontColor();
		t.rgbGreen = v;
		instance.SetFrontColor(t);
		});
	connect(ui.B, &QSlider::valueChanged, [&instance, this](int v) {
		_isHsl = false;
		auto t = instance.GetFrontColor();
		t.rgbBlue = v;
		instance.SetFrontColor(t);
		});
	connect(ui.H, &QSlider::valueChanged, [&instance, this](int v) {
		_isHsl = true;
		double h, s, l;
		h = ui.H->value();
		s = ui.S->value();
		l = ui.L->value();
		instance.SetFrontColor(HSLToRGB(h / 360.0, s / 100.0, l / 100.0));
		});
	connect(ui.S, &QSlider::valueChanged, [&instance, this](int v) {
		_isHsl = true;
		double h, s, l;
		//RGBToHSL(instance.GetFrontColor(), h, s, l);
		h = ui.H->value();
		s = ui.S->value();
		l = ui.L->value();
		instance.SetFrontColor(HSLToRGB(h / 360.0, s / 100.0, l / 100.0));
		});
	connect(ui.L, &QSlider::valueChanged, [&instance, this](int v) {
		_isHsl = true;
		double h, s, l;
		//RGBToHSL(instance.GetFrontColor(), h, s, l);
		h = ui.H->value();
		s = ui.S->value();
		l = ui.L->value();
		instance.SetFrontColor(HSLToRGB(h / 360.0, s / 100.0, l / 100.0));
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
