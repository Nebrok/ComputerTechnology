#pragma once
#include <vector>
#include "glm/glm.hpp"

class Dot;


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

	bool Contains(glm::vec2 point);
	bool Intersects(AABB other);
};


class QuadTree
{
public:
	AABB* Boundry;

private:
	int _capacity = 4;
	int _level = 0;

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
	void ContainedBy(AABB searhArea, std::vector<Dot*>& foundItems);


	void ClearTree();

};

