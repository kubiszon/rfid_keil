#include "mbed.h"
#include "LCD_DISCO_F429ZI.h"
#include "ID12RFID.h"
#include "serial.h"
#include <string> // for std::string


LCD_DISCO_F429ZI lcd;
DigitalOut led1(LED1);

// Global variables
int currentUser;	
int appConnected = 0;
//Start UART
ID12RFID rfid(PD_2); 				// UART5
Serial HC06(USBTX, USBRX);	// UART1

void rfid_interrupt();
void bt_interrupt();

int main()
{      
	
	  BSP_LCD_SetFont(&Font20);
		lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"HELLO", CENTER_MODE);
    
		HC06.baud(9600);
		//BT interrupt
		HC06.attach(&bt_interrupt, Serial::RxIrq);
		HC06.printf("LOG: RFID is ready\n");
		led1 = 1;
	
		
	
    while(1)
    { 
			wait(1);
      led1 = !led1;    
    }
}

// Interupt Routine to read in data from serial port
void rfid_interrupt() {
	__disable_irq();
	led1=1;
	uint8_t text[30];
	if(rfid.readable()) {
		int id = rfid.read();	
		
		
		lcd.Clear(LCD_COLOR_WHITE);
		if (currentUser){
			HC06.printf("LOG: User ID : %d\n", currentUser) ;
			HC06.printf("LOG: Device ID : %d\n", id) ;
			sprintf((char*)text, "USER %d", currentUser);
		  lcd.DisplayStringAt(0, LINE(4), (uint8_t *)&text, CENTER_MODE);
			sprintf((char*)text, "BORROWED %d", id);
		  lcd.DisplayStringAt(0, LINE(5), (uint8_t *)&text, CENTER_MODE);
			currentUser = 0;
			wait(3);

		} 
		else {
			HC06.printf("UID: %d\n", id);
			bool userExists = true; // = HC06.read();
			if (userExists){ // UZYTKOWNIK ISTNIEJE
				currentUser = id;
				sprintf((char*)text, "HELLO %d", id);
		    lcd.DisplayStringAt(0, LINE(3), (uint8_t *)&text, CENTER_MODE);
		  	lcd.DisplayStringAt(0, LINE(4), (uint8_t *)"USE YOUR DEVICE", CENTER_MODE);
			} 
			else { //uzytkownik nie istnieje
					
			}
		}
	}
	__enable_irq();
}
 
// Interupt Routine to read in data from serial port
void bt_interrupt() {
	__disable_irq();
//////
			
			
			if (appConnected) {
					// command service routine
			}
			else {
				// wait for message "APP READY"
				char buffer[10];
				string expectedStr = "APP READY";
				if (HC06.readable()){
					HC06.gets(buffer, 10);
					HC06.puts(buffer);
				}
				string bufferStr(buffer);
				if (expectedStr == bufferStr) {
					lcd.Clear(LCD_COLOR_WHITE);
					lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"APP CONNECTED", CENTER_MODE);
					lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"USE YOUR CARD", CENTER_MODE);
					lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"TO START", CENTER_MODE);
					//RFID interrupt
					rfid._rfid.attach(&rfid_interrupt, Serial::RxIrq);
					appConnected = 1;
					}
				}
	__enable_irq();
}
	 
	 
/*	lcd.Clear(LCD_COLOR_BLUE);
      lcd.SetBackColor(LCD_COLOR_BLUE);
      lcd.SetTextColor(LCD_COLOR_WHITE);
      wait(0.3);
      lcd.DisplayStringAt(0, LINE(4), (uint8_t *)"DISCOVERY", CENTER_MODE);
      lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"STM32F429ZI", CENTER_MODE);
      wait(1);

      lcd.Clear(LCD_COLOR_GREEN);
      
      lcd.SetTextColor(LCD_COLOR_BLUE);
      lcd.FillRect(10, 20, 50, 50);
      wait(0.1);
      lcd.SetTextColor(LCD_COLOR_BROWN);
      lcd.FillCircle(80, 80, 50);
      wait(0.1);
      lcd.SetTextColor(LCD_COLOR_YELLOW);
      lcd.FillEllipse(150, 150, 50, 100);
      wait(0.1);
      lcd.SetTextColor(LCD_COLOR_RED);
      lcd.FillCircle(200, 200, 40);
      wait(1);

      lcd.SetBackColor(LCD_COLOR_ORANGE);
      lcd.SetTextColor(LCD_COLOR_CYAN);
      BSP_LCD_SetFont(&Font24);
      lcd.DisplayStringAt(0, LINE(7), (uint8_t *)"HAVE FUN !!!", CENTER_MODE);
*/
