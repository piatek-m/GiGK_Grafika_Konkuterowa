import pygame
import numpy as np

pygame.init()

WINDOW_WIDTH = 1400
WINDOW_HEIGHT = 1400
window = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
pygame.display.set_caption("Wóz asenizacyjny Veenhuis")
YELLOW = (255, 255, 0)

original = pygame.image.load(
    "John_Deere_1640,_Veenhuis_slurry_trailer.jpg"
).convert_alpha()
original = pygame.transform.scale(original, (WINDOW_WIDTH // 2, WINDOW_HEIGHT // 3))


# Apply shear to a surface in X and/or Y axis
def apply_shear(surface, shear_x=0.0, shear_y=0.0):
    arr = pygame.surfarray.array3d(surface)
    width, height = surface.get_size()

    x_offset = int(abs(shear_x) * height)
    y_offset = int(abs(shear_y) * width)
    new_width = width + x_offset
    new_height = height + y_offset

    result = np.zeros((new_width, new_height, 3), dtype=np.uint8)

    for y in range(height):
        x_shift = int(shear_x * (height - y))
        for x in range(width):
            y_shift = int(shear_y * x)
            result[x + x_shift, y + y_shift] = arr[x, y]

    resultSurface = pygame.surfarray.make_surface(result)
    output = pygame.Surface(resultSurface.get_size())

    output.fill(YELLOW)
    resultSurface.set_colorkey((0, 0, 0))

    output.blit(resultSurface, (0, 0))
    return output


# Drawing different variants
def get_variant(n):
    img = original.copy()
    match n:
        case 1:
            return pygame.transform.smoothscale_by(img, 0.5)
        case 2:
            return pygame.transform.rotate(img, -45)
        case 3:
            flipped = pygame.transform.flip(img, False, True)
            return pygame.transform.smoothscale_by(flipped, (0.6, 1.5))
        case 4:
            flipped = pygame.transform.flip(img, True, False)
            sheared = apply_shear(flipped, 0.4)
            return pygame.transform.flip(sheared, True, False)
        case 5:
            scaled = pygame.transform.smoothscale_by(img, (1.5, 0.6))
            return scaled
        case 6:
            flipped = pygame.transform.flip(img, True, False)
            sheared = apply_shear(flipped, 0.4)
            rotated = pygame.transform.rotate(sheared, 90)
            return pygame.transform.flip(rotated, True, False)
        case 7:
            flipped = pygame.transform.flip(img, False, True)
            scaled = pygame.transform.smoothscale_by(flipped, (0.6, 1.5))
            return pygame.transform.flip(scaled, True, False)
        case 8:
            scaled = pygame.transform.smoothscale_by(img, (1.5, 0.6))
            return pygame.transform.rotate(scaled, -20)
        case 9:
            flipped = pygame.transform.flip(img, True, True)
            return apply_shear(flipped, 0, 0.4)
    return img


# Starting as variant 4
current_variant = 4
transformed = get_variant(current_variant)

run = True
while run:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

        if event.type == pygame.KEYDOWN:
            key = event.key
            if pygame.K_1 <= key <= pygame.K_9:
                current_variant = key - pygame.K_0
                transformed = get_variant(current_variant)

    window.fill(YELLOW)

    match current_variant:
        case 5:
            rect = transformed.get_rect(topleft=(WINDOW_WIDTH // 8, 0))
        case 8:
            rect = transformed.get_rect(
                midbottom=(WINDOW_WIDTH // 2.2, WINDOW_HEIGHT - (WINDOW_HEIGHT // 30))
            )
        case 9:
            rect = transformed.get_rect(topright=(WINDOW_WIDTH, WINDOW_HEIGHT // 3))
        case _:
            rect = transformed.get_rect(center=window.get_rect().center)

    window.blit(transformed, rect)
    pygame.display.flip()

pygame.quit()
