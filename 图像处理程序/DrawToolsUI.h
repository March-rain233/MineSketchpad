#pragma once

#include <QWidget>
#include "ui_DrawToolsUI.h"
#include"DrawTools.h"

class DrawToolUIFactory;

class DrawToolsUI : public QWidget//����UI�Ļ���
{
	Q_OBJECT
public:
	DrawToolsUI(QWidget *parent = Q_NULLPTR);
	~DrawToolsUI();
	DrawTools& GetTool();
protected:
	virtual void Init();//֪ͨUI��ʼ��
	friend DrawToolUIFactory;//�������ṩ�ڲ�Ȩ���Ա�����ע��
private:
	Ui::DrawToolsUI ui;
	DrawTools* _tool;//�󶨵Ĺ��ߣ�������ͨ������ת������
};
