#include "CreateImage.h"
#include <qpainter.h>
#include <QColorDialog>
#include <qdebug.h>

QIcon createColorIcon(QColor color) {
	QPixmap pixmap(16, 16);
	QPainter painter(&pixmap);
	painter.setPen(Qt::NoPen);
	painter.fillRect(QRect(0, 0, 16, 16), color);
	return QIcon(pixmap);
}

CreateImage::CreateImage(QWidget *parent)
	: QDialog(parent)
{
	_ui.setupUi(this);
	setFixedSize(this->width(), this->height());
	SetColor(Qt::white);
	SetHeight(500);
	SetWidth(500);
	_filename = QStringLiteral("ÐÂ½¨»­²¼");
	_ui.filename->setText(_filename);
	_ui.height->setText(QString::number(_height));
	_ui.width->setText(QString::number(_width));
	connect(_ui.filename, &QTextEdit::textChanged, [this] {
		_filename = _ui.filename->toPlainText();
		});
	connect(_ui.height, &QTextEdit::textChanged, [this] {
		QString t = _ui.height->toPlainText();
		SetHeight(t.toInt());
		});
	connect(_ui.width, &QTextEdit::textChanged, [this] {
		QString t = _ui.width->toPlainText();
		SetWidth(t.toInt());
		});
	connect(_ui.seletcedColor, &QPushButton::clicked, [this] {
		SetColor(QColorDialog::getColor(Qt::white, this));
		});
	connect(_ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(_ui.buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
}

CreateImage::~CreateImage()
{
}

QString CreateImage::GetFilename() {
	return _filename;
}

int CreateImage::GetHeight() {
	return _height;
}

int CreateImage::GetWidth() {
	return _width;
}

QColor CreateImage::GetColor() {
	return _color;
}

void CreateImage::SetWidth(int v) {
	_width = v;
	_ui.showWidth->setText(QString::number(v));
}

void CreateImage::SetHeight(int v) {
	_height = v;
	_ui.showHeight->setText(QString::number(v));
}

void CreateImage::SetColor(QColor v) {
	_color = v;
	_ui.seletcedColor->setIcon(createColorIcon(_color));
}
