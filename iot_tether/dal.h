#
# Copyright 2016 British Broadcasting Corporation and Contributors(1)
#
# (1) Contributors are listed in the AUTHORS file (please extend AUTHORS,
#     not this header)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#include "spark_font.h"
#include <avr/pgmspace.h>

#define PRESSED HIGH

#define MICROKIT
// #define MICROBUG
// #define Megabug

#ifdef MEGABUG

int row0 = 11; // Arduino Pin for row 0
int row1 = 3; // Arduino Pin for row 1
int row2 = 2; // Arduino Pin for row 2
int row3 = 4; // Arduino Pin for row 3
int row4 = 12; // Arduino Pin for row 4

int col0 = 6; // Arduino Pin for row 0
int col1 = 8; // Arduino Pin for row 1
int col2 = 9; // Arduino Pin for row 2
int col3 = 10; // Arduino Pin for row 3
int col4 = 5; // Arduino Pin for row 4

int lefteye = 7; // Arduino Pin for left eye
int righteye = 17; // Arduino Pin for left eye

int ButtonA = 15; // Arduino Pin for left eye
int ButtonB = 16; // Arduino Pin for left eye

#endif

#ifdef MICROBUG

int row0 = 12; // Arduino Pin for row 0
int row1 = 4; // Arduino Pin for row 1
int row2 = 2; // Arduino Pin for row 2
int row3 = 3; // Arduino Pin for row 3
int row4 = 11; // Arduino Pin for row 4

int col0 = 6; // Arduino Pin for row 0
int col1 = 8; // Arduino Pin for row 1
int col2 = 9; // Arduino Pin for row 2
int col3 = 10; // Arduino Pin for row 3
int col4 = 5; // Arduino Pin for row 4

int lefteye = 7; // Arduino Pin for left eye
int righteye = 17; // Arduino Pin for left eye

int ButtonA = 16; // Arduino Pin for left eye
int ButtonB = 15; // Arduino Pin for left eye

#endif

#ifdef MICROKIT

int row0 = 1; // Arduino Pin for row 4 // PIN 21 -- D1
int row1 = 0; // Arduino Pin for row 3  // PIN 20 -- D0
int row2 = 2; // Arduino Pin for row 2  // PIN 19 -- D2
int row3 = 3; // Arduino Pin for row 1  // PIN 18 -- D3
int row4 = 11; // Arduino Pin for row 0 // PIN 12 -- D11

int col0 = 4; // Arduino Pin for row 0  // PIN 25 -- D4
int col1 = 12; // Arduino Pin for row 1  // PIN 26 -- D12
int col2 = 6; // Arduino Pin for row 2  // PIN 27 -- D6
int col3 = 9; // Arduino Pin for row 3 // PIN 29 -- D9
int col4 = 13; // Arduino Pin for row 4  // PIN 32 -- D13

// int lefteye = 7; // Arduino Pin for left eye // PIN 1     -- D7
// int righteye = 14; // Arduino Pin for left eye // PIN 11  -- D14
int lefteye = 14; // Arduino Pin for left eye // PIN 1     -- D7
int righteye = 7; // Arduino Pin for left eye // PIN 11  -- D14

int ButtonA = 17; // Arduino Pin for left eye // PIN 8    -- D17
int ButtonB = 16; // Arduino Pin for left eye // PIN 10   -- D16
#endif


int counter =0;
#define DELAY 5

int left_eye_state = HIGH; // Initial state is set to high in setup
int right_eye_state = HIGH; // Initial state is set to high in setup

int timer4_counter;

int display_strobe_counter;
int display[5][5] = {
                      { LOW, LOW, LOW, LOW, LOW},
                      { LOW, LOW, LOW, LOW, LOW},
                      { LOW, LOW, LOW, LOW, LOW},
                      { LOW, LOW, LOW, LOW, LOW},
                      { LOW, LOW, LOW, LOW, LOW}
                    };

/* API ---------------------------------------------------------------------------------------- */

#define DISPLAY_WIDTH 5
#define DISPLAY_HEIGHT 5

#define ___ 0

typedef struct Image {
    int width;
    int height;
    int *data ;
} Image;

void set_eye(char id, int state);
void eye_on(char id);
void eye_off(char id);
void print_message(const char * message, int pausetime);
void showLetter(char c);
int getButton(char id);
void clear_display();
void plot(int x, int y);
void unplot(int x, int y);
int point(int x, int y);
void set_display(int sprite[5][5]);
void showViewport(Image& someImage, int x, int y);
void ScrollImage(Image someImage, boolean loop, int trailing_spaces);
int image_point(Image& someImage, int x, int y);
void set_image_point(Image& someImage, int x, int y, int value);
void showLetter(char c); // Could be just internal, but useful.
void print_message(const char * message, int pausetime);
void toggle_eye(char id);

