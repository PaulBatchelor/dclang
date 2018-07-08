/* struct for 'for' loops: */
typedef struct {
    MYINT limit;
    MYINT step;
} forloop_info;

forloop_info fl_stack[3];
MYINT fl_ptr;

/* array for q-do loop max amounts: */
MYINT qdo_info[3];
MYINT qdo_ptr;

/* looping */
static void dofunc()
{
    return_stack[return_stack_ptr++] = iptr;
    loop_counter_ptr++;
}

static void exitdofunc()
{
    loop_counter[--loop_counter_ptr] = 0;
    --return_stack_ptr;
}

static void redofunc()
{
    if (pop() != 0) {
        loop_counter[loop_counter_ptr - 1] += 1;
        iptr = return_stack[return_stack_ptr - 1];
    } else {
        loop_counter[--loop_counter_ptr] = 0;
        --return_stack_ptr;
    }
}

/* quicker version of 'do', consumes a single stack value that represents a
   maximum range */
static void qdofunc()
{
    return_stack[return_stack_ptr++] = iptr;
    qdo_info[qdo_ptr++] = (MYINT) pop();
    loop_counter[loop_counter_ptr++] = 0;         
}

static void qredofunc()
{
    if (loop_counter[loop_counter_ptr - 1] < qdo_info[qdo_ptr - 1] - 1) {
        loop_counter[loop_counter_ptr - 1] += 1;
        iptr = return_stack[return_stack_ptr - 1];
    } else {
        loop_counter[--loop_counter_ptr] = 0;
        --return_stack_ptr;
        --qdo_ptr; 
    }    
}

/* these 'for' loops are more flexible, allowing from/to/step parameters. */
static void forfunc()
{
    return_stack[return_stack_ptr++] = iptr;
    fl_stack[fl_ptr].step = (MYINT) pop();
    loop_counter[loop_counter_ptr++] = (MYINT) pop();
    fl_stack[fl_ptr++].limit = (MYINT) pop(); 
}

static void exitforfunc()
{
    fl_ptr = --fl_ptr < 1 ? 1 : fl_ptr;
    loop_counter[--loop_counter_ptr] = fl_stack[fl_ptr - 1].limit;
    --return_stack_ptr;
}

static void nextfunc()
{
    if (fl_stack[fl_ptr - 1].step > 0) {
        if (loop_counter[loop_counter_ptr - 1] < \
                (fl_stack[fl_ptr - 1].limit \
                 - fl_stack[fl_ptr - 1].step)) {
            loop_counter[loop_counter_ptr - 1] += fl_stack[fl_ptr - 1].step;
            iptr = return_stack[return_stack_ptr - 1];
        } else {
            exitforfunc();
        }
    } else {
        if (loop_counter[loop_counter_ptr - 1] > \
                ((fl_stack[fl_ptr - 1].limit) \
                  - fl_stack[fl_ptr - 1].step)) {
            loop_counter[loop_counter_ptr - 1] += fl_stack[fl_ptr - 1].step;
            iptr = return_stack[return_stack_ptr - 1];
        } else {
            exitforfunc();
        }
    }
}

static void ifunc()
{
    push_no_check(loop_counter[loop_counter_ptr - 1]);
}

static void jfunc()
{
    push_no_check(loop_counter[loop_counter_ptr - 2]);  
}

static void kfunc()
{
    push_no_check(loop_counter[loop_counter_ptr - 3]);  
}

/* Crazy as this may seem, this function below is all that is needed to
replace if/then/else structures. */
static void skipfunc()
{
    MYINT skipamt = (MYINT) pop();
    iptr += skipamt;
}
