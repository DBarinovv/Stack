#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

//=============================================================================

typedef int elem_t;

struct stack_t
{
    int saver_left;
    int size;
    elem_t *arr;
    char *name;
    int saver_right;
};

//=============================================================================

enum errors {Size_less_0 = 1000,
             Size_more_Max_Size,
             Invalid_Array,
             Try_To_Change_Array_After_Destruct,
             Try_To_Change_Size_After_Destruct,
             Push_Out_Of_Range,
             Pop_Out_Of_Range,
             Saver_left_is_broken,
             Saver_right_is_broken
             };

//=============================================================================

const int c_size = 5;
const int c_poison = -8;
const int c_error_size = -10;
const elem_t* c_error_null_ptr = (elem_t *)5;

//=============================================================================

#define DUMP(error) \
printf ("\n");                                                                        \
                                                                                      \
printf ("ERROR IN LINE = [%d]", __LINE__);                                            \
                                                                                      \
/* stack_t '%s' = [%p] */                                                             \
printf ("stack_t '%s' = [%p]\n", (this_ -> name), &(this_ -> name));                  \
                                                                                      \
printf ("{\n\t");                                                                     \
                                                                                      \
/* ERROR = %d (%s) */                                                                 \
printf ("ERROR = %d (%s)", error, #error);                                            \
                                                                                      \
Dump (this_);

//-----------------------------------------------------------------------------

#define ASSERT_OK \
if ((this_ -> size) < 0)                        \
{                                               \
    if ((this_ -> size) == c_error_size)        \
    {                                           \
        DUMP(Try_To_Change_Size_After_Destruct) \
    }                                           \
                                                \
    DUMP(Size_less_0)                           \
}                                               \
                                                \
if ((this_ -> size) >= c_size + 1)              \
{                                               \
    DUMP(Size_more_Max_Size)                    \
}                                               \
                                                \
if ((this_ -> arr) == nullptr)                  \
{                                               \
    DUMP(Invalid_Array)                         \
}                                               \
                                                \
if ((this_ -> arr) == c_error_null_ptr)         \
{                                               \
    DUMP(Try_To_Change_Array_After_Destruct)    \
}                                               \
                                                \
if ((this_ -> saver_left) != 0xDED32)           \
{                                               \
    DUMP(Saver_left_is_broken)                  \
}                                               \
                                                \
if ((this_ -> saver_right) != 0xDED64)          \
{                                               \
    DUMP(Saver_right_is_broken)                 \
}

//-----------------------------------------------------------------------------

#define STACK_CONSTRUCTOR(elem) \
stack_t* helper = &elem;        \
helper -> name = #elem;         \
Stack_Constructor (&elem);

//=============================================================================

void Stack_Constructor (stack_t* this_);

void Stack_Push (stack_t* this_, elem_t elem);

elem_t Stack_Pop (stack_t* this_);

bool Stack_Empty (stack_t* this_);

elem_t Stack_Top (stack_t* this_);

int Stack_Size (stack_t* this_);

bool Stack_Full (stack_t* this_);

void Stack_Destruct (stack_t* this_);

void Dump (stack_t* this_);

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

    (this_ -> saver_left)  = 0xDED32;
    (this_ -> saver_right) = 0xDED64;

    (this_ -> arr) = (elem_t *)calloc (c_size + 1, sizeof(elem_t *));

    ASSERT_OK

    for (int i = 0; i < c_size + 1; i++)
    {
        (this_ -> arr)[i] = c_poison;            // � ���� �� int?
    }

    ASSERT_OK
}

//=============================================================================

void Stack_Push (stack_t* this_, elem_t elem)
{
    ASSERT_OK

    if ((this_ -> size) >= c_size)
    {
        DUMP(Push_Out_Of_Range)
    }

    (this_ -> arr)[(this_ -> size)++] = elem;

    ASSERT_OK
}

//=============================================================================