struct StringSprite;
void scroll_sprite(StringSprite theSprite, int pausetime);

// Functions internal to the API
inline int image_point_index(Image& someImage, int x, int y);
void display_column(int i);
void setup_display();
void microbug_setup();
void bootloader_start(void);
void check_bootkey();


// Crutch during development
void scroll_string(const char * str); // FIXME: Crutch during development
void scroll_string(const char * str, int delay); // FIXME: Crutch during development


/* END API ------------------------------------------------------------------------------------ */


// CODE TO SUPPORT SWITCH TO DFU BOOTLOADER -------------------------------------------------------

#include "avr/wdt.h"
#include "atmel_bootloader.h"

#define Usb_detach()                              (UDCON   |=  (1<<DETACH))

void bootloader_start(void) {
    /* This needs to reset the devive to a state the bootloader expects.
     * This means for example:
     *   Detaching from USB
     *   Waiting for the host to register the detach (we wait for 200 milli)
     *   Switch off all timers
     *   Reset registers to initial states
     *   Then and only then jump to the DFU bootloader
     */
    Usb_detach();
    cli();
    delay(200);
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    TIMSK0 = USBINT = OTGCON = OTGTCON = OTGIEN = OTGINT = PLLCSR = TCNT1L = TCNT1H = TIMSK3 = TCCR1B = TCNT4L = TCNT4H = TCCR4B = 0x00;
    USBCON = 0x20;
    UHWCON = 0x80;
    USBSTA = 0x08;

    run_bootloader();
}

void check_bootkey() {
    if (digitalRead(ButtonA) == PRESSED) {
        bootloader_start();
    }
}
// END CODE TO SUPPORT SWITCH TO DFU BOOTLOADER -------------------------------------------------------


void setup_display() {
#ifdef MICROBUG

CLKPR = 0x80;
CLKPR = 0x01;
#endif

#ifdef MICROKIT

CLKPR = 0x80;
CLKPR = 0x01;
#endif

    // initialize timer4 
    noInterrupts();           // disable all interrupts
    TCCR4A = 0;
    TCCR4B = 0;
    // Set timer4_counter to the correct value for our interrupt interval
    // timer4_counter = 64911;     // preload timer 65536-16MHz/256/100Hz
    timer4_counter = 65224;     // preload timer 65536-16MHz/256/200Hz
    // timer4_counter = 65497;     // preload timer 65536-2MHz/256/200Hz // For some reason current microbug is operating at 2MHz

    TCNT4 = timer4_counter;   // preload timer
    TCCR4B |= (1 << CS12);    // 256 prescaler 
    TIMSK4 |= (1 << TOIE4);   // enable timer overfLOW interrupt
    interrupts();             // enable all interrupts
}


ISR(TIMER4_OVF_vect)        // interrupt service routine 
{
    TCNT4 = timer4_counter;   // preload timer
    display_column(display_strobe_counter % 5);

    display_strobe_counter += 1;
    if (display_strobe_counter > 200) {
        display_strobe_counter = 0; // reset
    }
}

void microbug_setup() { // This is a really MicroBug setup
    setup_display();

    display_strobe_counter = 0;

    pinMode(row0, OUTPUT);
    pinMode(row1, OUTPUT);
    pinMode(row2, OUTPUT);
    pinMode(row3, OUTPUT);
    pinMode(row4, OUTPUT);

    pinMode(col0, OUTPUT);
    pinMode(col1, OUTPUT);
    pinMode(col2, OUTPUT);
    pinMode(col3, OUTPUT);
    pinMode(col4, OUTPUT);

    pinMode(lefteye, OUTPUT);
    pinMode(righteye, OUTPUT);

    pinMode(ButtonA, INPUT);
    pinMode(ButtonB, INPUT);

    digitalWrite(row0, LOW);
    digitalWrite(row1, LOW);
    digitalWrite(row2, LOW);
    digitalWrite(row3, LOW);
    digitalWrite(row4, LOW);

    digitalWrite(col0, HIGH);
    digitalWrite(col1, HIGH);
    digitalWrite(col2, HIGH);
    digitalWrite(col3, HIGH);
    digitalWrite(col4, HIGH);

    digitalWrite(lefteye, HIGH);
    digitalWrite(righteye, HIGH);
check_bootkey();

}

