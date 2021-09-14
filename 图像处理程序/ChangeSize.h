#pragma once

#include <QDialog>
#include "ui_ChangeSize.h"

class ChangeSize : public QDialog
{
	Q_OBJECT

public:
	ChangeSize(QWidget *parent = Q_NULLPTR);
	~ChangeSize();

private:
	Ui::ChangeSize ui;
};
