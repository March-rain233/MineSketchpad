#include "DrawCommand.h"

PaintCommand::PaintCommand(const QVector<LayerModel*>& target):_target(target) {}

PaintCommand::~PaintCommand() {
	for (int i = 0; i < _changedPixel.count(); ++i) {
		delete _changedPixel[i];
	}
}

void PaintCommand::Execute() {
	for (auto info : _changedPixel) {
		_target[info->Layer]->GetImage().SetPixel(info->I, info->J, info->After);
	}
}

void PaintCommand::Unexecute() {
	for (auto info : _changedPixel) {
		_target[info->Layer]->GetImage().SetPixel(info->I, info->J, info->Before);
	}
}

void PaintCommand::SetPixel(int layer, int i, int j, MyImage::RGBQUAD color) {
	if (i < 0 || i >= _target[layer]->GetImage().GetWidth()) {
		return;
	}
	if (j < 0 || j >= _target[layer]->GetImage().GetHeight()) {
		return;
	}

	MyImage::RGBQUAD t = _target[layer]->GetImage().GetPixel(i, j);
	if (memcmp(&color, &t, 4) == 0)
	{
		return;
	}

	PixelInfo* temp = new PixelInfo();
	temp->Layer = layer;
	temp->I = i;
	temp->J = j;
	temp->After = color;
	temp->Before = t;
	_changedPixel.push_back(temp);
	_target[layer]->GetImage().SetPixel(i, j, color);
}
