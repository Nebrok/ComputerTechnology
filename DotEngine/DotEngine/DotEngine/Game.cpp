#include "Game.h"
#include "DotRenderer.h"
#include "Dot.h"
#include <cstdlib>
#include "glm/glm.hpp"
#include <algorithm>

#include "QuadTree.h"

std::vector<Dot*> dots;

const int DotAmount = 1000;

Game::Game(DotRenderer* aRenderer)
{
	renderer = aRenderer;

	for (size_t i = 0; i < DotAmount; i++)
	{
		int diry = std::rand() % 2;
		int dirx = std::rand() % 2;

		dirx = -1 ? dirx > 1 : dirx;
		diry = -1 ? diry > 1 : diry;

		Dot* d = new Dot({ std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT }, 3);

		dots.push_back(d);
	}

	glm::vec2 quadTreeCentre = glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	TheTree = new QuadTree(quadTreeCentre, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

}

void Game::Update(float aDeltaTime)
{
	//Rebuild the Quad Tree
	for (Dot* d : dots)
	{
		bool result = TheTree->Insert(d);
		int foo = 5;
	}

	std::vector<Dot*> toDestroy;

	for (Dot* d1 : dots)
	{
		AABB searchArea = AABB(d1->Position, 100, 100);
		std::vector<Dot*> surrounding;
		TheTree->ContainedBy(searchArea, surrounding);
		if (surrounding.size() == 0)
			continue;

		for (Dot* d2 : surrounding)
		{
			if (d1 != d2)// && d1 != nullptr && d2 != nullptr)
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
					//d2->TakeDamage(1);
					d1->Radius++;
					//d2->Radius++;
				}
				if (d1->Health <= 0)
				{
					toDestroy.push_back(d1);
				}
			}
		}

	}


	/*
	std::vector<Dot*> toDestroy;
	for (Dot* d1 : dots)
	{
		for (Dot* d2 : dots)
		{
			if (d1 != d2)// && d1 != nullptr && d2 != nullptr)
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
					//d2->TakeDamage(1);
					d1->Radius++;
					//d2->Radius++;
				}
				if (d1->Health <= 0)
				{
					toDestroy.push_back(d1);
				}
			}
		}
	}
	*/

	for (Dot* d : toDestroy)
	{
		d->Reset({ std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT }, 3);
	}

	toDestroy.clear();

	for (Dot* d : dots)
	{
		if (d != nullptr)
		{
			d->Render(renderer, aDeltaTime);
		}
	}

	TheTree->ClearTree();

}

void Game::CleanUp()
{

}
