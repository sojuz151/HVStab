#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "snmpwraper.h"
#include "mytimer.h"
#include "qcustomplot.h"

#include <iostream>
#include <fstream>

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QString>
#include <QProcess>
#include <QByteArray>
#include<unistd.h>
#include <QCloseEvent>



using namespace std;


void MainWindow::closeEvent (QCloseEvent *event)
{
    if(drsAmpMesure!=nullptr){
        cout << "Deleting old DRS..."<< endl;
        drsAmpMesure->close();
        event->accept();
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    crate_ = Crate();
// Setting up panels
    initialisation_ =  true;
    initTableData();

    // Initialisation variables:
        paletteGreen_.setColor(QPalette::Window, QColor(Qt::green));
        paletteRed_.setColor(QPalette::Window, QColor(Qt::red));
// Tooltips
//        ui->btn_ReadHV->setToolTip("Reads preset voltages from the modules");
        ui->btn_SystemCheck->setToolTip("Checks if HV at the crate level is enabled (ON)");
        ui->btn_HVStatus->setToolTip("Check each  status of HV for each channel and updates the table");
        ui->btn_LoadDataToCrate->setToolTip("Loads data from table to crate,modules,devices");
        ui->btn_Monitor->setToolTip("STARTs/STOPs monitoring of output voltage, updates table and graph");
        ui->btn_StartStab->setToolTip("STARTs/STOPs monitoring of the LP possitions , updates  graph");

// Action TOP line
    connect(ui->actionLoad_Setup, SIGNAL(triggered(bool)), this, SLOT(slotLoadSetup(bool)));
    connect(ui->actionSaveSetup, SIGNAL(triggered(bool)), this, SLOT(slotSaveSetup(bool)));
    connect(ui->actionCheck, SIGNAL(triggered(bool)),this, SLOT(slotMainSwitchCheck()));
    connect(ui->actionOn  , SIGNAL(triggered(bool)), this, SLOT(slotMainSwitchSetON(bool)));
    connect(ui->actionOff  , SIGNAL(triggered(bool)), this, SLOT(slotMainSwitchSetOFF(bool)));



// comunications
    connect(ui->btn_Close, SIGNAL(clicked()), this,  SLOT(close()));

    // -------------  HV  TAB -------------------

    connect(ui->lineXMLFile, SIGNAL(returnPressed()), this, SLOT(slotXMLFileUpdate()));
    connect(ui->lineIP, SIGNAL(returnPressed()), this, SLOT(slotIPChanged()));
    connect(ui->btn_HVStatus, SIGNAL(clicked()), this, SLOT(slotCheckCrateHVStatus()));
    connect(ui->btn_SystemCheck, SIGNAL(clicked()), this, SLOT(slotMainSwitchCheck()));
    connect(this, SIGNAL(systemCheck()),this, SLOT(slotMainSwitchCheck()));
    connect(ui->btn_LoadDataToCrate, SIGNAL(clicked()), this, SLOT(slotloadTableDataToCrate()));
    connect(ui->lineCrateAnswer, SIGNAL(textChanged(QString)), this, SLOT(slotNewCrateAnswer(QString)) );
    connect(this, SIGNAL(deviceDataChanged(bool)),this, SLOT(slotTableUpdate(bool)));
    connect(this, SIGNAL(graphRefresh()), this, SLOT(slotGraphRefresh()));
    connect(this, SIGNAL(checkCrateHVStatus()), this, SLOT(slotCheckCrateHVStatus()));
    connect(ui->tableData, SIGNAL(cellChanged(int, int)), this, SLOT(slotNewTableValue(int, int )));
    connect(ui->tableData_2, SIGNAL(cellChanged(int, int)), this, SLOT(slotNewTableValue(int, int )));

    //SpecPLot
    connect(ui->plotHV, SIGNAL(selectionChangedByUser()), this, SLOT(slotSelectionHVChanged()));
    connect(ui->plotLP, SIGNAL(selectionChangedByUser()), this, SLOT(slotSelectionLPChanged()));
    connect(ui->plotLPHist, SIGNAL(selectionChangedByUser()), this, SLOT(slotSelectionLPHChanged()));


    QThread* HVthread = new QThread;
    QThread* LPthread = new QThread;
   myHVTimer->moveToThread(HVthread);
   myLPTimer->moveToThread(LPthread);
//    connect(myTimer, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
//    connect(thread, SIGNAL(started()), myTimer, SLOT(slotCounter()));
//----HV  thread---
    connect(myHVTimer, SIGNAL(finished()), HVthread, SLOT(quit()));
    connect(myHVTimer, SIGNAL(finished()), myHVTimer, SLOT(deleteLater()));
    connect(HVthread, SIGNAL(finished()), HVthread, SLOT(deleteLater()));
    connect(this, SIGNAL(startmyHVTimer(bool)), myHVTimer, SLOT(slotHVCounter(bool)));
    connect(ui->btn_Monitor, SIGNAL(clicked(bool)), this, SLOT(slotHVTimerControl(bool)));
//    connect(ui->buttonEmergencyStop, SIGNAL(clicked(bool)), thread, SLOT(terminate()));
    connect(myHVTimer, SIGNAL(crateReadout()), this, SLOT(slotCrateReadout()));
    HVthread->start();


//  ----------------MATCHING TAB -----------------

//  ----------------STABILISATION TAB --------------

    connect(ui->btn_InitStab, SIGNAL(clicked(bool)), this, SLOT(slotInitStabilisation()));

    //------LP thread -----
    connect(myLPTimer, SIGNAL(finished()), LPthread, SLOT(quit()));
    connect(myLPTimer, SIGNAL(finished()), myLPTimer, SLOT(deleteLater()));
    connect(LPthread, SIGNAL(finished()), LPthread, SLOT(deleteLater()));
    connect(this, SIGNAL(startmyLPTimer(bool)), myLPTimer, SLOT(slotLPCounter(bool)));
    connect(ui->btn_StartStab, SIGNAL(clicked(bool)), this, SLOT(slotLPTimerControl(bool)));
 // to be replaced with the call to  DRS readout subroutine
    connect(myLPTimer, SIGNAL(crateReadout()), this, SLOT(slotCrateReadout()));
    connect(myLPTimer, SIGNAL(DRSAcq()),this, SLOT(slotStartStab()));
    LPthread->start();

}

void MainWindow::slotHVTimerControl(bool run)
{

   if (run)
    {

//    qDebug() << "RUN = TRUE emitting startmyTimer";
        myHVTimer->setStatus(true);
        bool ok;
        QString text = QInputDialog::getText(this, tr("Monitoring Interval"),
                                             tr("Monitoring interval (s)"),
                                             QLineEdit::Normal,
                                             "Interval in seconds", &ok);

        myHVTimer->setMonitoringInterval(text.toInt());
        ui->btn_Monitor->setText("STOP monitoring");
        emit startmyHVTimer(run);
    }
    if (!run)
    {
        myHVTimer->setStatus(false);
        ui->btn_Monitor->setText("START monitoring");
 //       qDebug() <<" RUN = FALSE emitting stopmyTimer" ;
    }
}


void MainWindow::initTableData()
{
    ui->tableData->setColumnWidth(0,50);
    ui->tableData->setColumnWidth(1,40);
    ui->tableData->setColumnWidth(2,50);
    ui->tableData->setColumnWidth(3,60);
    ui->tableData->setColumnWidth(4,60);
    ui->tableData->setColumnWidth(5,60);
    ui->tableData->setColumnWidth(6,60);
    ui->tableData->setColumnWidth(7,60);

    ui->tableData_2->setColumnWidth(0,50);
    ui->tableData_2->setColumnWidth(1,40);
    ui->tableData_2->setColumnWidth(2,50);
    ui->tableData_2->setColumnHidden(3,true);
    ui->tableData_2->setColumnHidden(4,true);
    ui->tableData_2->setColumnHidden(5,true);
    ui->tableData_2->setColumnHidden(6,true);
    ui->tableData_2->setColumnHidden(7,true);
}

void MainWindow::slotNewTableValue(int row, int column)
{

    if (!initialisation_)
    {
//    cout << "row " << row << " column " << column << endl;
    double current = deviceData_.at(row).at(5).toDouble();
    double voltage = deviceData_.at(row).at(3).toDouble();

    QString value = ui->tableData->item(row,column)->text();
      if (deviceData_.at(row).at(column) != value)
      {
      deviceData_[row].replace(column, value);
 //     qDebug() << deviceData_.at(row).at(column) << endl;
      int channelNumber = 0;
      QString location_;
      location_ = deviceData_.at(row).at(0);
      QStringList list_;
      list_ = location_.split(".");
      int cc = list_.at(0).toInt();
      int mm = list_.at(1).toInt();
      int dd = list_.at(2).toInt();
      channelNumber = cc*1000 + mm*100 + dd;
      if(column==5)current=value.toDouble();
      if(column==3) voltage=value.toDouble();
      hv newHV(current,voltage);
      crate_.GetOneModule(mm).GetOneChannel(dd).SetDeviceInitHV(newHV);

      SNMPWraper *snmp = new SNMPWraper(sysIP_);
      if(column==3)SNMPProces(snmp->SetOneChannel("outputVoltage",QString::number(channelNumber),voltage));
      if(column==5)SNMPProces(snmp->SetOneChannel("outputCurrent",QString::number(channelNumber),voltage));

      }
    }
}

void MainWindow::initPlot(QCustomPlot *plot)
{
    // set locale to english, so we get english month names:
    plot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

//    plot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
    // seconds of current time, we'll use it as starting point in time for data:
    double now = QDateTime::currentDateTime().toTime_t();
    plot->clearGraphs();
    plot->clearItems();
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);

    // show legend with slightly transparent background brush:
    plot->legend->setVisible(true);
    plot->legend->setBrush(QColor(255, 255, 255, 150));
    QFont legendFont =  font();
    legendFont.setPointSize(9);
    plot->legend->setFont(legendFont);
    plot->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items


   //make legend wrap text in two columns
    plot->legend->setWrap(2);
    plot->legend->setRowSpacing(1);
    plot->legend->setColumnSpacing(2);
    plot->legend->setFillOrder(QCPLayoutGrid::FillOrder::foColumnsFirst,true);

    for (int gi=0; gi<deviceData_.size(); ++gi)
    {
      int max_ =deviceData_.size();
        plot->addGraph();
//      QColor color(20+200/4.0*gi,70*(1.6-gi/4.0), 150, 150);
      QColor color(10+230/max_*gi,10, 120-(gi-max_/2)*120/max_, 255);
      plot->graph(gi)->setLineStyle(QCPGraph::LineStyle::lsLine);
//  old    ui->plotHV->graph(gi)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 7));
        plot->graph(gi)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, color, color, 2));
        plot->graph(gi)->setPen(QPen(color.lighter(200)));
