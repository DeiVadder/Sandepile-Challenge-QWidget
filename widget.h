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
    void newGrainData(QVector<QVector<quint8>> distribution, quint64 count);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    QTimer m_tUpdate;

    QImage m_Image;

    Worker *worker;
    QVector<const QRgb*> colors;

    QVector<QVector<uint8_t> > lastDistribution;
    quint64 lastCount = 0;
    quint64 currentGrains = 0;
};

#endif // WIDGET_H
