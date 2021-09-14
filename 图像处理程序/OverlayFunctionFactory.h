#pragma once
#include "OverlayFunction.h"
#include <qmap.h>
class OverlayFunctionFactory {
public:
	typedef std::function<MyImage::RGBQUAD(const MyImage::RGBQUAD&, const MyImage::RGBQUAD&, unsigned char)> OverlayFunction;
	static OverlayFunctionFactory& GetInstance();
	OverlayFunction Create(OverlayMode mode);
private:
	OverlayFunctionFactory() {

	}
private:
	static OverlayFunctionFactory* _instance;
	static QMap<OverlayMode, OverlayFunction> _dic;
};

