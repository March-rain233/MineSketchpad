#include "FilterFactory.h"

void NormalHandler(double* buffer, int radius) {
	memset(buffer, 0, sizeof(double) * pow(radius * 2 + 1, 2));
	buffer[(radius * 2 + 1) * (radius * 2 + 1) / 2] = 1;
}

void MeanHandler(double* buffer, int radius) {
	qFill(buffer, buffer + (radius * 2 + 1) * (radius * 2 + 1),
		1.0 / ((radius * 2 + 1) * (radius * 2 + 1)));
}

void GuassianHandler(double* buffer, int radius) {
	const double PI = 4.0 * atan(1.0); //‘≤÷‹¬ ¶–∏≥÷µ
	int sigma = 1;
	int size = (radius * 2 + 1);
	int center = size / 2;
	double sum = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			buffer[i * size + j] = (1 / (2 * PI * sigma * sigma)) * exp(-((i - center) * (i - center) + (j - center) * (j - center)) / (2 * sigma * sigma));
			sum += buffer[i * size + j];
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			buffer[i * size + j] /= sum;
		}
	}
}

void SharpenHandler(double* buffer, int radius) {
	qFill(buffer, buffer + (radius * 2 + 1) * (radius * 2 + 1), -1 * 1);
	buffer[(radius * 2 + 1) * (radius * 2 + 1) / 2] =
		((radius * 2 + 1) * (radius * 2 + 1) - 1) * 1 + 1;
}

void EdgeDetectionHandler(double* buffer, int radius) {
	qFill(buffer, buffer + (radius * 2 + 1) * (radius * 2 + 1), -1 * 1);
	buffer[(radius * 2 + 1) * (radius * 2 + 1) / 2] =
		((radius * 2 + 1) * (radius * 2 + 1) - 1) * 1;
}

void MotionBlurHandler(double* buffer, int radius) {
	int w = radius * 2 + 1;
	double v = 1 / w;
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < w; ++j) {
			if (i == j) {
				buffer[i * w + j] = v;
			}
			else {
				buffer[i * w + j] = 0;
			}
		}
	}
	buffer[radius * w + radius] = 0;
}

void EmbossingHandler(double* buffer, int radius) {
	int w = radius * 2 + 1;
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < w; ++j) {
			if (i <= radius && j <= radius) {
				buffer[i * w + j] = 1;
			}
			else if (i >= radius && j >= radius) {
				buffer[i * w + j] = 1;
			}
			else {
				buffer[i * w + j] = 0;
			}
		}
	}
	buffer[radius * w + radius] = 0;
}

QMap<FilterType, KernelFactory::CreateHandler> KernelFactory::CreateMap {
	{FilterType::Normal, NormalHandler},
	{FilterType::Mean, MeanHandler},
	{FilterType::Guassian, GuassianHandler},
	{FilterType::Sharpen, SharpenHandler},
	{FilterType::EdgeDetection, EdgeDetectionHandler},
	{FilterType::Embossing, EmbossingHandler},
	{FilterType::MotionBlur, MotionBlurHandler},
};

void KernelFactory::Create(double* buffer, int radius, FilterType mode) {
	CreateMap[mode](buffer, radius);
}

ImageFilter* FilterFactory::Create(int radius, FilterType mode) {
	switch (mode) {
	case FilterType::Median:
		return new MedianFilter(radius);
	default:
		LinearFilter* lf = new LinearFilter();
		lf->SetKernel(new double[(radius * 2 + 1) * (radius * 2 + 1)], radius);
		KernelFactory::Create(lf->GetKernel(), radius, mode);
		return lf;
	}
}
