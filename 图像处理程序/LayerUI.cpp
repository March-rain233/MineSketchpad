#include "LayerUI.h"
#include <qpainter.h>

LayerUI::LayerUI(QWidget *parent)
	: QPushButton(parent)
{
	ui.setupUi(this);
	ui.Image->installEventFilter(this);
	connect(ui.lock, &QAbstractButton::toggled, [this](bool v){
		_model->SetLock(v);
		});
	connect(ui.visible, &QAbstractButton::toggled, [this](bool v) {
		_model->SetVisible(v);
		});
	connect(ui.alpha, &QAbstractSlider::valueChanged, [this](int v) {
		ui.output->setText(QString::number(v));
		_model->SetAlpha(v);
		_model->Update();
		});
	connect(ui.mode, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int v) {
		_model->SetOverlayMode((OverlayMode)v);
		_model->Update();
		});
}

LayerUI::~LayerUI()
{
}

void LayerUI::Rigister(LayerModel* model) {
	_model = model;
	_model->PixelChanged += [this](int) {
		update();
	};
	ui.alpha->setValue(_model->GetAlpha());
	ui.mode->blockSignals(true);
	ui.mode->setCurrentIndex((int)model->GetOverlayMode());
	ui.mode->blockSignals(false);
	update();
}

void LayerUI::Clear() {
	memset(_model->GetImage().GetBits(), 0, _model->GetImage().GetWidth() * _model->GetImage().GetHeight() * 4);
	update();
}

bool LayerUI::eventFilter(QObject* watched, QEvent* event) {
	if (watched == ui.Image && event->type() == QEvent::Paint) {
		QPainter painter;
		painter.begin(ui.Image);
		painter.drawImage(ui.Image->rect(), _model->GetImage().ToQImage());
		painter.end();
	}
	return false;
}
