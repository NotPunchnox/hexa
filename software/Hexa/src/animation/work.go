package animation

import (
    "fmt"
    "log"
    "time"

    "gobot.io/x/gobot/drivers/i2c"
)

func Work(pca *i2c.PCA9685Driver) {
    if err := pca.Start(); err != nil {
        log.Fatalf("Impossible de démarrer pca : %v", err)
    }

    fmt.Println("work en cours")

    // Définissez votre animation en utilisant des étapes
    animation := []AnimationStep{
        {Channel: 10, Degrees: 30, Duration: 0 * time.Millisecond},         // coxa
        {Channel: 9, Degrees: 60, Duration: 0},          // tibia
        {Channel: 10, Degrees: 60, Duration: 600 * time.Millisecond}, // femur

        {Channel: 10, Degrees: 20, Duration: 0},         // femur
        {Channel: 9, Degrees: 40, Duration: 500 * time.Millisecond}, // tibia
        {Channel: 11, Degrees: 110, Duration: 200 * time.Millisecond}, // coxa

        {Channel: 9, Degrees: 60, Duration: 0},          // tibia
        {Channel: 10, Degrees: 60, Duration: 600 * time.Millisecond}, // femur
        {Channel: 11, Degrees: 70, Duration: 0},         // coxa
    }

    Run(pca, animation)
}
