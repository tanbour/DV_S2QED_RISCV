//===============================
// Degital Clock using IRQ Timer
//-------------------------------
// for VirtexE Evaluation Board
// Apr.30 2003 Ver.1
//===============================

//======================================================
// Address Map
//======================================================
// address           sz  wt wd device
// 00000000-00001FFF  8K 0  32 ROM (code)
// 00002000-00003DFF  6K 0  32 RAM (data)
// 00003E00-00003FFF 512 0  32 RAM (stack)
//
// 00000000-0000FFFF 64K 0  32 RAM (shadow every 16KB)
// 00010000-0001FFFF 64K 3  32 RAM (shadow every 16KB)
// 00020000-0002FFFF 64K 0  16 RAM (shadow every 16KB)
// 00030000-0003FFFF 64K 3  16 RAM (shadow every 16KB)
// 00040000-ABCCFFFF (shadow RAM)
// ABCD0000-ABCD00FF 256 3  32 PIO (shadow every 4B)
// ABCD0100-ABCD01FF 256 3  32 UART(shadow every 4B)
// ABCD0200-ABCD02FF 256 3  32 SYS (shadow every 8B)
// ABCD0300-FFFBFFFF (shadow RAM)
// FFFC0000-FFFCFFFF 64K 0  32 RAM (shadow every 16KB)
// FFFD0000-FFFDFFFF 64K 3  32 RAM (shadow every 16KB)
// FFFE0000-FFFEFFFF 64K 0  16 RAM (shadow every 16KB)
// FFFF0000-FFFFFFFF 64K 3  16 RAM (shadow every 16KB)
// 
//======================================================
// PORT OUTPUT
//======================================================
// ABCD0000 : reserved
//   31    30    29    28    27    26    25    24
//    7     6     5     4     3     2     1     0
//  -----------------------------------------------
// |     |     |     |     |     |     |     |     |
//  -----------------------------------------------;
// 
// ABCD0001 : KEYYO (Key SCAN Y-axis out)
//   23    22    21    20    19    18    17    16
//    7     6     5     4     3     2     1     0
//  -----------------------------------------------
// |     |     |     | KY4 | KY3 | KY2 | KY1 | KY0 |
//  -----------------------------------------------
// 
// ABCD0002 : LCDCON (LCD Control Signal)
//   15    14    13    12    11    10     9     8
//    7     6     5     4     3     2     1     0
//  -----------------------------------------------
// |     |     |     |     |     |  E  | R/W |  RS |
//  -----------------------------------------------
// 
// ABCD0003 : LCDOUT (Write Data to LCD)
//    7     6     5     4     3     2     1     0
//    7     6     5     4     3     2     1     0
//  -----------------------------------------------
// | DW7 | DW6 | DW5 | DW4 | DW3 | DW2 | DW1 | DW0 |
//  -----------------------------------------------
// 
//======================================================
// PORT INPUT
//======================================================
// ABCD0000 : reserved
//   31    30    29    28    27    26    25    24
//    7     6     5     4     3     2     1     0
//  -----------------------------------------------
// |     |     |     |     |     |     |     |     |
//  -----------------------------------------------;
// 
// ABCD0001 : KEYXI (Key SCAN X-axis in)
//   23    22    21    20    19    18    17    16
//    7     6     5     4     3     2     1     0
//  -----------------------------------------------
// |     |     |     | KX4 | KX3 | KX2 | KX1 | KX0 |
//  -----------------------------------------------
// 
// ABCD0002 : reserved
//  15    14    13    12    11    10     9     8
//    7     6     5     4     3     2     1     0
//  -----------------------------------------------
// |     |     |     |     |     |     |     |    |
//  -----------------------------------------------
// 
// ABCD0003 : LCDIN (Read Data from LCD)
//    7     6     5     4     3     2     1     0
//    7     6     5     4     3     2     1     0
//  -----------------------------------------------
// | DR7 | DR6 | DR5 | DR4 | DR3 | DR2 | DR1 | DR0 |
//  -----------------------------------------------
// 
//======================================================
// UART: SASC (www.opencores.com)
// Simple Asynchronous Serial Communication Device
//======================================================
// ABCD0100 : UARTBG0 Baud Rate Generator Div0 (R/W)
//   31    30    29    28    27    26    25    24
//    7     6     5     4     3     2     1     0
//  -----------------------------------------------
// | B07 | B06 | B05 | B04 | B03 | B02 | B01 | B00 |
//  -----------------------------------------------
// 
// ABCD0101 : UARTBG1 Baud Rate Generator Div1 (R/W)
//   23    22    21    20    19    18    17    16
//    7     6     5     4     3     2     1     0
//  -----------------------------------------------
// | B17 | B16 | B15 | B14 | B13 | B12 | B11 | B10 |
//  -----------------------------------------------
// 
// ABCD0102 : UARTCON (TXF=full_o, RXE=empty_o) (R only)
//   15    14    13    12    11    10     9      8
//    7     6     5     4     3     2     1      0
//  -----------------------------------------------
// |     |     |     |     |     |     | TXF | RXE |
//  -----------------------------------------------
// 
// ABCD0103 : UARTTXD(W only)/UARTRXD(R only)
//    7     6     5     4     3     2     1     0
//    7     6     5     4     3     2     1     0
//  -----------------------------------------------
// | TR7 | TR6 | TR5 | TR4 | TR3 | TR2 | TR1 | TR0 |
//  -----------------------------------------------
// 
//======================================================
// System Controller
// Interrupt and Exception Controller
//======================================================
// ABCD0200 : INTCTL Interrupt Control (32bit R/W only)
//   31    30    29    28    27    26    25    24
//  -----------------------------------------------
// |E_NMI|E_IRQ|E_CER|E_DER|E_MRS|     |TMRON|BRKON|
//  -----------------------------------------------
//   23    22    21    20    19    18    17    16
//  -----------------------------------------------
// |ILVL3|ILVL2|ILVL1|ILVL0|IVEC7|IVEC6|IVEC5|IVEC4|
//  ----------------------------------------------- 
//   15    14    13    12    11    10     9     8
//  -----------------------------------------------
// |IVEC3|IVEC2|IVEC1|IVEC0|TMR11|TMR10|TMR09|TMR08|
//  -----------------------------------------------  
//    7     6     5     4     3     2     1     0
//  -----------------------------------------------
// |TMR07|TMR06|TMR05|TMR04|TMR03|TMR02|TMR01|TMR00|
//  ----------------------------------------------- 
//     bit31: E_NMI Emulate NMI Interrupt     (W only)
//     bit30: E_IRQ Emulate IRQ Interrupt     (W only)
//     bit29: E_CER Emulate CPU Address Error (W only)
//     bit28: E_DER Emulate DMA Address Error (W only)
//     bit27: E_MRS Emulate Manual Reset      (W only)                               
//     bit26: reserved
//     bit25: TMRON INTTMR OFF/ON             (0:OFF, 1:ON)
//     bit24: BRKON Address Break(NMI) OFF/ON (0:OFF, 1:ON)
//     bit23: ILVL3 IRQ ILevel 3
//     bit22: ILVL2 IRQ ILevel 2
//     bit21: ILVL1 IRQ ILevel 1
//     bit20: ILVL0 IRQ ILevel 0
//     bit19: IVEC7 IRQ Vector 7
//     bit18: IVEC6 IRQ Vector 6
//     bit17: IVEC5 IRQ Vector 5
//     bit16: IVEC4 IRQ Vector 4
//     bit15: IVEC3 IRQ Vector 3
//     bit14: IVEC2 IRQ Vector 2
//     bit13: IVEC1 IRQ Vector 1
//     bit12: IVEC0 IRQ Vector 0
//     bit11: TMR11 (12 bit interval timer to generate IRQ)
//     ...
//     bit 0: TMR00 (12 bit interval timer to generate IRQ)
// 
// ABCD0204 : BRKADR Break Address (16bit R/W only)
//   15    14    13    12    11    10     9     8
//  -----------------------------------------------
// |ADR15|ADR14|ADR13|ADR12|ADR11|ADR10|ADR09|ADR08|
//  -----------------------------------------------  
//    7     6     5     4     3     2     1     0
//  -----------------------------------------------
// |ADR07|ADR06|ADR05|ADR04|ADR03|ADR02|ADR01|ADR00|
//  ----------------------------------------------- 
//     bit15: ADR15 Break Address15
//     ...
//     bit00: ADR00 Break Address00
// 

