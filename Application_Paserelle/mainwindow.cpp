#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "modbus.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) // <- ici l’ordre est important !
{
    networkManager = new QNetworkAccessManager(this); // Initialisation du gestionnaire réseau

    // Initialisation des chemins
    filePathPaserelle = "C:/Users/EtudiantIR2/Documents/Application_Paserelle/Json/Paserelle-M50.json";
    filePathModele = "C:/Users/EtudiantIR2/Documents/Application_Paserelle/Json/modele-acquisition-I-30.json";
    filePathMesure = "C:/Users/EtudiantIR2/Documents/Application_Paserelle/Json/modele-mesure-U-10.json";

    ui->setupUi(this); // toujours après le new Ui::MainWindow
    setupUi();
    connectToDatabase();

    ui->gatewayNameEdit->setVisible(false);
    ui->ipAddressEdit->setVisible(false);
    ui->gatewayNameLabel->setVisible(false);
    ui->ipAddressLabel->setVisible(false);
    ui->AppliquerModification->setVisible(false);
    ui->ipAddressPaserelleLabel->setVisible(false);
    ui->ipAddressDNSLabel->setVisible(false);
    ui->ipAddressMaskLabel->setVisible(false);
    ui->DHCPLabel->setVisible(false);
    ui->comboBoxDHCP->setVisible(false);
    ui->ipAddressPaserelleEdit->setVisible(false);
    ui->ipAddressMaskEdit->setVisible(false);
    ui->ipAddressDNSEdit->setVisible(false);
    ui->returnButton->setVisible(false);
    ui->NameModeleAcquisitionEdit->setVisible(false);
    ui->NameModeleAquisitionLabel->setVisible(false);
    ui->returnButton2->setVisible(false);
    ui->returnButton3->setVisible(false);
    ui->TypeCharge->setVisible(false);
    ui->comboBoxCharge->setVisible(false);
    ui->PeriodeIntegrationInst->setVisible(false);
    ui->PeriodeIntegrationInstEdit->setVisible(false);
    ui->PeriodeIntegrationMoy->setVisible(false);
    ui->PeriodeIntegrationMoyEdit->setVisible(false);
    ui->NameDomaineEdit->setVisible(false);
    ui->NameDomaine->setVisible(false);
}

void MainWindow::connectToDatabase() {
    db = QSqlDatabase::addDatabase("QODBC");
    QString connectionString = "DRIVER={MySQL ODBC 9.2 ANSI Driver};SERVER=192.168.17.2;DATABASE=Mesure_De;UID=admin;PWD=admin;";
    db.setDatabaseName(connectionString);

    if (db.open()) {
        QMessageBox::information(this, "Connexion réussie", "Connexion à la base de données MySQL établie.");
    } else {
        QMessageBox::critical(this, "Erreur de base de données", "Connexion échouée : " + db.lastError().text());
    }
}

void MainWindow::setupUi() {
    connect(ui->AppliquerModification, &QPushButton::clicked, this, &MainWindow::AppliquerModification);
    connect(ui->AppliquerModification, &QPushButton::clicked, this, &MainWindow::ModifieModbus);
    connect(ui->modifyGatewayButton, &QPushButton::clicked, this, &MainWindow::modifyGatewayButtonClicked);
    connect(ui->acquisitionModelButton, &QPushButton::clicked, this, &MainWindow::acquisitionModelButtonClicked);
    connect(ui->measureModelButton, &QPushButton::clicked, this, &MainWindow::measureModelButtonClicked);
    connect(ui->returnButton, &QPushButton::clicked, this, &MainWindow::returnButtonClicked);
    connect(ui->returnButton2, &QPushButton::clicked, this, &MainWindow::returnButtonClicked2);
    connect(ui->returnButton3, &QPushButton::clicked, this, &MainWindow::returnButtonClicked3);
    ui->comboBoxDHCP->addItem("", -1);
    ui->comboBoxDHCP->addItem("Activé", 1);
    ui->comboBoxDHCP->addItem("Desactivé", 0);
    ui->comboBoxCharge->addItem("", -1);
    ui->comboBoxCharge->addItem("monophasé 1P+N-1TC (0)", 0);
    ui->comboBoxCharge->addItem("triphasé 3P+N-3TC (1)", 1);
    ui->PrimaryKey->addItem("", -1);
    ui->PrimaryKey->addItem("Modbus (0)", 0);
    ui->PrimaryKey->addItem("MQTT (1)", 1);
    ui->PrimaryKey->addItem("ALL (2)", 2);

}

