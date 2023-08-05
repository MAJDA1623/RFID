#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

int GreenLed = 2;
int RedLed = 3;

String MasterTag = "A0A0A0A0";
String tagID = "";


MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
  SPI.begin(); 
  mfrc522.PCD_Init(); 
  pinMode(GreenLed, OUTPUT);
  pinMode(RedLed, OUTPUT);
}

void loop()
{
  // Attendre qu'un nouveau tag soit disponible
  while (getID())
  {
    if (tagID == MasterTag)
    {
      digitalWrite(GreenLed, HIGH);
      digitalWrite(RedLed, LOW);
      // Vous pouvez écrire ici n'importe quel code comme ouvrir des portes, allumer un relais, allumer une LED, ou toute autre action.
    }
    else
    {
      digitalWrite(RedLed, HIGH);
      digitalWrite(GreenLed, LOW);
    }

    delay(2000);
    digitalWrite(RedLed, LOW);
    digitalWrite(GreenLed, LOW);
  }
}

// La fonction getID pour lire un nouveau tag s'il est disponible
boolean getID()
{
  // Prêt à lire les PICCs
  if (!mfrc522.PICC_IsNewCardPresent()) { // Si un nouveau PICC est placé sur le lecteur RFID, continuer
    return false;
  }
  if (!mfrc522.PICC_ReadCardSerial()) { // Lire le PICC et continuer
    return false;
  }
  tagID = "";
  for (uint8_t i = 0; i < 4; i++)
  { // Les PICCs MIFARE que nous utilisons ont un UID de 4 octets
    //readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Ajoute les 4 octets dans une variable String
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Arrêter la lecture
  return true;
}
