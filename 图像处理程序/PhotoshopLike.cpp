#include "PhotoshopLike.h"
#include "Image.h"
#include "ToolFactory.h"
#include "Pencil.h"
#include "CreateImage.h"
#include <qstandardpaths.h>
#include <QTextCodec>
#include "LayerModel.h"
#include "DrawCommand.h"

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

	connect(ui.imageSize, &QAction::triggered, [this] {

		});

	connect(ui.rotate90, &QAction::triggered, [this] {
		QVector<LayerModel*> layers = ui.widget->GetLayers();
		GroupCommand* group = new GroupCommand();
		for(int i =0; i<layers.size(); ++i){
			layers[i]->GetImage().Rotate(90);
			FunctionCommand* func = new FunctionCommand();
			func->Redo = [layers, i]() {
				layers[i]->GetImage().Rotate(90);
			};
			func->Undo = [layers, i]() {
				layers[i]->GetImage().Rotate(270);
			};
			group->PushBackCommand(func);
		}
		ui.widget->PushCommand(group);
		ui.widget->ReDraw();
		ui.widget->update();
		});
	connect(ui.rotate180, &QAction::triggered, [this] {
		auto layers = ui.widget->GetLayers();
		GroupCommand* group = new GroupCommand();
		for (int i = 0; i < layers.size(); ++i) {
			layers[i]->GetImage().Rotate(180);
			FunctionCommand* func = new FunctionCommand();
			func->Redo = [layers, i]() {
				layers[i]->GetImage().Rotate(180);
			};
			func->Undo = func->Redo;
			group->PushBackCommand(func);
		}
		ui.widget->PushCommand(group);
		ui.widget->ReDraw();
		ui.widget->update();
		});
	connect(ui.rotate270, &QAction::triggered, [this] {
		auto layers = ui.widget->GetLayers();
		GroupCommand* group = new GroupCommand();
		for (int i = 0; i < layers.size(); ++i) {
			layers[i]->GetImage().Rotate(270);
			FunctionCommand* func = new FunctionCommand();
			func->Redo = [layers, i]() {
				layers[i]->GetImage().Rotate(270);
			};
			func->Undo = [layers, i]() {
				layers[i]->GetImage().Rotate(90);
			};
			group->PushBackCommand(func);
		}
		ui.widget->PushCommand(group);
		ui.widget->ReDraw();
		ui.widget->update();
		});
	ui.Tool->Rigister(ui.widget);
	ui.layerGroup->Rigister(ui.widget);
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
	ui.layerGroup->ClearAllUI();
	//Todo:这里会内存泄露
	MyImage::Image* m = MyImage::Image::ReadImage(name.c_str());
	ui.widget->SetBackground(MyImage::BitMap_32(m->GetHeight(), m->GetWidth(), MyImage::RGBQUAD{ 0,0,0,0 }));
	ui.layerGroup->AddLayer(new LayerModel(m));
}

void PhotoshopLike::CreateNewImage() {
	CreateImage dialog;
	if (dialog.exec() == QDialog::Accepted) {
		ui.layerGroup->ClearAllUI();
		_fileName = dialog.GetFilename();
		QColor color = dialog.GetColor();
		MyImage::RGBQUAD rgb
		{ color.blue(), color.green(), color.red(), color.alpha() };
		MyImage::BitMap_32 t(dialog.GetHeight(), dialog.GetWidth(), rgb);
		ui.widget->SetBackground(t);
		ui.layerGroup->AddLayer(new LayerModel(new MyImage::BitMap_32
		(dialog.GetHeight(), dialog.GetWidth(), MyImage::RGBQUAD{0,0,0,0})));
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
