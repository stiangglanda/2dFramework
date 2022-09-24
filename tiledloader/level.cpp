#include "level.hpp"
#include "assets.hpp"
#include "tmxlite/Map.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/TileLayer.hpp"
#include <iostream>
#include <algorithm>
#include "Framework.h"

tile::tile(std::vector<frame> animatedtiles, SDL_Texture* tset, int x, int y, int tx, int ty, int w, int h, bool animated)
: mAnimatedtiles(animatedtiles), sheet(tset), x(x), y(y), tx(tx), ty(ty), width(w), height(h), mAnimated(animated) {

}

void tile::draw(SDL_Renderer* ren, SDL_Rect& camera) {
    if (!ren || !sheet)
        return;



    SDL_Rect src;
    src.x = tx;
    src.y = ty;
    src.w = width;
    src.h = height;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = src.w;
    dest.h = src.h;

    //if (g_pFramework->checkCollision(camera, dest))
    //{
    //    //Show the tile
    //    //gTileTexture.render(dest.x - camera.x, dest.y - camera.y, &gTileClips[mType]);
    //    dest.x = -camera.x; 
    //    dest.y = -camera.y;

    //}
    SDL_RenderCopy(ren, sheet, &src, &dest);
}

void tile::drawNew(SDL_Renderer* ren, SDL_Rect& camera) {
    if (!ren || !sheet)
        return;

    if (mAnimated)
    {
        if (duration >= mAnimatedtiles.size())//TODO >=
        {
            duration = 0;
        }
        frame fr = mAnimatedtiles[duration];
        tx = fr.x;
        ty = fr.y;

        duration += 5 * Timer::Get()->GetElapsed();
        //duration += fr.duration * Timer::Get()->GetElapsed();
    }

    SDL_Rect src;
    src.x = tx;
    src.y = ty;
    src.w = width;
    src.h = height;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = src.w;
    dest.h = src.h;

    if (g_pFramework->checkCollision(camera, dest))
    {
        //Show the tile
        render(dest.x - camera.x, dest.y - camera.y, &src);
    }
}

void tile::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, width, height };

    //Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(g_pFramework->GetRenderer(), sheet, clip, &renderQuad, angle, center, flip);
}

level::level(const std::string& name) 
:  name(name), rows(0), cols(0) {

}

