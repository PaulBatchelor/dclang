const char *illegal[] = {"do", "redo", "exitdo",
                         "qdo", "qredo",
                         "for", "next", "exitfor",
                         "skip"};
MYINT num_illegal = sizeof(illegal) / sizeof(illegal[0]); 

/* function to validate and return an error message if we are using control
 * structures outside of a definition */
static MYINT validate(const char *token)
{
    MYINT checkval = 1;
    for (MYINT i=0; i < num_illegal; i++) {
        if(strcmp(token, illegal[i]) == 0) {
            printf("Error: '%s' -- illegal outside of function def.\n",
                   illegal[i]);
            checkval = 0;
            return checkval;
        }
    }
    return checkval;
}

// function to compile (or interpret) each incoming token
static void compile_or_interpret(const char *argument)
{
    char *endPointer = 0;
    double d;
    const struct primitive *pr = primitives;

    if (argument == 0) {
        return;
    } 

    // search dictionary (list, not hash) entry
    while (pr->name != 0) {
        if (strcmp(pr->name, argument) == 0) {
            if (def_mode) {
                // insert function only if it's not 's"' for strings:
                if (strcmp("s\"", argument) != 0) {
                    prog[iptr++].function.without_param = pr->function;
                } else {
                    (*(pr->function)) ();
                }
            } else {
                if (validate(argument)) {
                    (*(pr->function)) ();
                }
            }
            return;
        }
        pr++;
    }

    // search user-defined functions (words)
    for (MYINT x = num_user_functions - 1; x > -1 ; x--) {
        if (strcmp(user_functions[x].name, argument) == 0) {
            if (def_mode) {
                prog[iptr].function.with_param = gotofunc;
                prog[iptr++].param = user_functions[x].func_start;
            } else {
                MYINT cur_iptr = iptr;
                gotofunc(user_functions[x].func_start);
                // run the function
                while (iptr < cur_iptr) {
                    iptr += 1;
                    (*(prog[iptr].function.with_param)) (prog[iptr].param);                    
                }
            }
            return;
        }
    }

    /* primitive not found, user definitions not found.  OK, so next, try to
    convert to a number */
    d = strtod(argument, &endPointer);
    if (endPointer != argument) {
        if (def_mode) {
            prog[iptr].function.with_param = push;
            prog[iptr++].param = d;
        } else {
            push(d);    
        }
        return;
    }
    
    /* nothing found, we've reached an error condition, so report
    the situation and reset the stack */
    data_stack_ptr = 0;
    printf("%s -- syntax error.\n", argument); 
    return;    
}


void import(char *infilestr) {
    FILE *old_ifp = ifp;
    FILE *infile;
    infile = fopen(infilestr, "r");
    fseek(infile, 0, SEEK_END);
    long size = ftell(infile);
    rewind(infile);
    setinput(infile);
    while((ftell(infile) < (size - 1))) {
        /* get next input token */
        char *token;
        token = get_token();
        /* are we dealing with a function definition? */
        if (strcmp(token, "[") == 0) {
            startdeffunc();
            def_mode = 1;
            continue; // goto top of loop
        }
        if (strcmp(token, "]") == 0) {
            enddeffunc();
            def_mode = 0;
            continue; // goto top of loop
        }
        /* 'compile' it, or interpret it on-the-fly */
        compile_or_interpret(token);
    }
    fclose(infile);
    compile_or_interpret(0);
    setinput(old_ifp);
}

static void importfunc() {
    if (data_stack_ptr < 2) {
        printf("stack underflow! ");
        return;
    }
    unsigned long str_len = (unsigned long) pop();
    char *str_start = (char *)((unsigned long) pop());
    char dest[str_len + 1];
    char nullstr[] = "\0";
    memcpy(dest, (char *)(str_start + 1), str_len);
    memcpy(dest + str_len, (char *)nullstr, 1);
    import(dest);
}