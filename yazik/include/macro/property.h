#pragma once

#include "capitalizer.h"
#include "get_n_arg.h"
#include "cloak.h"

#define __YAZIK_VA_NARGS_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,N,...) N
#define __YAZIK_VA_NARGS(...)        __YAZIK_VA_NARGS_IMPL(X,##__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1,0)

#define __YAZIK_COMMA_IF_ARGS(...) \
	__YAZIK_IIF(__YAZIK_NOT_EQUAL(__YAZIK_VA_NARGS(__VA_ARGS__),0)) (__YAZIK_COMMA(),__YAZIK_EMPTY())

  #define __YAZIK_DUMMY(...)
  #define __YAZIK_COMPARE___YAZIK_MARKER(X)	X

#define __YAZIK_INVOKE_D(i, ...)                             \
	__YAZIK_GET(__YAZIK_INC(__YAZIK_INC(i)), __VA_ARGS__) 		\
	__YAZIK_IIF(__YAZIK_NOT_EQUAL(                            \
		__YAZIK_VA_NARGS(__VA_ARGS__),                       \
		__YAZIK_INC(__YAZIK_INC(i))                           \
	)) (                                                    \
		__YAZIK_COMMA(),                                     \
		__YAZIK_EMPTY()                                      \
	)

#define __YAZIK_INVOKE(...)                                  \
    __YAZIK_GET(1,__VA_ARGS__)(                              \
    	__YAZIK_EVAL(                                        \
    		__YAZIK_REPEAT(									\
    			__YAZIK_DEC( __YAZIK_VA_NARGS(__VA_ARGS__)),	\
    			__YAZIK_INVOKE_D,                            \
    			__VA_ARGS__                                 \
    		)                                               \
    	)                                                   \
    )

#define __YAZIK_public           public,
#define __YAZIK_protected        protected,
#define __YAZIK_private          private,

#define __YAZIK_SYNTESYZE_VISIBILITY_I(DEFAULT,...) \
    __YAZIK_IF(__YAZIK_EQUAL(__YAZIK_VA_NARGS(__VA_ARGS__),1))(DEFAULT,__YAZIK_GET(1,__VA_ARGS__))

#define __YAZIK_SYNTESYZE_VISIBILITY(TYPE,DEFAULT)	\
    __YAZIK_SYNTESYZE_VISIBILITY_I(DEFAULT,__YAZIK_CAT(__YAZIK_, TYPE))

#define __YAZIK_SYNTESYZE_TYPE_I(...)    \
    __YAZIK_GET(2,__VA_ARGS__)

#define __YAZIK_SYNTESYZE_TYPE(TYPE) \
    __YAZIK_SYNTESYZE_TYPE_I(__YAZIK_CAT(__YAZIK_, TYPE),TYPE)

#define __YAZIK_TAGGIFY(X)   \
    __YAZIK_SYNTESYZE_TYPE(X)

#define __YAZIK_CHECK_IS_I(TAG,X)    \
    __YAZIK_EQUAL(TAG, __YAZIK_GET(1,X))

#define __YAZIK_CHECK_IS(TAG,X)  \
    __YAZIK_CHECK_IS_I(__YAZIK_CAT(YAZIK_, TAG),__YAZIK_CAT(__YAZIK_UNWIND_,__YAZIK_TAGGIFY(X))(__YAZIK_MARKER))

#define __YAZIK_SYNTESYZE_GET_0(TYPE,NAME,TAG,...)   \
	__YAZIK_UNWIND_NO_ ## TAG

#define __YAZIK_SYNTESYZE_INVOKE_I(TYPE,NAME,TAG,X,X_TAG)	\
	__YAZIK_UNWIND_IMPL_ ## TAG (TYPE,NAME,X, __YAZIK_UNWIND_PROXY_ARGS_ ## X_TAG (__YAZIK_MARKER))

#define __YAZIK_SYNTESYZE_INVOKE_P(TYPE,NAME,TAG,X,X_TAG)  \
    __YAZIK_SYNTESYZE_INVOKE_I(TYPE,NAME,TAG,X,X_TAG)

#define __YAZIK_SYNTESYZE_INVOKE(TYPE,NAME,TAG,X,...)    \
    __YAZIK_SYNTESYZE_INVOKE_P(TYPE,NAME,TAG,X,__YAZIK_TAGGIFY(X) )

#define __YAZIK_SYNTESYZE_GET_1(TYPE,NAME,TAG,a) 			\
	__YAZIK_IIF(__YAZIK_CHECK_IS(TAG,a))(						\
		__YAZIK_SYNTESYZE_INVOKE(TYPE,NAME,TAG,a),			\
		__YAZIK_UNWIND_NO_ ## TAG							\
	)
#define __YAZIK_SYNTESYZE_GET_2(TYPE,NAME,TAG,a,b) 			\
	__YAZIK_IIF(__YAZIK_CHECK_IS(TAG,a))(						\
		__YAZIK_SYNTESYZE_INVOKE(TYPE,NAME,TAG,a),			\
		__YAZIK_SYNTESYZE_GET_1(TYPE,NAME,TAG,b)				\
	)
#define __YAZIK_SYNTESYZE_GET_3(TYPE,NAME,TAG,a,b,c) 		\
	__YAZIK_IIF(__YAZIK_CHECK_IS(TAG,a))(						\
		__YAZIK_SYNTESYZE_INVOKE(TYPE,NAME,TAG,a),			\
		__YAZIK_SYNTESYZE_GET_2(TYPE,NAME,TAG,b,c)			\
	)
