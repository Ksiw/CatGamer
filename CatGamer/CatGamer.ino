/*
 Name:		CatGamer.ino
 Created:	19.01.2019 11:49:53
 Author:	ken93
*/


#define SERVO_PIN PB3     // пин серво
#define GAME 1000 // 1000
#define SLEEP 1200  //2400 //
#include "SimpleServo.h"
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

uint32_t mainTimer, myTimer;
SimpleServo myServo;

#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC

uint32_t game = GAME;
uint32_t sleep = SLEEP;

void setup()
{
// все пины как входы, экономия энергии
	//for (byte i = 0; i < 6; i++)
	//{
	//	pinMode(i, INPUT);
	//}
	adc_disable();          // отключить АЦП (экономия энергии)

	//wdt_reset();            // инициализация ватчдога
	//wdt_enable(WDTO_1S);    // разрешаем ватчдог
	//// 15MS, 30MS, 60MS, 120MS, 250MS, 500MS, 1S, 2S, 4S, 8S

	//WDTCR |= _BV(WDIE);     // разрешаем прерывания по ватчдогу. Иначе будет резет.
	sei();                  // разрешаем прерывания
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // максимальный сон

	myServo.attach(SERVO_PIN);

}

void loop()
{
	while (game)
	{
		myServo.write(random(100, 335));
		delay(random(45, 500));
		myServo.write(random(100, 335));
		delay(random(45, 300));
		myServo.write(random(45, 135));
		delay(random(45, 3000));
		game--;
	}
	
	game = GAME;
	myServo.detach();

	while (sleep)
	{
		myServo.detach();
		WDTCR |= _BV(WDIE);
		wdt_reset();            // инициализация ватчдога
		wdt_enable(WDTO_8S);    // разрешаем ватчдог
		WDTCR |= _BV(WDIE);

		sleep_enable();   // разрешаем сон
		sleep_cpu();      // спать!
		sleep--;
	}

	sleep = SLEEP;
	myServo.attach(SERVO_PIN);


}

ISR(WDT_vect)
{
	WDTCR |= _BV(WDIE); // разрешаем прерывания по ватчдогу. Иначе будет реcет.
}
