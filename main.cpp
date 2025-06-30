#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

bool valid_tokens(std::vector<std::string> tokens);

const std::vector<std::string> operators = { "+", "-", "*", "/" };

int main() {
    while (true) {
        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> tokens;
        std::vector<char> buffer;

        for (char token : input) {
            if (isdigit(token)) {
                buffer.push_back(token);
            }
            else if (std::ranges::find(operators, std::string(1, token)) != operators.end()) {
                if (!buffer.empty()) {
                    std::string temp_token(buffer.begin(), buffer.end());
                    tokens.push_back(temp_token);
                    buffer.clear();
                }
                tokens.push_back(std::string(1, token));
            }
        }

        if (!buffer.empty()) {
            std::string temp_token(buffer.begin(), buffer.end());
            tokens.push_back(temp_token);
        }

        if (!valid_tokens(tokens)) {
            throw std::invalid_argument("Invalid input");
        }

        for (auto& token : tokens) {
            std::cout << token << ' ';
        }
    }
    return 0;
}

bool valid_tokens(std::vector<std::string> tokens) {
    //Check if the first symbol is simple operand
    //Check if the last symbol is operand without a parameter
    return true;
}