#include "FilterPenUI.h"
#include "Pencil.h"

FilterPenUI::FilterPenUI(QWidget *parent)
	: DrawToolsUI(parent)
{
	ui.setupUi(this);
}

FilterPenUI::~FilterPenUI()
{
}

void FilterPenUI::Init() {
	FilterPen& pencil = (FilterPen&)GetTool();
	ui.widget->SetFilter(pencil.GetFilter());
	ui.sizeInput->setValue(1);

	connect(ui.sizeSlider, &QAbstractSlider::valueChanged, [this, &pencil](int v) {
		pencil.SetRadius(v);
		});
	connect(ui.sizeInput, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		[this, &pencil](int v) {
			pencil.SetRadius(v);
		});
	ui.widget->FilterChanged += [&pencil](ImageFilter* f) {
		pencil.SetFilter(f);
	};

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
	pencil.UpdateAll();
}
