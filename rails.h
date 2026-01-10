#pragma once
#include <vector>
#include <array>
#include <cassert>
#include "glm/glm.hpp"

namespace t2 {
    enum class RailType {
        RAIL,
        BRIDGE_RAIL,
        TUNNEL_RAIL
    };

    enum class Direction : int {
        NORTH = 0,
        EAST = 1,
        SOUTH = 2,
        WEST = 3
    };

    struct Cell {
        int x, y;
        bool isSwitch;
        std::array<bool, 4> connections;
    };

    class RailGrid {
        public:
            RailGrid(int width, int height);

            RailGrid();

            ~RailGrid();
            bool hasRail(int x, int y);

            void connectCells(int x1, int y1, int x2, int y2);
            int numberOfConnections(Cell cell);
            void setRail(int x, int y, bool value = true);
            bool hasNeighbor(Cell cell, Direction dir);
            Cell getCell(int x, int y);
            glm::ivec2 dirOffset(Direction dir);

        private:
            int width;
            int height;
            int columns;
            int rows;
            std::vector<bool> railGrid;
    };
}