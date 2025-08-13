#include "Game.h"
#include "DotRenderer.h"
#include "Dot.h"
#include <cstdlib>
#include "glm/glm.hpp"
#include <algorithm>
#include <future>


#include "QuadTree.h"

Game::Game(DotRenderer* aRenderer)
{
	renderer = aRenderer;
	ThreadAmount = std::max(1u, std::thread::hardware_concurrency());


	for (size_t i = 0; i < DotAmount; i++)
	{
		Dot* d = new Dot({ std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT }, 1.5);

		dots.push_back(d);
	}

	glm::vec2 quadTreeCentre = glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	TheTree = new QuadTree(quadTreeCentre, SCREEN_WIDTH, SCREEN_HEIGHT);

}

void Game::Update(float deltaTime)
{
	TotalTime += deltaTime;

	//Rebuild the Quad Tree
	for (Dot* d : dots)
	{
		bool result = TheTree->Insert(d);
		int foo = 5;
	}
	//TheTree->DrawBoundries(renderer);

	std::vector<Dot*> toDestroy;

	for (Dot* d1 : dots)
	{
		AABB searchArea = AABB(d1->Position, SearchWidth, SearchWidth);
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

				if (dist <= minDist)
				{
					glm::vec2 normal = glm::normalize(d2->Position - d1->Position);

					d1->Velocity = glm::reflect(d1->Velocity, normal);
					d2->Velocity = glm::reflect(d2->Velocity, -normal);

					float overlap = 1.5f * ((minDist + 1) - dist);
					d1->Position -= normal * overlap;
					d2->Position += normal * overlap;
					d1->TakeDamage(1);
					d2->TakeDamage(1);
					d1->Radius += .5f;
					d2->Radius += .5f;
					if (d1->Health <= 0)
					{
						toDestroy.push_back(d1);
					}
					if (d2->Health <= 0)
					{
						toDestroy.push_back(d2);
					}
				}
			}
		}
	}

	for (Dot* d : toDestroy)
	{
		d->Reset({ std::rand() % SCREEN_WIDTH, std::rand() % SCREEN_HEIGHT }, 1);
	}

	toDestroy.clear();

	Render(deltaTime);
	

	
	TheTree->ClearTree();

}

void Game::Render(float deltaTime)
{
	renderer->ClearBuffer();

	std::vector<std::future<void>> futures;
	int dotsPerThread = dots.size() / ThreadAmount;

	for (int i = 0; i < ThreadAmount; i++)
	{
		int startIndex = i * dotsPerThread;
		int endIndex = (i == ThreadAmount - 1) ? dots.size() : (i + 1) * dotsPerThread;

		futures.push_back(std::async(std::launch::async, &Game::RenderPartition, this, startIndex, endIndex, deltaTime));
	}

	for (std::future<void>& f : futures)
	{
		f.get();
	}
	
	renderer->DrawPixelBuffer();
}

void Game::RenderPartition(int startIndex, int endIndex, float deltaTime)
{
	for (int i = startIndex; i < endIndex; i++)
	{
		if (dots[i] != nullptr)
		{
			dots[i]->Update(deltaTime);
			dots[i]->Render(renderer, TotalTime);
		}
	}

}

void Game::CleanUp()
{
	for (Dot* d : dots)
	{
		delete d;
	}
	delete TheTree;
}
