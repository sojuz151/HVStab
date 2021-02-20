#ifndef HV_H
#define HV_H

#include <string>

using namespace std;

class hv
{
public:
    hv();
    hv(double voltage, double current);

    double GetVoltage(){return voltage_;}
    double GetCurrent(){return current_;}
    string GetVoltageUnit(){return voltageUnit_;}
    string GetCurrentUnit(){return currentUnit_;}
    void SetVoltage(double voltage) {voltage_ = voltage;}
    void SetCurrent(double current) {current_ = current;}
    void SetVoltageUnit(string unit) {voltageUnit_ = unit;}
    void SetCurrentUnit(string unit) {currentUnit_ = unit;}


private:
    double voltage_;
    double current_;
    string voltageUnit_;
    string currentUnit_;

};

#endif // HV_H
