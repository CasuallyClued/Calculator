/* low_level_funcs_tiva.c
 * 
 * Set of functions at the bottom level for the 3662 calculator mini-project.
 * These are the hardware drivers.
 * 
 * For documentation, see the documentation in the corresponding .h file.
 * 
 * Dr Chris Trayner, 2019 September
 */

#include "TExaS.h"
#include "low_level_funcs_tiva.h"
#include "PLL.h" // For PLL and SysTick
#include "uart.h"
#include <stdio.h>

// =========================== CONSTANTS ============================

/* ----- What you need to do with these #define constants -----
 * 
 * You will need to define many hardware-specific #define constants 
 * to make your program work with the Tiva. They should all be 
 * defined here.
 * 
 * It is your job to decide which constants you need to define.
 * Any which you do define must have the names given here. This is 
 * the same as asking you to use specified names for functions. 
 * Rather than giving you a list of required names, you are given 
 * the definitions but without the values entered. In fact, they 
 * all have a value entered (0x0) to make sure the module will 
 * compile.
 * 
 * The port allocations (i.e. what bit of what port does what job) 
 * are given in Appendix C of the Mini-project Handout. For your 
 * convenience they are copied into the comment at the start of each 
 * port below.
 * 
 * For Ports A, B, D and E and the clocks, all the constants have 
 * been listed for you. This is simpler than deciding which you 
 * will use in your program. For definitions you don't need, 
 * just leave them as they are (with value 0x0) - it's not worth 
 * deleting them. Unused #define definitions do no harm.
 * 
 * These port and clock constants have been given the standard 
 * names, so there is no explanation of what they mean.
 * There are also some special LCD-related definitions which are 
 * not Tiva standards. These are at the end of these constants 
 * and have comments explaining what they are.
 */

// --------------------------- Ports -------------------------

// Port A (bit 2 is EN, bit 3 is RS):																			// BASE ADDRESSES (+3FC for entire bus)
#define GPIO_PORTA_DATA_R (*((volatile unsigned long *)0x400043FC))  //0x40004000
#define GPIO_PORTA_DIR_R (*((volatile unsigned long *)0x40004400))   //0x40004400
#define GPIO_PORTA_AFSEL_R (*((volatile unsigned long *)0x40004420)) //0x40004420
#define GPIO_PORTA_PUR_R (*((volatile unsigned long *)0x40004510))   //0x40004510
#define GPIO_PORTA_PDR_R (*((volatile unsigned long *)0x40004514))   //0x40004514
#define GPIO_PORTA_DEN_R (*((volatile unsigned long *)0x4000451C))   //0x4000451C
#define GPIO_PORTA_LOCK_R (*((volatile unsigned long *)0x40004520))  //0x40004520
#define GPIO_PORTA_CR_R (*((volatile unsigned long *)0x40004524))    //0x40004524
#define GPIO_PORTA_AMSEL_R (*((volatile unsigned long *)0x40004528)) //0x40004528
#define GPIO_PORTA_PCTL_R (*((volatile unsigned long *)0x4000452C))  //0x4000452C

// Port B (PORTB[2:5] are LCD DB4 to DB7):
#define GPIO_PORTB_DATA_R (*((volatile unsigned long *)0x400053FC))  //0x40005000
#define GPIO_PORTB_DIR_R (*((volatile unsigned long *)0x40005400))   //0x40005400
#define GPIO_PORTB_AFSEL_R (*((volatile unsigned long *)0x40005420)) //0x40005420
#define GPIO_PORTB_PUR_R (*((volatile unsigned long *)0x40005510))   //0x40005510
#define GPIO_PORTB_PDR_R (*((volatile unsigned long *)0x40005514))   //0x40005514
#define GPIO_PORTB_DEN_R (*((volatile unsigned long *)0x4000551C))   //0x4000551C
#define GPIO_PORTB_LOCK_R (*((volatile unsigned long *)0x40005520))  //0x40005520
#define GPIO_PORTB_CR_R (*((volatile unsigned long *)0x40005524))    //0x40005524
#define GPIO_PORTB_AMSEL_R (*((volatile unsigned long *)0x40005528)) //0x40005528
#define GPIO_PORTB_PCTL_R (*((volatile unsigned long *)0x4000552C))  //0x4000552C

