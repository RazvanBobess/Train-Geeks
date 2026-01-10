#pragma once
#include <vector>
#include <array>
#include <cassert>
#include "glm/glm.hpp"

namespace t2 {
    enum class Direction : int {
        NORTH = 0,
        EAST = 1,
        SOUTH = 2,
        WEST = 3
    };

    inline Direction Opposite(Direction d) {
        return static_cast<Direction>((static_cast<int>(d) + 2) % 4);
    }

    struct Vec2i {
        int x;
        int y;

        Vec2i operator+(const Vec2i& other) const {
            return { x + other.x, y + other.y };
        }

        Vec2i operator*(int v) const {
            return { x * v, y * v };
        }
    };

    inline Vec2i DirectionOffset(Direction d) {
        switch (d) {
            case Direction::NORTH: return { 0, -1 };
            case Direction::SOUTH: return { 0,  1 };
            case Direction::EAST:  return { 1,  0 };
            case Direction::WEST:  return { -1, 0 };
        }
        return {0, 0};
    }

    enum class RailType {
        RAIL,
        BRIDGE_RAIL,
        TUNNEL_RAIL
    };

    struct RailSegment {
        RailType type = RailType::RAIL;
        Vec2i cell;

        std::array<bool, 4> connections { false, false, false, false };
        std::array<struct RailSegment*, 4> nextSegments { nullptr, nullptr, nullptr, nullptr };
    };

    struct RenderTransform {
        glm::vec3 pos;
        float yaw;
    };

    struct Cell {
        Vec2i position;
        RailSegment* rail = nullptr;
    };

    class RailGrid {
        public:
            RailGrid(int width, int height);

            RailGrid();

            ~RailGrid();
            Cell& cell(int x, int y);
            const Cell& cell(int x, int y) const {
                return m_cells[y * m_width + x];
            }

            bool inBounds(const Vec2i& p);
            void createSimpleRail(int x, int y, bool horizontal);
            void createBridge(const Vec2i& start, Direction dir);
            void linkRails();

        private:
            int m_width;
            int m_height;
            std::vector<Cell> m_cells;
    };
}