package Servo

import (
	"gobot.io/x/gobot/drivers/i2c"
	"github.com/notpunchnox/Hexa/src/Conversion"
	"fmt"
)

func Set(pca *i2c.PCA9685Driver, channel int, degrees float64) {
	pulseWidth := Conversion.Degrees(degrees) // Convertir l'angle en largeur d'impulsion
	fmt.Println(channel, pulseWidth)
	pca.SetPWM(channel, 0, pulseWidth)
}