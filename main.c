#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#define MAXSIZE 1000

typedef struct
{
    double data[MAXSIZE];
    int top;//last elem index
}Stack;

void push(Stack* stack, double c)
{
    if (stack->top<MAXSIZE) stack->data[++stack->top] = c;
    else printf("error: stack full, cant push %g\n",c);
}

double pop(Stack* stack)
{
    if (stack->top == -1) {
        return 0.0;
    } else {
        return stack->data[stack->top--];
    }
}

bool isOperator(char c)
{
    if (c=='+' || c=='-' || c=='*' || c=='/' || c=='^') return true;
    else return false;
}

int getPrecedence(char c)
{
    if (c=='+' || c=='-') return 1;
    if (c=='*' || c=='/') return 2;
    if (c=='^') return 3;
    return -1;
}

int getAssosiationSide(char c) //left=0;right=1
{
    if(c=='^') return 1;
    else return 0;
}

char getType(char* str)
{
    if(str[0]>='0' && str[0]<='9') return '0';
    else if(str[0]=='+') return '+';
    else if(str[0]=='-') return '-';
    else if(str[0]=='*') return '*';
    else if(str[0]=='/') return '/';
    else if(str[0]=='^') return '^';
    else return 'e';//"e" stands for "error"
}

double power(double base, int exponent)
{
    int i=0;
    double result=1.0;
    for(i=0;i<exponent;i++)
    {
        result*=base;
    }
    return result;
}

int findResult(char* str,char* res)
{
    char* check;
    check=strtok(str,"=");
    check=strtok(NULL,"=");
    if (check==NULL) return 1;
    strncpy(res,check,strlen(check)+1);
    return 0;
}

int infixToRPN(char infix[], char* rpn, int* errorPos)
{
    //printf("%d\n",strlen(infix));
    Stack operators;
    operators.top = -1;
    //checkers
    bool numFlag=false;
    bool operatorFlag=false;
    bool leftBracketFlag=false;
    bool rightBracketFlag=false;
    int leftBracketCounter=0;
    int rightBracketCounter=0;

    int i, j;

    for (i = 0, j = 0; i<strlen(infix); i++) {
        char c = infix[i];
        if (c >= '0' && c <= '9')
        {
            if(rightBracketFlag)
            {
                *errorPos=i;
                return 5;
            }
            numFlag=true;
            operatorFlag=false;
            leftBracketFlag=false;
            rightBracketFlag=false;
            rpn[j++] = c;
        }
        else if(isOperator(c))
        {
            if(i==0 && (c!='+' && c!='-'))//prevents first symb error
            {
                *errorPos=i;
                return 1;
            }
            if(i==strlen(infix)-1)//prevents last symb error
            {
                *errorPos=i;
                return 2;
            }
            if(operatorFlag)//prevents 2++2
            {
                *errorPos=i;
                return 3;
            }
            if(leftBracketFlag && (c!='-' || c!='+'))//prevents ...(^...
            {
                *errorPos=i;
                return 6;
            }
            numFlag=false;
            operatorFlag=true;
            leftBracketFlag=false;
            rightBracketFlag=false;
            rpn[j++] = ' ';
            while(operators.top!=-1 && isOperator(operators.data[operators.top]) && operators.data[operators.top]!='(' \
                    && (getPrecedence(operators.data[operators.top])>getPrecedence(c) || (getPrecedence(operators.data[operators.top])==getPrecedence(c) \
                    && getAssosiationSide(c)==0)))
            {
                rpn[j++]=pop(&operators);
                rpn[j++] = ' ';
            }
            push(&operators,c);
        }
        else if(c=='(')
        {
            if (numFlag)//prevents ...3(...
            {
                *errorPos=i;
                return 4;
            }
            if (rightBracketFlag)
            {
                *errorPos=i;
                return 8;
            }
            numFlag=false;
            operatorFlag=false;
            leftBracketFlag=true;
            rightBracketFlag=false;
            leftBracketCounter+=1;
            push(&operators,c);
        }
        else if(c==')')
        {
            if(operatorFlag)
            {
                *errorPos=i;
                return 7;
            }
            if(leftBracketFlag)
            {
                *errorPos=i;
                return 11;
            }
            numFlag=false;
            operatorFlag=false;
            leftBracketFlag=false;
            rightBracketFlag=true;
            rightBracketCounter+=1;
            while(operators.top!=-1 && operators.data[operators.top]!='(')
            {
                rpn[j++] = ' ';
                rpn[j++]=pop(&operators);
            }
            if (operators.data[operators.top]=='(')
            {
                pop(&operators);
            }
        }
        else
        {
            *errorPos=i;
            return 12;
        }
    }
    while (operators.top != -1)
    {
        rpn[j++] = ' ';
        rpn[j++] = pop(&operators);
    }
    rpn[j] = '\0';

    if(leftBracketCounter<rightBracketCounter)
    {
        *errorPos=0;
        return 9;
    }
    else if(leftBracketCounter>rightBracketCounter)
    {
        *errorPos=strlen(infix);
        return 10;
    }
    return 0;
}

