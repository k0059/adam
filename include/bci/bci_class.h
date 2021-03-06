/**
 * @brief the built-in class interface
 * @details See doc/BCI.txt
 **/
#ifndef __BCI_CLASS_H_PREV__
#define __BCI_CLASS_H_PREV__
typedef struct _bci_class_t bci_class_t;
typedef struct _bci_class_wrap_t bci_class_wrap_t;
#endif /*__BCI_CLASS_H_PREV__*/ 

#include <constants.h>
#include <const_assertion.h>

#include <log.h>
#include <dalvik/dalvik_type.h>
#include <cesk/cesk_set.h>
#include <cesk/cesk_store.h>
#include <bci/bci_interface.h>
#include <tag/tag.h>

#ifndef __BCI_CLASS_H__
#define __BCI_CLASS_H__

#define BCI_BOOLEAN_FALSE 0
#define BCI_BOOLEAN_TRUE  1
#define BCI_BOOLEAN_UNKNOWN 2

#define BCI_CLASS_METHOD_CONST 0x40000000ul
#define BCI_CLASS_METHOD_IS_CONST(id) (BCI_CLASS_METHOD_CONST & (id))

/**
 * @brief the wrapper type for a built-in class
 **/
struct _bci_class_wrap_t {
	const char* path;          /*!< the class path */
	const bci_class_t* class;  /*!< the actual class def */
};

/**
 * @brief the buildin method definition
 **/
struct _bci_class_t {
	uint32_t size;/*!< the size of data section */
	
	int (*load)();/*!< actions after this class is loaded from the package return value < 0 means error */
	
	int (*unload)();/*!< actions before this class remove from the name table return value < 0 means error */
	
	int (*initialize)(void* this, const char * classpath, const void* init_param, tag_set_t** p_tags); /*!< how to initialize the data used by this instance, this is NOT CONSTRUCTOR return value < 0 means error, tags is the pointer to the reference to the tag set of this value, this function is resposible for the tag set changes caused by object creation */
	
	int (*finalize)(void* this);/*!< how to clean up this instance, NOT DESTRUCTOR return value < 0 means error*/

	int (*duplicate)(const void* this, void* that);/*!< how to make a duplicate return value < 0 means error*/

	cesk_set_t* (*get)(const void* this, const char* fieldname);/*!< callback that get a pointer to a field return an *NEW* set contains the field*/
	
	int (*put)(void* this, const char* fieldname, const cesk_set_t* set, cesk_store_t* store, int keep); /*!< set the field value */

	int (*read)(const void* this, uint32_t offset, uint32_t* buf,size_t sz);/*!< get the reference list address, return the number of address copied to buffer, < 0 when error */

	int (*write)(void* this, uint32_t offset, uint32_t* new, size_t N);  /*!< how to modify the address list */
	
	hashval_t (*hash)(const void* this);/*!< return the hashcode of this object */

	int (*equal)(const void* this, const void* that);/*!< if two instance are the same */

	const char* (*to_string)(const void* this, char* buf, size_t size);/*!< convert this object instance to string */
	
	int (*has_reloc_ref)(const void* this);/*!< how to get the relocation flag for this object */

	int (*merge)(void* this, const void* that);  /*!< merge the two built-in instances, if return value > 0, caller should merge the super class */


	int (*is_instance)(const void* this, const dalvik_type_t* classpath);     /*!< check wether or not this object is a instance of the class path */

	int (*get_method)(const void* this,
					  const char* classpath,
	                  const char* method,
	                  const dalvik_type_t * const * typelist,
					  const dalvik_type_t*  rtype);    /*!< return the method id if the highest bit is 1 indicates that this method won't modify 
														*  the object. if this class can not handle this function call, return value should be < 0 */

	int (*invoke)(int method_id, bci_method_env_t* env); /*< invoke a method sepecified by method_id using env as envrionment */

	const char* super;                           /*!< the super class of this built-in class, NULL means no super class */
	
	const char* provides[BCI_CLASS_MAX_PROVIDES];/*!< the class that this build-in class provides, end with a NULL */
}; 

/**
 * @brief initialize a built-in instance
 * @param mem the memory for this instance
 * @param class the class def
 * @param classpath the class path 
 * @param init_param the initialzation parameter
 * @param p_tags the pointer to the reference to the tag set of this value, used to modify the tag
 * @param class the class path
 * @return result of initialization, < 0 indicates an error
 **/
int bci_class_initialize(void* mem, const char* classpath, const void* init_param, tag_set_t** p_tags, const bci_class_t* class);
/**
 * @brief do cleanup before this instance finally get deleted
 * @param mem the memory for this instance
 * @param class the bci class
 * @return < 0 if there's any error
 **/
