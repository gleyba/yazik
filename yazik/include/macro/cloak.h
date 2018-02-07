#pragma once

#define __YAZIK_CAT(a, ...) __YAZIK_PRIMITIVE_CAT(a, __VA_ARGS__)
#define __YAZIK_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define __YAZIK_COMPL(b) __YAZIK_PRIMITIVE_CAT(__YAZIK_COMPL_, b)
#define __YAZIK_COMPL_0 1
#define __YAZIK_COMPL_1 0

#define __YAZIK_BITAND(x) __YAZIK_PRIMITIVE_CAT(__YAZIK_BITAND_, x)
#define __YAZIK_BITAND_0(y) 0
#define __YAZIK_BITAND_1(y) y

#define __YAZIK_INC(x) __YAZIK_PRIMITIVE_CAT(__YAZIK_INC_, x)
#define __YAZIK_INC_0 1
#define __YAZIK_INC_1 2
#define __YAZIK_INC_2 3
#define __YAZIK_INC_3 4
#define __YAZIK_INC_4 5
#define __YAZIK_INC_5 6
#define __YAZIK_INC_6 7
#define __YAZIK_INC_7 8
#define __YAZIK_INC_8 9
#define __YAZIK_INC_9 9

#define __YAZIK_DEC(x) __YAZIK_PRIMITIVE_CAT(__YAZIK_DEC_, x)
#define __YAZIK_DEC_0 0
#define __YAZIK_DEC_1 0
#define __YAZIK_DEC_2 1
#define __YAZIK_DEC_3 2
#define __YAZIK_DEC_4 3
#define __YAZIK_DEC_5 4
#define __YAZIK_DEC_6 5
#define __YAZIK_DEC_7 6
#define __YAZIK_DEC_8 7
#define __YAZIK_DEC_9 8

#define __YAZIK_CHECK_N(x, n, ...) n
#define __YAZIK_CHECK(...) __YAZIK_CHECK_N(__VA_ARGS__, 0,)
#define __YAZIK_PROBE(x) x, 1,

#define __YAZIK_IS_PAREN(x) __YAZIK_CHECK(__YAZIK_IS_PAREN_PROBE x)
#define __YAZIK_IS_PAREN_PROBE(...) __YAZIK_PROBE(~)

#define __YAZIK_NOT(x) __YAZIK_CHECK(__YAZIK_PRIMITIVE_CAT(__YAZIK_NOT_, x))
#define __YAZIK_NOT_0 __YAZIK_PROBE(~)

#define __YAZIK_BOOL(x) __YAZIK_COMPL(__YAZIK_NOT(x))

#define __YAZIK_IIF(c) __YAZIK_PRIMITIVE_CAT(__YAZIK_IIF_, c)
#define __YAZIK_IIF_0(t, ...) __VA_ARGS__
#define __YAZIK_IIF_1(t, ...) t

#define __YAZIK_IF(c) __YAZIK_IIF(__YAZIK_BOOL(c))

#define __YAZIK_EAT(...)
#define __YAZIK_EXPAND(...) __VA_ARGS__
#define __YAZIK_WHEN(c) __YAZIK_IF(c)(__YAZIK_EXPAND, __YAZIK_EAT)

#define __YAZIK_EMPTY()
#define __YAZIK_DEFER(id) id __YAZIK_EMPTY()
#define __YAZIK_OBSTRUCT(id) id __YAZIK_DEFER(__YAZIK_EMPTY)()

#define __YAZIK_EVAL(...)  __YAZIK_EVAL1(__YAZIK_EVAL1(__YAZIK_EVAL1(__VA_ARGS__)))
#define __YAZIK_EVAL1(...) __YAZIK_EVAL2(__YAZIK_EVAL2(__YAZIK_EVAL2(__VA_ARGS__)))
#define __YAZIK_EVAL2(...) __YAZIK_EVAL3(__YAZIK_EVAL3(__YAZIK_EVAL3(__VA_ARGS__)))
#define __YAZIK_EVAL3(...) __YAZIK_EVAL4(__YAZIK_EVAL4(__YAZIK_EVAL4(__VA_ARGS__)))
#define __YAZIK_EVAL4(...) __YAZIK_EVAL5(__YAZIK_EVAL5(__YAZIK_EVAL5(__VA_ARGS__)))
#define __YAZIK_EVAL5(...) __VA_ARGS__

#define __YAZIK_REPEAT(count, macro, ...) \
    __YAZIK_WHEN(count) \
    ( \
        __YAZIK_OBSTRUCT(__YAZIK_REPEAT_INDIRECT) () \
        ( \
            __YAZIK_DEC(count), macro, __VA_ARGS__ \
        ) \
        __YAZIK_OBSTRUCT(macro) \
        ( \
            __YAZIK_DEC(count), __VA_ARGS__ \
        ) \
    )
#define __YAZIK_REPEAT_INDIRECT() __YAZIK_REPEAT

#define __YAZIK_WHILE(pred, op, ...) \
    __YAZIK_IF(pred(__VA_ARGS__)) \
    ( \
        __YAZIK_OBSTRUCT(__YAZIK_WHILE_INDIRECT) () \
        ( \
            pred, op, op(__VA_ARGS__) \
        ), \
        __VA_ARGS__ \
    )
#define __YAZIK_WHILE_INDIRECT() __YAZIK_WHILE

#define __YAZIK_PRIMITIVE_COMPARE(x, y) __YAZIK_IS_PAREN \
( \
    __YAZIK_COMPARE_ ## x ( __YAZIK_COMPARE_ ## y) (())  \
)

#define __YAZIK_IS_COMPARABLE(x) __YAZIK_IS_PAREN( __YAZIK_CAT(__YAZIK_COMPARE_, x) (()) )

#define __YAZIK_NOT_EQUAL(x, y) \
__YAZIK_IIF(__YAZIK_BITAND(__YAZIK_IS_COMPARABLE(x))(__YAZIK_IS_COMPARABLE(y)) ) \
( \
   __YAZIK_PRIMITIVE_COMPARE, \
   1 __YAZIK_EAT \
)(x, y)

#define __YAZIK_EQUAL(x, y) __YAZIK_COMPL(__YAZIK_NOT_EQUAL(x, y))

#define __YAZIK_COMMA() ,

#define __YAZIK_COMMA_IF(n) __YAZIK_IF(n)(__YAZIK_COMMA, __YAZIK_EAT)()
