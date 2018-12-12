/*
 * lab3.c
 * Basil Lin
 * basill
 * ECE 222, Fall 2016
 * MP3
 * Subject: ECE222-1,#3
 *
 * Purpose: Perform string operations simulating arithmetic on a finite field.
 * I will first check the user input for any errors or unacceptable inputs,
 * and then perform a variety of arithmetic operations including addition,
 * multiplication, inversion, and power to the strings depending on the user's
 * input.
 *
 * Assumptions:
 *  #1   The user is prompted to enter a pseudo arithmetic command.  The
 *       input must be verified to be grammatically correct.
 *
 *  #2:  The string and character type library cannot be used under
 *       any circumstances.  You are encouraged to develop your own
 *       functions to perform any similar operations that are needed.
 *
 *  #3   No changes to the code in main.  Your code must be placed in
 *       functions.  Additional functions are encouraged.
 *
 * Bugs: none known
 *
 */

// do not include any additional libraries
#include <stdio.h>

// do not change these constants
#define MAXLINE 80
#define MAXOPER 13

// named constants and strings
enum operations { NOOP, ADD, MUL, DIV, POW};
const char *operation_str[] = {"Invalid", "+", "*", "/", "^"};

// function prototypes
int process_input(const char *input, char *op_left, char *op_right);
void calc_output(const char *op_l, int op, const char *op_r, char *result);
int stringlength(const char *string);
void stringcopy(const char src[], char dest[]);
void intcopystring(const int srcint[], char destchar[]);
long long int power(int x, int y);

// do not change any code in main.  We are grading based on the format
// of the output as given in the printfs in main.
int main()
{
    char input_line[MAXLINE];
    char left_operand[MAXOPER];
    char right_operand[MAXOPER];
    char answer[MAXOPER];
    int  operator;

    printf("\nMP3: Arithmetic on GF(47) with + * / ^ using letters\n");
    printf("Commands:\n\tabc+bbc\n\tturtle/frog\n\ttiger^one");
    printf("\tOperands are no more than 12 letters and no spaces\n");
    printf("\tCtrl-d to quit\n\n");
    printf("> ");

    // each call to fgets collects one line of input and stores in input_line
    // BEWARE: fgets includes the end-of-line character '\n' in input_line
    while (fgets(input_line, sizeof input_line, stdin) != NULL) {

        // clear for next round
        left_operand[0] = right_operand[0] = answer[0] = '\0';

        // check for valid grammar
        operator = process_input(input_line, left_operand, right_operand);

        if (operator == ADD || operator == MUL
                || operator == DIV || operator == POW) {

            // print parsed input
            printf("'%s'", left_operand);
            printf(" %s ", operation_str[operator]);
            printf("'%s' => ", right_operand);

            // perform pseudo arithmetic
            calc_output(left_operand, operator, right_operand, answer);

            // print result
            printf("'%s'\n\n", answer);
        } else {
            printf("# %s", input_line);
        }
        printf("> ");
    }
    printf("\nGoodbye\n");
    return 0;
}

/* Parse input of the form SOS where S is a string and O is a character.
 *
 * A string S must consist of up to 12 valid symbols a-z and A-U.
 * The operand O must be one character from: + * / ^
 * Any other characters found in the input, including spaces, are
 * grammatically incorrect and invalidate the input.
 *
 * There must be no spaces anywhere in the input, including between
 * either SO, OS, or leading or trailing spaces.
 *
 * Input: The input string is collected using fgets.  Recall the end-of-line
 *        character is included in the input string and marks the end of
 *        the input.  This string must not be changed.
 *
 * Output: There are three outputs from this function.
 *
 *   The return value is one of NOOP, ADD, MUL, DIV, POW which are
 *      named constants.  If the input is invalid for any reason
 *      then the output must be NOOP.  Otherwise the return value
 *      corresponds to operand O.
 *
 *   If the input is grammatically correct, then two strings are also
 *   returned, one for each of the left and right operands.  If the input
 *   in invalid the two output strings are undefined.
 */
