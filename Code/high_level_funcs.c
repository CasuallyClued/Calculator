/* high_level_funcs.c
 *
 * Set of functions at high level (above mid level but below main)  
 * for the 3662 calculator mini-project.
 * 
 * For documentation, see the corresponding .h file.
 * 
 * Dr Chris Trayner, 2019 September
 */

#include "TExaS.h"
#include "high_level_funcs.h"
#include "mid_level_funcs.h"
#include "low_level_funcs_tiva.h"

// ------------------------ Keyboard functions ---------------------

void ReadAndEchoInput(char *input_buffer, int input_buffer_size)
{
    // INITIALISE VARIOUS VARIABLES USED WITHIN FUNCTION //

    int chars_on_display = 0; // Counter variable. By incrementing this each time a character is printed to the
                              // display, this variable represents the number of characters on the display

    int end_input = 0;            // Variable to check whether to leave function (boolean)
    int valid_output = 1;         // Variable to check whether to output character to display (boolean)
    int maths_constant_check = 0; // Variable to check which mathematical constant is required to be printed (boolean)

    char maths_constants[3][7] = {"3.14159", "2.71828", "1.41421"}; // 2D array to store the values of
                                                                    // different maths constants

    const char null = ('\0'); // Variable to hold value for null
                              // (just to make the code easier read)
                              // Makes more sense to have a load of nulls everywhere than have a load of ('\0')'s

    char key_pressed = null; // Variable to hold pressed button character (Initialised to null)
    char output_char = null; // Variable to hold character to be output to display (Initialised to null)

    SetPrintPosition(1, 1); // Set print positon to top left of screen
    TurnCursorOnOff(1);     // Turn cursor on

    // EXTERNAL (ALL INPUTS) LOOP //
    // Enter loop to decipher what character to print
    // to screen or what action to do (e.g. '*', 'D')
    while (end_input == 0)
    {
        maths_constant_check = 0; // Initialise maths_constant check to 0 (don't print maths if normal char)
        valid_output = 1;         // Initialise valid_output boolean to 1

        // Stay in loop until a key_pressed is pressed
        while (key_pressed == null)
        //
        {
            key_pressed = GetKeyboardChar(); //Read the button pressed
            WaitMillisec(200);               // Short wait to stop double presses
        }

        // Leave loop and decipher what character to print
        // to screen or what action to do (e.g. '*', 'D')

        end_input = 0; // Initalise the end_input variable to 0 (check for input)

        // CHECK FOR ANY INPUT //

        // The switch below takes in any valid button press and either sets
        // the output_char variable to the correct character, depending
        // on what key is pressed, OR carries out specific instructions
        // depending on what function is required (e.g. #, rubout).
        // The press for 'D', shift opens up a switch of its own,
        // allowing for different functions/keys to be selected.

        switch (key_pressed) // Choose what to do depending on what key is pressed
        {
            // ====================== SHIFTED PRESSES =============================
            // Wait for another input //
        case 'D': // If D is pressed, wait for another input

            if (chars_on_display == 0) // Counter has been reset and answer is displayed on
                // This prevents a press of the shift button causing
                // the last input string being displayed on the display
            {
                ClearInputBuffer(input_buffer, input_buffer_size); // Clear input buffer
            }
            key_pressed = null;              // Reset key_pressed boolean so can check for new input
            PrintString(1, 1, input_buffer); // Re-print last input_buffer to screen
                                             // On the line below, when shifted, the custom shift
                                             // functions are displayed, as well as the '^' character
                                             // So that the user knows the shift button has been pressed

            PrintString(2, 1, "1=£ 2=e 3=$2   ^");     // On the line below, print the text shift
            SetPrintPosition(1, chars_on_display + 1); // Put the cursor back at the next position

            // Wait for another input - Stay in loop until a key_pressed is pressed
            while (key_pressed == null)
            //
            {
                key_pressed = GetKeyboardChar(); //Read the button pressed
                WaitMillisec(200);               // Short wait to stop double presses
            }
            // SHIFT FUNCTIONS //
            switch (key_pressed)
            {
            case '*':             // End input (User needs to be able to end when shifted or not)
                end_input = 1;    // Set end input variable to 1
                valid_output = 0; // This is not a valid output, so, set value to 0
                break;

            case 'A':
                output_char = 'x'; // Shifted character (valid_output already set to 1)
                break;

            case 'B':
                output_char = '/'; // Shifted character (valid_output already set to 1)
                break;

            case 'C':
                output_char = 'E'; // Shifted character (valid_output already set to 1)
                break;

            case 'D':
                key_pressed = null; // Reset pressed button
                valid_output = 0;   // This is not a valid output, so, set value to 0
                ClearDisplay();     // Clear display
                break;
                // SPECIAL CASES FOR MATHS CONSTANTS
            case '1':
                key_pressed = null;       // Reset pressed button
                valid_output = 0;         // This is not a valid output, so, set value to 0
                maths_constant_check = 1; // Sets the constant to PI
                ClearDisplay();           // Clear display
                break;

            case '2':
                key_pressed = null;       // Reset pressed button
                valid_output = 0;         // This is not a valid output, so, set value to 0
                maths_constant_check = 2; // Sets the constant to e
                ClearDisplay();           // Clear display
                break;

            case '3':
                key_pressed = null;       // Reset pressed button
                valid_output = 0;         // This is not a valid output, so, set value to 0
                maths_constant_check = 3; // Sets the constant to root 2
                ClearDisplay();           // Clear display
                break;

            case '#':
                // Shifted version clears the entire input
                ClearInputBuffer(input_buffer, input_buffer_size); // Clear input buffer
                ClearDisplay();                                    // Clear display
                chars_on_display = 0;                              // Reset counter
                valid_output = 0;                                  // This is not a valid output, so, set value to 0
                key_pressed = null;                                // Reset pressed button
                break;

            default:
                // When valid_output set to zero, pressing a number (otherwise
                // valid character, while shifted will not work. This could (validly in
                // my opinion) be set to 1 meaning that a shifted number press will
                // just return the number
                valid_output = 0;
                output_char = null; // Reset pressed button
                                    // If valid_output set to zero, output_char would need to be set to the character pressed
            }
            break;
            // ================= END OF SHIFTED FUNCTIONS ======================== //

            //==================== NON-SHIFTED FUNCTIONS ========================//
        case '*':             // End input
            end_input = 1;    // Set to 1 so leave loop and calculate_answer
            valid_output = 0; // // This is not a valid output, so, set value to 0
            break;

        case 'A':
            output_char = '+'; // Regular character (valid_output already set to 1)
            break;

        case 'B':
            output_char = '-'; // Regular character (valid_output already set to 1)
            break;

        case 'C':
            output_char = '.'; // Regular character (valid_output already set to 1)
            break;

        case '#':
            if (chars_on_display >= 1) // If there is user input on the screen, rubout the last character
            {
                input_buffer[chars_on_display - 1] = null; // Clears previous character
                ClearDisplay();                            // Clear display
                chars_on_display--;                        // Decrements chars_on_display to move back one space
            }
            else
            {                                                      // If chars_on_display = 0 (answer has been output to screen), clear buffer
                ClearInputBuffer(input_buffer, input_buffer_size); // Clear buffer
                ClearDisplay();                                    // Clear display
            }
            valid_output = 0;   // This is not a valid output, so, set value to 0
            key_pressed = null; // Reset pressed button
            break;

        default:
            output_char = key_pressed; // The number keys are printed exactly as they appear on the keyboard,
        }                              // No conversion is needed

        // ================== PRINTING TO DISPLAY ======================= //
        // The lines of code below print the relevant text to the display
        // They check whether the character to be printed is a mathematical
        // constant (printing a series of chars), whether the required output
        // will fit on the screen and whether to output anything to the display at all
        if (maths_constant_check) // If a mathematical constant is to be displayed
        {
            // Print constant to screen and append to buffer
            if (chars_on_display <= 9) // If there is enough room on the screen to display the 7 digit long constant (16-7 = 9)
            {
                ClearDisplay();              // Clear the display
                for (int i = 0; i <= 6; i++) // Iterate 7 times, once for each character of the string
                {
                    ClearDisplay();                                                                // Clear display
                    input_buffer[chars_on_display] = maths_constants[maths_constant_check - 1][i]; // Set current element to desired character
                    chars_on_display++;                                                            // Increment counter // increase value for characters on the screen
                }
                input_buffer[chars_on_display + 1] = null; // Append trailling null
                PrintString(1, 1, input_buffer);           // Print the buffer
            }
            else // If there isn't enough room to display the entire constant on the display
            {
                PrintDisplayFull(input_buffer); // Print display full
            }
        }

        else if (valid_output == 1 && chars_on_display < 16)
        // If a valid character is to be printed to the screen AND the display isn't already full
        {
            ClearDisplay();                               // Clear display
            input_buffer[chars_on_display] = output_char; // Set current element to desired character
            input_buffer[chars_on_display + 1] = null;    // Append trailling null
            PrintString(1, 1, input_buffer);              // Print the buffer
            chars_on_display++;                           // Increment character
        }
        else if (valid_output == 1 && chars_on_display >= 16)
        // If a valid character is to be printed BUT the display is full
        {
            if (chars_on_display == 16) // As chars_on_display increments everytime a character is added, chars_on_display == 16 represents, 16 characters on the screen
            {
                PrintDisplayFull(input_buffer); // Print display full
            }
        }
        else // If no character is to be printed to screen
        {
            ClearDisplay();                  // Clear display
            PrintString(1, 1, input_buffer); // Re-print buffer to display without modification
        }
        key_pressed = null; // // Reset pressed button
    }
} // ReadAndEchoInput

