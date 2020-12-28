#include "tpl_os.h"
#include "Arduino.h"
#include "avr/pgmspace.h"

#include <string.h>

#define M 5	// number of message
#define N 5	// number of char in string
#define LED 13
#define ENDWORD '/'

#define STARTALARM 488
#define REPEATALARM 98

//#define DEBUG
#define DURATIONTASKS

static char str[N] = "10";  			// str to print
static char MorseCoding[N] = ".-"; 		// translate letter in . and -
static byte ns = 0;						// sentece to code
static byte j = 0;						// letter to translate
static byte nb = 0; 					// position of symbol to translate
static byte nc = 0;						// position to print of 0 and 1

// sentebces to print
const static char string_0[] PROGMEM = "A FEATHER IN THE HAND IS BETTER THAN A BIRD IN THE AIR";
const static char string_1[] PROGMEM = "A SHORT PENCIL IS USUALLY BETTER THAN A LONG MEMORY ANY DAY";
const static char string_2[] PROGMEM = "ACCEPT SOMETHING THAT YOU CANNOT CHANGE AND YOU WILL FEEL BETTER";
const static char string_3[] PROGMEM = "ADVENTURE CAN BE REAL HAPPINESS";
const static char string_4[] PROGMEM = "ALL THE EFFORT YOU ARE MAKING WILL ULTIMATELY PAY OFF";

// morse code for letter
const static char cod_A[] PROGMEM = ".-";
const static char cod_B[] PROGMEM = "-...";
const static char cod_C[] PROGMEM = "-.-.";
const static char cod_D[] PROGMEM = "-..";
const static char cod_E[] PROGMEM = ".";
const static char cod_F[] PROGMEM = "..-.";
const static char cod_G[] PROGMEM = "--.";
const static char cod_H[] PROGMEM = "....";
const static char cod_I[] PROGMEM = "..";
const static char cod_J[] PROGMEM = ".---";
const static char cod_K[] PROGMEM = "-.-";
const static char cod_L[] PROGMEM = ".-..";
const static char cod_M[] PROGMEM = "--";
const static char cod_N[] PROGMEM = "-.";
const static char cod_O[] PROGMEM = "---";
const static char cod_P[] PROGMEM = ".--.";
const static char cod_Q[] PROGMEM = "--.-";
const static char cod_R[] PROGMEM = ".-.";
const static char cod_S[] PROGMEM = "...";
const static char cod_T[] PROGMEM = "-";
const static char cod_U[] PROGMEM = "..-";
const static char cod_V[] PROGMEM = "...-";
const static char cod_W[] PROGMEM = ".--";
const static char cod_X[] PROGMEM = "-..-";
const static char cod_Y[] PROGMEM = "-.--";
const static char cod_Z[] PROGMEM = "--..";

// translate symbol
const static char morse_0[] PROGMEM = "1110";
const static char morse_1[] PROGMEM = "10";
const static char morse_2[] PROGMEM = "0000";
const static char morse_3[] PROGMEM = "00";

const char *const msg[] PROGMEM = { string_0, string_1, string_2, string_3, string_4 };

const char *const code[] PROGMEM = {
									cod_A,cod_B,cod_C,cod_D,cod_E,cod_F,cod_G,cod_H,cod_I,cod_J,cod_K,
									cod_L,cod_M,cod_N,cod_O,cod_P,cod_Q,cod_R,cod_S,cod_T,cod_U,
									cod_V,cod_W,cod_X,cod_Y,cod_Z
								};

const char *const codeMorse[] PROGMEM = { morse_0, morse_1, morse_2, morse_3 };

static char *sentence = (char *)msg[0];
static char c = 'A';

#ifdef DEBUG
	static unsigned long time;
#endif

#ifdef DURATIONTASKS
	static unsigned long start, end;
#endif	

DeclareAlarm(alarmToPrint);
DeclareAlarm(alarmToSentence);

DeclareTask(newSentenceTask);
DeclareTask(codTask);
DeclareTask(translateTask);
DeclareTask(prinTask);

void setup()
{
	// initialize digital pin 13 as an output.
	pinMode(LED, OUTPUT);
	#if defined(DEBUG) || defined(DURATIONTASKS)
		Serial.begin(115200);
	#endif
}

TASK(newSentenceTask)
{
	#ifdef DURATIONTASKS
		start = micros();
	#endif

	CancelAlarm(alarmToPrint);
	// start againafter 500 ms and repeat each 100 ms
	SetRelAlarm(alarmToPrint, STARTALARM, REPEATALARM); 
	
	// don't send nothing
	digitalWrite(LED, LOW);

	ns++; 			// upgrade sentence to print
	if (ns == M) 
		ns = 0; 	// if arrive ns equal to M we start again from 0
	j = 0; 			// start at beginning of sentence
	nc = 0;			// start at beginning of string's bits

	// if prinTask is active it don't print nothing
	str[0] = ENDWORD;
	// second assignment to not activated translateTask and avoid possible problems
	str[1] = ENDWORD;

	// pointer to message to send
	sentence = (char *)pgm_read_word(&(msg[ns]));
		
	#ifdef DEBUG
		Serial.print("\n\n\n NEW SENTENCE NUMBER ");
		Serial.print(ns+1);
		Serial.print(" at time ");
	  	time = millis();
	  	Serial.print(time);
	  	Serial.print(" microsecods ");
		Serial.print("\n\n");
	#endif

	ActivateTask(codTask);

	#ifdef DURATIONTASKS
		end = micros();
		Serial.print("The NEW SENTENCE TASK has duration of: ");
		Serial.print(end - start);
		Serial.println(" microsecods");
	#endif
}