static QString newName;
static QString DomaineName;
static QString newNameModeleAcquisition;
static QString PeriodeIntInst;
static QString newIp;
static QString ipGateway;
static QString ipMask;
static QString ipDNS;
static QString PeriodeIntMoy;
static int phase;
static int DHCP;
int unitID = 255;
int unitID2 = 5;
int unitID3 = 6;

void MainWindow::on_OFFButton_clicked()
{
    ModbusCommunicator modbus("192.168.22.1", 502);
    int startRegister2 = 57856;
    uint16_t value2 = 178;
    bool successOFF = modbus.writeSingleRegister(unitID, startRegister2, value2);
    if (!successOFF) {
        qDebug() << "Échec de la mise en arret";
    } else {
        qDebug() << "La passerelle redemarre ";
    }
}


void MainWindow::ModifieModbus() {
    ModbusCommunicator modbus("192.168.22.1", 502);

    if (!modbus.isConnected()) {
        qDebug() << "Pas connecté au serveur Modbus";
        return;
    }

    qDebug() << "Nom à envoyer au Modbus :" << newName;
    qDebug() << "Domaine à envoyer au Modbus :" << DomaineName;
    qDebug() << "La valeur de la phase est :" << phase;
    qDebug() << "Le nom du modele d'acquisition est :" << newNameModeleAcquisition;
    qDebug() << "La valeur du DHCP est :" << DHCP;
    qDebug() << "L'adresse IP est :" << newIp;
    qDebug() << "L'adresse du Gateway est :" << ipGateway;
    qDebug() << "L'adresse du Mask est :" << ipMask;
    qDebug() << "L'adresse du DNS est :" << ipDNS;
    qDebug() << "La valeur de la periode moyenne est :" << PeriodeIntMoy;
    qDebug() << "La valeur instantaner est :" << PeriodeIntInst;






    // 1) Écriture du nouveau nom dans les registres à partir de 31232
    // Conversion du QString newName en tableau de valeurs 16 bits pour Modbus
    if (newName.length() % 2 != 0) {
        newName.append('\0');
    }

    QByteArray byteArrayName = newName.toUtf8();
    int lengthName = 24;

    byteArrayName.resize(lengthName);

    std::vector<uint16_t> modbusValuesName(lengthName / 2);

    for (int i = 0; i < lengthName / 2; ++i) {
        modbusValuesName[i] = (static_cast<uint8_t>(byteArrayName[2*i]) << 8) | static_cast<uint8_t>(byteArrayName[2*i + 1]);
    }

    int startRegisterName = 31232;
    modbus.writeMultipleRegisters(unitID, startRegisterName, modbusValuesName.data(), modbusValuesName.size());



    // 2)Ecriture du nouveau domaine dans les registre a partir de 29194
    if (DomaineName.length() % 2 != 0) {
        DomaineName.append('\0');
    }

    QByteArray byteArrayDomaine = DomaineName.toUtf8();
    int lengthDomaine = 18;

    byteArrayDomaine.resize(lengthDomaine);

    std::vector<uint16_t> domaineValues(lengthDomaine / 2);

    for (int i = 0; i < lengthDomaine / 2; ++i) {
        domaineValues[i] = (static_cast<uint8_t>(byteArrayDomaine[2*i]) << 8) | static_cast<uint8_t>(byteArrayDomaine[2*i + 1]);
    }

    int startRegister2 = 29194;
    modbus.writeMultipleRegisters(unitID, startRegister2, domaineValues.data(), domaineValues.size());



    //3)Ecriture du DHCP
    int dhcpIndex = DHCP; // DHCP est la variable contenant l'index du ComboBox

    if (dhcpIndex == 0 || dhcpIndex == 1) {
        int startRegister = 29190;
        uint16_t value = static_cast<uint16_t>(dhcpIndex); // 0 ou 1
        bool success = modbus.writeSingleRegister(unitID, startRegister, value);

        if (!success) {
            qDebug() << "Échec de l'écriture du registre DHCP avec la valeur :" << value;
        } else {
            qDebug() << "DHCP écrit avec succès : " << value;
        }
    } else if (dhcpIndex == -1) {
        qDebug() << "DHCP non sélectionné, aucun envoi Modbus effectué.";
    }

    //4)ecriture de l'adresse Ip
    if (newIp.length() % 2 != 0) {
        newIp.append('\0');
    }

    QByteArray byteArrayIP = newIp.toUtf8();
    int lengthIp = 2;

    byteArrayIP.resize(lengthIp);

    std::vector<uint16_t> modbusValuesIp(lengthIp / 2);

    for (int i = 0; i < lengthIp / 2; ++i) {
        modbusValuesIp[i] = (static_cast<uint8_t>(byteArrayIP[2*i]) << 8) | static_cast<uint8_t>(byteArrayIP[2*i + 1]);
    }

    long startRegisterIp = 29184;
    modbus.writeMultipleRegisters(unitID, startRegisterIp, modbusValuesIp.data(), modbusValuesIp.size());

    //5)ecriture de l'adresse de la paserelle
    if (ipGateway.length() % 2 != 0) {
        ipGateway.append('\0');
    }

    QByteArray byteArrayPasserelle = ipGateway.toUtf8();
    int lengthpasserelle = 4;

    byteArrayPasserelle.resize(lengthpasserelle);

    std::vector<uint16_t> modbusValuesPasserelle(lengthpasserelle / 2);

    for (int i = 0; i < lengthpasserelle / 2; ++i) {
        modbusValuesPasserelle[i] = (static_cast<uint8_t>(byteArrayPasserelle[2*i]) << 8) | static_cast<uint8_t>(byteArrayPasserelle[2*i + 1]);
    }

    long startRegisterPasserelle = 29188 ;
    modbus.writeMultipleRegisters(unitID, startRegisterPasserelle, modbusValuesPasserelle.data(), modbusValuesPasserelle.size());


    //6)ecriture de l'adresse du DNS
    if (ipDNS.length() % 2 != 0) {
        ipDNS.append('\0');
    }

    QByteArray byteArrayDNS= ipDNS.toUtf8();
    int lengthDNS = 4;

    byteArrayDNS.resize(lengthDNS);

    std::vector<uint16_t> modbusValuesDNS(lengthDNS / 2);

    for (int i = 0; i < lengthDNS / 2; ++i) {
        modbusValuesDNS[i] = (static_cast<uint8_t>(byteArrayDNS[2*i]) << 8) | static_cast<uint8_t>(byteArrayDNS[2*i + 1]);
    }

    long startRegisterDNS = 29192 ;
    modbus.writeMultipleRegisters(unitID, startRegisterDNS, modbusValuesDNS.data(), modbusValuesDNS.size());


    //7)ecriture du Mask
    if (ipMask.length() % 2 != 0) {
        ipMask.append('\0');
    }

    QByteArray byteArrayMask= ipMask.toUtf8();
    int lengthMask = 4;

    byteArrayMask.resize(lengthMask);

    std::vector<uint16_t> modbusValuesMask(lengthMask / 2);

    for (int i = 0; i < lengthMask / 2; ++i) {
        modbusValuesMask[i] = (static_cast<uint8_t>(byteArrayMask[2*i]) << 8) | static_cast<uint8_t>(byteArrayMask[2*i + 1]);
    }

    long startRegisterMask = 29186 ;
    modbus.writeMultipleRegisters(unitID, startRegisterMask, modbusValuesMask.data(), modbusValuesMask.size());



    //8)Ecriture de la phase
    int phaseIndex = phase; // 'phase' contient l'index du ComboBox

    if (phaseIndex == 0 || phaseIndex == 1) {
        int startRegister = 13825;
        uint16_t value = (phaseIndex == 0) ? 1 : 7;
        bool success = modbus.writeSingleRegister(unitID2, startRegister, value);

        if (!success) {
            qDebug() << "Échec de l'écriture du registre Phase avec la valeur :" << value;
        } else {
            qDebug() << "Phase écrite avec succès : " << value;
        }
    } else if (phaseIndex == -1) {
        qDebug() << "Phase non sélectionnée, aucun envoi Modbus effectué.";
    }

    //9)ecriture du modele d'acquisition
    if (newNameModeleAcquisition.length() % 2 != 0) {
        newNameModeleAcquisition.append('\0');
    }

    QByteArray byteArrayAcquisition = newNameModeleAcquisition.toUtf8();
    int lengthAcquisition = 15;

    byteArrayAcquisition.resize(lengthAcquisition);

    std::vector<uint16_t> modbusValuesAcquisition(lengthAcquisition / 2);

    for (int i = 0; i < lengthAcquisition / 2; ++i) {
        modbusValuesAcquisition[i] = (static_cast<uint8_t>(byteArrayAcquisition[2*i]) << 8) | static_cast<uint8_t>(byteArrayAcquisition[2*i + 1]);
    }

    long startRegisterAcquisition = 46592;
    modbus.writeMultipleRegisters(unitID2, startRegisterAcquisition, modbusValuesAcquisition.data(), modbusValuesAcquisition.size());

    //10)ecriture du valeur instantanée
    if (PeriodeIntInst.length() % 2 != 0) {
        PeriodeIntInst.append('\0');
    }

    QByteArray byteArrayIntInst = PeriodeIntInst.toUtf8();
    int lengthIntInst = 2;

    byteArrayIntInst.resize(lengthIntInst);

    std::vector<uint16_t> modbusValuesIntInst(lengthIntInst / 2);

    for (int i = 0; i < lengthIntInst / 2; ++i) {
        modbusValuesIntInst[i] = (static_cast<uint8_t>(byteArrayIntInst[2*i]) << 8) | static_cast<uint8_t>(byteArrayIntInst[2*i + 1]);
    }

    long startRegisterIntInst = 34816;
    modbus.writeMultipleRegisters(unitID3, startRegisterIntInst, modbusValuesIntInst.data(), modbusValuesIntInst.size());


    //11)ecriture du valeur moyenne
    if (PeriodeIntMoy.length() % 2 != 0) {
        PeriodeIntMoy.append('\0');
    }

    QByteArray byteArrayMoyenne = PeriodeIntInst.toUtf8();
    int lengthMoyenne = 2;

    byteArrayMoyenne.resize(lengthMoyenne);

    std::vector<uint16_t> modbusValuesMoyenne(lengthMoyenne / 2);

    for (int i = 0; i < lengthMoyenne / 2; ++i) {
        modbusValuesMoyenne[i] = (static_cast<uint8_t>(byteArrayMoyenne[2*i]) << 8) | static_cast<uint8_t>(byteArrayMoyenne[2*i + 1]);
    }
    long startRegisterMoyenne = 34817;
    modbus.writeMultipleRegisters(unitID3, startRegisterMoyenne, modbusValuesMoyenne.data(), modbusValuesMoyenne.size());

    // 12) Écriture de la valeur entière 161 dans le registre 58112
    /*
    int startRegister2 = 57856;
    uint16_t value2 = 161;
    bool success2 = modbus.writeSingleRegister(unitId, startRegister2, value2);

    if (success && success2 && success3 && success4) {
        qDebug() << "Écriture multiple réussie";
    } else {
        qDebug() << "Échec de l'écriture Modbus";
    }
*/

}

