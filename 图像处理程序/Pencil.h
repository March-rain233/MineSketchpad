#pragma once
#include"DrawTools.h"
#include"Image.h"
class Pencil: public DrawTools {
public:
	// 通过 DrawTools 继承
	virtual bool mousePressEvent(QMouseEvent*, DrawCanvas*) override;
	virtual bool mouseReleaseEvent(QMouseEvent*, DrawCanvas*) override;
	virtual bool mouseMoveEvent(QMouseEvent*, DrawCanvas*) override;
	
	void SetRadius(int);//设置半径
	void SetColor(MyImage::RGBQUAD);//设置颜色
	QPoint TransformPoint(QPoint, DrawCanvas*);//将画布坐标转换为图像坐标
	
private:
	void DrawPoint(QPoint, int);//画点
	void DrawLine(QPoint, QPoint, int);//画线
private:
	QPoint _startPoint;
	int _radius = 8;//画笔的半径
	MyImage::RGBQUAD _color = { 0,0,0,255 };//画笔的颜色
	PaintCommand* _command = nullptr;//命令
};

