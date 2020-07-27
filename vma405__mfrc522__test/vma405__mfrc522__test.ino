
/*
* edited by Velleman / Patrick De Coninck
* Read a card using a mfrc522 reader on your SPI interface
* Pin layout should be as follows (on Arduino Uno - Velleman VMA100):
* MOSI: Pin 11 / ICSP-4
* MISO: Pin 12 / ICSP-1
* SCK: Pin 13 / ISCP-3
* SS/SDA (MSS on Velleman VMA405) : Pin 10
* RST: Pin 9
* VCC: 3,3V (DO NOT USE 5V, VMA405 WILL BE DAMAGED IF YOU DO SO)
* GND: GND on Arduino UNO / Velleman VMA100
* IRQ: not used
*/

#include <SPI.h>
#include <RFID.h>

#define SS_PIN 10
#define RST_PIN 9

RFID rfid(SS_PIN,RST_PIN);


int power = 7;
int led = 8; 
int serNum[5];
/*
* This integer should be the code of Your Mifare card / tag 
*/
int cards[][5] = {{117,222,140,171,140}};

bool access = false;

void setup(){

    Serial.begin(9600);
    SPI.begin();
    rfid.init();
/*
* define VMA100 (UNO) pins 7 & 8 as outputs and put them LOW
*/
    pinMode(led, OUTPUT);
    pinMode (power,OUTPUT);
    digitalWrite(led, LOW);
    digitalWrite (power,LOW);
   
}

void loop(){
    
    if(rfid.isCard()){
    
        if(rfid.readCardSerial()){
            Serial.print(rfid.serNum[0]);
            Serial.print(" ");
            Serial.print(rfid.serNum[1]);
            Serial.print(" ");
            Serial.print(rfid.serNum[2]);
            Serial.print(" ");
            Serial.print(rfid.serNum[3]);
            Serial.print(" ");
            Serial.print(rfid.serNum[4]);
            Serial.println("");
            
            for(int x = 0; x < sizeof(cards); x++){
              for(int i = 0; i < sizeof(rfid.serNum); i++ ){
                  if(rfid.serNum[i] != cards[x][i]) {
                      access = false;
                      break;
                  } else {
                      access = true;
                  }
              }
              if(access) break;
            }
           
        }
        
       if(access){
           Serial.println("Welcome Velleman ");
/*
* Valid card : Switch ON the LED for 1000 ms (1 second)
*/
           digitalWrite(led, HIGH); 
           delay(1000);
/*
* Valid card : Switch ON the POWER PIN for 2000 ms (2 seconds)), the POWER PIN can activate for example a relais, controlling a doorlock)
*/           
           digitalWrite(power, HIGH);
           delay(2000);
           digitalWrite(power, LOW);
           digitalWrite(led, LOW);
           access = false;
           
      } else {
/*
* NON-Valid card : switch ON and OFF the LED twice for 0,5 seconds
*/
           Serial.println("Not allowed!"); 
           digitalWrite(led, HIGH);
           delay(500);
           digitalWrite(led, LOW); 
           delay(500);
           digitalWrite(led, HIGH);
           delay(500);
           digitalWrite(led, LOW);         
       }        
    }
    
    
    
    rfid.halt();

}
