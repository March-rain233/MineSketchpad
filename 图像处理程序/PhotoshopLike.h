#pragma once

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include "ui_PhotoshopLike.h"

class PhotoshopLike : public QMainWindow {
	Q_OBJECT

public:
	PhotoshopLike(QWidget* parent = Q_NULLPTR);

private:
	void OpenImage();
	void CreateNewImage();
	void SaveImage();
	void SaveNewImage();
	void CloseActions(bool);

private:
	Ui::PhotoshopLike ui;
	QString _fileName;//当前图片存取路径
};
