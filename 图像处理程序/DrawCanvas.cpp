#include "DrawCanvas.h"
#include <qdebug.h>
#include <qmessagebox.h>

DrawCanvas::DrawCanvas(QWidget* parent)
	: QWidget(parent), _drawPoint(0, 0) {
	_ui.setupUi(this);
	_ui.Image->installEventFilter(this);
	_canvas = nullptr;
	_tool = nullptr;
	_canvasFill = nullptr;
	_ui.horizontalScrollBar->setRange(0, 0);
	_ui.verticalScrollBar->setRange(0, 0);
	connect(_ui.horizontalScrollBar, &QAbstractSlider::valueChanged, this, &DrawCanvas::HorizontalMove);
	connect(_ui.verticalScrollBar, &QAbstractSlider::valueChanged, this, &DrawCanvas::VertiaclMove);

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
	UndoNumChanged(_historyCommand.size());
	RedoNumChanged(_redoCommand.size());
	ReDraw();
	update();
}

void DrawCanvas::Redo() {
	if (_redoCommand.isEmpty()) {
		return;
	}
	DrawCommand* cur = _redoCommand.pop();
	cur->Execute();
	_historyCommand.push(cur);
	UndoNumChanged(_historyCommand.size());
	RedoNumChanged(_redoCommand.size());
	ReDraw();
	update();
}

void DrawCanvas::ClearCommand() {
	for (int i = 0; i < _historyCommand.size(); ++i) {
		delete _historyCommand[i];
	}
	_historyCommand.clear();
	ClearRedoCommand();
}

void DrawCanvas::ClearRedoCommand() {
	for (int i = 0; i < _redoCommand.size(); ++i) {
		delete _redoCommand[i];
	}
	_redoCommand.clear();
	UndoNumChanged(_historyCommand.size());
	RedoNumChanged(_redoCommand.size());
}

void DrawCanvas::AddLayer(LayerModel* layer) {
	emit EmptyChanged(true);
	if (_layers.size() == 0) {
		int picMax = layer->GetImage().GetWidth() > layer->GetImage().GetHeight() ?
			layer->GetImage().GetWidth() : layer->GetImage().GetHeight();
		int labMac = _ui.Image->height() > _ui.Image->width() ? _ui.Image->height() : _ui.Image->width();
		_scale = labMac * 100.0f / picMax;
		_ui.horizontalScrollBar->setRange(0, 2 * GetDrawWidth());
		_ui.verticalScrollBar->setRange(0, 2 * GetDrawHeight());
	}
	layer->PixelChanged += [this](int i) {
		ReDraw(i);
	};
	layer->VisibleChanged += [this](bool) {
		ReDraw();
		update();
	};
	_layers.push_back(layer);
	//_seletcted.clear();
	//_seletcted.push_back(_layers.size() - 1);
	ReDraw();
	update();
}

void DrawCanvas::InsertLayer(LayerModel* layer, int i) {
	if (i == 0) {
		AddLayer(layer);
		return;
	}
	emit EmptyChanged(true);
	layer->PixelChanged += [this](int i) {
		ReDraw(i);
	};
	layer->VisibleChanged += [this](bool) {
		ReDraw();
		update();
	};
	_layers.insert(i, layer);
	//_seletcted.clear();
	//_seletcted.push_back(i);
	ReDraw();
	update();
}

void DrawCanvas::DeleteLayer(int index) {
	delete _layers[index];
	_layers.remove(index);
	for (int i = _seletcted.size() - 1; i >= 0; --i) {
		if (_seletcted[i] == index) {
			_seletcted.remove(i);
			break;
		}
	}
	if (IsEmpty()) {
		emit EmptyChanged(false);
	}
	ReDraw();
	update();
}

const QVector<LayerModel*>& DrawCanvas::GetLayers() {
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
	ReDraw();
	_canvas->WriteImage(fileName.toLatin1().data());
}

void DrawCanvas::PushCommand(DrawCommand* c) {
	ClearRedoCommand();
	_historyCommand.push(c);
	UndoNumChanged(_historyCommand.size());
	RedoNumChanged(_redoCommand.size());
}

void DrawCanvas::SetTool(DrawTools* t) {
	if (_tool != nullptr) {
		_tool->Unrigister();
	}
	_tool = t;
	_tool->Rigister(this);
}

