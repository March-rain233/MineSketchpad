#include "Tools.h"
#include"ToolFactory.h"

Tools::Tools(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

Tools::~Tools()
{

}

void Tools::Rigister(DrawCanvas*) {}

void Tools::SetTool(QString name) {
	_device->SetTool(ToolFactory::GetInstance().GetTool(name));
}
