#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 8000000UL

//====================================================
// PIC18F4520 CONFIGURATION
//====================================================

#pragma config OSC = INTIO67
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRT = ON
#pragma config BOREN = OFF
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LPT1OSC = OFF
#pragma config PBADEN = OFF
#pragma config CCP2MX = PORTC
#pragma config STVREN = ON
#pragma config LVP = OFF
#pragma config XINST = OFF


//====================================================
// PIN DEFINITIONS
//====================================================

// LCD
#define LCD_RS      PORTCbits.RC1
#define LCD_EN      PORTCbits.RC2

// Relay driver
#define RELAY       PORTCbits.RC0

// Motion sensor
#define MOTION      PORTBbits.RB4


//====================================================
// LIGHT THRESHOLD
//====================================================
//
// Your LDR circuit is:
//
// +5V
//  |
// 10K
//  |
//  +------ RA0
//  |
// LDR
//  |
// GND
//
// Therefore:
//
// DARK  -> LDR resistance HIGH -> RA0 voltage HIGH
// BRIGHT -> LDR resistance LOW -> RA0 voltage LOW
//
// 512 ~= 2.5 V with 5 V ADC reference.
//
// Adjust this value in Proteus if necessary.
//====================================================

#define DARK_THRESHOLD 512


//====================================================
// LCD FUNCTIONS
//====================================================

void LCD_Pulse(void)
{
    LCD_EN = 1;

    __delay_us(5);

    LCD_EN = 0;

    __delay_us(100);
}


void LCD_Command(unsigned char command)
{
    LCD_RS = 0;

    PORTD = command;

    LCD_Pulse();

    if(command == 0x01 || command == 0x02)
    {
        __delay_ms(2);
    }
}


void LCD_Data(unsigned char data)
{
    LCD_RS = 1;

    PORTD = data;

    LCD_Pulse();
}


void LCD_Init(void)
{
    __delay_ms(20);

    LCD_Command(0x38);      // 8-bit, 2-line
    LCD_Command(0x0C);      // Display ON, cursor OFF
    LCD_Command(0x06);      // Cursor increment
    LCD_Command(0x01);      // Clear display

    __delay_ms(2);
}


void LCD_String(const char *str)
{
    while(*str)
    {
        LCD_Data(*str);
        str++;
    }
}


void LCD_SetCursor(unsigned char row, unsigned char column)
{
    unsigned char address;

    if(row == 1)
    {
        address = 0x80 + column;
    }
    else
    {
        address = 0xC0 + column;
    }

    LCD_Command(address);
}


//====================================================
// ADC INITIALIZATION
//====================================================

void ADC_Init(void)
{
    // AN0 analog
    // Remaining analog channels disabled

    ADCON1 = 0x0E;

    // Right justified
    // Acquisition time = 8 TAD
    // ADC clock = Fosc/32

    ADCON2 = 0xAA;

    // ADC ON
    // Select AN0

    ADCON0 = 0x01;

    // RA0 input
    TRISAbits.TRISA0 = 1;
}


//====================================================
// ADC READ
//====================================================

unsigned int ADC_Read(void)
{
    unsigned int result;

    // Select AN0
    ADCON0bits.CHS = 0;

    __delay_us(20);

    // Start conversion
    ADCON0bits.GO = 1;

    // Wait for conversion
    while(ADCON0bits.GO);

    // Read ADC result
    result = ((unsigned int)ADRESH << 8) | ADRESL;

    return result;
}


//====================================================
// MAIN PROGRAM
//====================================================

void main(void)
{
    unsigned int adc_value;
    unsigned int light_voltage;

    unsigned char dark;
    unsigned char motion;

    char text[17];


    //================================================
    // INTERNAL OSCILLATOR = 8 MHz
    //================================================

    OSCCON = 0x72;


    //================================================
    // PORT CONFIGURATION
    //================================================

    // RA0 = LDR analog input
    TRISAbits.TRISA0 = 1;

    // RB4 = Motion sensor input
    TRISBbits.TRISB4 = 1;

    // RC0 = Relay output
    TRISCbits.TRISC0 = 0;

    // RC1 = LCD RS
    TRISCbits.TRISC1 = 0;

    // RC2 = LCD EN
    TRISCbits.TRISC2 = 0;

    // PORTD = LCD data
    TRISD = 0x00;


    //================================================
    // INITIAL OUTPUT STATES
    //================================================

    RELAY = 0;

    PORTD = 0x00;


    //================================================
    // ADC INITIALIZATION
    //================================================

    ADC_Init();


    //================================================
    // LCD INITIALIZATION
    //================================================

    LCD_Init();

    LCD_SetCursor(1,0);
    LCD_String("AUTO LIGHT");

    LCD_SetCursor(2,0);
    LCD_String("CONTROL SYSTEM");

    __delay_ms(2000);

    LCD_Command(0x01);


    //================================================
    // MAIN LOOP
    //================================================

    while(1)
    {
        //============================================
        // READ LDR
        //============================================

        adc_value = ADC_Read();


        //============================================
        // CONVERT ADC TO MILLIVOLTS
        //============================================

        light_voltage =
            ((unsigned long)adc_value * 5000UL) / 1023UL;


        //============================================
        // DETERMINE LIGHT CONDITION
        //============================================

        if(adc_value > DARK_THRESHOLD)
        {
            dark = 1;
        }
        else
        {
            dark = 0;
        }


        //============================================
        // READ MOTION SENSOR
        //============================================

        motion = MOTION;


        //============================================
        // LIGHT + MOTION CONTROL
        //============================================

        if(dark && motion)
        {
            // Dark + Motion
            // Turn lamp ON

            RELAY = 1;
        }
        else
        {
            // Bright OR no motion
            // Turn lamp OFF

            RELAY = 0;
        }


        //============================================
        // LCD LINE 1
        //============================================

        LCD_SetCursor(1,0);

        if(dark)
        {
            LCD_String("DARK ");
        }
        else
        {
            LCD_String("LIGHT");
        }

        LCD_String(" ");


        // Display sensor voltage

        sprintf(text, "%4umV", light_voltage);

        LCD_String(text);


        //============================================
        // LCD LINE 2
        //============================================

        LCD_SetCursor(2,0);

        if(motion)
        {
            LCD_String("MOTION: YES ");
        }
        else
        {
            LCD_String("MOTION: NO  ");
        }


        //============================================
        // DISPLAY LAMP STATUS
        //============================================

        if(RELAY)
        {
            LCD_String("ON ");
        }
        else
        {
            LCD_String("OFF");
        }


        //============================================
        // UPDATE EVERY 200 ms
        //============================================

        __delay_ms(200);
    }
}