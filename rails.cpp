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

    Cell RailGrid::getCell(int x, int y) {
        Cell cell;
        cell.x = x;
        cell.y = y;
        cell.isSwitch = false;
        cell.connections = {false, false, false, false};

        if (!hasRail(x, y)) return cell;

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

            for (int x = 0; x < width; x++) {
                for (int y = 0; y < height; y++) {
                    railGrid[y * width + x] = false;

                    bool isRail = (x == 0 || x == width - 1 || y == 0 || y == height - 1);
                    railGrid[y * width + x] = isRail;
                }
            }
        }

    RailGrid::~RailGrid(){
    }

    void RailGrid::connectCells(int x1, int y1, int x2, int y2) {
        int dirX = (x2 > x1) ? 1 : -1;
        int dirY = (y2 > y1) ? 1 : -1;

        for (int x = x1; x != x2; x += dirX) {
            setRail(x, y1, true);
        }

        for (int y = y1; y != y2; y += dirY) {
            setRail(x2, y, true);
        }
    }

    void RailGrid::setRail(int x, int y, bool value) {
        if (x < 0 || y < 0 || x >= width || y >= height) {
            return;
        }
        railGrid[y * width + x] = value;
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