#ifndef SNMPWRAPER_H
#define SNMPWRAPER_H

#include "snmpcomands.h"

#include <QString>
#include <QObject>

//#include <QProcess>

class SNMPWraper : public QObject
{
public:
    SNMPWraper(QObject *parent = 0);
    SNMPWraper(QString ip);

    void SettingCommands();
    void SettingFlags();
    void SetFlagIP(QString ip) {flagIP_ = ip+" ";}

    QString GetFlagIP() {return flagIP_;}

// SNMP calls
    QString SetOneChannel(QString comand, QString channel, double fvalue);
    void SetAll(QString comand, double voltage);

    QString GetOneChannel(QString comand, QString channel);
    QString GetAll(QString comand);
    QString Walk(QString comand);
    QString Get(QString commans);  // executes command as it is with predifined flags, no channels

public slots:
     void slotReadOut();
     void slotReadErr();

private:
    QString flagOptions_;
    QString flagVersion_;
    QString flagPath_;
    QString flagMIBFile_;
    QString flagOwner_;
    QString flagIP_;
    SNMPcomands comands_;
    QString flagCommand_;
    QString flagChannel_;
    double voltage_ ;
    QString flagType_;
//    QString flagAcess_;
    QString value_;


};

#endif // SNMPWRAPER_H
