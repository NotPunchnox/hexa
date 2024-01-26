package animation

import (
	"github.com/notpunchnox/Hexa/src/Conversion"
	"time"
)

func Run(steps []AnimationStep) {
	for _, step := range steps {
		degrees := Conversion.Degrees(step.Degrees)

		step.Pca.SetPWM(step.Channel, 0, degrees)
		time.Sleep(step.Duration)
	}
}