void MainWindow::AppliquerModification() {

    newName = ui->gatewayNameEdit->text();
    DomaineName = ui->NameDomaineEdit->text();
    newIp = ui->ipAddressEdit->text();
    ipGateway = ui->ipAddressPaserelleEdit->text();
    ipMask = ui->ipAddressMaskEdit->text();
    ipDNS = ui->ipAddressDNSEdit->text();
    newNameModeleAcquisition = ui->NameModeleAcquisitionEdit->text();
    PeriodeIntInst = ui->PeriodeIntegrationInstEdit->text();
    PeriodeIntMoy = ui->PeriodeIntegrationMoyEdit->text();



    phase= ui->comboBoxCharge->currentData().toInt();
    DHCP = ui->comboBoxDHCP->currentData().toInt();
    int idDispositif= ui->PrimaryKey->currentData().toInt();

    ModifieModbus();

    bool dbSuccess = false;
    if (db.open()) {
        QSqlQuery query(db);
        query.prepare("INSERT INTO Dispositif_Passerelle (ID_Dispositif_PK,"
                      "Nom_dispositif,"
                      "Adresse_IP_Passerelle,"
                      "Adresse_gateway_Passerelle,"
                      "Adresse_mask_gateway_Passerelle,"
                      "Adresse_DNS_Passerelle,"
                      "Adresse_DHCP_Passerelle,"
                      "Domaine_Passerelle,"
                      "Nom_model_acquisition,"
                      "Type_charge,"
                      "Periode_inst,"
                      "Periode_moyenne,"
                      "VALUES (?,?,?,?,?,?,?,?,?,?,?,?) "
                      "ON DUPLICATE KEY UPDATE "
                      "Nom_dispositif = IFNULL(?, Nom_dispositif),"
                      "Adresse_IP_Passerelle = IFNULL(?, Adresse_IP_Passerelle),"
                      "Adresse_gateway_Passerelle = IFNULL(?, Adresse_gateway_Passerelle),"
                      "Adresse_mask_gateway_Passerelle = IFNULL(?, Adresse_mask_gateway_Passerelle),"
                      "Adresse_DNS_Passerelle = IFNULL(?, Adresse_DNS_Passerelle),"
                      "Adresse_DHCP_Passerelle = IFNULL(?, Adresse_DHCP_Passerelle),"
                      "Domaine_Passerelle = IFNULL(?, Domaine_Passerelle),"
                      "Nom_model_acquisition = IFNULL(?, Nom_model_acquisition),"
                      "Type_charge = IFNULL(?, Type_charge),"
                      "Periode_inst = IFNULL(?, Periode_inst),"
                      "Periode_moyenne = IFNULL(?, Periode_moyenne),");

        // Première série de addBindValue() : Pour l'instruction INSERT et les valeurs à insérer si la ligne n'existe pas.
        query.addBindValue(idDispositif);
        query.addBindValue(newName.isEmpty() ? QVariant() : newName);
        query.addBindValue(newIp.isEmpty() ? QVariant() : newIp);
        query.addBindValue(ipGateway.isEmpty() ? QVariant() : ipGateway);
        query.addBindValue(ipMask.isEmpty() ? QVariant() : ipMask);
        query.addBindValue(ipDNS.isEmpty() ? QVariant() : ipDNS);
        query.addBindValue(DHCP == -1 ? QVariant() : DHCP);
        query.addBindValue(DomaineName.isEmpty() ? QVariant() : DomaineName);
        query.addBindValue(newNameModeleAcquisition.isEmpty() ? QVariant() : newNameModeleAcquisition);
        query.addBindValue(phase == -1 ? QVariant() : phase);
        query.addBindValue(PeriodeIntInst.isEmpty() ? QVariant() : PeriodeIntInst);
        query.addBindValue(PeriodeIntMoy.isEmpty() ? QVariant() : PeriodeIntMoy);

        // La première série de query.addBindValue() fournit les valeurs qui seront utilisées pour créer une nouvelle ligne
        // dans la table Dispositif_Passerelle si aucune ligne avec la même clé primaire (ID_Dispositif_PK) n'existe déjà.
        // Ces valeurs sont passées directement dans la clause VALUES (?,?,?,...) de l'instruction INSERT.*
        // Il faut au moin une valeur dans un autre attribut pour que la nouvelle table soit crée

        query.addBindValue(newName.isEmpty() ? QVariant() : newName);
        query.addBindValue(newIp.isEmpty() ? QVariant() : newIp);
        query.addBindValue(ipGateway.isEmpty() ? QVariant() : ipGateway);
        query.addBindValue(ipMask.isEmpty() ? QVariant() : ipMask);
        query.addBindValue(ipDNS.isEmpty() ? QVariant() : ipDNS);
        query.addBindValue(DHCP == -1 ? QVariant() : DHCP);;
        query.addBindValue(DomaineName.isEmpty() ? QVariant() : DomaineName);
        query.addBindValue(newNameModeleAcquisition.isEmpty() ? QVariant() : newNameModeleAcquisition);
        query.addBindValue(phase == -1 ? QVariant() : phase);
        query.addBindValue(PeriodeIntInst.isEmpty() ? QVariant() : PeriodeIntInst);
        query.addBindValue(PeriodeIntMoy.isEmpty() ? QVariant() : PeriodeIntMoy);

        // La seconde série de query.addBindValue() fournit les valeurs qui seront utilisées pour mettre à jour la ligne existante
        // dans la table Dispositif_Passerelle si une ligne avec la même clé primaire (ID_Dispositif_PK) existe déjà.
        // Ces valeurs sont associées aux ? dans la clause ON DUPLICATE KEY UPDATE Nom_dispositif = IFNULL(?, Nom_dispositif), ....
        // La logique IFNULL(?, Nom_dispositif) est cruciale ici. Pour chaque champ que nous voulons potentiellement mettre à jour,
        // nous fournissons une valeur. Si cette valeur est NULL (ce qui se produit lorsque le QLineEdit est vide ou le QComboBox est à -1,
        // grâce à l'utilisation de l'opérateur ternaire ... ? QVariant() : ...), alors la fonction SQL IFNULL fera en sorte que la
        // valeur existante dans la colonne (Nom_dispositif, Adresse_IP_Passerelle, etc.) soit conservée. Si la valeur n'est pas NULL
        // (l'utilisateur a entré ou sélectionné quelque chose), alors cette nouvelle valeur sera utilisée pour la mise à jour.


        if (query.exec()) {
            dbSuccess = true;
        }
        db.close();
    }

    if (dbSuccess) {
        QMessageBox::information(this, "Succès", "Configuration appliquée avec succès.");
    }
}

