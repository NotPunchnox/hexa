from controllers.Vars import variables

def calculate_face_position_percentage(face_data, image_width, image_height, tolerance=20):
    
    # Supposons qu'on s'intéresse uniquement au premier visage détecté
    face_data = face_data[0]
    x1, y1, x2, y2 = face_data["x1"], face_data["y1"], face_data["x2"], face_data["y2"]
    distance = face_data["distance"]

    # Gérer les commandes en fonction de la distance
    if distance > 1:
        variables.command = "StartWalk_2_0_2"
    elif distance < 0.5:
        variables.command = "StartWalk_2_0_-2"
    else:
        variables.command = "StopWalk"
    
    # Envoyer la commande si elle est nouvelle
    if variables.command != variables.last_command:
        print(variables.command)
        variables.last_command = variables.command

    # Calculer la position centrale du visage
    face_center_x = (x1 + x2) / 2
    face_center_y = (y1 + y2) / 2
    
    # Calculer les pourcentages de position par rapport à l'image
    left_percentage = (face_center_x / image_width) * 100
    right_percentage = 100 - left_percentage
    top_percentage = (face_center_y / image_height) * 100
    bottom_percentage = 100 - top_percentage
    
    # Messages d'orientation pour recentrer le visage
    guidance_message = ""
    move_directions = {
        "horizontal": "",
        "vertical": "",
        "distance": ""
    }
    
    # Centrage horizontal
    if left_percentage < 50 - tolerance:
        move_directions["horizontal"] = "aller à gauche"
        guidance_message += "Aller à gauche. "
    elif right_percentage < 50 - tolerance:
        move_directions["horizontal"] = "aller à droite"
        guidance_message += "Aller à droite. "
    else:
        move_directions["horizontal"] = "centré horizontalement"

    # Centrage vertical
    if top_percentage < 50 - tolerance:
        move_directions["vertical"] = "monter"
        guidance_message += "Monter. "
    elif bottom_percentage < 50 - tolerance:
        move_directions["vertical"] = "descendre"
        guidance_message += "Descendre. "
    else:
        move_directions["vertical"] = "centré verticalement"

    # Détection de distance pour savoir si la personne est trop proche ou trop loin
    face_width_in_frame = x2 - x1
    if face_width_in_frame < image_width * 0.1:  # Trop loin si la largeur du visage est inférieure à 10% de l'image
        move_directions["distance"] = "avancer"
        guidance_message += "Avancer. "
    elif face_width_in_frame > image_width * 0.4:  # Trop proche si la largeur du visage dépasse 40% de l'image
        move_directions["distance"] = "reculer"
        guidance_message += "Reculer. "
    else:
        move_directions["distance"] = "distance correcte"
    
    # Affichage des directions suggérées
    if guidance_message == "":
        guidance_message = "Le visage est correctement centré."

    # Retourner les pourcentages et les directions suggérées
    return {
        "percentages": {
            "left": left_percentage,
            "right": right_percentage,
            "top": top_percentage,
            "bottom": bottom_percentage
        },
        "movement_guidance": guidance_message,
        "directions": move_directions
    }