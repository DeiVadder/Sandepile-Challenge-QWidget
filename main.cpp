#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<QVector<quint64> >("QVector<quint64>");

    QApplication a(argc, argv);

    Widget w;
    w.show();

    return a.exec();
}
