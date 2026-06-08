#pragma once
class ScoreManager
{
public:
	ScoreManager(Surface* screen);
	void UpdateScore(int addedScore);
	void Draw();
	int getScore() {
		return totalScore;
	}

private:
	Surface* mpScreen;
	int totalScore = 0;
	int mAddedScore = 0;
	float2 mPosition = {0,0};
	const int mNUMBERWIDTH = 64;
	const int mNUMBERHEIGHT = 128;

};

