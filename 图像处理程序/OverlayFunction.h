#pragma once
#include<functional>
#include"Image.h"
#include<qdebug.h>
enum class OverlayMode {
	Normal,
	Darken,
	Multiply,
	ColorBurn,
	LinearBurn,
	DarkenColor,
	Lighten,
	Screen,
	ColorDodge,
	LightenColor,
	Add,
	Overlay,
	SoftLight,
	HardLight,
	VividLight,
	LinearLight,
	PinLight,
	HardMix,
	Difference,
	Exclusion,
	Subtract,
	Divide,
};

inline MyImage::RGBQUAD NormalOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	//auto blend = [](float a1, float a2, float c1, float c2) {
	//	return (c1 * a1 * (1.0 - a2) + c2 * a2) / (a1 + a2 - a1 * a2);
	//};

	//float fAlp1 = front.rgbReserved / 255.0;
	//float fAlp2 = back.rgbReserved / 255.0;
	//float fAlpBlend = fAlp1 + fAlp2 - fAlp1 * fAlp2;

	//float fRed1 = front.rgbRed / 255.0;
	//float fRed2 = back.rgbRed / 255.0;
	//float fRedBlend = blend(fAlp1, fAlp2, fRed1, fRed2);

	//float fGreen1 = front.rgbGreen / 255.0;
	//float fGreen2 = back.rgbGreen / 255.0;
	//float fGreenBlend = blend(fAlp1, fAlp2, fGreen1, fGreen2);

	//float fBlue1 = front.rgbBlue / 255.0;
	//float fBlue2 = back.rgbBlue / 255.0;
	//float fBlueBlend = blend(fAlp1, fAlp2, fBlue1, fBlue2);

	//res.rgbRed = fRedBlend * 255;
	//res.rgbGreen = fGreenBlend * 255;
	//res.rgbBlue = fBlueBlend * 255;
	//res.rgbReserved = fAlpBlend * 255;
	if (front.rgbReserved == 0) {
		res = back;
		res.rgbReserved = alpha;
		return res;
	}
	if (back.rgbReserved == 0) {
		res = front;
		res.rgbReserved = alpha;
		return res;
	}
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	res.rgbRed = blend(back.rgbRed, front.rgbRed);
	res.rgbGreen = blend(back.rgbGreen, front.rgbGreen);
	res.rgbBlue = blend(back.rgbBlue, front.rgbBlue);
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
};
inline MyImage::RGBQUAD DarkenOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	res.rgbBlue = std::min(back.rgbBlue, front.rgbBlue);
	res.rgbGreen = std::min(back.rgbGreen, front.rgbGreen);
	res.rgbRed = std::min(back.rgbRed, front.rgbRed);
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}
inline MyImage::RGBQUAD MultiplyOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	res.rgbBlue = back.rgbBlue * front.rgbBlue / 255.0;
	res.rgbGreen = back.rgbGreen * front.rgbGreen / 255.0;
	res.rgbRed = back.rgbRed * front.rgbRed / 255.0;
	return res;
}
inline MyImage::RGBQUAD ColorBurnOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	auto b2 = [](unsigned char b, unsigned char f)->unsigned char {
		return f == 0 ? f : std::max(0, std::max(0, (255 - ((255 - b) << 8) / f)));
	};
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	res.rgbRed = b2(back.rgbRed, front.rgbRed);
	res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
	res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
	return res;
}
inline MyImage::RGBQUAD LinearBurnOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
		return (A + B < 255) ? 0 : (A + B - 255);
	};
	res.rgbRed = b2(back.rgbRed, front.rgbRed);
	res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
	res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}
inline MyImage::RGBQUAD DarkenColorOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	int f = front.rgbBlue + front.rgbGreen + front.rgbRed;
	int b = back.rgbBlue + back.rgbGreen + back.rgbRed;
	if (f > b) {
		res = back;
	}
	else {
		res = front;
	}
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}
inline MyImage::RGBQUAD LightenOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	res.rgbBlue = std::max(back.rgbBlue, front.rgbBlue);
	res.rgbGreen = std::max(back.rgbGreen, front.rgbGreen);
	res.rgbRed = std::max(back.rgbRed, front.rgbRed);
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}
inline MyImage::RGBQUAD ScreenOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
		return 255 - (((255 - A) * (255 - B)) >> 8);
	};
	res.rgbRed = b2(back.rgbRed, front.rgbRed);
	res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
	res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}
inline MyImage::RGBQUAD ColorDodgeOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
		return (B == 255) ? B : std::min(255, ((A << 8) / (255-B)));
	};
	res.rgbRed = b2(back.rgbRed, front.rgbRed);
	res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
	res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}
inline MyImage::RGBQUAD AddOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
		return std::min(A + B, 255);
	};
	res.rgbRed = b2(back.rgbRed, front.rgbRed);
	res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
	res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}
