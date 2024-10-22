# get_dominant.py

import numpy as np
from rgb_to_name import rgb_to_name

def get_dominant_color(image):
    image = image.convert("RGBA").resize((50, 50))
    pixels = np.array(image)
    
    # Séparer les pixels rgb et alpha
    rgb_pixels = pixels[:, :, :3]
    alpha_channel = pixels[:, :, 3]
    
    # Filtrer les pixels qui ne sont pas transparents (alpha > 0)
    valid_pixels = rgb_pixels[alpha_channel > 0]
    
    # Ignorer les pixels noirs (ou presque)
    black_threshold = 10
    non_black_pixels = valid_pixels[np.all(valid_pixels > black_threshold, axis=1)]

    if len(non_black_pixels) == 0:
        return "black"

    dominant_color = np.mean(non_black_pixels, axis=0)
    result = rgb_to_name(tuple(dominant_color.astype(int)))
    
    return result

