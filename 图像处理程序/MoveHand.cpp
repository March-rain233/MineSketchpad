#include "MoveHand.h"

bool MoveHand::mousePressEvent(QMouseEvent* e) {
    _prePoint = e->pos();
    return true;
}

bool MoveHand::mouseReleaseEvent(QMouseEvent* e) {
    _prePoint.setX(0);
    _prePoint.setY(0);
    return false;
}

bool MoveHand::mouseMoveEvent(QMouseEvent* e) {
    QPoint draw = GetDevice().GetDrawPoint();
    int offsetX = e->pos().x() - _prePoint.x();
    int offsetY = e->pos().y() - _prePoint.y();
    draw.setX(draw.x() + offsetX);
    draw.setY(draw.y() + offsetY);
    GetDevice().SetDrawPoint(draw);
    _prePoint = e->pos();
    return false;
}
