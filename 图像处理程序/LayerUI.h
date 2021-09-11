#pragma once

#include <QWidget>
#include "ui_LayerUI.h"

class LayerUI : public QWidget
{
	Q_OBJECT

public:
	LayerUI(QWidget *parent = Q_NULLPTR);
	~LayerUI();

private:
	Ui::LayerUI ui;
};
