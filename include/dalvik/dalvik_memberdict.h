#ifndef __DALVIK_MEMBERDICT_H__
#define __DALVIK_MEMBERDICT_H__
#include <constants.h>
#include <dalvik/dalvik_method.h>
#include <dalvik/dalvik_field.h>
#include <dalvik/dalvik_class.h>

#include <log.h>

/* This file provide a group of function that can be used for member searching,
 * The search key is <pooled_class_path, pooled_member_name>
 */

void dalvik_memberdict_init();
void dalvik_memberdict_finalize();

/* register a method member for some class path */
int dalvik_memberdict_register_method(const char* class_path, dalvik_method_t* method);

/* register a field member for some class path */
int dalvik_memberdict_register_field(const char* class_path, dalvik_field_t* field);
/* register a class */
int dalvik_memberdict_register_class(const char* class_path, dalvik_class_t* class);
/* retrive a method by class_path and name and the type of args is type */
dalvik_method_t* dalvik_memberdict_get_method(const char* class_path, const char* name, dalvik_type_t *const* args);
/* retrive a field by class path and name */
dalvik_field_t* dalvik_memberdict_get_field(const char* class_path, const char* name);
/* retrive a class by class path */
dalvik_class_t* dalvik_memberdict_get_class(const char* class_path);


#endif /* __DALVIK_MEMBERDICT_H__ */