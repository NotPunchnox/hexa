# classes/person.py

from get_dominant_color import get_dominant_color

def analyze_person(model, processor, image):
    """Analyser l'image pour la classe 'person'."""
    classes = ["brown hair", "white hair", "blond hair", "no hair", "red hair", "black hair", "dark hair", "grey hair", "glasses"]
    inputs = processor(text=classes, images=image, return_tensors="pt", padding=True)

    # Get model outputs
    outputs = model(**inputs)
    logits_per_image = outputs.logits_per_image
    probs = logits_per_image.softmax(dim=1)

    # Classify hair colors
    probs_classes = [(probs[0][i], class_name) for i, class_name in enumerate(classes[:-1])]
    probs_classes_sorted = sorted(probs_classes, key=lambda x: x[0], reverse=True)  # Sort in descending order

    # Display results
    print("Sorted probabilities and classes:")
    for prob, class_name in probs_classes_sorted:
        print(f"{class_name}: {prob:.2f}")


    best_prob, best_class_name = probs_classes_sorted[0]

    hair_color = "inconnu"
    if best_prob > 0.2:
        hair_color = best_class_name.split(' ')[0]
        print(f"Detected: {best_class_name} with highest confidence {best_prob:.2f}")

    details = {
        "hair_color": hair_color,
        "clothes_color": get_dominant_color(image),
        "glasses": "oui" if probs[0][len(classes)-1] > 0.3 else "non"
    }
    
    return details