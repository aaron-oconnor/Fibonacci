#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UPPER_LIMIT  92         ///< @brief upper limit before uint64_t overflows (the 93rd fibonacci number will overflow)

typedef enum
{
    ERROR_INPUT_EXCESS = 0,
    ERROR_INPUT_CONVERSION,
    ERROR_INPUT_UPPER_LIMIT,
    
} error_enum;

static bool inputValid(char *input, uint64_t *converted);
static void printError(error_enum error);
static uint64_t calculateFibonacci(uint8_t index, bool reset);
static void printOutput(uint64_t fibNumber);
static bool isPrime(uint64_t toCheck);

static void runTests(void);
static uint8_t test_inputValid(void);
static uint8_t test_calculateFibonacci(void);
static uint8_t test_isPrime(void);

int main(int argc, char *argv[])
{
    /** verify the correct number of inputs */
    if (argc != 2)
    {
        printError(ERROR_INPUT_EXCESS);
        return 0;
    }
    
    /** check if tests should be run */
  	if (strcmp(argv[1], "--test") == 0)
	{
        runTests();
        return 0;
    }
    
    /** ensure the input was a valid number */
    uint64_t nFibonacci;
    if (inputValid(argv[1], &nFibonacci))
    {
        /** check upper limit */
        if (nFibonacci > UPPER_LIMIT)
        {
            printError(ERROR_INPUT_UPPER_LIMIT);
            return -1;
        }
        
        /** calculate 'nFibonacci' numbers */
        for (uint8_t i = 0; i < nFibonacci; ++i)
        {
            /** fetch the current fibonacci number .. (i == 0) forces the reset */
            uint64_t fibNumber = calculateFibonacci(i, (i == 0));
                 
            /** print the output */
            printOutput(fibNumber);		
        }
    }
    else
    {
        printError(ERROR_INPUT_CONVERSION);
    }
    
    return 0;
}

/**
 * @brief Returns true if the input is a number greater than zero
 */
static bool inputValid(char *input, uint64_t *converted)
{
    char *invalid;
    long output = strtol(input, &invalid, 10);

    /** entire string was converted */
    if (*invalid == 0)
    {
        /** only accept positive numbers */
   		if (output <= 0)
		{
			return false; 
		}     
        
        *converted = (uint64_t)output;
        return true;
    }
    
    return false;
}

/**
 * @brief Returns the fibonacci number given by index, reset will
 *        reset n-1 and n-2
 */
static uint64_t calculateFibonacci(uint8_t index, bool reset)
{
    /** initialise variables */
	static uint64_t fibNumber = 0;
	static uint64_t first     = 0;
	static uint64_t second    = 1;
    
    /** reset the variables since they are static */
    if (reset)
    {
        uint64_t fibNumber = 0;
        uint64_t first     = 0;
        uint64_t second    = 1;
    }
    
    /** verify input is valid */
    if (index > UPPER_LIMIT)
    {
        fibNumber = 0;
        return fibNumber;
    }
	
	/** 0 or 1 return themselves */
	if (index <= 1)
	{
		fibNumber = index;
	}
	
	/** calculate the number (n-1 + n-2) */
	else
	{
		fibNumber = first + second;
		first  = second;
		second = fibNumber;		
	}	

	return fibNumber;
}

/**
 * @brief Prints the buzz and/or fizz based on the input fibonacci number.
 */
static void printOutput(uint64_t fibNumber)
{
   	/** 
		"Buzz" when F(n) is divisible by 3.
		"Fizz" when F(n) is divisible by 5.
		"FizzBuzz" when F(n) is divisible by 15.
		"BuzzFizz" when F(n) is prime.
		the value F(n) otherwise.
	*/
    
	if (fibNumber == 0)
	{
		printf("0 \r\n");
	}
	else if (fibNumber % 15 == 0)
	{
		printf("FizzBuzz \r\n");
	}
	else if (fibNumber % 3 == 0)
	{
		printf("Buzz \r\n");		
	}
	else if (fibNumber % 5 == 0)
	{
		printf("Fizz \r\n");		
	}
	else if (isPrime(fibNumber))
	{
		printf("BuzzFizz \r\n");		
	}	
	else
	{
		printf("%lu \r\n", fibNumber);		
	} 
}

/**
 * @brief Returns true if the given number is a prime number 
 *        greater than 1
 */