// Port D (PORTD[0:3] are the outputs to the columns):
#define GPIO_PORTD_DATA_R (*((volatile unsigned long *)0x400073FC))  //0x40007000
#define GPIO_PORTD_DIR_R (*((volatile unsigned long *)0x40007400))   //0x40007400
#define GPIO_PORTD_AFSEL_R (*((volatile unsigned long *)0x40007420)) //0x40007420
#define GPIO_PORTD_PUR_R (*((volatile unsigned long *)0x40007510))   //0x40007510
#define GPIO_PORTD_PDR_R (*((volatile unsigned long *)0x40007514))   //0x40007514
#define GPIO_PORTD_DEN_R (*((volatile unsigned long *)0x4000751C))   //0x4000751C
#define GPIO_PORTD_LOCK_R (*((volatile unsigned long *)0x40007520))  //0x40007520
#define GPIO_PORTD_CR_R (*((volatile unsigned long *)0x40007524))    //0x40007524
#define GPIO_PORTD_AMSEL_R (*((volatile unsigned long *)0x40007528)) //0x40007528
#define GPIO_PORTD_PCTL_R (*((volatile unsigned long *)0x4000752C))  //0x4000752C

// Port E (PORTE[0:3] are the inputs from the rows):
#define GPIO_PORTE_DATA_R (*((volatile unsigned long *)0x400243FC))  //0x40024000
#define GPIO_PORTE_DIR_R (*((volatile unsigned long *)0x40024400))   //0x40024400
#define GPIO_PORTE_AFSEL_R (*((volatile unsigned long *)0x40024420)) //0x40024420
#define GPIO_PORTE_PUR_R (*((volatile unsigned long *)0x40024510))   //0x40024510
#define GPIO_PORTE_PDR_R (*((volatile unsigned long *)0x40024514))   //0x40024514
#define GPIO_PORTE_DEN_R (*((volatile unsigned long *)0x4002451C))   //0x4002451C
#define GPIO_PORTE_LOCK_R (*((volatile unsigned long *)0x40024520))  //0x40024520
#define GPIO_PORTE_CR_R (*((volatile unsigned long *)0x40024524))    //0x40024524
#define GPIO_PORTE_AMSEL_R (*((volatile unsigned long *)0x40024528)) //0x40024528
#define GPIO_PORTE_PCTL_R (*((volatile unsigned long *)0x4002452C))  //0x4002452C

// --------------------------- Clocks --------------------------
#if 0
/* You are not asked to define the following because they are defined 
 * in PLL.h, which is #included by TExaS.h, which is #included above.
 */

//PLL related Defines
#define SYSCTL_RIS_R (*((volatile unsigned long *)0x0))
#define SYSCTL_RCC_R (*((volatile unsigned long *)0x0))
#define SYSCTL_RCC2_R (*((volatile unsigned long *)0x0))
#define SYSCTL_RCGC1_R (*((volatile unsigned long *)0x0))
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x0))

//SysTick related Defines
#define NVIC_ST_CTRL_R (*((volatile unsigned long *)0x0))
#define NVIC_ST_RELOAD_R (*((volatile unsigned long *)0x0))
#define NVIC_ST_CURRENT_R (*((volatile unsigned long *)0x0))
#endif

// ------------------- Special definitions ----------------------

/* LCD-related definitions
 * 
 * These are explained in Appendix C of the Mini-project Handout;
 * before you write this code you should re-read that part of 
 * that Appendix. The port allocations are copied into the 
 * comments at the start of each port above.
 * These have all been defined here with the simple value 0 (so the 
 * program will compile), but you should think carefully about 
 * them. For instance, if you need to access bit 6 of a port, 
 * should you define the value to be 6? Or should you write a 
 * definition like (*((volatile unsigned long *) ... )) to make 
 * the port access this bit directly? It is your decision.
 */

#define LCD_RS (*((volatile unsigned long *)0x40004020))   /* PA3                                  \ \
                                                            * The single port bit connected to the \ \
                                                            * RS (Register Select) pin of the LCD. \ \
                                                            */
