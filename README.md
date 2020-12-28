# OSEK-assignment
Assignment of Operating System

## Goal

The goal of this lab is implementing a Morse-based Twitter.

The system sends Tweets composed of maximum 140 characters
* For the sake of simplicity we assume only uppercase characters
* Numbers and punctuation characters are not allowed.

The systems sends a pre-defined set of messages, one after the other
* Each message is sent (possibly multiple times) for a duration of 180 seconds
* After 180 seconds since starting displaying a message, the display is interrupted for 0.5 seconds, and then the display of the next message is started

The pre-defined messages are (from Fortune Cookie):
* A FEATHER IN THE HAND IS BETTER THAN A BIRD IN THE AIR
* A SHORT PENCIL IS USUALLY BETTER THAN A LONG MEMORY ANY DAY
* ACCEPT SOMETHING THAT YOU CANNOT CHANGE AND YOU WILL FEEL BETTER
* ADVENTURE CAN BE REAL HAPPINESS
* ALL THE EFFORT YOU ARE MAKING WILL ULTIMATELY PAY OFF

Remember that:
* Morse coding
* The dot is coded as symbol “ 1 ”
* The dash is coded as symbol “111”
* A pause “ 0 ” shall be inserted after each symbol
* After each codeword an inter-codeword pause shall be
* inserted, corresponding to “ 000 ”
* After each word, an inter-word pause shall be inserted,
* corresponding to “ 0000 ”
* Each bit (“ 0 ”/” 1 ”) shall be displayed on a LED (or using
* printf)
* The bit timing is 100msec

Propose a suitable architecture for generating the needed
bit timing, the message timing, and implement the
embedded system using either OSEK (EE VM, or EE
Arduino, or Trampoline) or Micrium μC/OS-III.

## Morse Code
![GitHub Logo](https://i.pinimg.com/originals/b7/b0/b0/b7b0b08627912646babaa4b375a445cf.jpg)
