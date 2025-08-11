#include "QuadTree.h"
#include "Dot.h"
#include "DotRenderer.h"

//-------------AABB-----------------

AABB::AABB()
	: Centre({0,0}), Width(0), Height(0), HalfWidth(0), HalfHeight(0)
{
}

AABB::AABB(glm::vec2 centre, float width, float height)
	: Centre(centre), Width(width), Height(height), HalfWidth(Width / 2), HalfHeight(Height / 2)
{
	//TopLeft = glm::vec2(Centre.x - (Width / 2), Centre.y - (Height / 2));
	//TopRight = glm::vec2(Centre.x + (Width / 2), Centre.y - (Height / 2));
	//BottomLeft = glm::vec2(Centre.x - (Width/2), Centre.y + (Height / 2));
	//BottomRight = glm::vec2(Centre.x + (Width / 2), Centre.y + (Height / 2));
}

AABB::~AABB()
{
}

glm::vec2 AABB::GetTopLeft()
{
	return glm::vec2(Centre.x - (Width / 2), Centre.y - (Height / 2));
}

glm::vec2 AABB::GetTopRight()
{
	return glm::vec2(Centre.x + (Width / 2), Centre.y - (Height / 2));
}

glm::vec2 AABB::GetBottomLeft()
{
	return glm::vec2(Centre.x - (Width / 2), Centre.y + (Height / 2));
}

glm::vec2 AABB::GetBottomRight()
{
	return glm::vec2(Centre.x + (Width / 2), Centre.y + (Height / 2));
}

bool AABB::Contains(glm::vec2 point)
{
	return (
		point.x > (Centre.x - HalfWidth) and
		point.x < (Centre.x + HalfWidth) and
		point.y < (Centre.y + HalfHeight) and
		point.y > (Centre.y - HalfHeight)
		);


	/*
	return (
		point.x > TopLeft.x and
		point.x < TopRight.x and
		point.y < BottomRight.y and
		point.y > TopRight.y
		);
		*/
}

bool AABB::Intersects(AABB& other)
{

	float dx = other.Centre.x - Centre.x;
	float px = (other.HalfWidth) + HalfWidth - abs(dx);
	if (px <= 0)
		return false;

	float dy = other.Centre.y - Centre.y;
	float py = (other.HalfHeight) + HalfHeight - abs(dy);
	if (py <= 0)
		return false;

	return true;

	/*
	return (
		other.TopLeft.x < TopRight.x and
		other.TopRight.x > TopLeft.x and
		other.TopLeft.y < BottomRight.y and
		other.BottomRight.y > TopLeft.y);
		*/
}


//-------------QuadTree-------------

QuadTree::QuadTree(glm::vec2 centre, float width, float height)
{
	Boundry = AABB(centre, width, height);
}

QuadTree::~QuadTree()
{
	DestroyTree();
}

bool QuadTree::Insert(Dot* object)
{
	if (not Boundry.Contains(object->Position))
	{
		return false;
	}

	if (_hitLimit or (_items.size() < _capacity and !_hasSplit))
	{
		_items.push_back(object);
		return true;
	}

	if (!_hasSplit)
	{
		if (!Subdivide())
			return false;
	}

	if (_northWest->Insert(object))
		return true;
	if (_northEast->Insert(object))
		return true;
	if (_southWest->Insert(object))
		return true;
	if (_southEast->Insert(object))
		return true;

	//Shouldn't ever get here but big problem if it does
	return false;
}

bool QuadTree::Subdivide()
{
	if (_northWest == nullptr)
	{
		float newWidth = Boundry.Width / 2;
		float newHeight = Boundry.Height / 2;

		glm::vec2 centreTopLeft = glm::vec2(Boundry.Centre.x - (newWidth / 2), Boundry.Centre.y - (newHeight / 2));
		glm::vec2 centreTopRight = glm::vec2(Boundry.Centre.x + (newWidth / 2), Boundry.Centre.y - (newHeight / 2));
		glm::vec2 centreBottomLeft = glm::vec2(Boundry.Centre.x - (newWidth / 2), Boundry.Centre.y + (newHeight / 2));
		glm::vec2 centreBottomRight = glm::vec2(Boundry.Centre.x + (newWidth / 2), Boundry.Centre.y + (newHeight / 2));

		_northWest = new QuadTree(centreTopLeft, newWidth, newHeight);
		_northEast = new QuadTree(centreTopRight, newWidth, newHeight);
		_southWest = new QuadTree(centreBottomLeft, newWidth, newHeight);
		_southEast = new QuadTree(centreBottomRight, newWidth, newHeight);

		if ((newWidth / 2) < 2 or (newHeight / 2) < 2)
		{
			_northWest->SetLimit();
			_northEast->SetLimit();
			_southWest->SetLimit();
			_southEast->SetLimit();
		}
	}

	for (Dot* d : _items)
	{
		if (_northWest->Insert(d))
			continue;
		if (_northEast->Insert(d))
			continue;
		if (_southWest->Insert(d))
			continue;
		if (_southEast->Insert(d))
			continue;
	}

	_hasSplit = true;
	return true;
}

void QuadTree::ContainedBy(AABB& searchArea, std::vector<Dot*>& foundItems)
{
	if (!Boundry.Intersects(searchArea))
		return;

	for (Dot* d : _items)
	{
		if (searchArea.Contains(d->Position))
			foundItems.push_back(d);
	}

	if (!_hasSplit)
		return;

	_northWest->ContainedBy(searchArea, foundItems);
	_northEast->ContainedBy(searchArea, foundItems);
	_southWest->ContainedBy(searchArea, foundItems);
	_southEast->ContainedBy(searchArea, foundItems);

	return;
}

void QuadTree::DrawBoundries(DotRenderer* dotRenderer)
{
	dotRenderer->SetDrawColor(255, 255, 255, 255);
	
	if (_hasSplit)
	{
		dotRenderer->DrawLine(Boundry.Centre.x, Boundry.Centre.y - (Boundry.HalfHeight), Boundry.Centre.x, Boundry.Centre.y + (Boundry.HalfHeight));
		dotRenderer->DrawLine(Boundry.Centre.x - (Boundry.HalfWidth), Boundry.Centre.y, Boundry.Centre.x + (Boundry.HalfWidth), Boundry.Centre.y);
		_northWest->DrawBoundries(dotRenderer);
		_northEast->DrawBoundries(dotRenderer);
		_southWest->DrawBoundries(dotRenderer);
		_southEast->DrawBoundries(dotRenderer);
	}
}

void QuadTree::ClearTree()
{
	if (_hasSplit)
	{
		_northWest->ClearTree();
		_northEast->ClearTree();
		_southWest->ClearTree();
		_southEast->ClearTree();
	}

	_hasSplit = false;
	_items.clear();
}

void QuadTree::DestroyTree()
{
	if (_northWest != nullptr)
	{
		_northWest->~QuadTree();
		delete _northWest;
		_northWest = nullptr;
	}

	if (_northEast != nullptr)
	{
		_northEast->~QuadTree();
		delete _northEast;
		_northEast = nullptr;
	}

	if (_southWest != nullptr)
	{
		_southWest->~QuadTree();
		delete _southWest;
		_southWest = nullptr;
	}

	if (_southEast != nullptr)
	{
		_southEast->~QuadTree();
		delete _southEast;
		_southEast = nullptr;
	}

	_items.clear();
}

