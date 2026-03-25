import pygame
import math

pygame.init()

WINDOW_WIDTH = 600
WINDOW_HEIGTH = 600
window = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGTH))
pygame.display.set_caption("Figura - wariant 4.")

color = (255, 0, 0)
background_color = (255, 255, 255)

window.fill(background_color)
PADDING = 20

LINE_LENGTH = WINDOW_WIDTH - 2 * PADDING
LINE_WIDTH = LINE_LENGTH // 40

line_surf = pygame.Surface((LINE_LENGTH, LINE_WIDTH), pygame.SRCALPHA)
line_surf.fill(color)

# Diagonal line rotation independent of window dimensions
x_Distance = LINE_LENGTH
y_Distance = WINDOW_HEIGTH - 2 * PADDING
DIAGONAL_LINE_LENGTH = (
    math.sqrt(x_Distance**2 + y_Distance**2) - 10
)  # -10 to achieve visible clipping like in the pdf
angle = (
    math.degrees(math.atan2(x_Distance, y_Distance)) - 0.7
)  # arg(x + iy); -0.7 to achieve visible clipping like in the pdf

diagonal_line_surf = pygame.Surface((DIAGONAL_LINE_LENGTH, LINE_WIDTH), pygame.SRCALPHA)
diagonal_line_surf.fill(color)
diagonal_line_surf = pygame.transform.rotate(diagonal_line_surf, angle)

run = True
while run:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

    # Top line = line surface drawn from top left
    topLine = line_surf.get_rect(topleft=(PADDING, PADDING))
    window.blit(line_surf, topLine)

    # Bottom line = line surface droawn from bottom left
    bottomLine = line_surf.get_rect(bottomleft=(PADDING, WINDOW_HEIGTH - PADDING))
    window.blit(line_surf, bottomLine)

    diagonalLine = diagonal_line_surf.get_rect(
        center=(WINDOW_WIDTH // 2, WINDOW_HEIGTH // 2)
    )
    window.blit(diagonal_line_surf, diagonalLine)

    pygame.display.flip()

pygame.quit()
