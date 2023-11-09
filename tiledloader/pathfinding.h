#pragma once
#include <vector>
#include <memory>
#include "level.hpp"

struct sNode
{
	bool bObstacle = false; // Is the node an obstruction?
	bool bVisited = false; // Have we searched this node before?
	float fGlobalGoal; // Distance to goal so far
	float fLocalGoal; // Distance to goal if we took the alternative route
	int x; // Nodes position in 2D space
	int y;
	std::vector<sNode*> vecNeighbours; // Connections to neighbours
	sNode* parent; // Node connecting to this node that offers shortest parent
};

class pathfinding
{
public:
	pathfinding(std::shared_ptr<level> map);
	void debugDraw(SDL_Renderer* ren, SDL_Rect& camera);
	bool Solve_AStar();
	void SetEnd(int x, int y) { nodeEnd = &nodes[y * nMapWidth + x]; };

private:
	sNode* nodes = nullptr;
	int nMapWidth = 0;
	int nMapHeight = 0;

	int tileSize = 0;

	sNode* nodeStart = nullptr;
	sNode* nodeEnd = nullptr;
};
