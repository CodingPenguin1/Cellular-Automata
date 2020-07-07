#!/usr/bin/env python3
""" Class for the world the CAs live in

"""

import concurrent.futures
from multiprocessing import cpu_count
from random import random

import numpy as np
from tabulate import tabulate


class World:
    def __init__(self, size=(720, 480)):
        self.width = size[0]
        self.height = size[1]
        self.grid = np.zeros((self.height, self.width), dtype=np.uint8)

    def toggleCell(self, coords):
        self.grid[coords[0]][coords[1]] = 0 if self.grid[coords[0]][coords[1]] else 1

    def randomize(self, toggleChance=0.5):
        for row in range(self.height):
            for col in range(self.width):
                if random() < toggleChance:
                    self.toggleCell((row, col))

    def updateConways(self):
        # Generate next board state
        nextBoardState = np.zeros((self.height, self.width), dtype=np.uint8)
        with concurrent.futures.ProcessPoolExecutor(cpu_count()) as executor:
            rows = [executor.submit(_generateRow, self.grid, i).result() for i in range(len(self.grid))]
        for row in rows:
            nextBoardState[row['index']] = row['values']
        self.grid = np.copy(nextBoardState)

    def __str__(self):
        return tabulate(self.grid, tablefmt='plain')


def _getNeighborCount(array, coords):
    count = 0 if array[coords[0]][coords[1]] == 0 else -1
    for row in range(coords[0] - 1, coords[0] + 2):
        for col in range(coords[1] - 1, coords[1] + 2):
            if row >= 0 and row < len(array) and col >= 0 and col < len(array[0]):
                if array[row][col] == 1:
                    count += 1
    return count


def _generateRow(array, rowIndex):
    row = np.copy(array[rowIndex])
    for col in range(len(array[rowIndex])):
        neighbors = _getNeighborCount(array, [rowIndex, col])
        if array[rowIndex][col] == 1:
            # Any live cell with fewer than two live neighbours dies
            if neighbors < 2:
                row[col] = 0
            # Any live cell with two or three live neighbours lives on to the next generation
            if neighbors == 2 or neighbors == 3:
                row[col] = 1
            # Any live cell with more than three live neighbours dies
            else:
                row[col] = 0
        else:
            # Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
            if neighbors == 3:
                row[col] = 1
            else:
                row[col] = 0
    return {'index': rowIndex, 'values': row}


if __name__ == '__main__':
    world = World((10, 2))
    print(world)
    while input() != 'q':
        world.updateConways()
        print(world)
