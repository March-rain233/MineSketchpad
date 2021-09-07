#include "DrawCanvas.h"

DrawCanvas::DrawCanvas(QWidget* parent)
	: QWidget(parent), _drawPoint(0, 0) {
	_ui.setupUi(this);
	_ui.Image->installEventFilter(this);
	_canvasFill = new QImage(50, 50, QImage::Format_RGBA8888);
	_tool = nullptr;
	_ui.horizontalScrollBar->setRange(0, _ui.Image->width());
	_ui.verticalScrollBar->setRange(0, _ui.Image->height());
	connect(_ui.horizontalScrollBar, &QAbstractSlider::sliderMoved, this, &DrawCanvas::HorizontalMove);
	connect(_ui.verticalScrollBar, &QAbstractSlider::sliderMoved, this, &DrawCanvas::VertiaclMove);
}

DrawCanvas::~DrawCanvas() {
	
}

void DrawCanvas::Undo() {
	if (_historyCommand.isEmpty()) {
		return;
	}
	DrawCommand* cur = _historyCommand.pop();
	cur->Unexecute();
	_redoCommand.push(cur);
	update();
}

void DrawCanvas::Redo() {
	if (_redoCommand.isEmpty()) {
		return;
	}
	DrawCommand* cur = _redoCommand.pop();
	cur->Execute();
	_historyCommand.push(cur);
	update();
}

void DrawCanvas::ClearCommand() {
	for (int i = 0; i < _historyCommand.count(); ++i) {
		delete _historyCommand[i];
	}
	_historyCommand.clear();
	ClearRedoCommand();
}

void DrawCanvas::ClearRedoCommand() {
	for (int i = 0; i < _redoCommand.count(); ++i) {
		delete _redoCommand[i];
	}
	_redoCommand.clear();
}

void DrawCanvas::AddLayer(const MyImage::Image& im) {
	if (_layers.size() == 0) {
		int picMax = im.GetWidth() > im.GetHeight() ? im.GetWidth() : im.GetHeight();
		int labMac = _ui.Image->height() > _ui.Image->width() ? _ui.Image->height() : _ui.Image->width();
		_scale = labMac * 100.0f / picMax;
	}
	_layers.push_back(im.Clone());
	_seletcted.clear();
	_seletcted.push_back(_layers.size() - 1);
	update();
}

void DrawCanvas::InsertLayer(const MyImage::Image& im, int i) {
	if (i == 0) {
		AddLayer(im);
		return;
	}
	_layers.insert(i, im.Clone());
	_seletcted.clear();
	_seletcted.push_back(i);
	update();
}

void DrawCanvas::DeleteLayer(int i) {
	delete _layers[i];
	_layers.remove(i);
	update();
}

const QVector<MyImage::Image*>& DrawCanvas::GetLayers() {
	return _layers;
}

QVector<int>& DrawCanvas::GetSelected() {
	return _seletcted;
}

void DrawCanvas::SetScale(float v) {
	if (v > _maxSizeRate) {
		v = _maxSizeRate;
	}
	else if(v < _minSizeRate) {
		v = _minSizeRate;
	}
	_scale = v;
	update();
}

float DrawCanvas::GetScale() {
	return _scale;
}

QPoint DrawCanvas::GetDrawPoint() {
	return _drawPoint;
}

void DrawCanvas::SaveImage(const QString& fileName) {
	int h = _layers[0]->GetHeight();
	int w = _layers[0]->GetWidth();
	MyImage::BitMap_32 res(h, w);
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			for (int k = 0; k < _layers.size(); ++k) {
				res.SetPixel(i, j, _layers[k]->GetPixel(i, j));
			}
		}
	}
	res.WriteImage(fileName.toLatin1().data());
}

void DrawCanvas::PushCommand(DrawCommand* c) {
	ClearRedoCommand();
	_historyCommand.push(c);
}

void DrawCanvas::SetTool(DrawTools* t) {
	if (_tool != nullptr) {
		_tool->Unrigister();
	}
	_tool = t;
	_tool->Rigister(this);
}

void DrawCanvas::ClearCanvas() {
	ClearCommand();
	for (int i = 0; i < _layers.size(); ++i) {
		delete _layers[i];
	}
	_layers.clear();
	update();
}

bool DrawCanvas::IsEmpty() {
	return _layers.isEmpty();
}

void DrawCanvas::mousePressEvent(QMouseEvent* e) {
	if (IsEmpty() || _tool == nullptr) {
		return;
	}
	if (!_tool->mousePressEvent(e)) {

	}
	update();
}

void DrawCanvas::mouseReleaseEvent(QMouseEvent* e) {
	if (IsEmpty() || _tool == nullptr) {
		return;
	}
	if (!_tool->mouseReleaseEvent(e)) {

	}
	update();
}

void DrawCanvas::mouseMoveEvent(QMouseEvent* e) {
	if (IsEmpty() || _tool == nullptr) {
		return;
	}
	if (!_tool->mouseMoveEvent(e)) {

	}
	update();
}

void DrawCanvas::contextMenuEvent(QContextMenuEvent*e) {
	QWidget::contextMenuEvent(e);
}

void DrawCanvas::keyPressEvent(QKeyEvent* e) {
	QWidget::keyPressEvent(e);
}

void DrawCanvas::resizeEvent(QResizeEvent* e) {
	_ui.horizontalScrollBar->setRange(0, e->size().width());
	_ui.verticalScrollBar->setRange(0, e->size().height());
	QWidget::resizeEvent(e);
}

bool DrawCanvas::eventFilter(QObject* watched, QEvent* event) {
	if (watched == _ui.Image && event->type() == QEvent::Paint) {
		DrawImage();
	}
	return QWidget::eventFilter(watched, event);
}

void DrawCanvas::DrawImage() {
	QPainter painter;
	painter.begin(_ui.Image);
	//painter.drawImage(_ui.Image->rect(), *_canvasFill);
	if (!_layers.isEmpty()) {
		for (int i = 0; i < _layers.count(); ++i) {
			painter.drawImage(GetDrawRect(), _layers[i]->ToQImage());
		}
	}
	painter.end();
}

void DrawCanvas::HorizontalMove(int v) {
	if (_layers.isEmpty()) {
		return;
	}
	_drawPoint.setX(_ui.Image->width() - v - GetDrawWidth() / 2);
	update();
}

void DrawCanvas::VertiaclMove(int v) {
	if (_layers.isEmpty()) {
		return;
	}
	_drawPoint.setY(_ui.Image->height() - v - GetDrawHeight() / 2);
	update();
}

int DrawCanvas::GetDrawHeight() {
	return _layers[0]->GetHeight() * _scale * 0.01;
}

int DrawCanvas::GetDrawWidth() {
	return _layers[0]->GetWidth()* _scale * 0.01;
}

QRect DrawCanvas::GetDrawRect() {
	QRect res;
	res.setTopLeft(_drawPoint);
	res.setHeight(_layers[0]->GetHeight() * _scale * 0.01);
	res.setWidth(_layers[0]->GetWidth() * _scale * 0.01);
	return res;
}
