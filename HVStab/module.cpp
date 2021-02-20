#include "module.h"

Module::Module()
{

}

Module::Module(int SlotNumber, string Name, bool StatusON, vector<Device> channels, int nofChannels)
{
   SetModuleSlotNumber(SlotNumber);
   SetModuleName(Name);
   SetModuleStatusON(StatusON);
   SetAllChannels(channels);
   SetNofChannels(nofChannels);
}

