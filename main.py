#!/usr/bin/env python3
"""
"""

import pygame


if __name__ == '__main__':
    pygame.init()
    window = pygame.display.set_mode((720, 480))
    clock = pygame.time.Clock()
    FPS = 60

    BLACK = (0, 0, 0)
    WHITE = (255, 255, 255)

    rect = pygame.Rect((0, 0), (32, 32))
    image = pygame.Surface((32, 32))
    image .fill(WHITE)

    while True:
        clock.tick(FPS)

        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_q:
                    quit()

        pygame.draw.rect(window, WHITE, (0, 0, 100, 100))
        pygame.display.update()
