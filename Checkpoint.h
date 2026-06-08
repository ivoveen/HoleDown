#pragma once
#include "GameObject.h"
class Player;
class Camera;
class ScoreManager;
namespace Audio {
	class Sound;
};

class Checkpoint : public GameObject
{
public:
	Checkpoint(float2 position, Player* player, Camera* camera, Surface* screen);
	~Checkpoint();
	void tick(float deltaTime);
	void CollisionDetection();
	void draw();
private:
	void Disappear();
	enum CheckpointState {
		innactive, active
	} mCheckpointState = innactive;

	const float mRADIUS = powf(20, 2);
	Sprite* mpCheckpointInnactive;
	Sprite* mpCheckpointActive;
	Audio::Sound* mpCheckpoint_S;

	float mSpriteFrame = 0;

};
