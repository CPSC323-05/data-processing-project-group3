#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <set>

// This function removes leading and trailing whitespace from a string
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Function to check if a character is an alphabet letter
bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Function to check if a character is a digit
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

// Function to check if a character is an operator
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '%');
}

// Function to check if a character is a delimiter
bool isDelimiter(char c) {
    return (c == ' ' || c == ',' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']');
}

// Function for processing the file, removing comments and excess whitespace, including empty lines
void processFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile);
    std::ofstream output(outputFile);
    std::string line;

    while (getline(input, line)) {
        std::string trimmedLine = trim(line); // Remove leading and trailing whitespace
        size_t commentIndex = trimmedLine.find("//");
        if (commentIndex != std::string::npos) {
            trimmedLine = trim(trimmedLine.substr(0, commentIndex)); // Remove comments and trailing whitespace after comments
        }
        if (!trimmedLine.empty()) { // Check if line still has content after trimming
            output << trimmedLine << std::endl; // Write to output file if line is not empty
        }
    }
}

// Function to tokenize and print output
void tokenize(const std::string& code) {
    std::set<std::string> keywords = {"int", "return", "using", "namespace"};
    std::vector<char> operators = {'=', '+', '*', '/', '=', '%'};
    std::vector<char> delimiters = {'(', ')', ':', ',', '{', '}', ';', '[', ']'};
    
    std::set<std::string> uniqueKeywords;
    std::set<std::string> uniqueIdentifiers;
    std::set<std::string> uniqueLiterals;
    std::set<char> uniqueOperators;
    std::set<char> uniqueDelimiters;

    std::string currentToken;
    bool isStringLiteral = false;

    auto commitToken = [&](char c = '\0') {
        if (!currentToken.empty()) {
            if (keywords.find(currentToken) != keywords.end()) {
                uniqueKeywords.insert(currentToken);
            } else if (std::isdigit(currentToken[0])) {
                uniqueLiterals.insert(currentToken);
            } else {
                uniqueIdentifiers.insert(currentToken);
            }
            currentToken.clear();
        }
        if (c != '\0') {
            currentToken += c;
        }
    };

    for (size_t i = 0; i < code.size(); ++i) {
        char c = code[i];

        if (c == '\"') {
            isStringLiteral = !isStringLiteral;
            currentToken += c;
            if (!isStringLiteral) {
                uniqueIdentifiers.insert(currentToken); // Treat string literals as identifiers
                currentToken.clear();
            }
        } else if (isStringLiteral) {
            currentToken += c;
        } else if (std::find(operators.begin(), operators.end(), c) != operators.end()) {
            commitToken();
            uniqueOperators.insert(c);
        } else if (std::find(delimiters.begin(), delimiters.end(), c) != delimiters.end()) {
            commitToken();
            uniqueDelimiters.insert(c);
        } else if (std::isspace(c)) {
            commitToken();
        } else {
            currentToken += c;
        }
    }
    commitToken(); // Commit any remaining token
    // Output section
    std::cout << "  -----------------------------------------------------------------------" << std::endl;
    std::cout << "  Lexeme                              Token" << std::endl;
    std::cout << "  ----------------------------------- -----------------------------------" << std::endl;
    
    for (const auto& kw : uniqueKeywords) {
        std::cout << "  " << std::setw(36) << std::left << kw << "Keyword" << std::endl;
    }
    for (const auto& id : uniqueIdentifiers) {
        std::cout << "  " << std::setw(36) << std::left << id << "Identifier" << std::endl;
    }
    for (const auto& lit : uniqueLiterals) {
        std::cout << "  " << std::setw(36) << std::left << lit << "Literal" << std::endl;
    }
    for (const auto& op : uniqueOperators) {
        std::cout << "  " << std::setw(36) << std::left << std::string(1, op) << "Operator" << std::endl;
    }
    for (const auto& delim : uniqueDelimiters) {
        std::cout << "  " << std::setw(36) << std::left << std::string(1, delim) << "Delimiter" << std::endl;
    }

    std::cout << "  -----------------------------------------------------------------------" << std::endl;
}


int main() {
    std::string inputFilename = "input.txt";
    std::string outputFilename = "output.txt";

    // Process the file: remove excess spaces and comments
    processFile(inputFilename, outputFilename);

    // Display code after removing comments and excess spaces
    std::cout << "Output1 - Code after removing excess space and comments:" << std::endl;
    std::ifstream input(outputFilename);
    std::string line;
    while (getline(input, line)) {
        std::cout << line << std::endl;
    }
    input.close();

    // Display tokenized code in a tabular format
    std::cout << std::endl << "Output2 - Tokenized code in tabular form:" << std::endl;
    input.open(outputFilename);
    std::string code((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    input.close();
    tokenize(code);

    return 0;
}