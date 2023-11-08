package animation

import (
    "fmt"
    "log"
    "time"

    "gobot.io/x/gobot/drivers/i2c"
)

func Sleep(p1, p2 *i2c.PCA9685Driver) {
    // Démarrer les deux cartes PCA9685
    if err := p1.Start(); err != nil {
        log.Fatalf("Impossible de démarrer pca : %v", err)
    } else {
        fmt.Println("PCA8695 n°1 est prêt à être utilisé")
    }
    if err := p2.Start(); err != nil {
        log.Fatalf("Impossible de démarrer pca2 : %v", err)
    }

    fmt.Println("Marche en cours...")

    animation := []AnimationStep{
        //LeftFront
        {Pca: p2, Channel: 3, Degrees: 50, Duration: 0 * time.Millisecond},// coxa
        {Pca: p1, Channel: 9, Degrees: 90, Duration: 500 * time.Millisecond}, // tibia
        {Pca: p1, Channel: 9, Degrees: 0, Duration: 10 * time.Millisecond}, // tibia
        {Pca: p1, Channel: 8, Degrees: 0, Duration: 0 * time.Millisecond}, // tibia

        /*
        //LeftMiddle
        //{Channel: 11, Degrees: 70, Duration: 0},// coxa
        {Channel: 10, Degrees: 0, Duration: 0 * time.Millisecond}, // femur
        {Channel: 9, Degrees: 0, Duration: 0},// tibia
        //LeftBack
        //{Channel: 15, Degrees: 70, Duration: 0},// coxa
        {Channel: 14, Degrees: 0, Duration: 0 * time.Millisecond}, // femur
        {Channel: 13, Degrees: 0, Duration: 0 * time.Millisecond}, // tibia
        {Channel: 1, Degrees: 80, Duration: 0 * time.Millisecond},// femur
        */
    }
    

    Run(animation)
}