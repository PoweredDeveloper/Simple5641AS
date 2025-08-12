#include "Simple5641AS.h"

// Simple5641AS constructor
Simple5641AS::Simple5641AS(const uint8_t * segmentPins, const uint8_t * digitSelectionPins) {
    memcpy(_segmentPins, segmentPins, sizeof(uint8_t) * 8);
    memcpy(_digitSelectionPins, digitSelectionPins, sizeof(uint8_t) * 4);

    for (int i = 0; i < sizeof(_segmentPins); i++) pinMode(_segmentPins[i], OUTPUT);
    for (int i = 0; i < sizeof(_digitSelectionPins); i++) pinMode(_digitSelectionPins[i], OUTPUT);

    for (int i = 0; i < sizeof(_segmentPins); i++) digitalWrite(_segmentPins[i], LOW);
    for (int i = 0; i < sizeof(_digitSelectionPins); i++) digitalWrite(_digitSelectionPins[i], LOW);
}

// Clean number to 4-deciminals
int Simple5641AS::cleanNumber(int number) {
    while (number >= 10000) number /= 10;
    return number;
}

// Clean words to 4-letters
char* Simple5641AS::cleanWord(char* word) {
    static char buffer[5];
    strncpy(buffer, word, 4);
    word[4] = '\0';
    return buffer;
}

// Get number of decimals on a number. Max: 4
int Simple5641AS::decimalPlaces(float number) {
    char buffer[12];
    dtostrf(number, 3, 4, buffer);

    int counter = strlen(buffer) - 1;
    while (buffer[counter] != '.' && buffer[counter] == '0' && counter != 0) {
        counter--;
    }

    int integerCounter = 0;
    while (buffer[integerCounter] != '.' && integerCounter + counter < strlen(buffer) + 1) {
        integerCounter++;
    }

    return counter - integerCounter;
}

// All pins LOW -> all pins selected
void Simple5641AS::selectAllDigits() {
    digitalWrite(_digitSelectionPins[0], LOW);
    digitalWrite(_digitSelectionPins[1], LOW);
    digitalWrite(_digitSelectionPins[2], LOW);
    digitalWrite(_digitSelectionPins[3], LOW);
}

// 1 pin LOW, rest of pins HIGH -> 1 pin selected
void Simple5641AS::selectDigit(int dig) {
    digitalWrite(_digitSelectionPins[0], HIGH);
    digitalWrite(_digitSelectionPins[1], HIGH);
    digitalWrite(_digitSelectionPins[2], HIGH);
    digitalWrite(_digitSelectionPins[3], HIGH);

    // Enable Selected Digit
    digitalWrite(_digitSelectionPins[dig], LOW); 
}

// all segments LOW
void Simple5641AS::clean() {
    for (int i = 0; i < 8; i++) digitalWrite(_segmentPins[i], LOW);
}

// number -> segment code -> digitalWrite(HIGH or LOW) for each pin. dot not activated
void Simple5641AS::shortDisplayNumber(int n) {
    for (int i = 0; i < 7; i++) digitalWrite(_segmentPins[i], (segmentCodes[n] & ( 1 << (7 - i) )) >> (7 - i) ? HIGH : LOW);
    digitalWrite(_segmentPins[7], LOW);
}

// number -> segment code ->  digitalWrite(HIGH or LOW) for each pin, dot included
void Simple5641AS::shortDisplayNumber(int n, boolean dot) {
    for (int i = 0; i < 7; i++) digitalWrite(_segmentPins[i], (segmentCodes[n] & ( 1 << (7 - i) )) >> (7 - i) ? HIGH : LOW);
    digitalWrite(_segmentPins[7], dot ? HIGH : LOW);
}

// char -> segment code -> digitalWrite(HIGH or LOW) for each pin. dot not activated
void Simple5641AS::shortDisplayLetter(char l) {
    l = toupper(l);

    for (int d = 0; d < sizeof(segmentCodesLetters) / sizeof(segmentCodesLetters[0]); d++) {
        if (segmentCodesLetters[d].letter == l) {
            for (int i = 0; i < 8; i++) digitalWrite(_segmentPins[i], (segmentCodesLetters[d].code & ( 1 << (7 - i) )) >> (7 - i) ? HIGH : LOW);
            break;
        }
    }

    digitalWrite(_segmentPins[7], LOW);
}

