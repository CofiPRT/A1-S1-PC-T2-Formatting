int wrap_text(char operation[], char original[][MAXCHARS], int *lines)
{
    int i, j, max_line_length;

    if (wrap_validation(operation, &max_line_length) == -1)
    {
        return -1;
    }

    for (i = 0; i < (*lines); i++)
    {
        //"grab" words from the next line, if possible
        if (strlen(original[i]) < max_line_length &&
            i + 1 != *lines)
        {
            if (original[i + 1][0] == '\n')
            {
                i++;
                continue;
                /*if the next line is just a NEWLINE
                "jump" over it*/
            }
            if (original[i][0] == '\n')
            {
                continue;
                /*if the previous wrap added another line
                and we landed on a NEWLINE, skip it*/
            }
            //replacing the newline with a whitespace
            original[i][strlen(original[i]) - 1] = ' ';
            //modifying the 'original' accordingly
            strcat(original[i], original[i + 1] + line_start(original[i + 1]));

            for (j = i + 1; j < *lines; j++)
            {
                //"shift" up the following lines
                strcpy(original[j], original[j + 1]);
            }

            i--;
            (*lines)--;
            continue;
            //re-test the current line
        }
        //"push" words into the next line, if possible
        if (strlen(original[i]) > max_line_length + 1)
        {
            if (push_words(i, original, lines, max_line_length) == -2)
            {
                return -2;
            }
        }
    }
    return 0;
}

int center_text(char operation[], char original[][MAXCHARS], int *lines)
{
    int values[2];  //values[0] will be start_line
                    //values[1] will be end_line

    if (validation(operation, lines, values) == -1)
    {
        return -1;
    }

    int start_line = values[0],
        end_line = values[1];

    if (check_limits(&start_line, &end_line, *lines) == -1)
    {
        return -1;
    }

    int i,
        max = 0; //number of characters in the longest line

    for (i = 0; i < *lines; i++)
    {
        if (strlen(original[i]) > max)
        {
            max = strlen(original[i]);
        }
    }

    for (i = start_line; i <= end_line; i++)
    {
        shift_right(original[i], (max + 1 - strlen(original[i])) / 2);
    }

    return 0;
}

int align_left(char operation[], char original[][MAXCHARS], int *lines)
{
    int values[2];  //values[0] will be start_line
                    //values[1] will be end_line
    if (validation(operation, lines, values) == -1)
    {
        return -1;
    }

    int start_line = values[0],
        end_line = values[1];

    if (check_limits(&start_line, &end_line, *lines) == -1)
    {
        return -1;
    }

    int i;
    for (i = start_line; i <= end_line; i++)
    {
        shift_left(original[i], line_start(original[i]) );
    }

    return 0;
}

int align_right(char operation[], char original[][MAXCHARS], int *lines)
{
    int values[2];  //values[0] will be start_line
                    //values[1] will be end_line
    if (validation(operation, lines, values) == -1)
    {
        return -1;
    }

    int start_line = values[0],
        end_line = values[1];

    if (check_limits(&start_line, &end_line, *lines) == -1)
    {
        return -1;
    }

    int i,
        max = 0; //number of characters in the longest line

    for (i = 0; i < *lines; i++)
    {
        if (strlen(original[i]) > max)
        {
            max = strlen(original[i]);
        }
    }

    for (i = start_line; i <= end_line; i++)
    {
        shift_right(original[i], max - strlen(original[i]) );
    }

    return 0;
}

int justify(char operation[], char original[][MAXCHARS], int *lines)
{
    int values[2];  //values[0] will be start_line
                    //values[1] will be end_line
    if (validation(operation, lines, values) == -1)
    {
        return -1;
    }

    int start_line = values[0],
        end_line = values[1];

    if (check_limits(&start_line, &end_line, *lines) == -1)
    {
        return -1;
    }

    int i,
        max = 0; //number of characters in the longest line

    for (i = 0; i < *lines; i++)
    {
        if (strlen(original[i]) > max)
        {
            max = strlen(original[i]);
        }
    }

    for (i = start_line; i <= end_line; i++)
    {
        shift_left(original[i], line_start(original[i]) );
        if (i == (*lines) - 1)
        {
            //end of the document
            break;
        }
        if (strcmp(original[i + 1], "\n") == 0)
        {
            //end of the paragraph
            continue;
        }

        justify_line(original[i], max);
    }

    return 0;
}

int paragraph(char operation[], char original[][MAXCHARS], int *lines)
{
    int values[3];  //values[0] will be indent_length
                    //values[1] will be start_line
                    //values[2] will be end_line
    if (paragraph_validation(operation, lines, values) == -1)
    {
        return -1;
    }

    int indent_length = values[0],
        start_line = values[1],
        end_line = values[2];

    if (check_limits(&start_line, &end_line, *lines) == -1)
    {
        return -1;
    }

    int i;
    for (i = start_line; i <= end_line; i++)
    {
        if (i == 0)
        {
            /*if this is the first line of the document,
            it is also the first line of a paragraph*/
            shift_right(original[i], indent_length - line_start(original[i]));
            continue;
        }

        if (strcmp(original[i - 1], "\n") == 0)
        {
            /*the previous line was a simple NEWLINE, so
            the current one has to start a new paragraph*/
            shift_right(original[i], indent_length - line_start(original[i]));
        }
    }

    return 0;
}

int list(char operation[], char original[][MAXCHARS], int *lines)
{
    char values[4][5];  //values[0] will be list_type
                        //values[1] will be special_character
                        //values[2] will be start_line
                        //values[3] will be end_line
    if (list_validation(operation, lines, values) == -1)
    {
        return -1;
    }

    //strings instead of single characters to ease the concatenation
    char list_type[2], special_character[2];
    strcpy(list_type, values[0]);
    strcpy(special_character, values[1]);

    int start_line = atoi(values[2]),
        end_line = atoi(values[3]);

    if (check_limits(&start_line, &end_line, *lines) == -1)
    {
        return -1;
    }

    int i,
        index = 1; //line index
    for (i = start_line; i <= end_line; i++)
    {
        shift_left(original[i], line_start(original[i]) );
        list_line(original[i], list_type, special_character, &index);
    }

    return 0;
}

int ordered_list(char operation[], char original[][MAXCHARS], int *lines)
{
    char values[5][5];  //values[0] will be list_type
                        //values[1] will be special_character
                        //values[2] will be ordering
                        //values[3] will be start_line
                        //values[4] will be end_line
    if (ordered_list_validation(operation, lines, values) == -1)
    {
        return -1;
    }

    //strings instead of single characters to ease concatenation
    char list_type[2], special_character[2], ordering[2];
    strcpy(list_type, values[0]);
    strcpy(special_character, values[1]);
    strcpy(ordering, values[2]);

    int start_line = atoi(values[3]),
        end_line = atoi(values[4]);

    if (check_limits(&start_line, &end_line, *lines) == -1)
    {
        return -1;
    }

    order_strings(original, start_line, end_line, ordering);

    int i,
        index = 1; //line index
    for (i = start_line; i <= end_line; i++)
    {
        shift_left(original[i], line_start(original[i]) );
        list_line(original[i], list_type, special_character, &index);
    }

    return 0;
}