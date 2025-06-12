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
#include <QCryptographicHash>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    networkManager = new QNetworkAccessManager(this);

    ui->setupUi(this);
    setupUi();
    connectToDatabase();

    ui->gatewayNameEdit->setVisible(false);
    ui->ipAddressEdit->setVisible(false);
    ui->gatewayNameLabel->setVisible(false);
    ui->ipAddressLabel->setVisible(false);
    ui->AppliquerModification_2->setVisible(false);
    ui->ipAddressPasserelleLabel->setVisible(false);
    ui->ipAddressPasserelleEdit->setVisible(false);
    ui->ipAddressDNSLabel->setVisible(false);
    ui->ipAddressMaskLabel->setVisible(false);
    ui->DHCPLabel->setVisible(false);
    ui->comboBoxDHCP->setVisible(false);
    ui->ipAddressMaskEdit->setVisible(false);
    ui->ipAddressMaskEdit->setVisible(false);
    ui->ipAddressDNSEdit->setVisible(false);
    ui->returnButton->setVisible(false);
    ui->NameModeleAcquisitionEdit->setVisible(false);
    ui->NameModeleAquisitionLabel->setVisible(false);
    ui->returnButton2->setVisible(false);
    ui->returnButton3->setVisible(false);
    ui->returnButton4->setVisible(false);
    ui->TypeCharge->setVisible(false);
    ui->comboBoxCharge->setVisible(false);
    ui->PeriodeIntegrationInst->setVisible(false);
    ui->PeriodeIntegrationInstEdit->setVisible(false);
    ui->PeriodeIntegrationMoy->setVisible(false);
    ui->PeriodeIntegrationMoyEdit->setVisible(false);
    ui->NameDomaineEdit->setVisible(false);
    ui->NameDomaine->setVisible(false);
    ui->idClientEdit->setVisible(false);
    ui->idClientLabel->setVisible(false);
    ui->MDPEdit->setVisible(false);
    ui->MDPLabel->setVisible(false);
    ui->showHideButton->setVisible(false);
    connect(ui->showHideButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    ui->showHideButton->setIcon(QIcon("C:/Users/EtudiantIR2/Documents/Appli_BDD/preview-show-interface-icon-free-vector.jpg"));
    ui->showHideButton->setIconSize(QSize(26, 26)); // Définir la taille de l'icône
}

