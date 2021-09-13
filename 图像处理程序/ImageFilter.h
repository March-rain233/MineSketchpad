#pragma once
#include "Image.h"
class ImageFilter {
public:
	virtual MyImage::RGBQUAD FliterPixel(MyImage::Image& m, int x, int y) = 0;
};

class LinearFilter :
	public ImageFilter{
public:
	virtual MyImage::RGBQUAD FliterPixel(MyImage::Image& m, int x, int y) override;
	void SetKernel(const double* kernel, int radius);
	double* GetKernel();
	int GetRadius();
private:
	double* _kernel;
	int _radius;
};