#include "pathfinding.h"
#include "level.hpp"
#include "Framework.h"

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
	nodeStart = &nodes[(nMapHeight / 2) * nMapWidth + 1];
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
				//SDL_Rect dest;
				//dest.x = x;
				//dest.y = y;
				//dest.w = src.w;
				//dest.h = src.h;

				//if (g_pFramework->checkCollision(camera, dest))
				//{
				//	//Show the tile
				//	render(dest.x - camera.x, dest.y - camera.y, &src);
				//}
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
			if(nodes[y * nMapWidth + x].bObstacle)
				SDL_SetRenderDrawColor(ren, 0xFF, 0x00, 0x00, 0xFF);
			SDL_RenderFillRect(ren, &fillRect);

			//Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder,
			//	(x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder,
			//	PIXEL_HALF, nodes[y * nMapWidth + x].bObstacle ? FG_WHITE : FG_BLUE);

			//if (nodes[y * nMapWidth + x].bVisited)
			//	Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder, (x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder, PIXEL_SOLID, FG_BLUE);

			//if (&nodes[y * nMapWidth + x] == nodeStart)
			//	Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder, (x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder, PIXEL_SOLID, FG_GREEN);

			//if (&nodes[y * nMapWidth + x] == nodeEnd)
			//	Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder, (x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder, PIXEL_SOLID, FG_RED);

		}


	// Draw Path by starting ath the end, and following the parent node trail
	// back to the start - the start node will not have a parent path to follow
	//if (nodeEnd != nullptr)
	//{
	//	sNode* p = nodeEnd;
	//	while (p->parent != nullptr)
	//	{
	//		DrawLine(p->x * nNodeSize + nNodeSize / 2, p->y * nNodeSize + nNodeSize / 2,
	//			p->parent->x * nNodeSize + nNodeSize / 2, p->parent->y * nNodeSize + nNodeSize / 2, PIXEL_SOLID, FG_YELLOW);

	//		// Set next node to this node's parent
	//		p = p->parent;
	//	}
	//}
}
