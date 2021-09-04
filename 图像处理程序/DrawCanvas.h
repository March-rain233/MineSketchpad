#pragma once
#include <QWidget>
#include <QMouseEvent>
#include <QStack.h>
#include <QVector.h>
#include <QPainter>
#include "ui_DrawCanvas.h"
#include "DrawCommand.h"
#include "Image.h"
#include "DrawTools.h"

class DrawTools;

/// <summary>
/// 画图板组件
/// </summary>
class DrawCanvas : public QWidget {
	Q_OBJECT

public:
	DrawCanvas(QWidget* parent = Q_NULLPTR);
	~DrawCanvas();

	void Undo();// 撤回操作
	void Redo();// 重新执行操作
	void ClearCommand();//清除操作记录
	void ClearRedoCommand();//清除可以重做的操作记录

	void AddLayer(const MyImage::Image&);//增加图层
	void InsertLayer(const MyImage::Image&, int);//在指定位置插入图层
	void DeleteLayer(int);//删除指定图层
	const QVector<MyImage::Image*>& GetLayers();//获取图层组
	QVector<int>& GetSelected();//获取当前选中的图层索引

	void SetScale(float);//设置缩放率
	float GetScale();//获得缩放率

	QPoint GetDrawPoint();//获得绘画起始点

	void SaveImage(const QString&);// 储存文件

	void PushCommand(DrawCommand*);//加入已执行命令

	void SetTool(DrawTools*);//设置绘画工具

	void ClearCanvas();//清除画板
	bool IsEmpty();//是否在工作
private:
	void mousePressEvent(QMouseEvent*) override;//重写鼠标按下事件
	void mouseReleaseEvent(QMouseEvent*) override;//重写鼠标释放事件
	void mouseMoveEvent(QMouseEvent*) override;//重写鼠标移动事件
	void contextMenuEvent(QContextMenuEvent*) override;//重写菜单事件
	void keyPressEvent(QKeyEvent* e) override; //重写按键事件

	void resizeEvent(QResizeEvent*) override;//重写大小变化事件

	bool eventFilter(QObject* watched, QEvent* event) override;//重写事件过滤器
	void DrawImage();//绘制图片

	void HorizontalMove(int);//渲染位置横向移动
	void VertiaclMove(int);//渲染位置纵向移动

	int GetDrawHeight();//获取图片渲染后的高度
	int GetDrawWidth();//获取图片渲染后的宽度

	QRect GetDrawRect();//获取相对于显示区域的渲染区域矩形
private:
	Ui::DrawCanvas _ui;

	QStack<DrawCommand*> _historyCommand;//历史命令
	QStack<DrawCommand*> _redoCommand;//可以重做的操作

	QVector<MyImage::Image*> _layers;//图层

	DrawTools* _tool;//当前使用的绘图工具

	QPoint _drawPoint;//渲染起点（左上角）
	QImage* _canvasFill;//画布背景填充
	const float _maxSizeRate = 3200;//最大画面放大率
	const float _minSizeRate = 1;//最大画面缩小率
	float _scale = 1;//当前缩放率

	QVector<int> _seletcted;//当前选中的图层
};
