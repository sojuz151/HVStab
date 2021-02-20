/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad_Setup;
    QAction *actionCheck;
    QAction *actionOn;
    QAction *actionOff;
    QAction *actionSaveSetup;
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *HVTab;
    QTableWidget *tableData;
    QFrame *frame;
    QLabel *label;
    QLineEdit *lineXMLFile;
    QLabel *lableXML;
    QLineEdit *lineIP;
    QLabel *labelIP;
    QLabel *lab_sysName;
    QLabel *lab_Name;
    QLabel *lab_sysStatus;
    QLabel *lab_Status;
    QTextBrowser *browserLog;
    QCustomPlot *plotHV;
    QPushButton *btn_Monitor;
    QPushButton *btn_HVStatus;
    QPushButton *btn_SystemCheck;
    QPushButton *btn_LoadDataToCrate;
    QWidget *MatchingTab;
    QWidget *tabStabilisation;
    QCustomPlot *plotLP;
    QPushButton *btn_InitStab;
    QPushButton *btn_StartStab;
    QLineEdit *led_initalLPfileName;
    QLineEdit *led_currentLPfileName;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QCheckBox *ch_Polarity;
    QSpinBox *spinBox;
    QPushButton *btn_Close;
    QLineEdit *lineCrateAnswer;
    QPushButton *buttonEmergencyStop;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHV_System;
    QMenu *menuON_OFF;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1068, 753);
        actionLoad_Setup = new QAction(MainWindow);
        actionLoad_Setup->setObjectName(QString::fromUtf8("actionLoad_Setup"));
        actionLoad_Setup->setEnabled(true);
        actionCheck = new QAction(MainWindow);
        actionCheck->setObjectName(QString::fromUtf8("actionCheck"));
        actionOn = new QAction(MainWindow);
        actionOn->setObjectName(QString::fromUtf8("actionOn"));
        actionOn->setCheckable(true);
        actionOff = new QAction(MainWindow);
        actionOff->setObjectName(QString::fromUtf8("actionOff"));
        actionOff->setCheckable(true);
        actionOff->setChecked(false);
        actionSaveSetup = new QAction(MainWindow);
        actionSaveSetup->setObjectName(QString::fromUtf8("actionSaveSetup"));
        actionSaveSetup->setEnabled(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setGeometry(QRect(0, 0, 1061, 651));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        HVTab = new QWidget();
        HVTab->setObjectName(QString::fromUtf8("HVTab"));
        tableData = new QTableWidget(HVTab);
        if (tableData->columnCount() < 8)
            tableData->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableData->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableData->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableData->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableData->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableData->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableData->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableData->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableData->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        tableData->setObjectName(QString::fromUtf8("tableData"));
        tableData->setGeometry(QRect(620, 10, 591, 461));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(2);
        sizePolicy1.setHeightForWidth(tableData->sizePolicy().hasHeightForWidth());
        tableData->setSizePolicy(sizePolicy1);
        QFont font;
        font.setPointSize(8);
        tableData->setFont(font);
        tableData->setRowCount(0);
        tableData->setColumnCount(8);
        tableData->horizontalHeader()->setDefaultSectionSize(70);
        tableData->verticalHeader()->setVisible(false);
        tableData->verticalHeader()->setDefaultSectionSize(20);
        frame = new QFrame(HVTab);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(840, 470, 221, 151));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 10, 101, 16));
        lineXMLFile = new QLineEdit(frame);
        lineXMLFile->setObjectName(QString::fromUtf8("lineXMLFile"));
        lineXMLFile->setGeometry(QRect(60, 30, 141, 21));
        lableXML = new QLabel(frame);
        lableXML->setObjectName(QString::fromUtf8("lableXML"));
        lableXML->setGeometry(QRect(0, 30, 60, 16));
        lineIP = new QLineEdit(frame);
        lineIP->setObjectName(QString::fromUtf8("lineIP"));
        lineIP->setGeometry(QRect(60, 50, 141, 21));
        labelIP = new QLabel(frame);
        labelIP->setObjectName(QString::fromUtf8("labelIP"));
        labelIP->setGeometry(QRect(0, 50, 60, 16));
        lab_sysName = new QLabel(frame);
        lab_sysName->setObjectName(QString::fromUtf8("lab_sysName"));
        lab_sysName->setGeometry(QRect(0, 70, 101, 16));
        lab_Name = new QLabel(frame);
        lab_Name->setObjectName(QString::fromUtf8("lab_Name"));
        lab_Name->setGeometry(QRect(100, 70, 111, 20));
        lab_sysStatus = new QLabel(frame);
        lab_sysStatus->setObjectName(QString::fromUtf8("lab_sysStatus"));
        lab_sysStatus->setGeometry(QRect(0, 90, 121, 16));
        lab_Status = new QLabel(frame);
        lab_Status->setObjectName(QString::fromUtf8("lab_Status"));
        lab_Status->setGeometry(QRect(130, 90, 60, 16));
        browserLog = new QTextBrowser(HVTab);
        browserLog->setObjectName(QString::fromUtf8("browserLog"));
        browserLog->setGeometry(QRect(0, 520, 701, 101));
        plotHV = new QCustomPlot(HVTab);
        plotHV->setObjectName(QString::fromUtf8("plotHV"));
        plotHV->setGeometry(QRect(10, 10, 601, 511));
        btn_Monitor = new QPushButton(HVTab);
        btn_Monitor->setObjectName(QString::fromUtf8("btn_Monitor"));
        btn_Monitor->setGeometry(QRect(700, 590, 140, 32));
        btn_Monitor->setCheckable(true);
        btn_HVStatus = new QPushButton(HVTab);
        btn_HVStatus->setObjectName(QString::fromUtf8("btn_HVStatus"));
        btn_HVStatus->setGeometry(QRect(700, 530, 140, 32));
        btn_SystemCheck = new QPushButton(HVTab);
        btn_SystemCheck->setObjectName(QString::fromUtf8("btn_SystemCheck"));
        btn_SystemCheck->setEnabled(true);
        btn_SystemCheck->setGeometry(QRect(700, 500, 140, 32));
        btn_LoadDataToCrate = new QPushButton(HVTab);
        btn_LoadDataToCrate->setObjectName(QString::fromUtf8("btn_LoadDataToCrate"));
        btn_LoadDataToCrate->setGeometry(QRect(700, 560, 140, 32));
        tabWidget->addTab(HVTab, QString());
        MatchingTab = new QWidget();
        MatchingTab->setObjectName(QString::fromUtf8("MatchingTab"));
        tabWidget->addTab(MatchingTab, QString());
        tabStabilisation = new QWidget();
        tabStabilisation->setObjectName(QString::fromUtf8("tabStabilisation"));
        plotLP = new QCustomPlot(tabStabilisation);
        plotLP->setObjectName(QString::fromUtf8("plotLP"));
        plotLP->setGeometry(QRect(0, 80, 1051, 541));
        btn_InitStab = new QPushButton(tabStabilisation);
        btn_InitStab->setObjectName(QString::fromUtf8("btn_InitStab"));
        btn_InitStab->setGeometry(QRect(130, 10, 121, 32));
        btn_StartStab = new QPushButton(tabStabilisation);
        btn_StartStab->setObjectName(QString::fromUtf8("btn_StartStab"));
        btn_StartStab->setGeometry(QRect(250, 10, 113, 32));
        btn_StartStab->setCheckable(true);
        led_initalLPfileName = new QLineEdit(tabStabilisation);
        led_initalLPfileName->setObjectName(QString::fromUtf8("led_initalLPfileName"));
        led_initalLPfileName->setGeometry(QRect(590, 20, 113, 21));
        led_currentLPfileName = new QLineEdit(tabStabilisation);
        led_currentLPfileName->setObjectName(QString::fromUtf8("led_currentLPfileName"));
        led_currentLPfileName->setGeometry(QRect(840, 20, 113, 21));
        label_2 = new QLabel(tabStabilisation);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(490, 20, 91, 16));
        label_3 = new QLabel(tabStabilisation);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(720, 20, 91, 16));
        label_4 = new QLabel(tabStabilisation);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(100, 50, 101, 20));
        ch_Polarity = new QCheckBox(tabStabilisation);
        ch_Polarity->setObjectName(QString::fromUtf8("ch_Polarity"));
        ch_Polarity->setEnabled(true);
        ch_Polarity->setGeometry(QRect(290, 50, 131, 24));
        ch_Polarity->setChecked(true);
        spinBox = new QSpinBox(tabStabilisation);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(211, 50, 61, 27));
        spinBox->setMaximum(2000);
        spinBox->setSingleStep(50);
        tabWidget->addTab(tabStabilisation, QString());
        btn_Close = new QPushButton(centralWidget);
        btn_Close->setObjectName(QString::fromUtf8("btn_Close"));
        btn_Close->setGeometry(QRect(900, 650, 71, 32));
        lineCrateAnswer = new QLineEdit(centralWidget);
        lineCrateAnswer->setObjectName(QString::fromUtf8("lineCrateAnswer"));
        lineCrateAnswer->setGeometry(QRect(0, 650, 701, 21));
        buttonEmergencyStop = new QPushButton(centralWidget);
        buttonEmergencyStop->setObjectName(QString::fromUtf8("buttonEmergencyStop"));
        buttonEmergencyStop->setGeometry(QRect(752, 650, 151, 32));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1068, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuFile->setEnabled(true);
        menuHV_System = new QMenu(menuBar);
        menuHV_System->setObjectName(QString::fromUtf8("menuHV_System"));
        menuON_OFF = new QMenu(menuHV_System);
        menuON_OFF->setObjectName(QString::fromUtf8("menuON_OFF"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHV_System->menuAction());
        menuFile->addAction(actionLoad_Setup);
        menuFile->addAction(actionSaveSetup);
        menuHV_System->addAction(actionCheck);
        menuHV_System->addAction(menuON_OFF->menuAction());
        menuON_OFF->addAction(actionOn);
        menuON_OFF->addAction(actionOff);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionLoad_Setup->setText(QCoreApplication::translate("MainWindow", "Load Setup", nullptr));
        actionCheck->setText(QCoreApplication::translate("MainWindow", "Check", nullptr));
        actionOn->setText(QCoreApplication::translate("MainWindow", "On", nullptr));
        actionOff->setText(QCoreApplication::translate("MainWindow", "Off", nullptr));
        actionSaveSetup->setText(QCoreApplication::translate("MainWindow", "SaveSetup", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableData->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Location", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableData->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableData->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Visible", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableData->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "V_init (V)", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableData->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "V_mon (V)", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableData->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "I_init (A)", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableData->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "I_mon (A)", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableData->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Initial Settings", nullptr));
        lableXML->setText(QCoreApplication::translate("MainWindow", "XML file", nullptr));
        labelIP->setText(QCoreApplication::translate("MainWindow", "Crate IP:", nullptr));
        lab_sysName->setText(QCoreApplication::translate("MainWindow", "System Name:", nullptr));
        lab_Name->setText(QCoreApplication::translate("MainWindow", "n/a", nullptr));
        lab_sysStatus->setText(QCoreApplication::translate("MainWindow", "System HV status:", nullptr));
        lab_Status->setText(QCoreApplication::translate("MainWindow", "unknown", nullptr));
        btn_Monitor->setText(QCoreApplication::translate("MainWindow", "START monitorig", nullptr));
        btn_HVStatus->setText(QCoreApplication::translate("MainWindow", "Check HV Status", nullptr));
        btn_SystemCheck->setText(QCoreApplication::translate("MainWindow", "System check", nullptr));
        btn_LoadDataToCrate->setText(QCoreApplication::translate("MainWindow", "Load Data to Crate", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(HVTab), QCoreApplication::translate("MainWindow", "HV", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(MatchingTab), QCoreApplication::translate("MainWindow", "Matching", nullptr));
        btn_InitStab->setText(QCoreApplication::translate("MainWindow", "Initialisation", nullptr));
        btn_StartStab->setText(QCoreApplication::translate("MainWindow", "START Stabil.", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Initial file name", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Current  data", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Trigger Level", nullptr));
        ch_Polarity->setText(QCoreApplication::translate("MainWindow", "Negative Polarity", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabStabilisation), QCoreApplication::translate("MainWindow", "Stabilisation", nullptr));
        btn_Close->setText(QCoreApplication::translate("MainWindow", "Close", nullptr));
        buttonEmergencyStop->setText(QCoreApplication::translate("MainWindow", "Emergeny STOP", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuHV_System->setTitle(QCoreApplication::translate("MainWindow", "HV System", nullptr));
        menuON_OFF->setTitle(QCoreApplication::translate("MainWindow", "ON/OFF", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
