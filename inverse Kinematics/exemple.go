package main

import (
	"fmt"
	"math"
)

func rtd(a float64) float64 {
	return a * (180 / math.Pi)
}

func inverseKinematics(hauteur, coxa, femur int, tibia float64, extension float64) (float64, float64, float64, error) {

	hypotenuse := math.Sqrt(float64(hauteur*hauteur) + extension*extension)

	// Calcul de l'angle du triangle réctangle permettant de calculer l'hypotenus de TB
	numerator := hypotenuse*hypotenuse + float64(hauteur*hauteur) - extension*extension
	denominator := 2 * hypotenuse * float64(hauteur)
	AngleTA := math.Acos(numerator / denominator)

	//Calcul de l'angle A du triangle formé par le femur et le tibia
	numeratorTBa := hypotenuse*hypotenuse + float64(femur*femur) - tibia*tibia
	denominatorTBa := 2 * hypotenuse * float64(femur)
	AngleTBa := math.Acos(numeratorTBa / denominatorTBa)

	//Calcul de l'angle B du triangle formé par le femur et le tibia
	numeratorTBb := tibia*tibia + float64(femur*femur) - hypotenuse*hypotenuse
	denominatorTBb := 2 * tibia * float64(femur)
	AngleTBb := math.Acos(numeratorTBb / denominatorTBb)

	a := rtd(AngleTBa)
	b := rtd(AngleTA)
	res := 360 - (a + 90 + b)

	return res, rtd(AngleTBb), hypotenuse, nil
}

func main() {

	hauteur := 8 //distance entre le bout de la patte et le corp du robot
	extensionLeg := 15.0

	coxa := 5
	femur := 6
	tibia := 13.5

	res, femurangle, hypotenuse, err := inverseKinematics(hauteur, coxa, femur, tibia, extensionLeg)
	if err != nil {
		fmt.Println("Erreur:", err)
		return
	}

	// Affichage des résultats
	fmt.Printf("Angle du Femur: %.2f °\n", res)
	fmt.Printf("Angle du Tibia (theta2): %.2f °\n", femurangle)
	fmt.Printf("Longueur de l'hypotenuse: %.2f cm\n", hypotenuse)
}
