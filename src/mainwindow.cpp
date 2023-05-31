#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): 
    QMainWindow(parent), 
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sessionFrame->setVisible(false);
    ui->menuFrame->setVisible(false);
    ui->settingsFrame->setVisible(false);
    ui->historyFrame->setVisible(false);
    ui->individualHistoryFrame->setVisible(false);
    ui->batteryAlert->setVisible(false);
    ui->connectAlert->setVisible(false);
    device = new Device(this);

    //connections for buttons on the device that show up in the UI
    connect(ui->power, SIGNAL(released()), device->getDCS(), SLOT (power()));
    connect(ui->up, SIGNAL(released()), device->getDCS(), SLOT (up()));
    connect(ui->down, SIGNAL(released()), device->getDCS(), SLOT (down()));
    connect(ui->left, SIGNAL(released()), device->getDCS(), SLOT (left()));
    connect(ui->right, SIGNAL(released()), device->getDCS(), SLOT (right()));
    connect(ui->select, SIGNAL(released()), device->getDCS(), SLOT (select()));
    connect(ui->back, SIGNAL(released()), device->getDCS()->getDisplay(), SLOT (back()));
    connect(ui->menu, SIGNAL(released()), device->getDCS(), SLOT (menu()));

    //connections for other device elements, such as Battery, BreathRegulation, etc.
    connect(device->getDCS()->getDisplay(), SIGNAL(toggleBattery(bool)), device->getBattery(), SLOT(toggleVisible(bool)));
    connect(device->getBattery(), SIGNAL(batteryAlert()), device->getDCS(), SLOT(displayBatteryAlert()));
    connect(device->getBattery(), SIGNAL(kill()), device->getDCS(), SLOT(kill()));
    connect(ui->reset, SIGNAL(released()), device->getDCS(), SLOT (reset()));
    connect(ui->checkBoxSetBreath, SIGNAL(stateChanged(int)), device->getDCS(), SLOT (setBreath()));
    connect(ui->breathLevel, SIGNAL(valueChanged(int)), device->getDCS(), SLOT (setInterval(int)));
    connect(device->getDCS()->getDisplay(), SIGNAL(toggleFrame(QFrame*)), this, SLOT(setFrameVisible(QFrame*)));
    connect(device->getDCS()->getDisplay(), SIGNAL(toggleButtonHighlight(QPushButton*)), this, SLOT(setButtonPressed(QPushButton*)));

    //connections for test/simulation buttons
    connect(ui->testLowCoherence, SIGNAL(released()), device->getDCS(), SLOT(lowCoherence()));
    connect(ui->testMedCoherence, SIGNAL(released()), device->getDCS(), SLOT(medCoherence()));
    connect(ui->testHighCoherence, SIGNAL(released()), device->getDCS(), SLOT(highCoherence()));
    connect(ui->testBatteryOut, SIGNAL(released()), device->getBattery(), SLOT(killBattery()));
    connect(ui->testRecharge, SIGNAL(released()), device->getBattery(), SLOT(recharge()));
    connect(ui->testDisconnect, SIGNAL(released()), device->getDCS(), SLOT(disconnectHR()));
    connect(ui->testReconnect, SIGNAL(released()), device->getDCS(), SLOT(reconnectHR()));

}   

MainWindow::~MainWindow() {
    delete ui;
    delete device;
}

void MainWindow::setFrameVisible(QFrame* frame) {
    if (frame != nullptr) {
        frame->setVisible(!frame->isVisible());
    }
}

void MainWindow::setButtonPressed(QPushButton* button) {
    if (button != nullptr) {
        button->setDown(!button->isDown());
    }
}
