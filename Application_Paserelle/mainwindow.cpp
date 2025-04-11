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
    loadJson();
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
    ui->EtatModbusLabel->setVisible(false);
    ui->comboBoxModbus->setVisible(false);
    ui->TimeZoneLabel->setVisible(false);
    ui->ipAddressPaserelleEdit->setVisible(false);
    ui->comboBoxTimeZone->setVisible(false);
    ui->ipAddressMaskEdit->setVisible(false);
    ui->ipAddressDNSEdit->setVisible(false);
    ui->returnButton->setVisible(false);
    ui->NameModeleAcquisitionEdit->setVisible(false);
    ui->NameModeleAquisitionLabel->setVisible(false);
    ui->returnButton2->setVisible(false);
    ui->returnButton3->setVisible(false);
    ui->status1->setVisible(false);
    ui->status2->setVisible(false);
    ui->status3->setVisible(false);
    ui->comboBoxStatus1->setVisible(false);
    ui->comboBoxStatus2->setVisible(false);
    ui->comboBoxStatus3->setVisible(false);
    ui->comboBoxUsage1->setVisible(false);
    ui->comboBoxUsage2->setVisible(false);
    ui->comboBoxUsage3->setVisible(false);
    ui->usage1->setVisible(false);
    ui->usage2->setVisible(false);
    ui->usage3->setVisible(false);
    ui->TypeCharge->setVisible(false);
    ui->comboBoxCharge->setVisible(false);
    ui->PeriodeIntegrationInst->setVisible(false);
    ui->PeriodeIntegrationInstEdit->setVisible(false);
    ui->PeriodeIntegrationMoy->setVisible(false);
    ui->PeriodeIntegrationMoyEdit->setVisible(false);
    ui->PeriodeIntegrationCourbe->setVisible(false);
    ui->PeriodeIntegrationCourbeEdit->setVisible(false);
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
    connect(ui->modifyGatewayButton, &QPushButton::clicked, this, &MainWindow::modifyGatewayButtonClicked);
    connect(ui->acquisitionModelButton, &QPushButton::clicked, this, &MainWindow::acquisitionModelButtonClicked);
    connect(ui->measureModelButton, &QPushButton::clicked, this, &MainWindow::measureModelButtonClicked);
    connect(ui->returnButton, &QPushButton::clicked, this, &MainWindow::returnButtonClicked);
    connect(ui->returnButton2, &QPushButton::clicked, this, &MainWindow::returnButtonClicked2);
    connect(ui->returnButton3, &QPushButton::clicked, this, &MainWindow::returnButtonClicked3);
    ui->comboBoxTimeZone->addItem("", -1);
    ui->comboBoxTimeZone->addItem("États-Unis New York UTC-5", 27);
    ui->comboBoxTimeZone->addItem("États-Unis Los Angeles UTC-8", 24);
    ui->comboBoxTimeZone->addItem("Chine UTC+8", 40);
    ui->comboBoxTimeZone->addItem("France UTC+1", 33);
    ui->comboBoxTimeZone->addItem("Russie (Moscou) UTC+3", 35);
    ui->comboBoxTimeZone->addItem(" Japon UTC+9", 41);
    ui->comboBoxTimeZone->addItem("Allemagne UTC+1", 33);
    ui->comboBoxDHCP->addItem("", -1);
    ui->comboBoxDHCP->addItem("Activé", 1);
    ui->comboBoxDHCP->addItem("Desactivé", 0);
    ui->comboBoxModbus->addItem("", -1);
    ui->comboBoxModbus->addItem("Activé", 1);
    ui->comboBoxModbus->addItem("Desactivé", 0);
    ui->comboBoxStatus1->addItem("", -1);
    ui->comboBoxStatus1->addItem("Activé", 1);
    ui->comboBoxStatus1->addItem("Desactivé", 0);
    ui->comboBoxStatus2->addItem("", -1);
    ui->comboBoxStatus2->addItem("Activé", 1);
    ui->comboBoxStatus2->addItem("Desactivé", 0);
    ui->comboBoxStatus3->addItem("", -1);
    ui->comboBoxStatus3->addItem("Activé", 1);
    ui->comboBoxStatus3->addItem("Desactivé", 0);
    ui->comboBoxUsage1->addItem("", -1);
    ui->comboBoxUsage1->addItem("Activé", 1);
    ui->comboBoxUsage1->addItem("Desactivé", 0);
    ui->comboBoxUsage2->addItem("", -1);
    ui->comboBoxUsage2->addItem("Activé", 1);
    ui->comboBoxUsage2->addItem("Desactivé", 0);
    ui->comboBoxUsage3->addItem("", -1);
    ui->comboBoxUsage3->addItem("Activé", 1);
    ui->comboBoxUsage3->addItem("Desactivé", 0);
    ui->comboBoxCharge->addItem("", -1);
    ui->comboBoxCharge->addItem("monophasé 1P+N-1TC (0)", 0);
    ui->comboBoxCharge->addItem("triphasé 3P+N-3TC (1)", 1);
    ui->PrimaryKey->addItem("", -1);
    ui->PrimaryKey->addItem("Modbus (0)", 0);
    ui->PrimaryKey->addItem("MQTT (1)", 1);
    ui->PrimaryKey->addItem("ALL (2)", 2);

}