#include "common.h"

//=================
// Define Functions
//=================
void          disp_time(void);
void          int_timer(int onoff);
unsigned char command(unsigned char *data);
unsigned char hex2asc(unsigned char hex);
unsigned char asc2hex(unsigned char asc);

void          waitNms(unsigned char n);
unsigned char key_scan();
unsigned char key_wait_on(void);
void          key_wait_off(void);

void          lcd_disp_char(unsigned char data);
void          lcd_disp_long(unsigned long data);
void          lcd_cursor(unsigned char con);
void          lcd_erase(unsigned char pos);
void          lcd_message(unsigned char pos, unsigned char *pstr);
void          lcd_disp(unsigned char chr);
void          lcd_pos(unsigned char pos);
void          lcd_ready(void);
unsigned char lcd_rd(int);
void          lcd_wr(int, unsigned char);
void          lcd_init(void);

//----------------
// Global Variables
//----------------
unsigned char mes_welcome[] = "Interrupt! Clock";
unsigned char mes_time[]    = "Time----";
unsigned char mes_hour[]    = "Input Hour =?   ";
unsigned char mes_min[]     = "Input Min  =?   ";
unsigned char mes_sec[]     = "Input Sec  =?   ";
unsigned char hour;
unsigned char min;
unsigned char sec;
unsigned long clock;
unsigned long onesec;
int countup;

