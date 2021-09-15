#pragma once

#include <QDialog>
#include "ui_ChangeRGB.h"
#include "DrawCanvas.h"

class ChangeRGB : public QDialog
{
	Q_OBJECT

public:
	ChangeRGB(DrawCanvas* device, QWidget *parent = Q_NULLPTR);
	~ChangeRGB();

private:
	Ui::ChangeRGB ui;
};
