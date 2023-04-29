#include "pathfinding.h"
#include "level.hpp"
#include "Framework.h"
#include <list>

pathfinding::pathfinding(std::shared_ptr<level> map)
{
	nMapWidth = map->GetLevelColumCnt();
	nMapHeight = map->GetLevelRowCnt();
	tileSize = map->GetTileWidth();

	auto navTiles= map->GetTilesByLayer(3);
	
// Create a 2D array of nodes - this is for convenience of rendering and construction
// and is not required for the algorithm to work - the nodes could be placed anywhere
// in any space, in multiple dimensions...
	nodes = new sNode[nMapWidth * nMapHeight];
	for (int x = 0; x < nMapWidth; x++)
		for (int y = 0; y < nMapHeight; y++)
		{

			nodes[y * nMapWidth + x].x = x; // ...because we give each node its own coordinates
			nodes[y * nMapWidth + x].y = y;
			nodes[y * nMapWidth + x].bObstacle = navTiles[y * nMapWidth + x].mObstacle;//x + (y * cols)
			nodes[y * nMapWidth + x].parent = nullptr;
			nodes[y * nMapWidth + x].bVisited = false;
		}

	// Create connections - in this case nodes are on a regular grid
	for (int x = 0; x < nMapWidth; x++)
		for (int y = 0; y < nMapHeight; y++)
		{
			if (y > 0)
				nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 0)]);
			if (y < nMapHeight - 1)
				nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 0)]);
			if (x > 0)
				nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x - 1)]);
			if (x < nMapWidth - 1)
				nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x + 1)]);

			// We can also connect diagonally
			if (y>0 && x>0)
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x - 1)]);
			if (y<nMapHeight-1 && x>0)
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x - 1)]);
			if (y>0 && x<nMapWidth-1)
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 1)]);
			if (y<nMapHeight - 1 && x<nMapWidth-1)
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 1)]);
			
		}

	// Manually positio the start and end markers so they are not nullptr
	nodeStart = &nodes[(nMapHeight / 2) * nMapWidth + (nMapWidth/3)+1];
	nodeEnd = &nodes[(nMapHeight / 2) * nMapWidth + nMapWidth - 2];
}

void pathfinding::debugDraw(SDL_Renderer* ren, SDL_Rect& camera)
{
	int nNodeSize = tileSize;
	int nNodeBorder = 5;

	// Draw Connections First - lines from this nodes position to its
	// connected neighbour node positions
	for (int x = 0; x < nMapWidth; x++)
		for (int y = 0; y < nMapHeight; y++)
		{
			for (auto n : nodes[y * nMapWidth + x].vecNeighbours)
			{
				SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0xFF, 0xFF);
				SDL_RenderDrawLine(ren, (x * nNodeSize + nNodeSize / 2) - camera.x, (y * nNodeSize + nNodeSize / 2) - camera.y, (n->x * nNodeSize + nNodeSize / 2) - camera.x, (n->y * nNodeSize + nNodeSize / 2) - camera.y);
			}
		}

	// Draw Nodes on top
	for (int x = 0; x < nMapWidth; x++)
		for (int y = 0; y < nMapHeight; y++)
		{
			SDL_Rect fillRect = { (x * nNodeSize + nNodeBorder) - camera.x, (y * nNodeSize + nNodeBorder) - camera.y, nNodeSize- nNodeBorder, nNodeSize- nNodeBorder };

			SDL_SetRenderDrawColor(ren, 0x00, 0xFF, 0x00, 0xFF);
			if (&nodes[y * nMapWidth + x] == nodeStart)
			{
				SDL_SetRenderDrawColor(ren, 0xFF, 0x7A, 0x00, 0xFF);
			}
			else if (&nodes[y * nMapWidth + x] == nodeEnd)
			{
				SDL_SetRenderDrawColor(ren, 0xE1, 0xE1, 0x00, 0xFF);
			}
			else if (nodes[y * nMapWidth + x].bVisited)
			{
				SDL_SetRenderDrawColor(ren, 0xBE, 0x00, 0xBE, 0xFF);
			}
			else if (nodes[y * nMapWidth + x].bObstacle)
			{
				SDL_SetRenderDrawColor(ren, 0xFF, 0x00, 0x00, 0xFF);
			} 
			SDL_RenderFillRect(ren, &fillRect);
		}


	// Draw Path by starting ath the end, and following the parent node trail
	// back to the start - the start node will not have a parent path to follow
	if (nodeEnd != nullptr)
	{
		sNode* p = nodeEnd;
		while (p->parent != nullptr)
		{
			SDL_SetRenderDrawColor(ren, 0xE1, 0xE1, 0x00, 0xFF);
			SDL_RenderDrawLine(ren, (p->x * nNodeSize + nNodeSize / 2) - camera.x, (p->y * nNodeSize + nNodeSize / 2) - camera.y, (p->parent->x * nNodeSize + nNodeSize / 2) - camera.x, (p->parent->y * nNodeSize + nNodeSize / 2) - camera.y);
			// Set next node to this node's parent
			p = p->parent;
		}
	}
}

bool pathfinding::Solve_AStar()
{
	// Reset Navigation Graph - default all node states
	for (int x = 0; x < nMapWidth; x++)
		for (int y = 0; y < nMapHeight; y++)
		{
			nodes[y * nMapWidth + x].bVisited = false;
			nodes[y * nMapWidth + x].fGlobalGoal = INFINITY;
			nodes[y * nMapWidth + x].fLocalGoal = INFINITY;
			nodes[y * nMapWidth + x].parent = nullptr;	// No parents
		}

	auto distance = [](sNode* a, sNode* b) // For convenience
	{
		return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
	};

	auto heuristic = [distance](sNode* a, sNode* b) // So we can experiment with heuristic
	{
		return distance(a, b);
	};

	// Setup starting conditions
	sNode* nodeCurrent = nodeStart;
	nodeStart->fLocalGoal = 0.0f;
	nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

	// Add start node to not tested list - this will ensure it gets tested.
	// As the algorithm progresses, newly discovered nodes get added to this
	// list, and will themselves be tested later
	std::list<sNode*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart);

	// if the not tested list contains nodes, there may be better paths
	// which have not yet been explored. However, we will also stop 
	// searching when we reach the target - there may well be better
	// paths but this one will do - it wont be the longest.
	while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd && !nodeEnd->bObstacle)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
	{
		// Sort Untested nodes by global goal, so lowest is first
		listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

		// Front of listNotTestedNodes is potentially the lowest distance node. Our
		// list may also contain nodes that have been visited, so ditch these...
		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
			listNotTestedNodes.pop_front();

		// ...or abort because there are no valid nodes left to test
		if (listNotTestedNodes.empty())
			break;

		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->bVisited = true; // We only explore a node once


		// Check each of this node's neighbours...
		for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
		{
			// ... and only if the neighbour is not visited and is 
			// not an obstacle, add it to NotTested List
			if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
				listNotTestedNodes.push_back(nodeNeighbour);

			// Calculate the neighbours potential lowest parent distance
			float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

			// If choosing to path through this node is a lower distance than what 
			// the neighbour currently has set, update the neighbour to use this node
			// as the path source, and set its distance scores as necessary
			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
			{
				nodeNeighbour->parent = nodeCurrent;
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

				// The best path length to the neighbour being tested has changed, so
				// update the neighbour's score. The heuristic is used to globally bias
				// the path algorithm, so it knows if its getting better or worse. At some
				// point the algo will realise this path is worse and abandon it, and then go
				// and search along the next best path.
				nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
			}
		}
	}

	return true;
}
