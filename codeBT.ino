#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <SoftwareSerial.h> // Ajouter cette bibliothèque

// Bluetooth sur Pins 2 (RX) et 3 (TX)
SoftwareSerial BTSerial(2, 3); 

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 6
#define LED_PIN 7

MFRC522 rfid(SS_PIN, RST_PIN);
Servo monServo;

byte uidAutorise[] = {0x04, 0x03, 0x02, 0x01}; 

void setup() {
  Serial.begin(9600);   // Pour la Gateway Python (USB)
  BTSerial.begin(9600); // Pour le Smartphone (Bluetooth)
  SPI.begin();
  rfid.PCD_Init();
  monServo.attach(SERVO_PIN);
  monServo.write(0);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("--- SYSTEME HYBRIDE PRET ---");
}

void ouvrirPorte() {
  Serial.println(">> ACCES_AUTORISE"); // Envoi à la Gateway Python
  monServo.write(90);
  delay(3000);
  monServo.write(0);
}

void loop() {
  // --- PARTIE 1 : BLUETOOTH ---
  if (BTSerial.available()) {
    char commande = BTSerial.read();
    if (commande == 'O') { // 'O' comme Open envoyé depuis le téléphone
      ouvrirPorte();
    }
  }

  // --- PARTIE 2 : RFID ---
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    bool accesOK = true;
    for (byte i = 0; i < 4; i++) {
      if (rfid.uid.uidByte[i] != uidAutorise[i]) accesOK = false;
    }

    if (accesOK) {
      ouvrirPorte();
    } else {
      Serial.println(">> ACCES_REFUSE");
      digitalWrite(LED_PIN, HIGH);
      delay(1000);
      digitalWrite(LED_PIN, LOW);
    }
    rfid.PICC_HaltA();
  }
}