#include "device.h"

Device::Device()
{

}

Device::Device(int channelNumber,
                   string deviceName,
                   string deviceType,
                   hv initHV,
                   hv limitHV,
                   double initLP,
                   string deviceDRSAdress,
                   bool status){

    SetChannelNumber(channelNumber);
    SetDeviceName(deviceName);
    SetDeviceType(deviceType);
    SetDeviceInitHV(initHV);
    SetDeviceLimitHV(limitHV);
    SetDeviceStatusON(status);
    hv currHV(0.0, 0.0);   // to fill zeros in to the table not any random values
    SetDeviceCurrHV(currHV);
    SetDeviceInitLP(initLP);
    SetDeviceCurrLP(600);
 //   AddHistoryHVData(0.0);
    SetDeviceDRSAdress(deviceDRSAdress);
}

void Device::AddHistoryHVData(double value)
{
    deviceHistoryHVkey_.push_back(QDateTime::currentDateTime().toTime_t());
    deviceHistoryHVvalue_.push_back(value);
//   qDebug() << "nowa wartosc  HV dodana" << value << deviceHistoryHVvalue_.size();
}

void Device::AddHistoryLPData(double value)
{
    deviceHistoryLPkey_.push_back(QDateTime::currentDateTime().toTime_t());
    deviceHistoryLPvalue_.push_back(value);
//   qDebug() << "nowa wartosc LP dodana" << value << deviceHistoryLPvalue_.size();
}
