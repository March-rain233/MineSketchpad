#include "LayerGroup.h"

LayerGroup::LayerGroup(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
}

LayerGroup::~LayerGroup()
{
}

void LayerGroup::Rigister(DrawCanvas* device) {
	_device = device;
}

void LayerGroup::AddLayer(LayerModel* layer) {
	_device->AddLayer(layer);
	LayerUI* newLayer = new LayerUI(this);
	newLayer->Rigister(layer);
	_layerUIs.push_back(newLayer);
	ui.layerLayout->addWidget(newLayer);
	newLayer->show();
}

void LayerGroup::InsertLayer(LayerModel* layer, int index) {
	_device->InsertLayer(layer, index);
	LayerUI* newLayer = new LayerUI(this);
	newLayer->Rigister(layer);
	_layerUIs.insert(index, newLayer);
	ui.layerLayout->insertWidget(index, newLayer);
	newLayer->show();
}

void LayerGroup::DeleteLayer(int i) {
	LayerUI* temp = _layerUIs[i];
	_layerUIs.remove(i);
	_device->DeleteLayer(i);
	delete temp;
}

void LayerGroup::ClearLayer(int i) {
	_layerUIs[i]->Clear();
}

void LayerGroup::ClearAllUI() {
	_device->ClearCanvas();
	for (int i = 0; i < _layerUIs.size(); ++i) {
		delete _layerUIs[i];
	}
	_layerUIs.clear();
}
