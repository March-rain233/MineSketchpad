#include "ToolFactory.h"
#include "Pencil.h"
#include "MoveHand.h"
#include "MyPaletteModel.h"
#include "OverlayFunction.h"

ToolFactory* ToolFactory::_instance = nullptr;

ToolFactory& ToolFactory::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ToolFactory();
	}
	return *_instance;
}

DrawTools* ToolFactory::GetTool(QString name) {
	if (_pool.contains(name)) {
		return _pool[name];
	}
	else {
		auto res = Create(name);
		if (res != nullptr) {
			_pool.insert(name, res);
		}
		return res;
	}
}

ToolFactory::ToolFactory() {}

DrawTools* ToolFactory::Create(QString name) {
	DrawTools* res = nullptr;
	if (name == "Pencil") {
		Pencil* p = new Pencil();
		p->SetRadius(3);
		p->SetAlpha(255);
		p->GetColor = [] {
			return MyPaletteModel::GetInstance().GetFrontColor();
		};
		p->OverlayMode = NormalOverlay;
		res = p;
	}
	else if (name == "Eraser") {
		Pencil* p = new Pencil();
		p->SetRadius(3);
		p->SetAlpha(255);
		p->GetColor = [] {
			return MyImage::RGBQUAD{ 0,0,0,0 };
		};
		p->OverlayMode = [](const MyImage::RGBQUAD& input, const MyImage::RGBQUAD& old)->MyImage::RGBQUAD {
			auto res = old;
			res.rgbReserved = res.rgbReserved > input.rgbReserved ? res.rgbReserved - input.rgbReserved : 0;
			return res;
		};
		res = p;
	}
	else if (name == "MoveHand") {
		res = new MoveHand();
	}
	return res;
}