//============
// IRQ Handler
//============
void irq_handler(void)
{
    // Timer Interval = 50ns * 2^12 = 50ns * 4096
    // 1sec = 50ns * 2E+07 (0x01312d00)

    // clock      onesec
    // 00000000   00000000
    // 00000000   01312d00
    // ...
    // 01313000 > 01312d00
    // 01313000   01312d00 + 01312d00
    // 01313000   02625A00
    // 00000000   01312A00
    // ....

    if (clock >= onesec)
    {
        onesec = onesec + 20000000 - clock;
        clock = 0;
        countup = 1;
    }
    clock = clock + 4096;
}

//============
// NMI Handler
//============
void nmi_handler(void)
{
}

//=============
// TRAP Handler
//=============
void trap_handler(void)
{
}

//====================
// Illegal Instruction
//====================
void illegal(void)
{
}

//==============
// Address Error
//==============
void error(void)
{
}

//=============
// Main Routine
//=============
void main_sh(void)
{
    unsigned char key;

    //-----------
    // Initialize
    //-----------
    key_wait_off();
    lcd_init();
    lcd_message(0, mes_welcome);

    hour    = 0;
    min     = 0;
    sec     = 0;
    clock   = 0;
    onesec  = 20000000;
    countup = 0;

    disp_time();
    int_timer(1);

    //----------------
    // Main Event Loop
    //----------------
    while(1)
    {
        if (countup)
        {
            countup = 0;

            sec = sec + 1;
            if ((sec % 16) >= 10) sec = sec + 6;
            if (sec >= 0x60) sec = 0;

            if (sec == 0) min = min + 1;
            if ((min % 16) >= 10) min = min + 6;
            if (min >= 0x60) min = 0;
             
            if ((sec == 0) & (min == 0)) hour = hour + 1;
            if ((hour % 16) >= 10) hour = hour + 6;
            if (hour >= 0x24) hour = 0;

            disp_time();
        }

        //----------------
        // Adjust Function
        //----------------
        if (key_scan() == KEYDAT)
        {
            waitNms(4);
            int_timer(0);

            lcd_message(0, mes_hour);
            do
            {
                key = command(&hour);
            } while (hour >= 0x24);
            disp_time();

            lcd_message(0, mes_min);
            do
            {
                key = command(&min);
            } while (min >= 0x60);
            disp_time();

            lcd_message(0, mes_sec);
            do
            {
                key = command(&sec);
            } while (sec >= 0x60);
            disp_time();

            lcd_message(0, mes_welcome);

            key_wait_off();

            clock   = 0;
            onesec  = 20000000;
            countup = 0;
            int_timer(1);
        }
    }
}

