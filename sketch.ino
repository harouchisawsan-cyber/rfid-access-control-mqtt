#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_PIN 7 // Votre LED rouge
#define SERVO_PIN 6

MFRC522 rfid(SS_PIN, RST_PIN);
Servo monServo;

// ID par défaut d'une carte Wokwi (à vérifier dans le terminal)
byte uidAutorise[] = {0x04, 0x03, 0x02, 0x01}; 

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  monServo.attach(SERVO_PIN);
  monServo.write(0); // Fermé
  pinMode(LED_PIN, OUTPUT);
  Serial.println("--- SYSTEME PRET ---");
}

void loop() {
  // Attendre une carte
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  Serial.print("Badge detecte! ID: ");
  bool accesOK = true;

  // Comparer l'ID lu avec l'ID autorisé
  for (byte i = 0; i < 4; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
    if (rfid.uid.uidByte[i] != uidAutorise[i]) accesOK = false;
  }
  Serial.println();

  if (accesOK) {
    Serial.println(">> ACCES_AUTORISE"); // Message pour Node-RED plus tard
    monServo.write(90); // Ouvre la porte
    delay(3000);
    monServo.write(0);  // Referme
  } else {
    Serial.println(">> ACCES_REFUSE");
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
  }
  
  rfid.PICC_HaltA(); // Arrêter la lecture pour cette carte
}