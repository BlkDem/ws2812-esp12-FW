void setup_pwm()
{
  //cycle_length = 1000000/PWM_FREQ; // the length of a single cycle of the PWM signal
}

void sPwmUpdate()
{
  sPwmCounter++;  
  if (sPwmCounter>=10)
  {
    sPwmCounter=0;
  }
  if (sPwmCounter <= duty)
  {
    digitalWrite(PWMPIN, HIGH);
  }
  else
  {
    digitalWrite(PWMPIN, LOW);
  }
}
