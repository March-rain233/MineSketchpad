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

inline void RGBToHSL(MyImage::RGBQUAD color, double& H, double& S, double& L) {
	double R, G, B, Max, Min, del_R, del_G, del_B, del_Max;
	R = color.rgbRed / 255.0;       //Where RGB values = 0 ÷ 255
	G = color.rgbGreen / 255.0;
	B = color.rgbBlue / 255.0;

	using namespace std;
	Min = min(R, min(G, B));    //Min. value of RGB
	Max = max(R, max(G, B));    //Max. value of RGB
	del_Max = Max - Min;        //Delta RGB value

	L = (Max + Min) / 2.0;

	if (del_Max == 0)           //This is a gray, no chroma...
	{
		//H = 2.0/3.0;          //Windows下S值为0时，H值始终为160（2/3*240）
		H = 0;                  //HSL results = 0 ÷ 1
		S = 0;
	}
	else                        //Chromatic data...
	{
		if (L < 0.5) S = del_Max / (Max + Min);
		else         S = del_Max / (2 - Max - Min);

		del_R = (((Max - R) / 6.0) + (del_Max / 2.0)) / del_Max;
		del_G = (((Max - G) / 6.0) + (del_Max / 2.0)) / del_Max;
		del_B = (((Max - B) / 6.0) + (del_Max / 2.0)) / del_Max;

		if (R == Max) H = del_B - del_G;
		else if (G == Max) H = (1.0 / 3.0) + del_R - del_B;
		else if (B == Max) H = (2.0 / 3.0) + del_G - del_R;

		if (H < 0)  H += 1;
		if (H > 1)  H -= 1;
	}
}

//---------------------------------------------------------------------------
inline double Hue2RGB(double v1, double v2, double vH) {
	if (vH < 0) vH += 1;
	if (vH > 1) vH -= 1;
	if (6.0 * vH < 1) return v1 + (v2 - v1) * 6.0 * vH;
	if (2.0 * vH < 1) return v2;
	if (3.0 * vH < 2) return v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0;
	return (v1);
}

inline MyImage::RGBQUAD HSLToRGB(double H, double S, double L) {
	double R, G, B;
	double var_1, var_2;
	if (S == 0)                       //HSL values = 0 ÷ 1
	{
		R = L * 255.0;                   //RGB results = 0 ÷ 255
		G = L * 255.0;
		B = L * 255.0;
	}
	else {
		if (L < 0.5) var_2 = L * (1 + S);
		else         var_2 = (L + S) - (S * L);

		var_1 = 2.0 * L - var_2;

		R = 255.0 * Hue2RGB(var_1, var_2, H + (1.0 / 3.0));
		G = 255.0 * Hue2RGB(var_1, var_2, H);
		B = 255.0 * Hue2RGB(var_1, var_2, H - (1.0 / 3.0));
	}
	return MyImage::RGBQUAD{ (unsigned char)B, (unsigned char)G, (unsigned char)R,255 };
}
