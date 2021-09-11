#pragma once
#include"DrawToolsUI.h"
#include<qmap.h>
class DrawToolUIFactory {
public:
	static DrawToolUIFactory& GetInstance();
	DrawToolsUI* Create(QString name, QWidget* parent);
private:
	DrawToolUIFactory() {

	}
private:
	static DrawToolUIFactory* _instance;
	QMap<QString, DrawToolsUI*> _pool;
};

