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
		static Image* ReadImage(const char*);//��ȡͼƬ
		virtual void WriteImage(const char*) const = 0;//����ͼƬ
		virtual RGBQUAD GetPixel(int, int) const = 0;//��ȡ����
		virtual void SetPixel(int, int, RGBQUAD) = 0;//��������
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
		virtual const RGBQUAD* GetBits() const = 0;//��ȡһά��������
		virtual Image& operator=(const Image&) = 0;//���
		virtual const Image& ToGrayScale();
	};

	class BitMap_32 : public Image {
	public:
		BitMap_32(int, int);
		BitMap_32(const RGBQUAD*, int, int);
		BitMap_32(int, int, const RGBQUAD&);
		// ͨ�� Image �̳�
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

		// ͨ�� Image �̳�
		virtual int GetHeight() const override;
		virtual int GetWidth() const override;

		// ͨ�� Image �̳�
		virtual const RGBQUAD* GetBits() const override;

	};
}

