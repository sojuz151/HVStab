#include "drsampmesure.h"

#include <math.h>
#include <string>
#ifdef _MSC_VER

#include <windows.h>

#elif defined(OS_LINUX)

#define O_BINARY 0

#include <unistd.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <errno.h>

#define DIR_SEPARATOR '/'

#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "strlcpy.h"
#include "DRS.h"
#include <stdexcept>
#include <vector>
#include <sstream>
#include <iostream>



DrsAmpMesure::DrsAmpMesure(float delayNs,float frequency){
    /* do initial scan, sort boards accordning to their serial numbers */
    drs = new DRS();
    drs->SortBoards();

    /* show any found board(s) */
    for (int i=0 ; i<drs->GetNumberOfBoards() ; i++) {
       b = drs->GetBoard(i);
       printf("Found DRS4 evaluation board, serial #%d, firmware revision %d\n",
          b->GetBoardSerialNumber(), b->GetFirmwareVersion());
       if (b->GetBoardType() < 8) {
          throw  std::invalid_argument("Found pre-V4 board, aborting. Try to power cycle drs.");
       }
    }

    /* exit if no board found */
    if (drs->GetNumberOfBoards() == 0) {
        throw  std::invalid_argument("No DRS4 evaluation board found\n");
    }

    /* use first board with highest serial number as the master board */
    mb = drs->GetBoard(0);

    /* common configuration for all boards */
    for (int i=0 ; i<drs->GetNumberOfBoards() ; i++) {
       b = drs->GetBoard(i);

       /* initialize board */
       b->Init();

       /* select external reference clock for slave modules */
       /* NOTE: this only works if the clock chain is connected */
       if (i > 0) {
          if (b->GetFirmwareVersion() >= 21260) { // this only works with recent firmware versions
             if (b->GetScaler(5) > 300000)        // check if external clock is connected
                b->SetRefclk(true);               // switch to external reference clock
          }
       }

       /* set sampling frequency */
       b->SetFrequency(frequency, true);

       /* set input range to -0.5V ... +0.5V */
       b->SetInputRange(0);

       /* enable hardware trigger */
       b->EnableTrigger(1, 0);
       b->SetTriggerSource(1<<4);
//       b->SetTriggerPolarity(false);
       b->SetTriggerDelayNs(delayNs-30*i);

//       if (i == 0) {
//          /* master board: enable hardware trigger on CH1 at 50 mV positive edge */
////          b->SetTranspMode(1);
//          b->SetTriggerSource(1<<4);
//          b->SetTriggerPolarity(false);
//          b->SetTriggerDelayNs(delayNs);
//       } else {
//          /* slave boards: enable hardware trigger on Trigger IN */
//          b->SetTriggerSource(1<<4);
//          b->SetTriggerPolarity(false);
//       }
    }
}
std::vector<SingleChanellResult> DrsAmpMesure::getAmp(std::chrono::duration<double> timeOut, int maxCount,bool save){
    FILE  *f;
    auto startTime = std::chrono::system_clock::now();

    if(save){
        f = fopen("data.txt", "w");
        if (f == NULL) {
           throw("ERROR: Cannot open file \"data.txt\"");
        }
    }
    int boardCount = drs->GetNumberOfBoards();
    int iterCount = 0 ;
    std::vector<SingleChanellResult> accu(4*boardCount) ;
    while (iterCount<=maxCount && std::chrono::system_clock::now()-startTime<timeOut) {
        /* start boards (activate domino wave), master is last */
        for (int j=boardCount-1 ; j>=0 ; j--)
           drs->GetBoard(j)->StartDomino();
        /* wait for trigger on master board */
        while (mb->IsBusy());


        for (int board=0 ; board<drs->GetNumberOfBoards() ; board++) {
           b = drs->GetBoard(board);
           if (b->IsBusy()) {
              iterCount--; /* skip that event, must be some fake trigger */ //ok, cokolwiek
              break;
           }
           /* read all waveforms from all boards */
           b->TransferWaves(0, 8);
          for(int chanell =0;chanell<4;chanell++){
                /* read time (X) array in ns */
                b->GetTime(0, chanell*2, b->GetTriggerCell(0), time_array[chanell]);
                /* decode waveform (Y) arrays in mV */
                b->GetWave(0, chanell*2, wave_array[chanell]);
                int n=100;
                int nStart = 10;
                double baseline = 0.0;

                for (int k=nStart; k<nStart+n; k++)
                   baseline+= wave_array[chanell][k];
                baseline/=n;

                double localAcu =0;
                for (int k=0 ; k<1024 ; k++)
                   localAcu+= wave_array[chanell][k]-baseline; //sum of integral per channel. Can be done much better but for now it is fine.

                accu[4*board+chanell].addPoint( localAcu);

          }
          if(save){
              fprintf(f, "#Board #%d ---------------------------------------------------\n# t1[ns]  u1[mV]  t2[ns]  u2[mV]  t3[ns]  u3[mV]  t4[ns]  u4[mV]\n", b->GetBoardSerialNumber());
              for (int k=0 ; k<1024 ; k++)
                 fprintf(f, "%7.3f %7.1f %7.3f %7.1f %7.3f %7.1f %7.3f %7.1f\n",
                         time_array[0][k], wave_array[0][k],
                         time_array[1][k], wave_array[1][k],
                         time_array[2][k], wave_array[2][k],
                         time_array[3][k], wave_array[3][k]);

            }
        }
        iterCount++;
    }
    if(save){
        fclose(f);
    }
    return accu;
}


