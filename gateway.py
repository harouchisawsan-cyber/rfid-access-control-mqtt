import serial
import paho.mqtt.client as mqtt
import time

# --- CONFIGURATION ---
PORT_SERIE = 'COM8'  # <--- Vérifiez bien que c'est le bon port (COM7, COM3, etc.)
BAUD_RATE = 9600
BROKER_MQTT = "localhost" # Puisque Mosquitto est sur le même PC
TOPIC_MQTT = "rfid/access"

# 1. Connexion au Broker Mosquitto
client = mqtt.Client()
try:
    client.connect(BROKER_MQTT, 1883, 60)
    print("✅ Connecté au Broker Mosquitto")
except:
    print("❌ Erreur: Mosquitto n'est pas lancé sur ce PC")
    exit()

# 2. Connexion à l'Arduino (Série)
try:
    ser = serial.Serial(PORT_SERIE, BAUD_RATE, timeout=1)
    print(f"✅ Connecté à l'Arduino sur {PORT_SERIE}")
except:
    print(f"❌ Erreur: Impossible d'ouvrir le port {PORT_SERIE}")
    exit()

print("--- Gateway Active (Appuyez sur Ctrl+C pour arrêter) ---")

# 3. Boucle de lecture
try:
    while True:
        if ser.in_waiting > 0:
            # Lire la ligne venant de l'Arduino
            ligne = ser.readline().decode('utf-8').strip()
            print(f"Arduino dit : {ligne}")

            # Envoyer vers MQTT selon le message reçu
            if "ACCES_AUTORISE" in ligne:
                client.publish(TOPIC_MQTT, "ACCES_OK")
                print("📢 MQTT envoyé : ACCES_OK")
            
            elif "ACCES_REFUSE" in ligne:
                client.publish(TOPIC_MQTT, "ACCES_REFUSE")
                print("📢 MQTT envoyé : ACCES_REFUSE")
        
        time.sleep(0.01) # Petite pause pour laisser respirer le processeur

except KeyboardInterrupt:
    print("\nArrêt de la gateway...")
    ser.close()
    client.disconnect()