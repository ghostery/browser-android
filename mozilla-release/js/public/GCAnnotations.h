/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef js_GCAnnotations_h
#define js_GCAnnotations_h

// Set of annotations for the rooting hazard analysis, used to categorize types
// and functions.
#ifdef XGILL_PLUGIN

#  define JS_EXPECT_HAZARDS __attribute__((annotate("Expect Hazards")))

// Mark a type as being a GC thing (eg js::gc::Cell has this annotation).
<<<<<<< HEAD
#define JS_HAZ_GC_THING __attribute__((annotate("GC Thing")))
||||||| merged common ancestors
# define JS_HAZ_GC_THING __attribute__((annotate("GC Thing")))
=======
#  define JS_HAZ_GC_THING __attribute__((annotate("GC Thing")))
>>>>>>> upstream-releases

// Mark a type as holding a pointer to a GC thing (eg JS::Value has this
<<<<<<< HEAD
// annotation.) "Inherited" by templatized types with
// MOZ_INHERIT_TYPE_ANNOTATIONS_FROM_TEMPLATE_ARGS.
#define JS_HAZ_GC_POINTER __attribute__((annotate("GC Pointer")))
||||||| merged common ancestors
// annotation.)
# define JS_HAZ_GC_POINTER __attribute__((annotate("GC Pointer")))
=======
// annotation.) "Inherited" by templatized types with
// MOZ_INHERIT_TYPE_ANNOTATIONS_FROM_TEMPLATE_ARGS.
#  define JS_HAZ_GC_POINTER __attribute__((annotate("GC Pointer")))
>>>>>>> upstream-releases

// Mark a type as a rooted pointer, suitable for use on the stack (eg all
<<<<<<< HEAD
// Rooted<T> instantiations should have this.) "Inherited" by templatized types
// with MOZ_INHERIT_TYPE_ANNOTATIONS_FROM_TEMPLATE_ARGS.
#define JS_HAZ_ROOTED __attribute__((annotate("Rooted Pointer")))
||||||| merged common ancestors
// Rooted<T> instantiations should have this.)
# define JS_HAZ_ROOTED __attribute__((annotate("Rooted Pointer")))
=======
// Rooted<T> instantiations should have this.) "Inherited" by templatized types
// with MOZ_INHERIT_TYPE_ANNOTATIONS_FROM_TEMPLATE_ARGS.
#  define JS_HAZ_ROOTED __attribute__((annotate("Rooted Pointer")))
>>>>>>> upstream-releases

// Mark a type as something that should not be held live across a GC, but which
<<<<<<< HEAD
// is not itself a GC pointer. Note that this property is *not* inherited by
// templatized types with MOZ_INHERIT_TYPE_ANNOTATIONS_FROM_TEMPLATE_ARGS.
#define JS_HAZ_GC_INVALIDATED __attribute__((annotate("Invalidated by GC")))
||||||| merged common ancestors
// is not itself a GC pointer.
# define JS_HAZ_GC_INVALIDATED __attribute__((annotate("Invalidated by GC")))
=======
// is not itself a GC pointer. Note that this property is *not* inherited by
// templatized types with MOZ_INHERIT_TYPE_ANNOTATIONS_FROM_TEMPLATE_ARGS.
#  define JS_HAZ_GC_INVALIDATED __attribute__((annotate("Invalidated by GC")))
>>>>>>> upstream-releases

// Mark a class as a base class of rooted types, eg CustomAutoRooter. All
// descendants of this class will be considered rooted, though classes that
// merely contain these as a field member will not be. "Inherited" by
// templatized types with MOZ_INHERIT_TYPE_ANNOTATIONS_FROM_TEMPLATE_ARGS
<<<<<<< HEAD
#define JS_HAZ_ROOTED_BASE __attribute__((annotate("Rooted Base")))
||||||| merged common ancestors
# define JS_HAZ_ROOTED_BASE __attribute__((annotate("Rooted Base")))
=======
#  define JS_HAZ_ROOTED_BASE __attribute__((annotate("Rooted Base")))
>>>>>>> upstream-releases

