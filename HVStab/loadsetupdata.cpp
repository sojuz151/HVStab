#include "loadsetupdata.h"
// #include "pugixml.hh"
#include "crate.h"
#include "drsbox.h"

#include    <iostream>
#include    <vector>
#include    <string>
#include <iomanip>
#include <sstream>

#include <QApplication>
#include <QTranslator>
#include <QDialog>
#include <QFileDialog>
//#include <QDebug>
#include <QMessageBox>
#include <QString>


double stringToDouble (std::string strValue)
{
    QString qstr = QString::fromStdString(strValue);
    return qstr.toDouble();
}

using namespace std;

LoadSetupData::LoadSetupData()
{

}
LoadSetupData::~LoadSetupData()
{

}

void LoadSetupData::SaveSetupDatatoXML(Crate crate_)
{


    string now_str =  QDateTime::currentDateTime().toString().toStdString();
    pugi::xml_node save = docOut_.append_child("Save");
    save.append_attribute("Time") = now_str.c_str();
        pugi::xml_node crate = docOut_.append_child("Crate");
//    crate.set_value("KRATA");
    crate.append_attribute("Name") = crate_.GetCrateName().c_str();
    crate.append_attribute("IP") = crate_.GetCrateIP().c_str();
    crate.append_attribute("ControlerType") = crate_.GetControlerType().c_str();
    crate.append_attribute("CrateStatusON") = crate_.GetCrateStatusON();
    crate.append_attribute("CrateHV_StatusON") = crate_.GetCrateHVStatusON();
    for(unsigned mm = 0; mm<crate_.GetNumberofModules(); mm++)
    {
    pugi::xml_node module = crate.append_child("Module");
        module.append_attribute("Name") = crate_.GetOneModule(mm).GetModuleName().c_str();
        module.append_attribute("SlotNumber") =  crate_.GetOneModule(mm).GetModuleSlotNumber();
        module.append_attribute("StatusON") = crate_.GetOneModule(mm).GetModuleStatusON();
        module.append_attribute("NofChannels") =  crate_.GetOneModule(mm).GetNofChannels();
        for (unsigned dd=0; dd<crate_.GetOneModule(mm).GetNofDevices(); dd++)
        {
            pugi::xml_node channel = module.append_child("Channel");
            channel.append_attribute("ChannelNumber") =  crate_.GetOneModule(mm).GetOneChannel(dd).GetChannelNumber();
            channel.append_attribute("Name") = crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceName().c_str();
            channel.append_attribute("Type") =crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceType().c_str();
            channel.append_attribute("InitV").set_value(toStringPrecision(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceInitHV().GetVoltage(),2).c_str());
            channel.append_attribute("InitI").set_value(toStringPrecision(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceInitHV().GetCurrent(),5).c_str());
            channel.append_attribute("UnitI") = crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceInitHV().GetCurrentUnit().c_str();
            channel.append_attribute("LimitV").set_value(toStringPrecision(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceLimitHV().GetVoltage(),2).c_str());
            channel.append_attribute("LimitI").set_value(toStringPrecision(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceLimitHV().GetCurrent(),5).c_str());
            channel.append_attribute("StatusON") = crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceStatusON();
            channel.append_attribute("InitLP").set_value(toStringPrecision(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceInitLP(),2).c_str());
            channel.append_attribute("CurrV").set_value(toStringPrecision(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceCurrHV().GetVoltage(),2).c_str());
            channel.append_attribute("CurrLP").set_value(toStringPrecision(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceCurrLP(),2).c_str());
        }
     }
    if (!docOut_.save_file(xmlOutputFileName_.c_str()))
    {
        QMessageBox msgBox;
        string text = "Error saving data to "+ xmlOutputFileName_ + " file.";
        msgBox.setText(QString::fromStdString(text) );
        msgBox.exec();
    }

}

