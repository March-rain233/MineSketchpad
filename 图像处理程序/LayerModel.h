#pragma once
#include"Image.h"
#include"OverlayFunctionFactory.h"
class LayerModel {
public:
	LayerModel(MyImage::Image*, OverlayMode mode = OverlayMode::Normal);
	~LayerModel();
	MyImage::Image& GetImage();//向外界提供图像资源以修改
	void SetImage(MyImage::Image*);//设置图片
	bool IsLock();//图层是否锁定
	bool IsVisible();//图层是否可视
	void SetLock(bool);//设定锁定
	void SetVisible(bool);//设定可视

	void BeginDraw();//开始绘画
	MyImage::Image& GetBuffer();//获取缓冲区
	void EndDraw();//结束绘画
	void CancelDraw();//放弃绘画

	void PaintEvent(MyImage::Image&);//将图层以自身的渲染模式渲染到画布上
	inline void PaintEvent(int, MyImage::Image&);
	void PaintEvent(int, int, MyImage::Image&);
	void SetOverlayMode(OverlayMode);//设置渲染模式
	OverlayMode GetOverlayMode();//获取渲染模式
	void Update();//刷新事件
public:
	MyEvent<LayerModel, bool> VisibleChanged;
	MyEvent<LayerModel, int> PixelChanged;
public:
	QString Name;
private:
	MyImage::Image* _image;//图层的图片资源
	MyImage::Image* _buffer;//图像修改临时缓存区
	bool _isLock;//图层是否锁定
	bool _isVisible;//图层是否可视
	bool _isPainting;//图层正在被绘制
	OverlayMode _overlayMode;//当前图层的渲染模式;
	std::function<MyImage::RGBQUAD(const MyImage::RGBQUAD&, const MyImage::RGBQUAD&)> _overlayHandler;//颜色混合函数
};

