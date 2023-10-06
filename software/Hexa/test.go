package main

import (
	"fmt"
	"log"
	"time"

	"gobot.io/x/gobot"
	"gobot.io/x/gobot/drivers/i2c"
	"gobot.io/x/gobot/platforms/raspi"
)

func main() {
	fmt.Println("Lancement du programme...")

	r := raspi.NewAdaptor()

	// Créez un pilote PCA9685
	pca := i2c.NewPCA9685Driver(r,
		i2c.WithBus(1),
		i2c.WithAddress(0x40))

	work := func() {
		// Démarrez le pilote PCA9685
		if err := pca.Start(); err != nil {
			log.Fatalf("Impossible de démarrer pca : %v", err)
		}

		// Définissez le canal du servo que vous souhaitez contrôler
		servoChannel := 5 // Par exemple, utilisez le canal 0

		// Déplacez le servo à 0 degrés (pouvez varier selon votre servo)
		pca.SetPWM(servoChannel, 0, 150)

		// Attendez un moment
		time.Sleep(2 * time.Second)

		// Déplacez le servo à 90 degrés (pouvez varier selon votre servo)
		pca.SetPWM(servoChannel, 0, 375)

		// Attendez à nouveau
		time.Sleep(2 * time.Second)

		// Déplacez le servo à 180 degrés (pouvez varier selon votre servo)
		pca.SetPWM(servoChannel, 0, 600)

		// Attendez encore un moment
		time.Sleep(2 * time.Second)

		// Assurez-vous de mettre les valeurs PWM correctes pour votre servo
		// Les valeurs 150, 375 et 600 sont des exemples et peuvent varier selon le servo.

		// Vous pouvez également utiliser des angles au lieu des valeurs PWM si votre servo le permet.
		// Par exemple, si votre servo accepte des angles entre 0 et 180 degrés :
		// angle := 90 // Réglez l'angle souhaité
		// pulseWidth := uint16(angle * 11) // Convertissez l'angle en largeur d'impulsion PWM
		// pca.SetPWM(servoChannel, 0, pulseWidth)
	}

	robot := gobot.NewRobot("robot",
		[]gobot.Connection{r},
		[]gobot.Device{pca},
		work,
	)

	robot.Start()
}
