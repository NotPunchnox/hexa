# controllers/__init__.py

from .extract_text import find_text
from .segmentation import Object_detection

__all__ = ["find_text", "Object_detection"]