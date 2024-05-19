#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#ifndef HELPERS_H
#define HELPERS_H


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
        std::string _operator;
        std::vector<double> _operands;

        // methods
        void set_operator(std::string op) {
            _operator = op;
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
    Helper functions: (OLD // FIX THIS)
        - clean_cmd   : store the string command in the Command object
            - comp_desc         : sort function to sort in descending order;
            - extract_substring : used by clean_cmd to remove parentheses
        - run_ops     : compute a clean operation from the Command object and returns the result
        - print_help  : prints a help message
        - sig_setup   : register signals
        - sig_handler : handle SIGINT (cntrl-c)
*/
void exec_cmd(std::string cmd);
double clean_and_execute(std::string str);

std::string extract_substring(const std::string& input, char start, char end);
bool comp_desc(int a, int b);
std::vector<std::string> split(const std::string& input, char delimiter);
double run_ops(Command cmd);
void throw_error();
int parse_neg(std::string cmd, Command command);

#endif

/*
    Entry point for an uncleaned command, entered by the user,
    to be parsed and executed. The return value is printed to
    the console
*/
void exec_cmd(std::string cmd) {

    // recursively call clean_and_execute to find all nested clauses
    // within each independent clause group

    // execute results of independent clause groups if there are some


    // separate into a list of operands/commands:

    std::string cmd_stream = cmd;
    // create a Command object
    Command command = Command();

    // remove all spaces (so whitespace does not affect anything)
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

    std::vector<std::string> operators = {"+", "-", "*", "/", ">>", "<<", "&", "|", "^"};

    // find independent clauses by searching for valid operators
    std::string clause_raw = extract_substring(cmd_stream, '(', ')');
    if (clause_raw.empty()) {
        // no parentheses were entered:
        clause_raw = cmd_stream;
    }

    // look for operators. get the substring of all before and after an operator
    std::vector<std::string> clauses = {};
    std::string in_focus = "";
    for(std::string op : operators) {
        std::vector<std::string> clauses_searched = split(clause_raw, op[0]);
        if (!clauses_searched.empty()) {
            // a valid operator was found
            clauses = clauses_searched;
            in_focus = op;
            break;
        }
    }
    if (clauses.empty()) {
        // no valid operator was found. throw an error
        throw_error();
    }
    
    // recursively search and execute clause groups in each subgroup.
    // return the result to the highest clause group
    // TODO: do this functionality (for now, limit to simple clause joins)

    // parse each clause
    for (std::string clause : clauses) {
        // TODO: make functionality for multiple clauses
        // for now, make it [operand] <operator> [operand]
        command.push_operand(std::stod(clause));
        command.set_operator(in_focus);
    }

    // execute command
    double ret = run_ops(command);

    // print result
    std::cout << std::fixed << std::setprecision(6) << ret << std::endl;

    return;
}

/*
    Recursive function to clean a single clause, 
    perform the operation on it, and return the result to
    the previous clause (to be the operand) (e.g., find the 
    "bottom" where two operands and not another parenthesis is found)
*/
double clean_and_execute(std::string str) {

    return 0.0;
}

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

// function to sort in descending order
bool comp_desc(int a, int b) {
    return a > b;
}

std::vector<std::string> split(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    
    while ((end = input.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end+1;
    }
    if (tokens.empty()) {
        return tokens;
    }
    
    // Add the last token
    tokens.push_back(input.substr(start));

    
    return tokens;
}

/*
    Takes a Command object, runs the operation, and returns
    the double result
*/
double run_ops(Command cmd) {

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
    else if (ret_operator == "**") { ret = std::pow(op1,op2); }

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
        } else if (ret_operator == "^") { 
            ret = iop1 ^ iop2;
        }
    }

    return ret;
}


/*
    Throws a custom error msg and how to fix it if the user
    entered unrecognized syntax
*/
void throw_error() {
    std::cout << "An Error Occurred. Check source code." << std::endl;
    perror("Custom Error.");
    return;
}

/*
    Given a clause string that contains one or both
    negative operands, parse
    TODO: do this
*/
int parse_neg(std::string str, Command command) {

    // look for negative numbers:
    for (int i=0; i<str.length(); i++) {
        char ch = str[i];
        if (ch == '-') {
            // this is a negative operator:
            if (i+1 == str.length()) throw_error();

            int operand = (int)((str[i+1] - '0'));
            // negate it
            operand = 0-operand;
            command.push_operand(operand);
            // check if this is the last operand:
            if (i+1 == str.length()-1) {

            } else {
                
            }
        }
    }

    return 0;
}