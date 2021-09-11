#include "LayerUI.h"
#include <qpainter.h>

LayerUI::LayerUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.Image->installEventFilter(this);
	connect(ui.lock, &QAbstractButton::toggled, [this](bool v){
		_model->SetLock(v);
		});
	connect(ui.visible, &QAbstractButton::toggled, [this](bool v) {
		_model->SetVisible(v);
		});
}

LayerUI::~LayerUI()
{
}

void LayerUI::Rigister(LayerModel* model) {
	_model = model;
	_model->GetImage().PixelChanged += [this](int) {
		update();
	};
	update();
}

void LayerUI::Clear() {
	memset(_model->GetImage().GetBits(), 0X00FFFFFF, _model->GetImage().GetWidth() * _model->GetImage().GetHeight());
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
