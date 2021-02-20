#ifndef MODULE_H
#define MODULE_H

#include <device.h>

class Module
{
public:
    Module();
    Module(int SlotNumber,
           string Name,
           bool StatusON,
           vector<Device> channels,
           int nofChannels);

    int GetModuleSlotNumber() {return moduleSlotNumber_;}
    string GetModuleName() {return moduleName_;}
    bool GetModuleStatusON() {return moduleStatusON_;}
    vector<Device> GetAllChannels() {return channel_;}
    vector<Device>* GetAllChannelsPtr() {return &channel_;}
    int GetNofChannels() {return nofChannels_;}
    Device GetOneChannel(int i) {return channel_.at(i);}

    int GetNofDevices() {return channel_.size();}

    void SetModuleSlotNumber(int slot) {moduleSlotNumber_ = slot;}
    void SetModuleName(string name) {moduleName_ =name;}
    void SetModuleStatusON(bool status) {moduleStatusON_ = status;}
    void SetAllChannels(vector<Device> channel) {channel_ = channel;}
    void SetNofChannels(int number) {nofChannels_ = number;}
    void SetOneChannel(int i, Device channel) {channel_[i] = channel;}

private:
    int moduleSlotNumber_;
    string moduleName_;
    bool moduleStatusON_;
    vector<Device> channel_;
    int nofChannels_;
};

#endif // MODULE_H