// ------------------------ Display functions ------------------------

void DisplayResult(double answer)
{
    TurnCursorOnOff(0); // Turn cursor off
    ClearDisplay();     // Clear display

    char converted[] = "";            // Declare empty string
    sprintf(converted, "%G", answer); // convert double to string in standard form
                                      // "%G" produces a number in scientific form when >= 1000000 or < 0.0001
                                      // I deem this produce a suitable output
    PrintString(2, 1, converted);     // Print the converted string to display on line 2
} // DisplayResult

void DisplayErrorMessage(const char *error_message_line1,
                         const char *error_message_line2)
{
    ClearDisplay(); // Clear display
    if (strlen(error_message_line1) <= 17 && strlen(error_message_line2) <= 17 && error_message_line1 != 0 && error_message_line2 != 0)
    // Check if the length of the error messages (including the trailling null) will fit on the display
    {
        PrintString(1, 1, error_message_line1); // Print first line of error on line 1
        PrintString(2, 1, error_message_line2); // Print second line of error on line 2
        TurnCursorOnOff(0);                     // Turn cursor off
        WaitSec(2);                             // Wait for two seconds (to read error), function from low level
        ClearDisplay();                         // Clear display
    }
} // DisplayErrorMessage

// ------------- CUSTOM FUNCTIONS ----------- //
void WelcomeScreen()
{
    WaitMillisec(50);         // Short wait
    const char null = ('\0'); // Variable to hold value for null
    char key_pressed = null;  // Reset pressed button

    for (int i = 1; i <= 4; i++) // Loop a set number of times for animation
    {
        TurnCursorOnOff(0); // Make sure the cursor is off
        ClearDisplay();     // Clear the display

        PrintString(1, i + 2, "£ Kamal's £");    // Print text to display (moving from left to right)
        PrintString(2, 5 - i, "£ Calculator £"); // Print text to display (moving from right to left)
        WaitMillisec(150);                       // Short wait between movements to convey motion
    }
    WaitMillisec(200); // Another wait at the end of the animation
    WaitMillisec(200); // Another wait at the end of the animation
    WaitMillisec(200); // Another wait at the end of the animation
    ClearDisplay();    // Clear the display

    // With if(1), the function waits until input from the user to advance to next funtion
    // In Version 2.0 (before the password functionality), this made more sense
    // Now, the CheckPassword() function acts as the intermediary between starting the device
    // and using the calculator.
    // It makes no sense to have two barriers to using the calculator
    // WelcomeScreen() now simply serves as a fun animation (and hopefully a demonstration
    // of good programming skill)
#if 0
    PrintString(1, 1, "Press any key"); // Print text to display
    PrintString(2, 1, "to calculate");  // Print text to display
		

    // wait for input
    while (key_pressed == null) // While there is no key pressed
    //
    {
        key_pressed = GetKeyboardChar(); //Read the button pressed
        WaitMillisec(200);               // Short wait to stop double presses
    }
#endif
}

