#pragma once
#include<QMouseEvent>
#include<qvector.h>
#include"Image.h"
#include"DrawCanvas.h"

class DrawCanvas;

//绘画工具基类
class DrawTools {
public:
	virtual bool mousePressEvent(QMouseEvent*, DrawCanvas*) = 0;//鼠标按下事件
	virtual bool mouseReleaseEvent(QMouseEvent*, DrawCanvas*) = 0;//鼠标释放事件
	virtual bool mouseMoveEvent(QMouseEvent*, DrawCanvas*) = 0;//鼠标移动事件
};

