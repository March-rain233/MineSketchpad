#include "ImageFilter.h"

MyImage::RGBQUAD LinearFilter::FliterPixel(MyImage::Image& m, int x, int y) {
	int w = m.GetWidth();
	int h = m.GetHeight();
	double r = 0;
	double g = 0;
	double b = 0;
	unsigned char temp = 0;
	auto check = [](double& v)->unsigned char {
		if (v > 255) {
			v = 255;
		}
		else if (v < 0) {
			v = 0;
		}
		return v;
	};
	for(int i = x - _radius, k = 0; i <= x + _radius; ++i){
		for (int j = y - _radius; j <= y + _radius; ++j, ++k) {
			if (i < 0 || i >= w || j < 0 || j >= h) {
				continue;
			}
			auto c = m.GetPixel(i, j);
			r += c.rgbRed * _kernel[k];
			g += c.rgbGreen * _kernel[k];
			b += c.rgbBlue * _kernel[k];
		}
	}
	return MyImage::RGBQUAD{ check(b), check(g), check(r), m.GetPixel(x,y).rgbReserved };
}

void LinearFilter::SetKernel(const double* kernel, int radius) {
	if (_kernel) {
		delete _kernel;
	}
	_radius = radius;
	_kernel = new double[(_radius * 2 + 1) * (_radius * 2 + 1)];
	memcpy(_kernel, kernel, 8 * (_radius * 2 + 1) * (_radius * 2 + 1));
}

double* LinearFilter::GetKernel() {
	return _kernel;
}

int ImageFilter::GetRadius() {
	return _radius;
}

void ImageFilter::SetRadius(int v) {
	_radius = v;
}

MedianFilter::MedianFilter(int r) {
	_radius = r;
}

MyImage::RGBQUAD MedianFilter::FliterPixel(MyImage::Image& m, int x, int y) {
	int total = (_radius * 2 + 1) * (_radius * 2 + 1);
	int w = m.GetWidth();
	int h = m.GetHeight();
	unsigned char* r = new unsigned char[total];
	unsigned char* g = new unsigned char[total];
	unsigned char* b = new unsigned char[total];
	auto sort = [](unsigned char* list, unsigned char v, int len) {
		if (len == 0) {
			list[0] = v;
			return;
		}
		int i = len - 1;
		while (i >= 0 && list[i] > v) {
			list[i + 1] = list[i];
			--i;
		}
		list[i + 1] = v;
	};
	for (int i = x - _radius, k = 0; i <= x + _radius; ++i) {
		for (int j = y - _radius; j <= y + _radius; ++j, ++k) {
			if (i < 0 || i >= w || j < 0 || j >= h) {
				sort(r, 0, k);
				sort(g, 0, k);
				sort(b, 0, k);
			}
			auto c = m.GetPixel(i, j);
			sort(r, c.rgbRed, k);
			sort(g, c.rgbGreen, k);
			sort(b, c.rgbBlue, k);
		}
	}
	return MyImage::RGBQUAD{ b[total / 2], g[total / 2], r[total / 2], m.GetPixel(x,y).rgbReserved };
}


