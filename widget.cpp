#include "widget.h"
#include "worker.h"

#include <QCoreApplication>
#include <QPaintEvent>
#include <QPainter>
#include <QThread>

const int cols(800);
const int rows(800);
const QRgb color1 = QRgb(0x00000000);
const QRgb color2 = QRgb(0x00A6A6FF);
const QRgb color3 = QRgb(0x004747D1);
const QRgb color4 = QRgb(0x00E68A00);


#include <QChartView>
#include <QtCharts>
#include <QLineSeries>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(cols,rows);

    m_Image = QImage(cols,rows, QImage::Format_RGB32);

    QThread *thread = new QThread();
    worker = new Worker(cols,rows, color1, color2, color3, color4);
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &Worker::initWorker);
    connect(thread, &QThread::finished, worker, &Worker::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    connect(qApp, &QCoreApplication::aboutToQuit, thread, &QThread::quit);


    connect(&m_tUpdate, &QTimer::timeout, this, &Widget::updateImage);
    m_tUpdate.start(30);

    colors.append(&color1);
    colors.append(&color2);
    colors.append(&color3);
    colors.append(&color4);

    thread->start(QThread::HighestPriority);

/*Uncomment for chartview visualisation
    connect(worker, &Worker::workDone, this, [=](QVector<quint64> times, QVector<quint64> grains){
        QtCharts::QChartView *cView; = new QtCharts::QChartView();
        cView->resize(400,400);
        cView->show();

        QLineSeries *lineSeries = new QLineSeries();
        for(int i(0); i < times.size(); i++){
            lineSeries->append(grains.at(i)/1000,(times.at(i)/1000.0));
        }
        QChart * chart = new QChart();
        chart->addSeries(lineSeries);
        chart->legend()->hide();

        QValueAxis *axisX = new QValueAxis();
        axisX->setTickCount(10);
        axisX->setLabelFormat("%i");
        axisX->setTitleText("Sandkörner 1/1000");
        chart->addAxis(axisX, Qt::AlignBottom);
        lineSeries->attachAxis(axisX);

        QValueAxis *time = new QValueAxis();
        time->setLabelFormat("%i");
        time->setTitleText("Zeit [sec]");
        chart->addAxis(time, Qt::AlignLeft);
        lineSeries->attachAxis(time);

        cView->setChart(chart);
    });*/
}

void Widget::updateImage()
{
    QByteArray data =worker->getData();
    memcpy(m_Image.bits(),data.data(),data.size());
    sGrains = QString::number(worker->getCount());
    update();
//    repaint();
}

void Widget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter p(this);

    //Background
    p.fillRect(rect(),*colors.at(0));

    //Adjust Sandpileimage without changing the aspect ratio
    int min = width() > height() ? height() : width();
    QRect rect((width()-min)/2, (height()-min)/2, min,min);
    p.drawImage(rect,m_Image,m_Image.rect());

    //Draw legend
    p.setPen(QColor("White"));
    p.drawText(0,height()-18,"Sandkörner: " + sGrains);

    int offsetY = 3;
    for(int i(0); i < colors.size(); i++){
        p.drawRect(5,i * (offsetY +18) + offsetY,18,18);
        p.fillRect(6,1 + i * (offsetY +18) +offsetY,16,16, *colors.at(i));
        p.setPen(QColor("White"));
        p.drawText(26, 15 + i *(18+offsetY)+offsetY, QString::number(i) + " Sandkörner");
    }
}
