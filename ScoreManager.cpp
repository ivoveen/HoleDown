#include "precomp.h"
#include "ScoreManager.h"
//#include <iostream>

ScoreManager::ScoreManager(Surface* screen) {
	mpScreen = screen;
}

void ScoreManager::UpdateScore(int addedScore) {
	mAddedScore = addedScore;
}

void ScoreManager::Draw() {
	if (mAddedScore > 0)totalScore+=3, mAddedScore-=3;
	if (mAddedScore < -10)totalScore -= 3, mAddedScore += 3;

	if (mAddedScore < 0 && mAddedScore > -10) totalScore += mAddedScore, mAddedScore =0;
	std::string str = std::to_string(totalScore);
	const char* charStr = str.c_str();
	mpScreen->Print("Score:", 10, 10, 0xFFFFFF, 8);
	mpScreen->Print(charStr, 300, 10, 0xFFFFFF, 8);

}