//      ui->plotHV->graph(gi)->setBrush(QBrush(color));
        plot->graph(gi)->setName(deviceData_.at(gi).at(1));
        plot->graph(gi)->selectable();
        plot->graph(gi)->selectionDecorator()->setPen(QPen(Qt::green));
//        ui->plotHV->graph(gi)->setVisible(true);


    }
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("HH. mm \nd. MMMM yyyy");
    plot->xAxis->setTicker(dateTicker);
    // configure left axis text labels:
//    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
//    textTicker->addTick(10, "a bit\nlow");
//    textTicker->addTick(50, "quite\nhigh");
//    ui->plotHV->yAxis->setTicker(textTicker);
    // set a more compact font size for bottom and left axis tick labels:
    plot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    plot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    // set axis labels:
    plot->xAxis->setLabel("Time");
    if(plot == ui->plotHV) plot->yAxis->setLabel("HV (V)");
    if(plot == ui->plotLP) plot->yAxis->setLabel("LP centroid possition (ch)");
    if(plot == ui->plotLPHist) plot->yAxis->setLabel("DRS Channel");
    // make top and right axes visible but without ticks and labels:
    plot->xAxis2->setVisible(true);
    plot->yAxis2->setVisible(true);
    plot->xAxis2->setTicks(false);
    plot->yAxis2->setTicks(false);
    plot->xAxis2->setTickLabels(false);
    plot->yAxis2->setTickLabels(false);
    // set axis ranges to show all data:
    plot->xAxis->setRange(now-1800, now+100);
    plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

//    ui->plotHV->yAxis->setRange(-1, 60);

    plot->replot();
}

