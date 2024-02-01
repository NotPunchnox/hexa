import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

def draw_leg(ax, start_point, angles, lengths, color='blue', label=''):
    """
    Dessine une patte de robot en 3D.

    Paramètres :
    - ax : L'objet Axes3D sur lequel dessiner.
    - start_point : Les coordonnées du point de départ (tuple de 3 éléments : x, y, z).
    - angles : Les angles de rotation pour chaque segment de la patte (tuple de 3 éléments : angle1, angle2, angle3).
    - lengths : Les longueurs de chaque segment de la patte (tuple de 3 éléments : longueur1, longueur2, longueur3).
    - color : Couleur du trait (par défaut, 'blue').
    - label : Étiquette pour la patte (par défaut, '').
    """
    x, y, z = start_point

    # Calcul des coordonnées finales de chaque segment en fonction des angles et des longueurs
    end_x = x + lengths[0] * np.cos(np.radians(angles[0]))
    end_y = y + lengths[0] * np.sin(np.radians(angles[0]))
    end_z = z

    mid_x = end_x + lengths[1] * np.cos(np.radians(angles[1]))
    mid_y = end_y + lengths[1] * np.sin(np.radians(angles[1]))
    mid_z = end_z

    final_x = mid_x + lengths[2] * np.cos(np.radians(angles[2]))
    final_y = mid_y + lengths[2] * np.sin(np.radians(angles[2]))
    final_z = mid_z

    # Dessin des segments de la patte
    ax.quiver(x, y, z, end_x - x, end_y - y, end_z - z, color=color, label=label, arrow_length_ratio=0.1)
    ax.quiver(end_x, end_y, end_z, mid_x - end_x, mid_z - end_z, mid_y - end_y, color=color, arrow_length_ratio=0.1)
    #ax.quiver(end_x, end_y, end_z, final_x - mid_x, final_z - mid_z, final_y - mid_y, color=color, arrow_length_ratio=0.1)
    ax.quiver(mid_x, mid_z, mid_y, final_x - mid_x, final_z - mid_z, final_y - mid_y, color=color, arrow_length_ratio=0.1)

    #ax.quiver(mid_x, mid_y, mid_z, final_x - mid_x, final_y - mid_y, final_z - mid_z, color=color, arrow_length_ratio=0.1)

# Création de la figure et de l'axe 3D
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Définir les paramètres de la patte (exemples)
start_point = (10, 10, 10)
angles = (0, 70, -70)
lengths = (5, 6, 13.5)

# Dessiner la patte
draw_leg(ax, start_point, angles, lengths, color='blue', label='Patte 1')

# Réglages des limites de l'axe
ax.set_xlim([0, 20])
ax.set_ylim([0, 20])
ax.set_zlim([0, 20])

# Afficher la légende
ax.legend()

# Afficher le graphique
plt.show()
