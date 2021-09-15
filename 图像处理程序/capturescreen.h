#pragma once

#include <QWidget>
#include "ui_capturescreen.h"

#include <QWidget>
#include <QPainter>

class CaptureScreen : public QWidget {
    Q_OBJECT

public:
    CaptureScreen(QWidget* parent = 0);
    ~CaptureScreen();

Q_SIGNALS:
    void signalCompleteCature(QPixmap catureImage);

private:
    void initWindow();
    void loadBackgroundPixmap();

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void paintEvent(QPaintEvent* event);
    QRect CaptureScreen::getRect(const QPoint& beginPoint, const QPoint& endPoint);

private:
    bool m_isMousePress;
    QPixmap m_loadPixmap, m_capturePixmap;
    int m_screenwidth;
    int m_screenheight;
    QPoint m_beginPoint, m_endPoint;
    QPainter m_painter;
};
