#pragma once
#include "Image.h"
#include "qvector.h"
#include "LayerModel.h"

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

//像素修改命令
class PaintCommand :public DrawCommand {
public:
	PaintCommand(const QVector<LayerModel*>&);
	~PaintCommand();
	// 通过 DrawCommand 继承
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
	QVector<PixelInfo*> _changedPixel;//修改的数据
	const QVector<LayerModel*>& _target;//修改对象
};

