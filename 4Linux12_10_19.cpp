#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include "TXLib.h"

//=============================================================================

typedef int elem_t;

struct stack_t
{
    int saver_left;

    const char *name;

    long int hash;

    int size;
    int capacity;

    elem_t arr[1];
};

//=============================================================================

enum errors {Size_less_0 = 1000,
             Size_more_Max_Size,
             Invalid_array,
             Try_to_change_array_after_destruct,
             Try_to_change_size_after_destruct,
             Push_out_of_range,
             Pop_out_of_range,
             Saver_left_is_broken,
             Saver_right_is_broken,
             Control_sum_changed
             };

//-----------------------------------------------------------------------------

// d - dark
enum colors {D_blue = 1, D_green, D_cyan, D_red, D_purple, D_yellow, D_white, Grey,
               Blue,       Green,   Cyan,   Red,   Purple,   Yellow,   White};

//=============================================================================

//const int C_size   =  5;
const elem_t C_poison = -8;                                                                                                                                                                                                                      const elem_t C_secret = 179;
const int C_start_capacity = 2;

const int     C_error_size     = -10;
const elem_t* C_error_null_ptr = (elem_t *)5;

const int C_saver_left     = 0xDED;
const elem_t C_saver_right = 0xDED;

//=============================================================================

#define PR_B(elem, color)     \
{                             \
    /*txSetConsoleAttr (color); */\
    printf (#elem);           \
    /*txSetConsoleAttr (7);   */  \
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
    printf (#bracket_1);                            \
    /*CH(color) */                                      \
    printf (#elem);                                 \
    /*CH_S  */                                          \
    printf (#bracket_2);                            \
}

//-----------------------------------------------------------------------------

//#define ERROR_IN_FUNC(where, func)            \
//    {                                         \
//    PR_B(ERROR, Red)                          \
//    printf (" ");                             \
//    PR_B(where #func\nSo we give you, Green)  \
//    printf (" ");                             \
//    PR_B(POISON!\n\n, Red)                    \
//    }

//-----------------------------------------------------------------------------

#define DUMP(error, line) \
    Dump (this_, line, error, #error);

//-----------------------------------------------------------------------------

#define STACK_CONSTRUCTOR(elem)                                            \
{                                                                          \
    *elem = (stack_t *) ((char *)calloc (sizeof (stack_t), sizeof(char))); \
    Stack_Constructor (*elem);                                             \
                                                                           \
    (**elem).name = #elem + 1;                                             \
                                                                           \
    RE_HASH(*elem)                                                         \
}

//=============================================================================

#define RE_HASH(this_)                                                                                   \
    (this_ -> hash) = Find_Hash (this_, sizeof (stack_t) + ((this_ -> capacity) - 1) * sizeof (elem_t)); \

//=============================================================================

#define ASSERT_OK(location, func)                \
{                                                \
    if (!Assert_Ok (this_, __LINE__))            \
    {                                            \
        PR_B(ERROR location func\n\n, Red)       \
        return false;                            \
    }                                            \
}

//=============================================================================

bool Stack_Constructor (stack_t* this_);

bool Stack_Destruct    (stack_t* this_);

bool Stack_Realloc     (stack_t* this_);

//-----------------------------------------------------------------------------

bool Stack_Push  (stack_t* this_, elem_t elem);

bool Stack_Pop   (stack_t* this_, elem_t *elem);

bool Stack_Empty (const stack_t* const this_);

elem_t Stack_Top (const stack_t* const this_);

int Stack_Size   (const stack_t* const this_);

bool Stack_Full  (const stack_t* const this_);

//-----------------------------------------------------------------------------

//long int Sum_Of_Struct     (const stack_t* const this_);

long int Find_Hash (const void* data, const size_t lenth);

bool Stack_Control_Sum (stack_t* this_, const long int sum, long int *res);

bool Assert_Ok (const stack_t* const this_, const int line);

bool Dump      (const stack_t* const this_, const int line, const int error, const char * error_name);

//=============================================================================

int Test_1 ()      // Test ASSERT_OK
{
    stack_t* this_ = (stack_t *)2;

    STACK_CONSTRUCTOR(&this_)

    this_ -> size = -4;
    ASSERT_OK(In, MAIN_POG)

    return 0;
}

//-----------------------------------------------------------------------------

int Test_2 ()      // Test Push with Dump
{
    stack_t* stk = (stack_t *)2;

    STACK_CONSTRUCTOR(&stk)

    Stack_Push (stk, 5);

    Dump (stk, 228, 5, "Test_2");

    return 0;
}

//-----------------------------------------------------------------------------

int Test_3 ()      // Test realloc
{
    stack_t* stk = (stack_t *)2;

    STACK_CONSTRUCTOR(&stk)

    Stack_Push (stk, 164);
    Stack_Push (stk, 198);
    Stack_Push (stk, 5);

    Dump (stk, 228, 5, "Test_3");

    return 0;
}

//-----------------------------------------------------------------------------

int Test_4 ()      // Test Pop
{
    stack_t* stk = (stack_t *)2;

    STACK_CONSTRUCTOR(&stk)

    Stack_Push (stk, 1337);
    Stack_Push (stk, 179);

    elem_t res = 0;

    Dump (stk, 228, 5, "Test_4");

    Stack_Pop (stk, &res);

    CH(Purple)
    printf ("\n\nres = [%d] \n\n", res);
    CH_S

    Dump (stk, 228, 5, "Test_4");

    return 0;
}

//-----------------------------------------------------------------------------

int Test_5 ()        // Test    Empty, Top, Size, Full
{
    stack_t* stk = (stack_t *)2;

    STACK_CONSTRUCTOR(&stk)

    Dump (stk, 228, 5, "Test_5");

    if (Stack_Empty (stk))
    {
        PR_B(Stack is empty!, Grey);
    }

    int Size = Stack_Size (stk);
    CH(Grey)
    printf ("\nSize = [%d]\n", Size);
    CH_S

    if (!Stack_Full (stk))
    {
        PR_B(Stack is not full!\n, Grey);
    }

    Stack_Push (stk, 179); // Push 1

    Dump (stk, 228, 5, "Test_5");

    if (!Stack_Empty (stk))
    {
        PR_B(Stack is not empty!\n, Grey);
    }

    elem_t Top = Stack_Top(stk);
    Size = Stack_Size (stk);

    CH(Grey)
    printf ("\nTop  = [%d]", Top);
    printf ("\nSize = [%d]\n", Size);
    CH_S

    if (!Stack_Full (stk))
    {
        PR_B(Stack is not full!\n, Grey);
    }

    Stack_Push (stk, 5);  // Push 2

    Dump (stk, 228, 5, "Test_5");

    if (Stack_Full (stk))
    {
        PR_B(Stack is full!, Grey);
    }

    return 0;
}

//-----------------------------------------------------------------------------

int Test_6 ()          // Pop from empty
{
    stack_t* stk = (stack_t *)2;

    STACK_CONSTRUCTOR(&stk)

    elem_t res = 0;
    Stack_Pop (stk, &res);

    return 0;
}

//-----------------------------------------------------------------------------

int Test_7 ()            // Changing right saver
{
    stack_t* stk = (stack_t *)2;

    STACK_CONSTRUCTOR(&stk)

    (stk -> arr)[stk -> capacity] = 0xBABA;

    Stack_Push (stk, 179);

    return 0;
}

//=============================================================================

bool Stack_Constructor (stack_t* this_)
{
    (this_ -> hash) = 0;
    (this_ -> size) = 0;

    (this_ -> capacity)   = C_start_capacity;
    (this_ -> saver_left) = C_saver_left;

    for (int i = 0; i <= (this_ -> capacity) - 1; i++)
    {
        (this_ -> arr) [i] = C_poison;
    }

    (this_ -> arr) [(this_ -> capacity)] = C_saver_right;

    RE_HASH(this_)

    ASSERT_OK(AFTER, Stack_Constructor)

    return true;
}
//=============================================================================

bool Stack_Destruct (stack_t* this_)
{
    ASSERT_OK(BEFORE, Stack_Destruct)

    for (int i = 0; i <= (this_ -> capacity); i++)
    {
        (this_ -> arr) [i] = C_poison;
    }

    (this_ -> arr) [0] = (elem_t)C_error_null_ptr;
    free (this_ -> arr);

    (this_ -> size) = C_error_size;

    return true;
}

//=============================================================================

bool Stack_Realloc (stack_t* this_)
{
    ASSERT_OK(BEFORE, Stack_Realloc)

    this_ = (stack_t *) realloc (this_, sizeof ((this_ -> capacity) * 2) * sizeof (elem_t));

    if (this_ == nullptr)
    {
        return false;
    }

    (this_ -> capacity) = (this_ -> capacity) * 2;

    for (int i = (this_ -> size); i < (this_ -> capacity); i++)
    {
        (this_ -> arr) [i] = C_poison;
    }

    (this_ -> arr) [(this_ -> capacity)] = C_saver_right;

    RE_HASH(this_)

    ASSERT_OK(AFTER, Stack_Realloc)

    return true;
}

//=============================================================================

bool Stack_Push (stack_t* this_, elem_t elem)
{
    ASSERT_OK(BEFORE, Stack_Push)

    if ((this_ -> size) >= (this_ -> capacity))
    {
        if (!Stack_Realloc (this_))
        {
      //      PR_B(NO MORE MEMORY\n, Red)
      //      Assert_Ok (this_);
            return false;
        }
    }

    (this_ -> arr) [(this_ -> size)++] = elem;

    RE_HASH(this_)

    ASSERT_OK(AFTER, Stack_Push)

    return true;
}

//=============================================================================

bool Stack_Pop (stack_t* this_, elem_t *elem)
{
    ASSERT_OK(BEFORE, Stack_Pop)

    if (Stack_Empty (this_))
    {
        DUMP(Pop_out_of_range, __LINE__)
    }

    *elem = (this_ -> arr) [--(this_ -> size)];
    (this_ -> arr) [(this_ -> size)] = C_poison;

    RE_HASH(this_)

    ASSERT_OK(AFTER, Stack_Pop)

    return true;
}

//=============================================================================

bool Stack_Empty (const stack_t* const this_)
{
    ASSERT_OK(In, Stack_Empty)

    return ((this_ -> size) <= 0);
}

//=============================================================================

elem_t Stack_Top (const stack_t* const this_)
{
    ASSERT_OK(In, Stack_Top)

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
    ASSERT_OK(In, Stack_Size)

    return (this_ -> size);
}

//=============================================================================

bool Stack_Full (const stack_t* const this_)
{
    ASSERT_OK(In, Stack_Full)

    return ((this_ -> size) == (this_ -> capacity));
}

//=============================================================================

long int Find_Hash (const void* data, const size_t lenth)
{
	assert (data);

	long int res = 0;

    for (int i = 0; i < lenth; i++)
    {
        assert ((char *) data + i);

//        res += ((((long int) (((char *) data)[i])) * 2) - C_secret);
    }

    return res;
}

//=============================================================================

//long int Sum_Of_Struct (const stack_t* const this_)
//{
//    char *helper_ptr = (char *) &(this_ -> size);
//    long int res = 0;
//
//    for (char i = 0; i <= (sizeof (int) * 2 + sizeof (elem_t) * (this_ -> capacity)); i++)
//    {
//        res += *(helper_ptr + i);
//    }
//
//    res += C_secret;
//    res += C_saver_left;
//
//    return res;
//}

//=============================================================================

bool Stack_Control_Sum (stack_t* this_, const long int sum, long int *res)
{
    assert (this_);
    assert (res);

    long int prev_hash = (this_ -> hash);

    (this_ -> hash) = 0;

    *res = Find_Hash (this_, sizeof (stack_t) + ((this_ -> capacity) - 1) * sizeof (elem_t));  // -1, так как в структуре array[1]

    if (*res != sum || *res != prev_hash)
    {
        CH(Red)
        printf ("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        printf ("res = [%li]\n", *res);
        printf ("sum = [%li]\n", sum);
        printf ("this_ -> hash = [%li]\n", this_ -> hash);
        printf ("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        CH_S
        return false;
    }

    (this_ -> hash) = *res;

    return true;
}

//=============================================================================

bool Assert_Ok (const stack_t* const this_, const int line)
{
    bool res = true;
    long int helper_for_control = 0;

    if (!Stack_Control_Sum ((stack_t *) this_, (this_ -> hash), &helper_for_control))
    {
        res = DUMP(Control_sum_changed, line);
    }

    if ((this_ -> size) < 0)
    {
        if ((this_ -> size) == C_error_size)
        {
            res = DUMP(Try_to_change_size_after_destruct, line);
        }
        else
        {
            res = DUMP(Size_less_0, line);
        }
    }

    if ((this_ -> arr) == nullptr)
    {
        res = DUMP(Invalid_array, line);
    }

    if ((this_ -> size) > (this_ -> capacity))
    {
        res = DUMP(Size_more_Max_Size, line);
    }

    if ((this_ -> arr) == C_error_null_ptr)
    {
        res = DUMP(Try_to_change_array_after_destruct, line);
    }

    if ((this_ -> saver_left) != C_saver_left)
    {
        res = DUMP(Saver_left_is_broken, line);
    }

    if ((this_ -> arr) [(this_ -> capacity)] != C_saver_right)
    {
        res = DUMP(Saver_right_is_broken, line);
    }

    return res;
}

//=============================================================================

bool Dump (const stack_t* const this_, const int line, const int error, const char * error_name)
{
    printf ("\n");

    /* ERROR IN LINE = [__LINE__] */
    PR_B(ERROR IN LINE, White)
    printf (" = [");
    CH(Yellow)
    printf ("%d", line);
    CH_S
    printf ("]\n");

    /* stack_t '%s' = [0x%p] */
    printf ("stack_t '");
    CH(White)
    printf ("%s", (this_ -> name));
    CH_S
    printf ("' = [");
    CH(Yellow)
    printf ("0x%p", this_);
    CH_S
    printf ("]\n");

    printf ("{\n\t");

    /* ERROR = %d (%s) */
    PR_B(ERROR, Red)
    printf(" = ");
    CH(Yellow)
    printf ("%d ", error);
    printf ("(");
    CH(Green)
    printf ("%s", error_name);
    CH_S
    printf (")\n");

    /* check left saver */
    if ((this_ -> saver_left) != C_saver_left)
    {
        /* !!!!!!!!ERROR!!!!!!!! */
        printf ("\n\t");
        PR_B(!!!!!!!!, D_red)
        PR_B(ERROR, Red)
        PR_B(!!!!!!!!, D_red)
        printf ("\n");

        printf ("\tSOMETHING CHANGED LEFT SAVER\n");
        printf ("\tLeft saver = [%d], but accept [%d]\n", (this_ -> saver_left), C_saver_left);

        return false;
//        exit (0);
    }

    if ((this_ -> arr) [(this_ -> capacity)] != C_saver_right)
    {
        /* !!!!!!!!ERROR!!!!!!!! */
        printf ("\n\t");

        PR_B(!!!!!!!!, D_red)
        PR_B(ERROR, Red)
        PR_B(!!!!!!!!, D_red)
        printf ("\n");

        printf ("\tSOMETHING CHANGED RIGHT SAVER\n");
        printf ("\tRight saver = [%d], but accept [%d]\n", (this_ -> arr) [(this_ -> capacity)], C_saver_right);

        return false;
//        exit (0);
    }

    printf ("\tSize = %d\n", (this_ -> size));
    /* arr[%d] = [0x%p] */
    printf ("\tarr[%d] = [", (this_ -> capacity) - 1);
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

        return false;
//        exit(0);
    }

    printf ("\t{\n");
    for (int i = 0; i < (this_ -> capacity); i++)
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
            std::cout << (this_ -> arr) [i];
//            printf ("%d ", (this_ -> arr)[i]);

            CH_S

            if ((int)(this_ -> arr) [i] == C_poison)
            {
                BRACKETS(|, |, Be careful - poison!, Red)
            }
        }
        else
        {
            if ((int)(this_ -> arr) [i] != C_poison)
            {
                printf ("\t\n-----------------------------------------\n");
                printf ("\tERROR!!!:\n");
                printf ("\tarr[%d] is NOT poison!!!\n", i);
                printf ("\ti = [%d], arr[i] = [", i);
                std::cout << (this_ -> arr) [i];
                printf ("], poison = %d\n", C_poison);
//                printf ("\ti = [%d], arr[i] = [%d], poison = %d\n", i, (this_ -> arr)[i], C_poison);
                printf ("-----------------------------------------\n");

                return false;
//                exit (0);
            }
            else
            {
                /* [%d]: %d |poison| */
                printf (" [%02d]: ", i);
                CH(White)
                std::cout << (this_ -> arr) [i];
//                printf ("%d ", (this_ -> arr)[i]);
                CH_S
                BRACKETS(|, |, poison, Red)
            }
        }
        printf ("\n");
    }
    printf ("\t}\n");
    printf ("}\n");

    return false;
//    exit (0);
}
