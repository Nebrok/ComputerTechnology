#pragma once
#include "vector"

static const int SCREEN_WIDTH = 1000;
static const int SCREEN_HEIGHT = 800;

class Dot;
class DotRenderer;

class Game
{
private:
	DotRenderer* _renderer;
	std::vector<Dot*> _dots;
	const int _dotAmount = 1000;

public:
	Game(DotRenderer* renderer);
	void Update(float deltaTime);
	void CleanUp();
};

