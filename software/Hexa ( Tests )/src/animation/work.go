package animation

import (
    "fmt"
    "log"

    "gobot.io/x/gobot/drivers/i2c"
)

func Work(pca, pca2 *i2c.PCA9685Driver) {
    // Démarrer les deux cartes PCA9685
    if err := pca.Start(); err != nil {
        log.Fatalf("Impossible de démarrer pca : %v", err)
    }
    if err := pca2.Start(); err != nil {
        log.Fatalf("Impossible de démarrer pca2 : %v", err)
    }

    fmt.Println("Marche en cours...")

/*
    animation := []AnimationStep{
        {Channel: 11, Degrees: 70, Duration: 0},// coxa
        {Channel: 9, Degrees: 50, Duration: 0},// tibia
        {Channel: 10, Degrees: 60, Duration: 2000 * time.Millisecond}, // femur

        {Channel: 10, Degrees: 20, Duration: 0},// femur
        {Channel: 9, Degrees: 40, Duration: 500 * time.Millisecond}, // tibia
        {Channel: 11, Degrees: 110, Duration: 200 * time.Millisecond}, // coxa

        {Channel: 9, Degrees: 60, Duration: 0},// tibia
        {Channel: 10, Degrees: 60, Duration: 600 * time.Millisecond}, // femur
        {Channel: 11, Degrees: 70, Duration: 0},// coxa
        
    }

    animation2 := []AnimationStep{
        {Channel: 8, Degrees: 70, Duration: 0 * time.Second},// coxa
        {Channel: 14, Degrees: 50, Duration: 0},// tibia
        {Channel: 15, Degrees: 60, Duration: 2000 * time.Millisecond}, // femur

        
        {Channel: 15, Degrees: 100, Duration: 0 * time.Millisecond},// femur
        {Channel: 14, Degrees: 40, Duration: 500 * time.Millisecond}, // tibia
        {Channel: 8, Degrees: 110, Duration: 200 * time.Millisecond}, // coxa

        
        {Channel: 14, Degrees: 60, Duration: 0},// tibia
        {Channel: 15, Degrees: 60, Duration: 600 * time.Millisecond}, // femur
        {Channel: 8, Degrees: 70, Duration: 0},// coxa
        
    }
    Run(pca, animation)
    Run(pca2, animation2)
    // Utilisez des goroutines pour exécuter les animations en parallèle
    var wg sync.WaitGroup
    wg.Add(2)

    go func() {
        defer wg.Done()
        Run(pca, animation)
    }()

    go func() {
        defer wg.Done()
        Run(pca2, animation2)
    }()

    // Attendez la fin des animations
    wg.Wait()
    */
}