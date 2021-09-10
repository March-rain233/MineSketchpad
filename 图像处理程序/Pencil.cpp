#include "Pencil.h"
#include "MyPaletteModel.h"

bool Pencil::mousePressEvent(QMouseEvent* e) {
    if (GetDevice().GetSelected().size() != 1) {
        return false;
    }
    _command = new PaintCommand(GetDevice().GetLayers());
    _startPoint = e->pos();
    return true;
}

bool Pencil::mouseReleaseEvent(QMouseEvent* e) {
    if (GetDevice().GetSelected().size() != 1) {
        return false;
    }
    if (_command != nullptr) {
        DrawLine(TransformPoint(_startPoint, &GetDevice()),
            TransformPoint(e->pos(), &GetDevice()), GetDevice().GetSelected()[0]);
        GetDevice().PushCommand(_command);
        _command = nullptr;
    }
    return false;
}

bool Pencil::mouseMoveEvent(QMouseEvent* e) {
    if (GetDevice().GetSelected().size() != 1) {
        return false;
    }
    if (_command != nullptr) {
        DrawLine(TransformPoint(_startPoint, &GetDevice()),
            TransformPoint(e->pos(), &GetDevice()), GetDevice().GetSelected()[0]);
        _startPoint = e->pos();
    }
    return false;
}

void Pencil::SetAlpha(unsigned char v) {
    _alpha = v;
    AlphaChanged(v);
}

void Pencil::SetRadius(int v) {
    _radius = v;
    RadiusChanged(v);
}

void Pencil::UpdateAll() {
    AlphaChanged(_alpha);
    RadiusChanged(_radius);
}

QPoint Pencil::TransformPoint(QPoint p, DrawCanvas* canvas) {
    QPoint res;
    QPoint drawStart = canvas->GetDrawPoint();
    res.setX((p.x() - drawStart.x()) / (canvas->GetScale() * 0.01));
    res.setY((p.y() - drawStart.y()) / (canvas->GetScale() * 0.01));
    return res;
}

void Pencil::FillRow(int x1, int x2, int y, MyImage::RGBQUAD v) {
    DrawCanvas& device = GetDevice();
    int layer = device.GetSelected()[0];
    for (int i = x1; i <= x2; ++i) {
        _command->SetPixel(layer, y, i, 
            OverplayMode(v,device.GetLayers()[layer]->GetPixel(i,y)));
    }
}

void Pencil::DrawPoint(QPoint p, int layer) {
    int i, j;
    int ym;
    int xymax = _radius / 1.414;//45°线与圆的交点坐标(取整)
    int x = p.x();
    int y = p.y();

    auto color = GetColor();
    for (i = 0; i <= xymax; i++)//遍历x轴，45°~90°区域
    {
        j = 0;
        while (i * i + j * j - _radius * _radius <= 0) {//找到圆的边界
            j++;
        }
        ym = (i * i + j * j - _radius * _radius - j < 0) ? j : (j - 1);//边界内外侧更接近圆的坐标
        FillRow(x - i, x + i, y - ym, color); //按行填充
        FillRow(x - i, x + i, y + ym, color);
        FillRow(x - ym, x + ym, y - i, color);
        FillRow(x - ym, x + ym, y + i, color);
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