void MainWindow::slotSelectionHVChanged()
{
    slotSelectionChanged(ui->plotHV);
}

void MainWindow::slotSelectionLPChanged()
{
    slotSelectionChanged(ui->plotLP);
}

void MainWindow::slotSelectionLPHChanged()
{
    slotSelectionChanged(ui->plotLPHist);
}
void MainWindow::slotSelectionChanged(QCustomPlot *plot)
{
    for (int i=0; i<plot->graphCount(); ++i)
    {
         QCPGraph *graph = plot->graph(i);
          QCPPlottableLegendItem *item = plot->legend->itemWithPlottable(graph);
          if (item->selected() || graph->selected())
            {
               item->setSelected(true);
               graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
            }
    }
}


void MainWindow::fillTableData()
{
// Setting buttons in the table


    deviceData_.clear();
//    int devicetot_ = 0 ; // crate_.GetNumberofDevices();
    const int cc = 0;    // assuming a single crate system cc=0;
    ui->tableData->setRowCount(0);  // reseting number of rows in tables
    ui->tableData_2->setRowCount(0);
        QVector<QString> rowData_;
        for(unsigned mm=0; mm<crate_.GetAllModules().size(); mm++)
        {
//            std::cout << " liczba of channels in main = " << crate_.GetOneModule(mm).GetNofChannels() << endl;
            for(unsigned dd=0; dd<crate_.GetOneModule(mm).GetAllChannels().size(); dd++)
            {
 //               devicetot_++;
                rowData_.clear();
                string text_ = to_string(cc) + "." + to_string(mm) + "." + to_string(dd);
                rowData_.push_back(QString::fromStdString(text_));   // column 0 - Location
                rowData_.push_back(QString::fromStdString(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceName()));  //column 1 - Device Name
                rowData_.push_back("1");  //column 2 - Visible pushbutton
                rowData_.push_back(QString::fromStdString(to_string(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceInitHV().GetVoltage())));  //column 3 - V_init
                rowData_.push_back(QString::fromStdString(to_string(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceCurrHV().GetVoltage())));  //column 4 - V_mon
                rowData_.push_back(QString::fromStdString(to_string(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceInitHV().GetCurrent())));  //column 5 - I_init
                rowData_.push_back(QString::fromStdString(to_string(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceCurrHV().GetCurrent())));  //column 6 - I_mon
                rowData_.push_back(QString::fromStdString(to_string(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceStatusON())));  //column 7 - ON/OFF pushbutton
                qDebug() << rowData_;
                deviceData_.push_back(rowData_);
                ui->tableData->insertRow(ui->tableData->rowCount());
                ui->tableData_2->insertRow(ui->tableData_2->rowCount());

            }
        }
        bool change_ = true;
        emit deviceDataChanged(change_);
}

void MainWindow::slotCrateReadout()
{
//    qDebug() << "In slotCrateReadout";
    SNMPWraper *snmp = new SNMPWraper(sysIP_);

    SNMPProces(snmp->Walk("outputMeasurementSenseVoltage"));
    SNMPProces(snmp->Walk("outputMeasurementCurrent"));

//    emit checkCrateHVStatus();
    emit graphRefresh();

}


void MainWindow::slotReadHV()
{
    //    qDebug() << "In slotREadHV";
        SNMPWraper *snmp = new SNMPWraper(sysIP_);
        SNMPProces(snmp->Walk("outputVoltage"));
}

void MainWindow::slotGraphRefresh()     // refreshing HV monitor graph
{
    double now = QDateTime::currentDateTime().toTime_t();


    for(int gi=0; gi<deviceData_.size(); ++gi){
        if (deviceData_.at(gi).at(2)=="1"){
            ui->plotHV->graph(gi)->setVisible(true);
            ui->plotLP->graph(gi)->setVisible(true);
            ui->plotLPHist->graph(gi)->setVisible(true);
        }
        if (deviceData_.at(gi).at(2)=="0"){
            ui->plotHV->graph(gi)->setVisible(false);
            ui->plotLP->graph(gi)->setVisible(false);
            ui->plotLPHist->graph(gi)->setVisible(false);
        }
        QString location_;
        location_ = deviceData_.at(gi).at(0);
        QStringList list_;
        list_ = location_.split(".");
        int cc = list_.at(0).toInt();  // crate number
        int mm = list_.at(1).toInt();  // module number
        int dd = list_.at(2).toInt();   // device number


        ui->plotHV->graph(gi)->setData(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceHistoryHVkey(),crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceHistoryHVvalue());
//        if(drsAmpMesure!=nullptr){
//            auto index = drsAmpMesure-> indexMap.at(mm).at(dd);
//            QVector<double> wave(1024);
//            QVector<double> time(1024);
//            for(int locIndex=0; locIndex<1024;locIndex++){
//                wave[locIndex]= drsAmpMesure->wave_array[index][locIndex];
//                time[locIndex]= drsAmpMesure->time_array[index][locIndex];
//            }
//            ui->plotLP->graph(gi)->setData(time,wave);
//        }


    //temporary for LPHisto-grams setData(double x,double y)
        ui->plotLP->graph(gi)->setData(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceHistoryLPkey(),crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceHistoryLPvalue());
//        ui->plotLPHist->graph(gi)->setData(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceHistoryLPkey(),crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceHistoryLPvalue());
        ui->plotLPHist->graph(gi)->setData(crate_.GetOneModule(mm).GetOneChannel(dd).histLocations,crate_.GetOneModule(mm).GetOneChannel(dd).histValues);


        ui->plotHV->graph(gi)->rescaleValueAxis(false,true);
        ui->plotLP->graph(gi)->rescaleValueAxis(false,true);
        ui->plotLPHist->graph(gi)->rescaleValueAxis(false,true);

    }
    ui->plotHV->xAxis->setRange(now-3600, now+100);
    ui->plotHV->rescaleAxes(true);

    ui->plotLP->xAxis->setRange(now-3600, now+100);
    ui->plotLP->rescaleAxes(true);

//    double upperRange = ui->plotHV->graph()->getValueRange(bool foundrange, QCP::sdBoth, QCPRange()).upper; //*1.1;


    ui->plotHV->yAxis->setRangeUpper(ui->plotHV->yAxis->range().upper*1.005);
    ui->plotHV->yAxis->setRangeLower(ui->plotHV->yAxis->range().lower*0.995);

    ui->plotLP->yAxis->setRangeUpper(ui->plotLP->yAxis->range().upper*1.005);
    ui->plotLP->yAxis->setRangeLower(ui->plotLP->yAxis->range().lower*0.995);

    ui->plotLPHist->yAxis->setRangeUpper(ui->plotLPHist->yAxis->range().upper*1.005);
    ui->plotLPHist->yAxis->setRangeLower(ui->plotLPHist->yAxis->range().lower*0.995);

    ui->plotLPHist->xAxis->setRangeUpper(ui->plotLPHist->xAxis->range().upper*1.005);
    ui->plotLPHist->xAxis->setRangeLower(ui->plotLPHist->xAxis->range().lower*0.995);

    ui->plotHV->replot();
    ui->plotLP->replot();
    ui->plotLPHist->replot();
}

