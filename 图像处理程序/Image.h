#pragma once
#include<qimage.h>
#include"Mat.hpp"
#include"MyEvent.h"
namespace MyImage {
#pragma pack(push,1)
	struct BITMAPFILEHEADER {
		unsigned short bfType;
		unsigned int   bfSize;
		unsigned short bfReserved1;
		unsigned short bfReserved2;
		unsigned int   bfOffBits;
	};

#pragma pack(pop)

	struct BITMAPINFOHEADER {
		unsigned long    biSize;            //本结构所占用字节数 40字节
		long             biWidth;           //位图的宽度，以像素为单位
		long             biHeight;          //位图的高度，以像素为单位
		unsigned short   biPlanes;          //目标设备的级别，必须为1
		unsigned short   biBitCount;        //每个像素所需的位数，必须是1（双色）、
											//4（16色）、8（256色）或24（真彩色）之一
		unsigned long    biCompression;     //位图压缩类型，必须是 0（BI_RGB不压缩）、
											//1（BI_RLE8压缩类型）
											//2（BI_RLE压缩类型）之一
		unsigned long    biSizeImage;       //位图的大小，以字节为单位
		long             biXPelsPerMeter;   //位图水平分辨率，每米像素数
		long             biYPelsPerMeter;   //位图垂直分辨率，每米像素数
		unsigned long    biClrUsed;         //位图实际使用的颜色表中的颜色数
		unsigned long    biClrImportant;    //位图显示过程中重要的颜色数 
	};
	typedef struct tagRGBQUAD {
		unsigned char    rgbBlue;		      //蓝色的亮度（值范围为0～255）
		unsigned char    rgbGreen;		      //绿色的亮度（值范围为0～255）
		unsigned char    rgbRed;			      //红色的亮度（值范围为0～255）
		unsigned char    rgbReserved;	      //保留，必须为0
	} RGBQUAD;

	class Image{
	public:
		virtual ~Image() {}
		static Image* ReadImage(const char*);//读取图片
		virtual void WriteImage(const char*) const = 0;//储存图片
		virtual const RGBQUAD& GetPixel(int, int) const = 0;//获取像素
		virtual const RGBQUAD& GetPixel(int) const = 0;//获取像素
		virtual void SetPixel(int, int, const RGBQUAD&) = 0;//设置像素
		virtual void SetPixel(int, const RGBQUAD&) = 0;//设置像素
		virtual int GetHeight() const = 0;//图像高度
		virtual int GetWidth() const = 0;//图像长度
		virtual void Resize(int, int) = 0;//放大缩小
		virtual void Crop(int x1, int y1, int x2, int y2) = 0;//裁剪点(x1,y1)到点(x2,y2)
		virtual void Rotate(int degree) = 0;//旋转，90度的整数倍
		virtual void Transpose() = 0; // 转置
		virtual void Flip(bool) = 0;//翻转，true：上下翻转 false：左右翻转
		virtual void Cat(Image& m, int code) = 0; // 将m与当前对象进行拼接，code代表拼接的方式
		virtual Image* Clone() const = 0; // 从当前对象拷贝创建一个新的矩阵，完成深拷贝
		virtual QImage ToQImage() const = 0;//转换为QImage
		virtual RGBQUAD* GetBits() const = 0;//获取一维像素数组
		virtual Image& operator=(const Image&) = 0;//深拷贝
		virtual Image& ToGrayScale();
	protected:
		void InvokePixelChanged(int v) {
			PixelChanged(v);
		}
	public:
		MyEvent<Image, int> PixelChanged;
	};

	class BitMap_32 : public Image {
	public:
		BitMap_32(int, int);
		BitMap_32(const RGBQUAD*, int, int);
		BitMap_32(int, int, const RGBQUAD&);
		BitMap_32(const BitMap_32&);
		~BitMap_32();
		// 通过 Image 继承
		virtual void WriteImage(const char*) const override;
		virtual const RGBQUAD& GetPixel(int, int) const override;
		virtual const RGBQUAD& GetPixel(int) const override;
		virtual void SetPixel(int, int, const RGBQUAD&) override;
		virtual void SetPixel(int, const RGBQUAD&) override;
		virtual void Resize(int, int) override;
		virtual void Crop(int x1, int y1, int x2, int y2) override;
		virtual void Rotate(int degree) override;
		virtual void Transpose() override;
		virtual void Flip(bool) override;
		virtual void Cat(Image& m, int code) override;
		virtual Image* Clone() const override;
		virtual QImage ToQImage() const override;
		virtual Image& operator=(const Image&) override;
	private:
		Mat<RGBQUAD>* _data;

		// 通过 Image 继承
		virtual int GetHeight() const override;
		virtual int GetWidth() const override;

		// 通过 Image 继承
		virtual RGBQUAD* GetBits() const override;

	};
}

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