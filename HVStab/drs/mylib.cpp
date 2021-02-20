
#include <math.h>

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

/*------------------------------------------------------------------*/



DRS *drs;
DRSBoard *b;   
extern "C"{
    
    
    void MyLibInit(){
           /* do initial scan */
            drs = new DRS();
               /* show any found board(s) */
            for (int i=0 ; i<drs->GetNumberOfBoards() ; i++) {
                b = drs->GetBoard(i);
                printf("Found DRS4 evaluation board, serial #%d, firmware revision %d\n", 
                    b->GetBoardSerialNumber(), b->GetFirmwareVersion());
            }
            int nBoards = drs->GetNumberOfBoards();
            /* exit if no board found */
            if (nBoards == 0) {
                printf("No DRS4 evaluation board found\n");
                return ;
            }
            /* continue working with first board only */
            b = drs->GetBoard(0);
               /* initialize board */
            b->Init();
               /* enable transparent mode needed for analog trigger */
            b->SetTranspMode(1);
               /* use following line to turn on the internal 100 MHz clock connected to all channels  */
            b->EnableTcal(1);
            // enable hardware trigger
            b->EnableTrigger(1, 0); 
    }
    void MyLibSetFrequency(double demand, bool wait){
        b->SetFrequency( demand,  wait);        
    }
    void MyLibSetTriggerSource(int source){ //4 - EKSTERNAL 
        b->SetTriggerSource(1<<source);  
    }
    void MyLibSetTriggerPolarity(bool negative){
        b->SetTriggerPolarity(negative);
    }
    void MyLibSetTriggerLevel(double voltage){
        b->SetTriggerLevel( voltage);
    }
    void MyLibSetTriggerDelayNs(int delay){
        b->SetTriggerDelayNs(delay);
    }
    void MyLibSetInputRange(double center){
        b->SetInputRange(center);
    }
    void MyLibstart(){
      /* start board (activate domino wave) */
      b->StartDomino();
    }
    void MyLibUnInit(){
        delete drs;
    }
    bool MyLibCheckIfOk(){
        return !b->IsBusy();
    }

    void MyLibGetData(float * time_array, float * wave_array ){
        b->TransferWaves(0, 8);
        for(u_char i =0;i<1024;i++){
            // read time (X) array 
            b->GetTime(0, i, b->GetTriggerCell(0), wave_array+1024*sizeof(float));
            // decode waveform (Y) array 
            b->GetWave(0, i, wave_array+1024*sizeof(float) );
        }
    }
}

