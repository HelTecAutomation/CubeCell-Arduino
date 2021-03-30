#ifndef __TYPEDEFINE_H__
#define __TYPEDEFINE_H__

typedef   signed          char INT8;
typedef   signed short     int INT16;
typedef   signed           int INT32;
typedef unsigned          char BOOL;
typedef unsigned          char UINT8;
typedef unsigned short     int UINT16;
typedef unsigned           int UINT32;

typedef   signed          char S8;
typedef   signed short     int S16;
typedef   signed           int S32;
typedef unsigned          char U8;
typedef unsigned short     int U16;
typedef unsigned           int U32;

//------------------------------------------------------------------
//                        Definitions
//------------------------------------------------------------------
#define REG32(addr)     (*((UINT32 volatile *)(addr)))
#define REG16(addr)     (*((UINT16 volatile *)(addr)))
#define REG8(addr)      (*((UINT8 volatile  *)(addr)))

#define TRUE   1
#define FALSE  0

#endif /* ifndef __TYPEDEFINE_H__ */
