#include "lab_m1/tema2/rails.h"

#include <cmath>

namespace t2 {
    RailGrid::RailGrid() : width(0), height(0) {
    }

    int directionToInt(Direction dir) {
        switch (dir)
        {
        case t2::Direction::NORTH:
			return 0;
            break;
        case t2::Direction::EAST:
			return 1;
            break;
        case t2::Direction::SOUTH:
			return 2;
            break;
        case t2::Direction::WEST:
			return 3;
            break;
        default:
            break;
        }

		return -1;
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

    glm::ivec2 RailGrid::dirOffsetConst(Direction dir) const {
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
        if (x < 0 || y < 0 || x >= width || y >= height) {
            return RailType::NULLRAIL;
		}
		return getCell(x, y).railType;
    }

    Cell& RailGrid::getCell(int x, int y) {
        assert(x >= 0 && y >= 0 && x < width && y < height);
        return cells[y * width + x];
    }

    bool RailGrid::hasRail(int x, int y) {
        if (x < 0 || y < 0 || x >= width || y >= height) {
            return false;
        }
		return getRailType(x, y) != RailType::NULLRAIL;
    }

    bool RailGrid::tryGetCell(int x, int y, Cell& outCell) const {
        if (x < 0 || y < 0 || x >= width || y >= height) {
            return false;
        }
        outCell = cells[y * width + x];
        return true;
    }

    bool RailGrid::getNextCell(int x, int y, Direction dir, int& xout, int& yout) {
		Cell& cell = getCell(x, y);
		int dirIdx = directionToInt(dir);

        if (!cell.connections[dirIdx])
			return false;

		glm::ivec2 off = dirOffset(dir);
        xout = x + off.x;
		yout = y + off.y;

        if (!hasRail(xout, yout)) return false;

		Cell& nextCell = getCell(xout, yout);
        int back = (dirIdx + 2) % 4;

		return nextCell.connections[back];
	}

    bool RailGrid::canMove(int x, int y, Direction dir) {
        if (!hasRail(x, y)) return false;
        
		glm::ivec2 off = dirOffset(dir);
        int nx = x + off.x;
        int ny = y + off.y;

        if (!hasRail(nx, ny)) return false;

		Cell cur = getCell(x, y);
		Cell next = getCell(nx, ny);

		int value = directionToInt(dir);

		Direction back = intToDirection((value + 2) % 4);
		int backValue = directionToInt(back);

		return cur.connections[value] && next.connections[backValue];
	}

    RailGrid::RailGrid(int columns, int rows)
        : columns(columns), rows(rows) {
        width = columns;
        height = rows;

		cells.resize(width * height);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Cell& c = cells[y * width + x];
                c.x = x;
                c.y = y;
                c.isSwitch = false;
                c.dir = Direction::NORTH;
				c.railType = RailType::NULLRAIL;
                c.connections.fill(false);
                c.neighbors.fill(nullptr);
            }
        }
    }

    RailGrid::~RailGrid() {
    }

    void RailGrid::setStraightRail(int x, int y, Direction dir, RailType rail) {
        if (x < 0 || y < 0 || x >= width || y >= height) {
            return;
		}

        Cell& cell = getCell(x, y);
		cell.railType = rail;
        cell.dir = dir;
        cell.isSwitch = false;
        cell.connections.fill(false);
        cell.neighbors.fill(nullptr);


        int d = directionToInt(dir);
        int back = (d + 2) % 4;

        cell.connections[d] = true;
        cell.connections[back] = true;

		int nx = x + dirOffset(dir).x;
        int ny = y + dirOffset(dir).y;

        if (hasRail(nx, ny)) {
            Cell& neigh = getCell(nx, ny);
            cell.neighbors[d] = &neigh;
            neigh.neighbors[back] = &cell;
            neigh.connections[back] = true;
		}
    }

    void RailGrid::connectCells(int x1, int y1, int x2, int y2, RailType rail) {
        int dx = (x2 > x1) ? 1 : -1;
        int dy = (y2 > y1) ? 1 : -1;

        if (x1 == x2) {
            Direction dir = (y2 > y1) ? Direction::SOUTH : Direction::NORTH;
            for (int y = y1; y != y2; y += dy)
                setStraightRail(x1, y, dir, rail);
        }
        else if (y1 == y2) {
            Direction dir = (x2 > x1) ? Direction::EAST : Direction::WEST;
            for (int x = x1; x != x2; x += dx)
                setStraightRail(x, y1, dir, rail);
        }
        else {
            for (int x = x1; x != x2; x += dx)
                setStraightRail(x, y1, (dx > 0) ? Direction::EAST : Direction::WEST, rail);
            for (int y = y1; y != y2; y += dy)
                setStraightRail(x2, y, (dy > 0) ? Direction::SOUTH : Direction::NORTH, rail);
        }
    }

    void RailGrid::buildDefaultNeighbors() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Cell& c = getCell(x, y);
                if (!hasRail(x, y)) continue;

                for (int d = 0; d < 4; d++) {
                    if (!c.connections[d]) continue;
                    Direction dir = (Direction)d;
                    int nx = x + dirOffset(dir).x;
                    int ny = y + dirOffset(dir).y;
                    if (hasRail(nx, ny)) {
                        c.neighbors[d] = &getCell(nx, ny);
                    }
                }
            }
        }
    }

    void RailGrid::setCellSwitch(Cell& cell, bool isSwitch) {
        cell.isSwitch = isSwitch;
    }

    bool RailGrid::hasNeighbor(Cell cell, Direction dir) {
        return cell.connections[directionToInt(dir)];
    }

    void RailGrid::setCellNeighbor(Cell& cell, Direction dir, Cell& neigh, bool skipMiddle) {
        int value = directionToInt(dir);
        int back = (value + 2) % 4;

        if (!skipMiddle) {
            cell.connections[value] = true;
            cell.neighbors[value] = &neigh;

            neigh.connections[back] = true;
            neigh.neighbors[back] = &cell;
        }
        else {

            cell.connections[value] = true;
            cell.neighbors[value] = &neigh;

            neigh.connections[back] = true;
            neigh.neighbors[back] = &cell;
        }
    }

    void RailGrid::connectSwich(Cell& sw, const std::vector<std::pair<Direction, Cell*>>& neighbors) {
        sw.connections.fill(false);
        sw.neighbors.fill(nullptr);
        sw.isSwitch = true;

        for (auto& p : neighbors) {
            setCellNeighbor(sw, p.first, *p.second, true);
        }
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