int process_input(const char *input, char *op_left, char *op_right) {

    int i, opvalue = 0;
    char input1[MAXLINE];
    stringcopy(input, input1);
    input1[stringlength(input)-1] = '\0';
    int inputlength = stringlength(input1);

    // finds the operator and if it is valid
    int opcheck = 0;
    for (i = 0; i < inputlength; i++) {
        if ((input1[i] == '+') || (input1[i] == '*') || (input1[i] == '/') || (input1[i] == '^')) {
            opvalue = i; // saves operator position
            opcheck++;
            // returns NOOP when there are too many operators
            if (opcheck > 1) {
                return NOOP;
            }
            // returns NOOP when there is nothing after the operator
            if (!((input1[opvalue + 1] > 64) && (input1[opvalue + 1] < 86)) && !((input1[opvalue + 1] > 96) && (input1[opvalue + 1] < 123))) {
                return NOOP;
            }
        }
    }

    // returns NOOP if operator is not found
    if (opcheck == 0) {
        return NOOP;
    }

    // copies input before operator to op_left
    for (i = 0; i < opvalue; i++) {
        op_left[i] = input1[i];
    }
    op_left[i] = '\0';

    // copies input after operator to op_right
    for (i = opvalue+1; input1[i] != 0; i++) {
        op_right[i-opvalue-1] = input1[i];
    }
    op_right[i - opvalue - 1] = '\0';

    // error detection algorithm
    for (i = 0; i < inputlength; i++) {
        // detects strings greater than length 12
        if (stringlength(op_left) > 12) {
            return NOOP;
        }
        if (stringlength(op_right) > 12) {
            return NOOP;
        }
        // detects if the first character entered is valid (not an operator)
        if (!((input1[0] > 64) && (input1[0] < 86)) && !((input1[0] > 96) && (input1[0] < 123))) {
            return NOOP;
        }
        // checks if input has valid characters
        if ( !((input1[i] > 64) && (input1[i] < 86)) && !((input1[i] > 96) && (input1[i] < 123))
        && (input1[i] != '+') && (input1[i] != '*') && (input1[i] != '/') && (input1[i] != '^') ) {
            return NOOP;
        }
    }

    // conditionals to determine the return type based on the operator
    if (input1[opvalue] == '+') {
        return ADD;
    }
    if (input1[opvalue] == '*') {
        return MUL;
    }
    if (input1[opvalue] == '/') {
        return DIV;
    }
    if (input1[opvalue] == '^') {
        return POW;
    }

    return 0;
}

/* Pseudo mathematical operations on the two operands work as follows.
 *
 * Each character is converted to an integer in the range 1...46, where a is 0,
 * b is 1, c is 2, ..., z is 25.  The operation is then performed using
 * math on a finite field with no carries.
 *
 * If the two input strings are not the same length, then each output character
 * beyond the length of the shorter string should be a copy of the character
 * from the longer string but with the opposite case.
 *
 * Input: The two operand strings and the operator are assumed to be valid (and
 *        are verified as valid in the parse_input function).
 *
 * Output: The final string generated by the above rules is stored in the
 *         output string named result.  The input strings must not be
 *         changed.
 */