TASK(codTask)
{
	#ifdef DURATIONTASKS
		start = micros();
	#endif

	// read letter
	c = pgm_read_byte_near(sentence + j++);

	// if there is a space or the end of our message
	if (c == ' ' || c == '\0') {
		// this will translate properly later
		MorseCoding[0] = ENDWORD;
		MorseCoding[1] = '\0';
		if (c == '\0')
			j = 0;                                                                                                                                                                                                                                                                                                                                                                                                                            
	}
	else {
		// if not take directly morse code value from FLASH memory
		strcpy_P(MorseCoding, (char *)pgm_read_word(&(code[c-'A'])));
	}

	nb = 0;	// start at beginning of morse code
	ActivateTask(translateTask);

	#ifdef DURATIONTASKS
		end = micros();
		Serial.print("The COD TASK has duration of: ");
		Serial.print(end - start);
		Serial.println(" microsecods");
	#endif
}

TASK(translateTask)
{	
	#ifdef DURATIONTASKS
		start = micros();
	#endif

	// if we are at the end of out codeword
	if (MorseCoding[nb] == '\0')
		strcpy_P(str, (char *)pgm_read_word(&(codeMorse[3])));
	else
		// if not take directly string's bits from FLASH memory
		strcpy_P(str, (char *)pgm_read_word(&(codeMorse[MorseCoding[nb]-'-'])));


	if (MorseCoding[nb] == '\0' || MorseCoding[nb] == ENDWORD) {
		#ifdef DURATIONTASKS
			end = micros();
			Serial.print("The TRANSLATE TASK has duration of: ");
			Serial.print(end - start);
			Serial.println(" microsecods");
		#endif
		// if the codeword end translate a new letter
		ActivateTask(codTask);
	} else { 
		nb++;

		#ifdef DURATIONTASKS
			end = micros();
			Serial.print("The TRANSLATE TASK has duration of: ");
			Serial.print(end - start);
			Serial.println(" microsecods");
		#endif
	}
}

TASK(prinTask)
{
	#ifdef DURATIONTASKS
		start = micros();
	#endif

	#ifdef DEBUG
		if ((nb == 1 && nc == 0) ||(nb == 0 && nc == 0)|| (nb == 0 && nc == 0 && j == 0))  {
			if ((nb == 1 && nc == 0) ||(nb == 0 && nc == 0 && j != 0))
				c = pgm_read_byte_near(sentence + j-1);

			if (j == 1) {
				time = millis();
				Serial.print("\nStart Sentence number ");	  	
			  	Serial.println(ns+1);
			}
			Serial.print("\nLetter To code => ");

			if (nb == 0) {
				if (strcmp(str, "0000") == 0)
					Serial.print("SPACE");
				else if (strcmp(str, "00") == 0)
					Serial.print("END CODEWORD");			
				else			
					Serial.print(c);
			}
			else
				Serial.print(c);

			Serial.print(" with Morse Code: ");
			if (strcmp(str, "0000") == 0)
				Serial.print("SPACE");
			else if (strcmp(str, "00") == 0)
				Serial.print("END CODEWORD");			
			else			
				Serial.print(MorseCoding);

			Serial.print("\n");
	}
	#endif

	#ifdef DEBUG
		Serial.print("\t \\-- Print time[");
	  	time = millis();
	  	Serial.print(time);
	  	Serial.print("] => ");
	#endif

	// control if 1 or 0 to avoid print when there is a pause time 
	// and for other type of interferences
	if(str[nc] == '1') {
		digitalWrite(LED, HIGH);
		#ifdef DEBUG
			Serial.print("1\n");
		#endif
	}
	else if (str[nc] == '0')  {
		digitalWrite(LED, LOW);
		#ifdef DEBUG
			Serial.print("0\n");
		#endif
	}
	#ifdef DEBUG
		else Serial.print("print is disactivated or other problems\n");
	#endif

	nc++;

	// if it sends all bits then procede to translate another symbol
	if (str[nc] == '\0') {
		nc = 0;
		ActivateTask(translateTask);
	}

	#ifdef DURATIONTASKS
		end = micros();
		Serial.print("The PRINT TASK has duration of: ");
		Serial.print(end - start);
		Serial.println(" microsecods");
	#endif
}
