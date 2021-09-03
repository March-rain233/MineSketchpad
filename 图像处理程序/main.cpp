#include "PhotoshopLike.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    PhotoshopLike w;
    w.show();
    return a.exec();
}
