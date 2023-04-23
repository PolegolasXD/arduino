#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 21
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);

int ledPin1 = 32;
int ledPin2 = 12;

String cartoes[6] = { "23D33A0A" };
int num_cartoes = 1;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String conteudo = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      conteudo.concat(String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""));
      conteudo.concat(String(rfid.uid.uidByte[i], HEX));
    }
    conteudo.toUpperCase();

    bool cartao_cadastrado = false;
    for (int i = 0; i < num_cartoes; i++) {
      if (conteudo == cartoes[i]) {
        cartao_cadastrado = true;
        digitalWrite(ledPin1, HIGH);
        delay(500);
        digitalWrite(ledPin1, LOW);
        delay(500);
        digitalWrite(ledPin1, HIGH);
        delay(500);
        digitalWrite(ledPin1, LOW);
        delay(500);
        digitalWrite(ledPin1, HIGH);
        delay(500);
        digitalWrite(ledPin1, LOW);
        break;
      }
    }
    if (!cartao_cadastrado) {
      digitalWrite(ledPin2, HIGH);
      delay(500);
      digitalWrite(ledPin2, LOW);
      delay(500);
      digitalWrite(ledPin2, HIGH);
      delay(500);
      digitalWrite(ledPin2, LOW);
      delay(500);
      digitalWrite(ledPin2, HIGH);
      delay(500);
      digitalWrite(ledPin2, LOW);


      // Verifica se ainda é possível cadastrar mais cartões
      if (num_cartoes < 6) {
        Serial.println("Deseja cadastrar este cartao? (SIM/NAO)");
        while (!Serial.available()) {}
        String resposta = Serial.readStringUntil('\n');
        if (resposta == "SIM" || resposta == "sim") {
          cartoes[num_cartoes] = conteudo;
          num_cartoes++;
          Serial.println("Cartao cadastrado com sucesso!");
        }
      }
    } else {
      Serial.println("Cartao lido: " + conteudo);
      Serial.println("Cartao cadastrado. Deseja remover? (SIM/NAO)");
      while (!Serial.available()) {}
      String resposta = Serial.readStringUntil('\n');
      if (resposta == "SIM" || resposta == "sim") {
        for (int i = 0; i < num_cartoes; i++) {
          if (conteudo == cartoes[i]) {
            for (int j = i; j < num_cartoes - 1; j++) {
              cartoes[j] = cartoes[j + 1];
            }
            num_cartoes--;
            Serial.println("Cartao removido com sucesso!");
            break;
          }
        }
      }
    }
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
