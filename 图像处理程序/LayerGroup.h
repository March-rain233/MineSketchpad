#pragma once

#include <QWidget>
#include "ui_LayerGroup.h"
#include "LayerModel.h"
#include "LayerUI.h"
#include "DrawCanvas.h"
#include <qvector.h>

class LayerGroup : public QWidget
{
	Q_OBJECT

public:
	LayerGroup(QWidget *parent = Q_NULLPTR);
	~LayerGroup();
	void Rigister(DrawCanvas*);
	void AddLayer(LayerModel*);//����ͼ��
	void InsertLayer(LayerModel*, int);//��ָ��λ�ò���ͼ��
	void DeleteLayer(int);//ɾ��ָ��ͼ��
	void ClearLayer(int);//���ָ��ͼ��
	void ClearAllUI();//���UI
private:
	Ui::LayerGroup ui;
	DrawCanvas* _device;
	QVector<LayerUI*> _layerUIs;
};