bool MainWindow::save() {
    int idDispositif= ui->PrimaryKey->currentData().toInt();
    if (idDispositif == -1) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un ID de Dispositif.");
        return false;
    }
}

void MainWindow::modifyGatewayButtonClicked() {
    ui->gatewayNameEdit->setVisible(true);
    ui->ipAddressEdit->setVisible(true);
    ui->gatewayNameLabel->setVisible(true);
    ui->ipAddressLabel->setVisible(true);
    ui->AppliquerModification->setVisible(true);
    ui->returnButton->setVisible(true);
    ui->modifyGatewayButton->setVisible(false);
    ui->acquisitionModelButton->setVisible(false);
    ui->measureModelButton->setVisible(false);
    ui->ipAddressPaserelleLabel->setVisible(true);
    ui->ipAddressDNSLabel->setVisible(true);
    ui->ipAddressMaskLabel->setVisible(true);
    ui->DHCPLabel->setVisible(true);
    ui->comboBoxDHCP->setVisible(true);
    ui->ipAddressPaserelleEdit->setVisible(true);
    ui->ipAddressMaskEdit->setVisible(true);
    ui->ipAddressDNSEdit->setVisible(true);
    ui->NameDomaineEdit->setVisible(true);
    ui->NameDomaine->setVisible(true);
}

