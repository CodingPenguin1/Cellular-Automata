#!/usr/bin/env python3
""" Class for the world the CAs live in

"""

from random import random

import numpy as np
from tabulate import tabulate


class World:
    def __init__(self, size=(720, 480)):
        self.grid = np.zeros(size, dtype=np.uint8)
        self.width = size[0]
        self.height = size[1]

    def toggleCell(self, coords):
        self.grid[coords[0]][coords[1]] = 0 if self.grid[coords[0]][coords[1]] else 1

    def randomize(self, toggleChance=0.5):
        for row in range(self.height):
            for col in range(self.width):
                if random() < toggleChance:
                    self.toggleCell((row, col))

    def updateConways(self):
        def getNeighborCount(array, coords):
            count = 0 if array[coords[0]][coords[1]] == 0 else -1
            for row in range(coords[0] - 1, coords[0] + 2):
                for col in range(coords[1] - 1, coords[1] + 2):
                    if row >= 0 and row < self.height and col >= 0 and col < self.width:
                        if self.grid[row][col] == 1:
                            count += 1
            return count

        nextBoardState = np.zeros((self.width, self.height), dtype=np.uint8)

        # Determine new state for each cell and set on new board
        for row in range(self.height):
            for col in range(self.width):
                neighbors = getNeighborCount(self.grid, [row, col])
                if self.grid[row][col] == 1:
                    # Any live cell with fewer than two live neighbours dies
                    if neighbors < 2:
                        nextBoardState[row][col] = 0
                    # Any live cell with two or three live neighbours lives on to the next generation
                    if neighbors == 2 or neighbors == 3:
                        nextBoardState[row][col] = 1
                    # Any live cell with more than three live neighbours dies
                    else:
                        nextBoardState[row][col] = 0
                else:
                    # Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
                    if neighbors == 3:
                        nextBoardState[row][col] = 1
        self.grid = np.copy(nextBoardState)

    def __str__(self):
        return tabulate(self.grid, tablefmt='plain')


if __name__ == '__main__':
    world = World((10, 10))
    world.grid[7][2] = 1
    world.grid[7][3] = 1
    world.grid[8][3] = 1
    print(world)
    while input() != 'q':
        world.updateConways()
        print(world)


