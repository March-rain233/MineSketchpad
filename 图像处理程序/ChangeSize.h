#pragma once

#include <QDialog>
#include "ui_ChangeSize.h"
#include "DrawCanvas.h"

class ChangeSize : public QDialog
{
	Q_OBJECT

public:
	ChangeSize(DrawCanvas* canvas, QWidget *parent = Q_NULLPTR);
	~ChangeSize();
	int GetWidth();
	int GetHeight();
private:
	Ui::ChangeSize ui;
};
