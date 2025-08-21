#pragma once
#include "vector"

static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;

class DotRenderer;
class QuadTree;
class Dot;

class Game
{
private:
	DotRenderer* _renderer;
	const float _dotRadius = 1.5f;

public:
	QuadTree* TheTree;
	const int SearchWidth = 10;
	const int DotAmount = 10000;

	int ThreadAmount = 4;

	std::vector<Dot*> dots;

	float TotalTime = 0;

	Game(DotRenderer* renderer);
	void Update(float deltaTime);
	void Render(float deltaTime);
	void RenderPartition(int startIndex, int endIndex, float deltaTime);

	void CleanUp();
};