void display_column(int i) {
    digitalWrite(col0, HIGH);
    digitalWrite(col1, HIGH);
    digitalWrite(col2, HIGH);
    digitalWrite(col3, HIGH);
    digitalWrite(col4, HIGH);

    digitalWrite(row0, display[i][0] );
    digitalWrite(row1, display[i][1] );
    digitalWrite(row2, display[i][2] );
    digitalWrite(row3, display[i][3] );
    digitalWrite(row4, display[i][4] );

    if (i == 0) digitalWrite(col0, LOW );
    if (i == 1) digitalWrite(col1, LOW );
    if (i == 2) digitalWrite(col2, LOW );
    if (i == 3) digitalWrite(col3, LOW );
    if (i == 4) digitalWrite(col4, LOW );
}


void set_display(int sprite[5][5]) {
    for(int i=0; i<5; i++) {
        for(int j=0; j<5; j++) {
            display[i][j] = sprite[i][j];
        }
    }
}

void plot(int x, int y) {
    if (x <0) return;
    if (x >DISPLAY_WIDTH-1) return;

    if (y <0) return;
    if (y >DISPLAY_HEIGHT -1) return;

     display[x][y] = HIGH;
}

void unplot(int x, int y) {
    if (x <0) return;
    if (x >DISPLAY_WIDTH-1) return;

    if (y <0) return;
    if (y >DISPLAY_HEIGHT -1) return;

     display[x][y] = LOW;
}

int point(int x, int y) {
    // Bounds checking
    if (x <0) return -1;
    if (x >DISPLAY_WIDTH-1) return -1;

    if (y <0) return -2;
    if (y >DISPLAY_HEIGHT -1) return -2;

     return display[x][y];
}

inline int image_point_index(Image& someImage, int x, int y) {
   return x*someImage.width +y;
}

int image_point(Image& someImage, int x, int y) {
    if (x<0) return -1;
    if (y<0) return -2;
    if (x>someImage.width-1) return -1;
    if (x>someImage.height-1) return -2;

    return someImage.data[image_point_index(someImage, x, y)];
}

void set_image_point(Image& someImage, int x, int y, int value) {
    if (x<0);
    if (y<0);
    if (x>someImage.width-1);
    if (x>someImage.height-1);

    someImage.data[image_point_index(someImage, x, y)] = value;
}


void clear_display() {
    for(int i=0; i< DISPLAY_WIDTH; i++) {
        for(int j=0; j< DISPLAY_HEIGHT; j++) {
            unplot(i,j);
        }
    }
}

int getButton(char id) {
    if (id == 'A') {
        return digitalRead(ButtonA);
    }
    if (id == 'B') {
        return digitalRead(ButtonB);
    }
    return -1; // Signify error
}

void showLetter(char c) {
    int letter_index = c-32;
    if (c>126) return;
    if (c<32) return;
    if (pgm_read_byte(&(font[letter_index][0])) != c) return;
    clear_display();
    for(int row=0; row<5; row++) {
        unsigned char this_row = pgm_read_byte(&(font[letter_index][row+1]));
        unsigned char L0 = 0b1000 & this_row ? HIGH : LOW;
        unsigned char L1 = 0b0100 & this_row ? HIGH : LOW;
        unsigned char L2 = 0b0010 & this_row ? HIGH : LOW;
        unsigned char L3 = 0b0001 & this_row ? HIGH : LOW;
        display[0][row] = L0;
        display[1][row] = L1;
        display[2][row] = L2;
        display[3][row] = L3;
        display[4][row] = LOW;
    }
}

void print_message(const char * message, int pausetime=100) {
    while(*message) {
        showLetter(*message);
        message++;
        delay(pausetime);
    }
}

void set_eye(char id, int state) {
    if ((id == 'A') || (id == 'L')) {
        digitalWrite(lefteye, state );
        left_eye_state = state;
    }
    if ((id == 'B') || (id == 'R')) {
        digitalWrite(righteye, state );
        right_eye_state = state;
    }
}

void toggle_eye(char id) {
    if ((id == 'A') || (id == 'L')) {
        if (left_eye_state == HIGH) {
            set_eye(id, LOW);
        } else {
            set_eye(id, HIGH);
        }
    }
    if ((id == 'B') || (id == 'R')) {
        if (right_eye_state == HIGH) {
            set_eye(id, LOW);
        } else {
            set_eye(id, HIGH);
        }
    }
}

void eye_on(char id) {
    set_eye(id, HIGH);
}

void eye_off(char id) {
    set_eye(id, LOW);
}