#define LCD_EN (*((volatile unsigned long *)0x40004010))   /* PA2                                       \ \
                                                            * The single port bit connected to the      \ \
                                                            * EN (ENable data transfer) pin of the LCD. \ \
                                                            */
#define LCD_DATA (*((volatile unsigned long *)0x400050F0)) /* PORT B[2:5]                             \ \
                                                            * The set of four adjacent bits connected \ \
                                                            * to the four data transfer bits (DB4 to  \ \
                                                            * DB7) of the LCD. */

/* Incidentlly, a  comment on C-writing technique:
 * You will have noticed that the comments above use to old C 
 * comment form starting with slash-star and ending with 
 * star-slash, rather than the newer slash-slash. This is because 
 * a slash-slash comment ends at the line end. When you edit a 
 * program, it is an easy mistake to copy and paste something in the 
 * middle of a comment. It compiles, but the comment now refers to 
 * the wrong lines of code. But with the old form it probably won't 
 * compile, so you find the problem.
 * This is also why the comments above start on the same line as 
 * the #define, even though there is only room for the opening 
 * slash-star. (At least, there would only be room for that if you 
 * used the (*((volatile unsigned long *) ... )) form.)
 * This is one of the personal tricks I have invented over the years 
 * to stop me making mistaakes.
 */
// ================== INITIALISE UART ================ //
#define GPIO_PORTA_AFSEL_R (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_DEN_R (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_AMSEL_R (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R (*((volatile unsigned long *)0x4000452C))
#define UART0_DR_R (*((volatile unsigned long *)0x4000C000))
#define UART0_FR_R (*((volatile unsigned long *)0x4000C018))
#define UART0_IBRD_R (*((volatile unsigned long *)0x4000C024))
#define UART0_FBRD_R (*((volatile unsigned long *)0x4000C028))
#define UART0_LCRH_R (*((volatile unsigned long *)0x4000C02C))
#define UART0_CTL_R (*((volatile unsigned long *)0x4000C030))
#define UART_FR_TXFF 0x00000020     // UART Transmit FIFO Full
#define UART_FR_RXFE 0x00000010     // UART Receive FIFO Empty
#define UART_LCRH_WLEN_8 0x00000060 // 8 bit word length
#define UART_LCRH_FEN 0x00000010    // UART Enable FIFOs
#define UART_CTL_UARTEN 0x00000001  // UART Enable
#define SYSCTL_RCGC1_R (*((volatile unsigned long *)0x400FE104))
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC1_UART0 0x00000001 // UART0 Clock Gating Control
#define SYSCTL_RCGC2_GPIOA 0x00000001 // port A Clock Gating Control

// =========================== FUNCTIONS ============================

// ------------------------ Keyboard functions ------------------------

void InitKeyboardPorts(void)
{
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000018; // 1) D & E clock activation
    delay = SYSCTL_RCGC2_R;
    GPIO_PORTD_LOCK_R = 0x4C4F434B; // 2) unlock PortD
    GPIO_PORTE_LOCK_R = 0x4C4F434B; // 2) unlock PortE

    GPIO_PORTD_CR_R = 0x0F; // allow changes to PD3-0
    GPIO_PORTE_CR_R = 0x0F; // allow changes to PE3-0

    GPIO_PORTD_DEN_R = 0x0F;
    GPIO_PORTE_DEN_R = 0x0F;

    GPIO_PORTD_AMSEL_R = 0x00; // 3) disable analog function
    GPIO_PORTE_AMSEL_R = 0x00; // 3) disable analog function

    GPIO_PORTD_PCTL_R = 0x00000000; // 4) GPIO clear bit PCTL
    GPIO_PORTE_PCTL_R = 0x00000000; // 4) GPIO clear bit PCTL

    GPIO_PORTD_DIR_R = 0xFF; // 5) PD[0:3] output, others output
    GPIO_PORTE_DIR_R = 0xF0; // 5) PE[0:3] input, others output

    GPIO_PORTD_AFSEL_R = 0x00; // 6) no alternate function
    GPIO_PORTE_AFSEL_R = 0x00; // 6) no alternate function

    GPIO_PORTE_PDR_R = 0x0F; // enable pulldown resistors on PE0-3 rows
} // InitKeyboardPorts

