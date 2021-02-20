#include "crate.h"

#include <iostream>
using namespace std;

Crate::Crate()
{

}
Crate::Crate(string IP)
{
    SetCrateIP(IP);
}

Crate::Crate(string Name, string controlerType, vector<Module> modules,
             bool statusON, bool hv_StatusON, string IP)
{
    SetCrateName(Name);
    SetControlerType(controlerType);
    SetAllModules(modules);
    SetCrateStatusON(statusON);
    SetCrateHVStatusON(hv_StatusON);
    SetCrateIP(IP);


}

int Crate::GetNumberofDevices()
{
    int numberOfDevices_ = 0;
    for (int i =0 ; i < module_.size() ; i++)
    {
        numberOfDevices_ =+ module_.at(i).GetNofChannels();
    }
//    std::cout << " number of DEvicecs = " << numberOfDevices_ << std::endl;
    return numberOfDevices_;
}
