#include "PhotoshopLike.h"
#include "Image.h"
#include "Pencil.h"
#include "CreateImage.h"
#include <qstandardpaths.h>
#include <QTextCodec>

PhotoshopLike::PhotoshopLike(QWidget* parent)
	: QMainWindow(parent) {
	ui.setupUi(this);
	//setWindowFlags(Qt::FramelessWindowHint);

	connect(ui.openImage, &QAction::triggered, this, &PhotoshopLike::OpenImage);
	connect(ui.createImage, &QAction::triggered, this, &PhotoshopLike::CreateNewImage);

	connect(ui.undo, &QAction::triggered, [this] {
		ui.widget->Undo();
		});
	connect(ui.redo, &QAction::triggered, [this] {
		ui.widget->Redo();
		});

	connect(ui.save, &QAction::triggered, this, &PhotoshopLike::SaveImage);
	connect(ui.saveAs, &QAction::triggered, this, &PhotoshopLike::SaveNewImage);
	ui.widget->SetTool(new Pencil());
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
	QTextCodec* code = QTextCodec::codecForName("GB2312");//解决中文路径问题
	std::string name = code->fromUnicode(OpenFile).data();
	_fileName = QString(name.c_str());
	ui.widget->ClearCanvas();
	//Todo:这里会内存泄露
	MyImage::Image* m = MyImage::Image::ReadImage(name.c_str());
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
	ui.widget->SaveImage(_fileName + ".bmp");
}

void PhotoshopLike::SaveNewImage() {
	if (ui.widget->IsEmpty()) {
		return;
	}
	QString filename = QFileDialog::getSaveFileName(
		this,
		"选择储存位置",
		QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
		"Image Files(*.bmp)");
	QTextCodec* code = QTextCodec::codecForName("GB2312");
	std::string name = code->fromUnicode(filename).data();
	ui.widget->SaveImage(name.c_str());
}
