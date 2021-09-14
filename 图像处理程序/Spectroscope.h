#pragma once
#include "DrawTools.h"
class Spectroscope :
    public DrawTools {
    // ͨ�� DrawTools �̳�
public:
    virtual bool mousePressEvent(QMouseEvent*) override;
    virtual bool mouseReleaseEvent(QMouseEvent*) override;
    virtual bool mouseMoveEvent(QMouseEvent*) override;
private:
    void GetColor(QPoint p, LayerModel* layer);
    QPoint TransformPoint(QPoint, DrawCanvas*);//����������ת��Ϊͼ������
};

