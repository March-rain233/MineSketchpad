#include "Pencil.h"

bool Pencil::mousePressEvent(QMouseEvent* e, DrawCanvas* canvas) {
    if (canvas->GetSelected().size() != 1) {
        return false;
    }
    return true;
}

bool Pencil::mouseReleaseEvent(QMouseEvent* e, DrawCanvas* canvas) {
    if (canvas->GetSelected().size() != 1) {
        return false;
    }
    return false;
}

bool Pencil::mouseMoveEvent(QMouseEvent* e, DrawCanvas* canvas) {
    if (canvas->GetSelected().size() != 1) {
        return false;
    }
    return false;
}

void Pencil::SetRadius(int v) {
    _radius = v;
}

void Pencil::SetColor(MyImage::RGBQUAD v) {
    _color = v;
}

QVector<QPoint> GetCircle(QRect rect, QPoint p, int r) {
    QVector<QPoint> list;

}

void Pencil::DrawPoint(QPoint p) {

}
