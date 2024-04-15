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
#include <unistd.h>   // For execvp
#include <sys/wait.h> // For waitpid
#include <cstdlib>     // For exit

/*
    Class define: Command
        Purpose: one executable clause in a string of clauses (statement) executed by 
                    the calculator
        Attributes:
            - operators : std::vector<char*> - all symbol operators
            - operands : std::vector<double> - all int/double operands
        Methods:
            - set_operator(char* op) {}   --> void  : set the operand
            - push_operand(double rand) {}    --> void  : push an operator to the operators list
*/
class Command {
    public:
        // constructor
        Command();

        // attributes
        char* _operator;
        std::vector<double> _operands;

        // methods
        void set_operator(char* op) {
            _operator = strdup(op);
        }
        void push_operand(double rand) {
            _operands.push_back(rand);
            return;
        }
};
// constructor definition (does nothing yet)
Command::Command() {
    _operator = "";
}


/*
    Example usage of the Command class
    std::vector<char*> ops = { "add", "subtract", "multiply" };
    std::vector<double> nums = { 10.5, 20.7, 5.0 };

    Command cmd(ops, nums);

    // Accessing the attributes of the cmd object
    std::cout << "Operators:" << std::endl;
    for (const auto& op : cmd.operators) {
        std::cout << op << std::endl;
    }

    std::cout << "Operands:" << std::endl;
    for (const auto& num : cmd.operands) {
        std::cout << num << std::endl;
    }
*/

/*
    Helper functions:
        - clean_cmd   : store the string command in the Command object
            - extract_substring : used by clean_cmd to remove parentheses
        - run_ops     : convert a Command object instance into a bash script to run.
        - print_help  : prints a help message
        - sig_setup   : register signals
        - sig_handler : handle SIGINT (cntrl-c)
*/
std::string extract_substring(const std::string& input, char start, char end) {
    const size_t start_idx = input.find(start);
    if (start_idx == std::string::npos) {
        // delim not found
        return "";
    }
    const size_t end_idx = input.find(end, start_idx+1);
    if (end_idx == std::string::npos) {
        // delim not found
        return "";
    }
    return input.substr(start_idx+1, end_idx-start_idx-1);
}

Command clean_cmd(char* cmd) {
    // separate into a list of operands/commands:
    // char* token;
    // char* saveptr;

    //std::vector<char*> all_tokens;
    std::string cmd_stream = cmd;
    
    // create a Command object
    Command command = Command();

    // TODO: remove all spaces

    // TODO: add better functionality for multiple
    // clause groups and nested clauses.
    // right now, functionality is limited to 1 clause with 1 set
    // of parantheses MAX.

    // look for clause groups:
    std::string clause_raw = extract_substring(cmd_stream, '(', ')');
    if (clause_raw.empty()) {
        // no parentheses were entered:
        clause_raw = cmd_stream;
    } else {
        // TODO: find other clauses:

        // while(!clause_raw.empty()) {
        //     // extract first operand(s):
        //     command.push_operand((double)clause_raw[0]);
        //     // extract operator
        //     command.set_operator((char*)clause_raw[1]);
        //     // search for another clause
        //     clause_raw = extract_substring(cmd_stream, '(', ')');
        // }
    }
    // add the operands to the Command object (convert to double)
    command.push_operand(clause_raw[0] - '0');
    command.push_operand(clause_raw[2] - '0');
    // extract operator
    char op[2];
    op[0] = clause_raw[1];
    op[1] = '\0'; 
    command.set_operator((char*)op);
    
    // std::cout << "all tokens parsed:" << std::endl;
    // for (const auto& num : all_tokens) {
    //     std::cout << num << std::endl;
    // }

    // test ; change this
    // char* op = "*";
    // std::vector<double> nums = { 10.5, 20.7, 5.0 };

    return command;
}

void run_ops(Command cmd) {

    // perform the calculation and print the result
    double ret = 0.0;
    double op1 = cmd._operands[0];
    double op2 = cmd._operands[1];
    std::string ret_operator = cmd._operator;

    // normal operations
    if (ret_operator == "+") { ret = op1 + op2;}
    else if (ret_operator == "-") { ret = op1 - op2; }
    else if (ret_operator == "*") {ret = op1 * op2; }
    else if (ret_operator == "/") { ret = op1 / op2; }
    else if (ret_operator == "-") { ret = op1 - op2; }

    // binary operations
    else {
        int iop1 = (int)op1;
        int iop2 = (int)op2;
        if (ret_operator == "<<") { 
            ret = iop1 << iop2;
        } else if (ret_operator == ">>") { 
            ret = iop1 >> iop2;
        } else if (ret_operator == "&") { 
            ret = iop1 & iop2;
        } else if (ret_operator == "|") { 
            ret = iop1 | iop2;
        } else if (ret_operator == "^=") { 
            ret = iop1 ^= iop2;
        }
    }

    // print result
    std::cout << ret << std::endl;

    return;
}

void print_help() {
    std::cout << "--------------------------------------\n" <<
            "               - Help -               \n" <<
            "--------------------------------------\n" <<
            "Command symbols and meanings\n" <<
            "\t - +, -, *, /   --> add, subtract, multiply, divide; these require 2 operands.\n" <<
            "\t - << n, >> n   --> n-left logical shift (*2^n), n-right logical shift (/2^n); these require an operand an an integer n.\n" <<
            "\t - -k           --> negative number; only when k is an integer and the dash directly follows k with no whitespace.\n" <<
            "\t - &, |, ~, ^=  --> Bitwise AND, OR, NOT, XOR; these require 2 operands.\n" <<
            "\t - x.x          --> Floating point number (decimal number), requires no whitespace between integers (x) and point.\n" <<
            "\t - ()           --> Parentheses; these must come in pairs.\n" <<
            "--------------------------------------\n" <<
    std::endl;
    // notes::::
    /*
        bash bitwise shift operator: 
            $((2 << 4)) (ret: 32)
            ^= --> XOR
        reference : https://www.baeldung.com/linux/bash-bitwise-operators
    */

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

// Global Vars:
// a list of all operational clauses in a statement, gets reset on every new loop
std::vector<Command*> clauses;

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

        // reset clauses list:
        //clauses = {};

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

        // clean up the command and store operators/operands
        // in a Command object
        Command to_add = clean_cmd(cmd);

        // run command
        run_ops(to_add);
        
    }
}
