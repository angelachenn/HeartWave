#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <QDebug>
#include <QDateTime>

using namespace std;

class Session: public QObject {
    Q_OBJECT
	public:
		Session(int);

		void increaseAchievement(int);
		void addCoherenceScore(int);

		// getters 
		int getId() const;
        QDateTime getCreatedAt() const;
		int getAchievementScore() const;
		vector<int>* getCoherenceScores();
		int getChallengeLevel() const;
		float getAverageCoherence() const;
		int getLowTime() const;
		int getMedTime() const;
		int getHighTime() const;
		int getTotalTime() const;
        int getSecondsPassed() const;
        void setSecondsPassed(int);

	private:
		static int nextId;
		int id;
        QDateTime createdAt;
		int achievementScore;
		int challengeLevel;
		int heartRateVariability;
		int outputCoherenceChange;
        int secondsPassed;
		
		vector<int> coherenceScores;
		int coherenceTime[3];
};

#endif
