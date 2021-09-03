#include"Image.h"
#include<fstream>
using namespace MyImage;
Image* MyImage::Image::ReadImage(const char* filename) {

	//��ȡ�ļ�ͷ����Ϣͷ
	std::fstream bmp(filename, std::ios::binary | std::ios::in);
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	bmp.read((char*)&bf, sizeof(BITMAPFILEHEADER));
	bmp.read((char*)&bi, sizeof(BITMAPINFOHEADER));
	Image* res;
	switch (bi.biBitCount) {
	case 2:
		break;
	case 4:
		break;
	case 8:
		break;
	case 24:
		break;
	case 32:
		res = new BitMap_32(bi.biHeight, bi.biWidth);
		int total = bi.biWidth * bi.biHeight;
		for (int i = 0; i < total; i++) {
			unsigned char buffer[4];
			bmp.read((char*)&buffer, 4);
			RGBQUAD t;
			t.rgbBlue = buffer[0];
			t.rgbGreen = buffer[1];
			t.rgbRed = buffer[2];
			t.rgbReserved = buffer[3];
			res->SetPixel(i / bi.biWidth, i % bi.biWidth, t);
		}
		break;
	}
	res->Flip(true);
	bmp.close();
	return res;
}

MyImage::BitMap_32::BitMap_32(int h, int w) {
	RGBQUAD default = { 255,255,255,255 };
	_data = new Mat<RGBQUAD>(h, w, default);
}

MyImage::BitMap_32::BitMap_32(const RGBQUAD* m, int h, int w) {
	_data = new Mat<RGBQUAD>(m, h, w);
}

MyImage::BitMap_32::BitMap_32(int h, int w, const RGBQUAD& v) {
	_data = new Mat<RGBQUAD>(h, w, v);
}

void MyImage::BitMap_32::WriteImage(const char* filename) const {
	std::fstream bmp(filename, std::ios::binary | std::ios::out);
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;

	//������Ϣͷ
	bi.biWidth = _data->Width();
	bi.biHeight = _data->Height();
	bi.biBitCount = 32;
	bi.biSize = 40;
	bi.biPlanes = 1;
	bi.biCompression = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biSizeImage = (bi.biWidth * bi.biBitCount + 31) / 32 * 4 * bi.biHeight;

	//�����ļ�ͷ
	bf.bfType = 0X4D42;
	bf.bfReserved1 = bf.bfReserved2 = 0;
	bf.bfOffBits = 14 + 40;
	bf.bfSize = bf.bfOffBits + bi.biSizeImage;

	bmp.write((char*)&bf, sizeof(BITMAPFILEHEADER));
	bmp.write((char*)&bi, sizeof(BITMAPINFOHEADER));
	//����д��
	char temp = 0;
	int total = _data->Width() * _data->Height();
	for (int i = 0; i < total; i++) {
		unsigned char buffer[4];
		buffer[0] = _data->At(i).rgbBlue;
		buffer[1] = _data->At(i).rgbGreen;
		buffer[2] = _data->At(i).rgbRed;
		buffer[3] = _data->At(i).rgbReserved;
		bmp.write((char*)buffer, 4);
	}
	bmp.close();
}

RGBQUAD MyImage::BitMap_32::GetPixel(int col, int row) const {
	return _data->At(col, row);
}

void MyImage::BitMap_32::SetPixel(int col, int row, RGBQUAD val) {
	_data->Set(col, row, val);
}

void MyImage::BitMap_32::Resize(int h, int w) {
	_data->Resize(h, w);
}

void MyImage::BitMap_32::Crop(int x1, int y1, int x2, int y2) {
	_data->Crop(x1, y1, x2, y2);
}

void MyImage::BitMap_32::Rotate(int degree) {
	_data->Rotate(degree);
}

void MyImage::BitMap_32::Transpose() {
	_data->Transpose();
}

void MyImage::BitMap_32::Cat(Image& m, int code) {
	Mat<RGBQUAD> t(m.GetBits(), m.GetHeight(), m.GetWidth());
	_data->Cat(t, code);
}

Image* MyImage::BitMap_32::Clone() const {
	BitMap_32* r = new BitMap_32(this->GetBits(), this->GetHeight(),this->GetWidth());
	return r;
}

QImage MyImage::BitMap_32::ToQImage() const {
	QImage res(_data->Width(), _data->Height(), QImage::Format_RGBA8888);
	memcpy(res.bits(), _data->GetBits(), _data->Width() * _data->Height() * 4);
	return res.rgbSwapped();
}

Image& MyImage::BitMap_32::operator=(const Image& res) {
	delete _data;
	_data = new Mat<RGBQUAD>(res.GetBits(), res.GetHeight(), res.GetWidth());
	return *this;
}

int MyImage::BitMap_32::GetHeight() const {
	return _data->Height();
}

int MyImage::BitMap_32::GetWidth() const {
	return _data->Width();
}

const RGBQUAD* MyImage::BitMap_32::GetBits() const {
	return _data->GetBits();
}

void MyImage::BitMap_32::Flip(bool code) {
	_data->Flip(code);
}
