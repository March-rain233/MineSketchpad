#pragma once
#include"DrawTools.h"
#include"Image.h"
class Pencil: public DrawTools {
public:
	// ͨ�� DrawTools �̳�
	virtual bool mousePressEvent(QMouseEvent*, DrawCanvas*) override;
	virtual bool mouseReleaseEvent(QMouseEvent*, DrawCanvas*) override;
	virtual bool mouseMoveEvent(QMouseEvent*, DrawCanvas*) override;
	
	void SetRadius(int);//���ð뾶
	void SetColor(MyImage::RGBQUAD);//������ɫ
	QPoint TransformPoint(QPoint, DrawCanvas*);//����������ת��Ϊͼ������
	
private:
	void DrawPoint(QPoint, int);//����
	void DrawLine(QPoint, QPoint, int);//����
private:
	QPoint _startPoint;
	int _radius = 8;//���ʵİ뾶
	MyImage::RGBQUAD _color = { 0,0,0,255 };//���ʵ���ɫ
	PaintCommand* _command = nullptr;//����
};

