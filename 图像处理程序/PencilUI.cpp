#include "PencilUI.h"
#include "Pencil.h"

PencilUI::PencilUI(QWidget* parent)
	: DrawToolsUI(parent) {
	ui.setupUi(this);
}

PencilUI::~PencilUI() {}

void PencilUI::Init() {
	Pencil& pencil = (Pencil&)GetTool();
	connect(ui.sizeSlider, &QAbstractSlider::valueChanged, [this, &pencil](int v) {
		pencil.SetRadius(v);
		});
	connect(ui.alphaSlider, &QAbstractSlider::valueChanged, [this, &pencil](int v) {
		pencil.SetAlpha(v);
		});
	connect(ui.sizeInput, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		[this, &pencil](int v) {
			pencil.SetRadius(v);
		});
	connect(ui.alphaInput, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		[this, &pencil](int v) {
			pencil.SetAlpha(v);
		});

	pencil.RadiusChanged += [this](int v) {
		if (ui.sizeInput->value() != v) {
			ui.sizeInput->blockSignals(true);
			ui.sizeInput->setValue(v);
			ui.sizeInput->blockSignals(false);
		}
		if (ui.sizeSlider->value() != v) {
			ui.sizeSlider->blockSignals(true);
			ui.sizeSlider->setValue(v);
			ui.sizeSlider->blockSignals(false);
		}
	};
	pencil.AlphaChanged += [this](int v) {
		if (ui.alphaInput->value() != v) {
			ui.alphaInput->blockSignals(true);
			ui.alphaInput->setValue(v);
			ui.alphaInput->blockSignals(false);
		}
		if (ui.alphaSlider->value() != v) {
			ui.alphaSlider->blockSignals(true);
			ui.alphaSlider->setValue(v);
			ui.alphaSlider->blockSignals(false);
		}
	};
	pencil.UpdateAll();
}
