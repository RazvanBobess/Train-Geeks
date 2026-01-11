#pragma once
#include <vector>
#include <array>
#include <cassert>
#include "glm/glm.hpp"

namespace t2 {
    enum class RailType {
        NULLRAIL,
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
        Direction dir;
        std::array<bool, 4> connections;
		std::array<Cell*, 4> neighbors = { nullptr, nullptr, nullptr, nullptr };
    };

    class RailGrid {
    public:
        RailGrid(int width, int height);

        RailGrid();

        ~RailGrid();
        bool hasRail(int x, int y);

        void connectCells(int x1, int y1, int x2, int y2, RailType rail = RailType::NULLRAIL);
        void setRail(int x, int y, bool value = true, RailType rail = RailType::NULLRAIL);
		void setStraightRail(int x, int y, Direction dir, RailType rail = RailType::NULLRAIL);
        void setCellNeighbor(Cell& cell, Direction dir, Cell& neigh);

        bool hasNeighbor(Cell cell, Direction dir);
        Cell getCell(int x, int y);
		RailType getRailType(int x, int y);

        int numberOfConnections(Cell cell);
        glm::ivec2 dirOffset(Direction dir);

    private:
        int width;
        int height;
        int columns;
        int rows;
        std::vector<bool> railGrid;
		std::vector<RailType> railTypes;
        std::vector<Direction> railDirs;
    };
}