static bool isPrime(uint64_t toCheck)
{
	/** 0 or 1 does not count */
	if (toCheck <= 1)
	{
		return false;
	}
	
	/** any event number cannot be prime */
	if (toCheck % 2 == 0)
	{
		return false;
	}
	
	/** only need to check half the points */
	uint64_t upperLimit = toCheck / 2;
	
	for (uint64_t i = 3; i < upperLimit; i += 2)
	{
		if (toCheck % i == 0)
		{
			return false;
		}
	}
	
	return true;
}

/**
 * @brief Prints an error message to the console
 */
static void printError(error_enum error)
{
    switch (error)
    {
        case ERROR_INPUT_EXCESS:
            printf(" incorrect number of inputs \r\n");
            printf(" enter fib --test to test the functions \r\n");
            printf(" enter fib x to calculate the xth fibonacci number \r\n");
            printf(" for example: fib 10 to calculate the 10th number \r\n");
            break;
            
        case ERROR_INPUT_CONVERSION:
            printf(" the input must be a whole number greater than 0 \r\n");
            printf(" for example: fib 10 to calculate the 10th number \r\n");
            break;
            
        case ERROR_INPUT_UPPER_LIMIT:
            printf(" the largest fibonacci number that can be calculated is %u \r\n", UPPER_LIMIT);
            break;
    }
}

/**
 * @brief Runs through the tests to test each function, a counter showing the number
 *        of failed tests (if any) is displayed at the end
 */
static void runTests(void)
{
    printf(" \r\n");
    printf(" running tests \r\n");
    
    uint8_t iFailed = 0;
    
    iFailed += test_inputValid();
    iFailed += test_calculateFibonacci();
    iFailed += test_isPrime();
    
    if (iFailed == 0)
    {
        printf("\r\n");
        printf(" All tests passed \r\n");
    }
    else
    {
        printf("\r\n");
        printf(" Failed %u test(s) \r\n", iFailed);
    }
}

/**
 * @brief Tests the inputValid function by passing specific numbers
 *        and checking the output
 */
static uint8_t test_inputValid(void)
{
    /** initialise variables */
    char input[5];
    uint64_t output;
    bool     result;
    uint8_t  iFailed = 0;
    
    printf(" \r\n");   
    printf(" testing inputValid .. \r\n");

    /** test 1 : testing input -1, expected to fail */
    printf("    input -1        : ");   
    input[0]    = '-'; 
    input[1]    = '1';   
    input[2]    = '\0';
    result      = inputValid(input, &output);
    printf("%s   ", result == false ? "pass \r\n" : "fail **\r\n");
    if (result)
    {
        ++iFailed;
    }
    
    /** test 2 : testing input 0, expected to fail */
    printf(" input  0        : ");
    input[0]    = '0';   
    input[1]    = '\0';
    result      = inputValid(input, &output);
    printf("%s   ", result == false ? "pass \r\n" : "fail **\r\n");
    if (result)
    {
        ++iFailed;
    }
    
    /** test 3 : testing input 1, expected to pass */
    printf(" input  1        : ");
    input[0]    = '1';   
    input[1]    = '\0';
    result      = inputValid(input, &output);
    if(result)
    {
       printf("%s   ", output == 1 ? "pass \r\n" : "fail **\r\n");  
       if(output != 1)
       {
            ++iFailed;
       }
    }
    else
    {
        printf("fail **\r\n");
        ++iFailed;
    }
    
    /** test 4 : testing input 100, expected to pass */
    printf(" input  100      : ");
    input[0]    = '1';   
    input[1]    = '0';
    input[2]    = '0';   
    input[3]    = '\0';
    result      = inputValid(input, &output);
    if(result)
    {
       printf("%s   ", output == 100 ? "pass \r\n" : "fail **\r\n");  
       if(output != 100)
       {
            ++iFailed;
       }
    }
    else
    {
        printf("fail **\r\n");
        ++iFailed;
    }
    
    /** test 5 : testing input abc, expected to fail */
    printf(" input  abc      : ");
    input[0]    = 'a';   
    input[1]    = 'b';
    input[2]    = 'c';   
    input[3]    = '\0';
    result      = inputValid(input, &output);
    printf("%s   ", result == false ? "pass \r\n" : "fail **\r\n");
    if (result)
    {
        ++iFailed;
    }
    
    return iFailed;
}

/**
 * @brief Tests the calculateFibonacci function by passing specific numbers
 *        and checking the output
 */
