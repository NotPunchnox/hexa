package Conversion

import "fmt"

func Degrees(degrees float64) uint16 {
	// Assurez-vous que les degrés sont dans la plage 0-180
	if degrees < 0 {
		degrees = 0
	} else if degrees > 180 {
		degrees = 180
	}

	pulseWidth := uint16(150 + (degrees/180.0)*450)
	fmt.Println(pulseWidth)
	return pulseWidth
}