void MainWindow::sendConfigToGateway(const QJsonObject &config) {
    QUrl url("http://192.168.22.1/api/config"); // Utilisez l'adresse IP de la passerelle
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QString userAndPassword = "Admin:Adminsn1234$";
    QByteArray base64Encoded = userAndPassword.toUtf8().toBase64();
    request.setRawHeader("Authorization", "Basic " + base64Encoded);


    QJsonDocument doc(config);
    QByteArray jsonData = doc.toJson();

    QNetworkReply *reply = networkManager->put(request, jsonData);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
            qDebug() << "Réponse de la passerelle:" << responseDoc;
            QMessageBox::information(this, "Succès", "Configuration envoyée à la passerelle.");
        } else {
            qDebug() << "Erreur lors de l'envoi de la configuration:" << reply->errorString();
            QMessageBox::critical(this, "Erreur", "Erreur lors de l'envoi de la configuration à la passerelle: " + reply->errorString());
        }
        reply->deleteLater();
    });
}








void MainWindow::loadJson() {
    auto loadFromFile = [](const QString &path, QJsonArray &outArray) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        if (!doc.isObject()) return false;
        QJsonObject obj = doc.object();
        QJsonValue val = obj.value("datavalues");
        if (!val.isArray()) return false;

        outArray = val.toArray();
        return true;
    };

    if (!loadFromFile(filePathPaserelle, jsonArrayPaserelle)) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger le fichier JSON Paserelle.");
    }
    if (!loadFromFile(filePathModele, jsonArrayModele)) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger le fichier JSON Modele.");
    }
    if (!loadFromFile(filePathMesure, jsonArrayMesure)) {
        QMessageBox::warning(this, "Erreur", "Impossible de charger le fichier JSON Modele.");
    }
}

bool MainWindow::saveJson() {
    int idDispositif= ui->PrimaryKey->currentData().toInt();
    if (idDispositif == -1) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un ID de Dispositif.");
        return false; // Retourne false en cas d'erreur
    }

    auto saveToFile = [](const QString &path, const QJsonArray &array) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) return false;

        QJsonObject obj;
        obj["datavalues"] = array;

        QJsonDocument doc(obj);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        return true;
    };

    bool paserelleSaved = saveToFile(filePathPaserelle, jsonArrayPaserelle);
    bool modeleSaved = saveToFile(filePathModele, jsonArrayModele);
    bool mesureSaved = saveToFile(filePathMesure, jsonArrayMesure);

    if (!paserelleSaved) {
        QMessageBox::warning(this, "Erreur", "Impossible de sauvegarder le fichier JSON Paserelle.");
    }
    if (!modeleSaved) {
        QMessageBox::warning(this, "Erreur", "Impossible de sauvegarder le fichier JSON Modele.");
    }
    if (!mesureSaved) {
        QMessageBox::warning(this, "Erreur", "Impossible de sauvegarder le fichier JSON Mesure.");
    }

    return paserelleSaved && modeleSaved && mesureSaved; // Retourne true si toutes les sauvegardes réussissent
}


