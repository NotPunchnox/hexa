package main

import (
	"fmt"
	"log"
	"time" // Ajout de l'importation du package time

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
		i2c.WithAddress(0x41))

	work := func() {
		// Démarrez le pilote PCA9685
		if err := pca.Start(); err != nil {
			log.Fatalf("Impossible de démarrer pca : %v", err)
		}

		// Mettez le servo dans une position initiale
		pca.SetPWM(3, 0, 400)
		time.Sleep(2000 * time.Millisecond)
		pca.SetPWM(3, 0, 200)
		time.Sleep(2000 * time.Millisecond)
		pca.SetPWM(3, 0, 350)
	}

	robot := gobot.NewRobot("robot",
		[]gobot.Connection{r},
		[]gobot.Device{pca},
		work,
	)

	robot.Start()
}
