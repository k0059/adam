#ifndef __DALVIK_CLASS_H__
#define __DALVIK_CLASS_H__
#include <sexp.h>
#include <dalvik/dalvik_attrs.h>
#include <const_assertion.h>
#include <constants.h>
/** @file dalvik_class.h
 *  @brief defination of a class
 */
/** @brief class define */
typedef struct{
	const char* path;  /*!<class path */
	const char* super;              /*!<The class path of super class */
	const char* implements[DALVIK_CLASS_MAX_NUM_IMPLEMENTS];         /*!<The interface path that the class implements, ends with a NULL */
	int         attrs;	/*!<attribute of this class */
	int         is_interface; /*!<if this class a interface*/
	const char* members[0];    /*!<We use variant length structure to represents a class */
} dalvik_class_t;
CONST_ASSERTION_FIRST(dalvik_class_t, path);
CONST_ASSERTION_LAST(dalvik_class_t, members);
CONST_ASSERTION_SIZE(dalvik_class_t, members, 0);
/** 
 * @brief
 * Build a new class from a S-Expression
 * @details
 * The reason why we don't need a destructor is
 * all class is managed by memberdict automaticly.
 * So the member take by all class definition and
 * all method and field will be desopsed when the 
 * memberdict finalization function is called 
 * @return the class defination
 */
dalvik_class_t* dalvik_class_from_sexp(const sexpression_t* sexp);

#endif