void MainWindow::slotCheckCrateHVStatus()
{
//    qDebug() << "In CheckCrate Status" ;
    SNMPWraper *snmp = new SNMPWraper(sysIP_);
    SNMPProces(snmp->Walk("outputSwitch"));
}

void MainWindow::slotMainSwitchCheck()
{
    SNMPWraper *snmp = new SNMPWraper(sysIP_);
    SNMPProces(snmp->Get("sysName.0"));
    SNMPProces(snmp->Get("sysMainSwitch.0"));
}

void MainWindow::slotMainSwitchSetON(bool change_)
{
    if(change_ )
    {
          SNMPWraper *snmp = new SNMPWraper(sysIP_);
          SNMPProces(snmp->SetOneChannel("sysMainSwitch.0", "0", 1));
          ui->actionOff->setChecked(false);
    }
}
void MainWindow::slotMainSwitchSetOFF(bool change_)
{
//    qDebug() << "MainSwithOFF" << change_;
    if(change_ )
    {
        SNMPWraper *snmp = new SNMPWraper(sysIP_);
        SNMPProces(snmp->SetOneChannel("sysMainSwitch.0", "0", 0));
        ui->actionOn->setChecked(false);

    }
}

void MainWindow::slotTableUpdate(bool change_)
{
    if (change_)
    {
//    qDebug() << "updating table" ;
        QString value = "";
        for(int i = 0; i<ui->tableData->rowCount();i++)
        {
            for(int j = 0; j<ui->tableData->columnCount();j++) //columns
            {

                if(j == 2)
                {
                    QWidget* pWidget = new QWidget();
                    QWidget* pWidgetLP = new QWidget();
                    QPushButton* visibleButton = new QPushButton();
                    QPushButton* visibleButton_2 = new QPushButton();
                    visibleButton->autoFillBackground();
                    visibleButton_2->autoFillBackground();
 //                   visibleButton->setText("Visible");
                    visibleButton->setCheckable(true);
                    visibleButton->setPalette(paletteGreen_);
                    visibleButton_2->setCheckable(true);
                    visibleButton_2->setPalette(paletteGreen_);
                    if(deviceData_.at(i).at(j)=="1")
                    {
                        visibleButton->setText("Visible");
                        visibleButton->setChecked(true);
                        visibleButton_2->setText("Visible");
                        visibleButton_2->setChecked(true);
                    }
                    else if(deviceData_.at(i).at(j)=="0")
                    {
                        visibleButton->setText("UnVisible");
                        visibleButton->setChecked(false);
                        visibleButton_2->setText("UnVisible");
                        visibleButton_2->setChecked(false);
                    }
                    connect(visibleButton, SIGNAL(clicked(bool)), this, SLOT(slotVisibleClicked(bool)));
                    connect(visibleButton_2, SIGNAL(clicked(bool)), this, SLOT(slotVisibleClicked(bool)));
//  connect(btn_edit, &QPushButton::clicked, this, &MainWindow::onClicked);
                    QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
                    pLayout->addWidget(visibleButton);
                    pLayout->setAlignment(Qt::AlignCenter);
                    pLayout->setContentsMargins(0, 0, 0, 0);
                    pWidget->setLayout(pLayout);
                    ui->tableData->setCellWidget(i, j, pWidget);
                    QHBoxLayout* pLayoutLP = new QHBoxLayout(pWidgetLP);
                    pLayoutLP->addWidget(visibleButton_2);
                    pLayoutLP->setAlignment(Qt::AlignCenter);
                    pLayoutLP->setContentsMargins(0, 0, 0, 0);
                    pWidgetLP->setLayout(pLayoutLP);
                    ui->tableData_2->setCellWidget(i, j, pWidgetLP);
                    continue;

                }
                if(j == 7)
                {
                    QWidget* pWidget = new QWidget();
                    QPushButton* statusButton = new QPushButton();
                    statusButton->autoFillBackground();
                    statusButton->setCheckable(true);
                    statusButton->setText("OFF");
                    statusButton->setPalette(paletteRed_);
                    if(deviceData_.at(i).at(j)=="1")
                    {
                        statusButton->setText("ON");
                        statusButton->setChecked(true);
                        statusButton->setPalette(paletteGreen_);
                    }
                    connect(statusButton, SIGNAL(clicked(bool)), this, SLOT(slotONOFFClicked(bool)));
                    QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
                    pLayout->addWidget(statusButton);
                    pLayout->setAlignment(Qt::AlignCenter);
                    pLayout->setContentsMargins(0, 0, 0, 0);
                    pWidget->setLayout(pLayout);
                    ui->tableData->setCellWidget(i, j, pWidget);
                    continue;

                }
 //               if(j!=4)   // leave V_mon for edit filling all the columns but 4
               else  {
                QTableWidgetItem*item = new QTableWidgetItem(deviceData_.at(i).at(j));
                QTableWidgetItem*item_2 = new QTableWidgetItem(deviceData_.at(i).at(j));
                if(j!=3)item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                if(j!=3)item_2->setFlags(item_2->flags() ^ Qt::ItemIsEditable);
                ui->tableData->setItem(i, j, item);
                ui->tableData_2->setItem(i, j, item_2);
               }

            }
        }

      //  emit graphRefresh();   <- fails when there are no data at the start-up.
          ui->plotHV->replot();
          ui->plotLP->replot();
          ui->plotLPHist->replot();
    }
}



