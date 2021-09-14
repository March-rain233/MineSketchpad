#pragma once

#include <QDialog>
#include "ui_Binaryzation.h"
#include "DrawCanvas.h"

class Binaryzation : public QDialog
{
	Q_OBJECT

public:
	Binaryzation(DrawCanvas* ,QWidget *parent = Q_NULLPTR);
	~Binaryzation();

private:
	Ui::Binaryzation ui;
};
