#include "LayerModel.h"
#include "OverlayFunctionFactory.h"
#include <qdebug.h>

LayerModel::LayerModel(MyImage::Image* val, OverlayMode mode) {
    _image = val;
    _isVisible = true;
    _isLock = false;
    _isPainting = false;
    _buffer = new MyImage::BitMap_32(val->GetHeight(), val->GetWidth());
    val->PixelChanged += [this](int i) {
        PixelChanged(i);
    };
    _buffer->PixelChanged += [this](int i) {
        PixelChanged(i);
    };
    SetOverlayMode(mode);
}

LayerModel::~LayerModel() {
    delete _image;
    delete _buffer;
}

MyImage::Image& LayerModel::GetImage() {
    return *_image;
}

bool LayerModel::IsLock() {
    return _isLock && _isVisible;
}

bool LayerModel::IsVisible() {
    return _isVisible;
}

void LayerModel::SetLock(bool v) {
    _isLock = v;
}

void LayerModel::SetVisible(bool v) {
    _isVisible = v;
    VisibleChanged(v);
}

void LayerModel::BeginDraw() {
    memcpy(_buffer->GetBits(), _image->GetBits(), _buffer->GetHeight() * _buffer->GetWidth() * 4);
    _isPainting = true;
}

MyImage::Image& LayerModel::GetBuffer() {
    return *_buffer;
}

void LayerModel::EndDraw() {
    _isPainting = false;
    std::swap(_buffer, _image);
}

void LayerModel::PaintEvent(MyImage::Image& canvas) {
    int total = canvas.GetHeight() * canvas.GetWidth(); 
    for (int i = 0; i < total; ++i) {
        PaintEvent(i, canvas);
    }
}

void LayerModel::PaintEvent(int i, MyImage::Image& canvas) {
    if (_isPainting) {
        canvas.SetPixel(i, _overlayHandler(_buffer->GetPixel(i), canvas.GetPixel(i)));
    }
    else {
        canvas.SetPixel(i, _overlayHandler(_image->GetPixel(i), canvas.GetPixel(i)));
    }
}

void LayerModel::PaintEvent(int i, int j, MyImage::Image& canvas) {
    PaintEvent(i * _image->GetWidth() + j, canvas);
}

void LayerModel::SetOverlayMode(OverlayMode mode) {
    _overlayMode = mode;
    _overlayHandler = OverlayFunctionFactory::GetInstance().Create(mode);
}

OverlayMode LayerModel::GetOverlayMode() {
    return _overlayMode;
}

void LayerModel::Update() {
    if (_isVisible) {
        VisibleChanged(true);
    }
}
