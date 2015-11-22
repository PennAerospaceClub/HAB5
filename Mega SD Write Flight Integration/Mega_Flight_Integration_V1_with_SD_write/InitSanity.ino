//EXPECT TO HAVE SERIAL HANDSHAKE WITH TRACKUINO IMPLEMENTED LATER

//4.5 Sanity
boolean initiallySane()
{
  if(!(currAlt == -1) || !(lat == -1) || !(longit == -1)){
    GPSgivingDataInitially = true;
  }
  if(inBdryBox()){
    inBdry = true;
  }
  if(!isFalling()){
    falling = false;
  }
  if(arduinoSerial && GPSSerial){
    softwareSerialsInitially = true;
  }
}

boolean sanityCheck()
{
  boolean bdryBool = true;
  boolean fallingBool = true;
  boolean gpsBool = true;
  
  if (!inBdryBox()){
    bdryBool =  false;
  }
  if (isFalling()){
    fallingBool =  false;
  }
  if ((currAlt == -1) || (lat == -1) || (longit == -1)){
    gpsBool =  false;
  }

  sanitySerial_SD_LED(bdryBool, fallingBool, gpsBool);

  if(bdryBool && fallingBool && gpsBool){
    return true;
  }  
  else{
    return false;
  }  
}

//RED LIGHT: GPS NOT UPDATING
//YELLOW LIGHT: THINKS IT'S FALLING
//GREEN LIGHT: THINKS IT'S OUTSIDE BOUNDARY BOX
void sanitySerial_SD_LED(boolean bdryBool, boolean fallingBool, boolean gpsBool)
{
     boolean myBdryBool = bdryBool;
     boolean myFallingBool = fallingBool;
     boolean myGpsBool = gpsBool;
      if(!myBdryBool || !myFallingBool || !myGpsBool){
        //Serial.println("no sane");
        if(!redLightOn){
          redLightBlinkStop = millis() + 1000;
          digitalWrite(LED_RED, HIGH);
          digitalWrite(LED_GREEN, LOW);
          redLightOn = true;
        }
        else{
          if(millis() > redLightBlinkStop){
            redLightBlinkStop = 0;
            redLightOn = false;
            digitalWrite(LED_RED, LOW);
            digitalWrite(LED_GREEN, LOW);
          }
        }
      }  
      else{
          if(!greenLightOn){
          redLightBlinkStop = millis() + 1000;
          digitalWrite(LED_RED, LOW);
          digitalWrite(LED_GREEN, HIGH);
          greenLightOn = true;
        }
        else{
          if(millis() > greenLightBlinkStop){
            greenLightOn = false;
          }
        }
      }         
}