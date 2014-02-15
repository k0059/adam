#include <string.h>

#include <log.h>
#include <cesk/cesk_set.h>
#define CESK_SET_INVALID (~0u)
/* We do not maintain a hash table for each set, because
 * most set contains a few element. 
 * Instead of maintaining a hash table for each set,
 * we maintain a large hash table use <set_idx, address>
 * as key. 
 */
struct _cesk_set_t {
    uint32_t set_idx;    /* the set index */
};
typedef struct _cesk_set_node_t cesk_set_node_t;
/* data that the some set holds */
typedef struct {
    cesk_set_node_t *next;
} cesk_set_data_entry_t;;
typedef struct {
    uint32_t size;          /* how many element in the set */
    uint32_t refcnt;        /* the reference count of this, indicates how many cesk_set_t for this set are returned */
    cesk_set_node_t* first; /* first element of the set */
} cesk_set_info_entry_t;
struct _cesk_set_node_t {
    uint32_t set_idx;       /* the set index */
    uint32_t addr;          /* the address this data entry refer */
    /* this pointer is for the hash table */
    cesk_set_node_t *next;
    cesk_set_node_t *prev;
    /* the following space is for the actuall data */
    char data_section[0]; /* the data section of this node */
    cesk_set_data_entry_t data_entry[0];   /* this is valid for a data entry node */
    cesk_set_info_entry_t info_entry[0];   /* this is valid for an info entry node */
};

struct _cesk_set_iter_t{
    cesk_set_node_t *next;
};
static cesk_set_node_t* _cesk_set_hash[CESK_SET_HASH_SIZE];

#define DATA_ENTRY 0
#define INFO_ENTRY 1

#define INFO_ADDR CESK_STORE_ADDR_NULL  /* this is a dumb address to distingush between data_entry and info_entry */

static inline cesk_set_node_t* _cesk_set_node_alloc(int type)
{
    size_t size = sizeof(cesk_set_node_t);
    switch(type)
    {
        case DATA_ENTRY:
            size += sizeof(cesk_set_data_entry_t);
            break;
        case INFO_ENTRY:
            size += sizeof(cesk_set_info_entry_t);
            break;
        default:
            LOG_ERROR("unknown type %d", type);
            return NULL;
    }
    cesk_set_node_t* ret = (cesk_set_node_t*)malloc(size);
    if(NULL == ret)
    {
        LOG_ERROR("can not allocate memory for node");
        return NULL;
    }
    memset(ret, 0, size);
    return ret;
}

static inline uint32_t _cesk_set_idx_hashcode(uint32_t hashidx, uint32_t addr)
{
    return (hashidx * MH_MULTIPLY) ^ ((addr & 0xffff) * MH_MULTIPLY) ^ (addr >> 16);
}
/* the function will insert a node in the hash table regardless if it's duplicated
 * The return value of the function is the header address of data section
 */
