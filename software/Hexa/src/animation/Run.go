package animation

import (
	"github.com/notpunchnox/Hexa/src/Servo"
	"gobot.io/x/gobot/drivers/i2c"
	"time"
)

func Run(pca *i2c.PCA9685Driver, steps []AnimationStep) {
	for _, step := range steps {
		Servo.Set(pca, step.Channel, step.Degrees)
		time.Sleep(step.Duration)
	}
}