#pragma once
#include "glm/glm.hpp"

class DotRenderer;

const static float DotVelocity = 50.0f;

class Dot
{
private:



public:

	Dot(glm::vec2 startPosition, float radius);
	void Reset(glm::vec2& startPosition, float radius);
	void Update(float deltaTime);
	void Render(DotRenderer* renderer);
	void TakeDamage(int damage);

	glm::vec2 Position;
	glm::vec2 StartPos;
	glm::vec2 Velocity;

	float TotalTime = 0;
	float Radius = 0;
	int Health;

};

