#pragma once

#define __YAZIK_CAPITALIZE_TO(...)               \
    __YAZIK_IF(__YAZIK_EQUAL(                     \
        __YAZIK_MARKER,                          \
        __YAZIK_GET(2,__VA_ARGS__)               \
    )) (                                        \
        __YAZIK_GET(3,__VA_ARGS__),              \
        __YAZIK_CAT(__YAZIK_GET(1,__VA_ARGS__),   \
                   __YAZIK_GET(2,__VA_ARGS__))   \
    )

#define __YAZIK_CAPITALIZE_a         A,
#define __YAZIK_CAPITALIZE_b         B,
#define __YAZIK_CAPITALIZE_c         C,
#define __YAZIK_CAPITALIZE_d         D,
#define __YAZIK_CAPITALIZE_e         E,
#define __YAZIK_CAPITALIZE_f         F,
#define __YAZIK_CAPITALIZE_g         G,
#define __YAZIK_CAPITALIZE_h         H,
#define __YAZIK_CAPITALIZE_i         I,
#define __YAZIK_CAPITALIZE_j         J,
#define __YAZIK_CAPITALIZE_k         K,
#define __YAZIK_CAPITALIZE_l         L,
#define __YAZIK_CAPITALIZE_m         M,
#define __YAZIK_CAPITALIZE_n         N,
#define __YAZIK_CAPITALIZE_o         O,
#define __YAZIK_CAPITALIZE_p         P,
#define __YAZIK_CAPITALIZE_q         Q,
#define __YAZIK_CAPITALIZE_r         R,
#define __YAZIK_CAPITALIZE_s         S,
#define __YAZIK_CAPITALIZE_t         T,
#define __YAZIK_CAPITALIZE_u         U,
#define __YAZIK_CAPITALIZE_v         V,
#define __YAZIK_CAPITALIZE_w         W,
#define __YAZIK_CAPITALIZE_x         X,
#define __YAZIK_CAPITALIZE_y         Y,
#define __YAZIK_CAPITALIZE_z         Z,

#define __YAZIK_NOCAPITALIZE_a       a,
#define __YAZIK_NOCAPITALIZE_b       b,
#define __YAZIK_NOCAPITALIZE_c       c,
#define __YAZIK_NOCAPITALIZE_d       d,
#define __YAZIK_NOCAPITALIZE_e       e,
#define __YAZIK_NOCAPITALIZE_f       f,
#define __YAZIK_NOCAPITALIZE_g       g,
#define __YAZIK_NOCAPITALIZE_h       h,
#define __YAZIK_NOCAPITALIZE_i       i,
#define __YAZIK_NOCAPITALIZE_j       j,
#define __YAZIK_NOCAPITALIZE_k       k,
#define __YAZIK_NOCAPITALIZE_l       l,
#define __YAZIK_NOCAPITALIZE_m       m,
#define __YAZIK_NOCAPITALIZE_n       n,
#define __YAZIK_NOCAPITALIZE_o       o,
#define __YAZIK_NOCAPITALIZE_p       p,
#define __YAZIK_NOCAPITALIZE_q       q,
#define __YAZIK_NOCAPITALIZE_r       r,
#define __YAZIK_NOCAPITALIZE_s       s,
#define __YAZIK_NOCAPITALIZE_t       t,
#define __YAZIK_NOCAPITALIZE_u       u,
#define __YAZIK_NOCAPITALIZE_v       v,
#define __YAZIK_NOCAPITALIZE_w       w,
#define __YAZIK_NOCAPITALIZE_x       x,
#define __YAZIK_NOCAPITALIZE_y       y,
#define __YAZIK_NOCAPITALIZE_z       z,

#define __YAZIK_UNCAPITALIZED(X)             \
    __YAZIK_INVOKE(                          \
        __YAZIK_CAPITALIZE_TO,               \
        __YAZIK_CAT(__YAZIK_NOCAPITALIZE_,X), \
        __YAZIK_MARKER,                      \
        X                                   \
    )
#define __YAZIK_CAPITALIZED(X)               \
    __YAZIK_INVOKE(                          \
        __YAZIK_CAPITALIZE_TO,               \
        __YAZIK_CAT(__YAZIK_CAPITALIZE_,X),   \
        __YAZIK_MARKER,                      \
        X                                   \
    )
