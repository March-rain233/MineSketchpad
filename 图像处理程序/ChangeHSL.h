#pragma once

#include <QDialog>
#include "ui_ChangeHSL.h"

class ChangeHSL : public QDialog
{
	Q_OBJECT

public:
	ChangeHSL(QWidget *parent = Q_NULLPTR);
	~ChangeHSL();

private:
	Ui::ChangeHSL ui;
};
