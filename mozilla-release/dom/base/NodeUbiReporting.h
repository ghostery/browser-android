/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef dom_NodeUbiReporting_h
#define dom_NodeUbiReporting_h

#include "Attr.h"
#include "Document.h"
#include "nsIContent.h"
#include "nsIContentInlines.h"
#include "nsINode.h"
#include "js/UbiNode.h"

/*
 * This file defines specializations of JS::ubi::Concrete for DOM nodes
 * so that the JS memory tools, which operate on the UbiNode graph, can
 * define subclasses of JS::ubi::Base that represent DOM nodes and
 * yield the outgoing edges in a DOM node graph for reporting.
<<<<<<< HEAD
 */

using mozilla::dom::Attr;
||||||| merged common ancestors
*/

using mozilla::dom::Attr;
=======
 */
>>>>>>> upstream-releases

namespace JS {
namespace ubi {

// The DOM node base class.
// This is an abstract class and therefore does not require a concreteTypeName.
<<<<<<< HEAD
template <>
class Concrete<nsINode> : public Base {
 protected:
  explicit Concrete(nsINode *ptr) : Base(ptr) {}

 public:
  static void construct(void *storage, nsINode *ptr);
||||||| merged common ancestors
template<>
class Concrete<nsINode> : public Base
{
protected:
  explicit Concrete(nsINode *ptr) : Base(ptr) { }

public:
  static void construct(void *storage, nsINode *ptr);
=======
template <>
class Concrete<nsINode> : public Base {
 protected:
  explicit Concrete(nsINode* ptr) : Base(ptr) {}

 public:
  static void construct(void* storage, nsINode* ptr);
>>>>>>> upstream-releases
  Size size(mozilla::MallocSizeOf mallocSizeOf) const override;
  js::UniquePtr<EdgeRange> edges(JSContext *cx, bool wantNames) const override;

  nsINode &get() const { return *static_cast<nsINode *>(ptr); }
  CoarseType coarseType() const final { return CoarseType::DOMNode; }
  const char16_t *descriptiveTypeName() const override;
};

<<<<<<< HEAD
template <>
class Concrete<nsIContent> : public Concrete<nsINode> {
 protected:
  explicit Concrete(nsIContent *ptr) : Concrete<nsINode>(ptr) {}
||||||| merged common ancestors
template<>
class Concrete<nsIContent> : public Concrete<nsINode>
{
protected:
  explicit Concrete(nsIContent *ptr) : Concrete<nsINode>(ptr) { }
=======
template <>
class Concrete<nsIContent> : public Concrete<nsINode> {
 protected:
  explicit Concrete(nsIContent* ptr) : Concrete<nsINode>(ptr) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  static void construct(void *storage, nsIContent *ptr) {
    new (storage) Concrete(ptr);
  }
  const char16_t *typeName() const override { return concreteTypeName; };
||||||| merged common ancestors
public:
  static void construct(void *storage, nsIContent *ptr) { new (storage) Concrete(ptr); }
  const char16_t* typeName() const override { return concreteTypeName; };
=======
 public:
  static void construct(void* storage, nsIContent* ptr) {
    new (storage) Concrete(ptr);
  }
  const char16_t* typeName() const override { return concreteTypeName; };
>>>>>>> upstream-releases
  static const char16_t concreteTypeName[];
};

<<<<<<< HEAD
template <>
class Concrete<nsIDocument> : public Concrete<nsINode> {
 protected:
  explicit Concrete(nsIDocument *ptr) : Concrete<nsINode>(ptr) {}
||||||| merged common ancestors
template<>
class Concrete<nsIDocument> : public Concrete<nsINode>
{
protected:
  explicit Concrete(nsIDocument *ptr) : Concrete<nsINode>(ptr) { }
=======
template <>
class Concrete<mozilla::dom::Document> : public Concrete<nsINode> {
 protected:
  explicit Concrete(mozilla::dom::Document* ptr) : Concrete<nsINode>(ptr) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  static void construct(void *storage, nsIDocument *ptr) {
    new (storage) Concrete(ptr);
  }
||||||| merged common ancestors
public:
  static void construct(void *storage, nsIDocument *ptr) { new (storage) Concrete(ptr); }
=======
 public:
  static void construct(void* storage, mozilla::dom::Document* ptr) {
    new (storage) Concrete(ptr);
  }
>>>>>>> upstream-releases
  Size size(mozilla::MallocSizeOf mallocSizeOf) const override;

<<<<<<< HEAD
  nsIDocument &getDoc() const { return *static_cast<nsIDocument *>(ptr); }
  const char16_t *typeName() const override { return concreteTypeName; };
||||||| merged common ancestors
  nsIDocument& getDoc() const { return *static_cast<nsIDocument*>(ptr); }
  const char16_t* typeName() const override { return concreteTypeName; };
=======
  mozilla::dom::Document& getDoc() const {
    return *static_cast<mozilla::dom::Document*>(ptr);
  }
  const char16_t* typeName() const override { return concreteTypeName; };
>>>>>>> upstream-releases
  static const char16_t concreteTypeName[];
};

<<<<<<< HEAD
template <>
class Concrete<Attr> : public Concrete<nsINode> {
 protected:
  explicit Concrete(Attr *ptr) : Concrete<nsINode>(ptr) {}
||||||| merged common ancestors
template<>
class Concrete<Attr> : public Concrete<nsINode>
{
protected:
  explicit Concrete(Attr *ptr) : Concrete<nsINode>(ptr) { }
=======
template <>
class Concrete<mozilla::dom::Attr> : public Concrete<nsINode> {
 protected:
  explicit Concrete(mozilla::dom::Attr* ptr) : Concrete<nsINode>(ptr) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  static void construct(void *storage, Attr *ptr) {
    new (storage) Concrete(ptr);
  }
  const char16_t *typeName() const override { return concreteTypeName; };
||||||| merged common ancestors
public:
  static void construct(void *storage, Attr *ptr) { new (storage) Concrete(ptr); }
  const char16_t* typeName() const override { return concreteTypeName; };
=======
 public:
  static void construct(void* storage, mozilla::dom::Attr* ptr) {
    new (storage) Concrete(ptr);
  }
  const char16_t* typeName() const override { return concreteTypeName; };
>>>>>>> upstream-releases
  static const char16_t concreteTypeName[];
};

}  // namespace ubi
}  // namespace JS

#endif
