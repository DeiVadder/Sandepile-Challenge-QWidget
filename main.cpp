#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<QVector<quint64> >("QVector<quint64>");
    qRegisterMetaType<quint64>("quint64");
    qRegisterMetaType<QVector<QVector<uint8_t> > >("QVector<QVector<uint8_t> >");

    QApplication a(argc, argv);

    Widget w;
    w.show();

    return a.exec();
}
