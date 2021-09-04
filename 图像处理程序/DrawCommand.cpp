#include "DrawCommand.h"

PaintCommand::PaintCommand(const QVector<MyImage::Image*>& target):_target(target) {}

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
	if (i < 0 || i >= _target[layer]->GetWidth()) {
		return;
	}
	if (j < 0 || j >= _target[layer]->GetHeight()) {
		return;
	}

	MyImage::RGBQUAD t = _target[layer]->GetPixel(i, j);
	if (t.rgbBlue == color.rgbBlue && t.rgbGreen == color.rgbGreen &&
		t.rgbRed == color.rgbRed && t.rgbReserved == color.rgbReserved)
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
	_target[layer]->SetPixel(i, j, color);
}
