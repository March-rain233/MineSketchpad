#include "PhotoshopLike.h"
#include "Image.h"
#include "CreateImage.h"
#include <qstandardpaths.h>

PhotoshopLike::PhotoshopLike(QWidget* parent)
	: QMainWindow(parent) {
	ui.setupUi(this);
	connect(ui.openImage, &QAction::triggered, this, &PhotoshopLike::OpenImage);
	connect(ui.createImage, &QAction::triggered, this, &PhotoshopLike::CreateNewImage);

	connect(ui.undo, &QAction::triggered, [this] {
		ui.widget->Undo();
		});
	connect(ui.redo, &QAction::triggered, [this] {
		ui.widget->Redo();
		});

}

void PhotoshopLike::OpenImage() {
	QString OpenFile, OpenFilePath;
	OpenFile = QFileDialog::getOpenFileName(
		this,
		"选择图片",
		QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
		"Image Files(*.bmp)");
	if (OpenFile.isEmpty()) {
		return;
	}
	ui.widget->ClearCanvas();
	//Todo:这里会内存泄露
	MyImage::Image* m = MyImage::Image::ReadImage(OpenFile.toLatin1().data());
	ui.widget->AddLayer(*m);
	delete m;
}

void PhotoshopLike::CreateNewImage() {
	CreateImage dialog;
	if (dialog.exec() == QDialog::Accepted) {
		_fileName = dialog.GetFilename();
		QColor color = dialog.GetColor();
		MyImage::RGBQUAD rgb
		{ color.blue(), color.green(), color.red(), color.alpha() };
		MyImage::BitMap_32 t(dialog.GetHeight(), dialog.GetWidth(), rgb);
		ui.widget->AddLayer(t);
	}
}

void PhotoshopLike::SaveImage() {
	if (ui.widget->IsEmpty()) {
		return;
	}
	ui.widget->SaveImage(_fileName);
}

void PhotoshopLike::SaveNewImage() {
	if (ui.widget->IsEmpty()) {
		return;
	}
	QString filename = QFileDialog::getOpenFileName(
		this,
		"选择储存位置",
		QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
		"Image Files(*.bmp)");
	ui.widget->SaveImage(filename);
}
