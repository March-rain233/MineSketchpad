#include "MyPaletteModel.h"

MyPaletteModel* MyPaletteModel::_instance = nullptr;

MyPaletteModel& MyPaletteModel::GetInstance() {
    if (_instance == nullptr) {
        _instance = new MyPaletteModel();
    }
    return *_instance;
}

void MyPaletteModel::AddCurrentColor() {
    _colorBoard.push_back(_frontColor);
}

MyImage::RGBQUAD MyPaletteModel::GetColor(int index) {
    return _colorBoard[index];
}

void MyPaletteModel::DeleteColor(int index) {
    _colorBoard.remove(index);
}

void MyPaletteModel::SwapColor() {
    std::swap(_frontColor, _backColor);
    FrontColorChange(_frontColor);
    BackColorChange(_backColor);
}

void MyPaletteModel::SetFrontColor(MyImage::RGBQUAD color) {
    _frontColor = color;
    FrontColorChange.Invoke(color);
}

void MyPaletteModel::SetBackColor(MyImage::RGBQUAD v) {
    _backColor = v;
    BackColorChange(v);
}

MyImage::RGBQUAD MyPaletteModel::GetFrontColor() {
    return _frontColor;
}

MyImage::RGBQUAD MyPaletteModel::GetBackColor() {
    return _backColor;
}

MyPaletteModel::MyPaletteModel() {}
