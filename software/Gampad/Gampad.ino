#define VRX_PIN  A0
#define VRY_PIN  A1

#define VRX_PIN2  A2
#define VRY_PIN2  A3

int xValue = 0;
int yValue = 0;
int xValue2 = 0;
int yValue2 = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  xValue2 = analogRead(VRX_PIN2);
  yValue2 = analogRead(VRY_PIN2);

  Serial.print("Joystick 1 - X = ");
  Serial.print(xValue);
  Serial.print(", Y = ");
  Serial.println(yValue);

  Serial.print("Joystick 2 - X = ");
  Serial.print(xValue2);
  Serial.print(", Y = ");
  Serial.println(yValue2);

  delay(50);
}
