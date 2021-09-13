#include "ImageFilter.h"

MyImage::RGBQUAD LinearFilter::FliterPixel(MyImage::Image& m, int x, int y) {
	int x1 = x - _radius;
	int x2 = x + _radius;
	int y1 = y - _radius;
	int y2 = y + _radius;
	int w = m.GetWidth();
	int h = m.GetHeight();
	double r = 0;
	double g = 0;
	double b = 0;
	unsigned char temp = 0;
	auto check = [](double v)->unsigned char {
		if (v > 255) {
			v = 255;
		}
		else if (v < 0) {
			v = 0;
		}
		return v;
	};
	for(int i = x1, k = 0; i <= x2; ++i){
		for (int j = y1; j <= y2; ++j, ++k) {
			if (i < 0 || i >= w || j < 0 || j >= h) {
				continue;
			}
			auto c = m.GetPixel(i, j);
			r += c.rgbRed * _kernel[k];
			g += c.rgbGreen * _kernel[k];
			b += c.rgbBlue * _kernel[k];
		}
	}
	return MyImage::RGBQUAD{ check(b), check(g), check(b), m.GetPixel(x,y).rgbReserved };
}

void LinearFilter::SetKernel(const double* kernel, int radius) {
	_radius = radius;
	_kernel = new double[(_radius * 2 + 1) * (_radius * 2 + 1)];
	memcpy(_kernel, kernel, 8 * (_radius * 2 + 1) * (_radius * 2 + 1));
}

double* LinearFilter::GetKernel() {
	return _kernel;
}

int LinearFilter::GetRadius() {
	return _radius;
}
