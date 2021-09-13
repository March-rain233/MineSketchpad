#include"Image.h"
#include<fstream>
#include<qdebug>
using namespace MyImage;

const Image& MyImage::Image::ToGrayScale() {
	BitMap_32 res(GetHeight(), GetWidth());
	for (int i = 0; i < GetHeight(); ++i) {
		for (int j = 0; j < GetWidth(); ++j) {
			RGBQUAD temp = GetPixel(i, j);
			temp.rgbBlue = temp.rgbGreen = temp.rgbRed = 
				(temp.rgbRed * 299 + temp.rgbGreen * 587 + temp.rgbBlue * 114) / 1000;
			res.SetPixel(i, j, temp);
		}
	}
	return res;
}

Image* MyImage::Image::ReadImage(const char* filename) {

	//读取文件头和信息头
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
	_data = new Mat<RGBQUAD>(h, w);
	memset(_data->GetBits(), 0, h * w * 4);
}

MyImage::BitMap_32::BitMap_32(const RGBQUAD* m, int h, int w) {
	_data = new Mat<RGBQUAD>(h, w);
	memcpy(_data->GetBits(), m, h * w * 4);
}

MyImage::BitMap_32::BitMap_32(int h, int w, const RGBQUAD& v) {
	_data = new Mat<RGBQUAD>(h, w);
	memset(_data->GetBits(), *reinterpret_cast<const int*>(&v), h * w * 4);
}

MyImage::BitMap_32::BitMap_32(const BitMap_32& res) {
	_data = new Mat<RGBQUAD>(res.GetHeight(), res.GetWidth());
	memcpy(_data->GetBits(), res.GetBits(), res.GetHeight() * res.GetWidth() * 4);
}

MyImage::BitMap_32::~BitMap_32() {
	qDebug() << "调用析构";
	delete _data;
}

void MyImage::BitMap_32::WriteImage(const char* filename) const {
	std::fstream bmp(filename, std::ios::binary | std::ios::out);
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;

	//定义信息头
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

	//定义文件头
	bf.bfType = 0X4D42;
	bf.bfReserved1 = bf.bfReserved2 = 0;
	bf.bfOffBits = 14 + 40;
	bf.bfSize = bf.bfOffBits + bi.biSizeImage;

	bmp.write((char*)&bf, sizeof(BITMAPFILEHEADER));
	bmp.write((char*)&bi, sizeof(BITMAPINFOHEADER));
	//数据写入
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

const RGBQUAD& MyImage::BitMap_32::GetPixel(int col, int row) const {
	return _data->At(col, row);
}

const RGBQUAD& MyImage::BitMap_32::GetPixel(int i) const {
	return _data->At(i);
}

void MyImage::BitMap_32::SetPixel(int col, int row, const RGBQUAD& val) {
	memcpy(&_data->At(col, row), &val, 4);
	InvokePixelChanged(row * _data->Width() + col);
}

void MyImage::BitMap_32::SetPixel(int i, const RGBQUAD& val) {
	memcpy(&_data->At(i), &val, 4);
	InvokePixelChanged(i);
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

RGBQUAD* MyImage::BitMap_32::GetBits() const {
	return _data->GetBits();
}

void MyImage::BitMap_32::Flip(bool code) {
	_data->Flip(code);
}

