#pragma once
#include <vector>
#include "glm/glm.hpp"

class Dot;
class DotRenderer;

struct AABB
{
public:
	float Width;
	float Height;

	glm::vec2 Centre;
	glm::vec2 TopLeft = glm::vec2(-1, -1);
	glm::vec2 TopRight = glm::vec2(1, -1);
	glm::vec2 BottomLeft = glm::vec2(-1, 1);
	glm::vec2 BottomRight = glm::vec2(1, 1);

public:
	AABB();
	AABB(glm::vec2 centre, float width, float height);
	~AABB();

	glm::vec2 GetTopLeft();
	glm::vec2 GetTopRight();
	glm::vec2 GetBottomLeft();
	glm::vec2 GetBottomRight();


	bool Contains(glm::vec2 point);
	bool Intersects(AABB& other);
};

class QuadTree
{
public:
	AABB Boundry;

private:
	int _capacity = 16;
	int _level = 0;
	//not implemented yet
	int _depthLimit = 10;

	std::vector<Dot*> _items;

	QuadTree* _northWest = nullptr;
	QuadTree* _northEast = nullptr;
	QuadTree* _southWest = nullptr;
	QuadTree* _southEast = nullptr;

public:
	QuadTree(glm::vec2 centre, float width, float height, int level);
	~QuadTree();

	bool Insert(Dot* object);
	bool Subdivide();
	void ContainedBy(AABB& searchArea, std::vector<Dot*>& foundItems);

	void DrawBoundries(DotRenderer* dotRenderer);

	void ClearTree();

};

