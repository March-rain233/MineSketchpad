#pragma once

#include <QDialog>
#include "ui_CreateImage.h"

class CreateImage : public QDialog
{
	Q_OBJECT

public:
	CreateImage(QWidget *parent = Q_NULLPTR);
	~CreateImage();
	QString GetFilename();
	int GetHeight();
	int GetWidth();
	QColor GetColor();
private:
	void SetWidth(int);
	void SetHeight(int);
	void SetColor(QColor);
private:
	Ui::CreateImage _ui;
	QString _filename;
	int _height;
	int _width;
	QColor _color = Qt::white;
};
