void setup(){
    pinMode(25, OUTPUT);
}
void loop(){
    digitalWrite(25, HIGH);
    delay(5000);
    digitalWrite(25, LOW);
    delay(5000);
}