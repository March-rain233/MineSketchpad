#pragma once

#include <QWidget>
#include "ui_DrawToolsUI.h"
#include"DrawTools.h"

class DrawToolUIFactory;

class DrawToolsUI : public QWidget//工具UI的基类
{
	Q_OBJECT
public:
	DrawToolsUI(QWidget *parent = Q_NULLPTR);
	~DrawToolsUI();
	DrawTools& GetTool();
protected:
	virtual void Init();//通知UI初始化
	friend DrawToolUIFactory;//给工厂提供内部权限以便依赖注入
private:
	Ui::DrawToolsUI ui;
	DrawTools* _tool;//绑定的工具，在子类通过类型转换访问
};