// char -> segment code -> digitalWrite(HIGH or LOW) for each pin, dot included
void Simple5641AS::shortDisplayLetter(char l, boolean dot) {
    l = toupper(l);

    for (int d = 0; d < sizeof(segmentCodesLetters) / sizeof(segmentCodesLetters[0]); d++) {
        if (segmentCodesLetters[d].letter == l) {
            for (int i = 0; i < 8; i++) digitalWrite(_segmentPins[i], (segmentCodesLetters[d].code & ( 1 << (7 - i) )) >> (7 - i) ? HIGH : LOW);
            break;
        }
    }

    digitalWrite(_segmentPins[7], dot ? HIGH : LOW);
}

// segment code -> digitalWrite(HIGH or LOW) for each pin, dot not included (only if in code)
void Simple5641AS::shortDisplayCode(uint8_t code) {
    for (int i = 0; i < 8; i++) digitalWrite(_segmentPins[i], (code & ( 1 << (7 - i) )) >> (7 - i) ? HIGH : LOW);
}

// segment code -> digitalWrite(HIGH or LOW) for each pin, dot included
void Simple5641AS::shortDisplayCode(uint8_t code, boolean dot) {
    if (dot) code |= 1;
    else code &= ~1;

    for (int i = 0; i < 8; i++) digitalWrite(_segmentPins[i], (code & ( 1 << (7 - i) )) >> (7 - i) ? HIGH : LOW);
}

// cycle function: display one number
void Simple5641AS::cycleNumber(long digitDisplayTime, int number, int dot) {
    number = cleanNumber(number);

    clean();
    for (int i = 0; i < 4; i++) {
        selectDigit(i);
        shortDisplayNumber((abs(number) / (int) pow(10, 3 - i)) % 10, dot == i);
        delayMicroseconds(digitDisplayTime);
        clean();
    }
}

// cycle function: display one 4-word
void Simple5641AS::cycleWord(long digitDisplayTime, char* word) {
    word = cleanWord(word);

    clean();
    for (int i = 0; i < 4; i++) {
        selectDigit(i);
        shortDisplayLetter(word[i]);
        delayMicroseconds(digitDisplayTime);
        clean();
    }
}

// cycle function
void Simple5641AS::cycle(long digitDisplayTime, char* word) {
    word = cleanWord(word);

    clean();
    for (int i = 0; i < 4; i++) {
        selectDigit(i);

        if (isdigit(word[i])) shortDisplayNumber(word[i] - '0'); // char to int
        else shortDisplayLetter(word[i]);

        delayMicroseconds(digitDisplayTime);
        clean();
    }
}

// customDisplayCycle function: display 4 symbols represented by 4 codes
void Simple5641AS::customDisplayCycle(long digitDisplayTime, uint8_t codes[4]) {
    clean();
    for (int i = 0; i < 4; i++) {
        selectDigit(i);
        shortDisplayCode(codes[i]);
        delayMicroseconds(digitDisplayTime);
        clean();
    }
}

// zeroAnimation function, with all parameters available
void Simple5641AS::zeroAnimation(long cycleDelay, long cycles) {
    int counter = 8;
    long cycleIterator = 0L;
    while (counter > 0) {
        while (cycleIterator < cycles) {
            clean();
            selectAllDigits();
            cycleIterator % 4 == 0 && counter % 2 == 0 ? shortDisplayNumber(0, false) : clean();
            
            delayMicroseconds(cycleDelay);
            cycleIterator++;
        }
        cycleIterator = 0;
        counter--;
    }
}

// zeroAnimation function, with default parameters
void Simple5641AS::zeroAnimation() {
    zeroAnimation(155, 100 * 16);
}

