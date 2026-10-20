# Système de Contrôle d'Accès RFID connecté via MQTT

Projet IoT – UIR. Arduino Uno + MFRC522 → Gateway Python → Mosquitto → Node-RED Dashboard.

## Architecture
Arduino (RFID + servo) --USB série--> gateway.py --MQTT--> Mosquitto --> Node-RED

## Matériel
Arduino Uno, MFRC522, servomoteur SG90, LED.

## Installation
1. Téléverser `arduino/sketch/sketch.ino` sur l'Arduino
2. `pip install -r requirements.txt`
3. Lancer Mosquitto (port 1883)
4. Importer `node-red/flows.json` dans Node-RED
5. `python gateway/gateway.py`

## Topics MQTT
| Topic | Payload | Action |
|---|---|---|
| rfid/access | ACCES_OK | Accès autorisé (vert) |
| rfid/access | ACCES_REFUSE | Alerte sécurité (rouge) |

## Auteur
Votre nom – UIR, 2026