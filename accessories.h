#define MAXCHARS 1000

int line_start(char string[])
{
    //this finds the position of the first non-space and non-tab character

    if (string[0] != ' ' && string[0] != '\t')
    {
        //not even a single starting whitespace/tab
        return 0;
    }

    int i;
    for (i = 0; i < strlen(string); i++)
    {
        if ((string[i] == ' ' || string[i] == '\t') &&
            (string[i + 1] != ' ' && string[i + 1] != '\t')) 
        {
            return i + 1;
        }
    }
    //impossible to reach here
    return 0;
}

int isnumber(char str[])
{
    int i;
    for (i = 0; i < strlen(str); i++)
    {
        if (isdigit(str[i]) == 0)
        {
            //if a letter is found, 'str' is not a number
            return 0;
        }
    }
    return 1;
}

int check_limits(int *start_line, int *end_line, int lines)
{
    if (*start_line > *end_line)
    {
        return -1; //obvious invalidation
    }

    if (*start_line < 0)
    {
        *start_line = 0;
    }

    if (*end_line > lines - 1)
    {
        *end_line = lines - 1;
    }

    return 0;
}

void shift_left(char string[], int number)
{
    char aux[MAXCHARS];
    int i;

    for (i = 0; i < number; i++)
    {
        if (string[0] != ' ' && string[0] != '\t')
        {
            //no point in shifting more
            break;
        }
        strcpy(aux, string + 1); //copying everything except the first char
        strcpy(string, aux); //saving the new string
    }
}

void shift_right(char string[], int number)
{
    //'number' represents the number of spaces 'string' should be shifted with
    if (strcmp(string, "\n") == 0)
    {
        //no point in doing anything
        return;
    }

    int i;
    char spaces[MAXCHARS]; //to add spaces

    strcpy(spaces, ""); //starting empty

    for (i = 0; i < number; i++)
    {
        strcat(spaces, " "); //adding a space
    }

    strcat(spaces, string);

    strcpy(string, spaces); //saving the shifted string
}

int is_list_type(char string[])
{
    if (strcmp(string, "n") != 0 &&
        strcmp(string, "a") != 0 &&
        strcmp(string, "A") != 0 &&
        strcmp(string, "b") != 0)
    {
        //if the string doesn't match any of the types, it's bad
        return 0;
    }
    return 1;
}

int is_ordering_type(char string[])
{
    if (strcmp(string, "a") != 0 &&
        strcmp(string, "z") != 0)
    {
        //if the string doesn't match any of the types, it's bad
        return 0;
    }
    return 1;
}

void swap_strings(char string1[], char string2[])
{
    char aux[MAXCHARS];

    strcpy(aux, string1);

    strcpy(string1, string2);

    strcpy(string2, aux);
}

void trailing(char string[])
{
    int i;
    for (i = strlen(string); i > 0; i--)
    {
        //search a non-space (or non-tab) character from the end
        if (string[i - 1] != ' ' &&
            string[i - 1] != '\0' &&
            string[i - 1] != '\n' &&
            string[i - 1] != '\t')
        {
            if (i == strlen(string))
            {
                break;
            }
            //add a newline immediately after it
            string[i] = '\n';
            //end the string with the NULL character
            string[i + 1] = '\0';
            break;
        }
    }

    if (i == 0)
    {
        //the line is full of spaces (or tabs)
        //replace the first space/tab with a NEWLINE
        string[0] = '\n';
        //end the string
        string[1] = '\0';
    }
}

void cp_text(char text1[][MAXCHARS], char text2[][MAXCHARS], int lines)
{
    int i;
    for (i = 0; i < lines; i++) {
        strcpy(text1[i], text2[i]);
    }
}