package animation

import (
	"time"
	"gobot.io/x/gobot/drivers/i2c"
)

type AnimationStep struct {
	Channel   int       // Le canal du servomoteur
	Degrees   float64   // L'angle en degrés
	Pca       *i2c.PCA9685Driver
	Duration  time.Duration // La durée pendant laquelle l'angle doit être atteint
}