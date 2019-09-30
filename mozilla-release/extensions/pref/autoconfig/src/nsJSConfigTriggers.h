#ifndef nsConfigTriggers_h
#define nsConfigTriggers_h

#include "nscore.h"
#include "js/TypeDecls.h"

<<<<<<< HEAD
nsresult EvaluateAdminConfigScript(const char *js_buffer, size_t length,
                                   const char *filename, bool bGlobalContext,
                                   bool bCallbacks, bool skipFirstLine,
||||||| merged common ancestors
nsresult EvaluateAdminConfigScript(const char *js_buffer, size_t length,
                                   const char *filename,
                                   bool bGlobalContext,
                                   bool bCallbacks,
                                   bool skipFirstLine,
=======
nsresult EvaluateAdminConfigScript(const char* js_buffer, size_t length,
                                   const char* filename, bool bGlobalContext,
                                   bool bCallbacks, bool skipFirstLine,
>>>>>>> upstream-releases
                                   bool isPrivileged = false);

nsresult EvaluateAdminConfigScript(JS::HandleObject sandbox,
<<<<<<< HEAD
                                   const char *js_buffer, size_t length,
                                   const char *filename, bool bGlobalContext,
                                   bool bCallbacks, bool skipFirstLine);
||||||| merged common ancestors
                                   const char *js_buffer, size_t length,
                                   const char *filename,
                                   bool bGlobalContext,
                                   bool bCallbacks,
                                   bool skipFirstLine);
=======
                                   const char* js_buffer, size_t length,
                                   const char* filename, bool bGlobalContext,
                                   bool bCallbacks, bool skipFirstLine);
>>>>>>> upstream-releases

#endif  // nsConfigTriggers_h
