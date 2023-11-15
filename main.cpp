#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

//Configuracion de cominicacion I"C
const int lm75_direccion = 0x48;
const int lm75_lectura = 0;
int lm75_temperatura;

void colorWipe(uint32_t color, int wait);

#define PIN 33
#define NUMPIXELS 16
Adafruit_NeoPixel pixels(NUMPIXELS,PIN, NEO_GRB);


int temp1;


void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  Wire.begin();

  //Inicializar el neopixel 
  pixels.begin();
  pixels.show();

  //Iniciar con un color al encender el ESP32
  colorWipe(pixels.Color(0, 255, 0), 50); // Rojo
  delay(1000);


}

void loop() {
 
//Función de comunicacion serial al momento de presionar el boton 1 de la TIVA C
  if (Serial2.available() > 0 ){
    temp1 = Serial2.read();
    if (temp1 == '1'){
      //Inicialización y lectura del sensor LM75
      Wire.beginTransmission(lm75_direccion);
      Wire.write(lm75_lectura);
      Wire.endTransmission();
      Wire.requestFrom(lm75_direccion, 2);
      lm75_temperatura = (Wire.read() << 8) | Wire.read();
      float temperaturaCelsius = lm75_temperatura / 256.0;
      String TempString = String (temperaturaCelsius);
      Serial2.print(TempString);
      Serial.print("Temperatura: ");
      Serial.println(TempString);
      Serial.println(" ºC");
      temp1 = 0;
      if (temperaturaCelsius <= 24.7 ){
        colorWipe(pixels.Color(0, 0, 200), 50); // Rojo
        delay(500);
      }
      else if (temperaturaCelsius > 24.7){
        colorWipe(pixels.Color(200, 0, 0), 50); // Rojo
        delay(500);
      }
      
    }   

    //Colocar un color morado en el Neopixel al presionar el boton 2 de la TIVA C
    if (temp1 == '2'){
      colorWipe(pixels.Color(128, 0, 128), 50); // Rojo
      delay(500);
    }

  }


}


// Función para llenar la tira de LEDs con un color específico
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(wait);
  }
}
