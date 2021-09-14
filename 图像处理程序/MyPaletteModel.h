#pragma once
#include"Image.h"
#include"qvector.h"
#include"MyEvent.h"
class MyPaletteModel {
public:
	static MyPaletteModel& GetInstance();
	void AddCurrentColor();//向色板添加当前前景色
	MyImage::RGBQUAD GetColor(int);//获取色板上指定索引的颜色
	void DeleteColor(int);//删除色板上指定位置的颜色
	void SwapColor();//交换前后景色
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
	QVector<MyImage::RGBQUAD> _colorBoard;//色板
	MyImage::RGBQUAD _frontColor = {0,0,0,255};//前景色
	MyImage::RGBQUAD _backColor = {0,0,0,255};//背景色
};
