#pragma once
#include"Image.h"
#include"OverlayFunctionFactory.h"
class LayerModel {
public:
	LayerModel(MyImage::Image*, OverlayMode mode = OverlayMode::Normal);
	~LayerModel();
	MyImage::Image& GetImage();//������ṩͼ����Դ���޸�
	bool IsLock();//ͼ���Ƿ�����
	bool IsVisible();//ͼ���Ƿ����
	void SetLock(bool);//�趨����
	void SetVisible(bool);//�趨����

	void BeginDraw();//��ʼ�滭
	MyImage::Image& GetBuffer();//��ȡ������
	void EndDraw();//�����滭

	void PaintEvent(MyImage::Image&);//��ͼ�����������Ⱦģʽ��Ⱦ��������
	inline void PaintEvent(int, MyImage::Image&);
	void PaintEvent(int, int, MyImage::Image&);
	void SetOverlayMode(OverlayMode);//������Ⱦģʽ
	OverlayMode GetOverlayMode();//��ȡ��Ⱦģʽ
	void Update();//ˢ���¼�
public:
	MyEvent<LayerModel, bool> VisibleChanged;
	MyEvent<LayerModel, int> PixelChanged;
public:
	QString Name;
private:
	MyImage::Image* _image;//ͼ���ͼƬ��Դ
	MyImage::Image* _buffer;//ͼ���޸���ʱ������
	bool _isLock;//ͼ���Ƿ�����
	bool _isVisible;//ͼ���Ƿ����
	bool _isPainting;//ͼ�����ڱ�����
	OverlayMode _overlayMode;//��ǰͼ�����Ⱦģʽ;
	std::function<MyImage::RGBQUAD(const MyImage::RGBQUAD&, const MyImage::RGBQUAD&)> _overlayHandler;//��ɫ��Ϻ���
};

