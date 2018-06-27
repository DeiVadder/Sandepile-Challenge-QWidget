#include "worker.h"

Worker::Worker(const uint &rows, const uint &cols, QObject *parent) : QObject(parent)
{
    this->rows = rows;
    this->cols = cols;

    m_GrainCount = 0;

    colStart = cols/2;
    rowStart = rows/2;


    for(uint i(0); i < cols; i++){
        QVector<uint8_t> v;
        for(uint j(0); j < rows; j++){
            v.append(0);
        }
        m_GrainsAt.append(v);
    }
}

void Worker::startWorker()
{
    tElapsed.start();

    while(!atBorder){
        m_GrainCount++;

        addGrainAt(colStart, rowStart);

        emit newGrainData(m_GrainsAt, m_GrainCount);

        //Uncomment for chartview visualisation
        /*if(m_GrainCount % 1000 == 0){
            vTimes << tElapsed.elapsed();
            vGrains << m_GrainCount;
        }*/
    }
    emit workDone(vTimes, vGrains);
}

void Worker::addGrainAt(uint &x,  uint &y)
{
    if(x < cols && y < rows){
        m_GrainsAt[x][y]++;

        if(m_GrainsAt.at(x).at(y) > 3){
            //pile topples
            m_GrainsAt[x][y] = 0;

            addGrainAt(x, --y);
            addGrainAt(++x, ++y);
            addGrainAt(--x, ++y);
            addGrainAt(--x, --y);
            x++;
        }
    }else
        atBorder = true;
}