void ClearInputBuffer(char *input_buffer, int input_buffer_size)
{
    const char null = ('\0');                    // Variable to hold value for null
    for (int i = 0; i <= input_buffer_size; i++) // For as long as the input buffer is
    {
        input_buffer[i] = null; // Clear the bit in the current position
    }
    ClearDisplay(); // Clear the display
}

void PrintDisplayFull(char *input_buffer)
{

    PrintString(2, 1, "DISPLAY FULL"); // Print full display on the screen
    TurnCursorOnOff(0);                // Turn cursor off
    WaitSec(1);                        // Display text on screen for 1 second
    TurnCursorOnOff(1);                // Turn cursor back on
    ClearDisplay();                    // Clear display
    PrintString(1, 1, input_buffer);   // Re-print buffer to display without modification
}

void CheckPassword(char *password, char *input_buffer)
{
    TurnCursorOnOff(0); // Ensure cursor is off

    const char null = ('\0');               // Variable to hold value for null
    char key_pressed = null;                // Variable to hold the current pressed character
    char *output_char;                      // = null; // Variable to hold character to be output to display
    int password_correct = 0;               // Variable (boolean) to hold the value of whether the password is being entered correctly or not
    char password_buffer[] = "";            // Empty buffer to hold the value of the password
    int password_length = strlen(password); // Variable to hold the length of the password
    int wrong_entry = 0;                    // Variable to hold the number of incorrect entries

    ClearDisplay();         // Clear the display
    SetPrintPosition(1, 1); // Set print position to top left of display

    // Stay in loop while the password has not been entered correctly
    while (!password_correct)
    {
        key_pressed = null;                    // Clear pressed key
        ClearInputBuffer(password_buffer, 16); // Clear the entire password buffer
        ClearInputBuffer(input_buffer, 16);    // Clear the entire password buffer
        password_correct = 1;                  // Set password to correct (true)
                                               // By starting with the value 1, the loop below can simply check whether the
                                               // input matches the password by checking each character entered against
                                               // the corresponding character of the password
                                               // If the input character doesn't match the password's character,
                                               // the password MUST be incorrect

        for (int i = 0; i <= password_length - 1; i++)
        {

            PrintString(1, 1, "Enter Password:"); // Print string to display
                                                  // Wait for input
            while (key_pressed == null)
            {
                SetPrintPosition(2, i + 1);      // Put the cursor at the next position (number being put in)
                TurnCursorOnOff(1);              // Turn cursor on
                key_pressed = GetKeyboardChar(); //Read the button pressed
                WaitMillisec(200);
                TurnCursorOnOff(0); // Turn cursor off (only show cursor when inputting data)
                // Short wait to stop double presses

                if (key_pressed == '#')
                // If the pressed key is #, display a hint
                {
                    TurnCursorOnOff(0);          // Turn Cursor off
                    ClearDisplay();              // Clear the display
                    PrintString(1, 1, "HINT: "); // Print text to display
                    PrintString(1, 6, password); // Print the password to the display at the end of the text
                    WaitSec(1);                  // Small wait so user can read hint

                    ClearDisplay();                       // Clear the display
                    PrintString(1, 1, "Enter Password:"); // Re-print original text to display
                    PrintString(2, 1, password_buffer);   // Re-print the password buffer to display
                    key_pressed = null;                   // Reset the key_pressed variable
                }
            }

            if (key_pressed != password[i])
            // If any key has been entered incorrectly
            {
                password_correct = 0; // Password is incorrect
            }

            password_buffer[i] = '*';           // Print an asterisk
            password_buffer[i + 1] = null;      // Append trailing null
            PrintString(2, 1, password_buffer); // Print the buffer

            key_pressed = null; // Reset pressed key variable (stay in loop)
        }

        if (password_correct == 0) // If the password has been entered incorrectly
        {
            WaitMillisec(200); // Short wait so screen doesn't jump to next thing too suddenly
            ClearDisplay();    // Clear the display

            PrintString(1, 1, "Incorrect PIN"); // Print text to display

            WaitSec(1); // Short wait to allow user to read text

            // The two if statements below mean that every 4 incorrect entries (starting
            // starting with the 2nd), causes the user to be prompted to press '#'
            // for the hint. This stops a new user
            // Of course, this isn't optimally secure, so a real life interperetation of
            // this wouldn't include this functionality, or at least the hint wouldn't
            // be the password itself.

            wrong_entry++;        // Increment wrong entry counter
            if (wrong_entry == 2) // Every second input
            {
                ClearDisplay();                        // Clear the display
                PrintString(2, 1, "Press # for hint"); // Print text to display
                                                       // Prompt user to check the hint
                WaitSec(1);                            // Short wait to allow user to read
            }
            if (wrong_entry > 3) // Every 4 wrong entries...
            {
                wrong_entry = 0; // ...Reset counter back to 0
            }
        }
        else // If the password is correct
        {
            WaitMillisec(200); // Short wait so screen doesn't jump to next thing too suddenly
        }
    }
    ClearDisplay(); // Clear the display
}