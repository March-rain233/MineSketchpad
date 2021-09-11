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

	void PaintEvent(MyImage::Image&);//��ͼ�����������Ⱦģʽ��Ⱦ��������
	void PaintEvent(int, MyImage::Image&);
	void PaintEvent(int, int, MyImage::Image&);
	void SetOverlayMode(OverlayMode);//������Ⱦģʽ
	OverlayMode GetOverlayMode();//��ȡ��Ⱦģʽ
private:
	MyImage::Image* _image;//ͼ���ͼƬ��Դ
	bool _isLock;//ͼ���Ƿ�����
	bool _isVisible;//ͼ���Ƿ����
	OverlayMode _overlayMode;//��ǰͼ�����Ⱦģʽ;
	std::function<MyImage::RGBQUAD(MyImage::RGBQUAD, MyImage::RGBQUAD)> _overlayHandler;//��ɫ��Ϻ���
};

