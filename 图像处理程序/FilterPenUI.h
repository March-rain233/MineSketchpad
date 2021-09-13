#pragma once

#include "DrawToolsUI.h"
#include "ui_FilterPenUI.h"

class FilterPenUI : public DrawToolsUI
{
	Q_OBJECT

public:
	FilterPenUI(QWidget *parent = Q_NULLPTR);
	~FilterPenUI();
	virtual void Init() override;
private:
	Ui::FilterPenUI ui;
};
