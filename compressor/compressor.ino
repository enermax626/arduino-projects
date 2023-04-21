
#define PIN_COMPRESSOR 2

void setup() {
  pinMode(PIN_COMPRESSOR, OUTPUT);
  digitalWrite(PIN_COMPRESSOR, LOW);

}

void loop() {
    delay(3000);

    digitalWrite(PIN_COMPRESSOR, HIGH);

    delay(3000);




}
