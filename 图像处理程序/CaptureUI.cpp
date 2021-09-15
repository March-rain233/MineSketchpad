#include "CaptureUI.h"

CaptureUI::CaptureUI(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
}

CaptureUI::~CaptureUI()
{
}

int CaptureUI::GetX1() {
	return ui.x1->value();
}

int CaptureUI::GetX2() {
	return ui.x2->value();
}

int CaptureUI::GetY1() {
	return ui.y1->value();
}

int CaptureUI::GetY2() {
	return ui.y2->value();
}