void DrawCanvas::SetBackground(const MyImage::Image& im) {
	if (_canvasFill != nullptr) {
		delete _canvasFill;
		delete _canvas;
	}
	_canvasFill = new MyImage::BitMap_32(im.GetBits(), im.GetHeight(), im.GetWidth());
	_canvas = new MyImage::BitMap_32(im.GetHeight(), im.GetWidth());
	ReDraw();
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

void DrawCanvas::Resize(int h, int w) {
	auto layer = GetLayers();
	_canvas->Resize(h, w);
	_canvasFill->Resize(h, w);
	for (int i = 0; i < layer.size(); ++i) {
		auto newImage = layer[i]->GetImage().Clone();
		newImage->Resize(h, w);
		layer[i]->SetImage(newImage);
		delete newImage;
	}
	ReDraw();
	update();
}

void DrawCanvas::ReDraw() {
	memcpy(_canvas->GetBits(), _canvasFill->GetBits(),
		_canvas->GetWidth() * _canvas->GetHeight() * 4);
	int num = _layers.count();
	for (int i = 0; i < num; ++i) {
		if (_layers[i]->IsVisible()) {
			_layers[i]->PaintEvent(*_canvas);
		}
	}
}

void DrawCanvas::ReDraw(int i) {
	_canvas->SetPixel(i, _canvasFill->GetPixel(i));
	//qDebug() << "原先" << _canvas->GetPixel(i).rgbRed << " " << _canvas->GetPixel(i).rgbGreen << " " << _canvas->GetPixel(i).rgbBlue << " " << _canvas->GetPixel(i).rgbReserved;
	int num = _layers.count();
	for (int j = 0; j < num; ++j) {
		if (_layers[j]->IsVisible()) {
			_layers[j]->PaintEvent(i, *_canvas);
		}
	}
	//qDebug() << "后来" << _canvas->GetPixel(i).rgbRed << " " << _canvas->GetPixel(i).rgbGreen << " " << _canvas->GetPixel(i).rgbBlue << " " << _canvas->GetPixel(i).rgbReserved;
}

void DrawCanvas::ReDraw(int, int) {}

int DrawCanvas::GetImageHeight() {
	return _canvasFill->GetHeight();
}

int DrawCanvas::GetImageWidth() {
	return _canvasFill->GetWidth();
}

void DrawCanvas::mousePressEvent(QMouseEvent* e) {
	if (IsEmpty() || _tool == nullptr) {
		return;
	}
	if (!_tool->mousePressEvent(e)) {
		QMessageBox::warning(NULL, "warning", QStringLiteral("当前选中图层不可进行指定操作"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
	update();
}

void DrawCanvas::mouseReleaseEvent(QMouseEvent* e) {
	if (IsEmpty() || _tool == nullptr) {
		return;
	}
	if (!_tool->mouseReleaseEvent(e)) {
		QMessageBox::warning(NULL, "warning", QStringLiteral("当前选中图层不可进行指定操作"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
	update();
}

void DrawCanvas::mouseMoveEvent(QMouseEvent* e) {
	if (IsEmpty() || _tool == nullptr) {
		return;
	}
	if (!_tool->mouseMoveEvent(e)) {
		QMessageBox::warning(NULL, "warning", QStringLiteral("当前选中图层不可进行指定操作"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
	update();
}

void DrawCanvas::contextMenuEvent(QContextMenuEvent*e) {
	QWidget::contextMenuEvent(e);
}

void DrawCanvas::keyPressEvent(QKeyEvent* e) {
	QWidget::keyPressEvent(e);
}

void DrawCanvas::wheelEvent(QWheelEvent* event) {
	int offestX = event->x() - _drawPoint.x();
	int offestY = event->y() - _drawPoint.y();
	float oldScale = GetScale();
	SetScale(_scale + event->delta() * 0.03);
	offestX *= (GetScale() / oldScale);
	offestY *= (GetScale() / oldScale);
	SetDrawPoint(QPoint(event->x() - offestX, event->y() - offestY));
	update();
}

void DrawCanvas::resizeEvent(QResizeEvent* e) {
	if (!IsEmpty()) {
		_ui.horizontalScrollBar->setRange(0, 2 * GetDrawWidth());
		_ui.verticalScrollBar->setRange(0, 2 * GetDrawHeight());
	}
	QWidget::resizeEvent(e);
}

bool DrawCanvas::eventFilter(QObject* watched, QEvent* event) {
	if (watched == _ui.Image && event->type() == QEvent::Paint) {
		DrawImage();
	}
	return QWidget::eventFilter(watched, event);
}

void DrawCanvas::DrawImage() {
	if (!IsEmpty()) {
		QPainter painter;
		painter.begin(_ui.Image);
		painter.drawImage(GetDrawRect(), _canvas->ToQImage());
		painter.end();
	}
}

void DrawCanvas::SetDrawPoint(QPoint p) {
	if (IsEmpty()) {
		return;
	}
	int x = p.x();
	int y = p.y();
	int w = _ui.Image->rect().width();
	int h = _ui.Image->rect().height();
	//过滤x
	if (x + GetDrawWidth() -  w / 2 < 0) {
		x = -GetDrawWidth() + w / 2;
	}
	else if (x > w / 2) {
		x = w / 2;
	}
	//过滤y
	if (y + GetDrawHeight() - h / 2 < 0) {
		y = -GetDrawHeight() + h / 2;
	}
	else if (y > h / 2) {
		y = h / 2;
	}
	_drawPoint.setX(x);
	_drawPoint.setY(y);

	_ui.horizontalScrollBar->blockSignals(true);
	_ui.verticalScrollBar->blockSignals(true);
	_ui.horizontalScrollBar->setValue(_ui.Image->width() / 2 - x);
	_ui.verticalScrollBar->setValue(_ui.Image->height() / 2 - y);
	_ui.verticalScrollBar->blockSignals(false);
	_ui.horizontalScrollBar->blockSignals(false);
	update();
}

void DrawCanvas::HorizontalMove(int v) {
	if (IsEmpty()) {
		return;
	}
	_drawPoint.setX(_ui.Image->width() / 2 - v);
	update();
}

void DrawCanvas::VertiaclMove(int v) {
	if (IsEmpty()) {
		return;
	}
	_drawPoint.setY(_ui.Image->height() / 2 - v);
	update();
}

int DrawCanvas::GetDrawHeight() {
	return _canvasFill->GetHeight() * _scale * 0.01;
}

int DrawCanvas::GetDrawWidth() {
	return _canvasFill->GetWidth()* _scale * 0.01;
}

QRect DrawCanvas::GetDrawRect() {
	QRect res;
	res.setTopLeft(_drawPoint);
	res.setHeight(_canvasFill->GetHeight() * _scale * 0.01);
	res.setWidth(_canvasFill->GetWidth() * _scale * 0.01);
	return res;
}
