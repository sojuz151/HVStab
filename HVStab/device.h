#ifndef DEVICE_H
#define DEVICE_H

#include<hv.h>

#include<string>
#include<vector>
#include <QVector>
#include <qcustomplot.h>


using namespace std;

class Device
{
public:
    Device();
    Device(int channelNumber,
           string deviceName,
           string deviceType,
           hv initHV,
           hv limitHV,
           double initLP,
           string deviceDRSAdress,
           bool status);

    string GetDeviceName() {return deviceName_;}
    string GetDeviceType() {return deviceType_;}
    hv GetDeviceInitHV() {return deviceInitHV_;}
    hv GetDeviceCurrHV() {return deviceCurrHV_;}
    double GetDeviceSetHV(){return setHV_;}
    double GetDeviceInitLP() {return deviceInitLP_;}
    double GetDeviceCurrLP() {return deviceCurrLP_;}
    bool GetDeviceStatusON() {return deviceStatusON_;}    // 0(false) = OFF ; 1(true) = ON
    hv GetDeviceLimitHV() {return deviceLimitHV_;}
    QVector<double> GetDeviceHistoryHVkey() {return deviceHistoryHVkey_;}
    QVector<double> GetDeviceHistoryHVvalue() {return deviceHistoryHVvalue_;}
    QVector<double> GetDeviceHistoryLPkey() {return deviceHistoryLPkey_;}
    QVector<double> GetDeviceHistoryLPvalue() {return deviceHistoryLPvalue_;}
    int GetChannelNumber() {return deviceChannelNumber_;}
    vector<double> GetDeviceLPCal() {return deviceLPCal_;}
    string GetDeviceDRSAdress(){return deviceDRSAdress_;}

    void SetChannelNumber(int channel) {deviceChannelNumber_ = channel;}
    void SetDeviceName(string device) {deviceName_ = device;}
    void SetDeviceType(string type) {deviceType_ = type;}
    void SetDeviceInitHV(hv initHV) {deviceInitHV_ = initHV;}
    void SetDeviceCurrHV(hv currHV) {deviceCurrHV_ = currHV;}
    void SetDeviceInitLP(double initLP) {deviceInitLP_ = initLP;}
    void SetDeviceCurrLP(double currLP) {deviceCurrLP_ = currLP;}
    void SetDeviceLPCal(vector<double> LPcal){deviceLPCal_ = LPcal;}
    void SetDeviceDRSAdress(string s){deviceDRSAdress_ = s;}
    void SetDeviceStatusON(bool status) {deviceStatusON_ = status;}
    void SetDeviceLimitHV(hv limitHV) {deviceLimitHV_ = limitHV;}
    void SetDeviceSetHV(double setHV) {setHV_ = setHV;}
    void AddHistoryHVData(double value);
    void AddHistoryLPData(double value);
    string deviceDRSAdress_;
    QVector<double> histLocations;
    QVector<double> histValues;




private:
    int deviceChannelNumber_;
    string deviceName_;
    string deviceType_;
    hv deviceInitHV_;
    hv deviceCurrHV_;
    double deviceInitLP_;
    double deviceCurrLP_;
    double deviceLPLimitMin_;
    double deviceLPLimitMax_;
    double setHV_;
    vector<double> deviceLPCal_;



    bool deviceStatusON_;
    hv deviceLimitHV_;
    QVector<double> deviceHistoryHVkey_;   // zminiec na typ DATA (time,y)
    QVector<double> deviceHistoryHVvalue_;
    QVector<double> deviceHistoryLPkey_;
    QVector<double> deviceHistoryLPvalue_;

};

#endif // DEVICE_H
