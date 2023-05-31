#include "BreathRegulation.h"

BreathRegulation::BreathRegulation(QProgressBar* widget): 
    breathLevel(10),
    progressBar(widget), 
    timer(new QTimer(this)),
    usingBreath(false),
    count(0),
    breathLevelChange(0)
{
    progressBar->setVisible(false);
    progressBar->setValue(60);
    progressBar->setRange(1, 60);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateDisplayBar()));
}

BreathRegulation::~BreathRegulation(){
    delete timer;
};

// setter for the using breathVariable which then indicates 
// if the progress bar is visible 
void BreathRegulation::setBreathRegulation(bool breathRegulation) {
    usingBreath = breathRegulation;
}

// slot for setting variables needed to change the breath level 
// within BreathRegulation
void BreathRegulation::changeBreathLevel(int newLevel) {
    breathLevel = newLevel;
    breathLevelChange = breathLevel;
    count = 0;
}

// slot for showcasing the progressBar as indicated in settings
void BreathRegulation::toggleVisibleBreath() {
    count = 0;

    if (usingBreath) {
        progressBar->setVisible(true);
        progressBar->setValue(60);

        breathLevelChange = breathLevel;
        timer->start(1000);
    } else {
        progressBar->setVisible(false);
        timer->stop();
    }
}

// slot for updating the progress bar based on the timeout signal
// progress bar updates the UI every second 
// the algorithm for having the progress bar move based on the breath level is:
// 60 - ((60/(breathLevel)) * count
void BreathRegulation::updateDisplayBar(){
    if (breathLevelChange == 0) {
        breathLevelChange = breathLevel;
        count = 0;
    }
    --breathLevelChange;

    progressBar->setValue(60 - ((60/(breathLevel)) * count));
    ++count;
}