static inline void* _cesk_set_hash_insert(uint32_t setidx, uint32_t addr)
{
    /* if addr == CESK_STORE_ADDR_NULL, the node is a info node */
    uint32_t h = _cesk_set_idx_hashcode(setidx, addr) % CESK_SET_HASH_SIZE;
    int type = DATA_ENTRY;
    if(addr == CESK_STORE_ADDR_NULL) type = INFO_ENTRY;
    cesk_set_node_t* ret = _cesk_set_node_alloc(type);
    if(NULL == ret) return NULL;
    ret->prev = NULL;
    ret->next = _cesk_set_hash[h];
    ret->set_idx = setidx;
    ret->addr = addr;
    if(_cesk_set_hash[h]) _cesk_set_hash[h]->prev = ret;
    _cesk_set_hash[h] = ret;
    return ret->data_section;
}
/* look for a value in the hash table, return the address of data section */
static inline void* _cesk_set_hash_find(uint32_t setidx, uint32_t addr)
{
    uint32_t h = _cesk_set_idx_hashcode(setidx, addr) % CESK_SET_HASH_SIZE;
    cesk_set_node_t *p;
    for(p = _cesk_set_hash[h]; p != NULL; p = p->next)
    {
        if(p->set_idx == setidx &&
           p->addr    == addr)
         {
             LOG_DEBUG("find set hash entry (%d %d)", setidx, addr);
             return p->data_section;
         }
    }
    LOG_TRACE("can not find the set hash entry (%d, %d)", setidx, addr);
    return NULL;
}
/* allocate a fresh set index */
static inline uint32_t _cesk_set_idx_alloc(cesk_set_info_entry_t** p_entry)
{
    static uint32_t next_idx = 0;
    cesk_set_info_entry_t* entry = (cesk_set_info_entry_t*)_cesk_set_hash_insert(next_idx, CESK_STORE_ADDR_NULL);
    if(NULL == entry)
    {
        LOG_ERROR("can not insert the set info to hash table");
        return CESK_SET_INVALID;
    }
    entry->size = 0;
    entry->refcnt = 0;
    entry->first = NULL;
    *(p_entry) = entry;
    return next_idx ++;
}
static cesk_set_t* _cesk_empty_set;   /* this is the only empty set in the table */
void cesk_set_init()
{
    /* make the constant empty set */
    _cesk_empty_set = (cesk_set_t*)malloc(sizeof(cesk_set_t));
    if(NULL == _cesk_empty_set)
    {
        LOG_FATAL("can not allocate memory for the constant set {}");
        return;
    }
    cesk_set_info_entry_t* entry = NULL;
    _cesk_empty_set->set_idx = _cesk_set_idx_alloc(&entry);
    if(NULL == entry)
    {
        LOG_FATAL("invalid hash info entry");
        return;
    }
    entry->refcnt ++;
}
void cesk_set_finalize()
{
    /* free all memory in the hash table */
    int i;
    for(i = 0; i < CESK_SET_HASH_SIZE; i ++)
    {
        cesk_set_node_t *p;
        for(p = _cesk_set_hash[i]; NULL != p; )
        {
            cesk_set_node_t *old = p;
            p = p->next;
            free(old);
        }
    }
    free(_cesk_empty_set);
}
void cesk_set_finalize();
/* fork a set */
cesk_set_t* cesk_set_fork(cesk_set_t* sour)
{
    if(NULL == sour) return NULL;
    /* verify if the set exists */
    cesk_set_info_entry_t* info = (cesk_set_info_entry_t*) _cesk_set_hash_find(sour->set_idx, CESK_STORE_ADDR_NULL);
    if(NULL == info)
    {
        LOG_ERROR("the set does not exist");
        return NULL;
    }
    cesk_set_t* ret = (cesk_set_t*)malloc(sizeof(cesk_set_t));
    if(NULL == ret) return NULL;
    /* set the set index */
    ret->set_idx = sour->set_idx;
    /* update the reference counter */
    info->refcnt ++;
    return ret;
}
/* make an empty set */
cesk_set_t* cesk_set_empty_set()
{
    return cesk_set_fork(_cesk_empty_set);   /* fork the empty set to user */
}

