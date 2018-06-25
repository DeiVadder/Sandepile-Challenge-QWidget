#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QVector>
#include <QColor>

#include <QMutex>

//---------
#include <QElapsedTimer>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(const uint &rows, const uint &cols,
                    const QRgb &one = QRgb(0x00000000),
                    const QRgb &two = QRgb(0x006666FF),
                    const QRgb &three = QRgb(0x004747D1),
                    const QRgb &four = QRgb(0x00E68A00),
                    QObject *parent = nullptr);
    ~Worker();

    void getData(uchar *image);
    const quint64 &getCount();

public slots:
    void startWorker();

private slots:
    void addGrainAt(uint &x, uint &y);

signals:
    void workDone(QVector<quint64> times, QVector<quint64> grains);

private:
    uchar *m_Grains, *m_GrainsCopy;

    QMutex m_Mutex;

    QRgb color1;
    QRgb color2;
    QRgb color3;
    QRgb color4;

    QVector<QRgb*> colors;

    uint colStart, rowStart, cols, rows, index1D, arraySize;

    quint64 m_GrainCount = 0;
    bool atBorder = false;

    //----
    QElapsedTimer tElapsed;
    QVector<quint64> vTimes;
    QVector<quint64> vGrains;
};

#endif // WORKER_H
