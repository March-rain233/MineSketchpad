#pragma once
#include<qimage.h>
#include"Mat.hpp"
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
		static Image* ReadImage(const char*);//读取图片
		virtual void WriteImage(const char*) const = 0;//储存图片
		virtual RGBQUAD GetPixel(int, int) const = 0;//获取像素
		virtual void SetPixel(int, int, RGBQUAD) = 0;//设置像素
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
		virtual const RGBQUAD* GetBits() const = 0;//获取一维像素数组
		virtual Image& operator=(const Image&) = 0;//深拷贝
		virtual const Image& ToGrayScale();
	};

	class BitMap_32 : public Image {
	public:
		BitMap_32(int, int);
		BitMap_32(const RGBQUAD*, int, int);
		BitMap_32(int, int, const RGBQUAD&);
		// 通过 Image 继承
		virtual void WriteImage(const char*) const override;
		virtual RGBQUAD GetPixel(int, int) const override;
		virtual void SetPixel(int, int, RGBQUAD) override;
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
		virtual const RGBQUAD* GetBits() const override;

	};
}

