#include "Pencil.h"

bool Pencil::mousePressEvent(QMouseEvent* e, DrawCanvas* canvas) {
    if (canvas->GetSelected().size() != 1) {
        return false;
    }
    _command = new PaintCommand(canvas->GetLayers());
    _startPoint = e->pos();
    return true;
}

bool Pencil::mouseReleaseEvent(QMouseEvent* e, DrawCanvas* canvas) {
    if (canvas->GetSelected().size() != 1) {
        return false;
    }
    if (_command != nullptr) {
        DrawLine(TransformPoint(_startPoint, canvas),
            TransformPoint(e->pos(), canvas), canvas->GetSelected()[0]);
        canvas->PushCommand(_command);
        _command = nullptr;
    }
    return false;
}

bool Pencil::mouseMoveEvent(QMouseEvent* e, DrawCanvas* canvas) {
    if (canvas->GetSelected().size() != 1) {
        return false;
    }
    if (_command != nullptr) {
        DrawLine(TransformPoint(_startPoint, canvas),
            TransformPoint(e->pos(), canvas), canvas->GetSelected()[0]);
        _startPoint = e->pos();
    }
    return false;
}

void Pencil::SetRadius(int v) {
    _radius = v;
}

void Pencil::SetColor(MyImage::RGBQUAD v) {
    _color = v;
}

QPoint Pencil::TransformPoint(QPoint p, DrawCanvas* canvas) {
    QPoint res;
    QPoint drawStart = canvas->GetDrawPoint();
    res.setX((p.x() - drawStart.x()) / (canvas->GetScale() * 0.01));
    res.setY((p.y() - drawStart.y()) / (canvas->GetScale() * 0.01));
    return res;
}

void FillRow(int x1, int y1, int x2, int y2, MyImage::RGBQUAD v, PaintCommand* c, int layer) {
    for (int i = x1; i <= x2; ++i) {
        c->SetPixel(layer, y1, i, v);
    }
}

void Pencil::DrawPoint(QPoint p, int layer) {
    int i, j;
    int ym;
    int xymax = _radius / 1.414;//45°线与圆的交点坐标(取整)
    int x = p.x();
    int y = p.y();
    for (i = 0; i <= xymax; i++)//遍历x轴，45°~90°区域
    {
        j = 0;
        while (i * i + j * j - _radius * _radius <= 0) {//找到圆的边界
            j++;
        }
        ym = (i * i + j * j - _radius * _radius - j < 0) ? j : (j - 1);//边界内外侧更接近圆的坐标
        FillRow(x - i, y - ym, x + i, y - ym, _color, _command, layer); //按行填充
        FillRow(x - i, y + ym, x + i, y + ym, _color, _command, layer);
        FillRow(x - ym, y - i, x + ym, y - i, _color, _command, layer);
        FillRow(x - ym, y + i, x + ym, y + i, _color, _command, layer);
    }
}

void Pencil::DrawLine(QPoint start, QPoint end, int layer) {
    int x1 = start.x();
    int x2 = end.x();
    int y1 = start.y();
    int y2 = end.y();
    int maxy = std::max(start.y(), end.y());
    float k = (end.y() - start.y()) * 1.0 / (end.x() - start.x());//直线斜率
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    for (int i = x1; i <= x2; ++i) {
        float t = (i - x1) * k;//准确值
        int j = maxy;
        while (j - y1 - t > 0) {
            --j;
        }
        int ym = abs(j - t) < abs(j + 1 - t) ? j : j + 1;
        DrawPoint(QPoint(i, ym), layer);
    }
}
