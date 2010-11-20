#ifndef INT_EXT_H
#define INT_EXT_H

#define RISING_EDGE  1  //Le pin passe de 0 � 1
#define FALLING_EDGE 2  //Le pin passe de 1 � 0
#define ANY_CHANGE	 3  //Le pin passe soit de 0 � 1 soit de 1 � 0
#define OFF			 4  //L'interruption est desactiv�e

void configure_INT0(unsigned char, void (* function) (void));  //PIND2
void configure_INT1(unsigned char, void (* function) (void));  //PIND3

#endif