void MainWindow::connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Bdd");
    db.setUserName("etudiant");
    db.setPassword("admincielir");
    QMessageBox::information(this, "Succes", QString("Connection à la base de donnée réussit"));

    if (!db.open()) {
        QSqlError error = db.lastError();
        QMessageBox::critical(this, "Erreur", QString("Impossible de se connecter à la base de données. Erreur: %1").arg(error.text()));
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
void writeIpAddress(ModbusCommunicator& modbus, const QString& ipString, int startRegister, int unitID) {
    if (ipString.trimmed().isEmpty())
        return;

    QHostAddress ipAddr(ipString);
    if (ipAddr.protocol() != QAbstractSocket::IPv4Protocol) {
        qDebug() << "Adresse IP invalide :" << ipString;
        return;
    }

    quint32 ip = ipAddr.toIPv4Address(); // Convertit en 32 bits (IPv4)

    std::vector<uint16_t> registers(2);
    // Encodage big-endian : haut 16 bits puis bas 16 bits
    registers[0] = static_cast<uint16_t>((ip >> 16) & 0xFFFF);
    registers[1] = static_cast<uint16_t>(ip & 0xFFFF);

    bool success = modbus.writeMultipleRegisters(unitID, startRegister, registers.data(), registers.size());
    if (!success) {
        qDebug() << "Échec de l'écriture de l'adresse IP" << ipString << "au registre" << startRegister;
    } else {
        qDebug() << "Adresse IP" << ipString << "écrite avec succès au registre" << startRegister;
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
    if (!newName.trimmed().isEmpty()) {
        if (newName.length() % 2 != 0) {
            newName.append('\0');
        }

        QByteArray byteArrayName = newName.toUtf8();
        int lengthName = 14;

        byteArrayName.resize(lengthName);

        std::vector<uint16_t> modbusValuesName(lengthName / 2);

        for (int i = 0; i < lengthName / 2; ++i) {
            modbusValuesName[i] = (static_cast<uint8_t>(byteArrayName[2*i]) << 8) | static_cast<uint8_t>(byteArrayName[2*i + 1]);
        }

        int startRegisterName = 31232;
        modbus.writeMultipleRegisters(unitID, startRegisterName, modbusValuesName.data(), modbusValuesName.size());
    }

    // 2) Ecriture du nouveau domaine dans les registre a partir de 29194
    if (!DomaineName.trimmed().isEmpty()) {
        if (DomaineName.length() % 2 != 0) {
            DomaineName.append('\0');
        }

        QByteArray byteArrayDomaine = DomaineName.toUtf8();
        int lengthDomaine = 14;

        byteArrayDomaine.resize(lengthDomaine);

        std::vector<uint16_t> domaineValues(lengthDomaine / 2);

        for (int i = 0; i < lengthDomaine / 2; ++i) {
            domaineValues[i] = (static_cast<uint8_t>(byteArrayDomaine[2*i]) << 8) | static_cast<uint8_t>(byteArrayDomaine[2*i + 1]);
        }

        int startRegister2 = 29194;
        modbus.writeMultipleRegisters(unitID, startRegister2, domaineValues.data(), domaineValues.size());
    }

    // 3) Ecriture du DHCP
    int dhcpIndex = DHCP;

    if (dhcpIndex == 0 || dhcpIndex == 1) {
        int startRegister = 29190;
        uint16_t value = static_cast<uint16_t>(dhcpIndex);
        bool success = modbus.writeSingleRegister(unitID, startRegister, value);

        if (!success) {
            qDebug() << "Échec de l'écriture du registre DHCP avec la valeur :" << value;
        } else {
            qDebug() << "DHCP écrit avec succès : " << value;
        }
    } else if (dhcpIndex == -1) {
        qDebug() << "DHCP non sélectionné, aucun envoi Modbus effectué.";
    }

    // 4) Adresse IP
    if (!newIp.trimmed().isEmpty())
        writeIpAddress(modbus, newIp, 29184, unitID);

    // 5) Masque
    if (!ipMask.trimmed().isEmpty())
        writeIpAddress(modbus, ipMask, 29186, unitID);

    // 6) Passerelle
    if (!ipGateway.trimmed().isEmpty())
        writeIpAddress(modbus, ipGateway, 29188, unitID);

    // 7 )DNS
    if (!ipDNS.trimmed().isEmpty())
        writeIpAddress(modbus, ipDNS, 29192, unitID);
    // 8) Ecriture de la phase
    int phaseIndex = phase;

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

    // 9) Ecriture du modèle d'acquisition
    if (!newNameModeleAcquisition.trimmed().isEmpty()) {
        if (newNameModeleAcquisition.length() % 2 != 0) {
            newNameModeleAcquisition.append('\0');
        }

        QByteArray byteArrayAcquisition = newNameModeleAcquisition.toUtf8();
        int lengthAcquisition = 10;

        byteArrayAcquisition.resize(lengthAcquisition);

        std::vector<uint16_t> modbusValuesAcquisition(lengthAcquisition / 2);

        for (int i = 0; i < lengthAcquisition / 2; ++i) {
            modbusValuesAcquisition[i] = (static_cast<uint8_t>(byteArrayAcquisition[2*i]) << 8) | static_cast<uint8_t>(byteArrayAcquisition[2*i + 1]);
        }

        long startRegisterAcquisition = 46592;
        modbus.writeMultipleRegisters(unitID2, startRegisterAcquisition, modbusValuesAcquisition.data(), modbusValuesAcquisition.size());
    }

    // 10) Ecriture de la valeur instantanée
    if (!PeriodeIntInst.trimmed().isEmpty()) {
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
    }

    // 11) Ecriture de la valeur moyenne
    if (!PeriodeIntMoy.trimmed().isEmpty()) {
        if (PeriodeIntMoy.length() % 2 != 0) {
            PeriodeIntMoy.append('\0');
        }

        QByteArray byteArrayMoyenne = PeriodeIntMoy.toUtf8();
        int lengthMoyenne = 2;

        byteArrayMoyenne.resize(lengthMoyenne);

        std::vector<uint16_t> modbusValuesMoyenne(lengthMoyenne / 2);

        for (int i = 0; i < lengthMoyenne / 2; ++i) {
            modbusValuesMoyenne[i] = (static_cast<uint8_t>(byteArrayMoyenne[2*i]) << 8) | static_cast<uint8_t>(byteArrayMoyenne[2*i + 1]);
        }

        long startRegisterMoyenne = 34817;
        modbus.writeMultipleRegisters(unitID3, startRegisterMoyenne, modbusValuesMoyenne.data(), modbusValuesMoyenne.size());
    }

    // 12) Écriture de la valeur entière 161 dans le registre 58112

    int startRegister2 = 57856;
    uint16_t value2 = 161;
    modbus.writeSingleRegister(unitID, startRegister2, value2);

}

void MainWindow::AppliquerModification() {

    // Mise à jour des variables globales uniquement si le champ utilisateur n’est pas vide
    QString tempName = ui->gatewayNameEdit->text().trimmed();
    if (!tempName.isEmpty()) newName = tempName;

    QString tempDomaine = ui->NameDomaineEdit->text().trimmed();
    if (!tempDomaine.isEmpty()) DomaineName = tempDomaine;

    QString tempIp = ui->ipAddressEdit->text().trimmed();
    if (!tempIp.isEmpty()) newIp = tempIp;

    QString tempGateway = ui->ipAddressPasserelleEdit->text().trimmed();
    if (!tempGateway.isEmpty()) ipGateway = tempGateway;

    QString tempMask = ui->ipAddressMaskEdit->text().trimmed();
    if (!tempMask.isEmpty()) ipMask = tempMask;

    QString tempDNS = ui->ipAddressDNSEdit->text().trimmed();
    if (!tempDNS.isEmpty()) ipDNS = tempDNS;

    QString tempModele = ui->NameModeleAcquisitionEdit->text().trimmed();
    if (!tempModele.isEmpty()) newNameModeleAcquisition = tempModele;

    QString tempInst = ui->PeriodeIntegrationInstEdit->text().trimmed();
    if (!tempInst.isEmpty()) PeriodeIntInst = tempInst;

    QString tempMoy = ui->PeriodeIntegrationMoyEdit->text().trimmed();
    if (!tempMoy.isEmpty()) PeriodeIntMoy = tempMoy;

    phase= ui->comboBoxCharge->currentData().toInt();
    DHCP = ui->comboBoxDHCP->currentData().toInt();
    int idDispositif= ui->PrimaryKey->currentData().toInt();
    QString idClient = ui->idClientEdit->text();


    ModifieModbus();

    if (idDispositif == -1)
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un ID de Dispositif.");
        return;
    }

    if (idDispositif == 0)
    {
        QSqlQuery queryProtocole(db);
        queryProtocole.prepare("UPDATE `Dispositif_Passerelle` SET `ID_Protocole_FK` = '0' WHERE `Dispositif_Passerelle`.`ID_Dispositif_PK` = 0;");
        queryProtocole.addBindValue(idDispositif == -1 ? QVariant() : idDispositif);

        if (!queryProtocole.exec())
        {
            QSqlError error = queryProtocole.lastError();
            QMessageBox::critical(this, "Erreur SQL", QString("Erreur lors de la modification de l'ID Protocole: %1").arg(error.text()));
            qDebug() << "Erreur SQL (ID Protocole):" << error.text() << " Requête: " << queryProtocole.lastQuery();
        } else {
            QMessageBox::information(this, "Succès", "Type de Protocole envoyer");
        }
    }

    if (idDispositif == 1)
    {
        QSqlQuery queryProtocole(db);
        queryProtocole.prepare("UPDATE `Dispositif_Passerelle` SET `ID_Protocole_FK` = '1' WHERE `Dispositif_Passerelle`.`ID_Dispositif_PK` = 1;");
        queryProtocole.addBindValue(idDispositif == -1 ? QVariant() : idDispositif);

        if (!queryProtocole.exec()) {
            QSqlError error = queryProtocole.lastError();
            QMessageBox::critical(this, "Erreur SQL", QString("Erreur lors de la modification de l'ID Protocole: %1").arg(error.text()));
            qDebug() << "Erreur SQL (ID Protocole):" << error.text() << " Requête: " << queryProtocole.lastQuery();
        } else {
            QMessageBox::information(this, "Succès", "Type de Protocole envoyer");
        }
    }

    if (db.open()) {
        QSqlQuery query(db);
        query.prepare("INSERT INTO Dispositif_Passerelle (ID_Dispositif_PK,"
                      "Nom_dispositif,"
                      "ID_Client_FK,"
                      "Adresse_IP_Passerelle,"
                      "Adresse_gateway_Passerelle,"
                      "Adresse_mask_gateway_Passerelle,"
                      "Adresse_DNS_Passerelle,"
                      "Adresse_DHCP_Passerelle,"
                      "Domaine_Passerelle,"
                      "Nom_model_acquisition,"
                      "Type_charge,"
                      "Periode_inst,"
                      "Periode_moyenne)"
                      "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?) "
                      "ON DUPLICATE KEY UPDATE "
                      "Nom_dispositif = IFNULL(?, Nom_dispositif),"
                      "ID_Client_FK = IFNULL(?, ID_Client_FK),"
                      "Adresse_IP_Passerelle = IFNULL(?, Adresse_IP_Passerelle),"
                      "Adresse_gateway_Passerelle = IFNULL(?, Adresse_gateway_Passerelle),"
                      "Adresse_mask_gateway_Passerelle = IFNULL(?, Adresse_mask_gateway_Passerelle),"
                      "Adresse_DNS_Passerelle = IFNULL(?, Adresse_DNS_Passerelle),"
                      "Adresse_DHCP_Passerelle = IFNULL(?, Adresse_DHCP_Passerelle),"
                      "Domaine_Passerelle = IFNULL(?, Domaine_Passerelle),"
                      "Nom_model_acquisition = IFNULL(?, Nom_model_acquisition),"
                      "Type_charge = IFNULL(?, Type_charge),"
                      "Periode_inst = IFNULL(?, Periode_inst),"
                      "Periode_moyenne = IFNULL(?, Periode_moyenne);");

        // Première série de addBindValue() : Pour l'instruction INSERT et les valeurs à insérer si la ligne n'existe pas.
        query.addBindValue(idDispositif);
        query.addBindValue(newName.isEmpty() ? QVariant() : newName);
        query.addBindValue(idClient.isEmpty() ? QVariant() : idClient);
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
        query.addBindValue(idClient.isEmpty() ? QVariant() : idClient);
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


        if (!query.exec()) {
            QSqlError error = query.lastError();
            QMessageBox::critical(this, "Erreur SQL", QString("requete échoué"));
            qDebug() << "Erreur SQL (AppliquerModification):" << error.text() << " Requête: " << query.lastQuery();
        } else {
            QMessageBox::information(this, "Succès", "Données mises à jour avec succès.");
        }
    }
}


void MainWindow::on_AppliquerModification_2_clicked()
{
    QString idClient = ui->idClientEdit->text();
    QString MDP = ui->MDPEdit->text(); // Mot de passe en clair
    int idDispositif = ui->PrimaryKey->currentData().toInt();

    // Vérifications d'entrée
    if (idDispositif == -1)
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un ID de Dispositif.");
        return;
    }

    if (idClient.isEmpty())
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez saisir un ID client.");
        return;
    }

    if (MDP.isEmpty())
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez saisir un Mot de passe.");
        return;
    }
    quint64 randValue = QRandomGenerator::system()->generate64();
    QByteArray sel = QByteArray::number(randValue).toHex();

    QByteArray mdpSale = MDP.toUtf8() + sel;
    QByteArray hash = QCryptographicHash::hash(mdpSale, QCryptographicHash::Sha256).toHex();

    QSqlQuery queryClientWeb(db);
    queryClientWeb.prepare("INSERT INTO Client_Web (`ID_Client_PK`, `Mot_de_passe`, `Salt`) VALUES (?, ?, ?) "
                           "ON DUPLICATE KEY UPDATE "
                           "Mot_de_passe = ?, Salt = ?;");

    // Liaison des valeurs pour INSERT et UPDATE
    queryClientWeb.addBindValue(idClient);
    queryClientWeb.addBindValue(hash);
    queryClientWeb.addBindValue(sel);

    queryClientWeb.addBindValue(hash);
    queryClientWeb.addBindValue(sel);

    qDebug() << "ID:" << idClient << "HASH:" << hash << "SEL:" << sel;

    if (!queryClientWeb.exec())
    {
        QSqlError error = queryClientWeb.lastError();
        QMessageBox::critical(this, "Erreur SQL", QString("Erreur lors de la modification du Client Web: %1").arg(error.text()));
        qDebug() << "Erreur SQL (Client Web):" << error.text() << " Requête: " << queryClientWeb.lastQuery();
        return;
    } else {
        QMessageBox::information(this, "Succès", "Information Client mise à jour dans Client_Web.");
    }
}


