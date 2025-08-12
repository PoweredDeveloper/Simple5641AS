#ifndef Simple5641_h
#define Simple5641_h

#include <Arduino.h>

const uint8_t nullCode = B00000000; // Null Code, nothing displayed
const uint8_t dotCode = B00000001; // Dot (.)
const uint8_t minusCode = B00000010; // Minus sign (-)

const uint8_t segmentCodes[] = {
                     B11111100, /* 0 */ 
                     B01100000, /* 1 */
                     B11011010, /* 2 */
                     B11110010, /* 3 */
                     B01100110, /* 4 */
                     B10110110, /* 5 */
                     B10111110, /* 6 */
                     B11100000, /* 7 */
                     B11111110, /* 8 */
                     B11110110  /* 9 */ };

typedef struct {
  char letter;
  uint8_t code;
} letter;

const letter segmentCodesLetters[] = {
                {'A', B11101110}, /* A */
                {'B', B00111110}, /* B */
                {'C', B10011100}, /* C */
                {'D', B01111010}, /* D */
                {'E', B10011110}, /* E */
                {'F', B10001110}, /* F */
                {'G', B10111100}  /* G */ };

class Simple5641AS 
{
  public:
    Simple5641AS(const uint8_t * segmentPins, const uint8_t * digitSelectionPins);
    
    void selectAllDigits();
    void selectDigit(int digit);
    
    void clean();
    
    void shortDisplayNumber(int n);
    void shortDisplayNumber(int n, boolean dot);

    void shortDisplayLetter(char l);
    void shortDisplayLetter(char l, boolean dot);

    void shortDisplayCode(uint8_t code);
    void shortDisplayCode(uint8_t code, boolean dot);

    void cycleNumber(long digitDisplayTime, int number, int dot);
    void cycleWord(long digitDisplayTime, char* word);
    void cycle(long digitDisplayTime, char* word);
    void customDisplayCycle(long digitDisplayTime, uint8_t codes[4]);
    
    void zeroAnimation(long cycleDelay, long cycles);
    void zeroAnimation();

    void displayNegativeNumber(long cycles, long cycleDelay, long digitDisplayTime, int number, int dot);
    void displayNumber(long cycles, long cycleDelay, long digitDisplayTime, int number, int dot);

    // Displating numbers
    void Display(long cycles, long cycleDelay, long digitDisplayTime, int number, int dot);
    void Display(long cycles, long cycleDelay, long digitDisplayTime, int number);
    void Display(int seconds, int number, int dot);
    void Display(int seconds, int number);
    void Display(int seconds, float number);

    // Displating codes
    void Display(long cycles, long cycleDelay, long digitDisplayTime, uint8_t codes[4], int dot);
    void Display(int seconds, uint8_t codes[4], int dot);

    // Displating words
    void Display(long cycles, long cycleDelay, long digitDisplayTime, char* word, int dot);
    void Display(int seconds, char* word, int dot);
    void Display(int seconds, char* word);

  private:
    int cleanNumber(int number);
    char* cleanWord(char* word);
    int decimalPlaces(float number);
    uint8_t _segmentPins[8];
    uint8_t _digitSelectionPins[4];
};

#endif
