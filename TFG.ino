// Servo - Version: Latest 
#include <Servo.h>

// Keypad - Version: Latest 
#include <Keypad.h>

const byte numR= 4; //number of rows on the keypad
const byte numC= 4; //number of columns on the keypad

/*keymap defines the key pressed according to the row and columns just as appears on the keypad*/
char keymap[numR][numC]=
{
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numR] = {'9','8','7','6'}; //Rows 0 to 3
byte colPins[numC]= {'5','4','3','2'}; //Columns 0 to 3

// MFRC522 - Version: Latest 
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_1_PIN        10         // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 2
#define SS_2_PIN        8          // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 1
#define SS_3_PIN        25

#define NR_OF_READERS   3

byte ssPins[] = {SS_1_PIN, SS_2_PIN, SS_3_PIN};             // RFID
String a = "";                                              // for incoming serial data

#include <LiquidCrystal.h>  //LCD lib

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //LCD

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                            // Create LCD instance.
MFRC522 mfrc522[NR_OF_READERS];                                       // Create MFRC522 instance.
Servo ServLock;                                                       // Create Servo instance.
Keypad KP= Keypad(makeKeymap(keymap), rowPins, colPins, numR, numC);  // Create KeyPad instance.

#define ficarrd1        34
#define ficarre1        35
#define ficarrd2        36
#define ficarre2        37
#define botop1          12
#define botop2          13
#define motor1.1        38
#define motor1.2        39
#define b_persiana      13

#define a 4                 //4 és l’exemple d’acceleració 

/**       
 * Initialize.
 */
void setup() {
  
  pinMode(boto, INPUT);   // Declarem el boto com a input
  pinMode(ficarro, INPUT);
  pinMode(ficarrt, INPUT);
  
  Serial.begin(9600);     // Initialize serial communications with the PC
  
  lcd.setCursor(1, 5);    //Set the cursos in middle of the lcd
  lcd.print(">Connectat"); //writte ">connectat"
  
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
  Delay(1000);
  lcd.clear;
}

/**
 * Main loop.
 */
void loop(){
  Char User[] = 0;
  Char Estat = 3;
  Boolean Persiana = False;
  
  //Estat 1 - Entrada usuari
  do{                                       
    User = wiat_user_rf();
    if (User = 0) {
      User = wait_user_kp();
      Estat = 2;
    }
  }while (User =! 1);
  
  //Estat 2 - Entrada contrasenya
  if Estat=2 then{                            
    User = wait_user_kp();
  }
  
  //Estat 3.0 - User autoritzat
  if (User = 1){
    do{  
      do{                                       //Estat 3.1 - Obrnt persiana
        pos=persiana("O");
        Persiana = digitalRead(b_persiana);     //Actualitzem el boto persiana
      }while (pos=2);
      
      chek_item();                              //Estat 3.2 - Comprova les peces
      Persiana = digitalRead(b_persiana);       //Actualitzem el boto persiana
      if(HIGH = digitalRead(botop1)){           //Estat 3.3 - Comprova boto del panell
        MouPanell(1);
      }else if(HIGH = digitalRead(botop2)){
        MouPanell(2); 
      }
    }while Persiana = False;
    pos=persiana("T");
    ServLock.write(0);
  }
}

char wait_user_rf() {
  reader = 0;
  if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
    Serial.print(00,HEX);                                                      //Send to RBPi a Request User code
    delay(10);                                                                 //10 ms delay
    dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);    //Read User ID from rfid 1 and send to RBPi
    delay(100);                     //100 msec max to read answer from RaspBerry Pi
    if (Serial.available() > 0) {   //Reading RaspBerry PI
        while(Serial.available()) {
        a= Serial.readString();     // read the incoming data as string
        }
    }
    if (a="OK"){
      lcd.print("Accés autoritzat");
      ServLock.write(90);
      delay(2000);
      lcd.clear;
      return 1;
    }
    else{
      LCD.print("Denegat");
      delay(2000);
      lcd.clear;
      return 0;
     }
  } //if (mfrc522[reader].PICC_IsNewC
  // Halt PICC
  mfrc522[reader].PICC_HaltA();
  // Stop encryption on PCD
  mfrc522[reader].PCD_StopCrypto1();
}

