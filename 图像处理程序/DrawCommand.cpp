#include "DrawCommand.h"

PaintCommand::PaintCommand(LayerModel& target):_target(target) {}

PaintCommand::~PaintCommand() {
	for (int i = 0; i < _changedPixel.size(); ++i) {
		delete _changedPixel[i];
	}
}

void PaintCommand::Execute() {
	for (auto info : _changedPixel) {
		_target.GetImage().SetPixel(info->I, info->J, info->After);
	}
}

void PaintCommand::Unexecute() {
	for (auto info : _changedPixel) {
		_target.GetImage().SetPixel(info->I, info->J, info->Before);
	}
}

void PaintCommand::SetPixel(int i, int j, const MyImage::RGBQUAD& color) {
	if (i < 0 || i >= _target.GetBuffer().GetWidth()) {
		return;
	}
	if (j < 0 || j >= _target.GetBuffer().GetHeight()) {
		return;
	}

	MyImage::RGBQUAD t = _target.GetBuffer().GetPixel(i, j);
	if (memcmp(&color, &t, 4) == 0)
	{
		return;
	}

	PixelInfo* temp = new PixelInfo();
	temp->I = i;
	temp->J = j;
	temp->After = color;
	temp->Before = t;
	_changedPixel.push_back(temp);
	_target.GetBuffer().SetPixel(i, j, color);
}

void FunctionCommand::Execute() {
	Redo();
}

void FunctionCommand::Unexecute() {
	Undo();
}

GroupCommand::~GroupCommand() {
	for (auto c : _commands) {
		delete c;
	}
}

void GroupCommand::Execute() {
	for (int i = 0; i < _commands.size() - 1; ++i) {
		_commands[i]->Execute();
	}
}

void GroupCommand::Unexecute() {
	for (int i = _commands.size() - 1; i>=0; --i) {
		_commands[i]->Unexecute();
	}
}

void GroupCommand::PushBackCommand(DrawCommand* c) {
	_commands.push_back(c);
}

CopyCommand::~CopyCommand() {
	if (Last) {
		delete Last;
	}
	if (After) {
		delete After;
	}
}

void CopyCommand::Execute() {
	Target->SetImage(After);
}

void CopyCommand::Unexecute() {
	Target->SetImage(Last);
}