void MainWindow::slotNewCrateAnswer(QString qstring)
{
    QStringList longList_;
    QVector<QStringList> vecList_;              // contains answer lines (vector) split into words (QStrings)
    QString qtmp_;
    bool changed_;
    changed_ = false;
    qstring.remove(" Opaque:");                 // removing Opaque from voltage and Current operations

    if(qstring.indexOf("\n") != qstring.lastIndexOf("\n"))  // for snmpWalk answers multichannel
     {
        longList_ = qstring.split("\n");
     }
      else if (qstring.indexOf("\n") == qstring.lastIndexOf("\n"))  //for single channel answers
     {
        qstring.remove("\n");                           // removing \n end line from string
        longList_.push_back(qstring);
     }
    for (int i=0; i< longList_.size(); i++)
     {
       vecList_.push_back(longList_.at(i).split(" "));
     }
//    qDebug() << "vecList in NewCrateAnswer:" << vecList_ ;

//    qDebug() << "vecList in NewCrateAnswer:" << vecList_.size() ;
//    qDebug() << "deviceData_.size() in NewCrateAnswer:" << deviceData_.size() ;

    unsigned limit = min(vecList_.size(),deviceData_.size());
    for (unsigned ik=0; ik< limit; ik++)
     {

      int jj = vecList_.at(ik).at(0).lastIndexOf("u");  // finds  the position of last u in first word
      QString qschannel_ = vecList_.at(ik).at(0).mid(jj+1,-1);  //getting channel number
     // CHENNEL NUMBER DECODING HERE
      int cc = 0 ; // crate number
      int mm = 0 ;  // module number
      int dd = 0 ; // channel/device number
      int ss=qschannel_.toInt();
       if (qschannel_.size()<=2)dd=ss;
       if (qschannel_.size()==3){ mm=ss/100; dd=ss-(mm*100);}
       if (qschannel_.size()==4){ cc=ss/1000; mm=(ss-cc*1000)/100; dd=ss-(mm*100);}
//       qDebug() << "crate: " << cc << "module: " << mm << "channel: " << dd;
      int ichannel;
      for (int i = 0; i < deviceData_.size(); i++)
       {
         string text_ = to_string(cc) + "." + to_string(mm) + "." + to_string(dd);
         if (text_ == deviceData_.at(i).at(0).toStdString()) ichannel = i;
        }
      QVector<QString> qvec = deviceData_.at(ichannel);     // should work for ALL modules - te be tested with FULL MTAS
    //finding what was the command
      int kk = vecList_.at(ik).at(0).lastIndexOf(":");      // should give the position of the comand
      int length = jj - kk - 2;                             // -2 to get rid of a dot
      QString command_ = vecList_.at(ik).at(0).mid(kk+1,length);
//    qDebug() << command_ ;
      if (vecList_.at(ik).size()>4)   // >4 is for answers values not the SWITCH
       {
         double value = vecList_.at(ik).at(3).toDouble() ;
         QString unit = vecList_.at(ik).at(4) ;
         if (command_ == "outputMeasurementSenseVoltage")
           {
            qvec.replace(4,vecList_.at(ik).at(3));
            changed_ = true;
 //           crate_.GetOneModule(mm).GetOneChannel(dd).AddHistoryHVData(value);
            hv currHV(value,0);
//            crate_.GetOneModule(mm).GetOneChannel(dd).SetDeviceCurrHV(currHV);
            Device device = crate_.GetOneModule(mm).GetOneChannel(dd);
            device.AddHistoryHVData(value);
            device.SetDeviceCurrHV(currHV);
            Module module = crate_.GetOneModule(mm);
            module.SetOneChannel(dd,device);
            crate_.SetOneModule(mm,module);
           }
         else if (command_ == "outputMeasurementCurrent")
           {
            qvec.replace(6,vecList_.at(ik).at(3));
            changed_ = true;
           }
         else if (command_ == "outputVoltage")
          {
           qvec.replace(3,vecList_.at(ik).at(3));
           changed_ = true;
          }
       }
      else if (vecList_.at(ik).size()==4 && command_ == "sysName.0")
      {
          ui->lab_Name->setText(vecList_.at(ik).at(3));
          crate_.SetCrateName(vecList_.at(ik).at(3).toStdString());
      }
      else if (vecList_.at(ik).size()==4 && command_ == "sysMainSwitch.0")
      {
          if ( vecList_.at(ik).at(3)=="on(1)" || vecList_.at(ik).at(3)=="On(1)")
          { sysMainSwitchON_ = "1";
            ui->lab_Status->setText("<font color='green'>ON</font>");
          }
          if ( vecList_.at(ik).at(3)=="off(0)" || vecList_.at(ik).at(3)=="Off(0)")
          {
              sysMainSwitchON_ = "0";
              ui->lab_Status->setText("<font color='red'>OFF</font>");
          }

      }
      else if (vecList_.at(ik).size()==4 && command_ == "outputSwitch")
       {
          QString statusString;
          QString statusHV_ = "0";
          if ( vecList_.at(ik).at(3)=="on(1)" || vecList_.at(ik).at(3)=="On(1)")statusHV_= "1";
          if ( vecList_.at(ik).at(3)=="off(0)" || vecList_.at(ik).at(3)=="Off(0)")statusHV_= "0";
          qvec.replace(7,statusHV_);
          changed_ = true;
       }
          deviceData_.replace(ichannel,qvec);


    }
    emit deviceDataChanged(changed_);

    if(deviceData_.size() == 0 )     // used when no initial data were loaded i.e. systemcheck
    {
        //finding what was the command
          int jj = vecList_.at(0).at(0).lastIndexOf("u");
          int kk = vecList_.at(0).at(0).lastIndexOf(":");      // should give the position of the comand
          int length = jj - kk - 2;                             // -2 to get rid of a dot
          QString command_ = vecList_.at(0).at(0).mid(kk+1,length);
        if (vecList_.at(0).size()==4 && command_ == "sysName.0")
        {
            ui->lab_Name->setText(vecList_.at(0).at(3));
            crate_.SetCrateName(vecList_.at(0).at(3).toStdString());
        }
        else if (vecList_.at(0).size()==4 && command_ == "sysMainSwitch.0")
        {

            if ( vecList_.at(0).at(3)=="on(1)" || vecList_.at(0).at(3)=="On(1)")
            { sysMainSwitchON_ = "1";
              crate_.SetCrateStatusON(true);
              ui->lab_Status->setText("<font color='green'>ON</font>");
              if(!ui->actionOn->isChecked())emit ui->actionOn->toggle();
              if(ui->actionOff->isChecked())emit ui->actionOff->toggle();

            }
            if ( vecList_.at(0).at(3)=="off(0)" || vecList_.at(0).at(3)=="Off(0)")
            {
                sysMainSwitchON_ = "0";
                crate_.SetCrateStatusON(false);
                ui->lab_Status->setText("<font color='red'>OFF</font>");
                if(!ui->actionOff->isChecked())emit ui->actionOff->toggle();
                if(ui->actionOn->isChecked())emit ui->actionOn->toggle();

            }

        }
    }

}

