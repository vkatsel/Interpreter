#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

bool valid_tokens(std::vector<std::string> tokens);
bool valid_function_usage(std::vector<std::string> args);

const std::vector<std::string> operators = { "+", "-", "*", "/" };
const std::vector<std::string> functions = { "pow", "abs", "max", "min" };
const std::vector<std::string> allowed = { "(", ")", "," };

int main() {
    while (true) {
        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> tokens;
        std::vector<char> buffer;

        for (int i = 0; i < input.length(); i++) {
            if (isdigit(input[i])) buffer.push_back(input[i]);
            else if (std::find(operators.begin(), operators.end(),std::string(1, input[i])) != operators.end()) {
                if (!buffer.empty()) {
                    std::string temp_token(buffer.begin(), buffer.end());
                    tokens.push_back(temp_token);
                    buffer.clear();
                }
                tokens.emplace_back(1, input[i]);
            } else if (std::isalpha(input[i])) {
                std::string temp_token = input.substr(i, 3);

                if (std::find(functions.begin(), functions.end(), temp_token) != functions.end()) {
                    tokens.emplace_back(temp_token);
                    i += 2;
                }
            } else if (std::find(allowed.begin(), allowed.end(), std::string(1, input[i])) != allowed.end()) {
                if (!buffer.empty()) {
                    std::string temp_token(buffer.begin(), buffer.end());
                    tokens.push_back(temp_token);
                    buffer.clear();
                }
            }
        }
        if (!buffer.empty()) {
            std::string temp_token(buffer.begin(), buffer.end());
            tokens.push_back(temp_token);
        }

        for (auto& token : tokens) {
            std::cout << token << ' ';
        }
        std::cout << std::endl;

        // if (!valid_tokens(tokens)) {
        //     std::cout << "Your input was incorrect. Please, try again" << std::endl;
        // }
    }
}

bool valid_tokens(std::vector<std::string> tokens) {
    if (tokens.size() == 1 || tokens.empty()) return false;

    int last_operand_idx = 0;

     for (int i = 0; i < tokens.size(); i++) {
         if (std::ranges::all_of(tokens[i].begin(), tokens[i].end(), ::isdigit)) {
             if (last_operand_idx == i - 1) return false;
             last_operand_idx = i;
         } else if (std::find(operators.begin(), operators.end(), tokens[i]) != operators.end()) {
             if (last_operand_idx != i - 1 || i == tokens.size() - 1) return false;
         } else if (std::find(functions.begin(), functions.end(), tokens[i]) != functions.end()) {

         }
         else return false;
     }
return true;
}