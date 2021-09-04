#pragma once

#include <QWidget>
#include "ui_tools.h"

class Tools : public QWidget
{
	Q_OBJECT

public:
	Tools(QWidget *parent = Q_NULLPTR);
	~Tools();

private:
	Ui::Tools ui;
};