void MainWindow::slotVisibleClicked(bool)
{
    QWidget *w = qobject_cast<QWidget *>(sender()->parent());
    if(w)
     {
        int row = ui->tableData->indexAt(w->pos()).row();  //here also/
        bool btn_status_;
        if(deviceData_.at(row).at(2)=="1")btn_status_=true;
        if(deviceData_.at(row).at(2)=="0")btn_status_=false;

//        qDebug() << "device data for visible(=1)" << deviceData_.at(row).at(2) ;
        if(btn_status_)  //graph visible change to nonvisible
         {
            QVector<QString> qvec = deviceData_.at(row);
            qvec.replace(2,"0");
            deviceData_.replace(row, qvec);
            btn_status_ = false;
         }
        else   // (!btn_status)  graph not visible change to visible
         {
            QVector<QString> qvec = deviceData_.at(row);
            qvec.replace(2,"1");
            deviceData_.replace(row, qvec);         // setting ON for the deviceData_
            btn_status_ = true;
         }

//        qDebug() << "device data after change for visible(=1)" << deviceData_.at(row).at(2) ;

       QWidget* pWidget = new QWidget();
       QPushButton* visibleButton = new QPushButton();
       visibleButton->setCheckable(true);
       if(deviceData_.at(row).at(2)=="1")
           {
               visibleButton->setText("Visible");
               visibleButton->setChecked(true);
           }
       if(deviceData_.at(row).at(2)=="0")
           {
               visibleButton->setText("Not visible");
               visibleButton->setChecked(false);
           }

       connect(visibleButton, SIGNAL(clicked(bool)), this, SLOT(slotVisibleClicked(bool)));
       QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
       pLayout->addWidget(visibleButton);
       pLayout->setAlignment(Qt::AlignCenter);
       pLayout->setContentsMargins(0, 0, 0, 0);
       pWidget->setLayout(pLayout);
       ui->tableData->setCellWidget(row, 2, pWidget);
 //?      ui->tableData_2->setCellWidget(row, 2, pWidget);
       emit deviceDataChanged(true);
       emit graphRefresh();

     }
}

void MainWindow::ReadOut()
{
//    qDebug() << "In ReadOut" ;
    QProcess *p = dynamic_cast<QProcess *>( sender() );
    if (p)
      ans_ = QString::fromStdString(p->readAllStandardOutput().toStdString());
    ui->browserLog->moveCursor(QTextCursor::Start, QTextCursor::MoveMode(0)); //   QTextCursor::Start, 0);
//      ui->browserLog->backwardAvailable(true);
//     ui->browserLog->home();
      ui->browserLog->insertPlainText(ans_);
//      ui->browserLog->append( ans_ );  // p->readAllStandardError()
      ui->browserLog->reload();
      ui->lineCrateAnswer->setText(ans_ );
}

void MainWindow::ReadErr()
{
    cout << "In ReadErr" << endl;
    QProcess *p = dynamic_cast<QProcess *>( sender() );
//    QProcess *p = dynamic_cast<QProcess *>( sender()->parent() );
    if (p)
        ui->browserLog->append(  p->readAllStandardError());
    ui->browserLog->reload();
}

void MainWindow::SNMPProces(QString comand)
{
    QProcess *p = new QProcess( this );
    connect( p, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadOut()) );
    connect( p, SIGNAL(readyReadStandardError()), this, SLOT(ReadErr()) );

    if (p)
    {
      p->setEnvironment( QProcess::systemEnvironment() );
      p->setProcessChannelMode( QProcess::MergedChannels );
      p->start(comand);
      p->waitForStarted();
      p->waitForFinished(-1);
      p->waitForReadyRead(20000);
     }
}
void MainWindow::slotONOFFClicked(bool)
{
    QWidget *w = qobject_cast<QWidget *>(sender()->parent());

     if(w)
       {
        int row = ui->tableData->indexAt(w->pos()).row();

        bool btn_status_;
        if(deviceData_.at(row).at(7)=="1")btn_status_=true;
        if(deviceData_.at(row).at(7)=="0")btn_status_=false;
           // getting the right channel number for the multi-module system
        int channelNumber = 0;
        QString location_;
        location_ = deviceData_.at(row).at(0);
        QStringList list_;
        list_ = location_.split(".");
        int cc = list_.at(0).toInt();
        int mm = list_.at(1).toInt();
        int dd = list_.at(2).toInt();
        channelNumber = cc*1000 + mm*100 + dd;

//        qDebug() << channelNumber << " cc:" <<cc << "mm:" <<mm << "dd:" << dd;

        if (btn_status_)      // crate from ON to OFF
        {
            QVector<QString> qvec = deviceData_.at(row);
            qvec.replace(7,"0");


            SNMPWraper *snmp = new SNMPWraper(QString::fromStdString(crate_.GetCrateIP()));

           SNMPProces(snmp->SetOneChannel("outputSwitch",QString::number(channelNumber),0));


            deviceData_.replace(row, qvec);
            btn_status_ = false;
        }
        else //(!btn_status_)       // crate from OFF to ON
        {
            QVector<QString> qvec = deviceData_.at(row);
            qvec.replace(7,"1");
            SNMPWraper *snmp = new SNMPWraper(sysIP_);
           SNMPProces(snmp->SetOneChannel("outputSwitch",QString::number(channelNumber),1));

           deviceData_.replace(row, qvec);         // setting ON for the deviceData_
            btn_status_ = true;
        }

            for(unsigned i=0; i<crate_.GetAllModules().size(); i++)
            {
                for(unsigned j = 0; j<crate_.GetOneModule(i).GetAllChannels().size(); j++)
                {
                string name1 = crate_.GetOneModule(i).GetOneChannel(j).GetDeviceName();
                std::string name2 = deviceData_.at(row).at(1).toStdString();
                if(name1 == name2) crate_.GetOneModule(i).GetOneChannel(j).SetDeviceStatusON(btn_status_);  //setting status for the source of data
                }

            }
        QWidget* pWidget = new QWidget();
        QPushButton* statusButton = new QPushButton();
        statusButton->setCheckable(true);
        if(deviceData_.at(row).at(7)=="1")
            {
                statusButton->setText("ON");
                statusButton->setChecked(true);
            }
        if(deviceData_.at(row).at(7)=="0")
            {
                statusButton->setText("OFF");
                statusButton->setChecked(false);
            }

        connect(statusButton, SIGNAL(clicked(bool)), this, SLOT(slotONOFFClicked(bool)));
        QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
        pLayout->addWidget(statusButton);
        pLayout->setAlignment(Qt::AlignCenter);
        pLayout->setContentsMargins(0, 0, 0, 0);
        pWidget->setLayout(pLayout);
        ui->tableData->setCellWidget(row, 7, pWidget);
 //?       ui->tableData_2->setCellWidget(row, 7, pWidget);

       }

}

