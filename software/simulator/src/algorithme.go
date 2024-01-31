package main

import (
	"flag"
	"fmt"
	"math"
)

var (
	x          float64
	y          float64
	z          float64
	deltaRouli float64
)

func init() {
	flag.Float64Var(&x, "x", 0.0, "Coordonnée x")
	flag.Float64Var(&y, "z", 0.0, "Coordonnée z (hauteur)")
	flag.Float64Var(&deltaRouli, "y", 0.0, "Coordonnée y")
	flag.Float64Var(&deltaRouli, "deltaRouli", 0.0, "Coordonnée deltaRouli (rouli)")
}

func rtd(a float64) float64 {
	return a * (180 / math.Pi)
}

func sens(valeurAngle, commande float64) float64 {
	if commande == 0 {
		return 0
	}

	// Utilisation de math.Abs pour obtenir la valeur absolue de la commande
	absCommande := math.Abs(commande)

	// Calcul de la valeur modifiée de l'angle
	valeurModifiee := valeurAngle * absCommande

	return valeurModifiee
}
func AlKashi(a, b, c float64) float64 {
	return math.Acos((a*a + b*b - c*c) / (2 * a * b))
}
func reverseKinematics(deltaRouli, coxa, femur, rouli, body, tibia, extension, y float64, gauche bool) (float64, float64, float64, float64, error) {

	var zLeg float64 // float64 = nombre à virgule
	/*
		si le nombre est 13.5 c'est float64
		si le nombre est 13 c'est int
	*/

	if rouli == 0 { // Rouli signifie l'inclinaison du corp du robot, 0 = aucune inclinaison
		zLeg = deltaRouli
	} else if gauche == true { // Choisir un calcul en fonction de l'emplacement de la patte
		zLeg = deltaRouli + rouli
	} else if gauche == false {
		zLeg = deltaRouli - rouli
	}

	// Calcul de l'angle du triangle réctangle permettant de calculer l'hypotenus de TB
	hypotenuse := math.Sqrt(zLeg*zLeg + extension*extension)
	hypotenuseY := math.Sqrt(extension*extension + y*y)

	//Calcul de l'angle A' du triangle rectangle permettant de calculer l'hypotenus de TB
	AngleTA := AlKashi(hypotenuse, zLeg, extension)

	//Calcul de l'angle A de TB permettant de calculer l'angle du femur
	AngleTBa := AlKashi(hypotenuse, femur, tibia)

	//Calcul de l'angle B de TB permettant de calculer l'angle du tibia
	AngleTibia := AlKashi(tibia, femur, hypotenuse)

	hyp := (body / 2) + coxa
	r := math.Sqrt((hyp * hyp) - (rouli * rouli))

	//Calcul de l'angle formé au centre du robot selon son inclinaison
	AngleRouli := AlKashi(r, hyp, rouli)

	a := rtd(AngleTBa)
	b := rtd(AngleTA)
	c := rtd(AngleRouli)
	AngleFemur := 180 - (a + b + c)
	AngleCoxa := AlKashi(hypotenuseY, extension, y)

	return AngleFemur, 180 - rtd(AngleTibia), c, rtd(AngleCoxa), nil
}

func main() {
	flag.Parse()

	hauteur := y
	extensionLeg := x //distance entre le bout de la patte et le coxa
	rouli := deltaRouli
	fmt.Println(rouli)

	body := 10.0
	coxa := 5.0
	femur := 6.0
	tibia := 13.5

	femurAngle, tibiaAngle, AngleRouli, AnglaCoxa, err := reverseKinematics(hauteur, coxa, femur, rouli, body, tibia, extensionLeg, y, false)
	if err != nil {
		fmt.Println("Erreur:", err)
		return
	}
	femurAngle2, tibiaAngle2, AngleRouli2, AnglaCoxa, err := reverseKinematics(hauteur, coxa, femur, rouli, body, tibia, extensionLeg, y, true)
	if err != nil {
		fmt.Println("Erreur:", err)
		return
	}

	// Affichage des résultats
	fmt.Println("femur:", femurAngle, "\ntibia:", tibiaAngle, "\nrouli", AngleRouli, "\ncoxa:", AnglaCoxa)
	fmt.Println("femur2:", femurAngle2, "\ntibia2:", tibiaAngle2, "\nrouli2", AngleRouli2, "\ncoxa2:", AnglaCoxa)

}
