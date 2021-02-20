#include "snmpwraper.h"

#include <QProcess>
#include <QDebug>

#include <mainwindow.h>

using namespace std;

SNMPWraper::SNMPWraper(QObject *parent)
{
    SettingCommands();
    SettingFlags();

}

SNMPWraper::SNMPWraper(QString ip)
{
    SettingCommands();
    SetFlagIP(ip);
    SettingFlags();

}

void SNMPWraper::SettingCommands()
{
//ComandNames definitions
    SNMPcomands *outputName = new SNMPcomands("outputName", "S","R","output channel name");
    SNMPcomands *outputVoltage = new SNMPcomands("outputVoltage", "F", "RW", "The channel set Voltage");
    SNMPcomands *outputCurrent = new SNMPcomands("outputCurrent", "F", "RW", "The channel current limit");
    SNMPcomands *outputMeasurementSenseVoltage = new SNMPcomands("outputMeasurementSenseVoltage",
                                                                 "F", "R",
                                                                 "Actual channel Voltage at sense line");
    SNMPcomands *outputMeasurementTerminalVoltage = new SNMPcomands("outputMeasurementTerminalVoltage",
                                                                    "F", "R",
                                                                    "Actual channel Voltage at terminal");
    SNMPcomands *outputMeasurementCurrent = new SNMPcomands("outputMeasurementCurrent",
                                                            "F", "R",
                                                            "Actual channels current");
    SNMPcomands *outputSwith = new SNMPcomands("outputSwitch", "i", "RW",
                                               "Turns channel OFF(0) / ON(1), resetEmergencyOff(2) setEmergencyOff(3), clearEvents(10)");

    SNMPcomands *outputVoltageRiseRate = new SNMPcomands("outputVoltageRiseRate", "F", "RW",
                                                         "Channels ramp up rate");
    SNMPcomands *outputVoltageFallRate = new SNMPcomands("outputVoltageFallRate", "F","RW",
                                                         "Channel ramp down rate");
    SNMPcomands *outputStatus = new SNMPcomands("outputStatus","Bits", "R","Channel status information");
}
void SNMPWraper::SettingFlags()
{

    flagOptions_ = " "; // -O qv
    flagVersion_ = "-v 2c ";
    flagPath_ = "";
    flagMIBFile_ = " -m +WIENER-CRATE-MIB " ;
    flagOwner_ = " -c public ";
//    flagIP_ = "10.13.3.233 ";  //should be set in the constructor

}

QString SNMPWraper::Get(QString comand)
{
    flagOptions_ =" ";
    flagOwner_ = "-c public " ;
    flagCommand_ = comand;
    QString master;
    master = "snmpget " + flagOptions_ + flagVersion_ +flagPath_ + flagMIBFile_
                                   + flagOwner_ + flagIP_ + flagCommand_;
//    qDebug() << "MASTER Get" << master;

    return master;
}



QString SNMPWraper::GetOneChannel(QString comand, QString channel)
{
    flagOptions_ = " "; //-O qv
    flagOwner_ = "-c public ";

    flagCommand_ = comand + ".u" + channel;

    QString master;
    master = "snmpget " + flagOptions_ + flagVersion_ +flagPath_ + flagMIBFile_
                                   + flagOwner_ + flagIP_ + flagCommand_;
//    qDebug() << "MASTER GetOne" << master;

    return master;
}

QString SNMPWraper::SetOneChannel(QString comand, QString channel, double fvalue)
{
    flagOptions_ = " ";
    flagOwner_ = "-c guru ";
    if(comand=="outputVoltage")flagType_ = "F ";
    if(comand=="outputSwitch")flagType_ = "i ";
    flagCommand_ = comand + ".u" + channel + " ";

    if(comand=="sysMainSwitch.0")    // to accomodate a global seting commands
    {
        flagType_ = "i ";
        flagCommand_ = comand + " ";
    }
    value_ = QString::number(fvalue);
    QString master;
    master = "snmpset " + flagOptions_ + flagVersion_ +flagPath_ + flagMIBFile_
                                   + flagOwner_ + flagIP_ + flagCommand_ + flagType_
                                + value_;
//    qDebug() << "MASTER SetOneChannel" << master;

    return master;
}

QString SNMPWraper::Walk(QString comand)
{
    flagOptions_ = " "; //-O qv
    flagOwner_ = "-c public ";

    flagCommand_ = comand;

    QString master;
    master = "snmpwalk " + flagOptions_ + flagVersion_ +flagPath_ + flagMIBFile_
                                   + flagOwner_ + flagIP_ + flagCommand_;
 //   qDebug() << "MASTER GetOne" << master;

    return master;
}
