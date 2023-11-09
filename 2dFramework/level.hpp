#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <map>
#include <tmxlite/Tileset.hpp>

using gid = int;

struct frame
{
	int x;
	int y;
	int duration;
};

// Stores information about an individual tile to be
// displayed.
struct tile
{
	SDL_Texture* sheet;
	// x coordinate in the world
	int x;
	// y coordinate in the world
	int y;
	// the x coordinate on the sprite sheet
	int tx;
	// the y coordinate on the sprite sheet
	int ty;
	int width;
	int height;

	std::vector<frame> mAnimatedtiles;
	bool mAnimated;
	bool mObstacle;
	float duration = 0;

	bool operator<(const tile& a)
	{
		return y < a.y;
	}

	tile(std::vector<frame> animatedtiles, SDL_Texture* tset, int x = 0, int y = 0,
	     int tx = 0, int ty = 0, int w = 0, int h = 0, bool animated = false, bool Obstacle = false);
	void draw(SDL_Renderer* ren, SDL_Rect& camera);
	void drawNew(SDL_Renderer* ren, SDL_Rect& camera);
	void render(int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr,
	            SDL_RendererFlip flip = SDL_FLIP_NONE);
};

class level
{
public:
	level(const std::string& name);
	void load(const std::string& path, SDL_Renderer* ren);
	void draw(SDL_Renderer* ren, SDL_Rect& camera);
	void drawLayer(SDL_Renderer* ren, SDL_Rect& camera, int layer);
	void drawLayerTillYvalue(SDL_Renderer* ren, SDL_Rect& camera, int layer, int y);
	void drawLayerAfterYvalue(SDL_Renderer* ren, SDL_Rect& camera, int layer, int y);
	float GetLevelWidth();
	int GetTileWidth();
	int GetLevelColumCnt(); //Width
	int GetLevelRowCnt(); //Height
	float GetLevelHight();
	std::vector<tile> GetTilesByLayer(int layer);
	std::vector<SDL_Rect>* GetCollisionLayer();

private:
	std::string name;
	// Think of the dimensions as a 2D array (after all, that's what our
	// Tiled map is)
	// The rows variable is the number of tiles from top to bottom (Y axis).
	int rows;
	// The cols variable is the number of tiles from left to right (X axis).
	int cols;
	int tile_width;
	int tile_height;
	// All of the tiles we will draw to the screen.
	std::vector<std::vector<tile>> tilesbyLayer;
	// All of the tilesets used by our Tiled map.
	std::map<gid, SDL_Texture*> tilesets;
	std::vector<SDL_Rect> collisionLayer;
};