int rpnCalc(char* rpnstr,double* rpnResult)
{
    Stack values;
    values.top=-1;

    char type;
    double op2;

    char* splitStr;
    splitStr=strtok(rpnstr," ");

    while (splitStr != NULL)
    {
        type=getType(splitStr);
        switch(type) {
        case '0':
            push(&values,atof(splitStr));
            break;
        case '+':
            push(&values,pop(&values) + pop(&values));
            break;
        case '*':
            push(&values,pop(&values) * pop(&values));
            break;
        case '-':
            op2 = pop(&values);
            push(&values,pop(&values) - op2);
            break;
        case '/':
            op2 = pop(&values);
            if(op2 == 0.0) return 13;
            push(&values,pop(&values) / op2);
            break;
        case '^':
            op2=pop(&values);
            if (op2==0.0) return 14;
            push(&values,power(pop(&values),op2));
            break;
        default:
            printf("Error! Unknown command: \" %s \"\n", splitStr);
            return 15;
            break;
        }
        splitStr=strtok(NULL," ");
    }
    *rpnResult=pop(&values);
    while(values.top!=-1) pop(&values);
    //printf("res %.2g\n", *rpnResult);
    return 0;
}

void showError(int pos)
{
    int i;
    char spaces[MAXSIZE];
    for(i=0;i<pos && pos<MAXSIZE-1;i++)
    {
        spaces[i]=' ';
    }
    spaces[pos]='^';
    spaces[pos+1]='\0';
    printf("\t%s\n",spaces);
    printf("\t%s\n",spaces);
    printf("\t%s\n",spaces);
}

int main()
{
    int c;
    double time_spent = 0.0;

    bool haveResult=false;

    int errorCode=0;
    //1=first symb is operator (for * / ^);2=more than one operator in a row; 3=number then bracket; 4=bracket then number;
    //5=bracket then operator(only for * / ^); 6=operator then bracket(all); 7=no opening bracket; 8=no closing bracket
    //9=nothing in brackets; 10=zero division; 11=zero in power of zero
    int errorPos;

    char infix[MAXSIZE];
    printf("Enter expression: (Only integer numbers)\n\"+\" \"-\" \"*\" \"/\" \"^\" \"(\" \")\" symbols are allowed. "\
           "Up to 999 symbols. Do not use spaces!\n");
    char format[23]; //because size_t might be 20symbs long + 2 literals + 1 null terminator
    snprintf(format, sizeof(format),"%%%zus",MAXSIZE-1);
    scanf(format,infix);
    fflush(stdin);
    infix[strlen(infix)]='\0';

    char rpn[MAXSIZE];

    char infixResult[MAXSIZE-2];
    double rpnResult;

    errorCode=findResult(infix,infixResult);
    if (errorCode==0) haveResult=true;

    printf("Enter amount of cycles:\n");
    scanf("%d",&cycles);
    fflush(stdin);

    clock_t begin = clock();

    errorCode=infixToRPN(infix,rpn,&errorPos);

    switch(errorCode)
    {
    case 1:
        printf("\nError %d: First symbol is operator!\n\t%s\n",errorCode,infix);
        showError(errorPos);
        return 0;
    case 2:
        printf("\nError %d: Last symbol is operator!\n\t%s\n",errorCode,infix);
        showError(errorPos);
        return 0;
    case 3:
        printf("\nError %d: Multiple operators in a row!\n\t%s\n",errorCode,infix);
        showError(errorPos);
        return 0;
    case 4:
        printf("\nError %d: Number is followed by a bracket!\n\t%s\n",errorCode,infix);
        showError(errorPos);
        return 0;
    case 5:
        printf("\nError %d: Bracket is followed by a number!\n\t%s\n",errorCode,infix);
        showError(errorPos);
        return 0;
    case 6:
        printf("\nError %d: Left bracket is followed by a operator!\n\t%s\n",errorCode,infix);
        showError(errorPos);
        return 0;
    case 7:
        printf("\nError %d: Operator is followed by a right bracket!\n\t%s\n",errorCode,infix);
        showError(errorPos);
        return 0;
    case 8:
        printf("\nError %d: Right bracket followed by a left bracket!\n\t%s\n",errorCode,infix);
        showError(errorPos);
        return 0;
    case 9:
        printf("\nError %d: No opening bracket!\n\t%s\n",errorCode,infix);
        showError(errorPos);
        return 0;
    case 10:
        printf("\nError %d: No closing bracket!\n\t%s\n",errorCode,infix);
        showError(errorPos);
        return 0;
    case 11:
        printf("\nError %d: Nothing in brackets!\n\t%s\n",errorCode,infix);
        showError(errorPos);
        return 0;
    case 12:
        printf("\nError %d: Unknown symbol!\n\t%s\n",errorCode,infix);
        showError(errorPos);
        return 0;
    default:
        break;
    }

    //printf("rpn:\n%s\n",rpn);

    errorCode=rpnCalc(rpn,&rpnResult);
    switch (errorCode)
    {
    case 13:
        printf("Error! Dividing by zero!\n");
        return 0;
    case 14:
        printf("Error! Zero in power of zero!\n");
        return 0;
    case 15:
        return 0;
    default:
        break;
    }

    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nDone in %f seconds\n\n\n",time_spent);

    if(haveResult)
    {
        if(rpnResult==atof(infixResult))
        {
            printf("This expression and result of it is correct.\n");
        }
        else
        {
            printf("This expression is correct, but result not: %g != %g\n",atof(infixResult),rpnResult);
        }
    }
    else
    {
        printf("This expression is correct. Result of this expression is approximately %.2g\n",rpnResult);
    }

    return 0;
}
