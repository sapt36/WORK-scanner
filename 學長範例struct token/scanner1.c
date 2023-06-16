#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct token
{
    int flag;
    int s;  		// Start line number
    int e;            // End line number
    char *token;      // Type of the token
    char word[100];   // Token word
} token_list[10000];

//stores reserved words in C 
char *REWD[] = {"if", "else", "while", "for", "do", "switch", "case", "default", "continue", "int", "long", "float", "double", "char", "break", "static", "extern", "auto", "register", "sizeof", "union", "struct", "short", "enum", "return", "goto", "const", "signed", "unsigned", "typedef", "void"};
char program[10000];
int cur_i = 0; // Current char index
int s = 1;  	//Line number
int index = 0;  // Token list index

// Function to identify an identifier token
void identifier()
{
    char iden[100];
    int i = 0;
    while (isalnum(program[cur_i]) || program[cur_i] == '_')
    {
        iden[i] = program[cur_i];
        i += 1;
        cur_i += 1;
    }
    iden[i] = '\0';

    int is_rewd = 0;
    for (int j = 0; j < 31; j++)
    {
        if (strcmp(iden, REWD[j]) == 0)
        {
            is_rewd = 1;
            break;
        }
    }

    if (is_rewd)
    {
        token_list[index].s = s;
        token_list[index].token = "REWD";
        strcpy(token_list[index].word, iden);
        index += 1;
    }
    else
    {
        token_list[index].s = s;
        token_list[index].token = "IDEN";
        strcpy(token_list[index].word, iden);
        index += 1;
    }
}

// Function to identify a number token
void number()
{
    char number[100];
    int i = 0;
    int is_float = 0;
    int is_exp = 0;
    while (isdigit(program[cur_i]) || program[cur_i] == '.' || program[cur_i] == 'e' || program[cur_i] == 'E' || program[cur_i] == '+' || program[cur_i] == '-')
    {
        if (program[cur_i] == '.')
        {
            is_float = 1;
        }
        if (program[cur_i] == 'e' || program[cur_i] == 'E')
        {
            is_exp = 1;
        }
        number[i] = program[cur_i];
        i += 1;
        cur_i += 1;
    }
    number[i] = '\0';

    if (is_float || is_exp)
    {
        token_list[index].s = s;
        token_list[index].token = "FLOT";
        strcpy(token_list[index].word, number);
        index += 1;
    }
    else
    {
        token_list[index].s = s;
        token_list[index].token = "INTE";
        strcpy(token_list[index].word, number);
        index += 1;
    }
}

char CHAR = '\'';
char STR = '\"';

char *OPER[] = {"+", "-", "*", "/", "=", ",", "%", ">>", "<<", "++", "--", "+=", "-=", "*=",
                "/=", "%=", "!", "&&", "||", "&", "[", "]", "|", "^", ".", "->", ">", "<", "==", ">=", "<=", "!="};
char SPEC[] = {'{', '}', '(', ')', ';', '?', ':'};
char *SC = "//";
char *MC[] = {"/*", "*/"};
char PREP = '#';

// Function to check if a character is a special symbol
int check_spec(char c)
{
    for (size_t i = 0; i < 7; i++)
    {
        if (c == SPEC[i])
        {
            return 1;
        }
    }
    return -1;
}

