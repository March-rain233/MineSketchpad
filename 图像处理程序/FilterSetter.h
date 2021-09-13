#pragma once

#include <QWidget>
#include "ui_FilterSetter.h"
#include "ImageFilter.h"
#include "FilterFactory.h"
#include "qtablewidget.h"
#include "MyEvent.h"

class FilterSetter : public QWidget
{
	Q_OBJECT

public:
	FilterSetter(QWidget *parent = Q_NULLPTR);
	~FilterSetter();
	ImageFilter* GetFilter();
	void SetFilter(ImageFilter*);
private:
	void SetRadius(int);
public:
	MyEvent<FilterSetter, ImageFilter*> FilterChanged;
private:
	Ui::FilterSetter ui;
	ImageFilter* _filter;
	QTableWidget* _table;
	double* _kernel;
};
