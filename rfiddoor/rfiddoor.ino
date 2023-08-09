#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9

#define GREEN_LED 3 //define green LED pin
#define RED_LED 4 //define red LED
#define BUZZER 2 //buzzer pin


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myServo; //define servo name

#include<LiquidCrystal.h>
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);//(RS, Enable, A2, A3, A4, A5)




void setup() 
{
  // Specify the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);   // Initiate a serial communication

  
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  myServo.attach(6); //servo pin
  myServo.write(0); //servo start position
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  Serial.println("Put your card to the reader...");
  Serial.println();
}


void loop() {
  lcd.setCursor(0, 0);
  lcd.print("LUTFEN");
  lcd.setCursor(0, 1);
  lcd.print("KARTINIZI OKUTUN");
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  lcd.println();
  lcd.print("Message : ");
  content.toUpperCase();
  
  if (content.substring(1) == "69 C6 E7 C1") //RFID TAG
  {
    lcd.setCursor(0, 0);
    lcd.println("KAPI ACILDI     ");
    lcd.setCursor(0, 1);
    lcd.print("ISIM :");
    lcd.println("EMRE TIGLI");
    delay(500);
    digitalWrite(GREEN_LED, HIGH);
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    myServo.write(180);
    delay(5000);
    myServo.write(0);
    digitalWrite(GREEN_LED, LOW);
    lcd.clear();
  }

  
//  else if (content.substring(1) == "73 6D 07 03") //RFID CARD
//  {
//    lcd.setCursor(0, 0);
//    lcd.println("Authorised access");
//    lcd.setCursor(0, 1);
//    lcd.print("NAME : ");
//    lcd.println("BETA MAXX");
//    delay(500);
//    digitalWrite(GREEN_LED, HIGH);
//    tone(BUZZER, 500);
//    delay(300);
//    noTone(BUZZER);
//    myServo.write(180);
//    delay(5000);
//    myServo.write(0);
//    digitalWrite(GREEN_LED, LOW);
//    lcd.clear();
//  }
  
 else   {
    lcd.setCursor(0, 0);
    lcd.println("KAPI ACILMADI       ");
    lcd.setCursor(0, 1);
    lcd.println("YEKISIZ GIRIS        ");
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER, 300);
    delay(1000);
    digitalWrite(RED_LED, LOW);
    lcd.clear();
    noTone(BUZZER);
  }
} 
