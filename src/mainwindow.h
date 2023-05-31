#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include "Device.h"
#include "Display.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    public slots:
        void setFrameVisible(QFrame* frame);
        void setButtonPressed(QPushButton* button);

    private:
        Ui::MainWindow *ui;
        Device* device;

};

#endif // MAINWINDOW_H
