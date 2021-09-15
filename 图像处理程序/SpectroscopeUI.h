#pragma once

#include "DrawToolsUI.H"
#include "ui_SpectroscopeUI.h"

class SpectroscopeUI : public DrawToolsUI
{
	Q_OBJECT

public:
	SpectroscopeUI(QWidget *parent = Q_NULLPTR);
	~SpectroscopeUI();
	void ChangeColor(LayerModel*, MyImage::RGBQUAD, MyImage::RGBQUAD, int, PaintCommand*);
private:
	Ui::SpectroscopeUI ui;
	MyImage::RGBQUAD _target;
	int _t;//ÈÝ²î
};