static uint8_t test_calculateFibonacci(void)
{
    uint8_t iFailed = 0;
    
    printf(" \r\n");
    printf(" testing calculateFibonacci .. \r\n");
    
    uint64_t f2   = 0;
    uint64_t f10  = 0;
    uint64_t f20  = 0;
    uint64_t f40  = 0;
    uint64_t f60  = 0;
    uint64_t f100 = 1; // initialised to non zero value as 0 is the expected result
   
    for (uint8_t i = 0; i <= 100; ++i)
    {
        /** fetch the current fibonacci number .. (i == 0) forces the reset */
        uint64_t fNumber = calculateFibonacci(i, (i == 0));
        
        if (i == 2)
        {
            f2 = fNumber;
        }
        else if(i == 10)
        {
            f10 = fNumber;
        }
        else if(i == 20)
        {
            f20 = fNumber;
        }
        else if(i == 40)
        {
            f40 = fNumber;
        }
        else if(i == 60)
        {
            f60 = fNumber; 
        } 
        else if(i == 100)
        {
            f100 = fNumber; 
        }            
    }
    
    /** test 1 : 2nd fibonacci number is 1 */  
    if(f2 == 1)
    {
        printf("    fibonacci 1     : pass \r\n");
    }
    else
    {
        printf("    fibonacci 10    : fail ** (returned %lu) \r\n", f2);
        ++iFailed;
    }
    
    /** test 2 : 10th fibonacci number is 55 */  
    if(f10 == 55)
    {
        printf("    fibonacci 10    : pass \r\n");
    }
    else
    {
        printf("    fibonacci 10    : fail ** (returned %lu) \r\n", f10);
        ++iFailed;
    }
      
    /** test 3 : 20th fibonacci number is 6,765 */  
    if(f20 == 6765)
    {
        printf("    fibonacci 20    : pass \r\n");
    }
    else
    {
        printf("    fibonacci 20    : fail ** (returned %lu) \r\n", f20);
        ++iFailed;
    }    
    
    /** test 4 : 40th fibonacci number is 102,334,155 */  
    if(f40 == 102334155)
    {
        printf("    fibonacci 40    : pass \r\n");
    }
    else
    {
        printf("    fibonacci 40    : fail ** (returned %lu) \r\n", f40);
        ++iFailed;
    }   
  
    /** test 5 : 60th fibonacci number is 1,548,008,755,920 */  
    if(f60 == 1548008755920)
    {
        printf("    fibonacci 60    : pass \r\n");
    }
    else
    {
        printf("    fibonacci 60    : fail ** (returned %lu) \r\n", f60);
        ++iFailed;
    }  

    /** test 6 : 100th fibonacci number should fail */  
    if(f100 == 0)
    {
        printf("    fibonacci 100   : pass \r\n");
    }
    else
    {
        printf("    fibonacci 100   : fail ** (returned %lu) \r\n", f100);
        ++iFailed;
    }   
    
    return iFailed;
}

/**
 * @brief Tests the isPrime function by passing specific numbers
 *        and checking the output
 */
static uint8_t test_isPrime(void)
{
    uint8_t iFailed = 0;
    
    printf(" \r\n");
    printf(" testing isPrime .. \r\n");
    
    /** test 1 : 0 should return false */  
    if(isPrime(0))
    {
        printf("    prime number 0   : fail ** (returned true) \r\n");       
        ++iFailed;      
    }
    else
    {
        printf("    prime check 0   : pass \r\n");       
    }
    
    /** test 2 : 3 should return true */  
    if(isPrime(3))
    {
        printf("    prime check 3   : pass \r\n");                   
    }
    else
    {
        printf("    prime check 3   : fail ** (returned false) \r\n");       
        ++iFailed;   
    }   
 
    /** test 2 : 17 should return true */  
    if(isPrime(17))
    {
        printf("    prime check 17  : pass \r\n");                   
    }
    else
    {
        printf("    prime check 17  : fail ** (returned false) \r\n");       
        ++iFailed;   
    }   

    /** test 2 : 40 should return false */  
    if(isPrime(40))
    {
        printf("    prime check 40  : fail ** (returned true) \r\n");       
        ++iFailed;   
    }
    else
    {
        printf("    prime check 40  : pass \r\n");       
    }   

    /** test 2 : 193 should return true */  
    if(isPrime(193))
    {
        printf("    prime check 193 : pass \r\n");                   
    }
    else
    {
        printf("    prime check 193 : fail ** (returned false) \r\n");       
        ++iFailed;   
    }

    return iFailed;    
}
