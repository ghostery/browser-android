#ifndef ALOG
<<<<<<< HEAD
#if defined(DEBUG) || defined(FORCE_ALOG)
#define ALOG(args...) __android_log_print(ANDROID_LOG_INFO, "Gecko", ##args)
#else
#define ALOG(args...) ((void)0)
#endif
||||||| merged common ancestors
#if defined(DEBUG) || defined(FORCE_ALOG)
#define ALOG(args...)  __android_log_print(ANDROID_LOG_INFO, "Gecko" , ## args)
#else
#define ALOG(args...) ((void)0)
#endif
=======
#  if defined(DEBUG) || defined(FORCE_ALOG)
#    define ALOG(args...) __android_log_print(ANDROID_LOG_INFO, "Gecko", ##args)
#  else
#    define ALOG(args...) ((void)0)
#  endif
>>>>>>> upstream-releases
#endif

#ifdef DEBUG
#  define ALOG_BRIDGE(args...) ALOG(args)
#else
#  define ALOG_BRIDGE(args...) ((void)0)
#endif
