#include "lab_m1/tema2/rails.h"

#include <cmath>

namespace t2 {
    RailGrid::RailGrid() : width(0), height(0) {
    }

    glm::ivec2 RailGrid::dirOffset(Direction dir) {
        switch (dir) {
        case Direction::NORTH:
            return glm::ivec2(0, -1);
        case Direction::EAST:
            return glm::ivec2(1, 0);
        case Direction::SOUTH:
            return glm::ivec2(0, 1);
        case Direction::WEST:
            return glm::ivec2(-1, 0);
        }
        return glm::ivec2(0, 0);
    }

    RailType RailGrid::getRailType(int x, int y) {
        RailType rail;

        if (!hasRail(x, y)) {
            rail = RailType::NULLRAIL;
        } else {
            rail = railTypes[y * width + x];
		}

        return rail;
    }

    Cell RailGrid::getCell(int x, int y) {
        Cell cell;
        cell.x = x;
        cell.y = y;
        cell.isSwitch = false;
        cell.connections = { false, false, false, false };

        if (!hasRail(x, y)) return cell;

		cell.dir = railDirs[y * width + x];
        int connectionsCount = 0;

        for (int dir = 0; dir < 4; dir++) {
            Direction direction = static_cast<Direction>(dir);
            glm::ivec2 offset = dirOffset(direction);

            if (hasRail(x + offset.x, y + offset.y)) {
                cell.connections[dir] = true;
                connectionsCount++;
            }
        }

        cell.isSwitch = (connectionsCount > 2);
        return cell;
    }

    bool RailGrid::hasRail(int x, int y) {
        if (x < 0 || y < 0 || x >= width || y >= height) {
            return false;
        }
        return railGrid[y * width + x];
    }

    RailGrid::RailGrid(int columns, int rows)
        : columns(columns), rows(rows) {
        width = columns;
        height = rows;
        railGrid.resize(width * height, false);
		railTypes.resize(width * height, RailType::NULLRAIL);
		railDirs.resize(width * height, Direction::NORTH);
    }

    RailGrid::~RailGrid() {
    }

    void RailGrid::connectCells(int x1, int y1, int x2, int y2, RailType rail) {
        int dirX = (x2 > x1) ? 1 : -1;
        int dirY = (y2 > y1) ? 1 : -1;

        Direction dir;

        if (x1 == x2) {
            dir = (y2 > y1) ? Direction::NORTH : Direction::SOUTH;
        } else {
            dir = (x2 > x1) ? Direction::WEST : Direction::EAST;
        }

        for (int x = x1; x != x2; x += dirX) {
			setStraightRail(x, y1, dir, rail);
        }

        for (int y = y1; y != y2; y += dirY) {
			setStraightRail(x2, y, dir, rail);
        }
    }

    void RailGrid::setRail(int x, int y, bool value, RailType rail) {
        if (x < 0 || y < 0 || x >= width || y >= height) {
            return;
        }
        railGrid[y * width + x] = value;
		railTypes[y * width + x] = value ? rail : RailType::NULLRAIL;
    }

    void RailGrid::setStraightRail(int x, int y, Direction dir, RailType rail) {
        setRail(x, y, true, rail);
        
		Cell cell = getCell(x, y);
        if (dir == Direction::NORTH || dir == Direction::SOUTH) {
            cell.connections[static_cast<int>(Direction::NORTH)] = true;
            cell.connections[static_cast<int>(Direction::SOUTH)] = true;
        } else if (dir == Direction::EAST || dir == Direction::WEST) {
            cell.connections[static_cast<int>(Direction::EAST)] = true;
            cell.connections[static_cast<int>(Direction::WEST)] = true;
		}
		railDirs[y * width + x] = dir;
		cell.isSwitch = false;
	}

    bool RailGrid::hasNeighbor(Cell cell, Direction dir) {
        return cell.connections[static_cast<int>(dir)];
    }

    int RailGrid::numberOfConnections(Cell cell) {
        int count = 0;

        bool north = cell.connections[0];
        bool east = cell.connections[1];
        bool south = cell.connections[2];
        bool west = cell.connections[3];

        if (north) count++;
        if (east) count++;
        if (south) count++;
        if (west) count++;

        return count;
    }
}