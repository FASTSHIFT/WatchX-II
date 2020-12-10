#include "evaluate.h"
#include <string.h>
#include <stdio.h>

#ifdef ARDUINO
#  include "itoa.h"
#  define _ltoa ltoa
#else
#  include <stdlib.h>
#endif

#define  PRECISSION 10

static char result[128];
static bool is_decimal_point;

bool evaluate_get_is_decimal_point()
{
    return is_decimal_point;
}

void evaluate_get_result(char* res, uint32_t len)
{
    if (len > sizeof(result))
    {
        len = sizeof(result);
    }
    memcpy(res, result, len);
}

static void dtoa(double ans);
static void safe_string_copy(char *put, const char *get, int32_t max_len);

/* Define the struct of stack to hold operand. */
typedef struct STACK_OPERAND_STRUCT
{
    double operand[128];
    int top;
} STACK_OPERAND;

/* Define the struct of stack to hold operator. */
typedef struct STACK_OPERATION_STRUCT
{
    char operation[128];
    int top;
} STACK_OPERATION;

/* Pop an operand from operand stack. */
static uint32_t popOperand(STACK_OPERAND *stack, double *operand)
{
    int32_t top;

    top = stack -> top;
    if(top >= 0)
    {
        *operand = stack -> operand[top];
        stack -> top --;
        return 0;
    }
    return 1;
}

/* Push an operand to operand stack. */
static uint32_t pushOperand(STACK_OPERAND *stack, double operand)
{
    stack -> top ++;
    stack -> operand[stack -> top] = operand;
    return 0;
}

/* Pop an operator from operation stack. */
static uint32_t popOperation(STACK_OPERATION *stack, char *operation)
{
    int32_t top;

    top = stack -> top;
    if (top >= 0)
    {
        *operation = stack -> operation[top];
        stack -> top --;
        return 0;
    }
    return 1;
}

/* Push an operator to operation stack. */
static uint32_t pushOperation(STACK_OPERATION *stack, char operation)
{
    stack -> top ++;
    stack -> operation[stack -> top] = operation;
    return 0;
}

static double calculate(double a, char operation, double b)
{
    switch(operation)
    {
    case '+':
        return a + b;

    case '-':
        return a - b;

    case '*':
        return a * b;

    case '/':
        return a / b;
    }
    return 0;
}

/* Judge whether the priority of operation1 is larger or equal to operation2. */
static uint32_t prior_compare(char operation1, char operation2)
{
    if ((operation1 == '*') || (operation1 == '/'))
    {
        return 1;
    }
    else if ((operation2 == '+') || (operation2 == '-'))
    {
        return 1;
    }
    return 0;
}

