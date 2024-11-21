# Mathematical expression correctness checker
Mathematical expression сorrectness checker based on Shunting yard algorithm and Reversed Polish notation. Supports `+`, `-`, `*`, `/` and `^` operations.

The program returns RPN of expression and statement whether the expression was correct and an approximate result of that expression.

If user already put result of the expression (e.g. `2+2=5`) program will check if the result is correct, if it is not, then program will return approxiamte result of this expression.

## Theory
In this program, numeric expressions are defined inductively. A numeric expression is a number or a number-operator-numeric expression, where a number is a digit or a digit-number, and an operator is a symbol defining the operation of addition, subtraction, multiplication, division, or raising to a power.  
The shunting yard algorithm is an algorithm that is converting an infix notation of an expression (2\*3+4) into a postfix notation (2 3\*4+) (to RPN). The main advantage of the conversion is that there are no brackets in the postfix notation, while the order of operations is preserved due to the location of the operand in the new notation.  
This conversion is simplifying the verification of the correctness of the expression.

### Pseudocode
```
While there are elements to read:
  If element is a number:
    put it into output queue
  If element is an operator op1:
    While on top of the stack there is an operator op2 which is left-associative AND
    ((op2 have greater precedence, than op1) OR (precedences are equal AND op1 is left-associative)):
      put op2 to output queue
    push op1 to stack
  If element is left parenthesis:
    push it to stack
  If element is right parenthesis:
    While stack top element is NOT left parenthesis:
      pop stack contents to output queue
    pop left parenthesis from stack
While stack is NOT empty:
  pop stack contents to output queue
```
[Precedence](https://en.wikipedia.org/wiki/Order_of_operations) is an order of operations (e.g. `*` have higher precedene than `+`)  
[Associativity](https://www.geeksforgeeks.org/operator-precedence-and-associativity-in-c/) means which operator occurence applied first

## Time complexity
Time complexity of this algorithm is O(n) (linear), since the iteration of all elements takes place within one full cycle.

## Errors
Each error have it's own code:  
| Code 	| Error description                        	|
|------	|------------------------------------------	|
| 1    	| First symbol is operator                 	|
| 2    	| Last symbol is operator                  	|
| 3    	| Multiple operators in a row              	|
| 4    	| Number is followed by a bracket          	|
| 5    	| Bracket is followed by a number          	|
| 6    	| Left bracket is followed by a operator   	|
| 7    	| Operator is followed by a right bracket  	|
| 8    	| Right bracket followed by a left bracket 	|
| 9    	| No opening bracket                       	|
| 10   	| No closing bracket                       	|
| 11   	| Nothing in brackets                      	|
| 12   	| Unknown symbol                           	|
| 13   	| Dividing by zero                         	|
| 14   	| Zero in power of zero                    	|
| 15   	| Unknown command                          	|

## Program description
The program returns statement whether the expression was correct and an approximate result of that expression.

If user already put result of the expression (e.g. `2+2=5`) program will check if the result is correct, if it is not, then program will return approxiamte result of this expression.

`MAXSIZE` – Maximum size of mathematical expression

Fields in `typedef struct Stack`:
- `double data[MAXSIZE]` – Stack (size based on `MAXSIZE`)
- `int top` – Last (top) stack element index

Program methods:
- `void push(Stack *stack, double c)`  
Pushes Element `c` in stack `stack` on top, while there is free space in `stack`.
- `double pop(Stack *stack)`  
This method returns top stack element while there is elements in `stack`.
- `bool isOperator(char c)`  
Returns `true`, if `c` is an operator, false if not.
- `int getPrecedence(char c)`  
Returns precedence of the operator `c`.
- `int getAssosiationSide(char c)`  
Returns associativity of the operator `c`.
- `char getType(char* str)`  
Returns type of the element `str` passed as a string (number, `+`, `-`, etc.).
- `double power(double base, int exponent)`  
Returns the result of raising `base` to the power `exponent`.
- `int findResult(char* str, char* res)`  
Writes elements into `res` string after `=` symbol, if that symbol exists in mathematical expression; otherwise do nothing.
- `int infixToRPN(char* infix, char* rpn, int* errorPos)`  
Transforms infix expression `infix` to postfix expression `rpn`. In case of an error returns error code and place of error `errorPos`.
- `int rpnCalc(char* rpnstr, double* rpnResult)`  
Returns the result of calculation of the postfix expression `rpnstr` via `rpnResult` variable. In case of an error returns error code and place of error `errorPos`.
- `void showError(int pos)`  
Draws arrow to show error at exact position `pos`.
- `int main()`  
Calls `infixToRPN` and `rpnCalc` functions, handles error if there any.

## Run
- Install [gcc](https://gcc.gnu.org/install/binaries.html) compiler
- Open command prompt in `main.c` folder
- Type in `gcc main.c -o rpn`
- After succesfull compilation run the program: `rpn.exe` or `rpn.out`

## References
[Reverse Polish notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation)  
[Shunting yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm)

