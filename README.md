# Expression Evaluator Report

## 1. Program Description

This program implements a **mathematical expression interpreter** that takes input in **standard infix notation**, supports **basic arithmetic operators** (`+`, `-`, `*`, `/`), and evaluates mathematical **functions** like:

- `pow(a, b)` – power
- `abs(a)` – absolute 
- `min(a, b)` – minimum
- `max(a, b)` – maximum

Features:
- Handling nested function calls
- Support of **negative numbers** and **parentheses**
- Validation of the syntax before evaluation _(is not fully supported for parentheses beyond function calls)_

---

## 2. Order of execution and main components

### ➡️ `main`
Reads input and calls primary functions
### ➡️ `parse_tokens`
Parses a raw string input into a list of string tokens. 
### ➡️ `valid_tokens`
Validates the token list
### ➡️ `convert_rpn`
Implements the Shunting Yard Algorithm to convert infix tokens to RPN.
### ➡️ `calculate_pn`
Evaluates the expression in RPN using a stack.
### ➡️ `apply_operator`
Applies a specific operator or function to operand(s).

---

## 3. Test Plan 

### ✅ Basic Arithmetic
| Expression  | Expected Result |
|-------------|-----------------|
| `2 + 3`     | `5`             |
| `2 + 3 * 4` | `14`            |
| `(2 + 3)*4` | `20`            |

### ✅ Negative Numbers
| Expression   | Expected Result |
|--------------|-----------------|
| `-5 + 3`     | `-2`            |
| `(-2 + 1)*3` | `-3`            |

### ✅ Functions
| Expression       | Expected Result |
|------------------|-----------------|
| `abs(-5)`        | `5`             |
| `pow(2, 3)`      | `8`             |
| `min(2, 3)`      | `2`             |
| `max(3, 1)`      | `3`             |

### ✅ Nested Functions
| Expression                     | Expected Result |
|--------------------------------|-----------------|
| `pow(2, abs(-3))`              | `8`             |
| `min(abs(-5), max(2, 3))`      | `3`             |
| `pow(2, pow(2, 1))`            | `4`             |

