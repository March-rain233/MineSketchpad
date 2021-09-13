#pragma once

#include <QWidget>
#include "ui_MyPalette.h"

class MyPalette : public QWidget
{
	Q_OBJECT

public:
	MyPalette(QWidget *parent = Q_NULLPTR);
	~MyPalette();

private:
	Ui::MyPalette ui;
	bool _isHsl;
};
