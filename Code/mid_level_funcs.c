/* mid_level_funcs.c
 *
 * Set of functions at middle level (above low-level hardware drivers but below the 
 * high level) for the 3662 calculator mini-project.
 * 
 * For documentation, see the documentation in the corresponding .h file.
 * 
 * Dr Chris Trayner, 2019 September
 */

#include "TExaS.h"
#include "mid_level_funcs.h"
#include "low_level_funcs_tiva.h"

// ------------------------ Keyboard functions ------------------------

char GetKeyboardChar()
{
    // Initialise variables
    int row = 0;    // Variable to hold row number
    int col = 0;    // Variable to hold column number
    char character; // Variable to hold character

    KeyboardReadRowCol(&row, &col);            // Get row and keyboard values
    character = KeyboardRowCol2Char(row, col); // Set variable to values

    return character; // Return character value
} // GetKeyboardChar

void KeyboardReadRowCol(int *row, int *col)
{
    // Initialise variables
    int button_pressed = 0;                             // Variable to hold whether button pressed or not (boolean)
    int i = 0;                                          // Counter
    unsigned char columns[] = {0x01, 0x02, 0x04, 0x08}; // Array to hold the different valid columns

    while (button_pressed == 0)
    // When a button isn't pressed, cycle through the columns
    // making each high, one at a time.
    // This allows a value for the rows to be checked
    {
        WriteKeyboardCol(columns[i]);
        // Make each column high in a row
        *col = i + 1; // Set column value
                      // The current column that is made high is the current column
                      // that has been pressed
        WaitMicrosec(1000);

        switch (ReadKeyboardRow())
        // The current row that is high is the current row that has been pressed
        {
        case 0x01:
            *row = 1;           // Set row value
            button_pressed = 1; // Leave while loop
            break;

        case 0x02:
            *row = 2;           // Set row value
            button_pressed = 1; // Leave while loop
            break;

        case 0x04:
            *row = 3;           // Set row value
            button_pressed = 1; // Leave while loop
            break;

        case 0x08:
            *row = 4;           // Set row value
            button_pressed = 1; // Leave while loop
            break;
        }
        WaitMillisec(1); // Short wait

        if (i == 3)
        {
            i = 0; // Reset Counter
        }
        else
        {
            i++; // Increment counter
        }
    }
} // KeyboardReadRowCol

char KeyboardRowCol2Char(int row, int col)
{
    // Return the character printed on the button pressed
    // Row and column combinations correspond to specific
    // button presses.
    if (row == 1 && col == 1)
    {
        return '1'; // Return character
    }
    else if (row == 1 && col == 2)
    {
        return '2'; // Return character
    }
    else if (row == 1 && col == 3)
    {
        return '3'; // Return character
    }
    else if (row == 1 && col == 4)
    {
        return 'A'; // Return character
    }

    else if (row == 2 && col == 1)
    {
        return '4'; // Return character
    }
    else if (row == 2 && col == 2)
    {
        return '5'; // Return character
    }
    else if (row == 2 && col == 3)
    {
        return '6'; // Return character
    }
    else if (row == 2 && col == 4)
    {
        return 'B'; // Return character
    }

    else if (row == 3 && col == 1)
    {
        return '7'; // Return character
    }
    else if (row == 3 && col == 2)
    {
        return '8'; // Return character
    }
    else if (row == 3 && col == 3)
    {
        return '9'; // Return character
    }
    else if (row == 3 && col == 4)
    {
        return 'C'; // Return character
    }

    else if (row == 4 && col == 1)
    {
        return '*'; // Return character
    }
    else if (row == 4 && col == 2)
    {
        return '0'; // Return character
    }
    else if (row == 4 && col == 3)
    {
        return '#'; // Return character
    }
    else if (row == 4 && col == 4)
    {
        return 'D'; // Return character
    }
    else
    {
        return '?'; // Return character (invalid entry)
    }
} // KeyboardRowCol2Char

// ------------------------ Display functions ------------------------

void PrintString(short int line, short int char_pos, const char *string)
{
    int length = strlen(string);      // Variable to store the length of the string
    SetPrintPosition(line, char_pos); // function already rejects invalid line/char pos

    if (length <= 16) // If string length can fit on screen
                      // Prevents any string being printed beyond the screen
    {
        for (int i = 0; i < length; i++)
        {
            PrintChar(string[i]);             // Print character in position
            char_pos += 1;                    // Increment position to be written to
            SetPrintPosition(line, char_pos); // Move cursor to next position
        }
    }
    else
    { // If string is longer than 16 chars, only print 16 chars
        for (int i = 0; i < 16; i++)
        {
            PrintChar(string[i]);             // Print character in position
            char_pos += 1;                    // Increment position to be written to
            SetPrintPosition(line, char_pos); // Move cursor to next position
        }
    }
}
// PrintString

/* DEBUG Functions
 * These functions were created to test the functionality of the
 * keyboard before the high level functions were completed
 * NOTE - mid_level_funcs.h must be included in main.c for these
 * functions to be used
*/

void Test_Keyboard()
{
    // This function prints the character Y on the screen if any
    // button is pressed and prints N if none
    // The launchpad must be reset after every press
    // and the button must be held while it boots

    //WriteKeyboardCol(0x0F); // All columns made high
    WriteKeyboardCol(0x08); // Column 4 made high

    unsigned char rows = ReadKeyboardRow();

    //if (rows != 0) { // Checks if any button pressed
    if (rows == 0x04)
    {                     // Checks if Key 1 pressed
        PrintChar('Y');   // Print character to screen
        WaitMicrosec(37); // Short wait
    }
    if (rows == 0)
    {                     // Checks if button not
        PrintChar('N');   // Print character to screen
        WaitMicrosec(37); // Short wait
    }
}
void Test_Keyboard_2()
{
    // Checks if KeyboardReadRowCol function works
    // by returning Y for key pressed, N for none
    int row = 0;
    int col = 0;
    KeyboardReadRowCol(&row, &col);
    WaitMicrosec(2000);
    if (row != 0)
    {
        PrintChar('Y'); // Print Y character to the display
    }
    else
    {
        PrintChar('N'); // Print N character to the display
    }
}
void Test_Keyboard_3()
{
    char c = GetKeyboardChar(); // Get input from button
    ClearDisplay();
    WaitMicrosec(500); // Short wait
    PrintChar(c);      // Print character to display
    WaitMicrosec(500); // Short wait
}