size_t cesk_set_size(cesk_set_t* set)
{
    if(NULL == set) return 0;
    cesk_set_info_entry_t* info = (cesk_set_info_entry_t*)_cesk_set_hash_find(set->set_idx, CESK_STORE_ADDR_NULL);
    if(NULL == info) return 0;
    return info->size;
}
void cesk_set_free(cesk_set_t* set)
{
    if(NULL == set) return;
    cesk_set_info_entry_t* info = (cesk_set_info_entry_t*)_cesk_set_hash_find(set->set_idx, CESK_STORE_ADDR_NULL);
    if(NULL == info) return;
    if(0 == --info->refcnt)
    {
        /* the reference counter is reduced to zero, no one is using this */
        cesk_set_node_t* info_node = (cesk_set_node_t*)(((char*)info) - sizeof(cesk_set_node_t));
        cesk_set_node_t* data_node;
        for(data_node = info->first; data_node != NULL;)
        {
            if(NULL != data_node->prev) 
                data_node->prev->next = data_node->next;
            else 
            {
                /* first element of the slot */
                uint32_t h = _cesk_set_idx_hashcode(data_node->set_idx, data_node->addr) % CESK_SET_HASH_SIZE;
                _cesk_set_hash[h] = data_node->next;
            }
            if(NULL != data_node->next) 
                data_node->next->prev = data_node->prev;
            cesk_set_node_t* tmp = data_node;
            data_node = data_node->next;
            free(tmp);
        }
        if(NULL != info_node->prev)
            info_node->prev->next = info_node->next;
        else
        {
            uint32_t h = _cesk_set_idx_hashcode(info_node->set_idx, CESK_STORE_ADDR_NULL) % CESK_SET_HASH_SIZE;
            _cesk_set_hash[h] = info_node->next;
        }
        if(NULL != info_node->next)
            info_node->next->prev = info_node->prev;
        free(info_node);
    }
    free(set);
    return;
}

cesk_set_iter_t* cesk_set_iter(cesk_set_t* set, cesk_set_iter_t* buf)
{
    if(NULL == set || NULL == buf) 
    {
        LOG_ERROR("invalid argument");
        return NULL;
    }
    cesk_set_info_entry_t* info = (cesk_set_info_entry_t*)_cesk_set_hash_find(set->set_idx, CESK_STORE_ADDR_NULL);
    if(NULL == info)
    {
        LOG_ERROR("can not find the set (id = %d)", set->set_idx);
        return NULL;
    }
    buf->next = info->first;
    return buf;
}

uint32_t cesk_set_iter_next(cesk_set_iter_t* iter)
{
    if(NULL == iter) return CESK_STORE_ADDR_NULL;
    if(NULL == iter->next) return CESK_STORE_ADDR_NULL;
    uint32_t ret = iter->next->addr;
    iter->next = iter->next->next;
    return ret;
}
/* for performance reason, we also maintain reference counter here
 * Because we assume the caller always returns a new set rather than 
 * the old one, if this function is called
 */
