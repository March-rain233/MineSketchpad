#include "PhotoshopLike.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    PhotoshopLike w;
    w.show();
    return a.exec();
}
