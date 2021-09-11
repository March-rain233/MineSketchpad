#pragma once
#include "OverlayFunction.h"
class OverlayFunctionFactory {
public:
	typedef std::function<MyImage::RGBQUAD(MyImage::RGBQUAD, MyImage::RGBQUAD)> OverlayFunction;
	static OverlayFunctionFactory& GetInstance();
	OverlayFunction Create(OverlayMode mode);
private:
	OverlayFunctionFactory() {

	}
private:
	static OverlayFunctionFactory* _instance;
};

