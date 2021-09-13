#pragma once
#include "ImageFilter.h"
#include "qmap.h"
#include <functional>

class MyEnum : public QObject {
    Q_OBJECT
public:
	enum class FilterType {
		Normal,
		Median,
		Mean,
		Guassian,
		Sharpen,
		EdgeDetection,
		Embossing,
		MotionBlur
	};

    Q_ENUM(FilterType)
};

typedef MyEnum::FilterType FilterType;

class KernelFactory {
public:
	typedef std::function<void(double* buffer, int radius)> CreateHandler;
	static void Create(double* buffer, int radius, FilterType mode);
	static QMap<FilterType, CreateHandler> CreateMap;
};
class FilterFactory {
public:
	static ImageFilter* Create(int radius, FilterType = FilterType::Mean);
};

