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

void LinearFilter::FliterImage(MyImage::Image& m) {
	int h = m.GetHeight();
	int w = m.GetWidth();
	auto check = [](double& v)->unsigned char {
		if (v > 255) {
			v = 255;
		}
		else if (v < 0) {
			v = 0;
		}
		return v;
	};
	auto t = new MyImage::RGBQUAD[h * w];
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			double r = 0;
			double g = 0;
			double b = 0;
			unsigned char temp = 0;
			for (int i = x - _radius, k = 0; i <= x + _radius; ++i) {
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
			t[y * w + x] = MyImage::RGBQUAD{ check(b), check(g), check(r), m.GetPixel(x,y).rgbReserved };
		}
	}
	memcpy(m.GetBits(), t, h * w * 4);
	delete t;
	//auto t = m.GetBits();
	//int kr = _radius * 2 + 1;
	//double* tempR = new double[w * h];
	//double* tempG = new double[w * h];
	//double* tempB = new double[w * h];
	//for (int i = 0; i < h; ++i) {
	//	for (int j = 0; j < w; ++j) {
	//		int up, down, left, right;
	//		//每一行第一个元素完整计算
	//		if (j == 0) {
	//			tempR[i * w] = tempG[i * w] = tempB[i * w] = 0;
	//			for (int ik = 0; ik < kr; ++ik) {
	//				for (int jk = 0; jk < kr; ++jk) {
	//					if (i - (_radius - ik) < 0 || i - (_radius - ik) >= h ||
	//						j - (_radius - jk) < 0 || j - (_radius - jk) >= w) {
	//						continue;
	//					}
	//					tempR[i * w] = t[(i - (_radius - ik)) * w + j - (_radius - jk)].rgbRed * _kernel[ik * w + jk];
	//					tempG[i * w] = t[(i - (_radius - ik)) * w + j - (_radius - jk)].rgbGreen * _kernel[ik * w + jk];
	//					tempB[i * w] = t[(i - (_radius - ik)) * w + j - (_radius - jk)].rgbBlue * _kernel[ik * w + jk];
	//				}
	//			}
	//		}
	//		else {//前缀和
	//			tempR[i * w + j] = tempR[i * w + j - 1];
	//			tempG[i * w + j] = tempG[i * w + j - 1];
	//			tempB[i * w + j] = tempB[i * w + j - 1];
	//			//减去原先的值
	//			if (j - _radius - 1 > 0) {
	//				for (int ik = 0; ik < kr; ++ik) {
	//					if (i - (_radius - ik) < 0 || i - (_radius - ik) >= h) {
	//						continue;
	//					}
	//					tempR[i * w + j] -= t[(i - (_radius - ik)) * w + j - _radius].rgbRed * _kernel[ik * w];
	//					tempG[i * w + j] -= t[(i - (_radius - ik)) * w + j - _radius].rgbGreen * _kernel[ik * w];
	//					tempB[i * w + j] -= t[(i - (_radius - ik)) * w + j - _radius].rgbBlue * _kernel[ik * w];
	//				}
	//			}
	//			//加上新加的值
	//			if (j + _radius + 1 > w) {
	//				for (int ik = 0; ik < kr; ++ik) {
	//					if (i - (_radius - ik) < 0 || i - (_radius - ik) >= h) {
	//						continue;
	//					}
	//					tempR[i * w + j] += t[(i - (_radius - ik)) * w + j - _radius].rgbRed * _kernel[ik * w];
	//					tempG[i * w + j] += t[(i - (_radius - ik)) * w + j - _radius].rgbGreen * _kernel[ik * w];
	//					tempB[i * w + j] += t[(i - (_radius - ik)) * w + j - _radius].rgbBlue * _kernel[ik * w];

	//				}
	//			}
	//		}
	//	}
	//}
	//int total = h * w;
	//auto check = [](double v)->unsigned char {
	//	if (v > 255) {
	//		return 255;
	//	}
	//	if (v < 0) {
	//		return 0;
	//	}
	//	return v;
	//};
	//for (int i = 0; i < total; ++i) {
	//	t[i].rgbRed = check(tempR[i]);
	//	t[i].rgbGreen = check(tempG[i]);
	//	t[i].rgbBlue = check(tempB[i]);
	//}
	//delete[] tempR;
	//delete[] tempG;
	//delete[] tempB;
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
				continue;
			}
			auto c = m.GetPixel(i, j);
			sort(r, c.rgbRed, k);
			sort(g, c.rgbGreen, k);
			sort(b, c.rgbBlue, k);
		}
	}
	return MyImage::RGBQUAD{ b[total / 2], g[total / 2], r[total / 2], m.GetPixel(x,y).rgbReserved };
}

void MedianFilter::FliterImage(MyImage::Image& m) {
	int h = m.GetHeight();
	int w = m.GetWidth();
	int total = (_radius * 2 + 1) * (_radius * 2 + 1);
	auto check = [](double& v)->unsigned char {
		if (v > 255) {
			v = 255;
		}
		else if (v < 0) {
			v = 0;
		}
		return v;
	};
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
	auto t = new MyImage::RGBQUAD[h * w];
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			unsigned char* r = new unsigned char[total];
			unsigned char* g = new unsigned char[total];
			unsigned char* b = new unsigned char[total];
			for (int i = x - _radius, k = 0; i <= x + _radius; ++i) {
				for (int j = y - _radius; j <= y + _radius; ++j, ++k) {
					if (i < 0 || i >= w || j < 0 || j >= h) {
						sort(r, 0, k);
						sort(g, 0, k);
						sort(b, 0, k);
						continue;
					}
					auto c = m.GetPixel(i, j);
					sort(r, c.rgbRed, k);
					sort(g, c.rgbGreen, k);
					sort(b, c.rgbBlue, k);
				}
			}
			t[y * w + x] = MyImage::RGBQUAD{ b[total / 2], g[total / 2], r[total / 2], m.GetPixel(x,y).rgbReserved };
		}
	}
	memcpy(m.GetBits(), t, h * w * 4);
	delete t;
}


