#include "hv.h"

hv::hv()
{

}
hv::hv(double voltage, double current)
{
    SetVoltage(voltage);
    SetCurrent(current);
    string curentUnit = "A";
    string voltageUnit = "V";
    SetCurrentUnit(curentUnit);
    SetVoltageUnit(voltageUnit);
}
