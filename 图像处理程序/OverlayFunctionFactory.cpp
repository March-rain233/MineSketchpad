#include "OverlayFunctionFactory.h"

OverlayFunctionFactory* OverlayFunctionFactory::_instance = nullptr;
QMap<OverlayMode, OverlayFunctionFactory::OverlayFunction> OverlayFunctionFactory::_dic = {
	{OverlayMode::Normal, NormalOverlay},
	{OverlayMode::Add, AddOverlay},
	{OverlayMode::ColorBurn, ColorBurnOverlay},
	{OverlayMode::ColorDodge, ColorDodgeOverlay},
	{OverlayMode::Darken, DarkenOverlay},
	{OverlayMode::DarkenColor, DarkenColorOverlay},
	{OverlayMode::Difference, DifferenceOverlay},
	{OverlayMode::Divide, DivideOverlay},
	{OverlayMode::Exclusion, ExclusionOverlay},
	{OverlayMode::HardLight, HardLightOverlay},
	{OverlayMode::HardMix, HardMixOverlay},
	{OverlayMode::Lighten, LightenOverlay},
	{OverlayMode::LightenColor, LightenColorOverlay},
	{OverlayMode::LinearBurn, LinearBurnOverlay},
	{OverlayMode::LinearLight, LinearLightOverlay},
	{OverlayMode::Multiply, MultiplyOverlay},
	{OverlayMode::Overlay, OverlayOverlay},
	{OverlayMode::PinLight, PinLightOverlay},
	{OverlayMode::Screen, ScreenOverlay},
	{OverlayMode::SoftLight, SoftLightOverlay},
	{OverlayMode::Subtract, SubtractOverlay},
	{OverlayMode::VividLight, VividLightOverlay},

};

OverlayFunctionFactory& OverlayFunctionFactory::GetInstance() {
	if (_instance == nullptr) {
		_instance = new OverlayFunctionFactory();
	}
	return *_instance;
}

OverlayFunctionFactory::OverlayFunction OverlayFunctionFactory::Create(OverlayMode mode) {
	return _dic[mode];
}
