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

    pca2 := i2c.NewPCA9685Driver(r,
        i2c.WithBus(1),
        i2c.WithAddress(0x41))

        /*
    work := func() {
        animation.Work(pca, pca2)
    }
    */
    sleep := func() {
        animation.Sleep(pca, pca2)
    }
    /*
    test := func() {
        animation.Test(pca)
    }*/

    robot := gobot.NewRobot("robot",
        []gobot.Connection{r},
        []gobot.Device{pca, pca2}, // Ajoutez pca2 aux périphériques
        //work,
        sleep,
        //test,
    )

    robot.Start()
}
