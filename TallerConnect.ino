/*Pines:
 * Foco1   pin 5 digital
 * Foco2   pin 6 digital
 * buzzer  pin 9 digital
 * signalIR pin A0 Analogico
 * botonDePanico pin 10 digital
 */
#define pinFoco1 5
#define pinFoco2 6
#define buzzer 9
#define signalIR A0
#define botonDePanico 10

//Variables globales
double distanciaInicial;
bool ladronDetectado=false;
bool alertaEnviada=false;

//Prende los dos focos
void prenderLuces(){
  digitalWrite(pinFoco1,HIGH);
  digitalWrite(pinFoco2,HIGH);
}

//Recibe el valor de la fecuencia con la que va sonar el buzzer
void suenaBocina(int frecuencia){
  tone(buzzer, frecuencia); //Frecuencia 1000 Hz
  delay(2000);        
  noTone(buzzer);     
  delay(2000); 
}

//Regresa un double con el valor de la distancia detectad
double getDistance(){
  double voltaje = analogRead(signalIR) * (5/1024); // Cambiar el valor de 0-1024 a un voltaje para sacar la distancia
  double distancia = 13*pow(voltaje,-1); //datasheet
  return distancia;
}

//Regresa true si se detecto ladron
bool detectarLadron(){
  bool botonPresionado = digitalRead(botonDePanico); //Revisa si se presiono el boton de panico
  return (getDistance()<distanciaInicial*0.95) || botonPresionado; //EL valor comparar depende de la distancia inicial con 5% de margen hacia abajo o boton de panico esta presionado
      
}

//Regresa true si se logro enviar la alerta
bool enviarAlerta(){
  //Enviar alerta por comunicacion serial al script de python que hace el post.
  Serial.write(1);
  delay(20);//Esperar a que se envie y se reciba la confirmacion
  if(Serial.available()>0){
    int incomingInfo = Serial.read();
    return (incomingInfo == 2); //Si se recibe la confirmacion de recibido se regresa  true
    }   
}
void setup() {
  //Definir pines de entrada y de salida
  //Entradas
  pinMode(signalIR,INPUT);
  pinMode(botonDePanico, INPUT);
  //Salidas
  pinMode(pinFoco1,OUTPUT);
  pinMode(pinFoco2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  //Inicial comunicacion serial
  Serial.begin(9600);
  //Establecer una  distancia Inicial
  double todDis=0;
  for(int i=0; i<10;i++){
    todDis+= getDistance();
  }
  distanciaInicial=todDis/10; //Promedio de 10 lecturas 
}

void loop() {
    
}
