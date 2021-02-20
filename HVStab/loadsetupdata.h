#ifndef LOADSETUPDATA_H
#define LOADSETUPDATA_H

#include "crate.h"
#include "pugixml.hh"
#include "drsbox.h"

using namespace std;

class LoadSetupData
{
public:
    LoadSetupData();
    ~LoadSetupData();
    void SetXmlInputFileName(std::string xmlFileName) {xmlInputFileName_ = xmlFileName;}
    void SetXmlOutputFileName(std::string xmlFileName) {xmlOutputFileName_ = xmlFileName;}
    void LoadSetupDataFromXML();
    void SaveSetupDatatoXML(Crate crate_);
    Crate GetCrate() {return crate_;}
    DRSBox GetDRSBox(){return DRSbox_;}
    string GetSetupDataFile() {return xmlInputFileName_;}
    string toStringPrecision(double input, int n);

private:
    string xmlInputFileName_;
    string xmlOutputFileName_;
    Crate crate_;
    DRSBox DRSbox_;
//    pugi::xml_document doc;
    pugi::xml_document docOut_;
};

#endif // LOADSETUPDATA_H
