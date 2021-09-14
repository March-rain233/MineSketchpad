#include "ChangeSize.h"

ChangeSize::ChangeSize(DrawCanvas* device, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
}

ChangeSize::~ChangeSize()
{
}

int ChangeSize::GetWidth() {
	return ui.width->value();
}

int ChangeSize::GetHeight() {
	return ui.height->value();
}
