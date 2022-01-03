int wrap_validation(char operation[], int *max_line_length)
{
    char *parameter;
    parameter = strtok(operation, " ");
    //this will just return the name of the command

    parameter = strtok(NULL, " ");
    if (parameter == NULL)
    {
        //wrap expects an argument
        return -1;
    }
    if (isnumber(parameter) == 0)
    {
        //max_line_length has to be an integer
        return -1;
    }
    (*max_line_length) = atoi(parameter);

    parameter = strtok(NULL, " ");

    if (parameter != NULL)
    {
        //too many arguments
        return -1;
    }

    return 0;
}

int validation(char operation[], int *lines, int values[])
{
    values[0] = 0; //default values
    values[1] = (*lines) - 1;
    
    char *parameter;
    parameter = strtok(operation, " ");
    /*this will just return the name of the command*/

    int i; //keeps track of the current number of the argument
    for (i = 0; i <= 2; i++)
    {
        parameter = strtok(NULL, " ");
        if (parameter == NULL)
        {
            break; //use default values for the remaining arguments
        }
        if (isnumber(parameter) == 0)
        {
            return -1; //all variables have to be integers
        }
        if (i != 2)
        {
            values[i] = atoi(parameter);
        }
        if (i == 2 && parameter != NULL)
        {
            return -1; //too many arguments
        }
    }

    return 0;
}

int paragraph_validation(char operation[], int *lines, int values[])
{
    values[1] = 0; //default values
    values[2] = (*lines) - 1;

    char *parameter;
    parameter = strtok(operation, " ");
    //this will just return the name of the command

    int i; //keeps track of the current number of the argument
    for (i = 0; i <= 3; i++)
    {
        parameter = strtok(NULL, " ");
        if (parameter == NULL)
        {
            if (i == 0)
            {
                return -1; //at least one argument expected
            }
            break; //use default values for the remaining arguments
        }
        if (isnumber(parameter) == 0)
        {
            return -1; //all variables have to be integers
        }
        if (i != 3)
        {
            values[i] = atoi(parameter);
        }
        if (i == 3 && parameter != NULL)
        {
            return -1; //too many arguments
        }
    }

    return 0;
}

int list_validation(char operation[], int *lines, char values[][5])
{
    strcpy(values[2], "0"); //default values

    char aux_values3[5];
    sprintf(aux_values3, "%d", (*lines) - 1);

    strcpy(values[3], aux_values3);

    char *parameter;
    parameter = strtok(operation, " ");
    //this will just return the name of the command

    int i; //keeps track of the current number of the argument
    for (i = 0; i <= 4; i++)
    {
        parameter = strtok(NULL, " ");
        if (parameter == NULL)
        {
            if (i <= 1)
            {
                return -1; //more arguments expected
            }
            break; //use default values for the remaining arguments
        }
        if (i == 0 && is_list_type(parameter) == 0)
        {
            return -1; //list_type has to be valid
        }
        if ( (i == 2 || i == 3) && isnumber(parameter) == 0)
        {
            return -1; //the last two arguments have to be integers
        }
        if (i != 4)
        {
            strcpy(values[i], parameter);
        }
        if (i == 4 && parameter != NULL)
        {
            return -1; //too many arguments
        }
    }

    return 0;
}

int ordered_list_validation(char operation[], int *lines, char values[][5])
{
    strcpy(values[3], "0"); //default values

    char aux_values4[5];
    sprintf(aux_values4, "%d", (*lines) - 1);

    strcpy(values[4], aux_values4);

    char *parameter;
    parameter = strtok(operation, " ");
    //this will just return the name of the command

    int i; //keeps track of the current number of the argument
    for (i = 0; i <= 5; i++)
    {
        parameter = strtok(NULL, " ");
        if (parameter == NULL)
        {
            if (i <= 2)
            {
                return -1; //more arguments expected
            }
            break; //use default values for the remaining arguments
        }
        if (i == 0 && is_list_type(parameter) == 0)
        {
            return -1; //list_type has to be valid
        }
        if (i == 2 && is_ordering_type(parameter) == 0)
        {
            return -1; //ordering has to be either 'a' or 'z'
        }
        if ( (i == 3 || i == 4) && isnumber(parameter) == 0)
        {
            return -1; //the last two arguments have to be integers
        }
        if (i != 5)
        {
            strcpy(values[i], parameter);
        }
        if (i == 5 && parameter != NULL)
        {
            return -1; //too many arguments
        }
    }

    return 0;
}