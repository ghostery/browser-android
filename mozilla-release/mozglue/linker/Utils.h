/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef Utils_h
#define Utils_h

#include <pthread.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>
#include "mozilla/Assertions.h"
#include "mozilla/Atomics.h"
#include "mozilla/Scoped.h"

/**
 * On architectures that are little endian and that support unaligned reads,
 * we can use direct type, but on others, we want to have a special class
 * to handle conversion and alignment issues.
 */
#if !defined(DEBUG) && (defined(__i386__) || defined(__x86_64__))
typedef uint16_t le_uint16;
typedef uint32_t le_uint32;
#else

/**
 * Template that allows to find an unsigned int type from a (computed) bit size
 */
template <int s>
struct UInt {};
template <>
struct UInt<16> {
  typedef uint16_t Type;
};
template <>
struct UInt<32> {
  typedef uint32_t Type;
};

/**
 * Template to access 2 n-bit sized words as a 2*n-bit sized word, doing
 * conversion from little endian and avoiding alignment issues.
 */
template <typename T>
class le_to_cpu {
 public:
  typedef typename UInt<16 * sizeof(T)>::Type Type;

  operator Type() const { return (b << (sizeof(T) * 8)) | a; }

<<<<<<< HEAD
  const le_to_cpu &operator=(const Type &v) {
||||||| merged common ancestors
  const le_to_cpu& operator =(const Type &v)
  {
=======
  const le_to_cpu& operator=(const Type& v) {
>>>>>>> upstream-releases
    a = v & ((1 << (sizeof(T) * 8)) - 1);
    b = v >> (sizeof(T) * 8);
    return *this;
  }

<<<<<<< HEAD
  le_to_cpu() {}
  explicit le_to_cpu(const Type &v) { operator=(v); }
||||||| merged common ancestors
  le_to_cpu() { }
  explicit le_to_cpu(const Type &v)
  {
    operator =(v);
  }
=======
  le_to_cpu() {}
  explicit le_to_cpu(const Type& v) { operator=(v); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const le_to_cpu &operator+=(const Type &v) {
    return operator=(operator Type() + v);
||||||| merged common ancestors
  const le_to_cpu& operator +=(const Type &v)
  {
    return operator =(operator Type() + v);
=======
  const le_to_cpu& operator+=(const Type& v) {
    return operator=(operator Type() + v);
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  const le_to_cpu &operator++(int) { return operator=(operator Type() + 1); }
||||||| merged common ancestors
  const le_to_cpu& operator ++(int)
  {
    return operator =(operator Type() + 1);
  }
=======
  const le_to_cpu& operator++(int) { return operator=(operator Type() + 1); }
>>>>>>> upstream-releases

 private:
  T a, b;
};

/**
 * Type definitions
 */
typedef le_to_cpu<unsigned char> le_uint16;
typedef le_to_cpu<le_uint16> le_uint32;
#endif

/**
 * AutoCloseFD is a RAII wrapper for POSIX file descriptors
 */
struct AutoCloseFDTraits {
  typedef int type;
  static int empty() { return -1; }
  static void release(int fd) {
    if (fd != -1) close(fd);
  }
};
typedef mozilla::Scoped<AutoCloseFDTraits> AutoCloseFD;

/**
 * AutoCloseFILE is a RAII wrapper for POSIX streams
 */
<<<<<<< HEAD
struct AutoCloseFILETraits {
  typedef FILE *type;
  static FILE *empty() { return nullptr; }
  static void release(FILE *f) {
    if (f) fclose(f);
  }
||||||| merged common ancestors
struct AutoCloseFILETraits
{
  typedef FILE *type;
  static FILE *empty() { return nullptr; }
  static void release(FILE *f) { if (f) fclose(f); }
=======
struct AutoCloseFILETraits {
  typedef FILE* type;
  static FILE* empty() { return nullptr; }
  static void release(FILE* f) {
    if (f) fclose(f);
  }
>>>>>>> upstream-releases
};
typedef mozilla::Scoped<AutoCloseFILETraits> AutoCloseFILE;

extern mozilla::Atomic<size_t, mozilla::ReleaseAcquire> gPageSize;

/**
 * Page alignment helpers
 */
static size_t PageSize() {
  if (!gPageSize) {
    gPageSize = sysconf(_SC_PAGESIZE);
  }

  return gPageSize;
}

static inline uintptr_t AlignedPtr(uintptr_t ptr, size_t alignment) {
  return ptr & ~(alignment - 1);
}

template <typename T>
<<<<<<< HEAD
static inline T *AlignedPtr(T *ptr, size_t alignment) {
  return reinterpret_cast<T *>(
      AlignedPtr(reinterpret_cast<uintptr_t>(ptr), alignment));
||||||| merged common ancestors
static inline T *AlignedPtr(T *ptr, size_t alignment)
{
  return reinterpret_cast<T *>(
         AlignedPtr(reinterpret_cast<uintptr_t>(ptr), alignment));
=======
static inline T* AlignedPtr(T* ptr, size_t alignment) {
  return reinterpret_cast<T*>(
      AlignedPtr(reinterpret_cast<uintptr_t>(ptr), alignment));
>>>>>>> upstream-releases
}

template <typename T>
static inline T PageAlignedPtr(T ptr) {
  return AlignedPtr(ptr, PageSize());
}

static inline uintptr_t AlignedEndPtr(uintptr_t ptr, size_t alignment) {
  return AlignedPtr(ptr + alignment - 1, alignment);
}

template <typename T>
<<<<<<< HEAD
static inline T *AlignedEndPtr(T *ptr, size_t alignment) {
  return reinterpret_cast<T *>(
      AlignedEndPtr(reinterpret_cast<uintptr_t>(ptr), alignment));
||||||| merged common ancestors
static inline T *AlignedEndPtr(T *ptr, size_t alignment)
{
  return reinterpret_cast<T *>(
         AlignedEndPtr(reinterpret_cast<uintptr_t>(ptr), alignment));
=======
static inline T* AlignedEndPtr(T* ptr, size_t alignment) {
  return reinterpret_cast<T*>(
      AlignedEndPtr(reinterpret_cast<uintptr_t>(ptr), alignment));
>>>>>>> upstream-releases
}

template <typename T>
static inline T PageAlignedEndPtr(T ptr) {
  return AlignedEndPtr(ptr, PageSize());
}

static inline size_t AlignedSize(size_t size, size_t alignment) {
  return (size + alignment - 1) & ~(alignment - 1);
}

static inline size_t PageAlignedSize(size_t size) {
  return AlignedSize(size, PageSize());
}

static inline bool IsAlignedPtr(uintptr_t ptr, size_t alignment) {
  return ptr % alignment == 0;
}

template <typename T>
<<<<<<< HEAD
static inline bool IsAlignedPtr(T *ptr, size_t alignment) {
||||||| merged common ancestors
static inline bool IsAlignedPtr(T *ptr, size_t alignment)
{
=======
static inline bool IsAlignedPtr(T* ptr, size_t alignment) {
>>>>>>> upstream-releases
  return IsAlignedPtr(reinterpret_cast<uintptr_t>(ptr), alignment);
}

template <typename T>
static inline bool IsPageAlignedPtr(T ptr) {
  return IsAlignedPtr(ptr, PageSize());
}

static inline bool IsAlignedSize(size_t size, size_t alignment) {
  return size % alignment == 0;
}

static inline bool IsPageAlignedSize(size_t size) {
  return IsAlignedSize(size, PageSize());
}

static inline size_t PageNumber(size_t size) {
  return (size + PageSize() - 1) / PageSize();
}

/**
 * MemoryRange stores a pointer, size pair.
 */
<<<<<<< HEAD
class MemoryRange {
 public:
  MemoryRange(void *buf, size_t length) : buf(buf), length(length) {}
||||||| merged common ancestors
class MemoryRange
{
public:
  MemoryRange(void *buf, size_t length): buf(buf), length(length) { }
=======
class MemoryRange {
 public:
  MemoryRange(void* buf, size_t length) : buf(buf), length(length) {}
>>>>>>> upstream-releases

  void Assign(void* b, size_t len) {
    buf = b;
    length = len;
  }

  void Assign(const MemoryRange &other) {
    buf = other.buf;
    length = other.length;
  }

<<<<<<< HEAD
  void *get() const { return buf; }
||||||| merged common ancestors
  void *get() const
  {
    return buf;
  }
=======
  void* get() const { return buf; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  operator void *() const { return buf; }
||||||| merged common ancestors
  operator void *() const
  {
    return buf;
  }
=======
  operator void*() const { return buf; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  operator unsigned char *() const {
    return reinterpret_cast<unsigned char *>(buf);
||||||| merged common ancestors
  operator unsigned char *() const
  {
    return reinterpret_cast<unsigned char *>(buf);
=======
  operator unsigned char*() const {
    return reinterpret_cast<unsigned char*>(buf);
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  bool operator==(void *ptr) const { return buf == ptr; }
||||||| merged common ancestors
  bool operator ==(void *ptr) const {
    return buf == ptr;
  }
=======
  bool operator==(void* ptr) const { return buf == ptr; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool operator==(unsigned char *ptr) const { return buf == ptr; }
||||||| merged common ancestors
  bool operator ==(unsigned char *ptr) const {
    return buf == ptr;
  }
=======
  bool operator==(unsigned char* ptr) const { return buf == ptr; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void *operator+(off_t offset) const {
    return reinterpret_cast<char *>(buf) + offset;
||||||| merged common ancestors
  void *operator +(off_t offset) const
  {
    return reinterpret_cast<char *>(buf) + offset;
=======
  void* operator+(off_t offset) const {
    return reinterpret_cast<char*>(buf) + offset;
>>>>>>> upstream-releases
  }

  /**
   * Returns whether the given address is within the mapped range
   */
<<<<<<< HEAD
  bool Contains(void *ptr) const {
    return (ptr >= buf) && (ptr < reinterpret_cast<char *>(buf) + length);
||||||| merged common ancestors
  bool Contains(void *ptr) const
  {
    return (ptr >= buf) && (ptr < reinterpret_cast<char *>(buf) + length);
=======
  bool Contains(void* ptr) const {
    return (ptr >= buf) && (ptr < reinterpret_cast<char*>(buf) + length);
>>>>>>> upstream-releases
  }

  /**
   * Returns the length of the mapped range
   */
  size_t GetLength() const { return length; }

  static MemoryRange mmap(void* addr, size_t length, int prot, int flags,
                          int fd, off_t offset) {
    return MemoryRange(::mmap(addr, length, prot, flags, fd, offset), length);
  }

<<<<<<< HEAD
 private:
  void *buf;
||||||| merged common ancestors
private:
  void *buf;
=======
 private:
  void* buf;
>>>>>>> upstream-releases
  size_t length;
};

/**
 * MappedPtr is a RAII wrapper for mmap()ed memory. It can be used as
 * a simple void * or unsigned char *.
 *
 * It is defined as a derivative of a template that allows to use a
 * different unmapping strategy.
 */
template <typename T>
<<<<<<< HEAD
class GenericMappedPtr : public MemoryRange {
 public:
  GenericMappedPtr(void *buf, size_t length) : MemoryRange(buf, length) {}
  explicit GenericMappedPtr(const MemoryRange &other) : MemoryRange(other) {}
  GenericMappedPtr() : MemoryRange(MAP_FAILED, 0) {}

  void Assign(void *b, size_t len) {
    if (get() != MAP_FAILED) static_cast<T *>(this)->munmap(get(), GetLength());
||||||| merged common ancestors
class GenericMappedPtr: public MemoryRange
{
public:
  GenericMappedPtr(void *buf, size_t length): MemoryRange(buf, length) { }
  explicit GenericMappedPtr(const MemoryRange& other): MemoryRange(other) { }
  GenericMappedPtr(): MemoryRange(MAP_FAILED, 0) { }

  void Assign(void *b, size_t len) {
    if (get() != MAP_FAILED)
      static_cast<T *>(this)->munmap(get(), GetLength());
=======
class GenericMappedPtr : public MemoryRange {
 public:
  GenericMappedPtr(void* buf, size_t length) : MemoryRange(buf, length) {}
  explicit GenericMappedPtr(const MemoryRange& other) : MemoryRange(other) {}
  GenericMappedPtr() : MemoryRange(MAP_FAILED, 0) {}

  void Assign(void* b, size_t len) {
    if (get() != MAP_FAILED) static_cast<T*>(this)->munmap(get(), GetLength());
>>>>>>> upstream-releases
    MemoryRange::Assign(b, len);
  }

  void Assign(const MemoryRange &other) {
    Assign(other.get(), other.GetLength());
  }

<<<<<<< HEAD
  ~GenericMappedPtr() {
    if (get() != MAP_FAILED) static_cast<T *>(this)->munmap(get(), GetLength());
||||||| merged common ancestors
  ~GenericMappedPtr()
  {
    if (get() != MAP_FAILED)
      static_cast<T *>(this)->munmap(get(), GetLength());
=======
  ~GenericMappedPtr() {
    if (get() != MAP_FAILED) static_cast<T*>(this)->munmap(get(), GetLength());
>>>>>>> upstream-releases
  }

  void release() { MemoryRange::Assign(MAP_FAILED, 0); }
};

<<<<<<< HEAD
struct MappedPtr : public GenericMappedPtr<MappedPtr> {
  MappedPtr(void *buf, size_t length)
      : GenericMappedPtr<MappedPtr>(buf, length) {}
  MOZ_IMPLICIT MappedPtr(const MemoryRange &other)
      : GenericMappedPtr<MappedPtr>(other) {}
  MappedPtr() : GenericMappedPtr<MappedPtr>() {}
||||||| merged common ancestors
struct MappedPtr: public GenericMappedPtr<MappedPtr>
{
  MappedPtr(void *buf, size_t length)
  : GenericMappedPtr<MappedPtr>(buf, length) { }
  MOZ_IMPLICIT MappedPtr(const MemoryRange& other)
  : GenericMappedPtr<MappedPtr>(other) { }
  MappedPtr(): GenericMappedPtr<MappedPtr>() { }
=======
struct MappedPtr : public GenericMappedPtr<MappedPtr> {
  MappedPtr(void* buf, size_t length)
      : GenericMappedPtr<MappedPtr>(buf, length) {}
  MOZ_IMPLICIT MappedPtr(const MemoryRange& other)
      : GenericMappedPtr<MappedPtr>(other) {}
  MappedPtr() : GenericMappedPtr<MappedPtr>() {}
>>>>>>> upstream-releases

 private:
  friend class GenericMappedPtr<MappedPtr>;
<<<<<<< HEAD
  void munmap(void *buf, size_t length) { ::munmap(buf, length); }
||||||| merged common ancestors
  void munmap(void *buf, size_t length)
  {
    ::munmap(buf, length);
  }
=======
  void munmap(void* buf, size_t length) { ::munmap(buf, length); }
>>>>>>> upstream-releases
};

/**
 * UnsizedArray is a way to access raw arrays of data in memory.
 *
 *   struct S { ... };
 *   UnsizedArray<S> a(buf);
 *   UnsizedArray<S> b; b.Init(buf);
 *
 * This is roughly equivalent to
 *   const S *a = reinterpret_cast<const S *>(buf);
 *   const S *b = nullptr; b = reinterpret_cast<const S *>(buf);
 *
 * An UnsizedArray has no known length, and it's up to the caller to make
 * sure the accessed memory is mapped and makes sense.
 */
template <typename T>
class UnsizedArray {
 public:
  typedef size_t idx_t;

  /**
   * Constructors and Initializers
   */
<<<<<<< HEAD
  UnsizedArray() : contents(nullptr) {}
  explicit UnsizedArray(const void *buf)
      : contents(reinterpret_cast<const T *>(buf)) {}
||||||| merged common ancestors
  UnsizedArray(): contents(nullptr) { }
  explicit UnsizedArray(const void *buf): contents(reinterpret_cast<const T *>(buf)) { }
=======
  UnsizedArray() : contents(nullptr) {}
  explicit UnsizedArray(const void* buf)
      : contents(reinterpret_cast<const T*>(buf)) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Init(const void *buf) {
||||||| merged common ancestors
  void Init(const void *buf)
  {
=======
  void Init(const void* buf) {
>>>>>>> upstream-releases
    MOZ_ASSERT(contents == nullptr);
    contents = reinterpret_cast<const T*>(buf);
  }

  /**
   * Returns the nth element of the array
   */
<<<<<<< HEAD
  const T &operator[](const idx_t index) const {
||||||| merged common ancestors
  const T &operator[](const idx_t index) const
  {
=======
  const T& operator[](const idx_t index) const {
>>>>>>> upstream-releases
    MOZ_ASSERT(contents);
    return contents[index];
  }

<<<<<<< HEAD
  operator const T *() const { return contents; }
||||||| merged common ancestors
  operator const T *() const
  {
    return contents;
  }
=======
  operator const T*() const { return contents; }
>>>>>>> upstream-releases
  /**
   * Returns whether the array points somewhere
   */
<<<<<<< HEAD
  explicit operator bool() const { return contents != nullptr; }

 private:
  const T *contents;
||||||| merged common ancestors
  explicit operator bool() const
  {
    return contents != nullptr;
  }
private:
  const T *contents;
=======
  explicit operator bool() const { return contents != nullptr; }

 private:
  const T* contents;
>>>>>>> upstream-releases
};

/**
 * Array, like UnsizedArray, is a way to access raw arrays of data in memory.
 * Unlike UnsizedArray, it has a known length, and is enumerable with an
 * iterator.
 *
 *   struct S { ... };
 *   Array<S> a(buf, len);
 *   UnsizedArray<S> b; b.Init(buf, len);
 *
 * In the above examples, len is the number of elements in the array. It is
 * also possible to initialize an Array with the buffer size:
 *
 *   Array<S> c; c.InitSize(buf, size);
 *
 * It is also possible to initialize an Array in two steps, only providing
 * one data at a time:
 *
 *   Array<S> d;
 *   d.Init(buf);
 *   d.Init(len); // or d.InitSize(size);
 *
 */
template <typename T>
class Array : public UnsizedArray<T> {
 public:
  typedef typename UnsizedArray<T>::idx_t idx_t;

  /**
   * Constructors and Initializers
   */
<<<<<<< HEAD
  Array() : UnsizedArray<T>(), length(0) {}
  Array(const void *buf, const idx_t length)
      : UnsizedArray<T>(buf), length(length) {}
||||||| merged common ancestors
  Array(): UnsizedArray<T>(), length(0) { }
  Array(const void *buf, const idx_t length)
  : UnsizedArray<T>(buf), length(length) { }
=======
  Array() : UnsizedArray<T>(), length(0) {}
  Array(const void* buf, const idx_t length)
      : UnsizedArray<T>(buf), length(length) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Init(const void *buf) { UnsizedArray<T>::Init(buf); }
||||||| merged common ancestors
  void Init(const void *buf)
  {
    UnsizedArray<T>::Init(buf);
  }
=======
  void Init(const void* buf) { UnsizedArray<T>::Init(buf); }
>>>>>>> upstream-releases

  void Init(const idx_t len) {
    MOZ_ASSERT(length == 0);
    length = len;
  }

  void InitSize(const idx_t size) { Init(size / sizeof(T)); }

<<<<<<< HEAD
  void Init(const void *buf, const idx_t len) {
||||||| merged common ancestors
  void Init(const void *buf, const idx_t len)
  {
=======
  void Init(const void* buf, const idx_t len) {
>>>>>>> upstream-releases
    UnsizedArray<T>::Init(buf);
    Init(len);
  }

<<<<<<< HEAD
  void InitSize(const void *buf, const idx_t size) {
||||||| merged common ancestors
  void InitSize(const void *buf, const idx_t size)
  {
=======
  void InitSize(const void* buf, const idx_t size) {
>>>>>>> upstream-releases
    UnsizedArray<T>::Init(buf);
    InitSize(size);
  }

  /**
   * Returns the nth element of the array
   */
<<<<<<< HEAD
  const T &operator[](const idx_t index) const {
||||||| merged common ancestors
  const T &operator[](const idx_t index) const
  {
=======
  const T& operator[](const idx_t index) const {
>>>>>>> upstream-releases
    MOZ_ASSERT(index < length);
    MOZ_ASSERT(operator bool());
    return UnsizedArray<T>::operator[](index);
  }

  /**
   * Returns the number of elements in the array
   */
  idx_t numElements() const { return length; }

  /**
   * Returns whether the array points somewhere and has at least one element.
   */
  explicit operator bool() const {
    return (length > 0) && UnsizedArray<T>::operator bool();
  }

  /**
   * Iterator for an Array. Use is similar to that of STL const_iterators:
   *
   *   struct S { ... };
   *   Array<S> a(buf, len);
   *   for (Array<S>::iterator it = a.begin(); it < a.end(); ++it) {
   *     // Do something with *it.
   *   }
   */
  class iterator {
   public:
    iterator() : item(nullptr) {}

<<<<<<< HEAD
    const T &operator*() const { return *item; }
||||||| merged common ancestors
    const T *operator ->() const
    {
      return item;
    }
=======
    const T& operator*() const { return *item; }

    const T* operator->() const { return item; }
>>>>>>> upstream-releases

<<<<<<< HEAD
    const T *operator->() const { return item; }

    iterator &operator++() {
||||||| merged common ancestors
    iterator &operator ++()
    {
=======
    iterator& operator++() {
>>>>>>> upstream-releases
      ++item;
      return *this;
    }

<<<<<<< HEAD
    bool operator<(const iterator &other) const { return item < other.item; }

   protected:
||||||| merged common ancestors
    bool operator<(const iterator &other) const
    {
      return item < other.item;
    }
  protected:
=======
    bool operator<(const iterator& other) const { return item < other.item; }

   protected:
>>>>>>> upstream-releases
    friend class Array<T>;
<<<<<<< HEAD
    explicit iterator(const T &item) : item(&item) {}
||||||| merged common ancestors
    explicit iterator(const T &item): item(&item) { }
=======
    explicit iterator(const T& item) : item(&item) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
   private:
    const T *item;
||||||| merged common ancestors
  private:
    const T *item;
=======
   private:
    const T* item;
>>>>>>> upstream-releases
  };

  /**
   * Returns an iterator pointing at the beginning of the Array
   */
  iterator begin() const {
    if (length) return iterator(UnsizedArray<T>::operator[](0));
    return iterator();
  }

  /**
   * Returns an iterator pointing past the end of the Array
   */
  iterator end() const {
    if (length) return iterator(UnsizedArray<T>::operator[](length));
    return iterator();
  }

  /**
   * Reverse iterator for an Array. Use is similar to that of STL
   * const_reverse_iterators:
   *
   *   struct S { ... };
   *   Array<S> a(buf, len);
   *   for (Array<S>::reverse_iterator it = a.rbegin(); it < a.rend(); ++it) {
   *     // Do something with *it.
   *   }
   */
<<<<<<< HEAD
  class reverse_iterator {
   public:
    reverse_iterator() : item(nullptr) {}

    const T &operator*() const {
      const T *tmp = item;
||||||| merged common ancestors
  class reverse_iterator
  {
  public:
    reverse_iterator(): item(nullptr) { }

    const T &operator *() const
    {
      const T *tmp = item;
=======
  class reverse_iterator {
   public:
    reverse_iterator() : item(nullptr) {}

    const T& operator*() const {
      const T* tmp = item;
>>>>>>> upstream-releases
      return *--tmp;
    }

<<<<<<< HEAD
    const T *operator->() const { return &operator*(); }
||||||| merged common ancestors
    const T *operator ->() const
    {
      return &operator*();
    }
=======
    const T* operator->() const { return &operator*(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
    reverse_iterator &operator++() {
||||||| merged common ancestors
    reverse_iterator &operator ++()
    {
=======
    reverse_iterator& operator++() {
>>>>>>> upstream-releases
      --item;
      return *this;
    }

<<<<<<< HEAD
    bool operator<(const reverse_iterator &other) const {
||||||| merged common ancestors
    bool operator<(const reverse_iterator &other) const
    {
=======
    bool operator<(const reverse_iterator& other) const {
>>>>>>> upstream-releases
      return item > other.item;
    }

   protected:
    friend class Array<T>;
<<<<<<< HEAD
    explicit reverse_iterator(const T &item) : item(&item) {}
||||||| merged common ancestors
    explicit reverse_iterator(const T &item): item(&item) { }
=======
    explicit reverse_iterator(const T& item) : item(&item) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
   private:
    const T *item;
||||||| merged common ancestors
  private:
    const T *item;
=======
   private:
    const T* item;
>>>>>>> upstream-releases
  };

  /**
   * Returns a reverse iterator pointing at the end of the Array
   */
  reverse_iterator rbegin() const {
    if (length) return reverse_iterator(UnsizedArray<T>::operator[](length));
    return reverse_iterator();
  }

  /**
   * Returns a reverse iterator pointing past the beginning of the Array
   */
  reverse_iterator rend() const {
    if (length) return reverse_iterator(UnsizedArray<T>::operator[](0));
    return reverse_iterator();
  }

 private:
  idx_t length;
};

/**
 * Transforms a pointer-to-function to a pointer-to-object pointing at the
 * same address.
 */
template <typename T>
<<<<<<< HEAD
void *FunctionPtr(T func) {
||||||| merged common ancestors
void *FunctionPtr(T func)
{
=======
void* FunctionPtr(T func) {
>>>>>>> upstream-releases
  union {
    void* ptr;
    T func;
  } f;
  f.func = func;
  return f.ptr;
}

class AutoLock {
<<<<<<< HEAD
 public:
  explicit AutoLock(pthread_mutex_t *mutex) : mutex(mutex) {
    if (pthread_mutex_lock(mutex)) MOZ_CRASH("pthread_mutex_lock failed");
||||||| merged common ancestors
public:
  explicit AutoLock(pthread_mutex_t *mutex): mutex(mutex)
  {
    if (pthread_mutex_lock(mutex))
      MOZ_CRASH("pthread_mutex_lock failed");
=======
 public:
  explicit AutoLock(pthread_mutex_t* mutex) : mutex(mutex) {
    if (pthread_mutex_lock(mutex)) MOZ_CRASH("pthread_mutex_lock failed");
>>>>>>> upstream-releases
  }
  ~AutoLock() {
    if (pthread_mutex_unlock(mutex)) MOZ_CRASH("pthread_mutex_unlock failed");
  }
<<<<<<< HEAD

 private:
  pthread_mutex_t *mutex;
||||||| merged common ancestors
private:
  pthread_mutex_t *mutex;
=======

 private:
  pthread_mutex_t* mutex;
>>>>>>> upstream-releases
};

#endif /* Utils_h */
