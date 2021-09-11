#include "OverlayFunctionFactory.h"

OverlayFunctionFactory* OverlayFunctionFactory::_instance = nullptr;

OverlayFunctionFactory& OverlayFunctionFactory::GetInstance() {
	if (_instance == nullptr) {
		_instance = new OverlayFunctionFactory();
	}
	return *_instance;
}

OverlayFunctionFactory::OverlayFunction OverlayFunctionFactory::Create(OverlayMode mode) {
	switch (mode) {
	case OverlayMode::Normal:
		return NormalOverlay;
	default:
		return OverlayFunction();
	}
}
