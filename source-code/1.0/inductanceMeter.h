void setupInductance()
{ 
  pinMode(pulseIN1, INPUT);
  pinMode(pulseIN2, INPUT);
  pinMode(triggerInductance, OUTPUT);
}

void cekInductance()
{
  digitalWrite(triggerA, LOW);
  digitalWrite(triggerB, LOW);
  
  digitalWrite(triggerInductance, LOW);
  delay(100);//give some time to charge inductor.
  digitalWrite(triggerInductance,HIGH);
  delayMicroseconds(100); //make sure resination is measured
  pulse = pulseIn(pulseIN1,LOW,5000);//returns 0 if timeout
  if(pulse > 0.1) //if a timeout did not occur and it took a reading:
  {     
//  #error insert your used capacitance value here. Delete this line after that
    capacitance = 11600.E-9; // - insert value here
     
    frequency = 1.E6/(2*pulse);
    inductance = 1./(capacitance*frequency*frequency*4.*3.14159*3.14159);//one of my profs told me just do squares like this
    inductance *= 1E6;
  
    //Serial print
    Serial.print("High for uS:");
    Serial.print( pulse );
    Serial.print("\tfrequency Hz:");
    Serial.print( frequency );
    Serial.print("\tinductance uH:");
    Serial.println( inductance );
    delay(1000);
  }

  digitalWrite(triggerInductance, LOW);
  delay(100);//give some time to charge inductor.
  digitalWrite(triggerInductance,HIGH);
  delayMicroseconds(100); //make sure resination is measured
  pulse2 = pulseIn(pulseIN2,LOW,5000);//returns 0 if timeout
  if(pulse2 > 0.1) //if a timeout did not occur and it took a reading:
  {     
//  #error insert your used capacitance value here. Delete this line after that
    capacitance2 = 11620.E-9; // - insert value here
     
    frequency2 = 1.E6/(2*pulse2);
    inductance2 = 1./(capacitance2*frequency2*frequency2*4.*3.14159*3.14159);//one of my profs told me just do squares like this
    inductance2 *= 1E6;
  
    //Serial print
    Serial.print("High2 for uS:");
    Serial.print( pulse2 );
    Serial.print("\tfrequency2 Hz:");
    Serial.print( frequency2 );
    Serial.print("\tinductance2 uH:");
    Serial.println( inductance2 );
    delay(1000);
  }
}
