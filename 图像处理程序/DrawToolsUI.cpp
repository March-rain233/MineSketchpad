#include "DrawToolsUI.h"

DrawToolsUI::DrawToolsUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

DrawToolsUI::~DrawToolsUI()
{
}

DrawTools& DrawToolsUI::GetTool() {
	return *_tool;
}

void DrawToolsUI::Init() {}
