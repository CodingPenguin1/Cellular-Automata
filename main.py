#!/usr/bin/env python3
"""
"""

from time import time

import pygame

from World import World

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
WORLD_SIZE = (500, 500)


if __name__ == '__main__':
    # Initialization
    pygame.init()
    window = pygame.display.set_mode(WORLD_SIZE)
    clock = pygame.time.Clock()
    world = World((WORLD_SIZE))
    targetFramerate = 1
    world.randomize()

    # Game loop
    while True:
        # Input
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_q:
                    quit()

        # Render frame
        print('Rendering frame')
        t0 = time()
        window.fill(WHITE)
        for x in range(world.width):
            for y in range(world.height):
                if world.grid[y][x] == 1:
                    pygame.draw.rect(window, BLACK, (x, y, 1, 1))
        print(f'{round(time() - t0, 2)}s')

        # Update world
        print('Updating world')
        t0 = time()
        world.updateConways()
        print(f'{round(time() - t0, 2)}s')

        # Update clock and display
        print('Displaying frame')
        t0 = time()
        clock.tick(targetFramerate)
        pygame.display.update()
        print(f'{round(time() - t0, 2)}s')
