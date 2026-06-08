#pragma once
#include"TileMap.h"
class Player;
class Camera;

class GameObject
{
public:
	GameObject(float2 position, Player* player, Camera* camera, Surface* screen) {
		mPosition = position;
		mpPlayer = player;
		mpCamera = camera;
		mpScreen = screen;
	};
	virtual ~GameObject() = default;
	float2 getCentrePos();
	virtual void tick(float deltaTime);
	virtual void draw();


protected:
	float2 mPosition{};
	Player* mpPlayer{};
	Camera* mpCamera{};
	Surface* mpScreen{};
	int mWidth = 50, mHeight = 50;
	float mDeltaTime;

};