static inline uint32_t _cesk_set_duplicate(cesk_set_info_entry_t* info, cesk_set_info_entry_t** new)
{
    //cesk_set_info_entry_t* info = (cesk_set_info_entry_t*)_cesk_set_hash_find(idx, CESK_STORE_ADDR_NULL);
    uint32_t idx;
    idx = ((cesk_set_node_t*)(((char*)info) - sizeof(cesk_set_node_t)))->set_idx;
    
    if(NULL == info)
    {
        LOG_ERROR("can not find the set (idx = %d)", idx);
        return CESK_SET_INVALID;
    }
    cesk_set_info_entry_t* new_info;
    uint32_t new_idx = _cesk_set_idx_alloc(&new_info);
    if(NULL == new_info) 
    {
        LOG_ERROR("can not create a new set");
        return CESK_SET_INVALID;
    }
    new_info->size = info->size;

    new_info->refcnt = 1;
    info->refcnt --;

    new_info->first = NULL;

    /* duplicate the elements */
    cesk_set_node_t* ptr;
    for(ptr = info->first; NULL != ptr; ptr = ptr->data_entry->next)
    {
        uint32_t addr = ptr->addr;
        /* 
         * we do not need to check if the element is in the set, because the set is 
         * newly created and must be empty 
         */
        cesk_set_node_t* node = (cesk_set_node_t*)(((char*)_cesk_set_hash_insert(new_idx, addr)) - sizeof(cesk_set_node_t));
        node->data_entry->next = new_info->first;
        new_info->first = node;
    }

    if(NULL != new) (*new) = new_info;

    return new_idx;
}
int cesk_set_push(cesk_set_t* dest, uint32_t addr)
{
    if(NULL == dest || CESK_STORE_ADDR_NULL == addr)
        return -1;
    if(CESK_SET_INVALID == dest->set_idx)
        return -1;
    cesk_set_info_entry_t *info;
    info = (cesk_set_info_entry_t*)_cesk_set_hash_find(dest->set_idx, CESK_STORE_ADDR_NULL);
    if(NULL == info)
    {
        LOG_ERROR("can not find set #%d", dest->set_idx);
        return -1;
    }
    if(info->refcnt > 1)
    {
        LOG_DEBUG("set #%d is refered by %d set objects, duplicate before writing", 
                   dest->set_idx,
                   info->refcnt);
        cesk_set_info_entry_t *new;
        uint32_t idx = _cesk_set_duplicate(info, &new);
        if(CESK_SET_INVALID == idx)
        {
            LOG_ERROR("error during duplicate the set");
            return -1;
        }
        dest->set_idx = idx;
        info = new;
    }
    /* here we need to check the duplicate element, because 
     * the set is not empty and might contains the same element
     * already
     */
    if(NULL == _cesk_set_hash_find(dest->set_idx, addr))
    {
        /* if this is not a duplicate */
        cesk_set_node_t* node = (cesk_set_node_t*)(((char*)_cesk_set_hash_insert(dest->set_idx, addr)) - sizeof(cesk_set_node_t));
        node->data_entry->next = info->first;
        info->first = node;
        info->size ++;
    }
    return 0;
}
int cesk_set_join(cesk_set_t* dest, cesk_set_t* sour)
{
    if(NULL == sour || NULL == dest) return -1;
    if(CESK_SET_INVALID == dest->set_idx ||
       CESK_SET_INVALID == sour->set_idx)
        return -1;
    cesk_set_info_entry_t *info_dst;
    info_dst = (cesk_set_info_entry_t*)_cesk_set_hash_find(dest->set_idx, CESK_STORE_ADDR_NULL);
    if(NULL == info_dst) 
    {
        LOG_ERROR("can not find set #%d", dest->set_idx);
        return -1;
    }
    cesk_set_info_entry_t *info_src;
    info_src = (cesk_set_info_entry_t*)_cesk_set_hash_find(sour->set_idx, CESK_STORE_ADDR_NULL);
    if(NULL == info_src)
    {
        LOG_ERROR("can not find set #%d", sour->set_idx);
        return -1;
    }
    if(info_dst->refcnt > 1)
    {
        LOG_DEBUG("set #%d is refered by %d set objects, duplicate it before merging", dest->set_idx, info_dst->refcnt);
        cesk_set_info_entry_t *new;
        uint32_t idx = _cesk_set_duplicate(info_dst, &new);
        if(CESK_SET_INVALID == idx)
        {
            LOG_ERROR("error during duplicate the set");
            return -1;
        }
        dest->set_idx = idx;
        info_dst = new;
    }
    cesk_set_node_t* ptr;
    for(ptr = info_src->first; NULL != ptr; ptr = ptr->data_entry->next)
    {
        uint32_t addr = ptr->addr;
        if(NULL == _cesk_set_hash_find(dest->set_idx, addr))
        {
            /* if this is not a duplicate */
            cesk_set_node_t* node = (cesk_set_node_t*)(((char*)_cesk_set_hash_insert(dest->set_idx, addr)) - sizeof(cesk_set_node_t));
            node->data_entry->next = info_dst->first;
            info_dst->first = node;
            info_dst->size ++;
        }
    }
    return 0;
}
int cesk_set_contain(cesk_set_t* set, uint32_t addr)
{
    if(NULL == set) return 0;
    if(addr == CESK_STORE_ADDR_NULL) return 0;
    return (NULL != _cesk_set_hash_find(set->set_idx, addr));
}