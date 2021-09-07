#pragma once
#include "DrawTools.h"
#include <qmap.h>
class ToolFactory {//�滭���߹��������ж����
public:
	static ToolFactory& GetInstance();
	DrawTools* GetTool(QString name);
private:
	ToolFactory();
	DrawTools* Create(QString name);
private:
	static ToolFactory* _instance;
	QMap<QString, DrawTools*> _pool;
};

