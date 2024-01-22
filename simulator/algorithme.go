package main

import (
	"flag"
	"fmt"
	"math"
)

var (
	x float64
	y float64
)

func init() {
	flag.Float64Var(&x, "x", 0.0, "Coordonnée x")
	flag.Float64Var(&y, "y", 0.0, "Coordonnée y (hauteur)")
}

func rtd(a float64) float64 {
	return a * (180 / math.Pi)
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
		hauteurLeg = hauteur + rouli
	} else if gauche == false {
		hauteurLeg = hauteur - rouli
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
	fmt.Println(a, b, c)
	AngleFemur := 180 - (a + b + c)

	return AngleFemur, 180 - rtd(AngleTibia), hypotenuse, nil
}

func main() {

	fmt.Println(x, y)

	hauteur := y
	extensionLeg := x //distance entre le bout de la patte et le coxa
	rouli := 0.0

	body := 10.0
	coxa := 5.0
	femur := 6.0
	tibia := 13.5

	gauche := true

	res, femurangle, hypotenuse, err := reverseKinematics(hauteur, coxa, femur, rouli, body, tibia, extensionLeg, gauche)
	if err != nil {
		fmt.Println("Erreur:", err)
		return
	}

	// Affichage des résultats
	fmt.Printf("Angle du Femur: %.2f °\n", res)
	fmt.Printf("Angle du Tibia (theta2): %.2f °\n", femurangle)
	fmt.Printf("Longueur de l'hypotenuse: %.2f cm\n", hypotenuse)
}
