#pragma once

#include "DrawToolsUI.h"
#include "ui_PencilUI.h"

class PencilUI : public DrawToolsUI
{
	Q_OBJECT

public:
	PencilUI(QWidget *parent = Q_NULLPTR);
	~PencilUI();

	// ͨ�� DrawToolsUI �̳�
	virtual void Init();
private:
	Ui::PencilUI ui;
};
