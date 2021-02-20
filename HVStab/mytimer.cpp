#include "mytimer.h"
#include "unistd.h"
#include <QDebug>

MyTimer::MyTimer()
{
    status_ = true;
    interval_= 10; // 10 seconds as a starting point for interval

}

MyTimer::~MyTimer()
{

}

void MyTimer::slotHVCounter(bool run)
{
    double i = 0;
    while (run && status_)
    {
//        qDebug() << " licznik" << i ;
    emit crateReadout();    // starts readout of the voltage and current in crates
    i++;
    sleep(interval_);  //in seconds
    }
    if(!run) this->~MyTimer();

}

void MyTimer::slotLPCounter(bool run)
{
    double i = 0;
    while (run && status_)
    {
//        qDebug() << "W LP counter licznik" << i ;
// emit signal to be replace with the emit signal for start_DRS_ACQ
    emit crateReadout();    // starts readout of the voltage and current in crates
    emit DRSAcq();    // to start DRS acquisition
        i++;
    sleep(interval_);  //in seconds
    }
    if(!run) this->~MyTimer();

}