void MainWindow::on_pushButton_clicked()
{
    if (passwordVisible)
    {
        // Le mot de passe est actuellement visible, masquez-le
        ui->MDPEdit->setEchoMode(QLineEdit::Password);
        ui->showHideButton->setIcon(QIcon("C:/Users/EtudiantIR2/Documents/Appli_BDD/preview-show-interface-icon-free-vector.jpg")); // Icône "œil fermé"
        passwordVisible = false;
    } else
    {
        // Le mot de passe est actuellement masqué, affichez-le
        ui->MDPEdit->setEchoMode(QLineEdit::Normal);
        ui->showHideButton->setIcon(QIcon("C:/Users/EtudiantIR2/Documents/Appli_BDD/eye-slash-icon-symbol-design-illustration-vector.jpg")); // Icône "œil ouvert" ou "œil barré"
        passwordVisible = true;
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
    ui->AppliquerModification->setVisible(false);
    ui->returnButton->setVisible(true);
    ui->modifyGatewayButton->setVisible(false);
    ui->acquisitionModelButton->setVisible(false);
    ui->measureModelButton->setVisible(false);
    ui->ipAddressPasserelleLabel->setVisible(true);
    ui->ipAddressDNSLabel->setVisible(true);
    ui->ipAddressMaskLabel->setVisible(true);
    ui->DHCPLabel->setVisible(true);
    ui->comboBoxDHCP->setVisible(true);
    ui->ipAddressPasserelleEdit->setVisible(true);
    ui->ipAddressMaskEdit->setVisible(true);
    ui->ipAddressDNSEdit->setVisible(true);
    ui->NameDomaineEdit->setVisible(true);
    ui->NameDomaine->setVisible(true);
    ui->OFFButton->setVisible(false);
    ui->PrimaryKey->setVisible(false);
    ui->PrimaryKeylabel->setVisible(false);
    ui->ClientWebButton->setVisible(false);
}

void MainWindow::acquisitionModelButtonClicked() {
    ui->NameModeleAcquisitionEdit->setVisible(true);
    ui->NameModeleAquisitionLabel->setVisible(true);
    ui->AppliquerModification->setVisible(false);
    ui->modifyGatewayButton->setVisible(false);
    ui->acquisitionModelButton->setVisible(false);
    ui->measureModelButton->setVisible(false);
    ui->returnButton2->setVisible(true);
    ui->TypeCharge->setVisible(true);
    ui->comboBoxCharge->setVisible(true);
    ui->OFFButton->setVisible(false);
    ui->PrimaryKey->setVisible(false);
    ui->PrimaryKeylabel->setVisible(false);
    ui->ClientWebButton->setVisible(false);
}

void MainWindow::measureModelButtonClicked() {;
    ui->returnButton3->setVisible(true);
    ui->AppliquerModification->setVisible(false);
    ui->modifyGatewayButton->setVisible(false);
    ui->acquisitionModelButton->setVisible(false);
    ui->measureModelButton->setVisible(false);
    ui->PeriodeIntegrationInst->setVisible(true);
    ui->PeriodeIntegrationInstEdit->setVisible(true);
    ui->PeriodeIntegrationMoy->setVisible(true);
    ui->PeriodeIntegrationMoyEdit->setVisible(true);
    ui->OFFButton->setVisible(false);
    ui->PrimaryKey->setVisible(false);
    ui->PrimaryKeylabel->setVisible(false);
    ui->ClientWebButton->setVisible(false);
}

void MainWindow::returnButtonClicked() {
    ui->gatewayNameEdit->setVisible(false);
    ui->ipAddressEdit->setVisible(false);
    ui->gatewayNameLabel->setVisible(false);
    ui->ipAddressLabel->setVisible(false);
    ui->returnButton->setVisible(false);
    ui->modifyGatewayButton->setVisible(true);
    ui->acquisitionModelButton->setVisible(true);
    ui->measureModelButton->setVisible(true);
    ui->ipAddressPasserelleLabel->setVisible(false);
    ui->ipAddressDNSLabel->setVisible(false);
    ui->ipAddressMaskLabel->setVisible(false);
    ui->DHCPLabel->setVisible(false);
    ui->comboBoxDHCP->setVisible(false);
    ui->ipAddressPasserelleEdit->setVisible(false);
    ui->ipAddressMaskEdit->setVisible(false);
    ui->ipAddressDNSEdit->setVisible(false);
    ui->NameDomaineEdit->setVisible(false);
    ui->NameDomaine->setVisible(false);
    ui->OFFButton->setVisible(true);
    ui->PrimaryKey->setVisible(true);
    ui->PrimaryKeylabel->setVisible(true);
    ui->AppliquerModification->setVisible(true);
    ui->ClientWebButton->setVisible(true);
}

void MainWindow::returnButtonClicked2()
{
    ui->modifyGatewayButton->setVisible(true);
    ui->AppliquerModification->setVisible(true);
    ui->acquisitionModelButton->setVisible(true);
    ui->measureModelButton->setVisible(true);
    ui->NameModeleAcquisitionEdit->setVisible(false);
    ui->NameModeleAquisitionLabel->setVisible(false);
    ui->returnButton2->setVisible(false);
    ui->TypeCharge->setVisible(false);
    ui->comboBoxCharge->setVisible(false);
    ui->OFFButton->setVisible(true);
    ui->PrimaryKey->setVisible(true);
    ui->PrimaryKeylabel->setVisible(true);
    ui->ClientWebButton->setVisible(true);
}

void MainWindow::returnButtonClicked3()
{
    ui->modifyGatewayButton->setVisible(true);
    ui->AppliquerModification->setVisible(true);
    ui->acquisitionModelButton->setVisible(true);
    ui->measureModelButton->setVisible(true);
    ui->returnButton3->setVisible(false);
    ui->PeriodeIntegrationInst->setVisible(false);
    ui->PeriodeIntegrationInstEdit->setVisible(false);
    ui->PeriodeIntegrationMoy->setVisible(false);
    ui->PeriodeIntegrationMoyEdit->setVisible(false);
    ui->OFFButton->setVisible(true);
    ui->PrimaryKey->setVisible(true);
    ui->PrimaryKeylabel->setVisible(true);
    ui->ClientWebButton->setVisible(true);
}

void MainWindow::on_returnButton4_clicked()
{
    ui->PrimaryKey->setVisible(false);
    ui->PrimaryKeylabel->setVisible(false);
    ui->modifyGatewayButton->setVisible(true);
    ui->acquisitionModelButton->setVisible(true);
    ui->measureModelButton->setVisible(true);
    ui->ClientWebButton->setVisible(true);
    ui->AppliquerModification_2->setVisible(false);
    ui->idClientEdit->setVisible(false);
    ui->idClientLabel->setVisible(false);
    ui->MDPEdit->setVisible(false);
    ui->MDPLabel->setVisible(false);
    ui->returnButton4->setVisible(false);
    ui->showHideButton->setVisible(false);
    ui->OFFButton->setVisible(true);
    ui->PrimaryKey->setVisible(true);
    ui->PrimaryKeylabel->setVisible(true);
    ui->ClientWebButton->setVisible(true);
    resetInputFields();
}

void MainWindow::resetInputFields()
{
    ui->gatewayNameEdit->clear();
    ui->ipAddressEdit->clear();
    ui->ipAddressPasserelleEdit->clear();
    ui->ipAddressMaskEdit->clear();
    ui->ipAddressDNSEdit->clear();
    ui->NameModeleAcquisitionEdit->clear();
    ui->PeriodeIntegrationInstEdit->clear();
    ui->PeriodeIntegrationMoyEdit->clear();
    ui->NameDomaineEdit->clear();
    ui->idClientEdit->clear();
    ui->comboBoxCharge->setCurrentIndex(0);
    ui->PrimaryKey->setCurrentIndex(0);
}

void MainWindow::on_ClientWebButton_clicked()
{
    ui->idClientEdit->setVisible(true);
    ui->idClientLabel->setVisible(true);
    ui->MDPEdit->setVisible(true);
    ui->MDPLabel->setVisible(true);
    ui->MDPEdit->setEchoMode(QLineEdit::Password);
    ui->returnButton4->setVisible(true);
    ui->AppliquerModification_2->setVisible(true);
    ui->modifyGatewayButton->setVisible(false);
    ui->acquisitionModelButton->setVisible(false);
    ui->measureModelButton->setVisible(false);
    ui->ClientWebButton->setVisible(false);
    ui->showHideButton->setVisible(true);
    ui->PrimaryKey->setVisible(false);
    ui->PrimaryKeylabel->setVisible(false);
    ui->OFFButton->setVisible(false);
    ui->AppliquerModification->setVisible(false);
}