void WriteKeyboardCol(unsigned char nibble)
{
    // Port D is columns
    // When a nibble (e.g. 0010) is written, the column where the 1 is will be made high.
    // When polling the columns, the row that is high (pressed) can be used to determine
    // which button has been pressed
    // By AND'ing Port D with the nibble

    GPIO_PORTD_DATA_R = nibble & 0x0F; // Read 4 LSB
                                       // Nibble chooses which row in mid level
} // WriteKeyboardCol

unsigned char ReadKeyboardRow(void)
{
    // Port E is rows
    // When a button is pressed, and a column is made high by
    // WriteKeyboardCol, the row of the row/column combination
    // will be made high
    // (due to pull down resistors - positive logic)
    // By selectively making columns high, reading the row/column
    // combination allows the pressed button to be deduced
    // All this function has to do is read the value from the rows to
    // check which row is high.

    return GPIO_PORTE_DATA_R;
} // ReadKeyboardRow

// ------------------------ Display functions ------------------------

void SendDisplayNibble(unsigned char byte, unsigned char instruction_or_data)
{
    // Check whether sending instruction or data first
    if (instruction_or_data == 0)
    {
        GPIO_PORTA_DATA_R &= ~0x8; // Set RS to 0 if instruction
    }
    else
    {
        GPIO_PORTA_DATA_R = 0x8; // Set RS to 1 if data
    }

    GPIO_PORTB_DATA_R = (byte) << 2; // Set Port B[2:5] with byte (by left bit-shifting by 2)
    GPIO_PORTA_DATA_R |= 0x04;       // Set EN
    Wait_12_5_Nanosec(36);           //wait for 450 ns (36*12.5ns) i.e. Pulse line for 450 ns
    GPIO_PORTA_DATA_R &= ~0x04;      // Disable EN by clearing bit
} // SendDisplayInstruction

void SendDisplayByte(unsigned char byte, unsigned char instruction_or_data)
{
    SendDisplayNibble(byte >> 4, instruction_or_data); // Send MSB first (bit shift)
    SendDisplayNibble(byte, instruction_or_data);      // Send LSB last
    WaitMicrosec(37);                                  // Wait 37 us

} // SendDisplayInstruction

void InitDisplayPort(void)
{
    // Initialise the various parameters required to interface with the LCD
    // REGISTER INITIALISATION
    SYSCTL_RCGC2_R |= 0x00000003; //activate clock for port A & port B

    unsigned long delay;
    delay = SYSCTL_RCGC2_R; //allow time for clock to start

    GPIO_PORTA_DEN_R = 0x0C; // 1) Enable digital on pins 2 & 3
    GPIO_PORTB_DEN_R = 0x3C; // 1) Enable digital on pins 2 to 5

    GPIO_PORTA_LOCK_R = 0x4C4F434B; // 2) unlock PortA
    GPIO_PORTB_LOCK_R = 0x4C4F434B; // 2) unlock PortB

    GPIO_PORTA_CR_R = 0x0C; // 3) allow changes to PA2&3
    GPIO_PORTB_CR_R = 0x3C; // 3) allow changes to PB2-5

    GPIO_PORTA_AMSEL_R = 0x00; // 4) disable analog function
    GPIO_PORTB_AMSEL_R = 0x00; // 4) disable analog function

    GPIO_PORTA_PCTL_R = 0x00000000; // 5) GPIO clear bit PCTL
    GPIO_PORTB_PCTL_R = 0x00000000; // 5) GPIO clear bit PCTL

    GPIO_PORTA_DIR_R = 0xFF; // 6) All ports set to output (none as input)
    GPIO_PORTB_DIR_R = 0xFF; // 6) All ports set to output (none as input)

    GPIO_PORTA_AFSEL_R = 0x00; // 7) no alternate function
    GPIO_PORTB_AFSEL_R = 0x00; // 7) no alternate function

    // SENDING DATA TO LCD TO INITIALISE DISPLAY
    WaitMillisec(16);           // wait for more than 15 ms
    SendDisplayNibble(0x03, 0); // Function set
    WaitMillisec(5);            // Wait for more than 4.1 ms
    SendDisplayNibble(0x03, 0); // Function set
    WaitMicrosec(110);          // Wait for more than 100 us
    SendDisplayNibble(0x02, 0);

    SendDisplayByte(0x0C, 0); // Function set
    SendDisplayByte(0x08, 0); // Set interface to be 4 bits long

    SendDisplayByte(0x01, 0); // Clear LCD
		SendDisplayByte(0x0C, 0); // Cursor off
    //SendDisplayByte(0x06, 0); // Not required
    SendDisplayByte(0x0E, 0); // Turn LCD On
} // InitDisplayPort