void LoadSetupData::LoadSetupDataFromXML()
{
    pugi::xml_document doc;
    qDebug() << !doc.load_file(xmlInputFileName_.c_str()) ;
    if (!doc.load_file(xmlInputFileName_.c_str()))
    {
        QMessageBox msgBox;
        string text = "Error connecting to "+ xmlInputFileName_ + " file.";
        msgBox.setText(QString::fromStdString(text) );
        msgBox.exec();
    }

    pugi::xml_node crate = doc.child("Crate");
    string crateName_ = crate.attribute("Name").value();
    string crateIP_ = crate.attribute("IP").value();
    string crateType_ = crate.attribute("ControlerType").value();
    bool crateStatusON_ =crate.attribute("CrateStatusON").as_bool();
    bool crateHV_StatusON = crate.attribute("CrateHV_StatusON").as_bool();
    std::vector<Module> modules;
    for (pugi::xml_node module = crate.first_child(); module; module = module.next_sibling())
    {
        int moduleSlotNumber = module.attribute("SlotNumber").as_int();
        string moduleName = module.attribute("Name").value();
        bool modulestatus = module.attribute("StatusON").as_bool();
        int moduleNofChannels = module.attribute("NofChannels").as_int();
        std::vector<Device> devices;
        for (pugi::xml_node device = module.first_child(); device; device = device.next_sibling())
        {
            int channelNumber = device.attribute("ChannelNumber").as_int();
            string deviceName = device.attribute("Name").value();
            string deviceType = device.attribute("Type").value();
//            double initV = device.attribute("InitV").as_double();
//            double initI = device.attribute("InitI").as_double();
            string initV_str = device.attribute("InitV").value();
            double initV = stringToDouble(initV_str);
            string initI_str = device.attribute("InitI").value();
            double initI = stringToDouble(initI_str);
            string unitI = device.attribute("UnitI").value();
//            double limitV = device.attribute("LimitV").as_double();
//            double limitI = device.attribute("LimitI").as_double();
            string limitV_str = device.attribute("LimitV").value();
            string limitI_str = device.attribute("LimitI").value();
            string initLP_str = device.attribute("InitLP").value();
            string DRSAdress = device.attribute("DRSAdress").value();
            double limitV = stringToDouble(limitV_str);
            double limitI = stringToDouble(limitI_str);
            double initLP = stringToDouble(initLP_str);
            if (unitI == "mA"){initI=initI/1000; limitI=limitI/1000;}
            else if (unitI == "A"){initI=initI; limitI=limitI;}
            else if (unitI == "uA"){initI=initI/1e6; limitI=limitI/1e6;}
            else {initI=initI/1000; limitI=limitI/1000;}   // for safety

            hv initHV;
            initHV.SetCurrent(initI);
            initHV.SetVoltage(initV);
            initHV.SetCurrentUnit("A");
            initHV.SetVoltageUnit("V");
            hv limitHV;
            limitHV.SetCurrent(limitI);
            limitHV.SetVoltage(limitV);
            initHV.SetCurrentUnit("A");
            initHV.SetVoltageUnit("V");
            bool status = device.attribute("StatusON").as_bool();

            devices.push_back(Device(channelNumber, deviceName, deviceType,
                                     initHV, limitHV, initLP, DRSAdress, status));

        }



        modules.push_back(Module(moduleSlotNumber, moduleName, modulestatus, devices, moduleNofChannels));
    }

    crate_ = Crate(crateName_, crateType_, modules, crateStatusON_, crateHV_StatusON, crateIP_);

       std::cout << "przd DRSBox" << endl;

    pugi::xml_node DRSBoxpugi = doc.child("DRSBox");
    int DRSBoxNumber_ = DRSBoxpugi.attribute("DRSBoxNumber").as_int();

    std:vector<DRSBoxChannel>DRSBoxChannels;
    for (pugi::xml_node DRSBoxChannelpugi = DRSBoxpugi.first_child(); DRSBoxChannelpugi; DRSBoxChannelpugi = DRSBoxChannelpugi.next_sibling())
    {
       int channelNumber = DRSBoxChannelpugi.attribute("ChannelNumber").as_int();
       int DRSSerialNumber = DRSBoxChannelpugi.attribute("DRSSerialNumber").as_int();
       int DRSChannel = DRSBoxChannelpugi.attribute("DRSChannel").as_int();

       DRSBoxChannels.push_back(DRSBoxChannel(channelNumber,DRSSerialNumber,DRSChannel));
    }

    DRSbox_ = DRSBox(DRSBoxNumber_, DRSBoxChannels);
}

std::string LoadSetupData::toStringPrecision(double input, int n)
{
     stringstream stream;
     stream << std::fixed << setprecision(n) << input;
     return stream.str();
}

