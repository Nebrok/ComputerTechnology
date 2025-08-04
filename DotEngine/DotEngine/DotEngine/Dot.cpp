#include "Dot.h"
#include "DotRenderer.h"
#include "Game.h"
#include <random>
#include <glm/gtc/constants.hpp>

const float DotVelocity = 50.0f;

Dot::Dot(glm::vec2 newPosition, float newRadius)
{
	Reset(newPosition, newRadius);
}

void Dot::Reset(glm::vec2 newPosition, float newRadius)
{
	Position = newPosition;
	StartPos = newPosition;
	Radius = newRadius;

	static std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
	std::uniform_real_distribution<float> dist(-100.0f, 100.0f);

	Velocity = glm::vec2(dist(rng), dist(rng));

	float angle = dist(rng) * glm::pi<float>() / 100.0f;
	Velocity = glm::vec2(cos(angle), sin(angle));

	Health = 3;
}


void Dot::Render(DotRenderer* aRenderer, float dt)
{
	TotalTime += dt;

	Position += Velocity * DotVelocity * dt;

	
	float redColor = (glm::cos((TotalTime + StartPos.x) * 0.1f) * 0.5f + 0.5f) * 255.0f;
	float greenColor = (glm::cos((TotalTime + StartPos.y) * 0.9f) * 0.5f + 0.5f) * 255.0f;
	float blueColor = (glm::cos(TotalTime * 0.4f) * 0.5f + 0.5f) * 255.0f;

	aRenderer->SetDrawColor(redColor, greenColor, blueColor, 255);
	aRenderer->DrawFilledCircle(Position.x, Position.y, Radius);

	if (Position.x - Radius < 0.0f)
	{
		Position.x = Radius;
		Velocity.x *= -1;
	}
	else if (Position.x + Radius > SCREEN_WIDTH)
	{
		Position.x = SCREEN_WIDTH - Radius;
		Velocity.x *= -1;
	}

	if (Position.y - Radius < 0.0f)
	{
		Position.y = Radius;
		Velocity.y *= -1;
	}
	else if (Position.y + Radius > SCREEN_HEIGHT)
	{
		Position.y = SCREEN_HEIGHT - Radius;
		Velocity.y *= -1;
	}
}

void Dot::TakeDamage(int someDamage)
{
	Health -= someDamage;
}





