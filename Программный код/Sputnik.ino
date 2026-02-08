#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);
const byte address[6] = "00001";
const int knopka_skan = 2; 
const int knopka_test = 3;  
int sdvig = 7;

void setup() {
  Serial.begin(9600);
  pinMode(knopka_skan, INPUT_PULLUP);
  pinMode(knopka_test, INPUT_PULLUP);
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);
  radio.openReadingPipe(1, address);
  radio.startListening();
  Serial.println("Пульт включен и слушает...");
}

void loop() {
  if (digitalRead(knopka_skan) == HIGH) {
    radio.stopListening();
    int kom = 1 + sdvig;
    radio.write(&kom, sizeof(kom)); 
    radio.startListening();
    Serial.println("--- ОТПРАВКА: СКАН ---");
    while(digitalRead(knopka_skan) == LOW); 
    delay(500);
  }
  else if (digitalRead(knopka_test) == HIGH) {
    radio.stopListening();
    int kom = 2 + sdvig;
    radio.write(&kom, sizeof(kom)); 
    radio.startListening();
    Serial.println("--- ОТПРАВКА: ТЕСТ ---");
    while(digitalRead(knopka_test) == LOW); 
    delay(500);
  }

  if (radio.available()) {
    int mas[4];
    radio.read(&mas, sizeof(mas));
    for(int i=0; i<4; i++) mas[i] -= sdvig;

    if(mas[0] == 99) { 
       if(mas[1] == 10) Serial.println("РЕЗУЛЬТАТ: ЛАЗЕР ОК");
       else if(mas[1] == 20) Serial.println("РЕЗУЛЬТАТ: МЕХАНИКА ОК");
       else if(mas[1] == 30) Serial.println("РЕЗУЛЬТАТ: СВЯЗЬ ОК");
    } else {
       Serial.print("ID:1 | X:"); Serial.print(mas[1]);
       Serial.print(" | Y:"); Serial.print(mas[2]);
       Serial.print(" | ЛАЗЕР:"); Serial.println(mas[3] == 1 ? "ВКЛ" : "ВЫКЛ");
    }
  }
}