void MainWindow::slotSaveSetup(bool)
{
    QString QfileName;
      QfileName = QFileDialog::getSaveFileName(this,
          "Save Setup File ", QString::fromStdString(("XML file (*.xml)")));
      qDebug() << "Saving Setup to: " << QfileName;

    setupData_.SetXmlOutputFileName(QfileName.toStdString());
    setupData_.SaveSetupDatatoXML(crate_);
}

void MainWindow::slotLoadSetup(bool)
{
    QString QfileName;
      QfileName = QFileDialog::getOpenFileName(this,          "Load Setup File",QString(), QString::fromStdString(("XML file (*.xml)")));

      qDebug() << "Loading setup from: " << QfileName;

    setupData_.SetXmlInputFileName(QfileName.toStdString());
    setupData_.LoadSetupDataFromXML();
    crate_ = setupData_.GetCrate();
    drsBox = setupData_.GetDRSBox();
    sysIP_= QString::fromStdString(crate_.GetCrateIP());
    std::cout << "Crate name : " << crate_.GetCrateName()  << std::endl;
    std::cout << "Crate IP : " << sysIP_.toStdString()  << std::endl;
    ui->lineXMLFile->setText(QfileName);
    ui->lineIP->setText(QString::fromStdString(crate_.GetCrateIP()));
    emit systemCheck();
    fillTableData();
    initPlot(ui->plotHV);
    initPlot(ui->plotLP);
    initPlot(ui->plotLPHist);
    emit checkCrateHVStatus();
    initialisation_ = false;

}

void MainWindow::slotXMLFileUpdate()
{
       setupData_.SetXmlInputFileName(ui->lineXMLFile->text().toUtf8().constData());
       QMessageBox msgBox;
       msgBox.setText("Reloading the Setup file");
       msgBox.exec();      // poprawic obsluge msgBox zeby bylo Cancel
       std::cout << setupData_.GetSetupDataFile() << endl;
       setupData_.LoadSetupDataFromXML();
       crate_ =setupData_.GetCrate();  // reloading crate data to creat_ to have new data in the main
       sysIP_= QString::fromStdString(crate_.GetCrateIP());
       fillTableData();

    }

void MainWindow::slotIPChanged()
{
    QString ip = ui->lineIP->text();
    sysIP_ = ip;
    if(crate_.GetCrateIP()=="")
    {
        QMessageBox msgBox;
        msgBox.setText("No crate in the system, using an empty crate");
        msgBox.exec();
    }
    crate_.SetCrateIP(sysIP_.toStdString());
    emit systemCheck();

}

void MainWindow::slotloadTableDataToCrate()
{


    const int cc = 0;   //    assuming single crate system crate "0"
    int channelNumber = 0;
    for(unsigned mm=0; mm<crate_.GetAllModules().size(); mm++)
    {
        for(unsigned dd=0; dd<crate_.GetOneModule(mm).GetAllChannels().size(); dd++)
        {

        SNMPWraper *snmp = new SNMPWraper(sysIP_);
        double value = crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceInitHV().GetVoltage();
        channelNumber = cc*1000 + mm*100 + dd;
//        qDebug() << QString::number(channelNumber) << value;
        int ivalue = crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceStatusON();
        qDebug()<< ivalue << "przed outputswitch";
        //    SNMPProces(snmp->SetOneChannel("outputSwitch",QString::number(channelNumber),crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceStatusON()))
        SNMPProces(snmp->SetOneChannel("outputVoltage",QString::number(channelNumber),value));
        SNMPProces(snmp->SetOneChannel("outputCurrent",QString::number(channelNumber),crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceInitHV().GetCurrent()));

 //       QString::fromStdString(to_string(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceCurrHV().GetCurrent())));  //column 6 - I_mon
//        QString::fromStdString(to_string(crate_.GetOneModule(mm).GetOneChannel(dd).GetDeviceStatusON())));  //column 7 - ON/OFF pushbutton
//

        }
    }
}

//-----------------------Stabilisation routines below ------------------------
//int customChanelNumber = 0;
//double customDevicevolt = 0;
double voltDif = 5.0;
int sampCount = 500;
int binCount = 20;

