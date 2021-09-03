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
	
private:
	void DrawPoint(QPoint);//����
private:
	int _radius;//���ʵİ뾶
	MyImage::RGBQUAD _color;//���ʵ���ɫ
	DrawCommand* _command;
};