void ClearDisplay()
{
    SendDisplayByte(0x01, 0); // Clear display
    WaitMicrosec(37);
} // ClearDisplay

void TurnCursorOnOff(short int On)
{
    if (On == 0)
    {
        SendDisplayByte(0x0C, 0); // Cursor off
    }
    else
    {
        SendDisplayByte(0x0F, 0); // Cursor on
    }
    WaitMicrosec(37);
} // TurnCursorOnOff

void SetPrintPosition(short int line, short int char_pos)
{
    // LCD registers start from 0
    SendDisplayByte(0x02, 0); // Reset cursor position to 1 (0) Return Home function
    WaitMillisec(2);          // 1.52 ms execution time
    if (line == 2)            // If line input is 2
    {
        SendDisplayByte(0xC0, 0); // Move cursor to 2nd line (function from pg 42 of datasheet)
    }
    // Reject invalid character positions
    if (char_pos < 0)
    {
        char_pos = 0;
    }
    if (char_pos > 16)
    {
        char_pos = 16;
    }

    for (int i = 1; i < char_pos; i++)
    {
        // For the required character postition,
        // this loop iterates, moving the cursor right
        SendDisplayByte(0x14, 0); // Shift cursor right
    }
    WaitMicrosec(40);
    // SetPrintPosition
}

void PrintChar(char ch)
{
		// Switch to determine what character to print to display
    switch (ch)
    {
    case '£':                     // Must be unused character so doesn't affect normal strings
        SendDisplayByte(0xF7, 1); // Send hex for PI
        break;

    case '$':                     // Must be unused character so doesn't affect normal strings
        SendDisplayByte(0xE8, 1); // Send hex for square root sign
        break;
		
    default: // If a special character isn't required to be displayed
        SendDisplayByte(ch, 1); // Send character to display
    }
    WaitMicrosec(37); // Small wait to allow for processing
} // PrintChar

// ------------------------ Flash memory functions ------------------------

void InitFlash()
{
    // Write your code here.
} // InitFlash

void WriteDoubleToFlash(double number)
{
    // Write your code here.
} // WriteFloatToFlash

double ReadDoubleFromFlash()
{
    // Write your code here.
} // ReadFloatFromFlash

// ------------------------ Sundry functions ------------------------
void InitAllOther()
{
    PLL_Init();     // Initialise phase lock loop
    SysTick_Init(); // Initialise System Tick
    InitFlash();    // Initialise flash
} // InitAllOther

void InitAllHardware()
{
    InitAllOther();      // Complete Tiva initialisations above
    InitDisplayPort();   // Initialise LCD
    InitKeyboardPorts(); // Initialise keyboard
} // InitAllHardware

void WaitMicrosec(long int wait_microsecs) // 80 MHz PLL (12.5 ns per cycle)
{
    NVIC_ST_RELOAD_R = (wait_microsecs * 80) - 1; // number of counts to wait
    NVIC_ST_CURRENT_R = 0;                        // any value written to CURRENT clears
    while ((NVIC_ST_CTRL_R & 0x00010000) == 0)
    { // wait for count flag
    }
} // WaitMicrosec

