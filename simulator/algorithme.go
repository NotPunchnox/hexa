package main

import (
	"flag"
	"fmt"
	"math"
)

var (
	x float64
	y float64
	z float64
)

func init() {
	flag.Float64Var(&x, "x", 0.0, "Coordonnée x")
	flag.Float64Var(&y, "y", 0.0, "Coordonnée y (hauteur)")
	flag.Float64Var(&z, "z", 0.0, "Coordonnée z (rouli)")
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

// Algo de cinématique inversée du robot Hexa
func reverseKinematics(hauteur, coxa, femur, rouli, body, tibia, extension float64, gauche bool) (float64, float64, float64, error) {

	var hauteurLeg float64 // float64 = nombre à virgule
	/*
		si le nombre est 13.5 c'est float64
		si le nombre est 13 c'est int
	*/

	if rouli == 0 { // Rouli signifie l'inclinaison du corp du robot, 0 = aucune inclinaison
		hauteurLeg = hauteur
	} else if gauche == true { // Choisir un calcul en fonction de l'emplacement de la patte
		hauteurLeg = hauteur - rouli
	} else if gauche == false {
		hauteurLeg = hauteur + rouli
	}

	// Calcul de l'angle du triangle réctangle permettant de calculer l'hypotenus de TB
	hypotenuse := math.Sqrt(hauteurLeg*hauteurLeg + extension*extension)

	//Calcul de l'angle A' du triangle rectangle permettant de calculer l'hypotenus de TB
	numerator := hypotenuse*hypotenuse + hauteurLeg*hauteurLeg - extension*extension
	denominator := 2 * hypotenuse * hauteurLeg
	AngleTA := math.Acos(numerator / denominator)

	//Calcul de l'angle A de TB permettant de calculer l'angle du femur
	numeratorTBa := hypotenuse*hypotenuse + femur*femur - tibia*tibia
	denominatorTBa := 2 * hypotenuse * femur
	AngleTBa := math.Acos(numeratorTBa / denominatorTBa)

	//Calcul de l'angle B de TB permettant de calculer l'angle du tibia
	numeratorTBb := tibia*tibia + femur*femur - hypotenuse*hypotenuse
	denominatorTBb := 2 * tibia * femur
	AngleTibia := math.Acos(numeratorTBb / denominatorTBb)

	hyp := (body / 2) + coxa
	r := math.Sqrt((hyp * hyp) - (rouli * rouli))

	//Calcul de l'angle formé au centre du robot selon son inclinaison
	numerator2 := r*r + hyp*hyp - rouli*rouli
	denominator2 := 2 * r * hyp

	AngleRouli := math.Acos(numerator2 / denominator2)

	a := rtd(AngleTBa)
	b := rtd(AngleTA)
	c := rtd(AngleRouli)
	//angleModifie := sens(c, rouli)
	AngleFemur := (180 - (a + b))
	//fmt.Println("sens:", sens(c, rouli))
	AngleTibia = 180 - rtd(AngleTibia)

	return AngleFemur, AngleTibia, c, nil
}

func main() {
	flag.Parse()

	hauteur := y
	extensionLeg := x //distance entre le bout de la patte et le coxa
	rouli := z

	body := 10.0
	coxa := 5.0
	femur := 6.0
	tibia := 13.5

	gauche := false

	femurAngle, tibiaAngle, AngleRouli, err := reverseKinematics(hauteur, coxa, femur, rouli, body, tibia, extensionLeg, gauche)
	if err != nil {
		fmt.Println("Erreur:", err)
		return
	}

	// Affichage des résultats
	fmt.Println("femur:", femurAngle, "\ntibia:", tibiaAngle, "\nrouli", AngleRouli)
}