elem_t Stack_Pop (stack_t* this_)
{
    ASSERT_OK

    (this_ -> arr)[(this_ -> size)] = c_poison;

    if (Stack_Empty (this_))
    {
        DUMP(Pop_Out_Of_Range)
    }

    return (this_ -> arr)[--(this_ -> size)];
}

//=============================================================================

bool Stack_Empty (stack_t* this_)
{
    ASSERT_OK

    return ((this_ -> size) <= 0);
}

//=============================================================================

elem_t Stack_Top (stack_t* this_)
{
    ASSERT_OK

    if (Stack_Empty(this_))
    {
        printf ("Stack is empty! There is no top element!\n");
        printf ("So, we give you POISON!\n");
        return c_poison;
    }

    return (this_ -> arr)[(this_ -> size) - 1];
}

//=============================================================================

int Stack_Size (stack_t* this_)
{
    ASSERT_OK

    return (this_ -> size);
}

//=============================================================================

bool Stack_Full (stack_t* this_)
{
    ASSERT_OK

    return ((this_ -> size) == c_size);
}

//=============================================================================

void Stack_Destruct (stack_t* this_)
{
    ASSERT_OK

    for (int i = 0; i <= c_size; i++)
    {
        (this_ -> arr)[i] = c_poison;
    }

    (this_ -> arr)  = (elem_t *)c_error_null_ptr;
    (this_ -> size) = c_error_size;
}

//=============================================================================

void Dump (stack_t* this_)
{
    /* check left saver */
    if ((this_ -> saver_left) != 0xDED32)
    {
        /* !!!!!!!!ERROR!!!!!!!! */
        printf ("!!!!!!!!ERROR!!!!!!!!\n");

        printf ("\tSOMETHING CHANGED LEFT SAVER\n");
        printf ("\tLeft saver = [%d], but accept [%d]\n", (this_ -> saver_left), 0xDED32);
        exit (0);
    }

    if ((this_ -> saver_right) != 0xDED64)
    {
        /* !!!!!!!!ERROR!!!!!!!! */
        printf ("!!!!!!!!ERROR!!!!!!!!\n");

        printf ("\tSOMETHING CHANGED RIGHT SAVER\n");
        printf ("\tRight saver = [%d], but accept [%d]\n", (this_ -> saver_right), 0xDED64);
        exit (0);
    }

    /* arr[%d] = [%p] */
    printf ("\tarr[%d] = [%p]\n", c_size, (this_ -> arr));

    /* Pointer is near zero! */
    if ((this_ -> arr) == nullptr || (this_ -> arr) == c_error_null_ptr)
    {
        printf ("\tPointer is near zero!");
        exit(0);
    }

    printf ("\t{\n");
    for (int i = 0; i < c_size; i++)
    {
        printf ("\t");
        if (i <= (this_ -> size))
        {
            /* [%d]: %d */
            printf ("*[%02d]: ", i);
            //std::cout << (this_ -> arr)[i];
            printf ("%d ", (this_ -> arr)[i]);

            if ((int)(this_ -> arr)[i] == c_poison)
            {
                printf (" Be careful - poison!");
            }
        }
        else
        {
            if ((int)(this_ -> arr)[i] != c_poison)
            {
                printf ("\t\n-----------------------------------------\n");
                printf ("\tERROR!!!:\n");
                printf ("\tarr[%d] is NOT poison!!!\n", i);
//                printf ("\ti = [%d], arr[i] = [", i);
 //               std::cout << (this_ -> arr)[i];
 //               printf ("], poison = %d\n", c_poison);
                printf ("\ti = [%d], arr[i] = [%d], poison = %d\n", i, (this_ -> arr)[i], c_poison);
                printf ("-----------------------------------------");
                exit (0);
            }
            else
            {
                /* [%d]: %d |poison| */
                printf (" [%d]: ", i);
//                std::cout << (this_ -> arr)[i];
                printf ("%d ", (this_ -> arr)[i]);
                printf (" Be careful - poison!");
            }
        }
        printf ("\n");
    }
    printf ("\t}\n");
    printf ("\tSize = %d\n", (this_ -> size));
    printf ("}\n");

    exit (0);
}
