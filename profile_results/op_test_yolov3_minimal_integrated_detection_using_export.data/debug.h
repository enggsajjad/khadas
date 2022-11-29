#ifndef _VNN_DEBUGGING_H_
#define _VNN_DEBUGGIN__H_


#define RELEASE

//#define GENERATE_TEMP_FILES

#ifndef RELEASE
  ///Define MYDEBUG for controlling debugging
  #define MY_DEBUG
#else
  ///unDefine MYDEBUG for controlling debugging
  #undef MY_DEBUG
#endif

#ifdef MY_DEBUG
  ///Define MY_DEB() for controlling serial.print debugging
  #define MY_DBG(fmt,args...) do {fprintf(stderr, "[Sajjad] %s:%d:%s(): " fmt,  __FILE__, __LINE__, __func__, ##args);} while (0)
  #define MY_DBG1(fmt,args...) do {fprintf(stderr, "[Sajjad]: " fmt, ##args);} while (0)


#else
  ///UnDefine MY_DEB() for controlling serial.print debugging
  #define MY_DBG(fmt,args...)
  #define MY_DBG1(fmr,args...)

#endif // !MY_DEBUG


#endif

