#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);
// Init array that will store new NUID
byte nuidPICC[4];

const int VOL_PIN1 = A1;
const int VOL_PIN2 = A2;
char t;

void setup() {
Serial.begin(9600);
SPI.begin(); // Init SPI bus
rfid.PCD_Init(); // Init MFRC522
Serial.println(F("Scan RFID NUID..."));
pinMode(5,OUTPUT);   //left motors forward
pinMode(4,OUTPUT);   //left motors reverse
pinMode(6,OUTPUT);   //right motors forward
pinMode(3,OUTPUT);   //right motors reverse
pinMode(2,OUTPUT);   //ENABLE LEFT
pinMode(8,OUTPUT);   //ENABLE RIGHT
}
 //movements for automatic mode
void stopA(){
  digitalWrite(5,LOW);
  digitalWrite(4,LOW);
  digitalWrite(6,LOW);
  digitalWrite(3,LOW);
  delay(100);
}

void forwordA(){
  stop();
  analogWrite(2, 130);
  analogWrite(8, 215);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  delay(50);
  stop();
  }

void backA(){
  stop();
  analogWrite(2, 130);
   analogWrite(8, 215);

  digitalWrite(4,HIGH);
  digitalWrite(3,HIGH);
  delay(50);
  stop();
  }

void rightA(){
   stop();
   analogWrite(2, 130);
   analogWrite(8, 215);

  digitalWrite(6,HIGH);
  delay(50);
  stop();
  }

 void leftA(){
   stop();
   analogWrite(2, 130);
   analogWrite(8, 215);
  digitalWrite(5,HIGH);
  delay(50);
  stop();
    }
//movements for manual mode
void stop(){
  digitalWrite(5,LOW);
  digitalWrite(4,LOW);
  digitalWrite(6,LOW);
  digitalWrite(3,LOW);
  //delay(50);  
}
void forword(){
  stop();
  analogWrite(2, 130);
   analogWrite(8, 215);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  delay(500);
  stop();
  }
void back (){
  stop();
  analogWrite(2, 130);
   analogWrite(8, 215);
  digitalWrite(4,HIGH);
  digitalWrite(3,HIGH);
  delay(500);
  stop();
  }
void right (){
   stop();
   analogWrite(2, 130);
   analogWrite(8, 215);
  digitalWrite(6,HIGH);
  delay(500);
  stop();
  }
 void left (){
     stop();
    analogWrite(2, 130);
   analogWrite(8, 215);
  digitalWrite(5,HIGH);
  delay(500);
  stop();
    }

void loop() {
  int value1;
    float volt1;
    int value2;
    float volt2;
    int n ;
  if(Serial.available()>=0){ //If some thing is received by bluetooth
    t =Serial.read();  //Read the serial buffer
    delay(100);
    readRFID();
    }
if(t == '1'){            //move forward(all motors rotate in forward direction) 
  forword();

}
else if(t == '5'){      //move reverse (all motors rotate in reverse direction)
back();
}
else if(t == '2'){      //turn right (left side motors rotate in forward direction, right side motors doesn't rotate)
   right();
}
else if(t == '4'){      //turn left (right side motors rotate in forward direction, left side motors doesn't rotate)
left();
}

else if(t == '6'){      //STOP (all motors stop)
  char t2="0";
  while(t2 !="6")
  {
  value1 = analogRead( VOL_PIN1 );
    value2 = analogRead( VOL_PIN2 );
    volt1 = value1 * 5.0 / 1023.0;
    volt2 = value2 * 5.0 / 1023.0;
    if ((volt1 >3) && (volt2 >3)){
      forwordA(); 
        
    }
    else if ((volt1 <3) && (volt2 >3)){
     backA();   
    }
    else if ((volt1 >3) && (volt2 <3)){
      rightA();
    }
    else if ((volt1 <3) && (volt2 <3)){
      stopA();
    }
    if (Serial.available()>=0)
    {
      t2 = Serial.read();
    
     delay(200);
}
  }
}
}

void readRFID()
{
 // Look for new card
 if ( ! rfid.PICC_IsNewCardPresent())
 return;
   // Verify if the NUID has been readed
 if (  !rfid.PICC_ReadCardSerial())
 return;
 
 if (rfid.uid.uidByte[0] != nuidPICC[0] ||
   rfid.uid.uidByte[1] != nuidPICC[1] ||
   rfid.uid.uidByte[2] != nuidPICC[2] ||
   rfid.uid.uidByte[3] != nuidPICC[3] ) {
   Serial.println(F("A new card has been detected."));
   // Store NUID into nuidPICC array
   for (byte i = 0; i < 4; i++) {
     nuidPICC[i] = rfid.uid.uidByte[i];
   }
  
   Serial.print(F("RFID tag in dec: "));
   printDec(rfid.uid.uidByte, rfid.uid.size);
   Serial.println();
 }
  
 // Halt PICC
 rfid.PICC_HaltA();
 // Stop encryption on PCD
 rfid.PCD_StopCrypto1();
}
/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
 for (byte i = 0; i < bufferSize; i++) {
   Serial.print(buffer[i] < 0x10 ? " 0" : " ");
   Serial.print(buffer[i], DEC);
 }
}
