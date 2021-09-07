#pragma once

#include <QWidget>
#include <qmap.h>
#include <qstring.h>
#include "DrawCanvas.h"
#include "ui_tools.h"

class Tools : public QWidget
{
	Q_OBJECT

public:
	Tools(QWidget *parent = Q_NULLPTR);
	~Tools();
	void Rigister(DrawCanvas*);
private:
	void SetTool(QString);
private:
	Ui::Tools ui;
	DrawCanvas* _device;
};