void MainWindow::updateValue(QJsonArray &array, int id, const QString &newValue) {
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject obj = array[i].toObject();
        if (obj["id"] == id) {
            obj["current"] = newValue;
            array[i] = obj;
            break;
        }
    }
}

void MainWindow::AppliquerModification() {
    QString newName = ui->gatewayNameEdit->text();
    QString newIp = ui->ipAddressEdit->text();
    QString ipGateway = ui->ipAddressPaserelleEdit->text();
    QString ipMask = ui->ipAddressMaskEdit->text();
    QString ipDNS = ui->ipAddressDNSEdit->text();
    QString newNameModeleAcquisition = ui->NameModeleAcquisitionEdit->text();
    QString PeriodeIntInst = ui->PeriodeIntegrationInstEdit->text();
    QString PeriodeIntMoy = ui->PeriodeIntegrationMoyEdit->text();
    QString PeriodeIntCourbe = ui->PeriodeIntegrationCourbeEdit->text();
    QString DomaineName = ui->NameDomaineEdit->text();


    int timeZoneValue = ui->comboBoxTimeZone->currentData().toInt();
    int DHCP = ui->comboBoxDHCP->currentData().toInt();
    int Modbus = ui->comboBoxModbus->currentData().toInt();
    int status1 = ui->comboBoxStatus1->currentData().toInt();
    int status2 = ui->comboBoxStatus2->currentData().toInt();
    int status3 = ui->comboBoxStatus3->currentData().toInt();
    int usage1 = ui->comboBoxUsage1->currentData().toInt();
    int usage2 = ui->comboBoxUsage2->currentData().toInt();
    int usage3= ui->comboBoxUsage3->currentData().toInt();
    int phase= ui->comboBoxCharge->currentData().toInt();
    int idDispositif= ui->PrimaryKey->currentData().toInt();

    bool jsonModified = false;

    if (!newName.isEmpty()) {
        updateValue(jsonArrayPaserelle, 425480, newName);
        jsonModified = true;
    }
    if (!newIp.isEmpty()) {
        updateValue(jsonArrayPaserelle, 425470, newIp);
        jsonModified = true;
    }
    if (!ipGateway.isEmpty()) {
        updateValue(jsonArrayPaserelle, 425472, ipGateway);
        jsonModified = true;
    }
    if (!ipMask.isEmpty()) {
        updateValue(jsonArrayPaserelle, 425471, ipMask);
        jsonModified = true;
    }
    if (!ipDNS.isEmpty()) {
        updateValue(jsonArrayPaserelle, 425474, ipDNS);
        jsonModified = true;
    }
    if (timeZoneValue != -1) {
        updateValue(jsonArrayPaserelle, 425521, QString::number(timeZoneValue));
        jsonModified = true;
    }
    if (DHCP != -1) {
        updateValue(jsonArrayPaserelle, 425473, QString::number(DHCP));
        jsonModified = true;
    }
    if (Modbus != -1) {
        updateValue(jsonArrayPaserelle, 425482, QString::number(Modbus));
        jsonModified = true;
    }
    if (!DomaineName.isEmpty()) {
        updateValue(jsonArrayPaserelle, 425475, DomaineName);
        jsonModified = true;
    }
    if (!newNameModeleAcquisition.isEmpty()) {
        updateValue(jsonArrayModele, 408153, newNameModeleAcquisition);
        jsonModified = true;
    }
    if (status1 != -1) {
        updateValue(jsonArrayModele, 408126, QString::number(status1));
        jsonModified = true;
    }
    if (status2 != -1) {
        updateValue(jsonArrayModele, 408127, QString::number(status2));
        jsonModified = true;
    }
    if (status3 != -1) {
        updateValue(jsonArrayModele, 408128, QString::number(status3));
        jsonModified = true;
    }
    if (usage1 != -1) {
        updateValue(jsonArrayModele, 408129, QString::number(usage1));
        jsonModified = true;
    }
    if (usage2 != -1) {
        updateValue(jsonArrayModele, 408130, QString::number(usage2));
        jsonModified = true;
    }
    if (usage3 != -1) {
        updateValue(jsonArrayModele, 408131, QString::number(usage3));
        jsonModified = true;
    }
    if (phase != -1) {
        updateValue(jsonArrayModele, 408174, QString::number(phase));
        jsonModified = true;
    }
    if (!PeriodeIntInst.isEmpty()) {
        updateValue(jsonArrayMesure, 408292, PeriodeIntInst);
        jsonModified = true;
    }
    if (!PeriodeIntMoy.isEmpty()) {
        updateValue(jsonArrayMesure, 408293, PeriodeIntMoy);
        jsonModified = true;
    }
    if (!PeriodeIntCourbe.isEmpty()) {
        updateValue(jsonArrayMesure, 408299, PeriodeIntCourbe);
        jsonModified = true;
    }

    bool dbSuccess = false;
    if (db.open()) {
        QSqlQuery query(db);
        query.prepare("INSERT INTO Dispositif_Passerelle (ID_Dispositif_PK,"
                      "Nom_dispositif,"
                      "Adresse_IP_Passerelle,"
                      "Adresse_gateway_Passerelle,"
                      "Adresse_mask_gateway_Passerelle,"
                      "Adresse_DNS_Passerelle,"
                      "time_zone_passerelle,"
                      "Adresse_DHCP_Passerelle,"
                      "Imodbus,"
                      "Domaine_Passerelle,"
                      "Nom_model_acquisition,"
                      "Statut_Capteur_1,"
                      "Statut_Capteur_2,"
                      "Statut_Capteur_3,"
                      "usage_Capteur_1,"
                      "usage_Capteur_2,"
                      "usage_Capteur_3,"
                      "Type_charge,"
                      "Periode_inst,"
                      "Periode_moyenne,"
                      "Periode_courbe) "
                      "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?) "
                      "ON DUPLICATE KEY UPDATE "
                      "Nom_dispositif = IFNULL(?, Nom_dispositif),"
                      "Adresse_IP_Passerelle = IFNULL(?, Adresse_IP_Passerelle),"
                      "Adresse_gateway_Passerelle = IFNULL(?, Adresse_gateway_Passerelle),"
                      "Adresse_mask_gateway_Passerelle = IFNULL(?, Adresse_mask_gateway_Passerelle),"
                      "Adresse_DNS_Passerelle = IFNULL(?, Adresse_DNS_Passerelle),"
                      "time_zone_passerelle = IFNULL(?, time_zone_passerelle),"
                      "Adresse_DHCP_Passerelle = IFNULL(?, Adresse_DHCP_Passerelle),"
                      "Imodbus = IFNULL(?, Imodbus),"
                      "Domaine_Passerelle = IFNULL(?, Domaine_Passerelle),"
                      "Nom_model_acquisition = IFNULL(?, Nom_model_acquisition),"
                      "Statut_Capteur_1 = IFNULL(?, Statut_Capteur_1),"
                      "Statut_Capteur_2 = IFNULL(?, Statut_Capteur_2),"
                      "Statut_Capteur_3 = IFNULL(?, Statut_Capteur_3),"
                      "usage_Capteur_1 = IFNULL(?, usage_Capteur_1),"
                      "usage_Capteur_2 = IFNULL(?, usage_Capteur_2),"
                      "usage_Capteur_3 = IFNULL(?, usage_Capteur_3),"
                      "Type_charge = IFNULL(?, Type_charge),"
                      "Periode_inst = IFNULL(?, Periode_inst),"
                      "Periode_moyenne = IFNULL(?, Periode_moyenne),"
                      "Periode_courbe = IFNULL(?, Periode_courbe)");

        // Première série de addBindValue() : Pour l'instruction INSERT et les valeurs à insérer si la ligne n'existe pas.
        query.addBindValue(idDispositif);
        query.addBindValue(newName.isEmpty() ? QVariant() : newName);
        query.addBindValue(newIp.isEmpty() ? QVariant() : newIp);
        query.addBindValue(ipGateway.isEmpty() ? QVariant() : ipGateway);
        query.addBindValue(ipMask.isEmpty() ? QVariant() : ipMask);
        query.addBindValue(ipDNS.isEmpty() ? QVariant() : ipDNS);
        query.addBindValue(timeZoneValue == -1 ? QVariant() : timeZoneValue);
        query.addBindValue(DHCP == -1 ? QVariant() : DHCP);
        query.addBindValue(Modbus == -1 ? QVariant() : Modbus);
        query.addBindValue(DomaineName.isEmpty() ? QVariant() : DomaineName);
        query.addBindValue(newNameModeleAcquisition.isEmpty() ? QVariant() : newNameModeleAcquisition);
        query.addBindValue(status1 == -1 ? QVariant() : status1);
        query.addBindValue(status2 == -1 ? QVariant() : status2);
        query.addBindValue(status3 == -1 ? QVariant() : status3);
        query.addBindValue(usage1 == -1 ? QVariant() : usage1);
        query.addBindValue(usage2 == -1 ? QVariant() : usage2);
        query.addBindValue(usage3 == -1 ? QVariant() : usage3);
        query.addBindValue(phase == -1 ? QVariant() : phase);
        query.addBindValue(PeriodeIntInst.isEmpty() ? QVariant() : PeriodeIntInst);
        query.addBindValue(PeriodeIntMoy.isEmpty() ? QVariant() : PeriodeIntMoy);
        query.addBindValue(PeriodeIntCourbe.isEmpty() ? QVariant() : PeriodeIntCourbe);

        // La première série de query.addBindValue() fournit les valeurs qui seront utilisées pour créer une nouvelle ligne
        // dans la table Dispositif_Passerelle si aucune ligne avec la même clé primaire (ID_Dispositif_PK) n'existe déjà.
        // Ces valeurs sont passées directement dans la clause VALUES (?,?,?,...) de l'instruction INSERT.*
        // Il faut au moin une valeur dans un autre attribut pour que la nouvelle table soit crée

        query.addBindValue(newName.isEmpty() ? QVariant() : newName);
        query.addBindValue(newIp.isEmpty() ? QVariant() : newIp);
        query.addBindValue(ipGateway.isEmpty() ? QVariant() : ipGateway);
        query.addBindValue(ipMask.isEmpty() ? QVariant() : ipMask);
        query.addBindValue(ipDNS.isEmpty() ? QVariant() : ipDNS);
        query.addBindValue(timeZoneValue == -1 ? QVariant() : timeZoneValue);
        query.addBindValue(DHCP == -1 ? QVariant() : DHCP);
        query.addBindValue(Modbus == -1 ? QVariant() : Modbus);
        query.addBindValue(DomaineName.isEmpty() ? QVariant() : DomaineName);
        query.addBindValue(newNameModeleAcquisition.isEmpty() ? QVariant() : newNameModeleAcquisition);
        query.addBindValue(status1 == -1 ? QVariant() : status1);
        query.addBindValue(status2 == -1 ? QVariant() : status2);
        query.addBindValue(status3 == -1 ? QVariant() : status3);
        query.addBindValue(usage1 == -1 ? QVariant() : usage1);
        query.addBindValue(usage2 == -1 ? QVariant() : usage2);
        query.addBindValue(usage3 == -1 ? QVariant() : usage3);
        query.addBindValue(phase == -1 ? QVariant() : phase);
        query.addBindValue(PeriodeIntInst.isEmpty() ? QVariant() : PeriodeIntInst);
        query.addBindValue(PeriodeIntMoy.isEmpty() ? QVariant() : PeriodeIntMoy);
        query.addBindValue(PeriodeIntCourbe.isEmpty() ? QVariant() : PeriodeIntCourbe);

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
    // Sauvegarde du JSON après la tentative de modification de la BDD
    bool jsonSaved = saveJson();




    if (jsonSaved) {
        auto sendFileContent = [this](const QString& filePath) {
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QByteArray jsonData = file.readAll();
                file.close();
                QJsonDocument doc = QJsonDocument::fromJson(jsonData);
                if (!doc.isNull()) {
                    sendConfigToGateway(doc.object()); // Envoie l'objet racine du JSON
                } else {
                    qDebug() << "Erreur lors de la lecture ou de la conversion JSON du fichier:" << filePath;
                    QMessageBox::warning(this, "Erreur", "Erreur lors de la lecture du fichier JSON: " + filePath);
                }
            } else {
                qDebug() << "Impossible d'ouvrir le fichier:" << filePath;
                QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier JSON: " + filePath);
            }
        };

        sendFileContent(filePathPaserelle);
        sendFileContent(filePathModele);
        sendFileContent(filePathMesure);

        QMessageBox::information(this, "Information", "Les fichiers de configuration JSON ont été envoyés à la passerelle.");
    } else {
        QMessageBox::warning(this, "Avertissement", "Erreur lors de la sauvegarde des fichiers JSON. L'envoi à la passerelle a été annulé.");
    }




    if (dbSuccess && jsonSaved && jsonModified) {
        QMessageBox::information(this, "Succès", "Configuration appliquée avec succès.");
    } else if (dbSuccess && jsonSaved && !jsonModified) {
        QMessageBox::information(this, "Succès", "Configuration de la base de données appliquée (aucun changement JSON détecté).");
    } else if (!dbSuccess && jsonSaved && jsonModified) {
        QMessageBox::warning(this, "Avertissement", "La configuration JSON a été sauvegardée, mais une erreur est survenue lors de la modification de la base de données.");
    } else if (dbSuccess && !jsonSaved && jsonModified) {
        QMessageBox::warning(this, "Avertissement", "La base de données a été mise à jour, mais une erreur est survenue lors de la sauvegarde de la configuration JSON.");
    } else if (!dbSuccess && !jsonSaved && jsonModified) {
        QMessageBox::critical(this, "Erreur", "Une erreur est survenue lors de l'application de la configuration (base de données et sauvegarde JSON échouée).");
    } else if (!dbSuccess && jsonSaved && !jsonModified) {
        QMessageBox::warning(this, "Avertissement", "Aucun changement effectuer (aucun changement JSON détecté).");
    } else if (dbSuccess && !jsonSaved && !jsonModified) {
        QMessageBox::warning(this, "Avertissement", "Erreur lors de la sauvegarde du JSON (aucun changement JSON détecté).");
    } else {
        QMessageBox::information(this, "Information", "Aucune modification détectée.");
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
    ui->EtatModbusLabel->setVisible(true);
    ui->comboBoxModbus->setVisible(true);
    ui->TimeZoneLabel->setVisible(true);
    ui->ipAddressPaserelleEdit->setVisible(true);
    ui->comboBoxTimeZone->setVisible(true);
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
    ui->EtatModbusLabel->setVisible(false);
    ui->comboBoxModbus->setVisible(false);
    ui->TimeZoneLabel->setVisible(false);
    ui->ipAddressPaserelleEdit->setVisible(false);
    ui->comboBoxTimeZone->setVisible(false);
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
    ui->status1->setVisible(true);
    ui->status2->setVisible(true);
    ui->status3->setVisible(true);
    ui->comboBoxStatus1->setVisible(true);
    ui->comboBoxStatus2->setVisible(true);
    ui->comboBoxStatus3->setVisible(true);
    ui->comboBoxUsage1->setVisible(true);
    ui->comboBoxUsage2->setVisible(true);
    ui->comboBoxUsage3->setVisible(true);
    ui->usage1->setVisible(true);
    ui->usage2->setVisible(true);
    ui->usage3->setVisible(true);
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
    ui->status1->setVisible(false);
    ui->status2->setVisible(false);
    ui->status3->setVisible(false);
    ui->comboBoxStatus1->setVisible(false);
    ui->comboBoxStatus2->setVisible(false);
    ui->comboBoxStatus3->setVisible(false);
    ui->comboBoxUsage1->setVisible(false);
    ui->comboBoxUsage2->setVisible(false);
    ui->comboBoxUsage3->setVisible(false);
    ui->usage1->setVisible(false);
    ui->usage2->setVisible(false);
    ui->usage3->setVisible(false);
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
    ui->PeriodeIntegrationCourbe->setVisible(true);
    ui->PeriodeIntegrationCourbeEdit->setVisible(true);
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
    ui->PeriodeIntegrationCourbe->setVisible(false);
    ui->PeriodeIntegrationCourbeEdit->setVisible(false);
}
