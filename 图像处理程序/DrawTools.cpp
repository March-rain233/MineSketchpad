#include "DrawTools.h"

void DrawTools::Rigister(DrawCanvas* device) {
	_device = device;
}

void DrawTools::Unrigister() {
	_device = nullptr;
}

DrawCanvas& DrawTools::GetDevice() {
	return *_device;
}
