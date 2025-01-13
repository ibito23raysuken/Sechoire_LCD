/*************************************************
Importation des Librairie
 ******************************************************/
#include <LiquidCrystal.h>
/*************************************************
 initialisation des pin arduino
 ******************************************************/
 /*parametre de debut*/
byte DOWN[8]{B00000, B00100, B00100, B00100, B10101, B01110, B00100,B00000};
byte UP[8]{B00000, B00100, B01110, B10101, B00100 ,B00100 ,B00000,B00000};
byte Compteur = 0;
int Compteur_1 = 0,E_actuelle;
boolean etat_tache[3];
const byte E_0=0,E_1=1,E_2=2,E_3=3,E_4=4,E_5=5,E_6=6;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

unsigned long previousMillis = 0; // Temps précédent pour le défilement
const unsigned long interval = 300; // Intervalle entre chaque défilement (en ms)
int scrollPosition = 0; // Position actuelle du défilement
const int maxScroll = 16; 

 /*parametre pour le bouton*/
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

/*************************************************
 initialisation des taches
 ******************************************************/
 typedef void (*action_t)(void);
 void Tache_Principale();
 void Tache_acquisition();
 void Tache_Lecture_touche();
 /*tableau de tache*/
 action_t Tache[]={
    Tache_Principale,
    Tache_acquisition,
 };

/*************************************************
 programme principale
 ******************************************************/
void setup()
{
  lcd.createChar(0,DOWN);
  lcd.createChar(1,UP);
  Initilisation_materiel();
  Initialisation_inter();

  /*configuration des  */
}
void loop(){
  if(etat_tache[0]==true) Tache[0]();
  if(etat_tache[1]==true) Tache[1]();
}
/*************************************************
gestion des intrruption
 ******************************************************/
ISR (TIMER2_OVF_vect) {
    Compteur++;
    if (Compteur == 5) { // Alterner toutes les 5 interruptions
        Compteur = 0;
        bool etat_ancienne= etat_tache[1];
        etat_tache[1] = etat_tache[0];
        etat_tache[0] = etat_ancienne; 
    }
}

/*************************************************
gestion des taches
 ******************************************************/
void Config_initial(){
  unsigned long currentMillis = millis();
  Serial.print("etape 1---->");
  Serial.println(currentMillis);
  if (scrollPosition == 0) {
    lcd.clear();           // Effacer l'écran
    lcd.setCursor(0, 0);   // Positionner le curseur au début
    lcd.print("CONFIGURATION DE DEBUT"); // Texte à afficher
    lcd.setCursor(0, 1);   // Positionner le curseur au début
    lcd.print("APPUYER SUR SELECT");
  }
  // Si l'intervalle est écoulé, faire défiler
  if (currentMillis - previousMillis >= interval) {
    Serial.println(currentMillis);
    previousMillis = currentMillis; // Mettre à jour le temps précédent
    // Faire défiler à gauche
    lcd.scrollDisplayLeft();

    scrollPosition++;
    if (scrollPosition >= maxScroll) {
      scrollPosition = 0; // Réinitialiser la position pour redémarrer l'affichage
    }
  }
}
void Tache_Principale(){
  unsigned long currentMillis = millis();
  lcd_key = read_LCD_buttons(); 
  switch(E_actuelle){
    case E_1:
      {
        Config_initial();
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis; // Mettre à jour le temps précédent
          if(lcd_key==btnSELECT){
            E_actuelle=E_2;
          }
        }
        break;
      }
    case E_2:
      {
        lcd.clear(); 
        lcd.setCursor(0, 0);   // Positionner le curseur au début
        lcd.print("> TEMPS");
        lcd.setCursor(0, 1);   // Positionner le curseur au début
        lcd.print("  TEMPERATURE");
        lcd.setCursor(15, 1);
        lcd.write(byte(0));
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis; // Mettre à jour le temps précédent
          if(lcd_key==btnDOWN){
            E_actuelle=E_3;
          }
          if(lcd_key==btnSELECT){
            E_actuelle=E_5;
          }
        }
        break;
      }
      case E_3:
      {
        lcd.clear(); 
        lcd.setCursor(0, 0);   // Positionner le curseur au début
        lcd.print("  TEMPS");
        lcd.setCursor(0, 1);   // Positionner le curseur au début
        lcd.print("> TEMPERATURE");
        lcd.setCursor(15, 1);
        lcd.write(byte(0));
        if (currentMillis - previousMillis >= interval/3) {
          previousMillis = currentMillis; // Mettre à jour le temps précédent
          previousMillis = currentMillis; // Mettre à jour le temps précédent
          if(lcd_key==btnUP){
            E_actuelle=E_2;
          }
          if(lcd_key==btnDOWN){
            E_actuelle=E_4;
          }
          if(lcd_key==btnSELECT){
            E_actuelle=E_6;
          }
        }
        break;
      }
      case E_4:
      {
        lcd.clear(); 
        lcd.setCursor(0, 0);   // Positionner le curseur au début
        lcd.print("> SORTIE");
        lcd.setCursor(15, 1);
        lcd.write(byte(1));
        if (currentMillis - previousMillis >= interval/3) {
          previousMillis = currentMillis; // Mettre à jour le temps précédent
          if(lcd_key==btnUP){
          E_actuelle=E_3;
          }
        if(lcd_key==btnSELECT){
            lcd.clear(); 
            Serial.print("etape 4---->");
            Serial.print(currentMillis);
            E_actuelle=E_1;
          }
        }

        break;
      }
      case E_5:
      {
        lcd.clear(); 
        lcd.setCursor(0, 0);   // Positionner le curseur au début
        lcd.print("Modifier TEMPS");
        lcd.setCursor(0, 1);   // Positionner le curseur au début
        lcd.print("00 J 00 H 00 Mn");
        break;
      }
      case E_6:
      {
        lcd.clear(); 
        lcd.setCursor(0, 0);   // Positionner le curseur au début
        lcd.print("Modifier TEmperature");
        lcd.setCursor(0, 1);   // Positionner le curseur au début
        lcd.print("000 C");
        break;
      }
  }


}
void Tache_acquisition(){
}
// Fonction de lecture des touches
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);   // Lecture du port analogique
 // Les valeurs qui suivent doivent être adaptées au shield
 if (adc_key_in > 1000) return btnNONE;   // En principe 1023 quand aucune touche n'est pressée
 if (adc_key_in < 50)   return btnRIGHT;     // 0
 if (adc_key_in < 195)  return btnUP;        // 99
 if (adc_key_in < 380)  return btnDOWN;      // 255
 if (adc_key_in < 555)  return btnLEFT;      // 409
 if (adc_key_in < 790)  return btnSELECT;    // 640
 return btnNONE;
}
/*************************************************
les divers initalisation materiel
 ******************************************************/
void Initilisation_materiel(){
  lcd.begin(16, 2);              // Démarrage de l'écran
  lcd.setCursor(0,0);            // Positionnement du curseur au début
  lcd.print("Initialisation..."); // Message
  /*initialisation des etat de tache*/
  etat_tache[0]=true;
  etat_tache[1]=false;
  E_actuelle=E_1;
  Serial.begin(9600);
}
/*************************************************
configuration watchdog
******************************************************/
void Initialisation_inter() {
    TCCR2A = 0; // Mode normal
    TCCR2B = 0b00000111; // Prescaler 1024
    TIMSK2 = 0b00000001; // Activation des interruptions overflow
    TCNT2 = 256 - 200; // Ajuster la valeur pour une durée correcte
    sei(); // Activer les interruptions globales
}
