#include "Pencil.h"
#include "MyPaletteModel.h"

bool Pencil::mousePressEvent(QMouseEvent* e) {
    if (GetDevice().GetSelected().size() != 1) {
        return false;
    }
    if (GetDevice().GetLayers()[GetDevice().GetSelected()[0]]->IsLock()) {
        return false;
    }
    _command = new PaintCommand(*GetDevice().GetLayers()
        [GetDevice().GetSelected()[0]]);
    GetDevice().GetLayers()
        [GetDevice().GetSelected()[0]]->BeginDraw();
    _startPoint = e->pos();
    return true;
}

bool Pencil::mouseReleaseEvent(QMouseEvent* e) {
    if (GetDevice().GetSelected().size() != 1) {
        return false;
    }
    if (GetDevice().GetLayers()[GetDevice().GetSelected()[0]]->IsLock()) {
        return false;
    }
    if (_command != nullptr) {
        DrawLine(TransformPoint(_startPoint, &GetDevice()),
            TransformPoint(e->pos(), &GetDevice()));
        GetDevice().PushCommand(_command);
        GetDevice().GetLayers()
            [GetDevice().GetSelected()[0]]->EndDraw();
        _command = nullptr;
    }
    return true;
}

bool Pencil::mouseMoveEvent(QMouseEvent* e) {
    if (GetDevice().GetSelected().size() != 1) {
        return false;
    }
    if (GetDevice().GetLayers()[GetDevice().GetSelected()[0]]->IsLock()) {
        return false;
    }
    if (_command != nullptr) {
        DrawLine(TransformPoint(_startPoint, &GetDevice()),
            TransformPoint(e->pos(), &GetDevice()));
        _startPoint = e->pos();
    }
    return true;
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

void Pencil::FillRow(int x1, int x2, int y) {
    DrawCanvas& device = GetDevice();
    int layer = device.GetSelected()[0];
    if (y < 0 || y >= device.GetImageHeight()) {
        return;
    }
    if (x1 > x2) {
        std::swap(x1, x2);
    }
    if (x1 >= device.GetImageWidth()) {
        return;
    }
    if (x1 < 0) {
        x1 = 0;
    }
    if (x2 < 0) {
        return;
    }
    if (x2 >= device.GetImageWidth()) {
        x2 = device.GetImageWidth() - 1;
    }
    for (int i = x1; i <= x2; ++i) {
        FillPixel(*device.GetLayers()[layer], i, y , _alpha, _command);
    }
}

void Pencil::DrawPoint(QPoint p) {
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
        FillRow(x - i, x + i, y - ym); //按行填充
        FillRow(x - i, x + i, y + ym);
        FillRow(x - ym, x + ym, y - i);
        FillRow(x - ym, x + ym, y + i);
    }
}

void Pencil::DrawLine(QPoint start, QPoint end) {
    int x1 = start.x();
    int x2 = end.x();
    int y1 = start.y();
    int y2 = end.y();
    if (x1 == x2) {
        if (y1 > y2) {
            std::swap(y1, y2);
        }
        if (y1 >= GetDevice().GetImageHeight() || y2 <= 0) {
            return;
        }
        if (y2 >= GetDevice().GetImageHeight()) {
            y2 = GetDevice().GetImageHeight() - 1;
        }
        if (y1 < 0) {
            y1 = 0;
        }
        for (int i = y1; i <= y2; ++i) {
            DrawPoint(QPoint(x1, i));
        }
    }
    else {
        float k = (end.y() - start.y()) * 1.0 / (end.x() - start.x());//直线斜率
        if (k <= 1) {
            int maxy = std::max(start.y(), end.y());
            if (x1 > x2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            if (x1 >= GetDevice().GetImageWidth() || x2 <= 0) {
                return;
            }
            if (x2 >= GetDevice().GetImageWidth()) {
                x2 = GetDevice().GetImageWidth() - 1;
            }
            if (x1 < 0) {
                x1 = 0;
            }
            for (int i = x1; i <= x2; ++i) {
                float t = (i - x1) * k;//准确值
                int j = maxy;
                while (j - y1 - t > 0) {
                    --j;
                }
                int ym = abs(j - t) < abs(j + 1 - t) ? j : j + 1;
                DrawPoint(QPoint(i, ym));
            }
        }
        else {
            int maxx = std::max(start.x(), end.x());
            if (y1 > y2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            if (y1 >= GetDevice().GetImageHeight() || y2 <= 0) {
                return;
            }
            if (y2 >= GetDevice().GetImageHeight()) {
                y2 = GetDevice().GetImageHeight() - 1;
            }
            if (y1 < 0) {
                y1 = 0;
            }
            for (int i = y1; i <= y2; ++i) {
                float t = (i - y1) / k;//准确值
                int j = maxx;
                while (j - x1 - t > 0) {
                    --j;
                }
                int xm = abs(j - t) < abs(j + 1 - t) ? j : j + 1;
                DrawPoint(QPoint(xm, i));
            }
        }
    }
}

FilterPen::FilterPen() {
    FillPixel = [this](LayerModel& im, int x, int y, unsigned char v, PaintCommand* c) {
        c->SetPixel(x, y, _filter->FliterPixel(im.GetImage(), x, y));
    };
}

void FilterPen::SetFilter(ImageFilter* f) {
    _filter = f;
}

ImageFilter* FilterPen::GetFilter() {
    return _filter;
}