//****************************************************
//*                                                  *
//*                Service Utilities                 *
//*                                                  *
//****************************************************

//==================
// Display Time
//------------------
//     Input  : none
//     Output : none
//==================
void disp_time(void)
{
    lcd_message(16, mes_time);
    lcd_disp_char(hour);
    lcd_disp(':');
    lcd_disp_char(min);
    lcd_disp(':');
    lcd_disp_char(sec);
}

//========================================
// Interrupt Timer ON/OFF
//----------------------------------------
//     Input  : onoff = timer on(1)/off(0)
//     Output : none
//========================================
void int_timer(int onoff)
{
    if (onoff)
        SYS.INTCTL = 0x02140fff;
    else
        SYS.INTCTL = 0x00040fff;
}

//=========================================
// Command Handler
//-----------------------------------------
//     Input  : data    = display char
//     Output : command = input command
//              data    = input integer
//=========================================
unsigned char command(unsigned char *data)
{
    unsigned char key;
    int i;

    lcd_pos(14);
    lcd_disp_char(*data);

    lcd_cursor(1);
    i = 0;

    do
    {
        if (i == 0) lcd_pos(14);

        key_wait_off();
        key = key_wait_on();
        if (key <= KEY9)
        {
            lcd_disp(hex2asc(key));
            *data = (*data & ~((0x0F) << ((1-i) * 4))) | (key << ((1-i) * 4));
            i = (i + 1) % 2;
        }
    } while (key != KEYDAT);

    lcd_cursor(0);
    return(key);
}

//=============================
// Convert Hex(nibble) to Ascii
//-----------------------------
//     Input  : hex (0x00-0x0F)
//     Output : hex2asc
//=============================
unsigned char hex2asc(unsigned char hex)
{
    hex = hex & 0x0f;
    if (hex <= 0x09)
        return(hex + '0');
    else
        return(hex - 0x0a + 'A');
}

//=============================
// Convert Ascii to Hex(nibble)
//-----------------------------
//     Input  : asc(0-9, A-F)
//     Output : asc2hex
//=============================
unsigned char asc2hex(unsigned char asc)
{
    if (asc <= '9')
        return((asc - '0') & 0x0f);
    else
        return((asc - 'A' + 0x0a) & 0x0f);
}

//****************************************************
//*                                                  *
//*                 Key Utilities                    *
//*                                                  *
//****************************************************

//===================================
// Wait until Key On
//-----------------------------------
//     Input  : none
//     Output : key = pushed key code
//===================================
unsigned char key_wait_on(void)
{
    unsigned char key;

    while((key = key_scan()) == KEYNONE) waitNms(4);

    return(key);
}

//===================
// Wait until Key Off
//-------------------
//     Input  : none
//     Output : none
//===================
void key_wait_off(void)
{
    while(key_scan() != KEYNONE) waitNms(4);
}

