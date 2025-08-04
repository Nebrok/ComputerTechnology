#pragma once
#include "glm/glm.hpp"

class DotRenderer;

class Dot
{
public:

	Dot(glm::vec2 aPosition, float aRadius);
	void Render(DotRenderer* aRenderer, float dt);
	void TakeDamage(int someDamage);
	void Reset(glm::vec2 newPosition, float newRadius);

	glm::vec2 Position;
	glm::vec2 StartPos;
	glm::vec2 Velocity;

	float TotalTime = 0;
	float Radius = 0;

	int Health;

private:
	bool _overriden = false;
};

