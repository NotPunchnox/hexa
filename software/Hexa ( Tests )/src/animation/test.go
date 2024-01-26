package animation

import (
    "fmt"
    "log"

    "gobot.io/x/gobot/drivers/i2c"
)

func Test(pca *i2c.PCA9685Driver) {
    if err := pca.Start(); err != nil {
        log.Fatalf("Impossible de démarrer pca : %v", err)
    }

    fmt.Println("Animation Test en cours...")
/*

    animation := []AnimationStep{
        {Channel: 6, Degrees: 50, Duration: 500 * time.Millisecond}, // femur
        {Channel: 6, Degrees: 100, Duration: 500 * time.Millisecond}, // femur
    }
    Run(pca, animation)
    */
}