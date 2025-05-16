#include "modbus.h"

ModbusCommunicator::ModbusCommunicator(const QString& serverIp, int port)
    : serverIp(serverIp), port(port), sock(INVALID_SOCKET) {
    initializeWinsock();
    connectToServer();
}

ModbusCommunicator::~ModbusCommunicator() {
    disconnectFromServer();
    WSACleanup();
}

bool ModbusCommunicator::isConnected() const {
    return sock != INVALID_SOCKET;
}

void ModbusCommunicator::initializeWinsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        qDebug() << "Erreur d'initialisation de Winsock";
    }
}

void ModbusCommunicator::connectToServer() {
    disconnectFromServer();

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        qDebug() << "Erreur de création du socket";
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverIp.toStdString().c_str(), &serverAddr.sin_addr);

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        qDebug() << "Erreur de connexion";
        disconnectFromServer();
    }
}

void ModbusCommunicator::disconnectFromServer() {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
}

// Écrire un registre simple (fonction 0x06)
bool ModbusCommunicator::writeSingleRegister(uint8_t unitId, uint16_t address, uint16_t value) {
    if (sock == INVALID_SOCKET) {
        qDebug() << "Socket invalide, pas connecté";
        return false;
    }

    // Construction de la trame Modbus TCP (MBAP + PDU)
    uint8_t request[12];
    // Transaction ID
    request[0] = 0x00; request[1] = 0x01;
    // Protocol ID
    request[2] = 0x00; request[3] = 0x00;
    // Length (6 bytes = UnitId + Function + Addr(2) + Value(2))
    request[4] = 0x00; request[5] = 0x06;
    // Unit ID
    request[6] = unitId;
    // Fonction 0x06 (Write Single Register)
    request[7] = 0x06;
    // Adresse registre
    request[8] = address >> 8;
    request[9] = address & 0xFF;
    // Valeur à écrire
    request[10] = value >> 8;
    request[11] = value & 0xFF;

    int sentBytes = send(sock, (char*)request, sizeof(request), 0);
    if (sentBytes != sizeof(request)) {
        qDebug() << "Erreur d'envoi de la requête Modbus";
        return false;
    }

    uint8_t response[256];
    int received = recv(sock, (char*)response, sizeof(response), 0);
    if (received < 12) {
        qDebug() << "Réponse Modbus trop courte ou erreur";
        return false;
    }

    if (response[7] == 0x06) { // Fonction écriture simple OK
        return true;
    } else if ((response[7] & 0x80) != 0) { // Exception Modbus
        qDebug() << "Exception Modbus, code:" << response[8];
        return false;
    }

    qDebug() << "Réponse inattendue Modbus";
    return false;
}

// Écrire plusieurs registres (fonction 0x10)
bool ModbusCommunicator::writeMultipleRegisters(uint8_t unitId, uint16_t startAddress, const uint16_t* values, int count) {
    if (sock == INVALID_SOCKET) {
        qDebug() << "Socket invalide, pas connecté";
        return false;
    }

    if (count <= 0 || count > 123) { // Limite protocole Modbus (max 123 registres)
        qDebug() << "Nombre de registres invalide";
        return false;
    }

    // Longueur totale = 7 + 2*count bytes
    int length = 7 + 2 * count;
    int packetSize = 7 + length; // MBAP (7) + PDU(length)

    uint8_t* request = new uint8_t[packetSize];

    // Transaction ID (simple incrément fixe ou 0x0002)
    request[0] = 0x00; request[1] = 0x02;
    // Protocol ID
    request[2] = 0x00; request[3] = 0x00;
    // Length (PDU length + Unit ID)
    request[4] = (length >> 8) & 0xFF;
    request[5] = length & 0xFF;
    // Unit ID
    request[6] = unitId;
    // Fonction 0x10 (Write Multiple Registers)
    request[7] = 0x10;
    // Adresse départ
    request[8] = startAddress >> 8;
    request[9] = startAddress & 0xFF;
    // Nombre registres à écrire
    request[10] = count >> 8;
    request[11] = count & 0xFF;
    // Nombre d’octets de données
    request[12] = count * 2;

    // Valeurs à écrire
    for (int i = 0; i < count; ++i) {
        request[13 + i * 2] = values[i] >> 8;
        request[14 + i * 2] = values[i] & 0xFF;
    }

    int sentBytes = send(sock, (char*)request, packetSize, 0);
    delete[] request;

    if (sentBytes != packetSize) {
        qDebug() << "Erreur d'envoi de la requête Modbus";
        return false;
    }

    uint8_t response[256];
    int received = recv(sock, (char*)response, sizeof(response), 0);
    if (received < 12) {
        qDebug() << "Réponse Modbus trop courte ou erreur";
        return false;
    }

    if (response[7] == 0x10) { // Fonction écriture multiple OK
        return true;
    } else if ((response[7] & 0x80) != 0) { // Exception Modbus
        qDebug() << "Exception Modbus, code:" << response[8];
        return false;
    }

    qDebug() << "Réponse inattendue Modbus";
    return false;
}
