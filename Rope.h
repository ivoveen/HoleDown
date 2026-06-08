#pragma once
#include "GameObject.h"
class Player;
class Rope : public GameObject
{
public:
	struct ropePoint {
		float2 position = { NULL,NULL };
		float2 prevPosition = { NULL,NULL };
		bool anchored = false;
	};
	Rope(float2 position, Player* player, Camera* camera, Surface* screen);
	~Rope();
	void tick(float deltaTime) override;
	void simulate(float deltaTime);
	void draw() override;
private:
	const float mGRAVITY = 0.005f;
	const float mINTERACTRANGE = 100;
	const int mMAXDIST = 25;
	const int mNUMITERATIONS = 30;
	const int mROPEPARTS = 15;
	bool boost = false;
	bool mMovingRight = false;
	float mMostRightPoint = NULL;
	float mSwingDistance = 30;
	float currentSwingDistance = mSwingDistance;

	ropePoint *mpRopePoints[30];
};

