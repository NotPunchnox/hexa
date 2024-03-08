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
	flag.Float64Var(&z, "z", 0.0, "Coordonnée z (hauteur)")
	flag.Float64Var(&y, "y", 0.0, "Coordonnée y")
	flag.Float64Var(&deltaRouli, "deltaRouli", 0.0, "Coordonnée deltaRouli (rouli)")
}

func rtd(a float64) float64 { //Radians to degrées
	return a * (180 / math.Pi)
}
func AlKashi(a, b, c float64) float64 {
	return rtd(math.Acos((a*a + b*b - c*c) / (2 * a * b)))
}
func Racine(a, b float64) float64 {
	return math.Sqrt(a*a + b*b)
}

// Algo de cinématique inversée du robot Hexa
func reverseKinematics(z, coxa, femur, rouli, body, tibia, x, y float64, gauche bool) (float64, float64, float64, float64, error) {

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

	//fmt.Println("TPatte & hypotenuse:", Tpatte-coxa, hypotenuse, "hauteur:", math.Sqrt(math.Pow(hypotenuse, 2)-math.Pow(Tpatte-coxa, 2)))

	A1 := rtd(math.Atan((Tpatte - coxa) / z))
	A2 := AlKashi(femur, hypotenuse, tibia)
	AngleTibia := AlKashi(tibia, femur, hypotenuse)

	AngleCoxa := rtd(math.Atan(y/x)) + 90

	hyp := (body / 2) + coxa
	r := Racine(hyp, rouli)

	//Calcul de l'angle formé au centre du robot selon son inclinaison
	AngleRouli := AlKashi(r, hyp, rouli)
	AngleFemur := 180 - (A2 + A1 + AngleRouli)
	fmt.Println(A2+A1, AngleRouli)

	return AngleFemur, 180 - AngleTibia, AngleCoxa, AngleRouli, nil
}

func main() {
	flag.Parse()

	body := 10.0
	coxa := 5.0
	femur := 6.0
	tibia := 13.5

	femurAngle, tibiaAngle, AngleCoxa, AngleRouli, err := reverseKinematics(z, coxa, femur, deltaRouli, body, tibia, x, y, false)
	if err != nil {
		fmt.Println("Erreur:", err)
		return
	}
	femurAngle2, tibiaAngle2, AngleCoxa2, AngleRouli2, err := reverseKinematics(z, coxa, femur, deltaRouli, body, tibia, x, y, true)
	if err != nil {
		fmt.Println("Erreur:", err)
		return
	}

	// Affichage des résultats
	fmt.Println("femur:", femurAngle, "\ntibia:", tibiaAngle, "\nrouli", AngleRouli, "\ncoxa:", AngleCoxa)
	fmt.Println("femur2:", femurAngle2, "\ntibia2:", tibiaAngle2, "\nrouli2", AngleRouli2, "\ncoxa2:", AngleCoxa2)

}
