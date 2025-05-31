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
    void AppliquerModification();
    void modifyGatewayButtonClicked();
    void acquisitionModelButtonClicked();
    void measureModelButtonClicked();
    void returnButtonClicked();
    void connectToDatabase();
    void returnButtonClicked2();
    void returnButtonClicked3();
    bool save();
    void ModifieModbus();

    void on_OFFButton_clicked();

private:
    void setupUi();
    void updateValue(QJsonArray &array, int id, const QString &newValue);
    void modifierRegistreModbus(const QString& adresseIp, int port, quint16 adresseRegistre, const QString& data);

    Ui::MainWindow *ui;
    QSqlDatabase db;
    QNetworkAccessManager *networkManager;
};

#endif // MAINWINDOW_H
