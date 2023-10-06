package animation

import "time"

type AnimationStep struct {
	Channel   int       // Le canal du servomoteur
	Degrees   float64   // L'angle en degrés
	Duration  time.Duration // La durée pendant laquelle l'angle doit être atteint
}