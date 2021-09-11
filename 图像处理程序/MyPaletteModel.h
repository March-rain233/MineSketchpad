#pragma once
#include"Image.h"
#include"qvector.h"
#include"MyEvent.h"
class MyPaletteModel {
public:
	static MyPaletteModel& GetInstance();
	void AddCurrentColor();//��ɫ����ӵ�ǰǰ��ɫ
	MyImage::RGBQUAD GetColor(int);//��ȡɫ����ָ����������ɫ
	void DeleteColor(int);//ɾ��ɫ����ָ��λ�õ���ɫ
	void SwapColor();//����ǰ��ɫ
	void SetFrontColor(MyImage::RGBQUAD);
	void SetBackColor(MyImage::RGBQUAD);
	MyImage::RGBQUAD GetFrontColor();
	MyImage::RGBQUAD GetBackColor();
private:
	MyPaletteModel();
public:
	MyEvent<MyPaletteModel, MyImage::RGBQUAD> FrontColorChange;
	MyEvent<MyPaletteModel, MyImage::RGBQUAD> BackColorChange;
private:
	static MyPaletteModel* _instance;
	QVector<MyImage::RGBQUAD> _colorBoard;//ɫ��
	MyImage::RGBQUAD _frontColor = {0,0,0,255};//ǰ��ɫ
	MyImage::RGBQUAD _backColor = {0,0,0,255};//����ɫ
};

