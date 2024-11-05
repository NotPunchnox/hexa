# controllers/__init__.py

from .extract_text import find_text
from .segmentation import Object_detection
from .Face import face_detector
from .Vars import variables

__all__ = ["find_text", "Object_detection", "face_detector", "variables"]
