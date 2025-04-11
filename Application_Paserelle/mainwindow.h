#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonArray>
#include <QSqlDatabase>
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QJsonObject>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void loadJson();
    //void saveJson();
    void AppliquerModification();
    void modifyGatewayButtonClicked();
    void acquisitionModelButtonClicked();
    void measureModelButtonClicked();
    void returnButtonClicked();
    void connectToDatabase();
    void returnButtonClicked2();
    void returnButtonClicked3();
    bool saveJson();
    void sendConfigToGateway(const QJsonObject &config);

private:
    void setupUi();
    void updateValue(QJsonArray &array, int id, const QString &newValue);

    Ui::MainWindow *ui;
    QString filePathPaserelle;
    QString filePathModele;
    QString filePathMesure;
    QJsonArray jsonArrayPaserelle;
    QJsonArray jsonArrayModele;
    QJsonArray jsonArrayMesure;
    QSqlDatabase db;
    QNetworkAccessManager *networkManager;
};

#endif // MAINWINDOW_H