int bci_class_finalize(void* mem, const bci_class_t* class);
/**
 * @brief get the value of the field
 * @param this the object memory
 * @param fieldname the name of the field
 * @param class the class def
 * @return the result address set of this operation(newly created object, so need to free by caller)
 **/
cesk_set_t* bci_class_get(const void* this, const char* fieldname, const bci_class_t* class);
/**
 * @brief set the value of the field
 * @param this the object memory
 * @param fieldname the fieldname
 * @param set the new address of the value
 * @param store the store contains this object, cuz we need to maintan the refcnt
 * @param keep if keep == 1, means keep the old value no matter this value has been reused
 * @param class the class def
 * @return the result of this operation
 **/
int bci_class_put(void* this, const char* fieldname, const cesk_set_t* set, cesk_store_t* store, int keep, const bci_class_t* class);

/**
 * @brief get an address list that used by this object 
 * @param this the object
 * @param offset the offset of the start point
 * @param buf the buffer
 * @param sz the buffer size
 * @param class the class def
 * @return the value copied to the buffer < 0 means error
 **/
int bci_class_read(const void* this, uint32_t offset, uint32_t* buf, size_t sz, const bci_class_t* class);

/**
 * @brief return the hashcode of an given built-in instance
 * @param this the object
 * @param class the class def
 * @return the hash code
 **/
hashval_t bci_class_hashcode(const void* this, const bci_class_t* class);

/**
 * @brief duplicate a object 
 * @param this the memory for the source instance
 * @param that the memory for the duplication
 * @param class the class def
 * @return result of copy
 **/
int bci_class_duplicate(const void* this, void* that, const bci_class_t* class);

/**
 * @brief if two object are equal
 * @note this function do not means the comparasion function, the result of this function
 *       is not equal to the compare function in java language, it's means this two object
 *       ARE ACTUALLY THE SAME THING IN DIFFIERENT STORES
 * @param this the left operand
 * @param that the right operand
 * @param class the class def
 * @return result of comparasion, 1 indicates equal, 0 means non-equal, < 0 indicates error
 **/
int bci_class_equal(const void* this, const void* that, const bci_class_t* class);

/**
 * @brief convert an object instance to string
 * @param this the object instance
 * @param buf the output buffer, if the buffer is NULL means use default buffer
 * @param size the buffer size
 * @param class the class def
 * @return the converted string, if it's NULL, an error is happend
 **/
const char* bci_class_to_string(const void* this, char* buf, size_t size, const bci_class_t* class);

/**
 * @brief return wether or not this object contains a relocated address
 * @param this the object instance
 * @param class the built-in class
 * @return the relocation flag bit, < 0 indicates an error
 **/
int bci_class_has_reloc_ref(const void* this, const bci_class_t* class);

/**
 * @brief merge two built-in instances
 * @param this the 'this' pointer
 * @param that the second operand
 * @param class the class def
 * @return < 0 ==> errors
 **/
int bci_class_merge(void* this, const void* that, const bci_class_t* class);

/**
 * @brief modify the address lit
 * @param this the 'this' pointer
 * @param offset the offset of the first address we want to modify
 * @param new the new address list
 * @param N how many new address in the list
 * @param class the class def
 * @return < 0 indicates errors
 **/
int bci_class_write(void* this, uint32_t offset, uint32_t* new, size_t N, const bci_class_t* class);

/**
 * @brief check if this object is a instance of the given classpath
 * @param this the 'this pointer
 * @param type the type descriptor
 * @param class the class def
 * @return 0 = negative 1 = positive < 0 means error
 **/
int bci_class_is_instance(const void* this, const dalvik_type_t* type, const bci_class_t* class);

/**
 * @brief get method id by method name, for static function, the this pointer should be NULL
 * @param this the this pointer
 * @param classpath the classpath of the class
 * @param methodname the methodname to get
 * @param class the class def
 * @param rtype the return value type
 * @param typelist the argument list or function signature
 * @return the method id, < 0 indicates an error / method not found
 **/
int bci_class_get_method(const void* this, const char* classpath, const char* methodname, 
                         const dalvik_type_t * const * typelist, 
						 const dalvik_type_t* rtype, const bci_class_t* class);

/**
 * @brief invoke a built-in method
 * @param method_id the id of the method to invoke
 * @param env the invoke environ
 * @param class the class def
 * @return the invoke result, < 0 indicates an error
 **/
int bci_class_invoke(int method_id, bci_method_env_t* env, const bci_class_t* class);
#endif
