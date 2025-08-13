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

	const int DotAmount = 10000;

	int ThreadAmount = 4;

	std::vector<Dot*> dots;

	float TotalTime = 0;

	Game(DotRenderer* renderer);
	void Update(float deltaTime);
	void Render(float deltaTime);
	void RenderPartition(int startIndex, int endIndex, float deltaTime);

	void CleanUp();
private:
	DotRenderer* renderer;
};

