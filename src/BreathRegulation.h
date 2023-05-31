#ifndef BREATHREGULATION_H
#define BREATHREGULATION_H

#include <QProgressBar>
#include <QTimer>
#include <QSpinBox>
#include <iostream>
#include <QTimer>

using namespace std;

class BreathRegulation : public QWidget{
    Q_OBJECT
    public:
        BreathRegulation(QProgressBar* widget);
        ~BreathRegulation();

        void setBreathRegulation(bool);

    private slots:
        void changeBreathLevel(int newLevel);
        void toggleVisibleBreath();
        void updateDisplayBar();

    private:
        int breathLevel;
        QProgressBar* progressBar;
        QTimer* timer;
        bool usingBreath;
        int count;
        int breathLevelChange;
};

#endif