// Function to identify special symbol tokens
void special_symbols()
{
    char c = program[cur_i];
    if (c == CHAR)
    {
        token_list[index].s = s;
        token_list[index].token = "CHAR";
        token_list[index].word[0] = c;
        index += 1;
        cur_i += 1;
    }
    else if (c == STR)
    {
        token_list[index].s = s;
        token_list[index].token = "STR";
        token_list[index].word[0] = c;
        index += 1;
        cur_i += 1;
    }
    else if (c == PREP)
    {
        token_list[index].s = s;
        token_list[index].token = "PREP";
        int i = 0;
        while (program[cur_i] != '\n')
        {
            token_list[index].word[i] = program[cur_i];
            cur_i += 1;
            i += 1;
        }
        index += 1;
    }
    else
    {
        if (c == '/' && program[cur_i + 1] == '/')
        {
            token_list[index].s = s;
            token_list[index].token = "SC";
            token_list[index].word[0] = c;
            token_list[index].word[1] = program[cur_i + 1];
            index += 1;
            cur_i += 2;
        }
        else if (c == '/' && program[cur_i + 1] == '*')
        {
            token_list[index].s = s;
            token_list[index].token = "MC";
            token_list[index].flag = 1;
            token_list[index].s = s;
            token_list[index].word[0] = c;
            token_list[index].word[1] = program[cur_i + 1];
            cur_i += 2;
            while (program[cur_i] != '/')
            {
                if (program[cur_i] == '\n')
                {
                    s += 1;
                    cur_i += 1;
                    continue;
                }
                cur_i += 1;
            }
            cur_i += 1;
            token_list[index].e = s;
            index += 1;
        }
        else
        {
            if (check_spec(c) == 1)
            {
                for (size_t i = 0; i < 7; i++)
                {
                    if (c == SPEC[i])
                    {
                        token_list[index].s = s;
                        token_list[index].token = "SPEC";
                        token_list[index].word[0] = c;
                        index += 1;
                        cur_i += 1;
                    }
                }
            }
            else
            {
                int flag = 0;
                for (int i = 0; i < 31; i++)
                {
                    if (strlen(OPER[i]) == 2)
                    {
                        if (program[cur_i + 1] == OPER[i][1])
                        {
                            flag = 1;
                        }
                    }
                }
                if (flag == 1)
                {
                    token_list[index].s = s;
                    token_list[index].token = "OPER";
                    token_list[index].word[0] = program[cur_i];
                    token_list[index].word[0] = program[cur_i + 1];
                    index += 1;
                    cur_i += 2;
                }
                else
                {
                    token_list[index].s = s;
                    token_list[index].token = "OPER";
                    token_list[index].word[0] = program[cur_i];
                    index += 1;
                    cur_i += 1;
                }
            }
        }
    }
}

int main()
{
    // Open the input file
    FILE *input_file = fopen("scanner1.c", "r");
    if (!input_file)
    {
        printf("Unable to open input file.");
        return 1;
    }
    else
    {
        printf("Input file opened successfully.\n");
        char c;
        int count = 0;
        do
        {
            // Read the characters from the input file
            c = fgetc(input_file);
            program[count] = c;
            count += 1;
            printf("%c", c);
        } while (c != EOF);

        s = 1;
        index = 0;
        cur_i = 0;

        while (cur_i < count)
        {
            // Handle newline character
            if (program[cur_i] == '\n')
            {
                s += 1;
                cur_i += 1;
                continue;
            }
            c = program[cur_i];
            // Handle identifiers
            if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_')
            {
                identifier();
            }
            // Handle numbers
            else if (isdigit(c) || c == '.' || c == 'e' || c == 'E' || c == '+' || c == '-')
            {
                number();
            }
            // Handle special symbols
            else
            {
                if (isspace(program[cur_i]))
                {
                    cur_i += 1;
                    continue;
                }
                special_symbols();
            }
        }
        // Open the output file
        FILE *output_file = fopen("scannerOut2.txt", "w");
        if (!output_file)
        {
            printf("Unable to open output file.");
            return 1;
        }
        else
        {
            // Print tokens to the console and write them to the output file
            for (size_t i = 0; i < 10000; i++)
            {
                if (token_list[i].s == 0)
                {
                    break;
                }
                printf("%d  %s  %s\n", token_list[i].s, token_list[i].token, token_list[i].word);
                fprintf(output_file, "%d  %s  %s\n", token_list[i].s, token_list[i].token, token_list[i].word);
            }
            fclose(output_file);
            fclose(input_file);
        }
    }
    return 0;
}

