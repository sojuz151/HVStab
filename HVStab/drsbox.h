#ifndef DRSBOX_H
#define DRSBOX_H
#include "vector"

using namespace std;

class DRSBoxChannel
{
public: DRSBoxChannel(int ChannelNumber,
                      int DRSSerialNumber,
                      int DRSChannel) :
        channelNumber_(ChannelNumber),
        dRSSerialNumber_(DRSSerialNumber),
        dRSChannel_(DRSChannel)
        {}

    int GetChannelNumber(){return channelNumber_;}
    int GetDRSSerialNumber(){return dRSSerialNumber_;}
    int GetdrsChannel(){return dRSChannel_;}

private:
    int channelNumber_;
    int dRSSerialNumber_;
    int dRSChannel_;
};

class DRSBox
{
public:
    DRSBox();
    DRSBox(int BoxNumber,vector<DRSBoxChannel> BoxChannels) :
        dRSBoxNumber_(BoxNumber),
        dRSBoxChannels_(BoxChannels)
    {}

    int GetDRSBoxNumber(){return dRSBoxNumber_;}
    vector<DRSBoxChannel> GetDRSBoxChannels() {return dRSBoxChannels_;}

private:
  int dRSBoxNumber_;
  vector<DRSBoxChannel> dRSBoxChannels_;

};

#endif // DRSBOX_H
