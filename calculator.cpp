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
// constructor definition (set _operator)
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
            - comp_desc         : sort function to sort in descending order;
            - extract_substring : used by clean_cmd to remove parentheses
        - run_ops     : compute a clean operation from the Command object.
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

// function for sort to help sort in descending order
bool comp_desc(int a, int b) {
    return a > b;
}

Command clean_cmd(char* cmd) {
    // separate into a list of operands/commands:

    std::string cmd_stream = cmd;
    // create a Command object
    Command command = Command();

    // remove all spaces (so whitespace does not affect anything)
    char space = ' ';
    // add all space indexes to a list to remove them after finding
    // them (avoid concurrrent mod error)
    std::vector<int> to_remove = {};
    for(int i=0; i<cmd_stream.length(); i++) {
        if(cmd_stream.at((size_t)i) == ' ') {
            to_remove.push_back(i);
        }
    }
    // sort the 'to_remove' vector in descending order,
    // so that removes from the end will not affect indices
    // near the beginning
    if(!to_remove.empty()) {
        std::sort(to_remove.begin(), to_remove.end(), comp_desc);
    }
    // now remove all ints from `to_remove`:
    for(int num : to_remove) {
        cmd_stream.erase(num, 1);
    }

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

    // add the operands to the Command object
    // if this is a simple clause, there are 3 chars --> op1, operator, op2
    if(clause_raw.length() == 3) {
        // the operator is +,-,*,/,&,|, or ^=
        command.push_operand(clause_raw[0] - '0');
        command.push_operand(clause_raw[2] - '0');
        // extract operator
        char op[2];
        op[0] = clause_raw[1];
        op[1] = '\0'; 
        command.set_operator((char*)op);
    } else {
        // this is a non-standard clause:

        // TODO: add functionality for negative numbers

        command.push_operand(clause_raw[0] - '0');
        command.push_operand(clause_raw[3] - '0');
        // extract operator
        char op[3];
        op[0] = clause_raw[1];
        op[1] = clause_raw[2];
        op[2] = '\0'; 
        command.set_operator((char*)op);
    }

    return command;
}

void run_ops(Command cmd) {

    // notes::::
    /*
        bash bitwise shift operator: 
            $((2 << 4)) (ret: 32)
            ^= --> XOR
        reference : https://www.baeldung.com/linux/bash-bitwise-operators
    */

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
            ret = iop1 * std::pow(2,iop2);
        } else if (ret_operator == ">>") { 
            ret = iop1 * std::pow(2,-iop2);
        } else if (ret_operator == "&") { 
            ret = iop1 & iop2;
        } else if (ret_operator == "|") { 
            ret = iop1 | iop2;
        } else if (ret_operator == "^=") { 
            ret = iop1 ^= iop2;
        }
    }

    // print result
    std::cout << std::fixed << std::setprecision(6) << ret << std::endl;

    return;
}

void print_help() {
    std::cout << "--------------------------------------\n" <<
            "               - Help -               \n" <<
            "--------------------------------------\n" <<
            "Command symbols and meanings\n" <<
            "\t - +, -, *, /   --> add, subtract, multiply, divide; these require 2 operands.\n" <<
            "\t - << n, >> n   --> n-left logical shift (*2^n), n-right logical shift (/2^n); these require an operand an an integer n.\n" <<
            "\t - -k           --> negative number, where k is any number.\n" <<
            "\t - &, |, ~, ^=  --> Bitwise AND, OR, NOT, XOR; these require 2 operands.\n" <<
            "\t - x.x          --> Floating point number (decimal number).\n" <<
            "\t - ()           --> Parentheses; these must come in pairs.\n" <<
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
        clauses.clear();

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
