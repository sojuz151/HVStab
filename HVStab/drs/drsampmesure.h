#ifndef DRSAMPMESURE_H
#define DRSAMPMESURE_H

#include "DRS.h"
#include <vector>
#include "crate.h"
#include "drsbox.h"


class DrsAmpMesure
{

    DRSBoard *b, *mb;
     std::vector<std::vector<int> > indexMap;
     DRS *drs;
public:

    float time_array[8][1024];
    float wave_array[8][1024];
    DrsAmpMesure(float delayNs,float frequency);
    void setIndexMap(Crate crate,DRSBox DRSbox);

    std::vector<std::vector<double > > getConvertedAmps(float,int,bool=false);
    std::vector<double> getAmp(float, int,bool);
    void close(){
        /* delete DRS object -> close USB connection */
        delete drs;
    }
};

#endif // DRSAMPMESURE_H
