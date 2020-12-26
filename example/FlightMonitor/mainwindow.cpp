#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include "qcustomplot.h"
#include <QApplication>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <QDebug>
#include <string>
#include <chrono>
#include <thread>

using namespace mavsdk;
using namespace std::chrono;
using namespace std::this_thread;

Mavsdk flightmonitor;
ConnectionResult connection_result;

float acc_x;
float acc_y;
float acc_z;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->acc_plot->addGraph();
    ui->acc_plot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionConnect_to_Vehicle_triggered()
{
    connectdialog * vehicleconnect = new connectdialog(this);
    int ret = vehicleconnect->exec();

    if (ret == QDialog::Accepted){
        QString url = vehicleconnect->getUrl();
        std::string url_std = url.toUtf8().constData();

        connection_result = flightmonitor.add_any_connection(url_std);

        sleep_for(seconds(2));

        if (connection_result != ConnectionResult::SUCCESS) {
            statusBar()->showMessage("Connection Failed!",5000);
        }else{
            System& system = flightmonitor.system();
            statusBar()->showMessage("Successfully connected to vehicle with UUID : " + QString::number(system.get_uuid()),5000);
            ui->actionConnect_to_Vehicle->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            setdatarates(50.0);
        }

    }else{
        statusBar()->showMessage("Connection Canceled.",5000);
    }

}

void MainWindow::on_actionDisconnect_triggered()
{

}

void MainWindow::setdatarates(double rate_hz)
{
    System& system = flightmonitor.system();
    auto telemetry = std::make_shared<Telemetry>(system);
    auto action = std::make_shared<Action>(system);

    const Telemetry::Result set_rate_result = telemetry->set_rate_imu_reading_ned(rate_hz);
    if (set_rate_result != Telemetry::Result::SUCCESS) {
        qDebug() << "Setting rate failed:" << Telemetry::result_str(set_rate_result);
    }

    telemetry->imu_reading_ned_async([](Telemetry::IMUReadingNED imu_reading) {
            acc_x=imu_reading.acceleration.north_m_s2;
            acc_y=imu_reading.acceleration.east_m_s2;
            acc_z=imu_reading.acceleration.down_m_s2;

        });

}
