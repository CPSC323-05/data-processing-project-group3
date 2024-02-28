#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

struct Token {
    std::string value;
    std::string type; // For simplicity, type might be identifier, keyword, literal, etc.
};

std::vector<std::string> readFile(const std::string& filePath);
std::string removeCommentsAndExcessSpaces(const std::string& line);
std::vector<Token> tokenize(const std::string& code);
void printCode(const std::vector<std::string>& lines);
void printTokens(const std::vector<Token>& tokens);

int main() {
    std::vector<std::string> lines;
    std::string programmingLanguage;
    return 1;
}

std::vector<std::string> readFile(std::vector<std::string>& lines, const std::string& filePath){
    std::ifstream file(filePath);
    std::string line;
    
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    return lines;
}

std::string removeCommentsAndExcessSpaces(const std::string& line) {
    std::string result = "";
    std::vector<std::string> words;
    bool commentStarted = false;
    
}