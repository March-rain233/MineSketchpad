#pragma once
#include "Image.h"
#include "qvector.h"
#include "LayerModel.h"
#include <functional>
#include <qqueue.h>

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

class GroupCommand :
	public DrawCommand {
	// ͨ�� DrawCommand �̳�
public:
	~GroupCommand();
	virtual void Execute() override;
	virtual void Unexecute() override;
	void PushBackCommand(DrawCommand*);
private:
	QQueue<DrawCommand*> _commands;
};

//�����޸�����
class PaintCommand :public DrawCommand {
public:
	PaintCommand(LayerModel&);
	~PaintCommand();
	// ͨ�� DrawCommand �̳�
	virtual void Execute() override;
	virtual void Unexecute() override;

	void SetPixel(int i, int j, const MyImage::RGBQUAD& color);
private:
	struct PixelInfo {
		int I;
		int J;
		MyImage::RGBQUAD Before;
		MyImage::RGBQUAD After;
	};
	QVector<PixelInfo*> _changedPixel;//�޸ĵ�����
	LayerModel& _target;//�޸Ķ���
};

//ִ�к�������
class FunctionCommand :
	public DrawCommand {
public:
	// ͨ�� DrawCommand �̳�
	virtual void Execute() override;
	virtual void Unexecute() override;
	std::function<void(void)> Redo;
	std::function<void(void)> Undo;
};

class CopyCommand :
	public DrawCommand {
public:
	~CopyCommand();
	virtual void Execute() override;
	virtual void Unexecute() override;
	MyImage::Image* Last = nullptr;
	MyImage::Image* After = nullptr;
	LayerModel* Target = nullptr;
};