// Display number as negative MAXIMUM 3-DIGITS
void Simple5641AS::displayNegativeNumber(long cycles, long cycleDelay, long digitDisplayTime, int number, int dot) {
    number = abs(number) % 1000;

    long cycleIterator = 0L;
    while (cycleIterator < cycles) {
        clean();
        for (int i = 0; i < 4; i++) {
            int prev_digit = (i != 0) ? (number / (int) pow(10, 3 - (i - 1))) % 10 : 0;
            int digit = (number / (int) pow(10, 3 - i)) % 10;
            int next_digit = (i != 3) ? (number / (int) pow(10, 3 - (i + 1))) % 10 : 0;
            
            selectDigit(i);

            if (prev_digit == 0 && digit == 0 && next_digit != 0) shortDisplayCode(minusCode, dot == i);
            else shortDisplayCode(segmentCodes[digit], dot == i);

            delayMicroseconds(digitDisplayTime);
            clean();
        }
            
        delayMicroseconds(cycleDelay);
        cycleIterator++;
    }
}

// displayPositiveNumber function, with all parameters available
void Simple5641AS::displayNumber(long cycles, long cycleDelay, long digitDisplayTime, int number, int dot) {
    number = cleanNumber(number);

    long cycleIterator = 0L;
    while (cycleIterator < cycles) {
        clean();
        for (int i = 0; i < 4; i++) {
            selectDigit(i);
            shortDisplayNumber((number / (int) pow(10, 3 - i)) % 10, dot == i);
            delayMicroseconds(digitDisplayTime);
            clean();
        }
            
        delayMicroseconds(cycleDelay);
        cycleIterator++;
    }
}

// displayNumber function, with all parameters available
void Simple5641AS::Display(long cycles, long cycleDelay, long digitDisplayTime, int number, int dot) {
    if (number > 0) displayNumber(cycles, cycleDelay, digitDisplayTime, number, dot);
    else displayNegativeNumber(cycles, cycleDelay, digitDisplayTime, number, dot);
}

// display a number for a number of seconds, and indicate where is the dot placed
void Simple5641AS::Display(int seconds, int number, int dot) {
    Display(seconds * 690L, 400, 155, number, dot);
}

// display a integer number for a number of seconds, without any dots visible
void Simple5641AS::Display(int seconds, int number) {
    Display(seconds * 690L, 400, 155, number, 5);
}

// display a float number, the dot is put accordingly
void Simple5641AS::Display(int seconds, float number) {
    number = (float)((long)(number * 1000.0)) / 1000.0;
    int decimals = decimalPlaces(number);
    float modifiedNumber = number;
    for (int i = 0; i < decimals; i++) modifiedNumber *= 10;
    Display(seconds * 690L, 400, 155, (int)modifiedNumber, decimals == 0 ? 5 : 3 - decimals);
}

// displayCode function, with all parameters available
void Simple5641AS::Display(long cycles, long cycleDelay, long digitDisplayTime, uint8_t codes[4], int dot) {
    long cycleIterator = 0L;
    while (cycleIterator < cycles) {
        clean();
        for (int i = 0; i < 4; i++) {
            selectDigit(i);
            shortDisplayCode(codes[i]);
            delayMicroseconds(digitDisplayTime);
            clean();
        }
        cycleIterator++;
    }
}

// displayCode function, with 3 parameters available
void Simple5641AS::Display(int seconds, uint8_t codes[4], int dot) {
    Display(seconds * 690L, 400, 155, codes, dot);
}

// display word with all functionality
void Simple5641AS::Display(long cycles, long cycleDelay, long digitDisplayTime, char* word, int dot) {
    word = cleanWord(word);
    long cycleIterator = 0L;
    while (cycleIterator < cycles) {
        clean();
        for (int i = 0; i < 4; i++) {
            selectDigit(i);

            if (isdigit(word[i])) shortDisplayNumber(word[i] - '0', dot == i); // char to int
            else shortDisplayLetter(word[i], dot == i);
            
            delayMicroseconds(digitDisplayTime);
            clean();
        }
            
        delayMicroseconds(cycleDelay);
        cycleIterator++;
    }
}

// display a word for a number of seconds, and indicate where is the dot placed
void Simple5641AS::Display(int seconds, char* word, int dot) {
    Display(seconds * 690L, 400, 155, word, dot);
}

// display a word number for a number of seconds, without any dots visible
void Simple5641AS::Display(int seconds, char* word) {
    Display(seconds * 690L, 400, 155, word, 5);
}

