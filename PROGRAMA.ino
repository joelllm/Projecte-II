//periodes encesa/apagada llums
int tOFFllum=5000;//5s
int tONllum=10000;//10s
//periodes encesa/apagada ventiladors
int tOFFvent=10000;//10s
int tONvent=5000;//5s
//temps amb la bomba encesa
int tONbomba=5000;//5s
//pins dels elements
int llum=10;//pin de la llum
int bomba=11;//pin de la bomba (PWM B)
int dirBomba=13;
int freBomba=8;
int vent=3;//pin del ventilador (PWM A)
int dirVent=12;
int freVent=9;
int soil=A5;//pin del sensor d'humitat del terra
int indMes=7;//pin del led que indica que el sensor de terra està prenent una mesura
//potències dels elements
int POTllum=256;//el màxim
int POTbomba=256;//PROVAR DIFERENTS VALORS
int POTvent=256;//PROVAR DIFERENTS VALORS
//estats del sistema
boolean fosc=true;//estat del LED
boolean ventilant=false;//estat del ventilador
boolean regant=false;//estat de la bomba
boolean regatRecentment=false;
boolean indicadorMesura=false;
//contadors de temps entre esdeveniments
unsigned long previLlum=0;
unsigned long previReg=0;//temps que ha passat des de l'últim cop que s'ha activat la bomba
unsigned long previVent=0;
unsigned long previMes=0;

int tempsEntreRegs=15000;//15 segons entre regs(provisional)
int tempsCheck=10000;//temps a tornar a comprovar la humitat (freqüència) un cop
                     //ja ha passat prou temps des de l'última regada

void setup() {
  // put your setup code here, to run once:
  pinMode(llum,OUTPUT);
  pinMode(freBomba,OUTPUT);       //Channel B Brake Pin Initialize 
  pinMode(dirBomba,OUTPUT);      //Channel B Direction Pin Initialize
  pinMode(freVent,OUTPUT);
  pinMode(dirVent,OUTPUT);
  digitalWrite(dirBomba,HIGH);
  digitalWrite(dirVent,HIGH);
  digitalWrite(freBomba,LOW);
  digitalWrite(freVent,LOW);
  pinMode(soil,INPUT);
  pinMode(indMes, INPUT);
}

void loop() {
//per encendre/apagar els llums
  if ((millis()-previLlum) >= tOFFllum && fosc) {
    digitalWrite(llum,HIGH);
    fosc = false;
    previLlum=millis();
  }
  else if ((millis()-previLlum) >= tONllum && (!fosc)) {
    digitalWrite(llum,LOW);
    fosc = true;
    previLlum=millis();
  }
//per encendre/apagar el ventilador
  if ((millis()-previVent) >= tOFFvent && !ventilant) {
    digitalWrite(vent,HIGH);
    //analogWrite(vent,POTvent);
    ventilant = true;
    previVent=millis();
  }
  else if ((millis()-previVent) >= tONvent && ventilant) {
    digitalWrite(vent,LOW);
    ventilant = false;
    previVent=millis();
  }
//per activar la bomba
//si fa prou temps des de l'última regada --> mesurar la humitat:
  if ((millis()-previReg) >= tempsEntreRegs){
    if ((millis()-previMes) >= 10000){   
      //mesura
      //(indicar que s'ha fet una mesura durant uns 5 segons-->+codi fora d'aquests ifs)
      indicadorMesura = true;
      digitalWrite(indMes,HIGH);
      //si no està prou humit --> regar
        analogWrite(bomba, 200);
        regant=true;
        previReg=millis();
        previMes=millis();
      //si està prou humit
        previMes=millis();
    }
  }
  else if ((millis()-previReg) >= tONbomba && regant) {
    digitalWrite(bomba,LOW);
    regant = false;
    previReg=millis();
  }
//inidicador mesura:
  if (millis()- previMes >= 3000 && indicadorMesura){
    digitalWrite(indMes,LOW);
  }
}
