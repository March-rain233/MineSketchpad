#pragma once
#include<functional>
#include"Image.h"
enum class OverlayMode {
	Normal,
};

inline MyImage::RGBQUAD NormalOverlay( MyImage::RGBQUAD input, MyImage::RGBQUAD old) {
	MyImage::RGBQUAD res;
	auto blend = [](float a1, float a2, float c1, float c2) {
		return (c1 * a1 * (1.0 - a2) + c2 * a2) / (a1 + a2 - a1 * a2);
	};

	float fAlp1 = input.rgbReserved / 255.0;
	float fAlp2 = old.rgbReserved / 255.0;
	float fAlpBlend = fAlp1 + fAlp2 - fAlp1 * fAlp2;

	float fRed1 = input.rgbRed / 255.0;
	float fRed2 = old.rgbRed / 255.0;
	float fRedBlend = blend(fAlp1, fAlp2, fRed1, fRed2);

	float fGreen1 = input.rgbGreen / 255.0;
	float fGreen2 = old.rgbGreen / 255.0;
	float fGreenBlend = blend(fAlp1, fAlp2, fGreen1, fGreen2);

	float fBlue1 = input.rgbBlue / 255.0;
	float fBlue2 = old.rgbBlue / 255.0;
	float fBlueBlend = blend(fAlp1, fAlp2, fBlue1, fBlue2);

	res.rgbRed = fRedBlend * 255;
	res.rgbGreen = fGreenBlend * 255;
	res.rgbBlue = fBlueBlend * 255;
	res.rgbReserved = fAlpBlend * 255;
	return res;
};