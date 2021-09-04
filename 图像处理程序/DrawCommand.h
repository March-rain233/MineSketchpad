#pragma once
#include "Image.h"
#include "qvector.h"

/// <summary>
/// ��ͼ�������
/// </summary>
class DrawCommand {
public:

	/// <summary>
	/// ִ��
	/// </summary>
	/// <param name="target">Ŀ��</param>
	virtual void Execute() = 0;

	 ///<summary>
	 ///����
	 ///</summary>
	virtual void Unexecute() = 0;
};

//�����޸�����
class PaintCommand :public DrawCommand {
public:
	PaintCommand(const QVector<MyImage::Image*>&);
	~PaintCommand();
	// ͨ�� DrawCommand �̳�
	virtual void Execute() override;
	virtual void Unexecute() override;

	void SetPixel(int layer, int i, int j, MyImage::RGBQUAD color);
private:
	struct PixelInfo {
		int Layer;
		int I;
		int J;
		MyImage::RGBQUAD Before;
		MyImage::RGBQUAD After;
	};
	QVector<PixelInfo*> _changedPixel;//�޸ĵ�����
	const QVector<MyImage::Image*>& _target;//�޸Ķ���
};

