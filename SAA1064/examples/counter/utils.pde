void clear_display()
{
  Wire.beginTransmission(display_add1); // transmit to device 1
  Wire.send(0x00);            // sends instruction byte  
  Wire.send(B01000111);             // sends controldata value byte 
  Wire.send(0x00);
  Wire.send(0x00);
  Wire.send(0x00);
  Wire.send(0x00);
  Wire.endTransmission();     // stop transmitting
  delay(50);
  Wire.beginTransmission(display_add2); // transmit to device 2
  Wire.send(0x00);            // sends instruction byte  
  Wire.send(B01000111);             // sends controldata value byte  
  Wire.send(0x00);
  Wire.send(0x00);
  Wire.send(0x00);
  Wire.send(0x00);
  Wire.endTransmission();     // stop transmitting
  delay(50);

}

void display_num(byte chip_add,byte display,byte numero)
{
  Wire.beginTransmission(chip_add); // transmit to device 
  Wire.send((byte)display);
  switch(numero) {
  case 1: 
    {
      Wire.send(UNO);    
      break; 
    }
  case 2:
    {
      Wire.send(DUE);        
      break; 
    }
  case 3:
    {
      Wire.send(TRE);    
      break; 
    }
  case 4:
    {
      Wire.send(QUATTRO);    
      break; 
    }
  case 5:
    {
      Wire.send(CINQUE);    
      break; 
    }
  case 6:
    {
      Wire.send(SEI);    
      break; 
    }
  case 7:
    {
      Wire.send(SETTE);    
      break; 
    }
  case 8:
    {
      Wire.send(OTTO);    
      break; 
    }
  case 9:
    {
      Wire.send(NOVE);    
      break; 
    }
  case 0:
    {
      Wire.send(ZERO);
      break; 
    }
  default:
    {
      Wire.send(0);
    }

  }  
  Wire.endTransmission();     // stop transmitting
}
