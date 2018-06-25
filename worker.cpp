#include "worker.h"

Worker::Worker(const uint &rows, const uint &cols,
               const QRgb &one, const QRgb &two, const QRgb &three, const QRgb &four,
               QObject *parent) : QObject(parent), color1(one) , color2(two), color3(three), color4(four)
{
    this->rows = rows;
    this->cols = cols;

    m_GrainCount = 0;

    colors.append(&color1);
    colors.append(&color2);
    colors.append(&color3);
    colors.append(&color4);

    arraySize = cols*rows;

    colStart = cols/2;
    rowStart = rows/2;

    m_Grains = new uchar[arraySize];
    m_GrainsCopy = new uchar[arraySize];

    for(uint i(0); i < rows*cols; i++){
        m_Grains[i] = 0;
        m_GrainsCopy[i] = 0;
    }
}

Worker::~Worker()
{
    delete [] m_Grains;
    delete [] m_GrainsCopy;
}

void Worker::startWorker()
{
    tElapsed.start();

    while(!atBorder){
        m_GrainCount++;

        addGrainAt(colStart, rowStart);

        //To prevent unwanted interim states in the ui
        if(m_Mutex.tryLock(0)){
            memcpy(m_GrainsCopy, m_Grains, arraySize);
            m_Mutex.unlock();
        }

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
        index1D = cols * y + x;
        m_Grains[index1D]++;

        if(m_Grains[index1D] > 3){
            //pile topples
            m_Grains[index1D] = 0;

            addGrainAt(x, --y);
            addGrainAt(++x, ++y);
            addGrainAt(--x, ++y);
            addGrainAt(--x, --y);
            x++;
        }
    }else
        atBorder = true;
}

void Worker::getData(uchar *image)
{
    m_Mutex.lock();
    for(uint i(0); i < arraySize; i++ ){
        memcpy(image + (i*4), colors.at(m_GrainsCopy[i]), 4);
    }
    m_Mutex.unlock();
}

const quint64 &Worker::getCount()
{
    return m_GrainCount;
}

