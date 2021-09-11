#pragma once

#include <QWidget>
#include "ui_LayerUI.h"
#include "LayerModel.h"

class LayerUI : public QWidget
{
	Q_OBJECT

public:
	LayerUI(QWidget *parent = Q_NULLPTR);
	~LayerUI();
	void Rigister(LayerModel*);
	void Clear();
	bool eventFilter(QObject* watched, QEvent* event) override;//重写事件过滤器
private:
	Ui::LayerUI ui;
	LayerModel* _model;
};
