/* 
    PROJECT1 in leaning C++:

    Simple Calculator: Create a basic calculator program that can perform arithmetic 
    operations (addition, subtraction, multiplication, division) based on user input.
*/

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include "helpers.h"

void print_help() {
    std::cout << "--------------------------------------\n" <<
            "               - Help -               \n" <<
            "--------------------------------------\n" <<
            "Command symbols and meanings\n" <<
            "\t - +, -, *, /, **  --> add, subtract, multiply, divide, raise to the power of; these require 2 operands.\n" <<
            "\t - << n, >> n     --> n-left logical shift (*2^n), n-right logical shift (/2^n); these require an operand an an integer n.\n" <<
            "\t - -k             --> negative number, where k is any number.\n" <<
            "\t - &, |, ~, ^     --> Bitwise AND, OR, NOT, XOR; besides NOT, these require 2 operands.\n" <<
            "\t - x.x            --> Floating point number (decimal number).\n" <<
            "\t - ()             --> Parentheses; these must come in pairs.\n" <<
            "--------------------------------------\n" <<
    std::endl;

    return;
}

void sig_handler(int sig_num) {
    // Handle signals
    switch(sig_num) {
        case SIGINT : {
            std::cout << "\n...Quitting Calculator..." << std::endl;
            exit(EXIT_SUCCESS);
            break;
        }
        default : {
            std::cout << "Unknown signal" << std::endl;
            break;
        }
    }
    return;
}

void sig_setup(int sig_num) {
    // call to register signals like SIGINT
    signal(sig_num, sig_handler);
    return;
}

/*
    ------ MAIN --------------------------------
*/
int main() {
    
    // print calculator starting info
    std::cout << "-----------------------------------------------------\n" <<
            "Welcome to the simple calculator!\n" << 
            "-----------------------------------------------------\n" <<
            "How to use: \n" <<
            "\t - Enter any sort of number operation, or binary operation\n" <<
            "\t - Use parentheses to be clear on the order of operations.\n" <<
            "\t - If an operation is not permitted, a helpful error message will tell you how to fix the statement.\n" <<
            "\t - Use the 'q' command, or '^C' (cntrl-C) to quit\n" <<
            "\t - Type 'h' for help on commands\n" <<
            "-----------------------------------------------------\n" <<
    std::endl;

    // register signals
    sig_setup(SIGINT);

    // grab user input
    while(true) {
        // wait for user input
        std::cout << ">> ";
        char cmd[100];
        std::cin.getline(cmd, sizeof(cmd));

        const char* op = cmd;

        // exit condition: when 'q' command entered or SIGINT (ctrl-c) signaled
        if (std::strstr(op, "q\0") != nullptr) {
            std::cout << "...Quitting Calculator..." << std::endl;
            exit(EXIT_SUCCESS);
            return 0;
        }

        // display help if 'h' command entered
        if (std::strstr(op, "h\0") != nullptr) {
            // print help info
            print_help();
            continue;
        }

        std::string to_exec = cmd;

        // execute the command
        exec_cmd(to_exec);
    }
}
