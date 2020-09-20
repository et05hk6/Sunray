  // Ardumower Sunray 
// Copyright (c) 2013-2020 by Alexander Grau, Grau GmbH
// Licensed GPLv3 for open source use
// or Grau GmbH Commercial License for commercial use (http://grauonline.de/cms2/?page_id=153)

#include "sdserial.h"
#include <SD.h>



#ifndef SDCARD_SS_PIN
#define SDCARD_SS_PIN 4
#endif

#if defined(_SAM3XA_)                 // Arduino Due
  #define CONSOLE SerialUSB
#else
  #define CONSOLE Serial
#endif


#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
SDSerial sdSerial;
#endif



void SDSerial::begin(unsigned long baud){  
  logFileName = "";
  packetIdx = 0;
  sdStarted = false;
  sdActive = false;
  CONSOLE.begin(baud);
}  

void SDSerial::beginSD(){  

  int counter = 0;
  while (true){
    if (SD.begin(SDCARD_SS_PIN)) break;    
    CONSOLE.println("no SD card found");          
    //delay(300);
    counter++;
    if (counter > 2){
      return;
    }
  }
  CONSOLE.println("SD card found!");
  
  for (int i=1000; i < 9000; i++){
    logFileName = "log";
    logFileName += i;
    logFileName += ".txt";    
    if (!SD.exists(logFileName)) {
      CONSOLE.print("logfile: ");
      CONSOLE.println(logFileName);    
      logFile = SD.open(logFileName, FILE_WRITE);
      if (logFile){        
        sdStarted = true;         
      } else {
        CONSOLE.println("ERROR opening file for writing");
      }
      break; 
      //SD.remove(UPDATE_FILE);
      //updateFile.close();
      //uint32_t updateSize = updateFile.size();
      //updateFile.seek(SDU_SIZE);      
    }
  }  
}

size_t SDSerial::write(uint8_t data){
  if ((sdStarted) && (!sdActive)) {
    sdActive = true;
    packetBuffer[packetIdx] = char(data);
    packetIdx++;
    if (packetIdx == 99){
      packetBuffer[packetIdx] = '\0';      
      logFile.write(packetBuffer);              
      logFile.flush();
      packetIdx = 0;            
    }
    sdActive = false;
  }  
  CONSOLE.write(data);
  return 1; 
}
  
  
int SDSerial::available(){
  return CONSOLE.available();
}


int SDSerial::read(){
  return CONSOLE.read();
}


int SDSerial::peek(){
  return CONSOLE.peek();
}

void SDSerial::flush(){  
  CONSOLE.flush();    
}



