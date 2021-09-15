#pragma once
#include<QMouseEvent>
#include<qvector.h>
#include"Image.h"
#include"DrawCanvas.h"

class DrawCanvas;

//绘画工具基类
class DrawTools {
public:
	void Rigister(DrawCanvas*);//注册绘制设备
	void Unrigister();//注销绘制设备
	virtual bool mousePressEvent(QMouseEvent*) = 0;//鼠标按下事件
	virtual bool mouseReleaseEvent(QMouseEvent*) = 0;//鼠标释放事件
	virtual bool mouseMoveEvent(QMouseEvent*) = 0;//鼠标移动事件
	DrawCanvas& GetDevice();
private:
	DrawCanvas* _device;
};

