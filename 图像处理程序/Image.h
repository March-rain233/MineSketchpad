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
		unsigned long    biSize;            //���ṹ��ռ���ֽ��� 40�ֽ�
		long             biWidth;           //λͼ�Ŀ�ȣ�������Ϊ��λ
		long             biHeight;          //λͼ�ĸ߶ȣ�������Ϊ��λ
		unsigned short   biPlanes;          //Ŀ���豸�ļ��𣬱���Ϊ1
		unsigned short   biBitCount;        //ÿ�����������λ����������1��˫ɫ����
											//4��16ɫ����8��256ɫ����24�����ɫ��֮һ
		unsigned long    biCompression;     //λͼѹ�����ͣ������� 0��BI_RGB��ѹ������
											//1��BI_RLE8ѹ�����ͣ�
											//2��BI_RLEѹ�����ͣ�֮һ
		unsigned long    biSizeImage;       //λͼ�Ĵ�С�����ֽ�Ϊ��λ
		long             biXPelsPerMeter;   //λͼˮƽ�ֱ��ʣ�ÿ��������
		long             biYPelsPerMeter;   //λͼ��ֱ�ֱ��ʣ�ÿ��������
		unsigned long    biClrUsed;         //λͼʵ��ʹ�õ���ɫ���е���ɫ��
		unsigned long    biClrImportant;    //λͼ��ʾ��������Ҫ����ɫ�� 
	};
	typedef struct tagRGBQUAD {
		unsigned char    rgbBlue;		      //��ɫ�����ȣ�ֵ��ΧΪ0��255��
		unsigned char    rgbGreen;		      //��ɫ�����ȣ�ֵ��ΧΪ0��255��
		unsigned char    rgbRed;			      //��ɫ�����ȣ�ֵ��ΧΪ0��255��
		unsigned char    rgbReserved;	      //����������Ϊ0
	} RGBQUAD;

	class Image{
	public:
		virtual ~Image() {}
		static Image* ReadImage(const char*);//��ȡͼƬ
		virtual void WriteImage(const char*) const = 0;//����ͼƬ
		virtual const RGBQUAD& GetPixel(int, int) const = 0;//��ȡ����
		virtual const RGBQUAD& GetPixel(int) const = 0;//��ȡ����
		virtual void SetPixel(int, int, const RGBQUAD&) = 0;//��������
		virtual void SetPixel(int, const RGBQUAD&) = 0;//��������
		virtual int GetHeight() const = 0;//ͼ��߶�
		virtual int GetWidth() const = 0;//ͼ�񳤶�
		virtual void Resize(int, int) = 0;//�Ŵ���С
		virtual void Crop(int x1, int y1, int x2, int y2) = 0;//�ü���(x1,y1)����(x2,y2)
		virtual void Rotate(int degree) = 0;//��ת��90�ȵ�������
		virtual void Transpose() = 0; // ת��
		virtual void Flip(bool) = 0;//��ת��true�����·�ת false�����ҷ�ת
		virtual void Cat(Image& m, int code) = 0; // ��m�뵱ǰ�������ƴ�ӣ�code����ƴ�ӵķ�ʽ
		virtual Image* Clone() const = 0; // �ӵ�ǰ���󿽱�����һ���µľ���������
		virtual QImage ToQImage() const = 0;//ת��ΪQImage
		virtual RGBQUAD* GetBits() const = 0;//��ȡһά��������
		virtual Image& operator=(const Image&) = 0;//���
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
		// ͨ�� Image �̳�
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

		// ͨ�� Image �̳�
		virtual int GetHeight() const override;
		virtual int GetWidth() const override;

		// ͨ�� Image �̳�
		virtual RGBQUAD* GetBits() const override;

	};
}

inline void RGBToHSL(MyImage::RGBQUAD color, double& H, double& S, double& L) {
	double R, G, B, Max, Min, del_R, del_G, del_B, del_Max;
	R = color.rgbRed / 255.0;       //Where RGB values = 0 �� 255
	G = color.rgbGreen / 255.0;
	B = color.rgbBlue / 255.0;

	using namespace std;
	Min = min(R, min(G, B));    //Min. value of RGB
	Max = max(R, max(G, B));    //Max. value of RGB
	del_Max = Max - Min;        //Delta RGB value

	L = (Max + Min) / 2.0;

	if (del_Max == 0)           //This is a gray, no chroma...
	{
		//H = 2.0/3.0;          //Windows��SֵΪ0ʱ��Hֵʼ��Ϊ160��2/3*240��
		H = 0;                  //HSL results = 0 �� 1
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
	if (S == 0)                       //HSL values = 0 �� 1
	{
		R = L * 255.0;                   //RGB results = 0 �� 255
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