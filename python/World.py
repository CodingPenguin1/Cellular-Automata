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
    row, col = coords
    height, width = len(array), len(array[0])
    return array[row - 1][col - 1] + array[row - 1][col] + array[row - 1][(col + 1) % width] +\
           array[row][col - 1] + array[row][(col + 1) % width] +\
           array[(row + 1) % height][col - 1] + array[(row + 1) % height][col] + array[(row + 1) % height][(col + 1) % width]


def _generateRow(array, rowIndex):
    row = np.copy(array[rowIndex])
    for col in range(len(array[rowIndex])):
        neighbors = _getNeighborCount(array, [rowIndex, col])
        row[col] = ((~neighbors >> 2) & (neighbors >> 1) & (array[rowIndex][col] | neighbors)) & 1
    return {'index': rowIndex, 'values': row}


if __name__ == '__main__':
    world = World((10, 10))

    world.randomize()
    # world.grid[5][5] = 1
    # world.grid[5][6] = 1
    # world.grid[6][5] = 1

    print(world)
    while input() != 'q':
        world.updateConways()
        print(world)
