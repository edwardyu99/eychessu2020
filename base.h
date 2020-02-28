#include <assert.h>
#include <sys/timeb.h>
//#include <intrin.h>
//#include "x86asm.h"

#ifndef BASE_H
#define BASE_H

#define __ASSERT(a) assert(a)
#define __ASSERT_BOUND(a, b, c) assert((a) <= (b) && (b) <= (c))
#define __ASSERT_BOUND_2(a, b, c, d) assert((a) <= (b) && (b) <= (c) && (c) <= (d))


#ifdef _MSC_VER

#define FORMAT_I64 "I64"

typedef signed __int64 sint64;     // ll
typedef unsigned __int64 uint64;   // qw

#else
#include <stdint.h>
#define FORMAT_I64 "ll"

typedef signed long long sint64;   // ll
typedef unsigned long long uint64; // qw

#endif
typedef signed long sint32;        // l
typedef unsigned long uint32;      // dw
typedef signed short sint16;       // s
typedef unsigned short uint16;     // w
typedef signed char sint8;         // c
typedef unsigned char uint8;       // uc



typedef int Bool;            // b
typedef sint8 BoolChar;      // bc
typedef sint16 BoolShort;    // bs
typedef sint32 BoolLong;     // bl
typedef sint64 BoolLongLong; // bll



#define FALSE 0
#define TRUE 1
/*
inline uint64 TimeStampCounter(void) {

//unsigned __int64 rdtsc()
//{
  return __rdtsc();
//}
         
//  __asm {
//    rdtsc;
//  }
}
*/
inline Bool EQV(Bool bArg1, Bool bArg2) {
  return bArg1 ? bArg2 : !bArg2;
}

inline Bool XOR(Bool bArg1, Bool bArg2) {
  return bArg1 ? !bArg2 : bArg2;
}

template <typename T> inline T MIN(T Arg1, T Arg2) {
  return Arg1 < Arg2 ? Arg1 : Arg2;
}

template <typename T> inline T MAX(T Arg1, T Arg2) {
  return Arg1 > Arg2 ? Arg1 : Arg2;
}

template <typename T> inline T ABS(T Arg) {
  return Arg < 0 ? -Arg : Arg;
}

template <typename T> inline T SQR(T Arg) {
  return Arg * Arg;
}

template <typename T> inline void SWAP(T &Arg1, T &Arg2) {
  T Temp;
  Temp = Arg1;
  Arg1 = Arg2;
  Arg2 = Temp;
}
/*
inline int PopCnt8(uint8 uc) {
  int n;
  n = ((uc >> 1) & 0x55) + (uc & 0x55);
  n = ((n >> 2) & 0x33) + (n & 0x33);
  return (n >> 4) + (n & 0x0f);
}

inline int PopCnt16(uint16 w) {
  int n;
  n = ((w >> 1) & 0x5555) + (w & 0x5555);
  n = ((n >> 2) & 0x3333) + (n & 0x3333);
  n = ((n >> 4) & 0x0f0f) + (n & 0x0f0f);
  return (n >> 8) + (n & 0x00ff); 
}

inline int PopCnt32(uint32 dw) {
  int n;
  n = ((dw >> 1) & 0x55555555) + (dw & 0x55555555);
  n = ((n >> 2) & 0x33333333) + (n & 0x33333333);
  n = ((n >> 4) & 0x0f0f0f0f) + (n & 0x0f0f0f0f);
  n = ((n >> 8) & 0x00ff00ff) + (n & 0x00ff00ff);
  return (n >> 16) + (n & 0x0000ffff);
}
*/
inline sint64 GetTime() {
  timeb tb;
  ftime(&tb);
  return (sint64) tb.time * 1000 + tb.millitm;
}

/* Note: "MutexAssign(bLock, TRUE)" is equivalent to "Lock(bLock)":
 *
 * inline Bool Lock(volatile Bool &bLock) {
 *   return MutexAssign(bLock, TRUE);
 * }
 */
 /*
inline Bool MutexAssign(volatile int &nDst, int nSrc) {
  return Exchange(&nDst, nSrc) != nSrc;
}

inline Bool MutexAssignEqv(volatile int &nDst, int nSrc, int nComp) {
  return CompareExchange(&nDst, nSrc, nComp) == nComp;
}

inline int MutexAdd(volatile int &nDst, int nSrc) {
  return ExchangeAdd(&nDst, nSrc) + nSrc;
}

inline int MutexIncr(volatile int &nDst) {
  return ExchangeAdd(&nDst, 1) + 1;
}

inline int MutexDecr(volatile int &nDst) {
  return ExchangeAdd(&nDst, -1) - 1;
}
*/

union C4dwStruct {
    char c[4];
    uint32 dw;
};
/*
struct TimerStruct {
  timeb tbStart;
  void Init(void) {
    ftime(&tbStart);
  }  
  int GetTimer(void) {
    timeb tb;
    ftime(&tb);
    return (tb.time - tbStart.time) * 1000 + tb.millitm - tbStart.millitm;
  }
}; // tb
*/
/* Here is the random number algorithm issued by Lewis, Goodman and Miller in 1969:
 * Multiplier = 7 ^ 5;
 * Divisor = 2 ^ 31 - 1; // which is a prime number
 * Seed *= Multiplier;
 * Seed %= Divisor;
 */
/* Deprecated
inline uint32 LongRand(uint32 &dwSeed, uint32 dwMultiplier = 16807) {
  dwSeed = LongMulMod(dwSeed, dwMultiplier, 0x7fffffff);
  return dwSeed;
}

inline uint32 GenLongSeed(void) {
  return (uint32) TimeStampCounter() & 0x7fffffff;
}
*/

struct RC4Struct {
  uint8 s[256];
  int x, y;

  void Init(void *lpKey, int nKeyLen) {
    int i, j;
    x = y = j = 0;
    for (i = 0; i < 256; i ++) {
      s[i] = i;
    }
    for (i = 0; i < 256; i ++) {
      j = (j + s[i] + ((uint8 *) lpKey)[i % nKeyLen]) & 255;
      SWAP(s[i], s[j]);
    }
  }

  void InitZero(void) {
    uint32 dwKey;
    dwKey = 0;
    Init(&dwKey, 4);
  }
/*
  void InitRand(void) {
    union {
      uint32 dw[2];
      uint64 qw;
    } Seed;
    timeb tb;
    ftime(&tb);
    Seed.qw = TimeStampCounter();
    Seed.dw[1] ^= (tb.time << 10) ^ tb.millitm;
    Init(&Seed, 8);
  }
*/
  uint8 NextByte(void) {
    x = (x + 1) & 255;
    y = (y + s[x]) & 255;
    SWAP(s[x], s[y]);
    return s[(s[x] + s[y]) & 255];
  }

  uint32 NextLong(void) {
    union {
      uint8 uc[4];
      uint32 dw;
    } Ret;
    Ret.uc[0] = NextByte();
    Ret.uc[1] = NextByte();
    Ret.uc[2] = NextByte();
    Ret.uc[3] = NextByte();
    return Ret.dw;
  }
};

#endif
