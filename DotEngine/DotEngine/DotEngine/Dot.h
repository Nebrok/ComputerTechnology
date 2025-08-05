#pragma once
#include "glm/glm.hpp"

class DotRenderer;

class Dot
{
public:
	glm::vec2 Position;
	glm::vec2 Velocity;
	float Radius = 0;
	int Health;

public:
	Dot(glm::vec2 aPosition, float aRadius);

	void Update(float dt);
	void Render(DotRenderer* aRenderer, float& totalTime);
	void TakeDamage(int someDamage);
	void Reset(glm::vec2 newPosition, float newRadius);


};