void MainWindow::returnButtonClicked() {
    ui->gatewayNameEdit->setVisible(false);
    ui->ipAddressEdit->setVisible(false);
    ui->gatewayNameLabel->setVisible(false);
    ui->ipAddressLabel->setVisible(false);
    ui->AppliquerModification->setVisible(false);
    ui->returnButton->setVisible(false);
    ui->modifyGatewayButton->setVisible(true);
    ui->acquisitionModelButton->setVisible(true);
    ui->measureModelButton->setVisible(true);
    ui->ipAddressPaserelleLabel->setVisible(false);
    ui->ipAddressDNSLabel->setVisible(false);
    ui->ipAddressMaskLabel->setVisible(false);
    ui->DHCPLabel->setVisible(false);
    ui->comboBoxDHCP->setVisible(false);
    ui->ipAddressPaserelleEdit->setVisible(false);
    ui->ipAddressMaskEdit->setVisible(false);
    ui->ipAddressDNSEdit->setVisible(false);
    ui->NameDomaineEdit->setVisible(false);
    ui->NameDomaine->setVisible(false);
}

void MainWindow::acquisitionModelButtonClicked() {
    ui->NameModeleAcquisitionEdit->setVisible(true);
    ui->NameModeleAquisitionLabel->setVisible(true);
    ui->modifyGatewayButton->setVisible(false);
    ui->acquisitionModelButton->setVisible(false);
    ui->measureModelButton->setVisible(false);
    ui->returnButton2->setVisible(true);
    ui->AppliquerModification->setVisible(true);
    ui->TypeCharge->setVisible(true);
    ui->comboBoxCharge->setVisible(true);
}

