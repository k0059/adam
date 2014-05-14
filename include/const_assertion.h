/** @brief the constant assertion utils, if assertion fails, a compilation error rises */
#ifndef __CONST_ASSERTION_H__
#define __CONST_ASSERTION_H__
#include <stddef.h>
/** @brief this macro is used to check if the constant is less than a given value, 
 *         if the assertion fails, this macro will rise a compilation error
 */
#define CONST_ASSERTION_LE(id,val) struct __const_checker_##id { int test[(val) - (id)];};
/** @brief this macro is used to check if the constant is greater than a given value, 
 *         if the assertion fails, this macro will rise a compilation error
 */
#define CONST_ASSERTION_GE(id,val) struct __const_checker_##id { int test[(id) - (val)];};
/** @brief this macro is used to check if the constant is less than a given value, 
 *         if the assertion fails, this macro will rise a compilation error
 */
#define CONST_ASSERTION_LT(id,val) struct __const_checker_##id { int test[(val) - (id) - 1];};
/** @brief this macro is used to check if the constant is greater than a given value, 
 *         if the assertion fails, this macro will rise a compilation error
 */
#define CONST_ASSERTION_GT(id,val) struct __const_checker_##id { int test[(id) - (val) - 1];};
/** @brief this macro is used to check if the constant is greater than a given value, 
 *         if the assertion fails, this macro will rise a compilation error
 */
#define CONST_ASSERTION_EQ(id,val) struct __const_checker_##id { int test[((id) - (val)) * ((val) - (id))];};

/* internal macros do not use outside of this file */
#define __CONST_ASSERTION_POSITIVE__(name, val) struct __const_checker_##name {int test[(val)];};
/** @brief check if the member B is followed by member A */
#define CONST_ASSERTION_FOLLOWS(type, A, B) __CONST_ASSERTION_POSITIVE__(follows_##type##_##A##_##B, \
		(offsetof(type,B) - offsetof(type,A) - sizeof(((type*)0)->A)) * \
		(offsetof(type,A) + sizeof(((type*)0)->A) - offsetof(type, B)))

/** @brief check if the member A is before member B */
#define CONST_ASSERTION_BEFORE(type, A, B) __CONST_ASSERTION_POSITIVE__(before_##type##_##A##_##B, \
		(offsetof(type, B) - offsetof(type, A)))

/** @brief check if the member B is after member A */
#define CONST_ASSERTION_AFETER(type, A, B) __CONST_ASSERTION_POSITIVE__(after_##type##_##A##_##B, \
		(offsetof(type, A) - offsetof(type, B)))

/** @brief check if the member is the last member */
#define CONST_ASSERTION_LAST(type, A) __CONST_ASSERTION_POSITIVE__(last_##type##_##A,\
		(offsetof(type, A) + sizeof(((type*)0)->A) - sizeof(type)) * \
		(sizeof(type) - offsetof(type, A) - sizeof(((type*)0)->A)))
/** @brief check the size */
#define CONST_ASSERTION_SIZE(type, A, S) __CONST_ASSERTION_POSITIVE__(size_##type##_##A,\
		(sizeof(((type*)0)->A) - (S)) *\
		((S) - sizeof(((type*)0)->A)))
#endif