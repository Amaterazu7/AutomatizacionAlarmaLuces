/*
 * 
 Titulo del Proyecto: Sistema de Automatización de Alarma y Luces 
 Autor: Diego Leonel Cañete
 Profesor: Diego Azcurra
 Cátedra: Sistemas Distribuidos
 Started Date: 06/11/2017
 Día del Desarrollo: 10/12/2017
 Versión: 1.7
 Día de la Revisión: 11/12/2017
*/
/////////////////////////////////////////////////////////////////
#include <Password.h> //Incluyo la libreria Password
#include <Keypad.h> //Incluyo la libreria Keypad
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
Password password = Password("1A2B3");  //Defino el Password
int dlugosc = 5;                        //Largo del Password
boolean estado = false; 
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//Creo las Variables de salida
int buzzer = 11; 
int redLED = 12; 
int blueLED = 13;
int relay = 53;
int sPIR = 10;
int mov = 0;
 
int ilosc; //Numero de Clicks
 
const byte ROWS = 4; // Cuatro Filas
const byte COLS = 4; // Cuatro Columnas

// Defino el Keymap
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = { 2, 3, 4, 5 };
byte colPins[COLS] = { 6, 7, 8, 9 };
 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
 
void setup() {
  Serial.begin(9600);
  keypad.addEventListener(keypadEvent);  
  pinMode(redLED, OUTPUT);  
  pinMode(blueLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(sPIR, INPUT);

  digitalWrite(relay, HIGH);
  digitalWrite(redLED, HIGH);
  digitalWrite(blueLED, LOW);

  //Seteo los valores del LCD
  lcd.begin(20, 4);
  lcd.setCursor(4,0);
  lcd.print("*BIENVENIDO*");
  //DIA
  lcd.setCursor(1,1);
  lcd.print(11, DEC);
  lcd.print('/'); 
  //We print the day
  lcd.print(12, DEC);
  lcd.print('/');  
  lcd.print(2017, DEC);
  lcd.print(' ');
  //HORA
  lcd.setCursor(13,1);
  lcd.print(10, DEC);
  lcd.print(':');
  lcd.setCursor(16,1);
  lcd.print(30, DEC);
  lcd.setCursor(2,2);
  lcd.print("ALARMA ACTIVADA!");
  lcd.setCursor(3,3);
  lcd.print("INGRESE SU PIN");
}
 
void loop() {

  keypad.getKey();
  if(estado) {
    //ALARMA DESACTIVADA
    digitalWrite(redLED, LOW);
    digitalWrite(blueLED, HIGH);
    digitalWrite(buzzer, LOW);
    
    mov=digitalRead(sPIR);
    delay(50);
    if(mov==HIGH){
      digitalWrite(relay, LOW);
      mov=0;
      delay(4000);
      digitalWrite(relay, HIGH);
    } 
  } else {
    //ALARMA ACTIVADA
    digitalWrite(redLED, HIGH);
    digitalWrite(blueLED, LOW);
    //Obligo a apagar las Luces de la casa si la alarma esta activada
    digitalWrite(relay, HIGH);
    
    mov=digitalRead(sPIR);
    delay(50);
    if(mov==HIGH){
      digitalWrite(buzzer, HIGH);
      mov=0;
    } 
  }

}

void keypadEvent(KeypadEvent eKey) {
  switch (keypad.getState()) {
    case PRESSED:
      /*
      int i;
      for( i = 1; i <= 1; i++ ) {
        digitalWrite(buzzer, HIGH);  
        delay(200);            
        digitalWrite(buzzer, LOW);  
        delay(100);      
      }
    */
    Serial.print("Pressed: ");
    Serial.println(eKey);
     
    switch (eKey) {
      default:
      ilosc=ilosc+1;
      password.append(eKey);
    }
 
    if(ilosc == 1) {
      lcd.clear();
      lcd.setCursor(5,1);
      lcd.print("<< PIN >>");
      lcd.setCursor(7,2);
      lcd.print("*_");
    }
    if(ilosc == 2) {
      lcd.clear();
      lcd.setCursor(5,1);
      lcd.print("<< PIN >>");
      lcd.setCursor(7,2);
      lcd.print("**_");
    }
    if(ilosc == 3) {
      lcd.clear();
      lcd.setCursor(5,1);
      lcd.print("<< PIN >>");
      lcd.setCursor(7,2);
      lcd.print("***_");
    }
    if(ilosc == 4) {
      lcd.clear();
      lcd.setCursor(5,1);
      lcd.print("<< PIN >>");
      lcd.setCursor(7,2);
      lcd.print("****_");
    }
    if(ilosc == 5) {
      lcd.clear();
      lcd.setCursor(5,1);
      lcd.print("<< PIN >>");
      lcd.setCursor(7,2);
      lcd.print("*****_");
    }
    
    if(ilosc == dlugosc) {
      delay(250);
      checkPassword();
      ilosc = 0;
    }
  }
}
 
void checkPassword() {
  if (password.evaluate()) {
    
    int i;
    for( i = 1; i <= 3; i++ ) {
      digitalWrite(buzzer, HIGH);  
      delay(120);            
      digitalWrite(buzzer, LOW);  
      delay(70);      
    } 
     
    ilosc = 0;
    password.reset();
      
    Serial.println("Correcto");       
    lcd.clear();
    lcd.setCursor(1,1);
    lcd.print("<< PIN CORRECTO >>");  
    delay(2000);
     
    if(estado) {
      estado = false;
      AlarmaActivada();
    } else {
      estado = true;
      AlarmaDesactivada();
    }

  } else {
    
    int i;
    for( i = 1; i <= 1; i++ ) {
      digitalWrite(buzzer, HIGH);  
      delay(300);            
      digitalWrite(buzzer, LOW);  
      delay(100);      
    } 
    
    ilosc = 0;  
    password.reset();
 
    Serial.println("Error");
             
    lcd.clear();
    lcd.setCursor(1,1);
    lcd.print("<< PIN  ERRONEO >>");
    delay(2000);
    
    if(estado) {
      AlarmaDesactivada();
    } else {
      digitalWrite(buzzer, HIGH);
      AlarmaActivada();
    }    
  }
}

void AlarmaDesactivada(){
  lcd.clear(); 
  lcd.setCursor(4,0);
  lcd.print("*BIENVENIDO*");
  //DIA
  lcd.setCursor(1,1);
  lcd.print(11, DEC);
  lcd.print('/'); 
  //We print the day
  lcd.print(12, DEC);
  lcd.print('/');  
  lcd.print(2017, DEC);
  lcd.print(' ');
  //HORA
  lcd.setCursor(13,1);
  lcd.print(10, DEC);
  lcd.print(':');
  lcd.setCursor(16,1);
  lcd.print(30, DEC);
  lcd.setCursor(1,2);
  lcd.print("ALARMA DESACTIVADA");
  lcd.setCursor(3,3);
  lcd.print("INGRESE SU PIN");
}

void AlarmaActivada() {
  lcd.clear(); 
  lcd.setCursor(4,0);
  lcd.print("*BIENVENIDO*");
  //DIA
  lcd.setCursor(1,1);
  lcd.print(11, DEC);
  lcd.print('/'); 
  //We print the day
  lcd.print(12, DEC);
  lcd.print('/');  
  lcd.print(2017, DEC);
  lcd.print(' ');
  //HORA
  lcd.setCursor(13,1);
  lcd.print(10, DEC);
  lcd.print(':');
  lcd.setCursor(16,1);
  lcd.print(30, DEC);
  lcd.setCursor(2,2);
  lcd.print("ALARMA ACTIVADA!");
  lcd.setCursor(3,3);
  lcd.print("INGRESE SU PIN");
}