std::vector<std::vector<SingleChanellResult> > DrsAmpMesure::getConvertedAmps(std::chrono::duration<double> timeOut, int maxCount,bool save){
    auto data = getAmp(timeOut,maxCount,save);
    std::vector<std::vector <SingleChanellResult> > ret;
    for(ulong i =0; i<indexMap.size();i++){
        ret.push_back({});
        for(ulong j= 0; j<indexMap[i].size();j++){
            auto value = data[indexMap[i][j]];
            ret[i].push_back(value);
        }
    }
    return ret;
}
void  DrsAmpMesure::setIndexMap(    Crate crate_,DRSBox dRSbox){
    //    assuming single crate system crate "0"
    indexMap.clear();
    for(unsigned mm=0; mm<crate_.GetAllModules().size(); mm++)
    {
        indexMap.push_back({});
        auto module = crate_.GetAllModulesPtr()->at(mm);
        auto size = module.GetAllChannelsPtr()->size();
        for(unsigned dd=0; dd<size; dd++)
            {
            auto device =  module.GetAllChannelsPtr()->at(dd);
            int box=0,boxChanell=0;

            ulong j = 0;
            while(device.deviceDRSAdress_.at(j)!='.'){
                 box=box *10 + device.deviceDRSAdress_.at(j) -'0';
                 j++;
            }
            j++;
            while(j< device.deviceDRSAdress_.size()){
                 boxChanell= boxChanell*10 + device.deviceDRSAdress_.at(j) -'0';
                 j++;
            }

             auto chanells = dRSbox.GetDRSBoxChannels();
            int k =0;
            for(;;k++)
            {
                if(chanells.at(k).GetChannelNumber() == boxChanell)
                    break;
            }
            auto chanell = chanells.at(k);
            int i=0;
            for(;;i++)
            {
                if(drs->GetBoard(i)->GetBoardSerialNumber() == chanell.GetDRSSerialNumber())
                    break;
            }
            indexMap[mm].push_back(4*i+chanell.GetdrsChannel());
            std::cout<<"mm = "<<mm<<" dd = "<<dd <<" i="<<i<<" chanel="<<chanell.GetdrsChannel() <<endl;
        }
    }
}
void  SingleChanellResult::setHist( QVector<double> * histValues, QVector<double> * locations,int count){
    double max = *std::min_element(values.begin() ,values.end()); //reversed polarity
    double min = *std::max_element(values.begin() ,values.end());
    double av = getAvrage();
    locations->resize(count);
    histValues->resize(count);
    for(int i=0;i<count;i++){
        (*locations)[i] = (min+ (max-min)*i*1.0/count)/av;
        (*histValues)[i]= 0;
    }
    for(int i=0;i<values.size();i++){
        int index = ((values[i] - min) /(max-min))*count;
        if(index>=count)
            index = count-1;
        (*histValues)[index]++;
    }


}

