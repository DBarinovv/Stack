#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <assert.h>

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

#define SIZE 50
#define POISON -8
#define ERROR_NULL_PTR 5
#define ERROR_SIZE -10

//-----------------------------------------------------------------------------

#define DUMP(error) \
printf ("ERROR IN LINE = [%d]\n", __LINE__);                                          \
printf ("stack_t '%s' [%p]\n", (this_ -> name), this_);                               \
printf ("{\n");                                                                       \
printf ("\tError = %d (%s)\n", error, #error);                                        \
                                                                                      \
if ((this_ -> saver_left) != 0xDED32)                                                 \
{                                                                                     \
    printf ("\n\t!!!!!!!!ERROR!!!!!!!!\n");                                           \
    printf ("\tSOMETHING CHANGED LEFT SAVER\n");                                      \
    printf ("\tLeft saver = [%d], but accept [%d]\n", (this_ -> saver_left), 0xDED32);\
    exit (0);                                                                         \
}                                                                                     \
                                                                                      \
if ((this_ -> saver_right) != 0xDED64)                                                \
{                                                                                     \
    printf ("\n\t!!!!!!!!ERROR!!!!!!!!\n");                                           \
    printf ("\tSOMETHING CHANGED RIGHT SAVER\n");                                     \
    printf ("\tRight saver = [%d], but accept [%d]\n", (this_ -> saver_right), 0xDED64);\
    exit (0);                                                                         \
}                                                                                     \
                                                                                      \
printf ("\tarr[SIZE] = [%p]\n", (this_ -> arr));                                      \
                                                                                      \                                                                                    \
if ((this_ -> arr) == nullptr || (this_ -> arr) == ERROR_NULL_PTR)                    \
{                                                                                     \
    printf ("Pointer is near zero!");                                                 \
    exit(0);                                                                          \
}                                                                                     \
                                                                                      \
printf ("\t{\n");                                                                     \
for (int i = 0; i < SIZE; i++)                                                        \
{                                                                                     \
    printf ("\t");                                                                    \
    if (i <= (this_ -> size))                                                         \
    {                                                                                 \
        printf ("*[%d]: %d ", i, (this_ -> arr)[i]);                                  \
                                                                                      \
        if ((this_ -> arr)[i] == POISON)                                              \
        {                                                                             \
            printf (" (Be careful - poison!) ");                                      \
        }                                                                             \
    }                                                                                 \
    else                                                                              \
    {                                                                                 \
        if ((this_ -> arr)[i] != POISON)                                              \
        {                                                                             \
            printf ("\n-----------------------------------------\n");                 \
            printf ("ERROR!!!:\n");                                                   \
            printf ("arr[%d] is NOT poison!!!", i);                                   \
            printf ("i = [%d], arr[i] = [%d], poison = POISON", i, (this_ -> arr)[i]);\
            printf ("-----------------------------------------");                     \
            exit (0);                                                                 \
        }                                                                             \
        else                                                                          \
        {                                                                             \
            printf (" [%d]: %d (poison)", i, (this_ -> arr)[i]);                      \
        }                                                                             \
    }                                                                                 \
    printf ("\n");                                                                    \
}                                                                                     \
printf ("\t}\n");                                                                     \
printf ("\tSize = %d\n", (this_ -> size));                                            \
printf ("}\n");                                                                       \
                                                                                      \
exit (0);



//-----------------------------------------------------------------------------

#define ASSERT_OK \
if ((this_ -> size) < 0)                        \
{                                               \
    if ((this_ -> size) == ERROR_SIZE)          \
    {                                           \
        DUMP(Try_To_Change_Size_After_Destruct) \
    }                                           \
                                                \
    DUMP(Size_less_0)                           \
}                                               \
                                                \
if ((this_ -> size) >= SIZE + 1)                \
{                                               \
    DUMP(Size_more_Max_Size)                    \
}                                               \
                                                \
if ((this_ -> arr) == nullptr)                  \
{                                               \
    DUMP(Invalid_Array)                         \
}                                               \
                                                \
if ((this_ -> arr) == ERROR_NULL_PTR)           \
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
Stack_Constructor (&elem);      \

//=============================================================================

void Stack_Constructor (stack_t* this_);

void Stack_Push (stack_t* this_, elem_t elem);

elem_t Stack_Pop (stack_t* this_);

bool Stack_Empty (stack_t* this_);

elem_t Stack_Top (stack_t* this_);

int Stack_Size (stack_t* this_);

bool Stack_Full (stack_t* this_);

void Stack_Destruct (stack_t* this_);

//=============================================================================

int main ()
{
    stack_t stk_1 = {};
    STACK_CONSTRUCTOR(stk_1)

    Stack_Push (&stk_1, 5);
    Stack_Push (&stk_1, 6);
    Stack_Push (&stk_1, 7);
    Stack_Push (&stk_1, 8);

    for (int i = 0; i < 4; i++)
    {
        printf ("%d\n", Stack_Pop(&stk_1));
    }

    return 0;
}

//=============================================================================

void Stack_Constructor (stack_t* this_)
{
    (this_ -> size) = 0;

    (this_ -> saver_left)  = 0xDED32;
    (this_ -> saver_right) = 0xDED64;

    ASSERT_OK

    memset (this_ -> arr, POISON, SIZE + 1);

//    return ASSERT_OK;
}

//=============================================================================

void Stack_Push (stack_t* this_, elem_t elem)
{
    ASSERT_OK

    if ((this_ -> size) >= SIZE)
    {
        DUMP(Push_Out_Of_Range)
    }

    (this_ -> arr)[(this_ -> size)++] = elem;

//    return ASSERT_OK;
}

//=============================================================================

elem_t Stack_Pop (stack_t* this_)
{
//    if (!Stack_Empty)
//        DUMP(Pop_Out_Of_Range)

    (this_ -> arr)[(this_ -> size)] = POISON;

    return (this_ -> arr)[--(this_ -> size)];
}

//=============================================================================

bool Stack_Empty (stack_t* this_)
{
    return ((this_ -> size) <= 0);
}

//=============================================================================

elem_t Stack_Top (stack_t* this_)
{
    return (this_ -> arr)[(this_ -> size)];
}

//=============================================================================

int Stack_Size (stack_t* this_)
{
    return (this_ -> size);
}

//=============================================================================

bool Stack_Full (stack_t* this_)
{
    return ((this_ -> size) == SIZE);
}

//=============================================================================

void Stack_Destruct (stack_t* this_)
{
    for (int i = 0; i <= SIZE; i++)
    {
        (this_ -> arr)[i] = POISON;
    }

    (this_ -> arr)  = ERROR_NULL_PTR;
    (this_ -> size) = ERROR_SIZE;
}
