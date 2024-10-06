package main

import (
	"fmt"
	"math"
)

func rtd(a float64) float64 { //Radians to degrées
	return a * (180 / math.Pi)
}
func AlKashi(a, b, c float64) float64 {
	return math.Acos((a*a + b*b - c*c) / (2 * a * b))
}

// Algo de cinématique inversée du robot Aash
func reverseKinematics(z, coxa, femur, rouli, body, tibia, extension, y float64, gauche bool) (float64, float64, float64, error) {

	var zLeg float64 // float64 = nombre à virgule
	/*
		si le nombre est 13.5 c'est float64
		si le nombre est 13 c'est int
	*/

	if rouli == 0 { // Rouli signifie l'inclinaison du corp du robot, 0 = aucune inclinaison
		zLeg = z
	} else if gauche == true { // Choisir un calcul en fonction de l'emplacement de la patte
		zLeg = z + rouli
	} else if gauche == false {
		zLeg = z - rouli
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

	return AngleFemur, 180 - rtd(AngleTibia), rtd(AngleCoxa), nil
}

func main() {

	z := 8.0  //hauteur
	x := 12.0 //distance entre le bout de la patte et le coxa
	y := 9.0
	rouli := 0.0

	body := 10.0
	coxa := 5.0
	femur := 6.0
	tibia := 13.5

	gauche := true

	AngleFemur, AngleTibia, AngleCoxa, err := reverseKinematics(z, coxa, femur, rouli, body, tibia, x, y, gauche)
	if err != nil {
		fmt.Println("Erreur:", err)
		return
	}

	// Affichage des résultats
	fmt.Printf("Angle du Femur: %.2f °\n", AngleFemur)
	fmt.Printf("Angle du Tibia (theta2): %.2f °\n", AngleTibia)
	fmt.Printf("Angle du Coxa: %.2f cm\n", AngleCoxa)
}
