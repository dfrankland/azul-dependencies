/*
 * Copyright © 2018  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_NULL_HH
#define HB_NULL_HH

#include "hb.hh"


/*
 * Static pools
 */

/* Global nul-content Null pool.  Enlarge as necessary. */

#define HB_NULL_POOL_SIZE 1024

extern HB_INTERNAL
hb_vector_size_impl_t const _hb_NullPool[(HB_NULL_POOL_SIZE + sizeof (hb_vector_size_impl_t) - 1) / sizeof (hb_vector_size_impl_t)];

/* Generic nul-content Null objects. */
template <typename Type>
static inline Type const & Null (void) {
  static_assert (sizeof (Type) <= HB_NULL_POOL_SIZE, "Increase HB_NULL_POOL_SIZE.");
  return *reinterpret_cast<Type const *> (_hb_NullPool);
}
#define Null(Type) Null<typename hb_remove_const<typename hb_remove_reference<Type>::value>::value>()

/* Specializations for arbitrary-content Null objects expressed in bytes. */
#define DECLARE_NULL_NAMESPACE_BYTES(Namespace, Type) \
	} /* Close namespace. */ \
	extern HB_INTERNAL const unsigned char _hb_Null_##Namespace##_##Type[Namespace::Type::min_size]; \
	template <> \
	/*static*/ inline const Namespace::Type& Null<Namespace::Type> (void) { \
	  return *reinterpret_cast<const Namespace::Type *> (_hb_Null_##Namespace##_##Type); \
	} \
	namespace Namespace { \
	static_assert (true, "Just so we take semicolon after.")
#define DEFINE_NULL_NAMESPACE_BYTES(Namespace, Type) \
	const unsigned char _hb_Null_##Namespace##_##Type[Namespace::Type::min_size]

/* Specializations for arbitrary-content Null objects expressed as struct initializer. */
#define DECLARE_NULL_INSTANCE(Type) \
	extern HB_INTERNAL const Type _hb_Null_##Type; \
	template <> \
	/*static*/ inline const Type& Null<Type> (void) { \
	  return _hb_Null_##Type; \
	} \
static_assert (true, "Just so we take semicolon after.")
#define DEFINE_NULL_INSTANCE(Type) \
	const Type _hb_Null_##Type

/* Global writable pool.  Enlarge as necessary. */

/* To be fully correct, CrapPool must be thread_local. However, we do not rely on CrapPool
 * for correct operation. It only exist to catch and divert program logic bugs instead of
 * causing bad memory access. So, races there are not actually introducing incorrectness
 * in the code. Has ~12kb binary size overhead to have it, also clang build fails with it. */
extern HB_INTERNAL
/*thread_local*/ hb_vector_size_impl_t _hb_CrapPool[(HB_NULL_POOL_SIZE + sizeof (hb_vector_size_impl_t) - 1) / sizeof (hb_vector_size_impl_t)];

/* CRAP pool: Common Region for Access Protection. */
template <typename Type>
static inline Type& Crap (void) {
  static_assert (sizeof (Type) <= HB_NULL_POOL_SIZE, "Increase HB_NULL_POOL_SIZE.");
  Type *obj = reinterpret_cast<Type *> (_hb_CrapPool);
  memcpy (obj, &Null(Type), sizeof (*obj));
  return *obj;
}
#define Crap(Type) Crap<typename hb_remove_const<typename hb_remove_reference<Type>::value>::value>()

template <typename Type>
struct CrapOrNull {
  static inline Type & get (void) { return Crap(Type); }
};
template <typename Type>
struct CrapOrNull<const Type> {
  static inline Type const & get (void) { return Null(Type); }
};
#define CrapOrNull(Type) CrapOrNull<Type>::get ()


/*
 * hb_nonnull_ptr_t
 */

template <typename P>
struct hb_nonnull_ptr_t
{
  typedef typename hb_remove_pointer<P>::value T;

  inline hb_nonnull_ptr_t (T *v_ = nullptr) : v (v_) {}
  inline T * operator = (T *v_) { return v = v_; }
  inline T * operator -> (void) const { return get (); }
  inline T & operator * (void) const { return *get (); }
  inline T ** operator & (void) const { return &v; }
  /* Only auto-cast to const types. */
  template <typename C> inline operator const C * (void) const { return get (); }
  inline operator const char * (void) const { return (const char *) get (); }
  inline T * get (void) const { return v ? v : const_cast<T *> (&Null(T)); }
  inline T * get_raw (void) const { return v; }

  T *v;
};


#endif /* HB_NULL_HH */
