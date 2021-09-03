#include "DrawCommand.h"

PaintCommand::PaintCommand(QVector<MyImage::Image*>& target):_target(target) {}

PaintCommand::~PaintCommand() {
	for (int i = 0; i < _changedPixel.count(); ++i) {
		delete _changedPixel[i];
	}
}

void PaintCommand::Execute() {
	for (auto info : _changedPixel) {
		_target[info->Layer]->SetPixel(info->I, info->J, info->After);
	}
}

void PaintCommand::Unexecute() {
	for (auto info : _changedPixel) {
		_target[info->Layer]->SetPixel(info->I, info->J, info->Before);
	}
}

void PaintCommand::SetPixel(int layer, int i, int j, MyImage::RGBQUAD color) {
	PixelInfo* temp = new PixelInfo();
	temp->Layer = layer;
	temp->I = i;
	temp->J = j;
	temp->After = color;
	temp->Before = _target[layer]->GetPixel(i, j);
	_changedPixel.push_back(temp);
	_target[layer]->SetPixel(i, j, color);
}
