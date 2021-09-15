#include "Tools.h"
#include"DrawToolUIFactory.h"
#include<QColorDialog>
#include"MyPaletteModel.h"
#include<qdebug.h>
#include<QButtonGroup>

Tools::Tools(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//绑定颜色数据
	MyPaletteModel& instance = MyPaletteModel::GetInstance();
	connect(ui.front, &QPushButton::clicked, [this, &instance] {
		MyImage::RGBQUAD color;
		auto temp = QColorDialog::getColor(Qt::black, this);
		color.rgbRed = temp.red();
		color.rgbGreen = temp.green();
		color.rgbBlue = temp.blue();
		color.rgbReserved = 255;
		instance.SetFrontColor(color);
		});
	connect(ui.background, &QPushButton::clicked, [this, &instance] {
		MyImage::RGBQUAD color;
		auto temp = QColorDialog::getColor(Qt::black, this);
		color.rgbRed = temp.red();
		color.rgbGreen = temp.green();
		color.rgbBlue = temp.blue();
		color.rgbReserved = 255;
		instance.SetBackColor(color);
		});
	connect(ui.swap, &QPushButton::clicked, [&instance] {
		instance.SwapColor();
		});
	instance.FrontColorChange += [this](MyImage::RGBQUAD v) {
		ui.front->setIcon(createColorIcon(QColor(v.rgbRed, v.rgbGreen, v.rgbBlue), ui.front->iconSize()));
	};
	instance.BackColorChange += [this](MyImage::RGBQUAD v) {
		ui.background->setIcon(createColorIcon(QColor(v.rgbRed, v.rgbGreen, v.rgbBlue), ui.front->iconSize()));
	};

	instance.SetFrontColor(instance.GetFrontColor());
	instance.SetBackColor(instance.GetBackColor());

	//绑定工具按钮
	auto buttons = ui.frame->findChildren<QAbstractButton *>();
	QButtonGroup* pButtonGroup = new QButtonGroup(this);
	for (int i = 0; i < buttons.count(); ++i) {
		QAbstractButton& temp = *buttons[i];
		pButtonGroup->addButton(&temp, i);
		connect(&temp, &QAbstractButton::toggled, [this, &temp](bool v) {
			if (v) {
				SetTool(temp.text());
			}});
	}
	//连接滑块
	connect(ui.verticalScrollBar, &QSlider::valueChanged, [this](int v) {
		ui.body->move(ui.body->pos().x(), -v);
		});
}

Tools::~Tools()
{

}

void Tools::Rigister(DrawCanvas* device) {
	_device = device;
	ui.MoveHand->setChecked(true);
}

void Tools::SetTool(QString name) {
	qDebug() << "使用" << name;
	if (ui.toolMenu != nullptr) {
		ui.toolMenu->hide();
	}
	ui.toolMenu = DrawToolUIFactory::GetInstance().Create(name, ui.widget);
	ui.widget->layout()->addWidget(ui.toolMenu);
	ui.toolMenu->show();
	_device->SetTool(&ui.toolMenu->GetTool());
	ui.verticalScrollBar->setRange(0, ui.body->rect().height());
}
