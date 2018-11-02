/*Pines:
 * Foco1   pin 5 digital
 * Foco2   pin 6 digital
 * buzzer  pin 9 digital
 * signalIR pin A0 Analogico
 * botonDePanico pin 10 digital
   Trigger pin 2 digital
   Echo pin 3 digital
 */
#define pinFoco1 5
#define pinFoco2 6
#define buzzer 9
#define signalIR A0
#define botonDePanico 10
#define Trigger 2   //Pin digital 2 para el Trigger del sensor
#define Echo 3

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
  double t; //timepo que demora en llegar el eco
  double d; //distancia en centimetros

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  
  t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  d = t/59;  
  return d;
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
  pinMode(Echo, INPUT);  //pin como entrada
  //Salidas
  pinMode(pinFoco1,OUTPUT);
  pinMode(pinFoco2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(Trigger, OUTPUT); //pin como salida
  //Inicial comunicacion serial
  Serial.begin(9600);
  //Establecer una  distancia Inicial
  double todDis=0;
  for(int i=0; i<10;i++){
    todDis+= getDistance();
  }
  distanciaInicial=todDis/10; //Promedio de 10 lecturas 
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
}

void loop() {
    
}