// =============== CUSTOM AND EXTRA FUNCTIONS ================= //
void WaitMillisec(long int wait_millisecs)
{
    /*
	 * The NVIC_ST_RELOAD register is a 24 bit register meaning
	 * the max value it can hold is 2^24-1 = 16777215.
	 * Running with a clock cycle time of 12.5 ns (80 MHz),
	 * the max time that this function can be used for is 
	 * (16777215*12.5 ns) = 0.2097 s ~ 200 ms
	 * I have thus chosen the maximum value that can be input here
	 * to be 200, preventing the register from overflowing
	 */
    if (wait_millisecs > 200)
    { // Limit wait to 200 ms
        wait_millisecs = 200;
    }
    WaitMicrosec(wait_millisecs * 1000);
} // Wait Millisec

void WaitSec(long int wait_secs)
{
    /* As the longest possible wait time is 200ms, longer waits
	 * have to be busy (software waits).
	 * Instead of trying to acheive this in higher levels, a
	 * simple loop is created here to iterate the
	 * WaitMillisec loop a set number of times to achieve the required waits.	
	 */
    for (int i = 1; i <= wait_secs * 5; i++)
    { // Multiplied by 5 as 5*200ms = 1s.
        WaitMillisec(200);
    }
}
void Wait_12_5_Nanosec(long int wait_nanosecs) // Waits 12.5ns
{
    // As clock is running at 80 MHz, the smallest time increment that can
    // be measured is 12.5 ns

    NVIC_ST_RELOAD_R = (wait_nanosecs)-1; // number of counts to wait
    NVIC_ST_CURRENT_R = 0;                // any value written to CURRENT clears
    while ((NVIC_ST_CTRL_R & 0x00010000) == 0)
    { // wait for count flag
    }
} // Wait 12.5Nanosec

void PLL_Init(void)
{
    // 0) Use RCC2
    SYSCTL_RCC2_R |= 0x80000000; // USERCC2
    // 1) bypass PLL while initializing
    SYSCTL_RCC2_R |= 0x00000800; // BYPASS2, PLL bypass
    // 2) select the crystal value and oscillator source
    SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x000007C0) // clear XTAL field, bits 10-6
                   + 0x00000540;                // 10101, configure for 16 MHz crystal
    SYSCTL_RCC2_R &= ~0x00000070;               // configure for main oscillator source
    // 3) activate PLL by clearing PWRDN
    SYSCTL_RCC2_R &= ~0x00002000;
    // 4) set the desired system divider
    SYSCTL_RCC2_R |= 0x40000000;                  // use 400 MHz PLL
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) // clear system clock divider
                                                  //+ (7<<22);      // configure for 50 MHz clock
                    + (4 << 22);                  // configure for 80 MHz
    // 5) wait for the PLL to lock by polling PLLLRIS
    while ((SYSCTL_RIS_R & 0x00000040) == 0)
    {
    }; // wait for PLLRIS bit
    // 6) enable use of PLL by clearing BYPASS
    SYSCTL_RCC2_R &= ~0x00000800;
}
// =========== EXTRA FUNCTIONS (Not written by me) ============== //
void SysTick_Init(void)
{
    NVIC_ST_CTRL_R = 0;            // disable SysTick during setup
    NVIC_ST_RELOAD_R = 0x00FFFFFF; // maximum reload value
    NVIC_ST_CURRENT_R = 0;         // any write to current clears it
    NVIC_ST_CTRL_R = 0x00000005;   // enable SysTick with core clock
}

void UART_Init(void)
{
    SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; // activate UART0
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate port A
    UART0_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
    UART0_IBRD_R = 43;                    // IBRD = int(80,000,000 / (16 * 115200)) = int(43.402778)
    UART0_FBRD_R = 26;                    // FBRD = round(0.402778 * 64) = 26
                                          // 8 bit word length (no parity bits, one stop bit, FIFOs)
    UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
    UART0_CTL_R |= UART_CTL_UARTEN; // enable UART
    GPIO_PORTA_AFSEL_R |= 0x03;     // enable alt funct on PA1-0
    GPIO_PORTA_DEN_R |= 0x03;       // enable digital I/O on PA1-0
                                    // configure PA1-0 as UART
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) + 0x00000011;
    GPIO_PORTA_AMSEL_R &= ~0x03; // disable analog functionality on PA
}