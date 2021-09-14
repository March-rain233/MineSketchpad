#pragma once
#include "DrawTools.h"
class Spectroscope :
    public DrawTools {
    // 通过 DrawTools 继承
public:
    virtual bool mousePressEvent(QMouseEvent*) override;
    virtual bool mouseReleaseEvent(QMouseEvent*) override;
    virtual bool mouseMoveEvent(QMouseEvent*) override;
private:
    void GetColor(QPoint p, LayerModel* layer);
    QPoint TransformPoint(QPoint, DrawCanvas*);//将画布坐标转换为图像坐标
};

