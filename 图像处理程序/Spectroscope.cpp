#include "Spectroscope.h"
#include "MyPaletteModel.h"

bool Spectroscope::mousePressEvent(QMouseEvent*e) {
    if (GetDevice().GetSelected().size() != 1) {
        return false;
    }
    if (GetDevice().GetLayers()[GetDevice().GetSelected()[0]]->IsLock()) {
        return false;
    }
    return true;
}

bool Spectroscope::mouseReleaseEvent(QMouseEvent*e) {
    if (GetDevice().GetSelected().size() != 1) {
        return false;
    }
    if (GetDevice().GetLayers()[GetDevice().GetSelected()[0]]->IsLock()) {
        return false;
    }
    GetColor(e->pos(), GetDevice().GetLayers()[GetDevice().GetSelected()[0]]);
    return true;
}

bool Spectroscope::mouseMoveEvent(QMouseEvent* e) {
    if (GetDevice().GetSelected().size() != 1) {
        return false;
    }
    if (GetDevice().GetLayers()[GetDevice().GetSelected()[0]]->IsLock()) {
        return false;
    }
    GetColor(e->pos(), GetDevice().GetLayers()[GetDevice().GetSelected()[0]]);
    return true;
}

inline void Spectroscope::GetColor(QPoint p, LayerModel* layer) {
    MyPaletteModel::GetInstance().
        SetFrontColor(layer->GetImage().GetPixel(TransformPoint
        (p, &GetDevice()).x(), TransformPoint(p, &GetDevice()).y()));
}

QPoint Spectroscope::TransformPoint(QPoint p, DrawCanvas* canvas) {
    QPoint res;
    QPoint drawStart = canvas->GetDrawPoint();
    res.setX((p.x() - drawStart.x()) / (canvas->GetScale() * 0.01));
    res.setY((p.y() - drawStart.y()) / (canvas->GetScale() * 0.01));
    return res;
}
