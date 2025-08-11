#pragma once
#include "vector"

static const int SCREEN_WIDTH = 1800;
static const int SCREEN_HEIGHT = 900;

class DotRenderer;
class QuadTree;
class Dot;

class Game
{
public:
	QuadTree* TheTree;
	const int SearchWidth = 30;

	const int DotAmount = 5000;

	std::vector<Dot*> dots;

	float TotalTime = 0;

	Game(DotRenderer* aRenderer);
	void Update(float aDeltaTime);
	void CleanUp();
private:
	DotRenderer* renderer;
};

