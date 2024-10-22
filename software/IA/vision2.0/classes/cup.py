# classes/cup.py
from get_dominant_color import get_dominant_color

def analyze_cup(image):
    """Analyser l'image pour la classe 'cup'."""
    details = {
        "cup_color": get_dominant_color(image)
    }
    
    return details