void MainWindow::returnButtonClicked2()
{
    ui->modifyGatewayButton->setVisible(true);
    ui->acquisitionModelButton->setVisible(true);
    ui->measureModelButton->setVisible(true);
    ui->NameModeleAcquisitionEdit->setVisible(false);
    ui->NameModeleAquisitionLabel->setVisible(false);
    ui->returnButton2->setVisible(false);
    ui->AppliquerModification->setVisible(false);
    ui->TypeCharge->setVisible(false);
    ui->comboBoxCharge->setVisible(false);
}

void MainWindow::measureModelButtonClicked() {;
    ui->returnButton3->setVisible(true);
    ui->modifyGatewayButton->setVisible(false);
    ui->acquisitionModelButton->setVisible(false);
    ui->measureModelButton->setVisible(false);
    ui->AppliquerModification->setVisible(true);
    ui->PeriodeIntegrationInst->setVisible(true);
    ui->PeriodeIntegrationInstEdit->setVisible(true);
    ui->PeriodeIntegrationMoy->setVisible(true);
    ui->PeriodeIntegrationMoyEdit->setVisible(true);
}
void MainWindow::returnButtonClicked3()
{
    ui->modifyGatewayButton->setVisible(true);
    ui->acquisitionModelButton->setVisible(true);
    ui->measureModelButton->setVisible(true);
    ui->returnButton3->setVisible(false);
    ui->AppliquerModification->setVisible(false);
    ui->PeriodeIntegrationInst->setVisible(false);
    ui->PeriodeIntegrationInstEdit->setVisible(false);
    ui->PeriodeIntegrationMoy->setVisible(false);
    ui->PeriodeIntegrationMoyEdit->setVisible(false);
}