void level::load(const std::string& path, SDL_Renderer* ren) {
    // Load and parse the Tiled map with tmxlite
    tmx::Map tiled_map;
    tiled_map.load(path);

    // We need to know the size of the map (in tiles)
    auto map_dimensions = tiled_map.getTileCount();
    rows = map_dimensions.y;
    cols = map_dimensions.x;
    // We also need to know the dimensions of the tiles.
    auto tilesize = tiled_map.getTileSize();
    tile_width = tilesize.x;
    tile_height = tilesize.y;

    std::map<int, std::vector<frame>> animations;
    // Load all of the tilesets and store them in a data structure.
    // I chose to store them in a map.
    auto& map_tilesets = tiled_map.getTilesets();
    for (auto& tset : map_tilesets) {
        for (auto itile : tset.getTiles())
        {
            if (itile.animation.frames.size() > 1)
            {
                std::vector<frame> frames;
                for (size_t i = 0; i < itile.animation.frames.size(); i++)
                {
                    frame fr;
                    fr.duration = itile.animation.frames[i].duration;
                    fr.x = tset.getTile(itile.animation.frames[i].tileID)->imagePosition.x;
                    fr.y = tset.getTile(itile.animation.frames[i].tileID)->imagePosition.y;
                    frames.push_back(fr);
                }
                animations.insert(std::pair<int, std::vector<frame>>(itile.ID, frames));
            }
        }
        auto tex = assets::instance()
            .load_texture(tset.getImagePath(), ren);
        tilesets.insert(std::pair<gid, SDL_Texture*>(tset.getFirstGID(), tex));
    }

    // This is the hard part; iterate through each layer in the map,
    // poke each tile for the information you need, and store it in
    // our tiles data structure. 
    //
    // We start at the bottom most layer, and work our way up with this
    // outer for-loop.
    auto& map_layers = tiled_map.getLayers();
    for (auto& layer : map_layers) {
        // We're only looking to render the tiles on the map, so if
        // this layer isn't a tile layer, we'll move on.
        if (layer->getType() == tmx::Layer::Type::Tile) 
        {
        

        std::vector<tile> tilesforDimention;

        auto* tile_layer = dynamic_cast<const tmx::TileLayer*>(layer.get());

        // Grab all of this layer's tiles. 
        auto& layer_tiles = tile_layer->getTiles();

        // Remember when we needed the size of the Tiled map? This
        // is where it comes into play. Because we're making a simple
        // 2D game, each tile belongs in an x,y coordinate, so using
        // a loop can help us visit each place if there's a tile there.
        for (auto y = 0; y < rows; ++y) {
            for (auto x = 0; x < cols; ++x) {
                // the `layer_tiles` vector that our `getTiles()` call
                // returned on line 78 isn't a 2D array, but we can
                // convert our x, y coordinates into an index that we
                // can access it at.
                auto tile_index = x + (y * cols);
                // Grab the GID of the tile we're at.
                auto cur_gid = layer_tiles[tile_index].ID;
                // If the GID is 0, that means it's an empty tile,
                // we don't want to waste time on nothing, nor do we
                // want to store an empty tile.
                if (cur_gid == 0) {
                    continue;
                }

                // Find the tileset that this tile belongs to. The tileset
                // whose first GID is less-than-or-equal to this GID, is
                // the tileset our GID belongs to. A tileset with a first
                // GID that is greater than this tile means that this tile
                // does not belong to that tileset.
                auto tset_gid = -1;
                for (auto& ts : tilesets) {
                    if (ts.first <= cur_gid) {
                        tset_gid = ts.first;
                        break;
                    }
                }
                // If we didn't find a valid tileset, skip the tile. We can't
                // render it if we don't have a tileset to pull from.
                if (tset_gid == -1) {
                    continue;
                }

                // Normalize the GID.
                cur_gid -= tset_gid;
                // Find the dimensions of the tile sheet. This is important,
                // because our tile is only a small sprite on that sheet, not
                // the whole sheet.
                auto ts_width = 0;
                auto ts_height = 0;
                SDL_QueryTexture(tilesets[tset_gid],
                    NULL, NULL, &ts_width, &ts_height);

                // Calculate the area on the tilesheet to draw from.
                auto region_x = (cur_gid % (ts_width / tile_width)) * tile_width;
                auto region_y = (cur_gid / (ts_width / tile_width)) * tile_height;

                std::vector<frame> animatedtiles;
                auto anim = animations.find(cur_gid);
                bool animated = false;
                if (anim != animations.end())
                {
                    animated = true;
                    animatedtiles = anim->second;
                }

                // Calculate the world position of our tile. This is easy,
                // because we're using nested for-loop to visit each x,y
                // coordinate.
                auto x_pos = x * tile_width;
                auto y_pos = y * tile_height;

                // Phew, all done. 
                tile t(animatedtiles, tilesets[tset_gid], x_pos, y_pos,
                    region_x, region_y, tile_width, tile_height, animated);
                tilesforDimention.push_back(t);

            
                //pusch to dimension
            }
        }
        tilesbyLayer.push_back(tilesforDimention);
        }
        else if (layer->getType() == tmx::Layer::Type::Object)
        {
            auto* tile_layer = dynamic_cast<const tmx::ObjectGroup*>(layer.get());
            for (auto object : tile_layer->getObjects())
            {
                SDL_Rect rect;
                rect.x = object.getAABB().left;
                rect.y = object.getAABB().top;
                rect.w = object.getAABB().width;
                rect.h = object.getAABB().height;

                collisionLayer.push_back(rect);
            }
        }
    }
}

void level::draw(SDL_Renderer* ren, SDL_Rect& camera) {
    for (auto& layer : tilesbyLayer) {
        for (auto& tile : layer) {
            tile.draw(ren, camera);
        }
    }
}

void level::drawLayer(SDL_Renderer* ren, SDL_Rect& camera, int layer) {
    for (auto& tile : tilesbyLayer[layer]) {
        tile.drawNew(ren, camera);
    }
}

void level::drawLayerTillYvalue(SDL_Renderer* ren, SDL_Rect& camera, int layer, int y) {
    for (auto& tile : tilesbyLayer[layer]) {
        if (tile.y < y)
        {
            tile.drawNew(ren, camera);
        }
    }
}

void level::drawLayerAfterYvalue(SDL_Renderer* ren, SDL_Rect& camera, int layer, int y) {
    for (auto& tile : tilesbyLayer[layer]) {
        if (tile.y >= y)
        {
            tile.drawNew(ren, camera);
        }
    }
}

float level::GetLevelWidth()
{
    return tile_width * cols;
}

float level::GetLevelHight()
{
    return tile_height * rows;
}

std::vector<tile>* level::GetTilesByLayer(int layer)
{
    return &tilesbyLayer[layer];
}

std::vector<SDL_Rect>* level::GetCollisionLayer()
{
    return &collisionLayer;
}
