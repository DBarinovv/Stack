#include <stdio.h>
#include <stdlib.h>

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
const elem_t C_poison = -8;                                                                                                                                                                                                                      const elem_t C_secret = 179179;
const int C_start_capacity = 2;

const int     C_error_size     = -10;
const elem_t* C_error_null_ptr = (elem_t *)5;

const int C_saver_left     = 0xDED;
const elem_t C_saver_right = 0xDED;

//=============================================================================

#define PR_B(elem, color)     \
{                             \
/*    txSetConsoleAttr (color); */\
    printf (#elem);           \
/*    txSetConsoleAttr (7);  */   \
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
    {                                               \
    printf (#bracket_1);                            \
    /*CH(color)*/                                       \
    printf (#elem);                                 \
    /*CH_S */                                           \
    printf (#bracket_2);                            \
    }

//-----------------------------------------------------------------------------

#define ERROR_IN_FUNC(where, func) \
    {                                         \
    PR_B(ERROR, Red)                          \
    printf (" ");                             \
    PR_B(where #func\nSo we give you, Green)  \
    printf (" ");                             \
    PR_B(POISON!\n\n, Red)                    \
    }

//-----------------------------------------------------------------------------

#define DUMP(error) \
    Dump (this_, __LINE__, error, #error);

//-----------------------------------------------------------------------------

#define STACK_CONSTRUCTOR(elem)          \
{                                        \
    elem = Stack_Constructor ();         \
    (*elem).name = #elem;                \
    (*elem).hash = Sum_Of_Struct (elem); \
}

//=============================================================================

#define RE_HASH(this_) \
{                                             \
    (this_ -> hash) = Sum_Of_Struct (this_);  \
}

//=============================================================================

stack_t * Stack_Constructor ();

bool Stack_Destruct (stack_t* this_);

bool Stack_Realloc  (stack_t* this_);

//-----------------------------------------------------------------------------

bool Stack_Push  (stack_t* this_, elem_t elem);

elem_t Stack_Pop (stack_t* this_);

bool Stack_Empty (const stack_t* const this_);

elem_t Stack_Top (const stack_t* const this_);

int Stack_Size   (const stack_t* const this_);

bool Stack_Full  (const stack_t* const this_);

//-----------------------------------------------------------------------------

long int Sum_Of_Struct     (const stack_t* const this_);

long int Stack_Control_Sum (const stack_t* const this_, const long int sum);

bool Assert_Ok (const stack_t* const this_, const int mode1 = 0);

bool Dump      (const stack_t* const this_, const int line, const int error, const char * error_name);

//=============================================================================

int main ()
{

    return 0;
}

//=============================================================================

stack_t * Stack_Constructor ()
{
    stack_t * res_ptr = nullptr;

    res_ptr = (stack_t *) ((char *)calloc (sizeof (stack_t), sizeof(char)));

    (res_ptr -> hash) = 0;
    (res_ptr -> size) = 0;

    (res_ptr -> capacity)   = C_start_capacity;
    (res_ptr -> saver_left) = C_saver_left;

    for (int i = 0; i <= (res_ptr -> capacity) - 1; i++)
    {
        (res_ptr -> arr) [i] = C_poison;
    }

    (res_ptr -> arr) [(res_ptr -> capacity)] = C_saver_right;
    RE_HASH(res_ptr)

    if (!Assert_Ok (res_ptr))
    {
        PR_B(ERROR AFTER Stack_Constructor\n\n, Red)
        return nullptr;
    }

    return res_ptr;
}

//=============================================================================

bool Stack_Destruct (stack_t* this_)
{
    if (!Assert_Ok (this_))
    {
        PR_B(ERROR BEFORE Stack_Destruct\n\n, Red)
        return false;
    }

    for (int i = 0; i <= (this_ -> capacity); i++)
    {
        (this_ -> arr) [i] = C_poison;
    }

    (this_ -> arr) [0]  = (elem_t)C_error_null_ptr;
    free (this_ -> arr);

    (this_ -> size) = C_error_size;

    return true;
}

//=============================================================================

bool Stack_Realloc (stack_t* this_)
{
    if (!Assert_Ok (this_, 0))
    {
        PR_B(ERROR BEFORE Stack_Realloc\n\n, Red)
        return false;
    }

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

    if (!Assert_Ok (this_))
    {
        PR_B(ERROR AFTER Stack_Realloc\n\n, Red)
        return false;
    }

    return true;
}

//=============================================================================

bool Stack_Push (stack_t* this_, elem_t elem)
{
    if ((this_ -> size) >= (this_ -> capacity))
    {
        if (!Stack_Realloc (this_))
        {
            PR_B(NO MORE MEMORY\n, Red)
            Assert_Ok (this_, 0);
            return false;
        }
    }

    if (!Assert_Ok (this_))
    {
        PR_B(ERROR BEFORE Stack_Push\n\n, Red)
        return false;
    }

    if ((this_ -> size) >= (this_ -> capacity))
    {
        DUMP(Push_out_of_range)
    }

    (this_ -> arr) [(this_ -> size)++] = elem;

    RE_HASH(this_)

    if (!Assert_Ok (this_, 0))
    {
        PR_B(ERROR AFTER Stack_Push\n\n, Red)
        return false;
    }

    return true;
}

//=============================================================================

elem_t Stack_Pop (stack_t* this_)
{
    if (!Assert_Ok (this_))
    {
        ;
        ERROR_IN_FUNC(BEFORE, Stack_Pop)
        return C_poison;
    }

    (this_ -> arr) [(this_ -> size)] = C_poison;

    RE_HASH(this_)

    if (Stack_Empty (this_))
    {
        DUMP(Pop_out_of_range)
    }

    elem_t res = (this_ -> arr) [--(this_ -> size)];

    RE_HASH(this_)

    if (!Assert_Ok (this_))
    {
        ERROR_IN_FUNC(AFTER, Stack_Pop)
        return C_poison;
    }

    return res;
}

//=============================================================================

bool Stack_Empty (const stack_t* const this_)
{
    if (!Assert_Ok (this_))
    {
        PR_B(ERROR IN Stack_Empty\n\n, Red)
    }

    return ((this_ -> size) <= 0);
}

//=============================================================================

elem_t Stack_Top (const stack_t* const this_)
{
    if (!Assert_Ok (this_))
    {
        PR_B(ERROR IN Stack_Top\n\n, Red)
        return C_poison;
    }

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
    if (!Assert_Ok (this_))
    {
        ERROR_IN_FUNC(IN, Stack_Size)
    }

    return (this_ -> size);
}

//=============================================================================

bool Stack_Full (const stack_t* const this_)
{
    if (!Assert_Ok (this_))
    {
        PR_B(ERROR IN Stack_Full\n\n, Red)
    }

    return ((this_ -> size) == (this_ -> capacity));
}

//=============================================================================

long int Sum_Of_Struct (const stack_t* const this_)
{
    char *helper_ptr = (char *) &(this_ -> size);
    long int res = 0;

    for (char i = 0; i <= (sizeof (int) * 2 + sizeof (elem_t) * (this_ -> capacity)); i++)
    {
        res += *(helper_ptr + i);
    }

    res += C_secret;
    res += C_saver_left;

    return res;
}

//=============================================================================

long int Stack_Control_Sum (const stack_t* this_, const long int sum)
{
    if (!Assert_Ok (this_, 1))
    {
        ERROR_IN_FUNC(BEFORE, Stack_Control_Sum)
        return C_poison;
    }

    long int res = 0;

    res = Sum_Of_Struct (this_);

    if (res != sum || res != (this_ -> hash))
    {
        CH(Red)
        printf ("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        printf ("res = [%li]\n", res);
        printf ("sum = [%li]\n", sum);
        printf ("this_ -> hash = [%li]\n", this_ -> hash);
        printf ("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        CH_S
        return C_poison;
    }

    if (!Assert_Ok (this_, 1))
    {
        ERROR_IN_FUNC(AFTER, Stack_Control_Sum)
        return C_poison;
    }

    return res;
}

//=============================================================================

bool Assert_Ok (const stack_t* const this_, const int mode1)
{
    bool res = true;

    if (mode1 == 0)
    {
        if (Stack_Control_Sum (this_, (this_ -> hash)) == C_poison)
        {
            res = DUMP(Control_sum_changed);
        }
    }

    if ((this_ -> size) < 0)
    {
        if ((this_ -> size) == C_error_size)
        {
            res = DUMP(Try_to_change_size_after_destruct);
        }
        else
        {
            res = DUMP(Size_less_0);
        }
    }

    if ((this_ -> arr) == nullptr)
    {
        res = DUMP(Invalid_array);
    }

    if ((this_ -> size) > (this_ -> capacity))
    {
        res = DUMP(Size_more_Max_Size);
    }

    if ((this_ -> arr) == C_error_null_ptr)
    {
        res = DUMP(Try_to_change_array_after_destruct);
    }

    if ((this_ -> saver_left) != C_saver_left)
    {
        res = DUMP(Saver_left_is_broken);
    }

    if ((this_ -> arr) [(this_ -> capacity)] != C_saver_right)
    {
        res = DUMP(Saver_right_is_broken);
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
