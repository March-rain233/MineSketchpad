#pragma once

#include <QDialog>
#include "ui_ChangeHSL.h"
#include "DrawCanvas.h"

class ChangeHSL : public QDialog
{
	Q_OBJECT

public:
	ChangeHSL(DrawCanvas* device, QWidget *parent = Q_NULLPTR);
	~ChangeHSL();
	void ChangeH(LayerModel*, int, MyImage::Image& res);
	void ChangeS(LayerModel*, int, MyImage::Image& res);
	void ChangeL(LayerModel*, int, MyImage::Image& res);
private:
	Ui::ChangeHSL ui;
};
