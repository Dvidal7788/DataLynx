# file_read_library
Library for reading from files, especially csv files.

This library contains functions with the intention of giving a C programmer close to the ease of use of Python, but with the control of C.


LIST OF FUCNTIONS:

___ INF_BUFFER() ____
char *inf_buffer(char *prompt)

- PARAMETERS: Takes string as input. Will prompt user with inputted string.
- Will malloc and realloc one char at a time until '\n' is reached, at which point that char will changed to '\0'.
- RETURN: Will return dynamically allocated string. It is up to the programmer to free this string in the calling function.

___ IF_ERROR() ___
void if_error(int16_t error_num)

- PARAMETERS: Takes integer (i.e. error code) as input.
- 1. Opens file errorKey.txt (which contains all of the potential error codes), scans the error key file for the appropriate corresponding error message.
- 2. Reads error message into dynamically allocated buffer (for ultimate modularity and ability for expansion moving forward)
- 3. Creates timestamp
- 4. Opens errorlog.csv, prints error code, error message and timestamp to error log
- 5. Prints error code/error message to the terminal
- 6. Exits program, returning correct error message from main().
- Note: This 1st iteration of this function took an error message as an input to the function, hwowver, when creating a program in C and trying to make it as robust as possible, of course there are many NULL checks and error checks throughout the program, each of which has an if statement that calls this if_error() function, if triggered. Since each of those calls to if_error() took an error message string as input, this began to quickly make the code look very messy. So, for ultimate modularity, ease of use and maintenance moving forward (in addition to cleaning up messy code in the calling function), this function was redesigned to read the corresponding error message from an error key).
- RETURN: No return. Exits program before returning.

