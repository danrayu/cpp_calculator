#include <iostream>
#include <string>
using namespace std;


//function returning range in string including indexes
string range(string s, int first_i, int last_i) {
    string result;
    for (int i = first_i; i < last_i; i++) {
        result += s[i];
    }
    return result;
}


// chech if char is a sign
bool is_sign(char c) {
    bool truth = false;
    if (c == '*' || c == '+' || c == '-' || c == '/' || c == '^') {
        truth = true;
    }
    return truth;
}


// chech if char is not a character used by my program
bool is_unknown(char c) {
    bool truth = false;
    if (c != ' ' && !is_sign(c) && c != '.' &&
        isdigit(c) == 0 && c != ')' && c != '(') {
        truth = true;
    }
    return truth;
}


// power function, doesn't work with float exponents
float power(float base, int exponent) {
    float argument = base;
    if (exponent == 0) {
        return 1;
    }
    for (int exp_i = 0; exp_i < exponent - 1; exp_i++) {
        argument *= base;
    }
    return argument;
}


// verify there is no unintended input
void verify_input(string s) {
    for (int char_i = 0; char_i <= s.length() - 1; char_i++) {
        if (is_unknown(s[char_i])) {
            throw "Unknown characters present. Terminating program.";
        }
    }
}


// actually calculates (+, *, /, -) To be passed only duos
string calculation_operate(string s) {
    float x = 0.0f;
    float y = 0.0f;
    char sign = 0;
    float result;

    // loop until all members (x, y, sign) found
    for (int char_i = 0; char_i <= s.length() - 1; char_i++) {
        char current = s[char_i];

        //find end of number (search_i). char_i is the beginning
        if (isdigit(current)) {
            int search_i;
            for (search_i = char_i; search_i <= s.length()-1; search_i++) {
                if (is_sign(s[search_i])) {
                    break;
                }
            }
           
            //extracting the value of first and second item
            if (char_i<=1) {
                x = std::stof(range(s, char_i, search_i));
            }
            else {
                y = std::stof(range(s, char_i, search_i));
            }
            char_i = search_i - 1;
        }

        //set sign 1+2
        if (is_sign(current)) {
            sign = current;
        }
    }

    // counting according to sign
    if (sign == '*') {
        result = x * y;
    }
    else if (sign == '/') {
        // print out error if divided by zero
        if (y == 0) {
            throw "Tried to divide by 0. Terminating code.";
        }
        result = x / y;
    }
    else if (sign == '^') {
        result = power(x, y);
    }
    else if (sign == '-') {
        result = x - y;
    }
    else if (sign == '+') {
        result = x + y;
    }

    return to_string(result);
}


//returns a section in a string with another string
string insert(string original, string passage, int first_i, int last_i) {
    string first_half = range(original, 0, first_i);
    string second_half = range(original, last_i + 1, original.length());

    return first_half + passage + second_half;
}


// calculates the result of the string
float calculate(string s) {
    verify_input(s);
    float result = 0.f;

    // loop for detecting brackets
    for (int char_i = 0; char_i <= s.length(); char_i++) {
        char current_char = s[char_i];

        // finding '(x+y)' and calling calculate('x+y') end
        if (current_char == '(') {
            int search_index;
            for (search_index = s.length() - 1; search_index > char_i; search_index--) {
                cout << search_index;
                if (s[search_index] == ')') {
                    break;
                }
            }
                    
            s = insert(s, to_string(calculate(range(s, char_i + 1, search_index))),
                char_i + 1, search_index); 
            cout << "brackets: " << s << endl;
            char_i = 0;
            break;
        }
    }

    // for loop for detecting * /
    for (int char_i = 0; char_i <= s.length(); char_i++) {
        char current_char = s[char_i];

        // finding x*y and pasting their results instead of them n+x*y; x*y=t; n+t
        if (current_char == '*' || current_char == '/' || current_char == '^') {

            int start_index = char_i - 1; // +1 and -1 to avoid catching current_char
            int end_index = char_i + 1;
            // finding the beginning of *, ^ or /
            for (start_index; start_index >= 0; start_index--) {
                char start_char = s[start_index];
                if (is_sign(start_char)) {
                    break;
                }
            }

            // finding the ending of *, ^ or /
            for (end_index; end_index <= s.length() - 1; end_index++) {
                char end_char = s[end_index];
                if (is_sign(end_char)) {
                    end_index -= 1;
                    break;
                }
            }

            // replacing with result; if 5*4=20, 20 is result
            s = insert(s, calculation_operate(range(s, start_index + 1, end_index + 1)),
                start_index + 1, end_index);
            cout << "multi: " << s << endl;
            char_i = 0;
        }
    }

    // for loop for detecting + -
    for (int char_i = 0; char_i <= s.length(); char_i++) {
        char current_char = s[char_i];

        if (current_char == '+' || current_char == '-') {

            int start_index = char_i - 1; // +1 and -1 to avoid catching current_char
            int end_index = char_i + 1;

            // finding the beginning of + or -
            for (start_index; start_index >= 0; start_index--) {
                char start_char = s[start_index];
                if (is_sign(start_char)) {
                    break;
                }
            }

            // finding the ending of * or /
            for (end_index; end_index <= s.length() - 1; end_index++) {
                char end_char = s[end_index];
                if (is_sign(end_char)) {
                    end_index -= 1;
                    break;
                }
            }

            // replacing with result; if 5*4=20, 20 is result
            s = insert(s, calculation_operate(range(s, start_index + 1, end_index + 1)),
                start_index + 1, end_index);
            cout << "plus: " << s << endl;
            char_i = 0;
        }
    }
    return stof(s);
}


int main()
{   
    char input_calculator[200] = { 0 };
    cout << "Type in problem: ";
    cin.getline(input_calculator, 200);

    // calculate the string and print any errors/exception 
    try {
        cout << "\nResult: " << calculate(input_calculator);
    }
    catch (const char* msg) {
        cout << msg;
    }
}
