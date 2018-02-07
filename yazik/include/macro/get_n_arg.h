#pragma once

#define __YAZIK_COMPARE_0(X)				X
#define __YAZIK_COMPARE_1(X)				X
#define __YAZIK_COMPARE_2(X)				X
#define __YAZIK_COMPARE_3(X)				X
#define __YAZIK_COMPARE_4(X)				X
#define __YAZIK_COMPARE_5(X)				X
#define __YAZIK_COMPARE_6(X)				X
#define __YAZIK_COMPARE_7(X)				X
#define __YAZIK_COMPARE_8(X)				X

#define __YAZIK_GET_IMPL_1_FROM_0()			__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_1_FROM_1(a)			a
#define __YAZIK_GET_IMPL_1_FROM_2(a,b)		a
#define __YAZIK_GET_IMPL_1_FROM_3(a,b,c)		a
#define __YAZIK_GET_IMPL_1_FROM_4(a,b,c,d)	a
#define __YAZIK_GET_IMPL_1_FROM_5(a,b,c,d,e)	a

#define __YAZIK_GET_IMPL_2_FROM_0()			__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_2_FROM_1(a)			__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_2_FROM_2(a,b)		b
#define __YAZIK_GET_IMPL_2_FROM_3(a,b,c)		b
#define __YAZIK_GET_IMPL_2_FROM_4(a,b,c,d)	b
#define __YAZIK_GET_IMPL_2_FROM_5(a,b,c,d,e)	b

#define __YAZIK_GET_IMPL_3_FROM_0()			__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_3_FROM_1(a)			__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_3_FROM_2(a,b)		__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_3_FROM_3(a,b,c)		c
#define __YAZIK_GET_IMPL_3_FROM_4(a,b,c,d)	c
#define __YAZIK_GET_IMPL_3_FROM_5(a,b,c,d,e)	c

#define __YAZIK_GET_IMPL_4_FROM_0()			__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_4_FROM_1(a)			__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_4_FROM_2(a,b)		__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_4_FROM_3(a,b,c)		__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_4_FROM_4(a,b,c,d)	d
#define __YAZIK_GET_IMPL_4_FROM_5(a,b,c,d,e)	d

#define __YAZIK_GET_IMPL_5_FROM_0()			__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_5_FROM_1(a)			__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_5_FROM_2(a,b)		__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_5_FROM_3(a,b,c)		__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_5_FROM_4(a,b,c,d)	__YAZIK_EMPTY()
#define __YAZIK_GET_IMPL_5_FROM_5(a,b,c,d,e)	e

#define __YAZIK_GET_IMPL_P(NUM,COUNT,...)    \
    __YAZIK_GET_IMPL_ ## NUM ## _FROM_ ## COUNT (__VA_ARGS__)

#define __YAZIK_GET_IMPL(NUM,COUNT,...)  \
    __YAZIK_GET_IMPL_P(NUM,COUNT,##__VA_ARGS__)

#define __YAZIK_GET(NUM,...) \
    __YAZIK_GET_IMPL(NUM,__YAZIK_VA_NARGS(__VA_ARGS__), ##__VA_ARGS__)
