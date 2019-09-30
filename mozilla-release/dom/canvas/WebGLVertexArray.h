/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef WEBGL_VERTEX_ARRAY_H_
#define WEBGL_VERTEX_ARRAY_H_

#include <vector>

#include "mozilla/LinkedList.h"
#include "nsWrapperCache.h"

#include "CacheInvalidator.h"
#include "WebGLObjectModel.h"
#include "WebGLStrongTypes.h"
#include "WebGLVertexAttribData.h"

namespace mozilla {

class WebGLVertexArrayFake;
namespace webgl {
struct LinkedProgramInfo;
}

<<<<<<< HEAD
class WebGLVertexArray : public nsWrapperCache,
                         public WebGLRefCountedObject<WebGLVertexArray>,
                         public LinkedListElement<WebGLVertexArray>,
                         public CacheInvalidator {
 public:
  static WebGLVertexArray* Create(WebGLContext* webgl);

  void Delete();

  WebGLContext* GetParentObject() const { return mContext; }
||||||| merged common ancestors
class WebGLVertexArray
    : public nsWrapperCache
    , public WebGLRefCountedObject<WebGLVertexArray>
    , public LinkedListElement<WebGLVertexArray>
    , public CacheInvalidator
{
public:
    static WebGLVertexArray* Create(WebGLContext* webgl);

    void BindVertexArray() {
        // Bind to dummy value to signal that this vertex array has ever been
        // bound.
        BindVertexArrayImpl();
    };

    // Implement parent classes:
    void Delete();
    bool IsVertexArray() const;
=======
class WebGLVertexArray : public nsWrapperCache,
                         public WebGLRefCountedObject<WebGLVertexArray>,
                         public LinkedListElement<WebGLVertexArray>,
                         public CacheInvalidator {
 public:
  static WebGLVertexArray* Create(WebGLContext* webgl);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual JSObject* WrapObject(JSContext* cx,
                               JS::Handle<JSObject*> givenProto) override;
||||||| merged common ancestors
    WebGLContext* GetParentObject() const {
        return mContext;
    }
=======
  void Delete();
>>>>>>> upstream-releases

<<<<<<< HEAD
  NS_INLINE_DECL_CYCLE_COLLECTING_NATIVE_REFCOUNTING(WebGLVertexArray)
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_NATIVE_CLASS(WebGLVertexArray)
||||||| merged common ancestors
    virtual JSObject* WrapObject(JSContext* cx, JS::Handle<JSObject*> givenProto) override;
=======
  WebGLContext* GetParentObject() const { return mContext; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void AddBufferBindCounts(int8_t addVal) const;
||||||| merged common ancestors
    NS_INLINE_DECL_CYCLE_COLLECTING_NATIVE_REFCOUNTING(WebGLVertexArray)
    NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_NATIVE_CLASS(WebGLVertexArray)
=======
  virtual JSObject* WrapObject(JSContext* cx,
                               JS::Handle<JSObject*> givenProto) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  WebGLVertexArray(WebGLContext* webgl, GLuint name);
  virtual ~WebGLVertexArray();
||||||| merged common ancestors
    GLuint GLName() const { return mGLName; }
=======
  NS_INLINE_DECL_CYCLE_COLLECTING_NATIVE_REFCOUNTING(WebGLVertexArray)
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_NATIVE_CLASS(WebGLVertexArray)
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void BindVertexArray() = 0;
  virtual void DeleteImpl() = 0;
||||||| merged common ancestors
    void AddBufferBindCounts(int8_t addVal) const;
=======
 protected:
  WebGLVertexArray(WebGLContext* webgl, GLuint name);
  virtual ~WebGLVertexArray();
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  const GLuint mGLName;
  bool mHasBeenBound = false;
||||||| merged common ancestors
protected:
    explicit WebGLVertexArray(WebGLContext* webgl);
    virtual ~WebGLVertexArray();
=======
  virtual void BindVertexArray() = 0;
  virtual void DeleteImpl() = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  nsTArray<WebGLVertexAttribData> mAttribs;
  WebGLRefPtr<WebGLBuffer> mElementArrayBuffer;
||||||| merged common ancestors
    virtual void GenVertexArray() = 0;
    virtual void BindVertexArrayImpl() = 0;
    virtual void DeleteImpl() = 0;
    virtual bool IsVertexArrayImpl() const = 0;
=======
 public:
  const GLuint mGLName;
  bool mHasBeenBound = false;
>>>>>>> upstream-releases

<<<<<<< HEAD
  friend class ScopedDrawHelper;
  friend class WebGLContext;
  friend class WebGLVertexArrayFake;
  friend class WebGL2Context;
  friend struct webgl::LinkedProgramInfo;
||||||| merged common ancestors
    GLuint mGLName;
    nsTArray<WebGLVertexAttribData> mAttribs;
    WebGLRefPtr<WebGLBuffer> mElementArrayBuffer;

    friend class ScopedDrawHelper;
    friend class WebGLContext;
    friend class WebGLVertexArrayFake;
    friend class WebGL2Context;
    friend struct webgl::LinkedProgramInfo;
=======
 protected:
  std::vector<WebGLVertexAttribData> mAttribs;
  WebGLRefPtr<WebGLBuffer> mElementArrayBuffer;

  friend class ScopedDrawHelper;
  friend class WebGLContext;
  friend class WebGLVertexArrayFake;
  friend class WebGL2Context;
  friend struct webgl::LinkedProgramInfo;
>>>>>>> upstream-releases
};

}  // namespace mozilla

#endif  // WEBGL_VERTEX_ARRAY_H_
