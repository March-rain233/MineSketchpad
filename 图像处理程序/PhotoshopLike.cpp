#include "PhotoshopLike.h"
#include "Image.h"
#include "ToolFactory.h"
#include "Pencil.h"
#include "CreateImage.h"
#include <qstandardpaths.h>
#include <QTextCodec>
#include "LayerModel.h"
#include "DrawCommand.h"
#include "ChangeHSL.h"
#include "ChangeContrast.h"
#include "ChangeSize.h"
#include "Binaryzation.h"

PhotoshopLike::PhotoshopLike(QWidget* parent)
	: QMainWindow(parent) {
	ui.setupUi(this);
	//setWindowFlags(Qt::FramelessWindowHint);
	CloseActions(false);
	connect(ui.widget, &DrawCanvas::EmptyChanged, this, &PhotoshopLike::CloseActions);
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
		ChangeSize size(ui.widget, this);
		if (size.exec() == QDialog::Accepted) {
			FunctionCommand* com = new FunctionCommand();
			int h = size.GetHeight();
			int w = size.GetWidth();
			int oh = ui.widget->GetImageHeight();
			int ow = ui.widget->GetImageWidth();
			com->Redo = [h, w, this] {ui.widget->Resize(h, w); };
			com->Undo = [oh, ow, this] {ui.widget->Resize(oh, ow); };
			ui.widget->Resize(h, w);
		}
		});

	connect(ui.HSL, &QAction::triggered, [this] {
		ChangeHSL t(ui.widget, this);
		t.exec();
		});
	connect(ui.cb, &QAction::triggered, [this] {
		ChangeContrast t(ui.widget, this);
		t.exec();
		});
	connect(ui.gray, &QAction::triggered, [this] {
		auto layer = ui.widget->GetLayers();
		auto select = ui.widget->GetSelected();
		GroupCommand* group = new GroupCommand();
		for (int i = 0; i < select.size(); ++i) {
			CopyCommand* copy = new CopyCommand();
			copy->Target = layer[select[i]];
			copy->Last = layer[select[i]]->GetImage().Clone();
			copy->After = layer[select[i]]->GetImage().ToGrayScale().Clone();
			layer[select[i]]->SetImage(copy->After);
			group->PushBackCommand(copy);
		}
		ui.widget->PushCommand(group);
		ui.widget->ReDraw();
		ui.widget->update();
		});
	connect(ui.inverse, &QAction::triggered, [this] {
		auto layer = ui.widget->GetLayers();
		auto select = ui.widget->GetSelected();
		GroupCommand* group = new GroupCommand();
		for (int i = 0; i < select.size(); ++i) {
			CopyCommand* copy = new CopyCommand();
			copy->Target = layer[select[i]];
			copy->Last = layer[select[i]]->GetImage().Clone();
			copy->After = layer[select[i]]->GetImage().Clone();
			for (int i = 0; i < copy->After->GetWidth(); ++i) {
				for (int j = 0; j < copy->After->GetHeight(); ++j) {
					auto t = copy->After->GetPixel(i, j);
					t.rgbBlue = 255 - t.rgbBlue;
					t.rgbRed = 255 - t.rgbRed;
					t.rgbGreen = 255 - t.rgbGreen;
					copy->After->SetPixel(i, j, t);
				}
			}
			layer[select[i]]->SetImage(copy->After);
			group->PushBackCommand(copy);
		}
		ui.widget->PushCommand(group);
		ui.widget->ReDraw();
		ui.widget->update();
		});
	connect(ui.bianry, &QAction::triggered, [this] {
		Binaryzation bi(ui.widget, this);
		bi.exec();
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
	connect(ui.flipHorizontal, &QAction::triggered, [this] {
		QVector<LayerModel*> layers = ui.widget->GetLayers();
		GroupCommand* group = new GroupCommand();
		for (int i = 0; i < layers.size(); ++i) {
			layers[i]->GetImage().Flip(false);
			FunctionCommand* func = new FunctionCommand();
			func->Redo = [layers, i]() {
				layers[i]->GetImage().Flip(false);
			};
			func->Undo = [layers, i]() {
				layers[i]->GetImage().Flip(false);
			};
			group->PushBackCommand(func);
		}
		ui.widget->PushCommand(group);
		ui.widget->ReDraw();
		ui.widget->update();
		});
	connect(ui.flipVertical, &QAction::triggered, [this] {
		QVector<LayerModel*> layers = ui.widget->GetLayers();
		GroupCommand* group = new GroupCommand();
		for (int i = 0; i < layers.size(); ++i) {
			layers[i]->GetImage().Flip(true);
			FunctionCommand* func = new FunctionCommand();
			func->Redo = [layers, i]() {
				layers[i]->GetImage().Flip(true);
			};
			func->Undo = [layers, i]() {
				layers[i]->GetImage().Flip(true);
			};
			group->PushBackCommand(func);
		}
		ui.widget->PushCommand(group);
		ui.widget->ReDraw();
		ui.widget->update();
		});

	connect(ui.exit, &QAction::triggered, [] {
		QApplication::quit();
		});

	ui.redo->setEnabled(false);
	ui.undo->setEnabled(false);
	connect(ui.widget, &DrawCanvas::RedoNumChanged, [this] (int i){
		if (i > 0) {
			ui.redo->setEnabled(true);
		}
		else {
			ui.redo->setEnabled(false);
		}
		});
	connect(ui.widget, &DrawCanvas::UndoNumChanged, [this](int i) {
		if (i > 0) {
			ui.undo->setEnabled(true);
		}
		else {
			ui.undo->setEnabled(false);
		}
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
	ui.widget->SetBackground(MyImage::BitMap_32(m->GetHeight(), m->GetWidth(), MyImage::RGBQUAD{ 255,255,255,0 }));
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
		(dialog.GetHeight(), dialog.GetWidth(), MyImage::RGBQUAD{255,255,255,0})));
	}
}

void PhotoshopLike::SaveImage() {
	if (ui.widget->IsEmpty()) {
		return;
	}
	QString fileName = _fileName.contains(".bmp") ? _fileName : _fileName + ".bmp";
	ui.widget->SaveImage(fileName);
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

void PhotoshopLike::CloseActions(bool v) {
	ui.bianry->setEnabled(v);
	ui.save->setEnabled(v);
	ui.saveAs->setEnabled(v);
	ui.menu_I->setEnabled(v);
	ui.menu_E->setEnabled(v);
}
