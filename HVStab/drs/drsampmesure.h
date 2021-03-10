#ifndef DRSAMPMESURE_H
#define DRSAMPMESURE_H

#include "DRS.h"
#include <vector>
#include "crate.h"
#include "drsbox.h"
#include <chrono>
class SingleChanellResult{
    public:
    void addPoint(double p ){
        values.push_back(p);
    }
    double getAvrage(){
        return accumulate( values.begin(), values.end(), 0.0)/values.size();
    }
    std::vector<double> values;
    void setHist( QVector<double>* histValues, QVector<double>* locations,int count);
};


class DrsAmpMesure
{

    DRSBoard *b, *mb;
    DRS *drs;
public:
    std::vector<std::vector<int> > indexMap;
    float time_array[8][1024];
    float wave_array[8][1024];
    DrsAmpMesure(float delayNs,float frequency);
    void setIndexMap(Crate crate,DRSBox DRSbox);

    std::vector<std::vector<SingleChanellResult > > getConvertedAmps(std::chrono::duration<double>,int,bool=false);
    std::vector<SingleChanellResult> getAmp(std::chrono::duration<double>, int,bool);
    void close(){
        /* delete DRS object -> close USB connection */
        delete drs;
    }
};

#endif // DRSAMPMESURE_H