void calc_output(const char *l_op, int op, const char *r_op, char *result) {

    int i, total[MAXOPER] = {0};
    char loperator[MAXOPER] = {0};
    char roperator[MAXOPER] = {0};
    int lengthl = stringlength(l_op);
    int lengthr = stringlength(r_op);

    // Copies strings so that they can be edited
    stringcopy(l_op, loperator);
    stringcopy(r_op, roperator);

    // Determines correct numerical value for l_op
    for (i = 0; i < lengthl; i++) {
        if (loperator[i] < 86) {
            loperator[i] -= 39;
        }
        else {
            loperator[i] -= 97;
        }
    }
    // Determines correct numerical value for r_op
    for (i = 0; i < lengthr; i++) {
        if (roperator[i] < 86) {
            roperator[i] = roperator[i] - 39;
        }
        else {
            roperator[i] -= 97;
        }
    }


    // Conditionals to carry out intended operation

    // adds l_op and r_op
    if (op == ADD) {
        // Adds the strings together when strings are equal
        if (lengthl == lengthr) {
            for (i = 0; i < lengthl; i++) {
                // Corrects output if greater than 46
                if ((loperator[i] + roperator[i]) > 46) {
                    total[i] = (loperator[i] + roperator[i]) % 47;
                }
                else { // Adds output regularly when <= 46
                    total[i] = loperator[i] + roperator[i];
                }
            }
        }
        // Adds strings and flips l_op case since l_op is longer
        if (lengthl > lengthr) {
            // Adds where both strings have inputs
            for (i = 0; i < lengthr; i++) {
                if ((loperator[i] + roperator[i]) > 46) {
                    total[i] = (loperator[i] + roperator[i]) % 47;
                }
                else {
                    total[i] = loperator[i] + roperator[i];
                }
            }
            // Flips case where strings cannot be added together
            for (i = lengthr; loperator[i] != '\0' || i < lengthl; i++) {
                if (loperator[i] < 26) {
                    loperator[i] += 26;
                }
                else {
                    loperator[i] -= 26;
                }
                total[i] += loperator[i];
            }
        }
        // Adds strings and flips r_op case since r_op is longer
        if (lengthr > lengthl) {
            for (i = 0; i < lengthl; i++) {
                if ((loperator[i] + roperator[i]) > 46) {
                    total[i] = (loperator[i] + roperator[i]) % 47;
                }
                else {
                    total[i] = loperator[i] + roperator[i];
                }
            }
            for (i = lengthl; roperator[i] != '\0' || i < lengthr; i++) {
                if (roperator[i] < 26) {
                    roperator[i] += 26;
                }
                else {
                    roperator[i] -= 26;
                }
                total[i] += roperator[i];
            }
        }
    }

    // Multiplies l_op and r_op
    if (op == MUL) {
        // Multiplies when strings are same size
        if (lengthl == lengthr) {
            for (i = 0; i < lengthl; i++) {
                total[i] = (loperator[i] * roperator[i]) % 47;
            }
        }
        // Multiplication algorithm if l_op is longer than r_op
        if (lengthl > lengthr) {
            for (i = 0; i < lengthr; i++) {
                total[i] = (loperator[i] * roperator[i]) % 47;
            }
            // Flips case where strings aren't multiplied
            for (i = lengthr; loperator[i] != '\0' || i < lengthl; i++) {
                if (loperator[i] < 26) {
                    loperator[i] += 26;
                }
                else {
                    loperator[i] -= 26;
                }
                total[i] = loperator[i];
            }
        }
        // Multiplication algorithm if r_op is longer than l_op
        if (lengthr > lengthl) {
            for (i = 0; i < lengthl; i++) {
                total[i] = (loperator[i] * roperator[i]) % 47;
            }
            // Flips case where strings aren't multiplied
            for (i = lengthl; roperator[i] != '\0' || i < lengthr; i++) {
                if (roperator[i] < 26) {
                    roperator[i] += 26;
                }
                else {
                    roperator[i] -= 26;
                }
                total[i] = roperator[i];
            }
        }
    }

    // Performs inversion
    if (op == DIV) {
        int z = -1; // Inversion result
        int k = 0;
        // performs inversion when strings are the same length
        if (lengthl == lengthr) {
            for (i = 0; i < lengthr; i++) {
                if (roperator[i] == 0) { // Checks against 0's in the denominator
                    total[i] = 0;
                }
                else {
                    z = -1;
                    k = 0;
                    while (z == -1) {
                        if (roperator[i] * k % 47 == loperator[i]) {
                            z = k;
                        }
                        k++;
                    }
                    while (z > 46) { // Wraps values too large
                        z -= 46;
                    }
                    total[i] = z;
                }
            }
        }
        // Performs inversion when l_op is longer than r_op
        if (lengthl > lengthr) {
            for (i = 0; i < lengthr; i++) {
                if (roperator[i] == 0) {
                    total[i] = 0;
                }
                else {
                    z = -1;
                    k = 0;
                    while (z == -1) {
                        if (roperator[i] * k % 47 == loperator[i]) {
                            z = k;
                        }
                        k++;
                    }
                    while (z > 46) {
                        z -= 46;
                    }
                    total[i] = z;
                }
            }
            // Flips case where one string is longer
            for (i = lengthr; loperator[i] != '\0' || i < lengthl; i++) {
                if (loperator[i] < 26) {
                    loperator[i] += 26;
                }
                else {
                    loperator[i] -= 26;
                }
                total[i] = loperator[i];
            }
        }
        // Performs inversion when r_op is greater than l_op
        if (lengthr > lengthl) {
            for (i = 0; i < lengthl; i++) {
                if (roperator[i] == 0) {
                    total[i] = 0;
                }
                else {
                    z = -1;
                    k = 0;
                    while (z == -1) {
                        if (roperator[i] * k % 47 == loperator[i]) {
                            z = k;
                        }
                        k++;
                    }
                    while (z > 46) {
                        z -= 46;
                    }
                    total[i] = z;
                }
            }
            // Flips case where one string is longer
            for (i = lengthl; roperator[i] != '\0' || i < lengthr; i++) {
                if (roperator[i] < 26) {
                    roperator[i] += 26;
                }
                else {
                    roperator[i] -= 26;
                }
                total[i] = roperator[i];
            }
        }
    }

    // Performs power on a finite field
    if (op == POW) {
        // Power when l_op is the same length as r_op
        if (lengthl == lengthr) {
            for (i = 0; i < lengthl; i++) {
                total[i] = power(loperator[i], roperator[i]) % 47;
            }
        }
        // Power when l_op is longer than r_op
        if (lengthl > lengthr) {
            for (i = 0; i < lengthr; i++) {
                total[i] = power(loperator[i], roperator[i]) % 47;
            }
            // Flips case where one string is longer
            for (i = lengthr; loperator[i] != '\0' || i < lengthl; i++) {
                if (loperator[i] < 26) {
                    loperator[i] += 26;
                }
                else {
                    loperator[i] -= 26;
                }
                total[i] = loperator[i];
            }
        }
        // Power when r_op is longer than l_op
        if (lengthr > lengthl) {
            for (i = 0; i < lengthl; i++) {
                total[i] = power(loperator[i], roperator[i]) % 47;
            }
	    // Flips case where one string is longer
            for (i = lengthl; roperator[i] != '\0' || i < lengthr; i++) {
                if (roperator[i] < 26) {
                    roperator[i] += 26;
                }
                else {
                    roperator[i] -= 26;
                }
                total[i] = roperator[i];
            }
        }
    }

    // Changes integers back to ascii characters
    if (lengthr > lengthl) {
        for (i = 0; i < lengthr; i++) {
            if (total[i] < 26) {
                total[i] += 97;
            }
            else {
                total[i] += 39;
            }
        }
    }
    else {
        for (i = 0; i < lengthl; i++) {
            if (total[i] < 26) {
                total[i] += 97;
            }
            else {
                total[i] += 39;
            }
        }
    }
    intcopystring(total, result); // Copies ascii codes to result for printing
}

/*
 * Purpose: count the length of strings)
 * inputs: character array
 * outputs: length of the string
 */
int stringlength(const char *string) {
    int len = 0;
    while (string[len] != '\0') {
        len++;
    }
    return len;
}

/*
 * Purpose: copies a string to another array
 * inputs: source array and destination array
 * outputs: none
 */
void stringcopy(const char src[], char dest[]) {
   int j = 0;
   while (src[j] != '\0') {
      dest[j] = src[j];
      j++;
   }
   dest[j] = '\0';
}

/*
 * Purpose: Copies and int array to a character array
 * inputs: source integer array, destination character array
 * outputs: none
 */
void intcopystring(const int srcint[], char destchar[]) {
   int j = 0;
   while (srcint[j] != 0) {
      destchar[j] = srcint[j];
      j++;
   }
   destchar[j] = '\0';
}

/*
 * Purpose: calculate the solution to a base raised to a power
 * inputs: integer base and integer power that the base is raised to
 * outputs: solution to the base raised to the power
 */
long long int power(int x, int y) {
    int i;
    long long int ans = 1;
    for (i = 0; i < y; i++) {
        ans *= x;
    }
    return ans;
}

