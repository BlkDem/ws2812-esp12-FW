void setIO()
{
  IOLEDPIN = HTTP.arg("IOLEDPIN").toInt();
  BOOTLEDPIN = HTTP.arg("BOOTLEDPIN").toInt();
  RELAYPIN4 = HTTP.arg("RELAYPIN4").toInt();
  BUTTONPIN = HTTP.arg("BUTTONPIN").toInt();
  PWMPIN = HTTP.arg("PWMPIN").toInt();
  PIN_SDA = HTTP.arg("PIN_SDA").toInt();
  PIN_SCL = HTTP.arg("PIN_SCL").toInt();
  IO_inversed = HTTP.arg("IO_inversed").toInt();
  BOOT_inversed = HTTP.arg("BOOT_inversed").toInt();
}

void IOSetup()
{
  //analogWriteFreq(100);
  /*pinMode(RELAYPIN4, OUTPUT);
  pinMode(IOLEDPIN, OUTPUT);
  pinMode(BOOTLEDPIN, OUTPUT);
  pinMode(TAHOPIN, INPUT_PULLUP);
  if (EnablePWM) 
  {    
    attachInterrupt(digitalPinToInterrupt(TAHOPIN), rpm, RISING);
  }
  pinMode(PWMPIN, OUTPUT);
  digitalWrite(PWMPIN, LOW);
  //SetPWM(500);
  pinMode(BUTTONPIN, INPUT);
  digitalWrite(BUTTONPIN, HIGH);
  if (IO_inversed==1)
  {
    digitalWrite(IOLEDPIN, HIGH);
  }
  if (BOOT_inversed==1)
  {
    digitalWrite(BOOTLEDPIN, HIGH);
  }*/
}

void BlinkAPMode()
{
  BootBlink(300); HTTP.handleClient(); BootBlink(50); HTTP.handleClient(); BootBlink(50); HTTP.handleClient();
  BootBlink(300); HTTP.handleClient(); BootBlink(50); HTTP.handleClient(); BootBlink(50); HTTP.handleClient();
}

void BootBlink(int _delay)
{
  if (UseStripAsActivityLed)
  {
    strip.setPixelColor( TOTALPIXELS-1, strip.Color( r2, g2, b2));
    strip.show();
    delay(_delay);
    strip.setPixelColor( TOTALPIXELS-1, strip.Color( 0, 0, 0));
    strip.show();
    RestoreSecondZone();
    return;
  }
  if (BOOT_inversed==1)
  {
    digitalWrite(BOOTLEDPIN, LOW);
    delay(_delay);
    digitalWrite(BOOTLEDPIN, HIGH);    
  }
  else
  {
    digitalWrite(BOOTLEDPIN, HIGH);
    delay(_delay);
    digitalWrite(BOOTLEDPIN, LOW);
  }
}

void IOBlink(int _delay)
{

  if (IO_inversed==1)
  {
    digitalWrite(IOLEDPIN, LOW);
    delay(_delay);
    digitalWrite(IOLEDPIN, HIGH);
  }
  else
  {
    digitalWrite(IOLEDPIN, HIGH);
    delay(_delay);
    digitalWrite(IOLEDPIN, LOW);
  }
}


void IOState(int _state)
{
  digitalWrite(IOLEDPIN, _state);
}
