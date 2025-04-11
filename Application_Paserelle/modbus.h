#ifndef MODBUS_H
#define MODBUS_H

#include <QString>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QDebug>

class ModbusCommunicator {
public:
    ModbusCommunicator(const QString& serverIp, int port);
    ~ModbusCommunicator();

    uint16_t readModbusRegister(unsigned char* request, int requestSize);  // Méthode pour lire un registre
    bool writeModbusRegister(unsigned char* request, int requestSize);    // Méthode pour écrire un registre
    bool isConnected() const;  // Vérifie la connexion
    void connectToServer();    // Connexion au serveur Modbus

private:
    void initializeWinsock();  // Initialisation de Winsock
    void disconnectFromServer();  // Déconnexion du serveur

    QString serverIp;  // Adresse IP du serveur Modbus
    int port;  // Port du serveur Modbus
    SOCKET sock;  // Socket de connexion
};

#endif // MODBUS_H