/* Evaluate the arithmetic expression. */
void evaluate_run_expression(const char *expression, bool  persent_flag)
{
    STACK_OPERAND   stack_operand;
    STACK_OPERATION stack_operation;

    int32_t len = (int32_t) strlen(expression);
    const char *text = expression;
    double   num, ans, a, b;
    char  operation;
    bool   is_decimal;
    int32_t decimal_len;
    char  ch;
    int32_t index = 0;

    /* Initialize stack. */
    stack_operand.top = -1;
    stack_operation.top = -1;

    while(index < len)
    {
        /* Read operand to stack operand. */
        ch = text[index];

        if ((ch >= '0') && (ch <= '9'))
        {
            num = 0;
            decimal_len = 0;
            is_decimal = false;

            while (((ch >= '0') && (ch <= '9')) || ch == '.')
            {
                if (is_decimal)
                {
                    decimal_len ++;
                }

                if (ch == '.')
                {
                    is_decimal = true;
                }
                else
                {
                    num = num * 10 + ch - '0';
                }

                index ++;
                ch = text[index];
            }

            if (is_decimal)
            {
                while (decimal_len --)
                {
                    num /= 10;
                }
            }

            pushOperand(&stack_operand, num);
        }
        else
        {
            /* Push current operation to stack_operation when it's priority higher than the stack top operation. */
            if ((stack_operation.top == -1) || !prior_compare(stack_operation.operation[stack_operation.top], ch))
            {
                pushOperation(&stack_operation, ch);
            }
            else
            {
                /* Pop out two operands and one operation and push the calculate result to stack_operand,
                   then push the current operation to the stack_operation. */
                if (stack_operand.top < 1)
                {
                    safe_string_copy(result, "Error Format", 100);
                    return;
                }

                popOperand(&stack_operand, &b);
                popOperand(&stack_operand, &a);
                popOperation(&stack_operation, &operation);
                ans = calculate(a, operation, b);

                pushOperand(&stack_operand, ans);
                pushOperation(&stack_operation, ch);
            }
            index ++;
        }
    }

    while (stack_operation.top >= 0)
    {
        if (stack_operand.top < 1)
        {
            safe_string_copy(result, "Error Format", 100);
            return;
        }
        popOperand(&stack_operand, &b);
        popOperand(&stack_operand, &a);
        popOperation(&stack_operation, &operation);
        ans = calculate(a, operation, b);

        pushOperand(&stack_operand, ans);
    }

    /* Pop out evaluate result. */
    popOperand(&stack_operand, &ans);

    if (persent_flag)
    {
        ans /= 100;
    }

    /* Round to the 10th decimal. */
    if(ans >= 0)
    {
        ans += 0.00000000005f;
    }
    else
    {
        ans += -0.00000000005f;
    }

    /* Save the calculation result to a char array. */
    dtoa(ans);
}

static double sw_pow(double a, int b)
{
    double result = 1;

    while (b--)
    {
        result *= a;
    }
    return result;
}

/* Convert a double value to an ASCII string. */
static void dtoa(double ans)
{
    int32_t digit_num = 1;
    int32_t integer;
    int32_t decimal[PRECISSION];
    int32_t index = 0;
    int32_t precision = PRECISSION;
    double temp;
    int result_index = 0;
    int exponent = 0;
    bool  exp = false;

    if (ans < 0)
    {
        result[result_index++] = '-';
        ans *= -1;
    }
    if (ans >= 10000000000000000.)
    {
        exp = true;
    }

    if(!exp)
    {
        /* Get integer number. */
        temp = ans;
        while(temp >= 10)
        {
            temp /= 10;
            digit_num ++;
        }

        /* Save the integer part to 'result' array. */
        if(digit_num > 1)
        {
            temp = sw_pow(10, digit_num - 1);
            for (index = 0; index < digit_num; index++)
            {
                integer = (int32_t)(ans / temp);
                result[result_index ++] = integer + '0';
                ans -= integer * temp;
                temp /= 10;
            }
        }
        else
        {
            integer = (int32_t)ans;
            result[result_index ++] = integer + '0';
            ans -= integer;
        }
    }
    else
    {

        while (ans >= 10)
        {
            ans /= 10;
            exponent ++;
        }

        result[result_index ++] = (int32_t)ans + '0';
        ans -= (int32_t)ans;
    }

    /* Save the decimal to 'decimal' array. */
    for (index = 0; index < precision; index ++)
    {
        decimal[index] = (int32_t)(ans * 10);
        ans = ans * 10 - decimal[index];
    }

    for (index = precision - 1; index >= 0; index --)
    {
        if (decimal[index] == 0)
        {
            precision --;
        }
        else
        {
            break;
        }
    }

    if (precision)
    {
        result[result_index ++] = '.';
        is_decimal_point = true;
    }
    else
    {
        is_decimal_point = false;
    }

    /* Save the decimal to 'result' array. */
    for(index = 0; index < precision; index ++)
    {
        result[result_index++] = decimal[index] + '0';
    }

    if (!exp)
    {
        result[result_index] = '\0';
    }
    else
    {
        result[result_index ++] = 'E';
        result[result_index ++] = '+';
        _ltoa(exponent, result + result_index, 3);
    }
}

static void safe_string_copy(char* put, const char* get, int32_t max_len)
{
    while (max_len > 1 && *get)
    {
        *put++ = *get++;
        max_len--;
    }
    *put = 0;
}
