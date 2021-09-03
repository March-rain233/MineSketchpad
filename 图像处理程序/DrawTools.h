#pragma once
#include<QMouseEvent>
#include<qvector.h>
#include"Image.h"
#include"DrawCanvas.h"

class DrawCanvas;

//�滭���߻���
class DrawTools {
public:
	virtual bool mousePressEvent(QMouseEvent*, DrawCanvas*) = 0;//��갴���¼�
	virtual bool mouseReleaseEvent(QMouseEvent*, DrawCanvas*) = 0;//����ͷ��¼�
	virtual bool mouseMoveEvent(QMouseEvent*, DrawCanvas*) = 0;//����ƶ��¼�
};

