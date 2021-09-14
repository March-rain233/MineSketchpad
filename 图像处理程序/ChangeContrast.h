#pragma once

#include <QDialog>
#include "ui_ChangeContrast.h"
#include "DrawCanvas.h"

class ChangeContrast : public QDialog
{
	Q_OBJECT

public:
	ChangeContrast(DrawCanvas*, QWidget *parent = Q_NULLPTR);
	~ChangeContrast();

private:
	Ui::ChangeContrast ui;
};
