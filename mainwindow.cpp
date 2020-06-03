#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendButtonClicked);
    connect(ui->messageEdit, &QLineEdit::returnPressed, this, &MainWindow::sendButtonClicked);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectButtonClicked);

    connect(&client, &QWebSocket::connected, this, &MainWindow::onConnected);
    connect(&client, &QWebSocket::textMessageReceived, this, &MainWindow::processTextMessage);

    for(int i = 0; i<10; i++)
        messages.push_back("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTextBox()
{
    /**
    * @brief Обновление текста на форме
    */
    QString text;
    for(int i = 0; i < 10; i++)
        text += messages[i] + "\n";
    ui->textEdit->setText(text);
}

void MainWindow::sendButtonClicked()
{
    /**
    * @brief Обработчик нажатия кнопки отправки сообщения
    */
    if(!client.sendTextMessage(nickname + ": " + ui->messageEdit->text()))
    {
        disconnectFromServer();
        QMessageBox::critical(0, "Error", "Lost connection");
    }
    ui->messageEdit->clear();
}

void MainWindow::connectButtonClicked()
{
    /**
    * @brief Обработчик нажатия кнопки подключения
    */
    if(!isConnected)
        connectToServer();
    else
        disconnectFromServer();
}

void MainWindow::connectToServer()
{
    /**
    * @brief Попытка подключения к серверу
    */
    client.open(QUrl(ui->urlEdit->text()));
}

void MainWindow::disconnectFromServer()
{
    /**
    * @brief Отключение от сервера
    */
    client.sendTextMessage("disconnect");

    client.close(QWebSocketProtocol::CloseCodeNormal, "");

    ui->connectButton->setText("Connect");
    isConnected = false;
    ui->urlEdit->setEnabled(true);
    ui->sendButton->setEnabled(false);
    ui->nicknameEdit->setEnabled(true);
    ui->statusLabel->setText("Status: disconnected");
}

void MainWindow::onConnected()
{
    /**
    * @brief Обработчик успешного подключения к серверу
    */
    nickname = ui->nicknameEdit->text();
    ui->connectButton->setText("Disconnect");
    isConnected = true;
    ui->urlEdit->setEnabled(false);
    ui->sendButton->setEnabled(true);
    ui->nicknameEdit->setEnabled(false);
    ui->statusLabel->setText("Status: connected");

    client.sendTextMessage("New user - " + nickname);
}

void MainWindow::processTextMessage(QString message)
{
    /**
    * @brief Обрабатывает полученное сообщение
    * @param message Сообщение
    */
    messages.pop_front();
    messages.push_back(message);
    updateTextBox();
}
