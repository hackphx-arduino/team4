//Make sure the dip switches are turned ON, and none of your shields are using pins A0,A1,A2,A3 or D4

#include <Shieldbot.h>	//includes the Shieldbot Library

Shieldbot shieldbot = Shieldbot();	//decares a Shieldbot object

#define ARR_LEN(x) (sizeof(x) / sizeof((x)[0]))

//#define DBGTEAM4

void setup(){
  Serial.begin(9600);//Begin serial comm for debugging
  shieldbot.setMaxSpeed(60,70);//255 is max, if one motor is faster than another, adjust values 
}

void loop()
{
  int s[5];
  
  s[4] = shieldbot.readS1();
  s[3] = shieldbot.readS2();
  s[2] = shieldbot.readS3();
  s[1] = shieldbot.readS4();
  s[0] = shieldbot.readS5();

  #define ___ 255     // don't care
  #define ON_ HIGH    // light
  #define OFF LOW     // dark
  
  #ifdef DBGTEAM4
  #define txt(x) ,x
  #else
  #define txt(x)
  #endif
  
  // Table that defines the behavior of the motors, based on the
  // sensors. The entries in the table either have to match the sensors
  // exactly, or they can 
  const struct lookup_t
  {
    int s[5];
    char lspeed, rspeed;
  #ifdef DBGTEAM4
    const char *desc;
  #endif  
  } lookups[] =
  {
    { { OFF, OFF, ON_, OFF, OFF },  127,  127 txt("1 center")  }, // 1 center light: straight ahead
    { { OFF, ON_, OFF, ON_, OFF },  127,  127 txt("2 center")  }, // 2 center lights: straight ahead
    { { OFF, ON_, ON_, ON_, OFF },  127,  127 txt("3 center")  }, // 3 center lights: straight ahead
    { { ON_, ON_, ON_, ON_, ON_ },    0,    0 txt("all lit")   }, // All lit: stop, we don't know where to go
    { { ON_, ON_, ON_, ___, OFF },    0,  127 txt("3/4 left")  }, // 3 or 4 left lights: soft left
    { { OFF, ___, ON_, ON_, ON_ },  127,    0 txt("3/4 right") }, // 3 or 4 right lights: soft right
    { { OFF, ON_, ___, OFF, OFF },  127,    0 txt("1/2 left")  }, // 1 or 2 left lights: soft left
    { { OFF, OFF, ___, ON_, OFF },    0,  127 txt("1/2 right") }, // 1 or 2 right lights: soft right
    { { ON_, ON_, OFF, OFF, OFF },    0,  127 txt("2 left")    }, // 2 left lights: soft left
    { { OFF, OFF, OFF, ON_, ON_ },  127,    0 txt("2 right")   }, // 2 right lights: soft right
    { { ON_, OFF, OFF, OFF, OFF }, -128,  127 txt("1 left")    }, // 1 left light: hard left
    { { OFF, OFF, OFF, OFF, ON_ },  127, -128 txt("1 right")   }, // 1 right light: hard right
  };
  
  int found;
  for (unsigned lut = 0; lut < ARR_LEN(lookups); lut++)
  {
    const struct lookup_t *plookup = &lookups[lut];
    found = 1;

    for (unsigned index = 0; (found) && (index < 5); index++)
    {
      int wanted = plookup->s[index];

      if ( (wanted != ___) && (s[index] != wanted) )
      {
        found = 0;
        break;
      }
    }
    
    if (found)
    {
  #ifdef DBGTEAM4
      Serial.println(plookup->desc);
  #endif
      shieldbot.drive(plookup->lspeed, plookup->rspeed);
      break;
    }
  }
  
  if (!found)
  {
  #ifdef DBGTEAM4
    Serial.println("???");
  #endif
    shieldbot.drive(0, 0);
    delay(100);
  }
}

