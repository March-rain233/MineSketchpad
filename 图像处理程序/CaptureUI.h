#pragma once

#include <QDialog>
#include "ui_CaptureUI.h"

class CaptureUI : public QDialog
{
	Q_OBJECT

public:
	CaptureUI(QWidget *parent = Q_NULLPTR);
	~CaptureUI();
	int GetX1();
	int GetX2();
	int GetY1();
	int GetY2();
private:;
	Ui::CaptureUI ui;
};
