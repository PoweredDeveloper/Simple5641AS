# Simple5641AS Arduino Library

With this library, you can easily control the 5641AS to display some number, display a countdown, count from 0 to 100..etc.

*THIS IS A FORK OF ORIGINAL Simple5641AS (https://github.com/adrian200223/Simple5641AS)*

## Inspiration

The library is inspired by the Test library (https://www.arduino.cc/en/Hacking/Libraries), the ShiftRegister 75HC595 library (https://github.com/Simsso/ShiftRegister74HC595/) and this video showing how the 5641AS works (https://www.youtube.com/watch?v=E4CjNPsPrnQ).


## How does it works

Here is an ascii art representing the 5641AS:
```javascript

                      D1             a             f            D2            D3              b
                      /              /             /            /             /               /
                      /              /             /            /             /               /
,.............................@.............................#............................/,............................@
,.                            @                             #                            /,                            @
,.           Digit 1          @           Digit 2           #           Digit 3          /,           Digit 4          @
,.                            @                             #                            /,                            @
,.        -@@@@@@@@@@@@.      @        -@@@@@@@@@@@@.       #        -@@@@@@@@@@@@.      /,        -@@@@@@@@@@@@.      @
,.       *@,     a    #@@.    @       &@@           @@@     #       @@%           @@(    /,      ,@@*          (@@.    @
,.      &@@           @@@  b  @       @@%           @@(     #      *@@*          (@@.    /,      #@@           &@@     @
,.  f   @@%           @@(     @      *@@*          (@@.     #      #@@           &@@     /,      @@&           @@%     @
,.     *@@*          (@@      @      %@@           @@@      #      @@&           @@%     /,     .@@*          *@@,     @
,.        *@@@@@@@@@*         @         *@@@@@@@@@*         #        *@@@@@@@@@*         /,        *@@@@@@@@@*         @
,.     @@&,.   g   .,@@#      @     @@&,.       .,@@#       #      @@&,.       .,@@#     /,     @@&,.       .,@@#      @
,.     @@*          *@@,      @     #@@           &@@       #     @@@           @@%      /,     @@(          ,@@*      @
,. e  #@@           &@@   c   @     @@@           @@%       #     @@(          ,@@*      /,    /@@.          #@@       @
,.    @@&           @@%       @    .@@#          ,@@*       #    /@@.          #@@       /,    @@@           @@&       @
,.   .@@#          *@@*       @    /@@.          #@@        #    @@@           @@&       /,    @@%          .@@/       @
,.      ;@@@@@@@@@@;   .@@&   @       ;@@@@@@@@@@;   .@@&   #      ;@@@@@@@@@@;   .@@&   /,      ;@@@@@@@@@@;   .@@&   @
,.            d         dot   @                             #                            /,                            @
,.                            @                             #                            /,                            @
,,............................@.............................%............................/,............................@
                      /              /             /            /             /               /
                      /              /             /            /             /               /
                      e              d            dot           c             g               D4
```


Taking a look at the first digit, it has the letters asociated with the segments. Each letter represents one segment. In order to make numbers, you need to figure out letter by letter if they are on or off. In the case of the 0 symbol:

---------------------------------
|    Segment    |   On or off?  |
| ------------- | ------------- |
|        a      |       On      |
|        b      |       On      |
|        c      |       On      |
|        d      |       On      |
|        e      |       On      |
|        f      |       On      |
|        g      |       Off     |
|       dot     |       Off     |
---------------------------------

Each of these letters is represented by a pin (pin a, pin b, pin c, pin c,...), as shown in the diagram above.


Then, what does D1, D2, D3 and D4 mean? Those are what I call, the 'digit control' pins. When you are outputting 1s and 0s to the segment pins (a,b,c,d,...), those pins determinate what digit will shine. D1 is for Digit1, D2 is for Digit 2, D3 is for digit 3 and D4 is for digit 4.

As there are no Vcc (5V or 3.3V, for example) nor ground pins, the mechanism behind is easy to decipher: those 'digit control' pins that are low will be the ground, and the segment pins will be the Vcc.



Finally, the last part. How do we show different numbers for each digit? Well, with this system, we can only display 1 different number at a time, but we can display it for a very short time and go on to the next one. Similarly to how video works, if a light has a high frequency of on/off cycle, we can only see it as "on". So, we will cycle through each digit, displaying it, then putting all pins to 0 and then going to the next digit.




## Limitations

As this code needs to cycle between digits at a very fast pace:
- It is difficult to execute code at the same time that a number is being displayed. This is because in the arudino board there is 1 proccessor, so there can only be 1 thing executing at a time. This means that if you want to do something while the 5641AS is displaying someting, you will have to implement your own cycle function, and balance the load between each iteration of the loop, in order to not cause delay in the numbers being displayed.


## How to use it

1. Download the zip
2. Add the library to your Arduino IDE (Program, Add library, Add .ZIP library)
3. Add `#include <Simple5641AS.h>` to the first lines of your code
4. Instanciate the display, like this (the numbers represent the pins on your arduino board): 
```c++
// segmentPins[] = { a, b, c, d, e, f, g, dot };
const uint8_t segmentPins[] = { 4, 6, 3, 10, 9, 2, 5, 13 };
// digitSelectionPins[] = { D1, D2, D3, D4 };
const uint8_t digitSelectionPins[] = { 12, 11, 8, 7 };
Simple5641AS component( segmentPins, digitSelectionPins );
```
5. Call the method you want on your code, for example:
```c++
  // Display example
  Serial.println("Display example");
  component.Display(1, "2bg4");
  component.zeroAnimation();
  delay(1000);
```


## Functions
```c++
    // constructor
    Simple5641AS(const uint8_t * segmentPins, const uint8_t * digitSelectionPins);
    
    // digit selection functions
    void selectAllDigits();
    void selectDigit(int digit);
    
    // clean function
    void clean();
    
    // brief display functions
    void shortDisplayNumber(int n);
    void shortDisplayNumber(int n, boolean dot);

    void shortDisplayLetter(char l);
    void shortDisplayLetter(char l, boolean dot);

    void shortDisplayCode(uint8_t code);
    void shortDisplayCode(uint8_t code, boolean dot);
    
    // cycle functions
    void cycleNumber(long digitDisplayTime, int number, int dot);
    void cycleWord(long digitDisplayTime, char* word);
    void cycle(long digitDisplayTime, char* word);
    void customDisplayCycle(long digitDisplayTime, uint8_t codes[4]);
    
    // zeroAnimation functions
    void zeroAnimation(long cycleDelay, long cycles);
    void zeroAnimation();
    
    // Only displaying numbers
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
```
