#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebSocket>
#include <QMessageBox>
#include <QQueue>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWebSocket client;
    bool isConnected = false;
    QQueue<QString> messages;
    QString nickname;
    void updateTextBox();
    void connectToServer();
    void disconnectFromServer();

private Q_SLOTS:
    void sendButtonClicked();
    void connectButtonClicked();
    void onConnected();
    void processTextMessage(QString message);
};
#endif // MAINWINDOW_H
