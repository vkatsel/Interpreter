#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <math.h>
#include <queue>
#include <stack>

bool valid_tokens(std::vector<std::string> tokens);
bool valid_function_usage(std::vector<std::string> tokens, int& i);
float apply_operator(float a, const std::string &operation, float b = 0);
std::vector<std::string> convert_rpn(std::vector<std::string> tokens);
void parse_tokens(std::string& input, std::vector<std::string>& tokens, std::vector<char>& buffer);
void process_tokens(std::vector<std::string> tokens);
void calculate_pn(std::vector<std::string> tokens);

const std::vector<std::string> operators = { "+", "-", "*", "/" };
const std::vector<std::string> functions = { "pow", "abs", "max", "min" };
const std::vector<std::string> allowed = { "(", ")", "," };
const std::map<std::string, int> precedence =
    {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"pow", 3}, {"abs", 3}, {"max", 3}, {"min", 3}, {")", 0}, {"(", 0}};


int main() {
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        input.erase(std::remove_if(input.begin(), input.end(), isspace), input.end());

        std::vector<std::string> tokens;
        std::vector<char> buffer;

        parse_tokens(input, tokens, buffer);
        while (!valid_tokens(tokens)) {
            std::cout << "Your input was incorrect. Please, try again" << std::endl;
            input.clear(); tokens.clear(); buffer.clear();
            std::getline(std::cin, input);
            input.erase(std::remove_if(input.begin(), input.end(), isspace), input.end());
            parse_tokens(input, tokens, buffer);
        }

        process_tokens(tokens);
    }
}

bool is_number(const std::string& token) {
    if (token.empty()) return false;
    int start = (token[0] == '-') ? 1 : 0;
    return start < token.size() && std::all_of(token.begin() + start, token.end(), ::isdigit);
}

void parse_tokens(std::string& input, std::vector<std::string>& tokens, std::vector<char>& buffer) {
    for (int i = 0; i < input.length(); i++) {
        if (isdigit(input[i]) || (input[i] == '-' && (i == 0 || input[i - 1] == ',' || input[i - 1] == '(')))
            buffer.push_back(input[i]);
        else if (std::find(operators.begin(), operators.end(),std::string(1, input[i])) != operators.end()) {
            if (!buffer.empty()) {
                std::string temp_token(buffer.begin(), buffer.end());
                tokens.push_back(temp_token);
                buffer.clear();
            }
            tokens.emplace_back(1, input[i]);
        } else if (std::isalpha(input[i])) {
            for (const auto& func : functions) {
                if (input.substr(i, func.length()) == func) {
                    tokens.emplace_back(func);
                    i += func.length() - 1;
                    break;
                }
            }
        } else if (std::find(allowed.begin(), allowed.end(), std::string(1, input[i])) != allowed.end()) {
            if (!buffer.empty()) {
                std::string temp_token(buffer.begin(), buffer.end());
                tokens.push_back(temp_token);
                buffer.clear();
            }
            tokens.emplace_back(1, input[i]);
        }
    }
    if (!buffer.empty()) {
        std::string temp_token(buffer.begin(), buffer.end());
        tokens.push_back(temp_token);
    }
}

bool valid_tokens(std::vector<std::string> tokens) {
    if (tokens.empty()) return false;

    int last_operand_idx = 0;

     for (int i = 0; i < tokens.size(); i++) {
         if (is_number(tokens[i])) {
             if (last_operand_idx == i - 1) return false;
             last_operand_idx = i;
         } else if (std::find(operators.begin(), operators.end(), tokens[i]) != operators.end()) {
             if (last_operand_idx != i - 1 || i == tokens.size() - 1 || i == 0) return false;
         } else if (std::find(functions.begin(), functions.end(), tokens[i]) != functions.end()) {
             if (!valid_function_usage(tokens, i)) return false;
             last_operand_idx = i - 1;
         } else if (std::find(allowed.begin(), allowed.end(), tokens[i]) != allowed.end()) continue;
         else return false;
     }
return true;
}

bool valid_function_usage(std::vector<std::string> tokens, int& i) {
    int expected_args = tokens[i] == "abs" ? 1 : 2;

    if (i + 1 >= tokens.size() || tokens[i + 1] != "(") return false;
    i += 2;

    int arg_count = 0;
    int depth = 0;
    std::vector<std::string> arg;

    while (i < tokens.size()) {
        if (tokens[i] == "(") {
            depth++;
            arg.push_back(tokens[i]);
            i++;
        } else if (tokens[i] == ")") {
            if (depth > 0) {
                depth--;
                arg.push_back(tokens[i]);
                i++;
            } else {
                if (!valid_tokens(arg)) return false;
                arg.clear();
                arg_count++;
                i++;
                return arg_count == expected_args;
            }
        } else if (tokens[i] == "," && depth == 0) {
            if (!valid_tokens(arg)) return false;
            arg.clear();
            arg_count++;
            if (arg_count >= expected_args) return false;
            i++;
        } else {

            arg.push_back(tokens[i]);
            i++;
        }
    }

    return false;
}

void process_tokens(std::vector<std::string> tokens) {
    std::vector<std::string> rpn_tokens = convert_rpn(tokens);
    calculate_pn(rpn_tokens);
}

void calculate_pn(std::vector<std::string> tokens) {
    std::stack<float> result_stack;

    for (auto& token : tokens) {
        if (is_number(token)) {
            result_stack.push(std::stof(token));
        } else {
            if (token == "abs") {
                float a = result_stack.top();
                result_stack.pop();
                result_stack.push(apply_operator(a, "abs"));
            } else {
                float a = result_stack.top();
                result_stack.pop();
                float b = result_stack.top();
                result_stack.pop();
                result_stack.push(apply_operator(a, token, b));
            }
        }
    }

    std::cout << result_stack.top() << std::endl;
}

float apply_operator(float a, const std::string& operation, float b) {
    if (operation == "+") return a + b;
    if (operation == "-") return b - a;
    if (operation == "*") return a * b;
    if (operation == "/") return b / a;
    if (operation == "max") return a > b ? a : b;
    if (operation == "min") return a < b ? a : b;
    if (operation == "pow") return std::pow(b, a);
    if (operation == "abs") return std::abs(a);
    return 0;
}


std::vector<std::string> convert_rpn(std::vector<std::string> tokens) {
    std::stack<std::string> operator_stack;
    std::queue<std::string> output_queue;

    for (auto& token : tokens) {
        if (is_number(token)) {
            output_queue.push(token);
        }
        else if (token == ",") continue;
        else if (token == "(") operator_stack.push(token);
        else if (token == ")") {
            while (!operator_stack.empty() && operator_stack.top() != "(") {
                output_queue.push(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.pop();
            if (!operator_stack.empty() && std::find(functions.begin(), functions.end(), operator_stack.top()) != functions.end()) {
                output_queue.push(operator_stack.top());
                operator_stack.pop();
            }
        }
        else if (precedence.contains(token)) {
            while (!operator_stack.empty() && precedence.contains(operator_stack.top()) && precedence.at(operator_stack.top()) >= precedence.at(token)) {
                output_queue.push(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.push(token);
        }
    }
    while (!operator_stack.empty()) {
        output_queue.push(operator_stack.top());
        operator_stack.pop();
    }

    std::vector<std::string> output;
    while (!output_queue.empty()) {
        output.push_back(output_queue.front());
        output_queue.pop();
    }
    return output;
}


