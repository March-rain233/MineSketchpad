#pragma once

#include <QWidget>
#include "ui_LayerGroup.h"

class LayerGroup : public QWidget
{
	Q_OBJECT

public:
	LayerGroup(QWidget *parent = Q_NULLPTR);
	~LayerGroup();

private:
	Ui::LayerGroup ui;
};
