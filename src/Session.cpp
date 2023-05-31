#include "Session.h"

int Session::nextId = -1;

Session::Session(int c):
    id(++nextId), 
    createdAt(QDateTime::fromTime_t(time(NULL))),
    achievementScore(0), 
    challengeLevel(c),
    heartRateVariability(0),
    outputCoherenceChange(0),
    secondsPassed(0)
{
    for (int i = 0; i < 3; ++i){
        coherenceTime[i] = 0;
    }
}

void Session::increaseAchievement(int coherence) {
    achievementScore += coherence;
}

// adding coherence value to the correct vector within in coherenceTime 
// to reflect the percentage of time spent in each coherence level 
// when a new coherence level is reached the user is notified with a cout
void Session::addCoherenceScore(int coherence) {
    if (coherence < 5 && outputCoherenceChange != 1){
        ++coherenceTime[0];
        if (outputCoherenceChange != 1) {
            cout << "NEW COHERENCE LEVEL: Basic good beginner level coherence" << endl;
        }
        outputCoherenceChange = 1;
    } else if (coherence < 10 && outputCoherenceChange != 2){
        ++coherenceTime[1];
        if (outputCoherenceChange != 2) {
            cout << "NEW COHERENCE LEVEL: Good coherence" << endl;
        }
        outputCoherenceChange = 2;
    } else {
        ++coherenceTime[2];
        if (outputCoherenceChange != 3) {
            cout << "NEW COHERENCE LEVEL: Very good coherence" << endl;
        }
        outputCoherenceChange = 3;
    }
    coherenceScores.push_back(coherence);
    
}

int Session::getId() const {
    return id;
}

QDateTime Session::getCreatedAt() const {
    return createdAt;
}

int Session::getAchievementScore() const {
    return achievementScore;
}

vector<int>* Session::getCoherenceScores() {
    return &coherenceScores;
}

int Session::getChallengeLevel() const {
    return challengeLevel;
}

float Session::getAverageCoherence() const {
    return achievementScore/coherenceScores.size();
}

int Session::getLowTime() const {
    return coherenceTime[0];
}

int Session::getMedTime() const {
    return coherenceTime[1];
}

int Session::getHighTime() const {
    return coherenceTime[2];
}

int Session::getTotalTime() const {
    return coherenceScores.size();
}

int Session::getSecondsPassed() const {
    return secondsPassed;
}

void Session::setSecondsPassed(int s) {
    secondsPassed = s;
}