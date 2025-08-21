#include "Game.h"
#include "DotRenderer.h"
#include "Dot.h"
#include <cstdlib>
#include "glm/glm.hpp"
#include <algorithm>


Game::Game(DotRenderer* renderer)
{
	_renderer = renderer;

	for (size_t i = 0; i < _dotAmount; i++)
	{
		Dot* d = new Dot({ std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT }, 3);

		_dots.push_back(d);
	}
}

void Game::Update(float deltaTime)
{
	std::vector<Dot*> toDestroy;
	for (Dot* d1 : _dots)
	{
		for (Dot* d2 : _dots)
		{
			if (d1 != d2 && d1 != nullptr && d2 != nullptr)
			{
				float dist = glm::distance(d1->Position, d2->Position);
				float minDist = d1->Radius + d2->Radius;

				if (dist < minDist)
				{
					glm::vec2 normal = glm::normalize(d2->Position - d1->Position);

					d1->Velocity = glm::reflect(d1->Velocity, normal);
					d2->Velocity = glm::reflect(d2->Velocity, -normal);

					float overlap1 = 1.5f * ((minDist + 1) - dist);
					float overlap2 = 1.5f * (minDist - dist);
					d1->Position -= normal * overlap1;
					d2->Position += normal * overlap2;
					d1->TakeDamage(1);
					d1->Radius++;
				}
				if (d1->Health <= 0)
				{
					toDestroy.push_back(d1);
				}
			}
		}
	}

	for (Dot* d : toDestroy)
	{
		glm::vec2 newPos = { std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT };
		d->Reset(newPos, 3);
	}

	for (Dot* d : _dots)
	{
		d->Update(deltaTime);
		d->Render(_renderer);
	}
}

void Game::CleanUp()
{

}