//===================================
// Key Scan
//-----------------------------------
//     Input  : none
//     Output : key = pushed key code
//===================================
unsigned char key_scan()
{
    unsigned char keyx4;
    unsigned char keyx3;
    unsigned char keyx2;
    unsigned char keyx1;
    unsigned char keyx0;

    PORTO.KEYYO.BYTE = 0xef; // KEYY4
    waitNms(1);
    keyx4 = PORTI.KEYXI.BYTE & 0x1f;

    PORTO.KEYYO.BYTE = 0xf7; // KEYY3
    waitNms(1);
    keyx3 = PORTI.KEYXI.BYTE & 0x1f;

    PORTO.KEYYO.BYTE = 0xfb; // KEYY2
    waitNms(1);
    keyx2 = PORTI.KEYXI.BYTE & 0x1f;

    PORTO.KEYYO.BYTE = 0xfd; // KEYY1
    waitNms(1);
    keyx1 = PORTI.KEYXI.BYTE & 0x1f;

    PORTO.KEYYO.BYTE = 0xfe; // KEYY0
    waitNms(1);
    keyx0 = PORTI.KEYXI.BYTE & 0x1f;

    switch(keyx4)
    {
        case 0x17 : return(KEYPUT);
        case 0x1b : return(KEYGET);
        case 0x1d : return(KEYRUN);
        case 0x1e : return(KEYUTL);
    }
    switch(keyx3)
    {
        case 0x0f : return(KEYADR);
        case 0x17 : return(KEYF);
        case 0x1b : return(KEYE);
        case 0x1d : return(KEYD);
        case 0x1e : return(KEYC);
    }
    switch(keyx2)
    {
        case 0x0f : return(KEYINC);
        case 0x17 : return(KEYB);
        case 0x1b : return(KEYA);
        case 0x1d : return(KEY9);
        case 0x1e : return(KEY8);
    }
    switch(keyx1)
    {
        case 0x0f : return(KEYDEC);
        case 0x17 : return(KEY7);
        case 0x1b : return(KEY6);
        case 0x1d : return(KEY5);
        case 0x1e : return(KEY4);
    }
    switch(keyx0)
    {
        case 0x0f : return(KEYDAT);
        case 0x17 : return(KEY3);
        case 0x1b : return(KEY2);
        case 0x1d : return(KEY1);
        case 0x1e : return(KEY0);
    }
    return(KEYNONE);
}

//==================
// Wait n ms
//------------------
//     Input  : n ms
//     Output : none
//==================
void waitNms(unsigned char n)
{
    int  i, j;
    unsigned char dummy;

    for (i = 0 ; i < n ; i++)
    {
        // 1ms = 20000 * 50ns (20MHz)
        for (j = 0 ; i <= 10000 ; i++) dummy = PORTI.RESERVED_0;
    }
}

//****************************************************
//*                                                  *
//*                 LCD Utilities                    *
//*                                                  *
//****************************************************

//======================================
// LCD Display Char Hex data (2 digit)
//--------------------------------------
//     Input  : data = display char data
//     Output : none
//======================================
void lcd_disp_char(unsigned char data)
{
    int i;
    unsigned char digit;

    for (i = 1 ; i >= 0 ; i--)
    {
        digit = (unsigned char) (data / (1 << i * 4)) % 0x10;
        lcd_disp(hex2asc(digit));
    }
}

//======================================
// LCD Display Long Hex data (8 digit)
//--------------------------------------
//     Input  : data = display long data
//     Output : none
//======================================
void lcd_disp_long(unsigned long data)
{
    int i;
    unsigned char digit;

    for (i = 7 ; i >= 0 ; i--)
    {
        digit = (unsigned char) (data / (1 << i * 4)) % 0x10;
        lcd_disp(hex2asc(digit));
    }
}

//================================
// Cursor On/Off
//--------------------------------
//     Input  : cur = off(0)/on(1)
//     Output : none
//================================
void lcd_cursor(unsigned char con)
{
    lcd_ready();
    if (con)
        lcd_wr(INST, 0x0d);
    else
        lcd_wr(INST, 0x0c);
}


//===============================
// Erase 1 line from the position
//-------------------------------
//     Input  : pos = position
//               1st line = 00-15
//               2nd line = 16-31
//     Ouput  : none
//===============================
void lcd_erase(unsigned char pos)
{
    int i;

    lcd_pos(pos);
    for (i = 0; i < 16; i++) lcd_disp(' ');
}

//========================================
// Print a Message from Current Position
//----------------------------------------
//     Input  : pstr = message top address
//     Outpur : none
//========================================
void lcd_message(unsigned char pos, unsigned char *pstr)
{
    unsigned char *pmessage;

    lcd_pos(pos);
    pmessage = pstr;
    while (*pmessage) lcd_disp(*pmessage++);
}

