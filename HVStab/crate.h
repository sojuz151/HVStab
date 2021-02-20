#ifndef CRATE_H
#define CRATE_H

#include <module.h>

#include <string>
#include <vector>

//using namespace std;

class Crate
{
public:
    Crate();
    Crate(string IP);
    Crate(string Name, string controlerType, vector<Module> modules,
          bool statusON, bool hv_StatusON, string IP);
//Getters
    string GetCrateName() {return crateName_;}
    string GetControlerType() {return controlerType_;}
    vector<Module> GetAllModules() {return module_;}
        vector<Module>* GetAllModulesPtr() {return &module_;}
    Module GetOneModule(int i) {return module_.at(i);}

    bool GetCrateStatusON() {return crateStatusON_;}
    bool GetCrateHVStatusON() {return crateHV_StatusON_;}
    string GetCrateIP() {return crateIP_;}
    int GetNumberofDevices() ;
    int GetNumberofModules() {return module_.size();}
    int GetCrateNumber() {return crateNumber_;}

// Setters
    void SetCrateName(string name) {crateName_ = name;}
    void SetControlerType(string type) {controlerType_ = type;}
    void SetAllModules(vector<Module> modules) {module_ = modules;}
    void SetOneModule(int i, Module module) {module_[i] = module;}
    void SetCrateStatusON(bool status) {crateStatusON_ = status;}
    void SetCrateHVStatusON(bool statusHV) {crateHV_StatusON_ = statusHV;}
    void SetCrateIP(string IP) {crateIP_ = IP;}
    void SetCrateNumber(int number) {crateNumber_ = number;}

private:
   string crateName_;
   string controlerType_;
   vector<Module> module_;
   bool crateStatusON_;
   bool crateHV_StatusON_;
   string crateIP_;
   int crateNumber_;
};

#endif // CRATE_H
