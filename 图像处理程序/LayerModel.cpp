#include "LayerModel.h"
#include "OverlayFunctionFactory.h"
#include <qdebug.h>

LayerModel::LayerModel(MyImage::Image* val, OverlayMode mode) {
    _image = val;
    _isVisible = true;
    _isLock = false;
    SetOverlayMode(mode);
}

LayerModel::~LayerModel() {
    delete _image;
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
}

void LayerModel::PaintEvent(MyImage::Image& canvas) {
    int total = canvas.GetHeight() * canvas.GetWidth();
    auto p1 = canvas.GetBits();
    auto p2 = _image->GetBits();
    for (int i = 0; i < total; ++i) {
        p1[i] = _overlayHandler(p2[i], p1[i]);
    }
}

void LayerModel::PaintEvent(int i, MyImage::Image& canvas) {
    qDebug() << "ԭ��" << canvas.GetPixel(i).rgbRed << " " << canvas.GetPixel(i).rgbGreen << " " << canvas.GetPixel(i).rgbBlue << " " << canvas.GetPixel(i).rgbReserved;
    canvas.SetPixel(i, _overlayHandler(_image->GetPixel(i), canvas.GetPixel(i)));
    qDebug() << "����" << canvas.GetPixel(i).rgbRed << " " << canvas.GetPixel(i).rgbGreen << " " << canvas.GetPixel(i).rgbBlue << " " << canvas.GetPixel(i).rgbReserved;
}

void LayerModel::PaintEvent(int i, int j, MyImage::Image& canvas) {
    canvas.SetPixel(i, j, _overlayHandler(_image->GetPixel(i, j), canvas.GetPixel(i, j)));
}

void LayerModel::SetOverlayMode(OverlayMode mode) {
    _overlayMode = mode;
    _overlayHandler = OverlayFunctionFactory::GetInstance().Create(mode);
}

OverlayMode LayerModel::GetOverlayMode() {
    return _overlayMode;
}
