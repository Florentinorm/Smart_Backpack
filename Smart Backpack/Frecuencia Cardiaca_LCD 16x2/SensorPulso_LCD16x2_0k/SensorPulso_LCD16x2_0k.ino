
int pulsePin = 0;                   // Sensor de Pulso conectado al puerto A0
// Estas variables son volatiles porque son usadas durante la rutina de interrupcion en la segunda Pestaña
volatile int BPM;                   // Pulsaciones por minuto
volatile int Signal;                // Entrada de datos del sensor de pulsos
volatile int IBI = 600;             // tiempo entre pulsaciones
volatile boolean Pulse = false;     // Verdadero cuando la onda de pulsos es alta, falso cuando es Baja
volatile boolean QS = false;        // Verdadero cuando el Arduino Busca un pulso del Corazon

//LIBRERÍAS OLED Y PULSADOR

#include <Wire.h>     // libreria para bus I2C
#include <Adafruit_GFX.h>   // libreria para pantallas graficas
#include <Adafruit_SSD1306.h>   // libreria para controlador SSD1306
 
#define ANCHO 128     // reemplaza ocurrencia de ANCHO por 128
#define ALTO 64       // reemplaza ocurrencia de ALTO por 64

#define pulsador 8 // pin en donde se conecta el pulsador
bool estPulActual=0; // variable donde se guarda el estado logico actual del pulsador
int value = 0;

int minutos = 25;
int segundos = 55;
int horas = 26;
int horass = 3;
int dia = 14;
int mes = 8;
int anio = 2022;
#define OLED_RESET 4      // necesario por la libreria pero no usado
Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);  // crea objeto


void setup(){ 
 //OLED
  Wire.begin();         // inicializa bus I2C
  Serial.begin(9600);
  pinMode(pulsador, INPUT_PULLUP); //el pin pulsador es asignado como entrada
  delay(100);  // se da una espera de 100ms para que el display inicie correctamente
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // inicializa pantalla con direccion 0x3C
  oled.setTextColor(SSD1306_WHITE);


 //FRECUENCIA
 pinMode(13, OUTPUT);                               
 Serial.begin(9600);                // Puerto serial configurado a 9600 Baudios
 interruptSetup();                  // Configura la interrucion para leer el sensor de pulsos cada 2mS  
}

void loop(){ 
  oled.clearDisplay();
  value = digitalRead(pulsador);  //lectura digital de pin

 if(value == 1){
Serial.println("Encendido");
        //CODIGO DE LA HORA
        oled.clearDisplay();      // limpia pantalla 
        segundos++;
        
        if(segundos>59) {
          minutos++;
          segundos=0;
        }

        if(minutos>59) {
          horas++;
          minutos=0;
          segundos=0;
        }

        if(horas>59) {
          horas++;
          minutos=0;
          segundos=0;
        }

        if(horass>=24) {
          horas=0;
          minutos=0;
          segundos=0;
          horass=0;
          dia++;

          if( ((mes==1) || (mes==3) || (mes==5) || (mes==7) || (mes==8) || (mes==10) || (mes==12))&&(dia>31)){
            mes++;
            dia=1;
          }

          if(mes==13)
          {
            mes=1;
            anio++;
            dia=1;
          }

          if(((mes==4) || (mes==6) || (mes==9) || (mes==11) && (dia>30)))
          {
            mes++;
            dia=1;
          }

          if(( (mes==2) && (dia>28) ))
          {
            mes++;
            dia=1;
          }
        }
        
        //CODIGO DE LA MUESTRA DEL SENSOR 
        oled.fillRoundRect(0, 0, 128, 12, 3, SSD1306_WHITE);
        oled.setCursor(8, 2);
        // Tamaño del texto
        oled.setTextSize(1);
        oled.setTextColor(SSD1306_BLACK);
        oled.print("Fecha: ");  // escribe texto
        oled.print(dia);  // escribe texto
        oled.print("/");
        oled.print(mes);  // escribe texto
        oled.print("/");  // escribe texto
        oled.print(anio);  // escribe texto

        //Horas
        // Tamaño del texto
        oled.setTextSize(2);
         oled.setTextColor(WHITE);    // establece color al unico disponible (pantalla monocromo)
        oled.setCursor(15, 20);
        oled.print(horass);  // escribe texto
        oled.print(":");  // escribe texto
        oled.print(horas);  // escribe texto
        oled.print(":");  // escribe texto
        oled.print(minutos);

        //Segundos
        oled.drawCircle(117, 48, 10, SSD1306_WHITE);
        oled.setCursor(112, 45);
        oled.setTextSize(1);
         oled.setTextColor(WHITE);    // establece color al unico disponible (pantalla monocromo)
        oled.print(segundos);  // escribe texto
        oled.display();     // muestra en pantalla todo lo establecido anteriormente



  } else {
    oled.clearDisplay();
                //CODIGO DE LA MUESTRA DEL SENSOR 
        oled.fillRoundRect(0, 0, 128, 12, 3, SSD1306_WHITE);
        oled.setCursor(8, 2);
        // Tamaño del texto
        oled.setTextSize(1);
        oled.setTextColor(SSD1306_BLACK);
        oled.print("Frecuencia Cardiaca");  // escribe texto



     int pulso = analogRead(A0);           //Lee el valor del pulsometro conectado al puerto Analogo A0
     if (pulso >= 530) {                   // Enciende led 13 cuando el pulso pasa de un valor (debe ajustarse)
        digitalWrite(13, HIGH);
     }  
     else{
        digitalWrite(13, LOW);
     }  
               
        oled.drawRect(20, 20, 88, 44, WHITE); // dibuja rectangulo
        oled.setCursor(28, 34);   // ubica cursor en coordenadas 28,34
        oled.setTextSize(1);      // establece tamano de texto en 2
        oled.setTextColor(WHITE);   // establece color al unico disponible (pantalla monocromo)
        oled.print("BPM:");
        oled.setTextSize(2);      // establece tamano de texto en 2
        oled.print(BPM);     // escribe texto
        Serial.print(BPM);
        oled.display();     // muestra en pantalla todo lo establecido anteriormente
       Serial.println(pulso);                          // envia el valor del pulso por el puerto serie  (desabilitarla si habilita la anterior linea)
      if (QS == true){                       // Bandera del Quantified Self es verdadera cuando el Arduino busca un pulso del corazon
        QS = false;                          // Reset a la bandera del Quantified Self 
      }
      }
}
