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
	
private:
	void DrawPoint(QPoint);//画点
private:
	int _radius;//画笔的半径
	MyImage::RGBQUAD _color;//画笔的颜色
	DrawCommand* _command;
};

