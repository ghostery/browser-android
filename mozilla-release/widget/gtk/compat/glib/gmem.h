/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GMEM_WRAPPER_H
#define GMEM_WRAPPER_H

#define g_malloc_n g_malloc_n_
#define g_malloc0_n g_malloc0_n_
#define g_realloc_n g_realloc_n_
#include_next <glib/gmem.h>
#undef g_malloc_n
#undef g_malloc0_n
#undef g_realloc_n

#include <glib/gmessages.h>

#undef g_new
<<<<<<< HEAD
#define g_new(type, num) ((type *)g_malloc_n((num), sizeof(type)))
||||||| merged common ancestors
#define g_new(type, num) \
    ((type *) g_malloc_n((num), sizeof(type)))
=======
#define g_new(type, num) ((type*)g_malloc_n((num), sizeof(type)))
>>>>>>> upstream-releases

#undef g_new0
<<<<<<< HEAD
#define g_new0(type, num) ((type *)g_malloc0_n((num), sizeof(type)))
||||||| merged common ancestors
#define g_new0(type, num) \
    ((type *) g_malloc0_n((num), sizeof(type)))
=======
#define g_new0(type, num) ((type*)g_malloc0_n((num), sizeof(type)))
>>>>>>> upstream-releases

#undef g_renew
<<<<<<< HEAD
#define g_renew(type, ptr, num) ((type *)g_realloc_n(ptr, (num), sizeof(type)))

#define _CHECK_OVERFLOW(num, type_size)                                    \
  if (G_UNLIKELY(type_size > 0 && num > G_MAXSIZE / type_size)) {          \
    g_error("%s: overflow allocating %" G_GSIZE_FORMAT "*%" G_GSIZE_FORMAT \
            " bytes",                                                      \
            G_STRLOC, num, type_size);                                     \
  }

static inline gpointer g_malloc_n(gsize num, gsize type_size) {
||||||| merged common ancestors
#define g_renew(type, ptr, num) \
    ((type *) g_realloc_n(ptr, (num), sizeof(type)))

#define _CHECK_OVERFLOW(num, type_size) \
    if (G_UNLIKELY(type_size > 0 && num > G_MAXSIZE / type_size)) { \
      g_error("%s: overflow allocating %" G_GSIZE_FORMAT "*%" G_GSIZE_FORMAT " bytes", \
              G_STRLOC, num, type_size); \
    }

static inline gpointer
g_malloc_n(gsize num, gsize type_size)
{
=======
#define g_renew(type, ptr, num) ((type*)g_realloc_n(ptr, (num), sizeof(type)))

#define _CHECK_OVERFLOW(num, type_size)                                    \
  if (G_UNLIKELY(type_size > 0 && num > G_MAXSIZE / type_size)) {          \
    g_error("%s: overflow allocating %" G_GSIZE_FORMAT "*%" G_GSIZE_FORMAT \
            " bytes",                                                      \
            G_STRLOC, num, type_size);                                     \
  }

static inline gpointer g_malloc_n(gsize num, gsize type_size) {
>>>>>>> upstream-releases
  _CHECK_OVERFLOW(num, type_size)
  return g_malloc(num * type_size);
}

static inline gpointer g_malloc0_n(gsize num, gsize type_size) {
  _CHECK_OVERFLOW(num, type_size)
  return g_malloc0(num * type_size);
}

static inline gpointer g_realloc_n(gpointer ptr, gsize num, gsize type_size) {
  _CHECK_OVERFLOW(num, type_size)
  return g_realloc(ptr, num * type_size);
}
#endif /* GMEM_WRAPPER_H */
