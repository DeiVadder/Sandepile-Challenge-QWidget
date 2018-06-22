#include "worker.h"

Worker::Worker(const uint &rows, const uint &cols,
               const QRgb &one, const QRgb &two, const QRgb &three, const QRgb &four,
               QObject *parent) : QObject(parent)
{
    this->rows = rows;
    this->cols = cols;

    color1 = one;
    color2 = two;
    color3 = three;
    color4 = four;
}

void Worker::initWorker()
{
    colors.clear();
    m_GrainPiles.clear();
    m_GrainCount = 0;

    colors.append(&color1);
    colors.append(&color2);
    colors.append(&color3);
    colors.append(&color4);

    p_Data.resize(cols*rows*4);
    m_Data.resize(cols*rows*4);
    m_Data.fill(0);
    p_Data.fill(0);

    colStart = cols/2;
    rowStart = rows/2;

    for(uint i(0); i < cols; i++){
        QVector<uint> v;
        for(uint j(0); j < rows; j++){
            v.append(0);
        }
        m_GrainPiles.append(v);
    }
    startWorker();
}

void Worker::startWorker()
{
    uint x(colStart), y(rowStart);
    tElapsed.start();
    while(!atBorder){
        m_GrainCount++;

        addGrainAt(x, y);

        //To prevent unwanted interim states in the ui
        memcpy(p_Data.data(),m_Data.data(),p_Data.length());

        /*Uncomment for chartview visualisation
        if(m_GrainCount % 1000 == 0){
            vTimes << tElapsed.elapsed();
            vGrains << m_GrainCount;
        }*/
    }
    emit workDone(vTimes, vGrains);
}

void Worker::addGrainAt(uint &x,  uint &y)
{
    if(x < cols && y < rows){
        m_GrainPiles[x][y] ++;
        if(m_GrainPiles.at(x).at(y) > 3){
            //pile topples
            m_GrainPiles[x][y] = 0;

            //direkt x & y manipulation, to prevent stack overflow
            addGrainAt(x,   --y);
            addGrainAt(++x, ++y);
            addGrainAt(--x, ++y);
            addGrainAt(--x, --y);
            ++x;
        }

        index1D = (cols *y + x)*4;
        memcpy(m_Data.data()+index1D,
               colors.at(m_GrainPiles.at(x).at(y)),
               4);
    }else
        atBorder = true;
}

const QByteArray &Worker::getData()
{
    //Readonly access & potential artefacts for the Image are rare and irrelevant -> no Mutex to increase performance
    return p_Data;
}

const quint64 &Worker::getCount()
{
    return m_GrainCount;
}

