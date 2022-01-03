void split(char string1[], char string2[], int characters)
{
    if (strcmp(string2, "") != 0)
    {
        string1[ strlen(string1) - 1] = ' ';
    }

    if (strcmp(string2, "\n") == 0)
    {
        //we don't want to add more NEWLINEs
        string1[ strlen(string1) - 1] = '\0';
    }

    string1[characters] = '\n';

    char first_characters[MAXCHARS],    //... from string1
        last_characters[MAXCHARS];      //... from string1

    strncpy(first_characters, string1, characters + 1);

    //placing a NULL immediately after the NEWLINE
    first_characters[characters + 1] = '\0';

    strcpy(last_characters, string1 + characters + 1);

    /*at this point, string1 is split into
    first_characters and last_characters*/

    //saving the new string1
    strcpy(string1, first_characters);

    /*moving the last characters of string1 at
    the beginning of string2 (saved into last_characters)*/
    strcat(last_characters, string2 + line_start(string2) );

    //saving the new string2
    strcpy(string2, last_characters);
}

int push_words( int i,                      //current line
                char original[][MAXCHARS],  //original text
                int *lines,                 //text's lines
                int max_line_length )       //word_wrap parameter
{
    int j;
    if (original[i + 1][ line_start(original[i + 1]) ] == '\n')
    {
        for (j = *lines; j > i + 1; j--)
        {
            //"shift" down the following lines
            strcpy(original[j], original[j - 1]);
        }
        (*lines)++;
    }
    j = max_line_length;
    //to search for the last whitespace before 'max_line_length'
    while (original[i][j] != ' ')
    {
        j--;
        if (j == line_start(original[i]) - 1)
        {
            //there is a word bigger than 'max_line_length'
            printf("Cannot wrap!\n");
            return -2;
        }
    }

    if (i + 1 == *lines)
    {
        strcpy(original[i + 1], "");
        /*if the document is about to end, make
        sure the following line is not empty in order
        to use 'split' with success*/
        (*lines)++;
        //keep testing the last line
    }
    /*remove the newline at the end of the line and
    place it on the last whitespace before 'max_line_length'
    (in function split)*/
    split(original[i], original[i + 1], j);

    return 0;
}

void justify_line(char string[], int max)
{
    //we will move through the string using 'curr_char'
    int curr_char = 0;    

    if (strcmp(string, "\n") == 0 || strlen(string) == max)
    {
        //no point in doing anything
        return;
    }

    char aux[MAXCHARS]; //help with adding the space

    while(strlen(string) < max)
    {
        while(string[curr_char] != ' ' || string[curr_char + 1] == ' ')
        {
            curr_char++;
            if (curr_char == strlen(string) - 2)
            {
                //make sure we are still 'in the string'
                curr_char = 0;
            }
        }

        //copying the first curr_char + 1 characters (including the space)
        strncpy(aux, string, curr_char + 1);
        //avoid stack overflow
        aux[curr_char + 1] = '\0';

        //adding a space
        strcat(aux, " ");

        //adding the rest of the string
        strcat(aux, string + curr_char + 1);

        //saving the new string
        strcpy(string, aux);

        curr_char += 2; // jump over the newly created space

        if (curr_char == strlen(string) - 2)
        {
            //again, don't leave the string's frontiers
            curr_char = 0;
        }
    }
}

void order_strings( char original[][MAXCHARS],  //original strings
                    int start_line,             //only those between start-end
                    int end_line,
                    char ordering[] )           //alphabetical or reverse
{
    //basic bubble sort
    int i, j;
    for (i = start_line; i < end_line; i++)
    {
        for (j = start_line; j < end_line - (i - start_line); j++)
        {
            //first order alphabetically
            if (strcmp(original[j], original[j + 1]) > 0)
            {
                swap_strings(original[j], original[j + 1]);
            }
        }
    }

    if (ordering[0] == 'z')
    {
        //reverse order
        for (i = start_line; i <= (end_line + start_line) / 2; i++)
        {
            swap_strings(original[i], original[end_line - (i - start_line)]);
        }
    }
}

void list_line( char string[],              //string to modify
                char list_type[],           
                char special_character[],
                int *index )                //number that will be incremented
{
    char aux[MAXCHARS]; //to form our new string
    strcpy(aux, ""); //start empty

    if (list_type[0] == 'n')
    {
        sprintf(aux, "%d", *index); //add the line index
    }
    if (list_type[0] == 'a' || list_type[0] == 'A')
    {
        sprintf(aux, "%c", list_type[0] - 1 + *index);
        //e.g.: 'a' - 1 + 1 = 'a', 'A' - 1 + 2 = 'B'
    }

    strcat(aux, special_character); //adding the special character/bullet

    strcat(aux, " "); //adding a space

    strcat(aux, string); //adding the rest of the string

    strcpy(string, aux); //saving the new string

    if (string[strlen(string) - 1] != '\n' && string[strlen(string)] == '\0')
    {
        //make sure the item ends in a NEWLINE
        string[strlen(string) - 1] = '\n';
    }

    (*index)++;
}