void MainWindow::slotInitStabilisation()
{
    try{
        delete drsLogFile;
        drsLogFile = new ofstream("drsLog.data");
        *drsLogFile<<"# ";



        initPlot(ui->plotLP);   //initialising LP possition plot
        slotReadHV();           // read current HV from the power supply
        if(drsAmpMesure!=nullptr){
            cout << "Deleting old DRS..."<< endl;
            drsAmpMesure->close();
            delete drsAmpMesure;

        }
            cout << "Initializing DRS..."<< endl;
        drsAmpMesure = new DrsAmpMesure(300,4);
        drsAmpMesure->setIndexMap(crate_,drsBox);

        cout << "Starting mesurments.."<< endl;
        SNMPWraper *snmp = new SNMPWraper(sysIP_);
        vector<Module>* tempModVector = crate_.GetAllModulesPtr();
        for(ulong i=0;i<tempModVector->size();i++){
            vector<Device>* tempDevVector = tempModVector->at(i).GetAllChannelsPtr();
            for(ulong j=0;j<tempDevVector->size();j++){
                Device* device = &tempDevVector->at(j);
                double defVolt= device->GetDeviceInitHV().GetVoltage();
                SNMPProces(snmp->SetOneChannel("outputVoltage",QString::number(device->GetChannelNumber()),defVolt+voltDif));
                *drsLogFile<<device->GetDeviceName()<<" ";
            }
        }
        *drsLogFile<<std::endl;
        sleep(1);
        auto mes1 = drsAmpMesure->getConvertedAmps(std::chrono::seconds(1),sampCount,true);

        for(ulong i=0;i<tempModVector->size();i++){
            vector<Device>* tempDevVector = tempModVector->at(i).GetAllChannelsPtr();
            for(ulong j=0;j<tempDevVector->size();j++){
                Device* device = &tempDevVector->at(j);
                double defVolt= device->GetDeviceInitHV().GetVoltage();
                SNMPProces(snmp->SetOneChannel("outputVoltage",QString::number(device->GetChannelNumber()),defVolt-voltDif));
                device->SetDeviceSetHV(defVolt);
            }
        }
        sleep(1);
        auto mes2 = drsAmpMesure->getConvertedAmps(std::chrono::seconds(1),sampCount);

        for(ulong i=0;i<tempModVector->size();i++){
            vector<Device>* tempDevVector = tempModVector->at(i).GetAllChannelsPtr();
            for(ulong j=0;j<tempDevVector->size();j++){
                Device* device = &tempDevVector->at(j);
                double defVolt= device->GetDeviceInitHV().GetVoltage();
                SNMPProces(snmp->SetOneChannel("outputVoltage",QString::number(device->GetChannelNumber()),defVolt));

                double m1 = mes1.at(i).at(j).getAvrage();
                double m2 = mes2.at(i).at(j).getAvrage();
                cout <<"dev="<<device->GetDeviceName() <<" m1=" << m1<< " m2=" << m2<< endl;


                device->SetDeviceCurrLP(0.5*(m1+m2));

                vector<double> vec ;
                vec.push_back(m1 - (m1-m2)/(2*voltDif) * defVolt); //constant term
                vec.push_back((m1-m2)/(2*voltDif)); //linear term
                device->SetDeviceLPCal(vec);


                SNMPProces(snmp->SetOneChannel("outputVoltage",QString::number(device->GetChannelNumber()),device->GetDeviceInitHV().GetVoltage()));
                //        if((m1-m2)/voltDif<0.0)//jakas lepsza wartośc potem
                //            throw ;
            }
        }

        string fileName = "StabInit.xml";
        setupData_.SetXmlOutputFileName(fileName);   // setups output filename for reference purpose
        setupData_.SaveSetupDatatoXML(crate_);      // outputs all data including LP
        delete snmp;
        cout << "Done..."<< endl;
    } catch (const std::exception& e ) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error",e.what());

}

}

double stabCoef = 0.1;

void MainWindow::slotStartStab()
{
    stabilisationRunning_ = true;
    SNMPWraper *snmp = new SNMPWraper(sysIP_);
//    SNMPProces( snmp->Walk("outputMeasurementSenseVoltage"));
//    sleep(5);

    auto mes = drsAmpMesure->getConvertedAmps(std::chrono::seconds(1),sampCount);
    vector<Module>* tempModVector = crate_.GetAllModulesPtr();
    int totalCount = 0;
    *drsLogFile<<QDateTime::currentDateTime().toString(Qt::ISODate).toStdString()<<" ";
    for(ulong i=0;i<tempModVector->size();i++){
        vector<Device>* tempDevVector = tempModVector->at(i).GetAllChannelsPtr();
        for(ulong j=0;j<tempDevVector->size();j++){
            Device* device = &tempDevVector->at(j);

            double oldVolts =  device->GetDeviceSetHV();
            double m1 = mes.at(i).at(j).getAvrage();
            double newV =oldVolts - stabCoef * (m1- device->GetDeviceCurrLP())/device->GetDeviceLPCal()[1];
            cout <<"oldV="<<oldVolts <<" stab nV =" << newV<< " m = " <<m1 <<" om="<< device->GetDeviceCurrLP()<< endl;
            auto maxV = device->GetDeviceLimitHV().GetVoltage();
            if(maxV==0)
                maxV = 900;
            if(newV>maxV)
                newV = maxV;
            if(newV>maxV|| newV<0 )
                throw ;
            SNMPProces(snmp->SetOneChannel("outputVoltage",QString::number(device->GetChannelNumber()),newV));
            device->AddHistoryLPData(m1/device->GetDeviceCurrLP() + totalCount++*0);
            *drsLogFile<<oldVolts<<" "<<m1<<" ";
            device->SetDeviceSetHV(newV);
            //histValues, QVector<double> * locations,
            mes.at(i).at(j).setHist(&(device->histValues),&(device->histLocations),binCount);




//            device->singleChanellResult = mes.at(i).at(j).values;
        }
    }
    *drsLogFile<<std::endl;
    delete snmp;
}


void MainWindow::slotLPTimerControl(bool run)
{
//    qDebug() << "slotLPTimerCounter::start", run;
   if (run)
    {
//        qDebug() << "RUN = TRUE emitting startmyTimer";

        myLPTimer->setStatus(true);
        bool ok;
        QString text = QInputDialog::getText(this, tr("Stabilisation Monitoring Interval"),
                                             tr("Monitoring interval (s)"),
                                             QLineEdit::Normal,
                                             "Interval in seconds", &ok);

        myLPTimer->setMonitoringInterval(text.toInt());
        ui->btn_StartStab->setText("STOP stabil.");
        slotHVTimerControl(false);    // to stop HV readout if running
        ui->btn_Monitor->setEnabled(false);
        ui->btn_Monitor->setChecked(false);
        emit startmyLPTimer(run);
    }
    if (!run)
    {
//       qDebug() <<" RUN = FALSE emitting stopmyTimer" ;

        myLPTimer->setStatus(false);
        ui->btn_StartStab->setText("START stabil.");
        ui->btn_Monitor->setEnabled(true);

    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
