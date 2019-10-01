#include <stdio.h>
#include <stdlib.h>

//=============================================================================

typedef int elem_t;

struct stack_t
{
    int saver_left;

    const char *name;

    int size;
    elem_t *arr;

    int saver_right;
};

//=============================================================================

enum errors {Size_less_0 = 1000,
             Size_more_Max_Size,
             Invalid_Array,
             Try_to_change_array_after_destruct,
             Try_to_change_size_after_destruct,
             Push_out_of_range,
             Pop_Out_Of_Range,
             Saver_left_is_broken,
             Saver_right_is_broken
             };

//-----------------------------------------------------------------------------

// d - dark
enum colors {D_blue = 1, D_green, D_cyan, D_red, D_purple, D_yellow, D_white, Grey,
               Blue,       Green,   Cyan,   Red,   Purple,   Yellow,   White};

//=============================================================================

const int C_size   =  5;
const int C_poison = -8;

const int     C_error_size     = -10;
const elem_t* C_error_null_ptr = (elem_t *)5;

const int C_saver_left  = 0xDED32;
const int C_saver_right = 0xDED64;

//=============================================================================

#define PR_B(elem, color)     \
    {                         \
    /*txSetConsoleAttr (color);*/ \
    printf (#elem);           \
   /* txSetConsoleAttr (7);*/     \
    }

//-----------------------------------------------------------------------------

// CH - change color
#define CH(color) \
//    txSetConsoleAttr (color);

//-----------------------------------------------------------------------------

// CH_S - change color to standard
#define CH_S \
//    txSetConsoleAttr (7);

//-----------------------------------------------------------------------------

// output elem in color in brackets
#define BRACKETS(bracket_1, bracket_2, elem, color) \
    {                                                   \
    printf (#bracket_1);                                \
    /*CH(color) */                                          \
    printf (#elem);                                     \
    /* CH_S */                                               \
    printf (#bracket_2);                                \
    }

//-----------------------------------------------------------------------------

#define DUMP(error) \
    printf ("\n");                                                                        \
                                                                                          \
    /* ERROR IN LINE = [__LINE__] */                                                      \
    PR_B(ERROR IN LINE, White)                                                            \
    printf (" = [");                                                                      \
    CH(Yellow)                                                                            \
    printf ("%d", __LINE__);                                                              \
    CH_S                                                                                  \
    printf ("]\n");                                                                       \
                                                                                          \
    /* stack_t '%s' = [0x%p] */                                                           \
    printf ("stack_t '");                                                                 \
    CH(White)                                                                             \
    printf ("%s", (this_ -> name));                                                       \
    CH_S                                                                                  \
    printf ("' = [");                                                                     \
    CH(Yellow)                                                                            \
    printf ("0x%p",(this_ -> name));                                                      \
    CH_S                                                                                  \
    printf ("]\n");                                                                       \
                                                                                          \
    printf ("{\n\t");                                                                     \
                                                                                          \
    /* ERROR = %d (%s) */                                                                 \
    PR_B(ERROR, Red)                                                                      \
    printf(" = ");                                                                        \
    CH(Yellow)                                                                            \
    printf ("%d ", error);                                                                \
    printf ("(");                                                                         \
    CH(Green)                                                                             \
    printf ("%s", #error);                                                                \
    CH_S                                                                                  \
    printf (")\n");                                                                       \
                                                                                          \
    Dump (this_);

//-----------------------------------------------------------------------------

#define STACK_CONSTRUCTOR(elem)     \
    elem.name = #elem;              \
    Stack_Constructor (&elem);

//=============================================================================

void Stack_Constructor (stack_t* this_);

void Stack_Destruct (stack_t* this_);

//-----------------------------------------------------------------------------

void Stack_Push (stack_t* this_, elem_t elem);

elem_t Stack_Pop (stack_t* this_);

bool Stack_Empty (const stack_t* const this_);

elem_t Stack_Top (const stack_t* const this_);

int Stack_Size (const stack_t* const this_);

bool Stack_Full (const stack_t* const this_);

//-----------------------------------------------------------------------------

bool Assert_Ok (const stack_t* const this_);

void Dump (const stack_t* const this_);

//=============================================================================

int main ()
{

    stack_t stk_1 = {};
    STACK_CONSTRUCTOR(stk_1)

    return 0;
}

//=============================================================================

void Stack_Constructor (stack_t* this_)
{
    (this_ -> size) = 0;

    (this_ -> saver_left)  = C_saver_left;
    (this_ -> saver_right) = C_saver_right;

    (this_ -> arr) = (elem_t *)calloc (C_size + 1, sizeof(elem_t *));

    Assert_Ok;

    for (int i = 0; i < C_size + 1; i++)
    {
        (this_ -> arr) [i] = C_poison;
    }

    Assert_Ok;
}

//=============================================================================

void Stack_Destruct (stack_t* this_)
{
    Assert_Ok;

    for (int i = 0; i <= C_size; i++)
    {
        (this_ -> arr)[i] = C_poison;
    }

    (this_ -> arr)  = (elem_t *)C_error_null_ptr;
    free (this_ -> arr);

    (this_ -> size) = C_error_size;
}

//=============================================================================

void Stack_Push (stack_t* this_, elem_t elem)
{
    Assert_Ok;

    if ((this_ -> size) >= C_size)
    {
        DUMP(Push_out_of_range)
    }

    (this_ -> arr) [(this_ -> size)++] = elem;

    Assert_Ok;
}

//=============================================================================

elem_t Stack_Pop (stack_t* this_)
{
    Assert_Ok;

    (this_ -> arr)[(this_ -> size)] = C_poison;

    if (Stack_Empty (this_))
    {
        DUMP(Pop_Out_Of_Range)
    }

    elem_t res = (this_ -> arr)[--(this_ -> size)];

    Assert_Ok;

    return res;
}

//=============================================================================

bool Stack_Empty (const stack_t* const this_)
{
    Assert_Ok;

    return ((this_ -> size) <= 0);
}

//=============================================================================

elem_t Stack_Top (const stack_t* const this_)
{
    Assert_Ok;

    if (Stack_Empty(this_))
    {
        CH(Green)
        printf ("\nStack is empty! There is no top element!\n");
        printf ("So, we give you ");
        PR_B(POISON, Red)
        printf ("!\n");
        return C_poison;
    }

    return (this_ -> arr)[(this_ -> size) - 1];
}

//=============================================================================

int Stack_Size (const stack_t* const this_)
{
    Assert_Ok;

    return (this_ -> size);
}

//=============================================================================

bool Stack_Full (const stack_t* const this_)
{
    Assert_Ok;

    return ((this_ -> size) == C_size);
}

//=============================================================================

bool Assert_Ok (const stack_t* const this_)
{
    if ((this_ -> size) < 0)
    {
        if ((this_ -> size) == C_error_size)
        {
            DUMP(Try_to_change_size_after_destruct)
        }

        DUMP(Size_less_0)
    }

    if ((this_ -> size) >= C_size + 1)
    {
        DUMP(Size_more_Max_Size)
    }

    if ((this_ -> arr) == nullptr)
    {
        DUMP(Invalid_Array)
    }

    if ((this_ -> arr) == C_error_null_ptr)
    {
        DUMP(Try_to_change_array_after_destruct)
    }

    if ((this_ -> saver_left) != C_saver_left)
    {
        DUMP(Saver_left_is_broken)
    }

    if ((this_ -> saver_right) != C_saver_right)
    {
        DUMP(Saver_right_is_broken)
    }
}

//=============================================================================

void Dump (const stack_t* const this_)
{
    /* check left saver */
    if ((this_ -> saver_left) != 0xDED32)
    {
        /* !!!!!!!!ERROR!!!!!!!! */
        printf ("\n\t");
        PR_B(!!!!!!!!, D_red)
        PR_B(ERROR, Red)
        PR_B(!!!!!!!!, D_red)
        printf ("\n");

        printf ("\tSOMETHING CHANGED LEFT SAVER\n");
        printf ("\tLeft saver = [%d], but accept [%d]\n", (this_ -> saver_left), 0xDED32);
        exit (0);
    }

    if ((this_ -> saver_right) != 0xDED64)
    {
        /* !!!!!!!!ERROR!!!!!!!! */
        printf ("\n\t");
        PR_B(!!!!!!!!, D_red)
        PR_B(ERROR, Red)
        PR_B(!!!!!!!!, D_red)
        printf ("\n");

        printf ("\tSOMETHING CHANGED RIGHT SAVER\n");
        printf ("\tRight saver = [%d], but accept [%d]\n", (this_ -> saver_right), 0xDED64);
        exit (0);
    }

    /* arr[%d] = [0x%p] */
    printf ("\tarr[%d] = [", C_size);
    CH(Yellow)
    printf ("0x%p", (this_ -> arr));
    CH_S
    printf ("]\n");

    /* Pointer is near zero! */
    if ((this_ -> arr) == nullptr || (this_ -> arr) == C_error_null_ptr)
    {
        CH(Red)
        printf ("\tPointer is near zero!");
        CH_S
        exit(0);
    }

    printf ("\t{\n");
    for (int i = 0; i < C_size; i++)
    {
        printf ("\t");
        if (i <= (this_ -> size))
        {
            /* [%d]: %d */
            CH(Purple)
            printf ("*");
            CH_S
            printf ("[%02d]: ", i);
            CH(White)
            std::cout << (this_ -> arr)[i];
//            printf ("%d ", (this_ -> arr)[i]);
            CH_S

            if ((int)(this_ -> arr)[i] == C_poison)
            {
                BRACKETS(|, |, Be careful - poison!, Red)
            }
        }
        else
        {
            if ((int)(this_ -> arr)[i] != C_poison)
            {
                printf ("\t\n-----------------------------------------\n");
                printf ("\tERROR!!!:\n");
                printf ("\tarr[%d] is NOT poison!!!\n", i);
                printf ("\ti = [%d], arr[i] = [", i);
                std::cout << (this_ -> arr)[i];
                printf ("], poison = %d\n", C_poison);
//                printf ("\ti = [%d], arr[i] = [%d], poison = %d\n", i, (this_ -> arr)[i], C_poison);
                printf ("-----------------------------------------");
                exit (0);
            }
            else
            {
                /* [%d]: %d |poison| */
                printf (" [%02d]: ", i);
                CH(White)
                std::cout << (this_ -> arr)[i];
//                printf ("%d ", (this_ -> arr)[i]);
                CH_S
                BRACKETS(|, |, poison, Red)
            }
        }
        printf ("\n");
    }
    printf ("\t}\n");
    printf ("\tSize = %d\n", (this_ -> size));
    printf ("}\n");

    exit (0);
}
