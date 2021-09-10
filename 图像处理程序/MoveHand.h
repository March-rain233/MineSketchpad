#pragma once
#include "DrawTools.h"
class MoveHand :
    public DrawTools {
public:
    // ͨ�� DrawTools �̳�
    virtual bool mousePressEvent(QMouseEvent*) override;
    virtual bool mouseReleaseEvent(QMouseEvent*) override;
    virtual bool mouseMoveEvent(QMouseEvent*) override;
private:
    QPoint _prePoint;
};

