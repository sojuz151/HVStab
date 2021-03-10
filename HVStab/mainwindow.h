#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <loadsetupdata.h>
#include <mytimer.h>

#include <string>

#include <QMainWindow>

#include "drs/drsampmesure.h"
//#include <QString>

using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initTableData();
    void fillTableData();
    void SNMPProces(QString);
    void closeEvent (QCloseEvent *event);


    void initPlot(QCustomPlot *plot);

public slots:
    void slotXMLFileUpdate();           /**< Loads file name for the XML configuration file */
    void slotLoadSetup(bool);           /**< Loads configuration setup from the chosen file */
    void slotSaveSetup(bool);           /**< Save configuration set-up to file */
    void slotVisibleClicked(bool);      /**< If true set the device data to be visible on the graph and repots the graf*/
    void slotONOFFClicked(bool);        /**< Turnes ON/OFF one device from the table */
    void ReadOut();
    void ReadErr();
    void slotNewCrateAnswer(QString);   /**< Processes informations send back from system as an answer to SNMP commands*/
    void slotTableUpdate(bool);
    void slotCrateReadout();
    void slotHVTimerControl(bool);
    void slotLPTimerControl(bool run);
    void slotGraphRefresh();
    void slotSelectionHVChanged();
    void slotSelectionLPChanged();
    void slotSelectionLPHChanged();
    void slotSelectionChanged(QCustomPlot *plot);
    void slotCheckCrateHVStatus();      // check status of the HV on all channels
    void slotIPChanged();  //sets the local IP variable
    void slotMainSwitchCheck();
    void slotMainSwitchSetON(bool change_);
    void slotMainSwitchSetOFF(bool change_);
    void slotloadTableDataToCrate();
    void slotReadHV();
    void slotNewTableValue(int row, int column);
    void slotInitStabilisation();
    void slotStartStab();

signals:
    void deviceDataChanged(bool);
    void startmyHVTimer(bool);
    void startmyLPTimer(bool);
    void graphRefresh();
    void checkCrateHVStatus();
    void systemCheck();

private:
    Ui::MainWindow *ui;

    string xMLFileName_;
    LoadSetupData setupData_;
    Crate crate_;               // main data holder
    DRSBox drsBox;
    QVector<QVector<QString>>deviceData_;   // vector of rows in the table for active devices
    QPalette paletteGreen_;
    QPalette paletteRed_;
    QString ans_;       //Answer from the crate to be deconvoluted by program
//    QThread* thread = new QThread;
    MyTimer* myHVTimer = new MyTimer();
    MyTimer* myLPTimer = new MyTimer();
    QString sysIP_;
    QString sysMainSwitchON_;
    bool initialisation_;
    bool stabilisationRunning_;
    DrsAmpMesure* drsAmpMesure = nullptr;
    ofstream* drsLogFile = nullptr;

};

#endif // MAINWINDOW_H
