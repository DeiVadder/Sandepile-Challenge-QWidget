#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QVector>

class Worker;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);

private slots:
    void updateImage();

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    QTimer m_tUpdate;

    QImage m_Image;
    QString sGrains;

    Worker *worker;
    QVector<const QRgb*> colors;
};

#endif // WIDGET_H