char wait_user_kp(){
  char KPress[7];
  int j;
  Kpress[0] = KP.getKey();
  delay(10);
  if (Kpress[0] != NO_KEY){
    for (int i=1; i<=7; i++){
      j = 0;
      while (KP.getKey=NO_KEY){  	//mentre no es detecti una  tecla, espera 50 ms
        delay(50);
        j++;
        if(j=50000){	          	//si passa molta estona sense apretar una tecla
        	i = 10;		              // i està en una posició impossible
        	break;		              // surt del while
        }
      }
    }
	}
}
Kpress[i]=KP.getKey		//captura la següent tecla
delay(100);			//espera 100ms

    }
    Serial.print(00,HEX);                                                      //Send to RBPi a Request User code
    delay(10);                                                                 //10 ms delay
    dump_byte_array(Kpress[], 8);   //Read User ID from kp 1 and send to RBPi
    delay(100);                     //100 msec max to read answer from RaspBerry Pi
    if (Serial.available() > 0) {   //Reading RaspBerry PI
        while(Serial.available()) {
        a= Serial.readString();     // read the incoming data as string
        }
    }
    if (a="OK"){
      lcd.print("Entra el PIN");
      ServLock.write(90);
      delay(2000);
      lcd.clear;
      return 1;
    }
    else{
      LCD.print("Denegat");
      delay(2000);
      lcd.clear;
      return 0;
    }
  }
}

char persiana(String S){
  char pos=0;
  if(ficarro=1 & ficarrt=0 & S="T"){        //comprova si està oberta i que la instrucció sigui tancar
    pos=tanca();
  }
  else if(ficarro=0 & ficarrt=1 & S="O"){   //comprova si està tancada
    pos=obre();
  }
  else if(ficarro=0 & ficarrt=0){           //sinó la persiana no es troba ni adalt ni abaix
    pos=tanca();
  }
  else{
    lcd.clear();
    lcd.setCursor(1, 6);
    lcd.print(">ERROR!<");
    lcd.setCursor(1, 2);
    lcd.print("posició persiana");
    lcd.setCursor(1, 5);
    lcd.print("errònia.");
  }
  return pos;
}

void check_item(){
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Look for new cards
        Serial.print("RI" & reader);                                              //Send to RBPi a Request Item code
        delay(10);                                                                //10 ms delay
        if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
          dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);   //Item ID   
          delay(1000);                    //1 second max to read answer from RaspBerry Pi
          if (Serial.available() > 0) {   //Reading RaspBerry PI
            while(Serial.available()) {
             a= Serial.readString();// read the incoming data as string
            } //while serial.available
            Serial.print(a);
          } //if serial.available
        } //if (mfrc522[reader].PICC_IsNewC
        else
        {
          Serial.print("NO");
        }
      // Halt PICC
      mfrc522[reader].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[reader].PCD_StopCrypto1();
    
  } //for(uint8_t reader)
}

void mou_panell(char p){
  analogWrite(11, 0);
  analogWrite(12, 0);
  
  if (ficarrd1||ficarrd2){                           //Si està a la dreta
    digitalWrite(motor1.1, HIGH);                    //Configura motor a la dreta
    digitalWrite(motor1.2, LOW);        
    digitalWrite(motor2.1, HIGH);
    digitalWrite(motor2.2, LOW);
    
  }else{
    digitalWrite(motor1.1, LOW);                      //Configura motor a la esquerra
    digitalWrite(motor1.2, HIGH);
    digitalWrite(motor2.1, LOW);
    digitalWrite(motor2.2, HIGH);
  }
  t1=millis();
  do{
    t=(millis()-t1)/1000;
    if(p=1){
      analogWrite(11, map(a*t,0,2000,0,255);
    }else{
      analogWrite(12, map(a*t,0,2000,0,255);
    }
    delay(5);
  }while ((a*t)<2000);
  if (p=1){
    do{                                  //velositat constant fins al final
      analogWrite(11,255);
    }while (ficarre1=HIGH || ficarrd2=HIGH):
  }else{
    do{                         
      analogWrite(11,255);
    }while (ficarre2=HIGH || ficarrd2=HIGH):
  }
}

void dump_byte_array(byte *buffer, byte bufferSize) { //Helper routine to dump a byte array as hex values to Serial.
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