//==========================================
// Display One Character on Current Position
//------------------------------------------
//     Input  : chr = display charcter
//     Output : none
//==========================================
void lcd_disp(unsigned char chr)
{
    lcd_ready();
    lcd_wr(DATA, chr);
}

//================================
// Set LCD Display Position
//--------------------------------
//     Input  : pos = position
//                1st line = 00-15
//                2nd line = 16-31
//     Ouput  : none
//================================
void lcd_pos(unsigned char pos)
{
    unsigned char phypos;

    phypos = pos % 32;
    if (phypos >= 16)
        phypos = (phypos - 16) + 64;
    phypos = phypos | 0x80; 

    lcd_ready();
    lcd_wr(INST, phypos);
}

//=====================
// Wait until LCD ready
//---------------------
//     Input  : none
//     Output : none 
//=====================
void lcd_ready(void)
{
    while ((lcd_rd(INST) & 0x80) == 0x80); 
}

//=================================
// LCD Read
//---------------------------------
//     Input  : rs (0=Instr,1=Data)
//     Output : lcd_rd (Read Data) 
//=================================
unsigned char lcd_rd(int rs)
{
    int i;
    unsigned char dummy;
    unsigned char data;

    PORTO.LCDCON.BYTE = 0x02 | (rs & 0x01);

    // wait 60ns = 2cyc@20MHz
    dummy = PORTI.RESERVED_0; 
    dummy = PORTI.RESERVED_0;


    PORTO.LCDCON.BYTE = 0x06 | (rs & 0x01);

    // wait 500ns = 10cyc@20MHz
    for (i = 0 ; i <= 10 ; i++) dummy = PORTI.RESERVED_0;

    data = PORTI.LCDIN;

    PORTO.LCDCON.BYTE = 0x02 | (rs & 0x01);

    // wait 500ns = 10cyc@20MHz
    for (i = 0 ; i <= 10 ; i++) dummy = PORTI.RESERVED_0;

    return(data);
}

//=================================
// LCD Write
//---------------------------------
//     Input  : rs (0=Instr,1=Data)
//              wd (Write Data)
//     Output : none
//=================================
void lcd_wr(int rs, unsigned char wd)
{
    int i;
    unsigned char dummy;

    PORTO.LCDOUT = wd;

    PORTO.LCDCON.BYTE = 0x00 | (rs & 0x01);

    // wait 60ns = 2cyc@20MHz
    dummy = PORTI.RESERVED_0; 
    dummy = PORTI.RESERVED_0;

    PORTO.LCDCON.BYTE = 0x04 | (rs & 0x01);

    // wait 500ns = 10cyc@20MHz
    for (i = 0 ; i <= 10 ; i++) dummy = PORTI.RESERVED_0;

    PORTO.LCDCON.BYTE = 0x00 | (rs & 0x01);

    // wait 500ns = 10cyc@20MHz
    for (i = 0 ; i <= 10 ; i++) dummy = PORTI.RESERVED_0;
}

//==================
// LCD Initialize
//------------------
//     Input  : none
//     Output : none
//==================
void lcd_init(void)
{
    int i;
    unsigned char dummy;

    lcd_wr(INST, 0x30);

    // wait 4100us=82000cyc@20MHz
    for (i = 0 ; i < 82000 ; i++) dummy = PORTI.RESERVED_0;

    lcd_wr(INST, 0x30);

    // wait 100us=2000cyc@20MHz
    for (i = 0 ; i < 2000 ; i++) dummy = PORTI.RESERVED_0;

    lcd_wr(INST, 0x30);

    // set function (8bit, 2lines)
    lcd_ready();
    lcd_wr(INST, 0x38);

    // display (disp on, cursor off, blink off)
    lcd_ready();
    lcd_wr(INST, 0x0c);

    // clear
    lcd_ready();
    lcd_wr(INST, 0x01);

    // entry mode (increment, no-shift)
    lcd_ready();
    lcd_wr(INST, 0x06);
}

//===============
// End of Program
//===============
