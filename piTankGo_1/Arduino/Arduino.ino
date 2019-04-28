const int xpin = A0; // x-axis of the accelerometer
const int ypin = A1; // y-axis
const int zpin = A2; // z-axis (only on 3-axis models)
const byte EMPTY[] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
const byte CERO[] = { 0x1C, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1C, 0x0 };       // ---------------------------------------------------
const byte UNO[] = { 0x08, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C };
const byte DOS[] = { 0x1C, 0x22, 0x02, 0x04, 0x18, 0x20, 0x22, 0x3E };
const byte TRES[] = { 0x3E, 0x02, 0x02, 0x1C, 0x02, 0x02, 0x22, 0x1C };
const byte CUATRO[] = { 0x04, 0x08, 0x10, 0x20, 0x24, 0x3E, 0x04, 0x04 };   //    Números en representación gráfica matricial 8x8
const byte CINCO[] = { 0x3E, 0x20, 0x20, 0x1C, 0x02, 0x02, 0x22, 0x1C };
const byte SEIS[] = { 0x1C, 0x20, 0x20, 0x3C, 0x22, 0x22, 0x22, 0x1C };
const byte SIETE[] = { 0x3E, 0x02, 0x02, 0x04, 0x08, 0x08, 0x08, 0x08 };
const byte OCHO[] = { 0x1C, 0x22, 0x22, 0x1C, 0x22, 0x22, 0x22, 0x1C };
const byte NUEVE[] = { 0x1C, 0x22, 0x22, 0x1E, 0x02, 0x02, 0x22, 0x1C };    // ----------------------------------------------------
int score=-1;  // Puntuación inicial
unsigned long Time;   // Variable para registrar tiempos
void setup()
{
  for (int j=22; j<38; j++){   //Configurar los pines del marcador 
        pinMode(j, OUTPUT);
  }        
  // inicializar la comunicación serie:
  Serial.begin(9600);
  Time = millis();

}
void loop(){
  if( Time + 300 <=  millis() ){   // -----------------------------------------------------------------
    joystick();
    Time = millis();               // Ejecutamos joystick() una vez cada 300ms para liberar recursos
  }                                // -----------------------------------------------------------------
  
  if (Serial.available() > 0) { //Cuando recibamos un byte
    int incomingByte = Serial.read();
    if(incomingByte == 49){      //Cuando ese byte sea un 1 en ASCII
      score++;   //Aumentamos la puntuación     
    }
    else if(incomingByte == 50){  //Cuando ese byte sea un 2 en ASCII
      score = -1;
    }
  }
  marcador(score);
}

void joystick(){  //Función que maneja el acelerómetro
  float zero_G = 384.0; //ADC is 0~1023 the zero g output equal to 1,2375V
  //ADXL335 power supply by Vs 3.3V
  float scale = 3300.0/(1024.0*253.0); //ADXL335330 Sensitivity is 253mv/g

  int x = analogRead(xpin); //Lectura del eje X y conversión a g´s
  float x_g = ((float)x - zero_G)*scale;
  delay(1);
  int y = analogRead(ypin); //Lectura del eje Y y conversión a g´s
  float y_g = ((float)y - zero_G)*scale;
  delay(1); 
//  int z = analogRead(zpin); //Lectura del eje Z y conversión a g´s
//  float z_g = ((float)z - zero_G)*scale;

  if(x_g<-0.5){
    Serial.print(">dcha");
  }
  else if(x_g>0.5){
    Serial.print(">izqd");
  }
  if(y_g<-0.5){
    Serial.print(">arrb");
  }
  else if(y_g>0.5){
    Serial.print(">abjo");
  }

}

void marcador(int score){ //Función que actualiza la puntuación
  Clear();
  for (int fil = 0; fil <8 ; fil++){
     digitalWrite( fil + 22 , HIGH) ; // Activamos la fila para el barrido
     byte F = Selecciona(score, fil); // Seleccionamos el byte correspondiente a cada fila

     for (int col =7; col >= 0 ; col--)
       {
          bool b = GetBit(F, col)  ;
          if (b){
              digitalWrite( 37 - col  ,LOW); // Activamos la columna
          } else{
              digitalWrite( 37 - col  ,HIGH); // Si 0, apagamos
        }}                                   
     digitalWrite( fil + 22 , LOW) ; // Apagamos fila antes de salir
  }
}

void Clear(){ //Función que apaga el marcador
      for (int j=22; j<30; j++){  // Valores de los pines de fila
      digitalWrite(j, LOW);     // Todos apagados
      }for (int k= 30 ; k<38 ; k++){
           digitalWrite(k, HIGH);    // Todas las columnas cortadas
      }
}

byte Selecciona( int n, int fil){ // Función que devuelve el byte de cada cifra correspondiente a una fila
       if ( n == -1){         return( EMPTY[fil]); }
       if ( n == 0){          return( CERO[fil]); }
       if ( n == 1){          return( UNO[fil]); }
       if ( n == 2){          return( DOS[fil]); }
       if ( n == 3){          return( TRES[fil]); }
       if ( n == 4){          return( CUATRO[fil]); }
       if ( n == 5){          return( CINCO[fil]); }
       if ( n == 6){          return( SEIS[fil]); }
       if ( n == 7){          return( SIETE[fil]); }
       if ( n == 8){          return( OCHO[fil]); }
       if ( n == 9){          return( NUEVE[fil]); }
}

bool GetBit( byte N, int pos){  // Función que indica si una columna se debe activar                 
       // pos = 7 6 5 4 3 2 1 0
       int b = N >> pos ;         // Shift bits
       b = b & 1 ;                // coger solo el ultimo bit
       return b ;
}
