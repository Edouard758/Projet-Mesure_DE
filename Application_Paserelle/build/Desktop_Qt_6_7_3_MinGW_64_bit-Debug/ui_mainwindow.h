/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *gatewayNameLabel;
    QLineEdit *gatewayNameEdit;
    QPushButton *AppliquerModification;
    QLabel *ipAddressLabel;
    QLineEdit *ipAddressEdit;
    QPushButton *modifyGatewayButton;
    QPushButton *acquisitionModelButton;
    QPushButton *measureModelButton;
    QPushButton *returnButton;
    QLabel *ipAddressPaserelleLabel;
    QLabel *ipAddressMaskLabel;
    QLabel *ipAddressDNSLabel;
    QLabel *TimeZoneLabel;
    QLabel *DHCPLabel;
    QLabel *EtatModbusLabel;
    QLineEdit *ipAddressPaserelleEdit;
    QLineEdit *ipAddressMaskEdit;
    QLineEdit *ipAddressDNSEdit;
    QComboBox *comboBoxDHCP;
    QComboBox *comboBoxModbus;
    QComboBox *comboBoxTimeZone;
    QLabel *NameModeleAquisitionLabel;
    QLineEdit *NameModeleAcquisitionEdit;
    QPushButton *returnButton2;
    QPushButton *returnButton3;
    QLabel *PeriodeIntegrationInst;
    QLineEdit *PeriodeIntegrationInstEdit;
    QLabel *status1;
    QLabel *status2;
    QLabel *status3;
    QComboBox *comboBoxStatus1;
    QComboBox *comboBoxStatus2;
    QComboBox *comboBoxStatus3;
    QLabel *TypeCharge;
    QComboBox *comboBoxCharge;
    QLabel *PeriodeIntegrationMoy;
    QLabel *PeriodeIntegrationCourbe;
    QLineEdit *PeriodeIntegrationMoyEdit;
    QLineEdit *PeriodeIntegrationCourbeEdit;
    QLabel *NameDomaine;
    QLineEdit *NameDomaineEdit;
    QLabel *PrimaryKeylabel;
    QComboBox *PrimaryKey;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(632, 800);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gatewayNameLabel = new QLabel(centralwidget);
        gatewayNameLabel->setObjectName("gatewayNameLabel");
        gatewayNameLabel->setGeometry(QRect(20, 110, 221, 51));
        gatewayNameEdit = new QLineEdit(centralwidget);
        gatewayNameEdit->setObjectName("gatewayNameEdit");
        gatewayNameEdit->setGeometry(QRect(250, 120, 151, 31));
        AppliquerModification = new QPushButton(centralwidget);
        AppliquerModification->setObjectName("AppliquerModification");
        AppliquerModification->setGeometry(QRect(230, 650, 171, 41));
        ipAddressLabel = new QLabel(centralwidget);
        ipAddressLabel->setObjectName("ipAddressLabel");
        ipAddressLabel->setGeometry(QRect(20, 160, 191, 51));
        ipAddressEdit = new QLineEdit(centralwidget);
        ipAddressEdit->setObjectName("ipAddressEdit");
        ipAddressEdit->setGeometry(QRect(250, 170, 151, 31));
        modifyGatewayButton = new QPushButton(centralwidget);
        modifyGatewayButton->setObjectName("modifyGatewayButton");
        modifyGatewayButton->setGeometry(QRect(30, 110, 131, 61));
        acquisitionModelButton = new QPushButton(centralwidget);
        acquisitionModelButton->setObjectName("acquisitionModelButton");
        acquisitionModelButton->setGeometry(QRect(200, 110, 201, 61));
        measureModelButton = new QPushButton(centralwidget);
        measureModelButton->setObjectName("measureModelButton");
        measureModelButton->setGeometry(QRect(420, 110, 201, 61));
        returnButton = new QPushButton(centralwidget);
        returnButton->setObjectName("returnButton");
        returnButton->setGeometry(QRect(500, 40, 91, 51));
        ipAddressPaserelleLabel = new QLabel(centralwidget);
        ipAddressPaserelleLabel->setObjectName("ipAddressPaserelleLabel");
        ipAddressPaserelleLabel->setGeometry(QRect(20, 220, 201, 51));
        ipAddressMaskLabel = new QLabel(centralwidget);
        ipAddressMaskLabel->setObjectName("ipAddressMaskLabel");
        ipAddressMaskLabel->setGeometry(QRect(20, 290, 191, 51));
        ipAddressDNSLabel = new QLabel(centralwidget);
        ipAddressDNSLabel->setObjectName("ipAddressDNSLabel");
        ipAddressDNSLabel->setGeometry(QRect(20, 360, 201, 51));
        TimeZoneLabel = new QLabel(centralwidget);
        TimeZoneLabel->setObjectName("TimeZoneLabel");
        TimeZoneLabel->setGeometry(QRect(20, 430, 191, 51));
        DHCPLabel = new QLabel(centralwidget);
        DHCPLabel->setObjectName("DHCPLabel");
        DHCPLabel->setGeometry(QRect(20, 490, 191, 51));
        EtatModbusLabel = new QLabel(centralwidget);
        EtatModbusLabel->setObjectName("EtatModbusLabel");
        EtatModbusLabel->setGeometry(QRect(20, 540, 191, 51));
        ipAddressPaserelleEdit = new QLineEdit(centralwidget);
        ipAddressPaserelleEdit->setObjectName("ipAddressPaserelleEdit");
        ipAddressPaserelleEdit->setGeometry(QRect(250, 230, 151, 31));
        ipAddressMaskEdit = new QLineEdit(centralwidget);
        ipAddressMaskEdit->setObjectName("ipAddressMaskEdit");
        ipAddressMaskEdit->setGeometry(QRect(250, 300, 151, 31));
        ipAddressDNSEdit = new QLineEdit(centralwidget);
        ipAddressDNSEdit->setObjectName("ipAddressDNSEdit");
        ipAddressDNSEdit->setGeometry(QRect(250, 370, 151, 31));
        comboBoxDHCP = new QComboBox(centralwidget);
        comboBoxDHCP->setObjectName("comboBoxDHCP");
        comboBoxDHCP->setGeometry(QRect(250, 500, 151, 31));
        comboBoxModbus = new QComboBox(centralwidget);
        comboBoxModbus->setObjectName("comboBoxModbus");
        comboBoxModbus->setGeometry(QRect(250, 550, 151, 31));
        comboBoxTimeZone = new QComboBox(centralwidget);
        comboBoxTimeZone->setObjectName("comboBoxTimeZone");
        comboBoxTimeZone->setGeometry(QRect(250, 440, 181, 31));
        NameModeleAquisitionLabel = new QLabel(centralwidget);
        NameModeleAquisitionLabel->setObjectName("NameModeleAquisitionLabel");
        NameModeleAquisitionLabel->setGeometry(QRect(20, 60, 221, 51));
        NameModeleAcquisitionEdit = new QLineEdit(centralwidget);
        NameModeleAcquisitionEdit->setObjectName("NameModeleAcquisitionEdit");
        NameModeleAcquisitionEdit->setGeometry(QRect(250, 70, 151, 31));
        returnButton2 = new QPushButton(centralwidget);
        returnButton2->setObjectName("returnButton2");
        returnButton2->setGeometry(QRect(500, 40, 91, 51));
        returnButton3 = new QPushButton(centralwidget);
        returnButton3->setObjectName("returnButton3");
        returnButton3->setGeometry(QRect(500, 40, 91, 51));
        PeriodeIntegrationInst = new QLabel(centralwidget);
        PeriodeIntegrationInst->setObjectName("PeriodeIntegrationInst");
        PeriodeIntegrationInst->setGeometry(QRect(20, 110, 271, 51));
        PeriodeIntegrationInstEdit = new QLineEdit(centralwidget);
        PeriodeIntegrationInstEdit->setObjectName("PeriodeIntegrationInstEdit");
        PeriodeIntegrationInstEdit->setGeometry(QRect(450, 120, 161, 31));
        status1 = new QLabel(centralwidget);
        status1->setObjectName("status1");
        status1->setGeometry(QRect(20, 160, 191, 51));
        status2 = new QLabel(centralwidget);
        status2->setObjectName("status2");
        status2->setGeometry(QRect(20, 220, 191, 51));
        status3 = new QLabel(centralwidget);
        status3->setObjectName("status3");
        status3->setGeometry(QRect(20, 290, 191, 51));
        comboBoxStatus1 = new QComboBox(centralwidget);
        comboBoxStatus1->setObjectName("comboBoxStatus1");
        comboBoxStatus1->setGeometry(QRect(250, 170, 151, 31));
        comboBoxStatus2 = new QComboBox(centralwidget);
        comboBoxStatus2->setObjectName("comboBoxStatus2");
        comboBoxStatus2->setGeometry(QRect(250, 230, 151, 31));
        comboBoxStatus3 = new QComboBox(centralwidget);
        comboBoxStatus3->setObjectName("comboBoxStatus3");
        comboBoxStatus3->setGeometry(QRect(250, 300, 151, 31));
        TypeCharge = new QLabel(centralwidget);
        TypeCharge->setObjectName("TypeCharge");
        TypeCharge->setGeometry(QRect(20, 110, 191, 51));
        comboBoxCharge = new QComboBox(centralwidget);
        comboBoxCharge->setObjectName("comboBoxCharge");
        comboBoxCharge->setGeometry(QRect(250, 120, 271, 31));
        PeriodeIntegrationMoy = new QLabel(centralwidget);
        PeriodeIntegrationMoy->setObjectName("PeriodeIntegrationMoy");
        PeriodeIntegrationMoy->setGeometry(QRect(20, 160, 271, 51));
        PeriodeIntegrationCourbe = new QLabel(centralwidget);
        PeriodeIntegrationCourbe->setObjectName("PeriodeIntegrationCourbe");
        PeriodeIntegrationCourbe->setGeometry(QRect(20, 220, 411, 51));
        PeriodeIntegrationMoyEdit = new QLineEdit(centralwidget);
        PeriodeIntegrationMoyEdit->setObjectName("PeriodeIntegrationMoyEdit");
        PeriodeIntegrationMoyEdit->setGeometry(QRect(450, 170, 161, 31));
        PeriodeIntegrationCourbeEdit = new QLineEdit(centralwidget);
        PeriodeIntegrationCourbeEdit->setObjectName("PeriodeIntegrationCourbeEdit");
        PeriodeIntegrationCourbeEdit->setGeometry(QRect(450, 230, 161, 31));
        NameDomaine = new QLabel(centralwidget);
        NameDomaine->setObjectName("NameDomaine");
        NameDomaine->setGeometry(QRect(20, 600, 101, 41));
        NameDomaineEdit = new QLineEdit(centralwidget);
        NameDomaineEdit->setObjectName("NameDomaineEdit");
        NameDomaineEdit->setGeometry(QRect(250, 610, 151, 31));
        PrimaryKeylabel = new QLabel(centralwidget);
        PrimaryKeylabel->setObjectName("PrimaryKeylabel");
        PrimaryKeylabel->setGeometry(QRect(30, 20, 161, 41));
        PrimaryKey = new QComboBox(centralwidget);
        PrimaryKey->setObjectName("PrimaryKey");
        PrimaryKey->setGeometry(QRect(250, 20, 151, 31));
        MainWindow->setCentralWidget(centralwidget);
        measureModelButton->raise();
        acquisitionModelButton->raise();
        modifyGatewayButton->raise();
        gatewayNameLabel->raise();
        gatewayNameEdit->raise();
        AppliquerModification->raise();
        ipAddressLabel->raise();
        ipAddressEdit->raise();
        returnButton->raise();
        ipAddressPaserelleLabel->raise();
        ipAddressMaskLabel->raise();
        ipAddressDNSLabel->raise();
        TimeZoneLabel->raise();
        DHCPLabel->raise();
        EtatModbusLabel->raise();
        ipAddressPaserelleEdit->raise();
        ipAddressMaskEdit->raise();
        ipAddressDNSEdit->raise();
        comboBoxDHCP->raise();
        comboBoxModbus->raise();
        comboBoxTimeZone->raise();
        NameModeleAquisitionLabel->raise();
        NameModeleAcquisitionEdit->raise();
        returnButton2->raise();
        returnButton3->raise();
        PeriodeIntegrationInst->raise();
        PeriodeIntegrationInstEdit->raise();
        status1->raise();
        status2->raise();
        status3->raise();
        comboBoxStatus1->raise();
        comboBoxStatus2->raise();
        comboBoxStatus3->raise();
        TypeCharge->raise();
        comboBoxCharge->raise();
        PeriodeIntegrationMoy->raise();
        PeriodeIntegrationCourbe->raise();
        PeriodeIntegrationMoyEdit->raise();
        PeriodeIntegrationCourbeEdit->raise();
        NameDomaine->raise();
        NameDomaineEdit->raise();
        PrimaryKeylabel->raise();
        PrimaryKey->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 632, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        gatewayNameLabel->setText(QCoreApplication::translate("MainWindow", "Nouveau Nom Paserelle :", nullptr));
        AppliquerModification->setText(QCoreApplication::translate("MainWindow", "Appliquer la configuration", nullptr));
        ipAddressLabel->setText(QCoreApplication::translate("MainWindow", "Nouvelle Adresse Ip :", nullptr));
        modifyGatewayButton->setText(QCoreApplication::translate("MainWindow", "Modifer Paserelle M50", nullptr));
        acquisitionModelButton->setText(QCoreApplication::translate("MainWindow", "Modifier modele d'aquisition I-30", nullptr));
        measureModelButton->setText(QCoreApplication::translate("MainWindow", "Modifer modele de mesure U-10", nullptr));
        returnButton->setText(QCoreApplication::translate("MainWindow", "Retour", nullptr));
        ipAddressPaserelleLabel->setText(QCoreApplication::translate("MainWindow", "Nouvelle Adresse Ip de la paserelle:", nullptr));
        ipAddressMaskLabel->setText(QCoreApplication::translate("MainWindow", "Nouveau Mask de sous r\303\251seaux :", nullptr));
        ipAddressDNSLabel->setText(QCoreApplication::translate("MainWindow", "Nouvelle Adresse de serveur DNS :", nullptr));
        TimeZoneLabel->setText(QCoreApplication::translate("MainWindow", "Time zone de la paserelle :", nullptr));
        DHCPLabel->setText(QCoreApplication::translate("MainWindow", "DHCP :", nullptr));
        EtatModbusLabel->setText(QCoreApplication::translate("MainWindow", "Modbus :", nullptr));
        NameModeleAquisitionLabel->setText(QCoreApplication::translate("MainWindow", "Nouveau nom modele acquisition  :", nullptr));
        returnButton2->setText(QCoreApplication::translate("MainWindow", "Retour", nullptr));
        returnButton3->setText(QCoreApplication::translate("MainWindow", "Retour", nullptr));
        PeriodeIntegrationInst->setText(QCoreApplication::translate("MainWindow", "Valeur de periode d'integration instantaner (*0.2s) :", nullptr));
        status1->setText(QCoreApplication::translate("MainWindow", "Status du Capteur N\302\2601 : ", nullptr));
        status2->setText(QCoreApplication::translate("MainWindow", "Status du Capteur N\302\2602 : ", nullptr));
        status3->setText(QCoreApplication::translate("MainWindow", "Status du Capteur N\302\2603 : ", nullptr));
        TypeCharge->setText(QCoreApplication::translate("MainWindow", "Type de charge :", nullptr));
        PeriodeIntegrationMoy->setText(QCoreApplication::translate("MainWindow", "Valeur de periode de mesure moyen (en minute) :", nullptr));
        PeriodeIntegrationCourbe->setText(QCoreApplication::translate("MainWindow", "Valeur de periode de mise a jour de courbe des charges (en minute) :", nullptr));
        NameDomaine->setText(QCoreApplication::translate("MainWindow", "Nom Domaine :", nullptr));
        PrimaryKeylabel->setText(QCoreApplication::translate("MainWindow", "Configuration Dispositif :", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
