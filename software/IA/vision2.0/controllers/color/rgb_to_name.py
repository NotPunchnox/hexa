# get_dominant.py

import numpy as np

COLOR_NAMES = {
    "black": (0, 0, 0),
    "white": (255, 255, 255),
    "red": (255, 0, 0),
    "green": (0, 255, 0),
    "blue": (0, 0, 255),
    "gray": (128, 128, 128),
    "light gray": (211, 211, 211),
    "dark gray": (169, 169, 169),
    "light red": (255, 102, 102),
    "light green": (102, 255, 102),
    "light blue": (102, 102, 255),
    "salmon": (250, 128, 114),
    "beige": (245, 222, 179),
    "tan": (210, 180, 140),
    "brown": (165, 42, 42),
    "violet": (238, 130, 238),
    "orchid": (218, 112, 214),
    "silver": (192, 192, 192),
    "charcoal": (54, 69, 79),
}

def rgb_to_name(rgb):
    r, g, b = rgb
    closest_color = None
    min_distance = float('inf')

    for color_name, color_value in COLOR_NAMES.items():
        distance = np.sqrt((r - color_value[0]) ** 2 + (g - color_value[1]) ** 2 + (b - color_value[2]) ** 2)
        if distance < min_distance:
            min_distance = distance
            closest_color = color_name

    return closest_color
