# Cours de Cinématique Inversée sur Robot Hexapode

## Introduction
La cinématique inversée est une méthode utilisée en robotique pour déterminer les angles des articulations nécessaires pour placer un effecteur final (comme le pied d'une patte de robot) à une position donnée. Ce cours se concentrera sur l'application de la cinématique inversée à un robot hexapode, un robot à six pattes, avec une attention particulière aux articulations : coxa, fémur et tibia.

## Terminologie
Avant de plonger dans les détails de la cinématique inversée, il est important de comprendre les termes clés :

- **Coxa** : La première articulation de la patte, connectée au corps du robot. Elle permet principalement le mouvement horizontal de la patte.
- **Fémur** : La deuxième section de la patte, connectée à la coxa. Elle permet le mouvement vertical de la patte.
- **Tibia** : La troisième section de la patte, connectée au fémur. Elle permet également le mouvement vertical de la patte, complétant la portée de la patte.

## Modèle de Patte d'un Robot Hexapode
Chaque patte d'un robot hexapode est modélisée avec trois articulations :
1. **Coxa (Cx)** : Rotation horizontale.
2. **Fémur (Fm)** : Rotation verticale.
3. **Tibia (Tb)** : Rotation verticale.

Ces articulations permettent de déplacer l'extrémité de la patte dans un espace tridimensionnel.

## Cinématique Inversée
La cinématique inversée consiste à trouver les angles des articulations (coxa, fémur, tibia) qui positionneront l'extrémité de la patte (le pied) à une position cible dans l'espace.

### Étapes de Calcul
1. **Définir les Longueurs des Segments**
   - \( L1 \) : Longueur de la coxa
   - \( L2 \) : Longueur du fémur
   - \( L3 \) : Longueur du tibia

2. **Position Cible**
   - \( (X, Y, Z) \) : Coordonnées cibles de l'extrémité de la patte dans le repère du robot.

3. **Calculer les Angles**
   - **Angle de la Coxa (\( \theta_1 \))**
     - \[ \theta_1 = \arctan2(Y, X) \]
   - **Distance Horizontale (\( D_h \))**
     - \[ D_h = \sqrt{X^2 + Y^2} - L1 \]
   - **Distance Verticale (\( D_v \))**
     - \[ D_v = Z \]
   - **Distance Totale (\( D \))**
     - \[ D = \sqrt{D_h^2 + D_v^2} \]
   - **Angle du Fémur (\( \theta_2 \))**
     - \[ \theta_2 = \arccos\left(\frac{D^2 + L2^2 - L3^2}{2 \cdot D \cdot L2}\right) + \arctan2(D_v, D_h) \]
   - **Angle du Tibia (\( \theta_3 \))**
     - \[ \theta_3 = \arccos\left(\frac{L2^2 + L3^2 - D^2}{2 \cdot L2 \cdot L3}\right) \]

### Exemple de Calcul
Supposons que nous ayons les longueurs suivantes pour les segments de la patte :
- \( L1 = 2 \) unités
- \( L2 = 5 \) unités
- \( L3 = 7 \) unités

Et nous voulons positionner l'extrémité de la patte à :
- \( X = 6 \)
- \( Y = 3 \)
- \( Z = 4 \)

Les étapes de calcul seraient les suivantes :
1. Calcul de \( \theta_1 \)
   - \( \theta_1 = \arctan2(3, 6) = 0.464 \) radians

2. Calcul de \( D_h \)
   - \( D_h = \sqrt{6^2 + 3^2} - 2 = \sqrt{45} - 2 = 6.708 - 2 = 4.708 \)

3. Calcul de \( D_v \)
   - \( D_v = 4 \)

4. Calcul de \( D \)
   - \( D = \sqrt{4.708^2 + 4^2} = \sqrt{22.169 + 16} = \sqrt{38.169} = 6.179 \)

5. Calcul de \( \theta_2 \)
   - \( \theta_2 = \arccos\left(\frac{6.179^2 + 5^2 - 7^2}{2 \cdot 6.179 \cdot 5}\right) + \arctan2(4, 4.708) = 1.286 \) radians

6. Calcul de \( \theta_3 \)
   - \( \theta_3 = \arccos\left(\frac{5^2 + 7^2 - 6.179^2}{2 \cdot 5 \cdot 7}\right) = 0.906 \) radians

## Conclusion
La cinématique inversée est essentielle pour contrôler les robots hexapodes et leur permettre d'atteindre des positions spécifiques avec précision. En utilisant les formules et les étapes de calcul décrites ci-dessus, il est possible de déterminer les angles des articulations nécessaires pour déplacer les pattes du robot hexapode à des positions cibles dans l'espace.

## Ressources Supplémentaires
- **Livres** :
  - "Introduction to Robotics: Mechanics and Control" de John J. Craig
  - "Robotics: Modelling, Planning and Control" de Bruno Siciliano et al.
- **Sites Web** :
  - [IEEE Robotics and Automation Society](https://www.ieee-ras.org/)
  - [Robotics Stack Exchange](https://robotics.stackexchange.com/)
- **Tutoriels Vidéo** :
  - "Inverse Kinematics for Hexapod Robots" (YouTube)
  - "Robot Leg Inverse Kinematics" (Coursera)

---

Ce document offre un aperçu de la cinématique inversée appliquée aux robots hexapodes, avec des explications détaillées sur les calculs nécessaires pour déterminer les angles des articulations.