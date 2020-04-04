/*-------------------------------------------------------------|
  Name:                 Slang en ladder game voor blinden
  Copyright:            
  Author:               Djalil Shariefie
  Date:                 05-03-20        
  Description:          Versie 1.0

  Uitleg spel:          Welkom bij de slangen en ladders!
                        Het spel is simpel en iedereen kan het spelen. Laten we beginnen.
                        Voor je ligt een bord met 6 knoppen; 1 start knop, 1 reset knop en 
                        2 knoppen voor ierdere speler. Met de rechter knop kun je dubbelsteen gooien en met de
                        linker knop wordt de positie van je vakje opgevraagd.
                        Er zijn 100 vakjes, je begint op vakje 1 en moet op vakje 100 eindigen om te kunnen winnen.
                        Met een druk op de knop kun je de dubbelsteen gooien en de plaaats waar je eindigd wordt
                        via een speaker opgenoemd. Let op voor de slangen, als je opgegeten wordt door een slang,
                        dan moet je een aantal stappen terug. Er zijn ook ladders in de spel, als je hierop belandt
                        dan klim je een aantal stappen op. Degene die het eerst op vakje 100 aankomt wint het spel.

|--------------------------------------------------------------*/

#include <time.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Digitale poorten en vaste waardes toewijzen
#define speler1 2           //Pushbutton voor speler 1
#define speler2 3           //Pushbutton voor speler 2
#define positie1 4          //Knop om de positie op te vragen voor speler 1
#define positie2 5          //Knop om de positie op te vragen voor speler 2
#define startknop 6         //Startknop
#define restart 7           //restarten van het spel
#define N 100               //Grootte van de spelbord
#define mp3delay 1200
#define ledPin 13           // Interne LED (voor testen!)


// Variables die veranderen.
char buttonTeller = 0;          // counter for the number of button presses
char buttonStatus = 0;          // current state of the button
char lastbuttonStatus = 0;      // previous state of the button
char buttonTeller2 = 0;
char buttonStatus2 = 0;
char lastbuttonStatus2 = 0;
char positieSpeler1 = 1;
char positieSpeler2 = 1;
char nieuwPositieSpeler1 = 0;
char nieuwPositieSpeler2 = 0;
char positie = 0;
char newi =0;

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX  Poorten voor geluidspeler
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  pinMode(buttonPin, INPUT);                // Initialize de knoppen als input:
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(ledPin, OUTPUT);                  // initialize de LED als output:
  Serial.begin(9600);                       // initialize serial communicatie:
  randomSeed(analogRead(5));                // Elke keer als programma start nieuwe random getallen creeren.
  
  // DFPlayer settings
  mySoftwareSerial.begin(9600);
  !myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(8);                   //Set volume value. van 0 tot 30
}