#define __YAZIK_SYNTESYZE_GET_4(TYPE,NAME,TAG,a,b,c,d) 		\
	__YAZIK_IIF(__YAZIK_CHECK_IS(TAG,a))(						\
		__YAZIK_SYNTESYZE_INVOKE(TYPE,NAME,TAG,a),			\
		__YAZIK_SYNTESYZE_GET_3(TYPE,NAME,TAG,b,c,d)			\
	)
#define __YAZIK_SYNTESYZE_GET_5(TYPE,NAME,TAG,a,b,c,d,e) 	\
	__YAZIK_IIF(__YAZIK_CHECK_IS(TAG,a))(						\
		__YAZIK_SYNTESYZE_INVOKE(TYPE,NAME,TAG,a),			\
		__YAZIK_SYNTESYZE_GET_4(TYPE,NAME,TAG,b,c,d,e)		\
	)

#define __YAZIK_SYNTESYZE_GET(TAG,TYPE,NAME,...)    \
    __YAZIK_CAT(__YAZIK_SYNTESYZE_GET_,__YAZIK_VA_NARGS(__VA_ARGS__))(TYPE,NAME,TAG,__VA_ARGS__)

#define __YAZIK_SYNTESYZE_NAME(NAME) \
	__YAZIK_UNCAPITALIZED(NAME)

#define __YAZIK_SYNTESYZE_FUN_NAME(PREF,NAME) \
	__YAZIK_CAT(PREF,__YAZIK_CAPITALIZED(NAME))

#define __YAZIK_SYNTESYZE_TYPE_DEFINITION(TYPE,NAME,...) \
    __YAZIK_SYNTESYZE_TYPE(TYPE)                         \
    __YAZIK_CAT(_,__YAZIK_SYNTESYZE_NAME(NAME))           \
    __YAZIK_SYNTESYZE_GET(_,TYPE,NAME,__VA_ARGS__);

#define __YAZIK_SYNTESYZE_GETTER(TYPE,NAME, ...) \
	__YAZIK_SYNTESYZE_GET(get,TYPE,NAME,##__VA_ARGS__)

#define __YAZIK_SYNTESYZE_SETTER(TYPE,NAME, ...) \
	__YAZIK_SYNTESYZE_GET(set,TYPE,NAME,##__VA_ARGS__)

//default value
#define __YAZIK_COMPARE_YAZIK__(X)        				X
#define __YAZIK_UNWIND_IMPL__(TYPE,NAME,XF,X,...)        X
#define __YAZIK_UNWIND_NO__                              __YAZIK_EMPTY()
#define __YAZIK_UNWIND_PROXY_ARGS__(...)                 __VA_ARGS__ __YAZIK_DUMMY
#define __YAZIK_UNWIND__(x)                              YAZIK__,

//getter modders
#define __YAZIK_COMPARE_YAZIK_get(X)                             X
#define __YAZIK_UNWIND_PROXY_ARGS_get(...)           \
	__YAZIK_IIF(                                     \
        __YAZIK_EQUAL(__YAZIK_MARKER,                 \
                     __YAZIK_GET(1,__VA_ARGS__)))    \
        (__YAZIK_EMPTY(),__VA_ARGS__ __YAZIK_DUMMY)
#define __YAZIK_UNWIND_IMPL_get(TYPE,NAME,X,...)             \
	__YAZIK_SYNTESYZE_VISIBILITY(X,public):                  \
	__YAZIK_GET(1,__VA_ARGS__)                               \
   	__YAZIK_SYNTESYZE_TYPE(TYPE)                             \
   	__YAZIK_GET(2,__VA_ARGS__)                               \
   	__YAZIK_SYNTESYZE_FUN_NAME(get,NAME)() const 			\
   	{return  __YAZIK_CAT(_,__YAZIK_SYNTESYZE_NAME(NAME));}
#define __YAZIK_UNWIND_NO_get					__YAZIK_EMPTY()
#define __YAZIK_UNWIND_get(...)              	YAZIK_get,

//setter modders
#define __YAZIK_COMPARE_YAZIK_set(X)       				X
#define __YAZIK_UNWIND_PROXY_ARGS_set(...)	\
	__YAZIK_IIF(__YAZIK_EQUAL(__YAZIK_MARKER,__YAZIK_GET(1,__VA_ARGS__)))(__YAZIK_EMPTY(),__VA_ARGS__ __YAZIK_DUMMY)

#define __YAZIK_UNWIND_IMPL_set(TYPE,NAME,X,...)									\
	__YAZIK_SYNTESYZE_VISIBILITY(X,public):										\
	void __YAZIK_SYNTESYZE_FUN_NAME(set,NAME) (									\
	__YAZIK_GET(1,__VA_ARGS__)                               					\
   	__YAZIK_SYNTESYZE_TYPE(TYPE)                             					\
   	__YAZIK_GET(2,__VA_ARGS__)	                              					\
   	__YAZIK_SYNTESYZE_NAME(NAME) )                                               \
   	{__YAZIK_CAT(_,__YAZIK_SYNTESYZE_NAME(NAME)) = __YAZIK_SYNTESYZE_NAME(NAME);}

#define __YAZIK_UNWIND_NO_set					__YAZIK_EMPTY()
#define __YAZIK_UNWIND_set(...)              	YAZIK_set,

#define YPROP(TYPE,NAME,...)                           			\
__YAZIK_SYNTESYZE_VISIBILITY(TYPE,protected):                 	\
	__YAZIK_SYNTESYZE_TYPE_DEFINITION(TYPE,NAME, ##__VA_ARGS__)  \
	__YAZIK_SYNTESYZE_GETTER(TYPE,NAME, ##__VA_ARGS__)			\
	__YAZIK_SYNTESYZE_SETTER(TYPE,NAME, ##__VA_ARGS__)
