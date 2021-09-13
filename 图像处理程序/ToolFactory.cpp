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
		p->FillPixel = [](LayerModel& im, int x, int y, unsigned char v, PaintCommand* c) {
			MyImage::RGBQUAD color = MyPaletteModel::GetInstance().GetFrontColor();
			color.rgbReserved = v;
			c->SetPixel(x, y, NormalOverlay(color, im.GetBuffer().GetPixel(x, y)));
		};
		//FilterPen* p = new FilterPen();
		//p->SetRadius(3);
		//p->SetAlpha(255);
		//LinearFilter* f = new LinearFilter();
		//double k[9] = {0.0947, 0.1183, 0.0947,
		//			   0.1183, 0.1477, 0.1183,
		//			   0.0947, 0.1183, 0.0947};
		//f->SetKernel(k, 1);
		//p->SetFilter(f);
		res = p;
	}
	else if (name == "Eraser") {
		Pencil* p = new Pencil();
		p->SetRadius(3);
		p->SetAlpha(255);
		p->FillPixel = [](LayerModel& im, int x, int y, unsigned char v, PaintCommand* c) {
			MyImage::RGBQUAD color = im.GetBuffer().GetPixel(x, y);
			color.rgbReserved = color.rgbReserved > v ? color.rgbReserved - v : 0;
			c->SetPixel(x, y, color);
		};
		res = p;
	}
	else if (name == "MoveHand") {
		res = new MoveHand();
	}
	return res;
}