void loop() 
{
  if ((nieuwPositieSpeler1 <N) && (nieuwPositieSpeler2 < N))
  {
    player1();
    positionStatus1(positie);
    player2();
    positionStatus2(positie);
  }
  else
  winnaar();
}
//------------------------------------------------------------------------
// Functie voor winnende speler:
//------------------------------------------------------------------------
char winnaar (void)
{
  char i = 0;
  char j = 0;
  while ((i = 0) && (j = 0))
  {
    if (nieuwPositieSpeler1 >= N)
    {
      Serial.println("Winnaar is speler ONE!");
      i = 1;
    }
    if (nieuwPositieSpeler2 >= N)
    {
      Serial.println("Winnaar is speler TWO");
      j = 1;
    }
  }
  
}
//------------------------------------------------------------------------
// Functie voor het gooien van dubbelsteen 2:
//------------------------------------------------------------------------
char Dubbelsteen (void)
{
  char num;
  num = random(1, 7);
  return num;
}
//------------------------------------------------------------------------
// Functie om de positie te bepalen:
//------------------------------------------------------------------------
char positionStatus1 (char positie)
{
  buttonStatus = digitalRead(buttonPin3);
  if (buttonStatus == HIGH)
  {
    char i;
    for (i=1; i <= N; i++)
    {
      if (positie == i)
      { 
        if (i == 5 || i == 12 || i == 22 || i == 35 || i == 41 || i == 65 || i == 88 || i == 13 || i == 34 || i == 50 || i == 55 || i == 73 || i == 81 || i == 97)
          checkpositie(i);
        else
        {      
          myDFPlayer.play(i);
          Serial.println("Positie is: ");
          Serial.println(i, DEC);
          delay(1500);
        }
      }
    }
  }
}
//------------------------------------------------------------------------
// Functie om de positie te bepalen:
//------------------------------------------------------------------------
char positionStatus2 (char positie)
{
  buttonStatus = digitalRead(buttonPin4);
  if (buttonStatus == HIGH)
  {
    char i;
    for (i=1; i <= N; i++)
    {
      if (positie == i)
      {
        if (i == 5 || i == 12 || i == 22 || i == 35 || i == 41 || i == 65 || i == 88 || i == 13 || i == 34 || i == 50 || i == 55 || i == 73 || i == 81 || i == 97)
          checkpositie(i);
        else
        {
          myDFPlayer.play(i);
          Serial.println("PPPPPPPPPPPPPPP: ");
          Serial.println(i, DEC);
          delay(mp3delay);
        }
      }
    }
  }
}
//------------------------------------------------------------------------
// Functie voor Speler 2:
//------------------------------------------------------------------------
char player1 (void)
{ 
  // Lees de pushbutton input pin:
  buttonStatus = digitalRead(buttonPin);
  
  // Vergrlijk de satus van de button
  if (buttonStatus != lastbuttonStatus) 
  {
    // Als de button HIGH is tellen (counter)
    if (buttonStatus == HIGH) 
    {
      // Gooi dubbelsteen
      char num = Dubbelsteen();
      
      // Als de huidige staat HIGH is dan is de button van OFF naar ON:
      buttonTeller++;
      positieSpeler1 += num;
      positie = positieSpeler1;
      
      // positionStatus1(positie);
      
      checkpositie1();
      positieSpeler1 = nieuwPositieSpeler1;
      Serial.print("Resultaat dubbelsteen: ");
      Serial.println(num, DEC);
      Serial.print("Nieuw positie speler EEN is: ");
      Serial.println(positieSpeler1, DEC);
      Serial.println("ON");
      Serial.print("Aantal keer op de knop gedrukt voor Player EEN: ");
      Serial.println(buttonTeller, DEC);
    }
    else 
    {
      // Test of de button status "OFF" gaat
      Serial.println("OFF");
    }
    // Delay nodig voor hapering te voorkomen
    delay(50);
  }
  // Onthoud de huidige status voor de volgende loop
  lastbuttonStatus = buttonStatus;
}
//------------------------------------------------------------------------
// Functie voor Speler 2:
//------------------------------------------------------------------------
char player2 (void)
{
  buttonStatus2 = digitalRead(buttonPin2);
  // compare the buttonStatus to its previous state
  if (buttonStatus2 != lastbuttonStatus2) 
  {
  // if the state has changed, increment the counter
    if (buttonStatus2 == HIGH) 
    {
      char num = Dubbelsteen();
      // if the current state is HIGH then the button went from off to on:
      buttonTeller2++;
      checkpositie2();
      positieSpeler2 += num;
      positie = positieSpeler2;
      checkpositie2();  
      positieSpeler2 = nieuwPositieSpeler2;
      Serial.print("Resultaat dubbelsteen: ");
      Serial.println(num, DEC);
      Serial.print("Nieuw positie speler EEN is: ");
      Serial.println(positieSpeler2, DEC);
      Serial.println("ON");
      Serial.print("Aantal keer op de knop gedrukt voor Player TWEE: ");
      Serial.println(buttonTeller2, DEC);
    }
    else
    {
      // if the current state is LOW then the button went from on to off:
      Serial.println("OFF");
    }
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastbuttonStatus2 = buttonStatus2;
}
//------------------------------------------------------------------------
// Functie om de positie te bepalen:
//------------------------------------------------------------------------
char checkpositie1()
{
  switch(positie)
  {
//------------------------------------------------------------------------
// POSITIE VAN DE LADDERS!
//------------------------------------------------------------------------
    case 5:
      Serial.println("LADDER");
      nieuwPositieSpeler1 = 25;
      myDFPlayer.play(nieuwPositieSpeler1);
      break;
    case 12:
      Serial.println("LADDER");
      nieuwPositieSpeler1 = 30;
      myDFPlayer.play(nieuwPositieSpeler1);
      break;
    case 22:
      Serial.println("LADDER");
      nieuwPositieSpeler1 = 37;
      myDFPlayer.play(nieuwPositieSpeler1);
      break;
    case 35:
      Serial.println("LADDER");
      nieuwPositieSpeler1 = 52;
      myDFPlayer.play(nieuwPositieSpeler1);
      break;
    case 41:
      Serial.println("LADDER");
      nieuwPositieSpeler1 = 61;
      myDFPlayer.play(nieuwPositieSpeler1);
      break;
    case 65:
      Serial.println("LADDER");
      nieuwPositieSpeler1 = 78;
      myDFPlayer.play(nieuwPositieSpeler1);
      break;
    case 88:
      Serial.println("LADDER");
      nieuwPositieSpeler1 = 96;
      myDFPlayer.play(nieuwPositieSpeler1);
      break;      
//------------------------------------------------------------------------
// POSITIE VAN DE SLANGEN!
//------------------------------------------------------------------------ 
    case 14:
      Serial.println("SLANG");
      nieuwPositieSpeler1 = 6;
      myDFPlayer.play(nieuwPositieSpeler1);
      break;    
    case 34:
      Serial.println("SLANG");
      nieuwPositieSpeler1 = 20;
      myDFPlayer.play(nieuwPositieSpeler1);
      break;
    case 50:
      Serial.println("SLANG");
      nieuwPositieSpeler1 = 28;
      myDFPlayer.play(nieuwPositieSpeler1);
      break;
    case 55:
      Serial.println("SLANG");
      nieuwPositieSpeler1 = 40;
      myDFPlayer.play(nieuwPositieSpeler1);
      break;
    case 73:
      Serial.println("SLANG");
      nieuwPositieSpeler1 = 54;
      myDFPlayer.play(nieuwPositieSpeler1);
      break;
    case 81:
      Serial.println("SLANG");
      nieuwPositieSpeler1 = 56;
      myDFPlayer.play(nieuwPositieSpeler1);
      break;
    case 97:
      Serial.println("SLANG");
      nieuwPositieSpeler1 = 79;
      myDFPlayer.play(nieuwPositieSpeler1);
      break;
      default:
    return nieuwPositieSpeler1 = positieSpeler1;
  }
}
//------------------------------------------------------------------------
// Functie om de positie te bepalen:
//------------------------------------------------------------------------
char checkpositie2()
{ 
  switch(positie)
  {
//------------------------------------------------------------------------
// POSITIE VAN DE LADDERS!
//------------------------------------------------------------------------ 
    case 5:
      Serial.println("LADDER");
      nieuwPositieSpeler2 = 25;
      myDFPlayer.play(nieuwPositieSpeler2);
      delay(mp3delay);
      break;
    case 12:
      Serial.println("LADDER");
      nieuwPositieSpeler2 = 30;
      myDFPlayer.play(nieuwPositieSpeler2);
      delay(mp3delay);
      break;
    case 22:
      Serial.println("LADDER");
      nieuwPositieSpeler2 = 37;
      myDFPlayer.play(nieuwPositieSpeler2);
      delay(mp3delay);
      break;
    case 35:
      Serial.println("LADDER");
      nieuwPositieSpeler2 = 52;
      myDFPlayer.play(nieuwPositieSpeler2);
      delay(mp3delay);
      break;
    case 41:
      Serial.println("LADDER");
      nieuwPositieSpeler2 = 61;
      myDFPlayer.play(nieuwPositieSpeler2);
      delay(mp3delay);
      break;
    case 65:
      Serial.println("LADDER");
      nieuwPositieSpeler2 = 78;
      myDFPlayer.play(nieuwPositieSpeler2);
      delay(mp3delay);
      break;
    case 88:
      Serial.println("LADDER");
      nieuwPositieSpeler2 = 96;
      myDFPlayer.play(nieuwPositieSpeler2);
      delay(mp3delay);
      break;
//------------------------------------------------------------------------
// POSITIE VAN DE SLANGEN!
//------------------------------------------------------------------------ 
    case 14:
      Serial.println("SLANG");
      nieuwPositieSpeler2 = 6;
      myDFPlayer.play(nieuwPositieSpeler2);
      delay(mp3delay);
      break;    
    case 34:
      Serial.println("SLANG");
      nieuwPositieSpeler2 = 20;
      myDFPlayer.play(nieuwPositieSpeler2);
      delay(mp3delay);
      break;
    case 50:
      Serial.println("SLANG");
      nieuwPositieSpeler2 = 28;
      myDFPlayer.play(nieuwPositieSpeler2);
      delay(mp3delay);
      break;
    case 55:
      Serial.println("SLANG");
      nieuwPositieSpeler2 = 40;
      myDFPlayer.play(nieuwPositieSpeler2);
      delay(mp3delay);
      break;
    case 73:
      Serial.println("SLANG");
      nieuwPositieSpeler2 = 54;
      myDFPlayer.play(nieuwPositieSpeler2);
      delay(mp3delay);
      break;
    case 81:
      Serial.println("SLANG");
      nieuwPositieSpeler2 = 56;
      myDFPlayer.play(nieuwPositieSpeler2);
      delay(mp3delay);
      break;
    case 97:
      Serial.println("SLANG");
      nieuwPositieSpeler2 = 79;
      myDFPlayer.play(nieuwPositieSpeler2);
      delay(mp3delay);
      break;
      default:
    return nieuwPositieSpeler2 = positieSpeler2;
  }
}
//------------------------------------------------------------------------
// Functie om de positie te bepalen:
//------------------------------------------------------------------------
char checkpositie(char i)
{
  switch(i)
  {
//------------------------------------------------------------------------
// POSITIE VAN DE LADDERS!
//------------------------------------------------------------------------ 
    case 5:
      i = 25;
      Serial.println(i, DEC);
      Serial.println("LADDER");
      myDFPlayer.play(i);
      delay(mp3delay);
      break;
    case 12:
      i = 30;
      Serial.println(i, DEC);
      Serial.println("LADDER"); 
      myDFPlayer.play(i);
      delay(mp3delay);
      break;
    case 22:
      i = 37;
      Serial.println(i, DEC);
      Serial.println("LADDER"); 
      myDFPlayer.play(i);
      delay(mp3delay);
      break;
    case 35:
      i = 52;
      Serial.println(i, DEC);
      Serial.println("LADDER"); 
      myDFPlayer.play(i);
      delay(mp3delay);
      break;
    case 41:
      i = 61;
      Serial.println(i, DEC);
      Serial.println("LADDER");
      myDFPlayer.play();
      delay(mp3delay);
      break;
    case 65:
      i = 78;
      Serial.println(i, DEC);
      Serial.println("LADDER"); 
      myDFPlayer.play(i);
      delay(mp3delay);
      break;
    case 88:
      i = 96;
      Serial.println(i, DEC);
      Serial.println("LADDER"); 
      myDFPlayer.play(i);
      delay(mp3delay);
      break;
//------------------------------------------------------------------------
// POSITIE VAN DE SLANGEN!
//------------------------------------------------------------------------ 
    case 14:
      i = 6;
      Serial.println(i, DEC);
      Serial.println("SLANG");
      myDFPlayer.play(i);
      delay(mp3delay);
      break;    
    case 34:
      i = 20;
      Serial.println(i, DEC);
      Serial.println("SLANG");
      myDFPlayer.play(i);
      delay(mp3delay);
      break;
    case 50:
      i = 28;
      Serial.println(i, DEC);
      Serial.println("SLANG");
      myDFPlayer.play(i);
      delay(mp3delay);
      break;
    case 55:
      i = 40;
      Serial.println(i, DEC);
      Serial.println("SLANG");
      myDFPlayer.play(i);
      delay(mp3delay);
      break;
    case 73:
      i = 54;
      Serial.println(i, DEC);
      Serial.println("SLANG");
      myDFPlayer.play();
      delay(mp3delay);
      break;
    case 81:
      i = 56;
      Serial.println(i, DEC);
      Serial.println("SLANG");
      myDFPlayer.play(i);
      delay(mp3delay);
      break;
    case 97:
      i = 79;
      Serial.println(i, DEC);
      Serial.println("SLANG");
      myDFPlayer.play(i);
      delay(mp3delay);
      break;
      default:
    return newi = i;
  }
}
