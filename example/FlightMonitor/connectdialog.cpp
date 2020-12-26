#include "connectdialog.h"
#include "ui_connectdialog.h"
#include <QComboBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStackedWidget>
#include <QString>
#include <QDebug>


connectdialog::connectdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connectdialog)
{
    ui->setupUi(this);

    //Set fixed size
    this->setFixedSize(270,180);

    //Setup connnection type combobox
    ui->typecomboBox->addItem("serial");
    ui->typecomboBox->addItem("tcp");
    ui->typecomboBox->addItem("udp");

    //setup baudrate combobox
    ui->baudratecomboBox->addItem("9600");
    ui->baudratecomboBox->addItem("19200");
    ui->baudratecomboBox->addItem("38400");
    ui->baudratecomboBox->addItem("57600");
    ui->baudratecomboBox->addItem("115200");
    ui->baudratecomboBox->addItem("230400");
    ui->baudratecomboBox->setCurrentIndex(4);

    //Setup available port combobox
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        ui->portcomboBox->addItem(info.portName());

    //Change between serial port and IP when connection type is changed
    connect(ui->typecomboBox,&QComboBox::currentTextChanged,[=](){
        if (ui->typecomboBox->currentIndex()==0){
            ui->stackedWidget->setCurrentIndex(0);
            ui->baudratecomboBox->setEnabled(true);
        }else{
            ui->stackedWidget->setCurrentIndex(1);
            ui->baudratecomboBox->setEnabled(false);
        }
    });


}

connectdialog::~connectdialog()
{
    delete ui;
}


void connectdialog::on_ConnectButton_clicked()
{
    if (ui->typecomboBox->currentIndex()==0){
        url = ui->typecomboBox->currentText() + ":///dev/" + ui->portcomboBox->currentText() + ":" + ui->baudratecomboBox->currentText();
    }else{
        url = ui->typecomboBox->currentText() + "://" + ui->IPlineEdit->text();
    }

    accept();

}

QString connectdialog::getUrl() const
{
    return url;
}