void showViewport(Image& someImage, int x, int y) {
    if (someImage.width<4) return; // Not implemented yet
    if (someImage.height<4) return; // Not implemented yet
    for(int i=0; (i+x<someImage.width) && (i<5); i++) {
        for(int j=y; (j+y<someImage.height) && (j<5); j++) {
            int value = someImage.data[(j+y)*someImage.width+ x+i ];
            display[i][j]=value;
        }
    }
}

void ScrollImage(Image someImage, boolean loop=false, int trailing_spaces=false) {
    // Example, width is 16
    // Display width is 5
    // This counts from 0 <= i < 12 -- ie from 0 to 11
    // The last viewport therefore tries to display indices 11, 12, 13, 14, 15
    for(int i=0; i<someImage.width-DISPLAY_WIDTH+1; i++) {
        clear_display();
        showViewport(someImage, i,0);
        delay(80);
    }
}

typedef struct StringSprite {
    int mPixelPos;
    int mPixelData[50]; // Sufficient to hold two characters.
    char *mString;
    int mStrlen;

    StringSprite() {}
    StringSprite(const char * str) {
        setString(str);
    }
    ~StringSprite() {}

    void setString(const char * str) {
        mString = (char *) str;
        mPixelPos = 0;
        for(int i=0; i++; i<50) {
            mPixelData[i] = 0;
        }
        mStrlen = strlen(mString);
    }

    void update_display() {
        Image myImage;
        int mPP = mPixelPos%5;
        myImage.width=10;
        myImage.height=5;
        myImage.data = mPixelData;
        showViewport(myImage, mPP,0);

    }
    void render_string(){
        // Renders into the pixel data buffer
        int first_char;
        int second_char;
//         unsigned char *first_char_data;
//         unsigned char *second_char_data;

        unsigned char first_char_data1[6];
        unsigned char second_char_data1[16];
        int char_index1;

        int char_index0 = (mPixelPos / 5);

        char_index0 = char_index0 % mStrlen;

        first_char = mString[char_index0];
        for(int i=0; i<6; i++){
            first_char_data1[i] = pgm_read_byte(&(font[first_char-32][i]));
        }

        char_index1 = (char_index0 +1) ;
        if (char_index1 < mStrlen) {
            char_index1 = char_index1 % mStrlen;
            second_char = mString[char_index1];
//            second_char_data = (unsigned char*) ( font[second_char-32] );

            for(int i=0; i<6; i++){
                second_char_data1[i] = pgm_read_byte(&(font[second_char-32][i]));
            }
        } else {
//            second_char_data =  (unsigned char*) ( font[0] );
            for(int i=0; i<6; i++){
                second_char_data1[i] = pgm_read_byte(&(font[0][i]));
            }
        }

        for(int row=0; row<5; row++) {
            int row_first = first_char_data1[row + 1];
            int row_second = second_char_data1[row + 1];

            int F0 = 0b1000 & row_first ? HIGH : LOW;
            int F1 = 0b0100 & row_first ? HIGH : LOW;
            int F2 = 0b0010 & row_first ? HIGH : LOW;
            int F3 = 0b0001 & row_first ? HIGH : LOW;

            int S0 = 0b1000 & row_second ? HIGH : LOW;
            int S1 = 0b0100 & row_second ? HIGH : LOW;
            int S2 = 0b0010 & row_second ? HIGH : LOW;
            int S3 = 0b0001 & row_second ? HIGH : LOW;

            mPixelData[0+row*10] = F0;
            mPixelData[1+row*10] = F1;
            mPixelData[2+row*10] = F2;
            mPixelData[3+row*10] = F3;
            mPixelData[4+row*10] = 0;

            mPixelData[5+row*10] = S0;
            mPixelData[6+row*10] = S1;
            mPixelData[7+row*10] = S2;
            mPixelData[8+row*10] = S3;
            mPixelData[9+row*10] = 0;
        }
        update_display();
    }
    void pan_right() {
        // Move the viewport 1 pixel to the right. (Looks like scrolling left)
        mPixelPos += 1;
        if (mPixelPos>=pixel_width()) {
            mPixelPos =0;
        }
    }
    int pixel_width() {
        return mStrlen * 5;
    }
} StringSprite;

void scroll_sprite(StringSprite theSprite, int pausetime=100) {
    for(int i=0; i<theSprite.pixel_width(); i++) {
        theSprite.render_string();
        theSprite.pan_right();
        delay(pausetime);
    }
}

void scroll_string(const char * str) {
    scroll_sprite(StringSprite(str), 50);
}
void scroll_string(const char * str, int delay) {
    scroll_sprite(StringSprite(str), delay);
}

/* END - API IMPLEMENTATION ------------------------------------------------------------------*/
