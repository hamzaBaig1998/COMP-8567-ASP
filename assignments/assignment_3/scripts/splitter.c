#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 1024 // maximum input length
#define MAX_TOKENS 64         // maximum number of tokens

int main()
{
    char input[MAX_INPUT_LENGTH]; // buffer to hold the input string
    char *tokens[MAX_TOKENS];     // array of token strings
    int num_tokens = 0;           // number of tokens
    int i;

    printf("Enter a string: ");
    fgets(input, MAX_INPUT_LENGTH, stdin); // read input from user

    // remove newline character from the end of the input string
    input[strcspn(input, "\n")] = '\0';

    // split input string into tokens using space as a delimiter
    char *token = strtok(input, " ");
    while (token != NULL && num_tokens < MAX_TOKENS)
    {
        tokens[num_tokens++] = token;
        token = strtok(NULL, " ");
    }

    // print the array of tokens
    for (i = 0; i < num_tokens; i++)
    {
        printf("Token %d: %s\n", i + 1, tokens[i]);
    }

    return 0;
}
