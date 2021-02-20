#ifndef MYTIMER_H
#define MYTIMER_H

#include <QObject>

class MyTimer : public QObject
{
    Q_OBJECT
public:
   MyTimer();
//    explicit MyTimer(QObject *parent = 0);

    void setStatus(bool status){status_ = status;}
    void setMonitoringInterval(int interval){interval_ =interval;}

   ~MyTimer();

signals:
   void finished();
   void error(QString err);
   void crateReadout();
   void DRSAcq();

public slots:
   void slotHVCounter(bool run);
   void slotLPCounter(bool run);

private:
   bool status_;
   int interval_;
};

#endif // MYTIMER_H
