#pragma once
#include "Image.h"
#include "qvector.h"
#include "LayerModel.h"
#include <functional>
#include <qqueue.h>

/// <summary>
/// 绘图命令基类
/// </summary>
class DrawCommand {
public:

	/// <summary>
	/// 执行
	/// </summary>
	/// <param name="target">目标</param>
	virtual void Execute() = 0;

	 ///<summary>
	 ///撤回
	 ///</summary>
	virtual void Unexecute() = 0;
};

class GroupCommand :
	public DrawCommand {
	// 通过 DrawCommand 继承
public:
	~GroupCommand();
	virtual void Execute() override;
	virtual void Unexecute() override;
	void PushBackCommand(DrawCommand*);
private:
	QQueue<DrawCommand*> _commands;
};

//像素修改命令
class PaintCommand :public DrawCommand {
public:
	PaintCommand(LayerModel&);
	~PaintCommand();
	// 通过 DrawCommand 继承
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
	QVector<PixelInfo*> _changedPixel;//修改的数据
	LayerModel& _target;//修改对象
};

//执行函数命令
class FunctionCommand :
	public DrawCommand {
public:
	// 通过 DrawCommand 继承
	virtual void Execute() override;
	virtual void Unexecute() override;
	std::function<void(void)> Redo;
	std::function<void(void)> Undo;
};