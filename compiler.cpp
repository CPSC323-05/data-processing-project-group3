// Includes necessary libraries for file handling, string manipulation, and data structures
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <regex>
#include <cctype>
#include <iomanip>

// Forward declarations of functions and struct
struct Token;
void readFile(std::vector<std::string>& lines, const std::string& filePath);
std::string removeCommentsAndExcessSpaces(const std::string& line);
std::vector<Token> tokenize(const std::string& code);
void printCode(const std::vector<std::string>& lines);
void printTokens(const std::vector<Token>& tokens);

// Token struct to hold the value and type of each token
struct Token {
    std::string value;
    std::string type;  // e.g., identifier, keyword, literal
};

// Main function: Orchestrates file reading, processing, and output
int main() {
    std::vector<std::string> lines;  // Stores lines read from the file
    std::string filePath = "input.txt";  // File path to read from
    readFile(lines, filePath);  // Reads the file content into lines

    std::vector<Token> allTokens;  // Stores all tokens identified in the file
    // Process each line to remove comments/excess spaces and tokenize
    for (auto& line : lines) {
        line = removeCommentsAndExcessSpaces(line);
        std::vector<Token> tokens = tokenize(line);
        allTokens.insert(allTokens.end(), tokens.begin(), tokens.end());
    }

    printCode(lines);  // Prints processed code
    printTokens(allTokens);  // Prints tokens in tabular form

    return 0;
}

// Reads all lines from a file into a vector
void readFile(std::vector<std::string>& lines, const std::string& filePath) {
    std::ifstream file(filePath);  // Opens the file
    std::string line;
    
    // Reads each line and adds to the vector
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
}

// Removes comments and excess spaces from a line of code
std::string removeCommentsAndExcessSpaces(const std::string& line) {
    std::string result;  // Holds the cleaned line
    std::istringstream stream(line);  // Stream to parse the line word by word
    std::string word;

    // Reads each word, stopping if a comment start is found
    while (stream >> word) {
        if (word.find("//") != std::string::npos) {
            break;  // Ignores the rest of the line if comment is found
        }
        if (!result.empty()) {
            result += " ";  // Adds a space before the next word
        }
        result += word;
    }

    return result;  // Returns the cleaned line
}

// Enhanced tokenize function with refined matching
std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;

    // Adjusted regular expressions
    std::regex keywordRegex("\\b(int|return|if|else|for)\\b");
    std::regex identifierRegex("\\b[a-zA-Z_][a-zA-Z0-9_]*\\b");
    std::regex literalRegex("\\b\\d+\\b"); // Numeric literals
    std::regex stringLiteralRegex("\"[^\"]*\""); // String literals
    std::regex operatorRegex("([&*+=\\-/<>!]{1,2}|[;{}(),\\[\\]]|<<|>>)");
    std::regex preprocessorRegex("#include[ ]*<[^>]+>|#include[ ]*\"[^\"]+\"|using namespace std;");

    // Tokenization logic using updated regex
    std::istringstream stream(code);
    std::string word;
    while (stream >> word) {
        Token token;
        token.value = word;
        // Determine token type
        if (std::regex_match(word, preprocessorRegex)) {
            token.type = "Preprocessor";
        } else if (std::regex_match(word, keywordRegex)) {
            token.type = "Keyword";
        } else if (std::regex_match(word, identifierRegex)) {
            token.type = "Identifier";
        } else if (std::regex_match(word, literalRegex)) {
            token.type = "Literal";
        } else if (std::regex_match(word, stringLiteralRegex)) {
            token.type = "String Literal";
        } else if (std::regex_match(word, operatorRegex)) {
            token.type = "Operator/Delimiter";
        } else {
            token.type = "Unknown";
        }
        tokens.push_back(token);
    }

    return tokens;
}


// Prints the processed code without comments and excess spaces
void printCode(const std::vector<std::string>& lines) {
    for (const auto& line : lines) {
        std::cout << line << std::endl;
    }
}

// Helper function to get the maximum token length in each category
int getMaxTokenLength(const std::vector<std::string>& tokens) {
    int max_length = 0;
    for (const auto& token : tokens) {
        max_length = std::max(max_length, static_cast<int>(token.length()));
    }
    return max_length;
}

// Modified printTokens function to format like a table
void printTokens(const std::vector<Token>& tokens) {
    // Create a map to hold tokens by type
    std::map<std::string, std::vector<std::string>> categorizedTokens;
    for (const auto& token : tokens) {
        categorizedTokens[token.type].push_back(token.value);
    }

    // Calculate padding for each category
    std::map<std::string, int> categoryPadding;
    for (const auto& category : categorizedTokens) {
        categoryPadding[category.first] = getMaxTokenLength(category.second);
    }

    // Print the table header
    std::cout << std::left << std::setw(20) << "Category" << "Tokens\n";
    std::cout << "------------------------------------------------------------\n";

    // Print tokens categorized by type with proper padding
    for (const auto& category : categorizedTokens) {
        std::cout << std::left << std::setw(20) << category.first; // Print the category name
        for (const auto& token : category.second) {
            std::cout << std::left << token << ", ";
        }
        std::cout << std::endl; // End of category line
    }
}

