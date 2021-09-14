#pragma once

#include <QWidget>
#include "ui_LayerGroup.h"
#include "LayerModel.h"
#include "LayerUI.h"
#include "DrawCanvas.h"
#include <qvector.h>
#include <qbuttongroup.h>

class LayerGroup : public QWidget
{
	Q_OBJECT

public:
	LayerGroup(QWidget *parent = Q_NULLPTR);
	~LayerGroup();
	void Rigister(DrawCanvas*);
	void AddLayer(LayerModel*);//增加图层
	void InsertLayer(LayerModel*, int);//在指定位置插入图层
	void DeleteLayer(int);//删除指定图层
	void ClearLayer(int);//清除指定图层
	void ClearAllUI();//清空UI
	void OpenFile();
	void CloseActions(bool);
private:
	void UnCheckOther(int);//关闭其他选择框
	void AddCheck(int);//选中选择框的处理函数
private:
	Ui::LayerGroup ui;
	DrawCanvas* _device;
	QVector<LayerUI*> _layerUIs;
};
