// Note:  the #if/#elif conditions are to get past the #include order checking.
#if A
<<<<<<< HEAD
#include "tests/style/BadIncludes.h"   // bad: self-include
#include "tests/style/BadIncludes2.h"  // ok
||||||| merged common ancestors
#include "tests/style/BadIncludes.h"    // bad: self-include
#include "tests/style/BadIncludes2.h"   // ok
=======
#  include "tests/style/BadIncludes.h"   // bad: self-include
#  include "tests/style/BadIncludes2.h"  // ok
>>>>>>> upstream-releases
#elif B
<<<<<<< HEAD
#include "BadIncludes2.h"  // bad: not a full path
||||||| merged common ancestors
#include "BadIncludes2.h"               // bad: not a full path
=======
#  include "BadIncludes2.h"  // bad: not a full path
>>>>>>> upstream-releases
#elif C
<<<<<<< HEAD
#include <tests/style/BadIncludes2.h>  // bad: <> form used for local file
||||||| merged common ancestors
#include <tests/style/BadIncludes2.h>   // bad: <> form used for local file
=======
#  include <tests/style/BadIncludes2.h>  // bad: <> form used for local file
>>>>>>> upstream-releases
#elif D
<<<<<<< HEAD
#include "stdio.h"  // bad: "" form used for system file
||||||| merged common ancestors
#include "stdio.h"                      // bad: "" form used for system file
=======
#  include "stdio.h"  // bad: "" form used for system file
>>>>>>> upstream-releases
#endif
