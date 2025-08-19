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
	void Render(DotRenderer* renderer, float& totalTime);
	void TakeDamage(const int damage);
	void Reset(glm::vec2& newPosition, float radius);


};

