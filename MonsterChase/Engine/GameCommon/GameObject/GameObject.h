#pragma once
#include "Core\Math\Point2D.h"

class GameObject
{
public:
	char* Name = nullptr;
	char* TempName = nullptr;
	char inputCharacter;
	int count = 0;
	explicit GameObject(const Engine::Math::Point2D & i_Position);
	~GameObject();
	void move(const Engine::Math::Point2D & i_Position);
	void getName();
	void print();
	void getPositions();
	void clean();

private:
	Engine::Math::Point2D position;
};



