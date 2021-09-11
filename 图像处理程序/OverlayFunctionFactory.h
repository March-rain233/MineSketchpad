#pragma once
#include "OverlayFunction.h"
class OverlayFunctionFactory {
public:
	typedef std::function<MyImage::RGBQUAD(const MyImage::RGBQUAD&, const MyImage::RGBQUAD&)> OverlayFunction;
	static OverlayFunctionFactory& GetInstance();
	OverlayFunction Create(OverlayMode mode);
private:
	OverlayFunctionFactory() {

	}
private:
	static OverlayFunctionFactory* _instance;
};

