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

    bool writeSingleRegister(uint8_t unitId, uint16_t address, uint16_t value);
    bool writeMultipleRegisters(uint8_t unitId, uint16_t startAddress, const uint16_t* values, int count);
    void writeIpAddress(ModbusCommunicator& modbus, const QString& ipString, int startRegister, int unitID);

    bool isConnected() const;

private:
    void initializeWinsock();
    void connectToServer();
    void disconnectFromServer();

    QString serverIp;
    int port;
    SOCKET sock;
};

#endif // MODBUS_H