// Mark a type that would otherwise be considered a GC Pointer (eg because it
// contains a JS::Value field) as a non-GC pointer. It is handled almost the
// same in the analysis as a rooted pointer, except it will not be reported as
// an unnecessary root if used across a GC call. This should rarely be used,
// but makes sense for something like ErrorResult, which only contains a GC
// pointer when it holds an exception (and it does its own rooting,
// conditionally.)
<<<<<<< HEAD
#define JS_HAZ_NON_GC_POINTER __attribute__((annotate("Suppressed GC Pointer")))
||||||| merged common ancestors
# define JS_HAZ_NON_GC_POINTER __attribute__((annotate("Suppressed GC Pointer")))
=======
#  define JS_HAZ_NON_GC_POINTER \
    __attribute__((annotate("Suppressed GC Pointer")))
>>>>>>> upstream-releases

// Mark a function as something that runs a garbage collection, potentially
// invalidating GC pointers.
<<<<<<< HEAD
#define JS_HAZ_GC_CALL __attribute__((annotate("GC Call")))
||||||| merged common ancestors
# define JS_HAZ_GC_CALL __attribute__((annotate("GC Call")))
=======
#  define JS_HAZ_GC_CALL __attribute__((annotate("GC Call")))
>>>>>>> upstream-releases

// Mark an RAII class as suppressing GC within its scope.
<<<<<<< HEAD
#define JS_HAZ_GC_SUPPRESSED __attribute__((annotate("Suppress GC")))
||||||| merged common ancestors
# define JS_HAZ_GC_SUPPRESSED __attribute__((annotate("Suppress GC")))
=======
#  define JS_HAZ_GC_SUPPRESSED __attribute__((annotate("Suppress GC")))
>>>>>>> upstream-releases

// Mark a function as one that can run script if called.  This obviously
// subsumes JS_HAZ_GC_CALL, since anything that can run script can GC.`
<<<<<<< HEAD
#define JS_HAZ_CAN_RUN_SCRIPT __attribute__((annotate("Can run script")))

// Mark a function as able to call JSNatives. Otherwise, JSNatives don't show
// up in the callgraph. This doesn't matter for the can-GC analysis, but it is
// very nice for other uses of the callgraph.
#define JS_HAZ_JSNATIVE_CALLER __attribute__((annotate("Calls JSNatives")))
||||||| merged common ancestors
# define JS_HAZ_CAN_RUN_SCRIPT __attribute__((annotate("Can run script")))
=======
#  define JS_HAZ_CAN_RUN_SCRIPT __attribute__((annotate("Can run script")))

// Mark a function as able to call JSNatives. Otherwise, JSNatives don't show
// up in the callgraph. This doesn't matter for the can-GC analysis, but it is
// very nice for other uses of the callgraph.
#  define JS_HAZ_JSNATIVE_CALLER __attribute__((annotate("Calls JSNatives")))
>>>>>>> upstream-releases

#else

<<<<<<< HEAD
#define JS_HAZ_GC_THING
#define JS_HAZ_GC_POINTER
#define JS_HAZ_ROOTED
#define JS_HAZ_GC_INVALIDATED
#define JS_HAZ_ROOTED_BASE
#define JS_HAZ_NON_GC_POINTER
#define JS_HAZ_GC_CALL
#define JS_HAZ_GC_SUPPRESSED
#define JS_HAZ_CAN_RUN_SCRIPT
#define JS_HAZ_JSNATIVE_CALLER
||||||| merged common ancestors
# define JS_HAZ_GC_THING
# define JS_HAZ_GC_POINTER
# define JS_HAZ_ROOTED
# define JS_HAZ_GC_INVALIDATED
# define JS_HAZ_ROOTED_BASE
# define JS_HAZ_NON_GC_POINTER
# define JS_HAZ_GC_CALL
# define JS_HAZ_GC_SUPPRESSED
# define JS_HAZ_CAN_RUN_SCRIPT
=======
#  define JS_EXPECT_HAZARDS
#  define JS_HAZ_GC_THING
#  define JS_HAZ_GC_POINTER
#  define JS_HAZ_ROOTED
#  define JS_HAZ_GC_INVALIDATED
#  define JS_HAZ_ROOTED_BASE
#  define JS_HAZ_NON_GC_POINTER
#  define JS_HAZ_GC_CALL
#  define JS_HAZ_GC_SUPPRESSED
#  define JS_HAZ_CAN_RUN_SCRIPT
#  define JS_HAZ_JSNATIVE_CALLER
>>>>>>> upstream-releases

#endif

#endif /* js_GCAnnotations_h */
