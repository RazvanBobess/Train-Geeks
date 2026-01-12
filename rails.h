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
		RailType railType;
        Direction dir;
        std::array<bool, 4> connections;
		std::array<Cell*, 4> neighbors = { nullptr, nullptr, nullptr, nullptr };
    };

    class RailGrid {
    public:
        RailGrid(int width, int height);
        RailGrid();
        ~RailGrid();

		RailType getRailType(int x, int y);
        Cell& getCell(int x, int y);
        bool hasRail(int x, int y);
		bool getNextCell(int x, int y, Direction dir, int& xout, int& yout);
        void connectCells(int x1, int y1, int x2, int y2, RailType rail = RailType::NULLRAIL);
		void setCellSwitch(Cell& cell, bool isSwitch = false);
        bool hasNeighbor(Cell cell, Direction dir);
        int numberOfConnections(Cell cell);
		void setStraightRail(int x, int y, Direction dir, RailType rail = RailType::NULLRAIL);
		void buildDefaultNeighbors();
        bool canMove(int x, int y, Direction dir);
        glm::ivec2 dirOffset(Direction dir);

        bool tryGetCell(int x, int y, Cell& outCell) const; 

        Direction intToDirection(int value) {
            switch (value) {
            case 0:
                return Direction::NORTH;
            case 1:
                return Direction::EAST;
            case 2:
                return Direction::SOUTH;
            case 3:
                return Direction::WEST;
            default:
                return Direction::NORTH;
            }
		}
        glm::ivec2 dirOffsetConst(Direction dir) const;

        void setCellNeighbor(Cell& cell, Direction dir, Cell& neigh, bool skipMiddle = false);
		void connectSwich(Cell& sw, const std::vector<std::pair<Direction, Cell*>>& neighbors);

    private:
        int width;
        int height;
        int columns;
        int rows;

		std::vector<Cell> cells;
    };
}