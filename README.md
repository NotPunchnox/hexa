# Hexa
*Hexa est un robot open source en phase de développement. Son objectif est d'aider dans l'initiation à la robotique. Il sera équipé d'un système d'intelligence artificielle pour la communication, la gestion des animations ( cinématique inversée ), l'adaptabilité à son environnement.*

### En cours de développement
- **Intelligence Artificielle ( `Langage Naturel`, `Gestion d'Algorithmes`: `cinématique inversée`/`matrice` )**: Actuellement je travail sur les animations du robot et l'intégration d'intelligence artificielles, J'utilise une version gratuite de Gemini qui est suffisante pour l'utilisation dont on a besoin, ceci nous permet d'entrainer le robot à comprendre comment fonctionne ses animations et algorithmes.

- **Animations ( `Walk`, `Up`, `Sleep`, `ChangeTop`, `InverseKinematic`: `rouli`... )**: Sur les animations je travail sur les animations de cinématique inversé et de mouvements de bases, le but est que l'IA puisse faire ses propres matrices de mouvements, pour avoir un rendu optimal. J'ai ajouté des matrices permettant de contrôller chaque pattes indépendament des autres, et un mouvement progressif entre chaque positions permettant de fluidifier les séquences animations avec de belles transitions.

**Exemple de conversation avec l'IA:**
![Image](./medias/IA_Chat.PNG)

**Exemple de cinématique inversée:** *En cours de développement*
<div>
    <img src="medias/TurnZ.gif" width="20%" height="50%"/>
    <img src="medias/Animation.gif" width="20%" height="50%"/>
</div>


<div>
    <img src="medias/robot.jpg" width="30%" height="50%"/>
    <img src="medias/robot_vu_de_face.jpg" width="50%" height="50%"/>
</div>


- Composition du robot: [Composants du robot](https://github.com/NotPunchnox/hexa/blob/main/hardware/Composants.md)
- Images du robot en 3D (autodesk Fusion360): [Images en 3D](https://github.com/NotPunchnox/hexa/blob/main/hardware/Hexa%203d.md)

## Fonctionnalités

- Conception économique et polyvalente
- Système d'intelligence artificielle pour la communication et l'adaptabilité
- Intégration d'une IA [Go to IA](https://github.com/NotPunchnox/hexa/blob/main/software/IA/)

## Contenu du dépôt

- **Documentation** : Contient la documentation du projet. => [Documentation](./Documentation/README.md)
- **Cours**: Explications sur les mathématiques utilisés dans le robot, tel que la cinématique inversée, programmation etc => [Cinématique inverse](./learn/README.md)
- **Hardware** : Contient l'assemblage, les composants, le câblage, etc. => [Documentation matérielle](./hardware/Composants.md)
- **Software** : 
    - **Arduino**: Contient Le code open source du Robot Hexa [Arduino](./software/Arduino/)
    - **IA**: Comprend le code source du fonctionnement d'apprentissage de Hexa ( IA de reproduction de langage naturel + contrôle de cinématique inversé à partir de Gemini-Flash-1.5).[Version Communication orale: Python](./software/IA/speaker/readme.md), [API de traitement d'instruction: NodeJs](./software/IA/API/)
    - **Simulateur**: Contient un simulateur pour le robot Hexa. [Simulateur](./software/simulator/README.md)
- Issues : Section pour signaler des bugs ou suggérer des améliorations.

## Licence

Ce projet est sous licence [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/deed.fr). Vous êtes autorisé à utiliser, partager, modifier et distribuer le projet à des fins non commerciales, en mentionnant l'auteur d'origine et en partageant toute modification sous la même licence.


## Collaborateurs

<img src="https://avatars.githubusercontent.com/u/126868056?v=4" alt="Trotiflex Avatar" width="10%" height="10%" style="border-radius: 50%;">

[Trotiflex](https://github.com/trotiflex) est une collaboratrice clé qui a apportée une contribution significative au projet. Principalement axé sur la cinématique inversée, Trotiflex a joué un rôle essentiel dans l'écriture de cours détaillés sur ce sujet. [Cours](./learn/maths/cinématique_inversée.md).
