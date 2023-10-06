package main

import (
	"fmt"

	"gobot.io/x/gobot"
	"gobot.io/x/gobot/drivers/i2c"
	"gobot.io/x/gobot/platforms/raspi"

	"github.com/notpunchnox/Hexa/src/animation"
)

func main() {
	fmt.Println("Lancement du programme...")

	r := raspi.NewAdaptor()

	pca := i2c.NewPCA9685Driver(r,
		i2c.WithBus(1),
		i2c.WithAddress(0x40))


	work := func() {
		animation.Work(pca)
	}

	robot := gobot.NewRobot("robot",
		[]gobot.Connection{r},
		[]gobot.Device{pca},
		work,
	)

	robot.Start()
}
