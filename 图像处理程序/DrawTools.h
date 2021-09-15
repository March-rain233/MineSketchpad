#pragma once
#include<QMouseEvent>
#include<qvector.h>
#include"Image.h"
#include"DrawCanvas.h"

class DrawCanvas;

//�滭���߻���
class DrawTools {
public:
	void Rigister(DrawCanvas*);//ע������豸
	void Unrigister();//ע�������豸
	virtual bool mousePressEvent(QMouseEvent*) = 0;//��갴���¼�
	virtual bool mouseReleaseEvent(QMouseEvent*) = 0;//����ͷ��¼�
	virtual bool mouseMoveEvent(QMouseEvent*) = 0;//����ƶ��¼�
	DrawCanvas& GetDevice();
private:
	DrawCanvas* _device;
};

