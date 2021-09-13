#pragma once
#include"DrawTools.h"
#include"Image.h"
#include"MyEvent.h"
#include<functional>
#include"ImageFilter.h"

class ToolFactory;

class Pencil: public DrawTools {
public:
	// 通过 DrawTools 继承
	virtual bool mousePressEvent(QMouseEvent*) override;
	virtual bool mouseReleaseEvent(QMouseEvent*) override;
	virtual bool mouseMoveEvent(QMouseEvent*) override;
	
	void SetAlpha(unsigned char);//设置透明度
	void SetRadius(int);//设置半径

	void UpdateAll();//通知Pencil刷新触发所有事件
public:
	MyEvent<Pencil, int> RadiusChanged;
	MyEvent<Pencil, unsigned char> AlphaChanged;

	std::function<void(LayerModel& im, int x, int y, unsigned char v, PaintCommand* c)> FillPixel;//像素点填充算法
private:
	QPoint TransformPoint(QPoint, DrawCanvas*);//将画布坐标转换为图像坐标
	void FillRow(int, int, int);//填充一行像素
	void DrawPoint(QPoint);//画点
	void DrawLine(QPoint, QPoint);//画线
private:
	QPoint _startPoint;
	int _radius = 8;//画笔的半径
	unsigned char _alpha;//画笔浓度
	PaintCommand* _command = nullptr;//命令
};

class FilterPen :
	public Pencil {
public:
	FilterPen();
	void SetFilter(ImageFilter*);
	ImageFilter* GetFilter();
private:
	ImageFilter* _filter;
};