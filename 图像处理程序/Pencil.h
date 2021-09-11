#pragma once
#include"DrawTools.h"
#include"Image.h"
#include"MyEvent.h"
#include<functional>

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

	std::function<MyImage::RGBQUAD(void)> GetColor;
	std::function<MyImage::RGBQUAD(MyImage::RGBQUAD, MyImage::RGBQUAD)> OverlayMode;
private:
	QPoint TransformPoint(QPoint, DrawCanvas*);//����������ת��Ϊͼ������
	void FillRow(int, int, int, MyImage::RGBQUAD);//���һ������
	void DrawPoint(QPoint, int);//����
	void DrawLine(QPoint, QPoint, int);//����
private:
	QPoint _startPoint;
	int _radius = 8;//���ʵİ뾶
	unsigned char _alpha;//����Ũ��
	PaintCommand* _command = nullptr;//����
};

