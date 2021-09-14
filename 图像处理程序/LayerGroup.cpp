#include "LayerGroup.h"
#include <qmessagebox.h>
#include <qstandardpaths.h>
#include <QTextCodec>
#include <qfiledialog.h>

LayerGroup::LayerGroup(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	CloseActions(false);
	connect(ui.NewLayer, &QPushButton::clicked, [this] {
		int num = _device->GetSelected()[_device->GetSelected().size() - 1];
		LayerModel* layer = new LayerModel(new MyImage::BitMap_32
		(_device->GetImageHeight(), _device->GetImageWidth(), MyImage::RGBQUAD{ 255,255,255,0 }));
		InsertLayer(layer, num + 1);
		});
	connect(ui.DeleteLayer, &QPushButton::clicked, [this] {
		for (int i = _device->GetSelected().size() - 1; i >= 0; --i) {
			DeleteLayer(_device->GetSelected()[i]);
		}
		});
	connect(ui.ClearLayer, &QPushButton::clicked, [this] {
		for (int i = _device->GetSelected().size() - 1; i >= 0; --i) {
			ClearLayer(_device->GetSelected()[i]);
		}
		});
	connect(ui.openFile, &QPushButton::clicked, this, &LayerGroup::OpenFile);
}

LayerGroup::~LayerGroup()
{
}

void LayerGroup::Rigister(DrawCanvas* device) {
	_device = device;
	connect(_device, &DrawCanvas::EmptyChanged, this, &LayerGroup::CloseActions);
}

void LayerGroup::AddLayer(LayerModel* layer) {
	_device->AddLayer(layer);

	LayerUI* newLayer = new LayerUI(this);
	newLayer->Rigister(layer);
	connect(newLayer, &QPushButton::toggled, [this, newLayer](bool v) {
			int index = 0;
			for (int i = _layerUIs.size() - 1; i >= 0; --i) {
				if (newLayer == _layerUIs[i]) {
					index = i;
					break;
				}
			}
			if (v) {
				AddCheck(index);
				//UnCheckOther(index);
			}
			else if(_device->GetSelected().size() == 1) {
				newLayer->blockSignals(true);
				newLayer->setChecked(true);
				newLayer->blockSignals(false);
			}
			else {
				QVector<int>& select = _device->GetSelected();
				for (int i = select.size() - 1; i >= 0; --i) {
					if (select[i] == index) {
						select.remove(i);
					}
				}
			}
		});
	_layerUIs.push_back(newLayer);
	ui.layerLayout->addWidget(newLayer);
	newLayer->setCheckable(true);
	newLayer->setChecked(true);
	//UnCheckOther(_layerUIs.size() - 1);
	newLayer->show();
}

void LayerGroup::InsertLayer(LayerModel* layer, int index) {
	_device->InsertLayer(layer, index);
	LayerUI* newLayer = new LayerUI(this);
	newLayer->Rigister(layer);
	connect(newLayer, &QPushButton::toggled, [this, newLayer](bool v) {
		int index = 0;
		for (int i = _layerUIs.size() - 1; i >= 0; --i) {
			if (newLayer == _layerUIs[i]) {
				index = i;
				break;
			}
		}
		if (v) {
			AddCheck(index);
			//UnCheckOther(index);
		}
		else if (_device->GetSelected().size() == 1) {
			newLayer->blockSignals(true);
			newLayer->setChecked(true);
			newLayer->blockSignals(false);
		}
		else {
			QVector<int>& select = _device->GetSelected();
			for (int i = select.size() - 1; i >= 0; --i) {
				if (select[i] == index) {
					select.remove(i);
				}
			}
		}
		});
	_layerUIs.insert(index, newLayer);
	ui.layerLayout->insertWidget(index, newLayer);
	newLayer->setCheckable(true);
	newLayer->setChecked(true);
	//UnCheckOther(index);
	newLayer->show();
}

void LayerGroup::DeleteLayer(int i) {
	if (_layerUIs.size() == 1) {
		QMessageBox::warning(NULL, "warning", QStringLiteral("无法删除当前图层"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	delete _layerUIs[i];
	_layerUIs.remove(i);
	_device->DeleteLayer(i);
	if (i >= _layerUIs.size()) {
		i = _layerUIs.size() - 1;
	}
	_device->GetSelected().clear();
	_layerUIs[i]->setChecked(true);
}

void LayerGroup::ClearLayer(int i) {
	_layerUIs[i]->Clear();
	_device->ReDraw();
	_device->update();
}

void LayerGroup::ClearAllUI() {
	_device->ClearCanvas();
	for (int i = 0; i < _layerUIs.size(); ++i) {
		delete _layerUIs[i];
	}
	_layerUIs.clear();
}

void LayerGroup::OpenFile() {
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
	MyImage::Image* m = MyImage::Image::ReadImage(name.c_str());
	int h = _device->GetImageHeight();
	int w = _device->GetImageWidth();
	int ch = m->GetHeight();
	int cw = m->GetWidth();
	MyImage::Image* m2 = new MyImage::BitMap_32(h, w);
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			if (i < ch && j < cw) {
				m2->SetPixel(j, i, m->GetPixel(j, i));
			}
			else {
				m2->SetPixel(j, i, MyImage::RGBQUAD{ 255,255,255,0 });
			}
		}
	}
	AddLayer(new LayerModel(m2));
}

void LayerGroup::CloseActions(bool v) {
	ui.ClearLayer->setEnabled(v);
	ui.DeleteLayer->setEnabled(v);
	ui.NewLayer->setEnabled(v);
	ui.openFile->setEnabled(v);
}

void LayerGroup::UnCheckOther(int index) {
	for (int i = 0; i < _layerUIs.size(); ++i) {
		if (i != index) {
			_layerUIs[i]->blockSignals(true);
			_layerUIs[i]->setChecked(false);
			_layerUIs[i]->blockSignals(false);
		}
	}
	QVector<int>& t = _device->GetSelected();
	for (int i = t.size() - 1; i >= 0; --i) {
		if (index != t[i]) {
			t.remove(i);
		}
	}
}

void LayerGroup::AddCheck(int i) {
	_device->GetSelected().push_back(i);
	qSort(_device->GetSelected());
}
