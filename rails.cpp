#include "lab_m1/tema2/rails.h"

#include <cmath>

namespace t2 {
    RailGrid::RailGrid() : m_width(0), m_height(0) {
    }

    Cell& RailGrid::cell(int x, int y) {
        return m_cells[y * m_width + x];
    }

    RailGrid::RailGrid(int width, int height) : m_width(width), m_height(height) {
        m_cells.resize(width * height);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                m_cells[y * width + x].position = { x, y };
            }
        }
    }

    RailGrid::~RailGrid(){
        for (auto& c : m_cells) {
            delete c.rail;
        }
    }

    bool RailGrid::inBounds(const Vec2i& p) {
        return p.x >= 0 && p.y >= 0 && p.x < m_width && p.y < m_height;
    }

    void RailGrid::createSimpleRail(int x, int y, bool horizontal) {
        RailSegment* rail = new RailSegment();
        rail->type = RailType::RAIL;

        rail->cell.x = x;
        rail->cell.y = y;
        
        if (horizontal) {
            rail->connections[(int)Direction::EAST] = true;
            rail->connections[(int)Direction::WEST] = true;
        } else {
            rail->connections[(int)Direction::NORTH] = true;
            rail->connections[(int)Direction::SOUTH] = true;
        }

        cell(x, y).rail = rail;
    }

    void RailGrid::createBridge(const Vec2i& start, Direction dir) {
        RailSegment* bridgeRoot = new RailSegment();
        bridgeRoot->type = RailType::BRIDGE_RAIL;

        bridgeRoot->cell.x = start.x;
        bridgeRoot->cell.y = start.y;

        bridgeRoot->connections[(int)dir] = true;
        bridgeRoot->connections[(int)Opposite(dir)] = true;

        cell(start.x, start.y).rail = bridgeRoot;
    }

    void RailGrid::linkRails() {
        for (auto& c : m_cells) {
            RailSegment* rail = c.rail;
            if (!rail) continue;

            for (int d = 0; d < 4; ++d) {
                if (!rail->connections[d]) continue;

                Vec2i npos = c.position + DirectionOffset((Direction)d);
                if (!inBounds(npos)) continue;

                RailSegment* neighbor = cell(npos.x, npos.y).rail;
                if (!neighbor) continue;

                if (neighbor->connections[(int)Opposite((Direction)d)]) {
                    rail->nextSegments[d] = neighbor;
                }
            }
        }
    }
}