inline MyImage::RGBQUAD LightenColorOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	int f = front.rgbBlue + front.rgbGreen + front.rgbRed;
	int b = back.rgbBlue + back.rgbGreen + back.rgbRed;
	if (f < b) {
		res = back;
	}
	else {
		res = front;
	}
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}
inline MyImage::RGBQUAD OverlayOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
		return std::min(A + B, 255);
	};
	res.rgbRed = b2(back.rgbRed, front.rgbRed);
	res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
	res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}
inline MyImage::RGBQUAD SoftLightOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	int f = front.rgbBlue + front.rgbGreen + front.rgbRed;
	if (f <= 128) {
		auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
			return A * B / 128.0 + pow(A / 255.0, 2) * (255 - 2 * B);
		};
		res.rgbRed = b2(back.rgbRed, front.rgbRed);
		res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
		res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
		res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
		return res;
	}
	else {
		auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
			return A * (255 - B) / 128.0 + sqrt(A / 255.0) * (2 * B - 255);
		};
		res.rgbRed = b2(back.rgbRed, front.rgbRed);
		res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
		res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
		res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
		return res;
	}
}
inline MyImage::RGBQUAD HardLightOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	int f = front.rgbBlue + front.rgbGreen + front.rgbRed;
	if (f <= 128) {
		auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
			return B * A / 128.0;
		};
		res.rgbRed = b2(back.rgbRed, front.rgbRed);
		res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
		res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
		res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
		return res;
	}
	else {
		auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
			return 255 - (255 - B) * (255 - A) / 128.0;
		};
		res.rgbRed = b2(back.rgbRed, front.rgbRed);
		res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
		res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
		res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
		return res;
	}
}
inline MyImage::RGBQUAD VividLightOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	int f = front.rgbBlue + front.rgbGreen + front.rgbRed;
	if (f <= 128) {
		auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
			return A*1 - (255 - A*1) * (255 - 2 * B) / 2 * B;
		};
		res.rgbRed = b2(back.rgbRed, front.rgbRed);
		res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
		res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
		res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
		return res;
	}
	else {
		auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
			return A + A * (2 * B - 255) / 2 * (255 - B);
		};
		res.rgbRed = b2(back.rgbRed, front.rgbRed);
		res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
		res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
		res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
		return res;
	}
}
inline MyImage::RGBQUAD LinearLightOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
		return A + 2 * B - 255;
	};
	res.rgbRed = b2(back.rgbRed, front.rgbRed);
	res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
	res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}
inline MyImage::RGBQUAD PinLightOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	int f = front.rgbBlue + front.rgbGreen + front.rgbRed;
	if (f <= 128) {
		auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
			return std::min(A * 1, 2 * B);
		};
		res.rgbRed = b2(back.rgbRed, front.rgbRed);
		res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
		res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
		res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
		return res;
	}
	else {
		auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
			return std::min(A * 1, 2 * B - 255);
		};
		res.rgbRed = b2(back.rgbRed, front.rgbRed);
		res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
		res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
		res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
		return res;
	}
}
inline MyImage::RGBQUAD HardMixOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
		return A*1 + B >= 255 ? 255 : 0;
	};
	res.rgbRed = b2(back.rgbRed, front.rgbRed);
	res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
	res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}
inline MyImage::RGBQUAD DifferenceOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
		return abs(A * 1 - B);
	};
	res.rgbRed = b2(back.rgbRed, front.rgbRed);
	res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
	res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}
inline MyImage::RGBQUAD ExclusionOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
		return A + B - A * B / 128.0;
	};
	res.rgbRed = b2(back.rgbRed, front.rgbRed);
	res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
	res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}
inline MyImage::RGBQUAD SubtractOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
		return std::max(A - B*1, 0);
	};
	res.rgbRed = b2(back.rgbRed, front.rgbRed);
	res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
	res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}
inline MyImage::RGBQUAD DivideOverlay(const MyImage::RGBQUAD& back, const MyImage::RGBQUAD& front, unsigned char alpha) {
	MyImage::RGBQUAD res;
	alpha = front.rgbReserved * (alpha / 255.0);
	double d = (255 - alpha) / 255.0;
	auto blend = [d](unsigned char b, unsigned char f)->unsigned char {
		return f * (1 - d) + b * d;
	};
	auto b2 = [](unsigned char A, unsigned char B)->unsigned char {
		return A * 255.0 / B;
	};
	res.rgbRed = b2(back.rgbRed, front.rgbRed);
	res.rgbBlue = b2(back.rgbBlue, front.rgbBlue);
	res.rgbGreen = b2(back.rgbGreen, front.rgbGreen);
	res.rgbReserved = blend(back.rgbReserved, front.rgbReserved);
	return res;
}