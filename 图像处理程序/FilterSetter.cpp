#include "FilterSetter.h"
#include <QMetaEnum>

FilterSetter::FilterSetter(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.radiusSlider, &QAbstractSlider::valueChanged, [this](int v) {
		SetRadius(v);
		ui.radiusInput->blockSignals(true);
		ui.radiusInput->setValue(v);
		ui.radiusInput->blockSignals(false);
		});
	connect(ui.radiusInput, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int v) {
		SetRadius(v);
		ui.radiusSlider->blockSignals(true);
		ui.radiusInput->setValue(v);
		ui.radiusSlider->blockSignals(false);
		});
	QMetaEnum meta = QMetaEnum::fromType<FilterType>();
	for (int i = 0; i < meta.keyCount(); ++i) {
		ui.filterType->addItem(meta.key(i));
	}
	connect(ui.filterType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		[this, meta](int i) {
			SetFilter(FilterFactory::Create(_filter->GetRadius(), (FilterType)meta.value(i)));
		});
	_table = nullptr;
	_filter = nullptr;
}

FilterSetter::~FilterSetter()
{
	
}

void FilterSetter::SetFilter(ImageFilter* f) {
	_filter = f;
	if (_table != nullptr) {
		_table->close();
		delete _table;
		_table = nullptr;
	}
	if (typeid(*f) == typeid(LinearFilter)) {
		LinearFilter* lf = static_cast<LinearFilter*>(f);
		_kernel = lf->GetKernel();
		int w = lf->GetRadius() * 2 + 1;
		_table = new QTableWidget(w, w, this);
		for (int i = 0; i < w; ++i) {
			for (int j = 0; j < w; ++j) {
				_table->setItem(i, j, new QTableWidgetItem(QString::number(_kernel[i * w + j])));
			}
		}
		connect(_table, &QTableWidget::itemChanged, [this](QTableWidgetItem* item) {
			_kernel[item->row() * (_filter->GetRadius() * 2 + 1) + item->column()] =
				item->text().toDouble();
			});
		ui.verticalLayout->addWidget(_table);
	}
	FilterChanged(f);
}

void FilterSetter::SetRadius(int v) {
	if (_table) {
		LinearFilter* lf = static_cast<LinearFilter*>(_filter);
		int oldW = _filter->GetRadius() * 2 + 1;
		int newW = v * 2 + 1;
		double* kernel = new double[newW * newW];
		_table->blockSignals(true);
		_table->clear();
		_table->setRowCount(0);
		_table->setRowCount(newW);
		_table->setColumnCount(newW);
		for (int i = 0; i < newW; ++i) {
			for (int j = 0; j < newW; ++j) {
				if (i < oldW && j < oldW) {
					kernel[i * newW + j] = _kernel[i * oldW + j];
				}
				else {
					kernel[i * newW + j] = 0;
				}
				_table->setItem(i, j, new QTableWidgetItem(QString::number(kernel[i * newW + j])));
			}
		}
		_table->blockSignals(false);
		lf->SetKernel(kernel, v);
		delete[] kernel;
	}
	else {
		_filter->SetRadius(v);
	}
}
