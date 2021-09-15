#pragma once
#include "Image.h"
class ImageFilter {
public:
	virtual MyImage::RGBQUAD FliterPixel(MyImage::Image& m, int x, int y) = 0;
	virtual void FliterImage(MyImage::Image& m) = 0;
	int GetRadius();
	void SetRadius(int);
protected:
	int _radius;
};

class LinearFilter :
	public ImageFilter{
public:
	virtual MyImage::RGBQUAD FliterPixel(MyImage::Image& m, int x, int y) override;
	void SetKernel(const double* kernel, int radius);
	double* GetKernel();
private:
	double* _kernel = nullptr;

	// ͨ�� ImageFilter �̳�
	virtual void FliterImage(MyImage::Image& m) override;
};

class MedianFilter :
	public ImageFilter {
public:
	MedianFilter(int);
	virtual MyImage::RGBQUAD FliterPixel(MyImage::Image& m, int x, int y) override;

	// ͨ�� ImageFilter �̳�
	virtual void FliterImage(MyImage::Image& m) override;
};