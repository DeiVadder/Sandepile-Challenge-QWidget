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
    explicit Worker(const uint &rows, const uint &cols, QObject *parent = nullptr);

public slots:
    void startWorker();

private slots:
    void addGrainAt(uint &x, uint &y);

signals:
    void workDone(QVector<quint64> times, QVector<quint64> grains);
    void newGrainData(QVector<QVector<uint8_t>> distribution, quint64 count);

private:
    QVector<QVector<uint8_t> > m_GrainsAt;

    uint colStart, rowStart, cols, rows;

    quint64 m_GrainCount = 0;
    bool atBorder = false;

    //----
    QElapsedTimer tElapsed;
    QVector<quint64> vTimes;
    QVector<quint64> vGrains;
};

#endif // WORKER_H
