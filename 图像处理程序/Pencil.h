#pragma once
#include"DrawTools.h"
#include"Image.h"
#include"MyEvent.h"
#include<functional>
#include"ImageFilter.h"

class ToolFactory;

class Pencil: public DrawTools {
public:
	// ͨ�� DrawTools �̳�
	virtual bool mousePressEvent(QMouseEvent*) override;
	virtual bool mouseReleaseEvent(QMouseEvent*) override;
	virtual bool mouseMoveEvent(QMouseEvent*) override;
	
	void SetAlpha(unsigned char);//����͸����
	void SetRadius(int);//���ð뾶

	void UpdateAll();//֪ͨPencilˢ�´��������¼�
public:
	MyEvent<Pencil, int> RadiusChanged;
	MyEvent<Pencil, unsigned char> AlphaChanged;

	std::function<void(LayerModel& im, int x, int y, unsigned char v, PaintCommand* c)> FillPixel;//���ص�����㷨
private:
	QPoint TransformPoint(QPoint, DrawCanvas*);//����������ת��Ϊͼ������
	void FillRow(int, int, int);//���һ������
	void DrawPoint(QPoint);//����
	void DrawLine(QPoint, QPoint);//����
private:
	QPoint _startPoint;
	int _radius = 8;//���ʵİ뾶
	unsigned char _alpha;//����Ũ��
	PaintCommand* _command = nullptr;//����
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