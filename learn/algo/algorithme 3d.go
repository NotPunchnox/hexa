package main

import (
	"fmt"
	"math"
)

func rtd(a float64) float64 { //Radians to degrées
	return a * (180 / math.Pi)
}
func AlKashi(a, b, c float64) float64 {
	return rtd(math.Acos((a*a + b*b - c*c) / (2 * a * b)))
}
func Racine(a, b float64) float64 {
	return math.Sqrt(a*a + b*b)
}

// Algo de cinématique inversée du robot Aash
func reverseKinematics(z, coxa, femur, rouli, body, tibia, x, y float64, gauche bool) (float64, float64, float64, error) {

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

	Tpatte := Racine(y, x) //Taille de la patte
	hypotenuse := Racine(zLeg, Tpatte-coxa)

	fmt.Println("TPatte & hypotenuse:", Tpatte-coxa, hypotenuse, "hauteur:", math.Sqrt(math.Pow(hypotenuse, 2)-math.Pow(Tpatte-coxa, 2)))

	A1 := rtd(math.Atan((Tpatte - coxa) / z))
	A2 := AlKashi(femur, hypotenuse, tibia)
	AngleTibia := AlKashi(tibia, femur, hypotenuse)

	AngleCoxa := rtd(math.Atan(y / x))

	hyp := (body / 2) + coxa
	r := Racine(hyp, rouli)

	//Calcul de l'angle formé au centre du robot selon son inclinaison
	AngleRouli := AlKashi(r, hyp, rouli)
	AngleFemur := 180 - (A2 + A1 + AngleRouli)
	fmt.Println(A2+A1, AngleRouli)

	return AngleFemur, 180 - AngleTibia, AngleCoxa, nil
}

func main() {

	z := 9.0
	x := 10.0 //distance entre le bout de la patte et le coxa
	y := 0.0
	rouli := 0.0

	body := 10.0
	coxa := 5.0
	femur := 6.0
	tibia := 13.5

	gauche := true

	res, femurangle, AngleCoxa, err := reverseKinematics(z, coxa, femur, rouli, body, tibia, x, y, gauche)
	if err != nil {
		fmt.Println("Erreur:", err)
		return
	}

	// Affichage des résultats
	fmt.Printf("Angle du Femur: %.2f °\n", res)
	fmt.Printf("Angle du Tibia: %.2f °\n", femurangle)
	fmt.Printf("Angle du Coxa: %.2f °\n", 90+AngleCoxa)
}
