#include <string.h>
#include <assert.h>

#include <log.h>
#include <vector.h>

#include <dalvik/dalvik_block.h>

typedef struct _dalvik_block_cache_node_t{
    const char*     methodname;
    const char*     classpath;
    dalvik_block_t* block;
    struct _dalvik_block_cache_node_t * next;
} dalvik_block_cache_node_t;

static dalvik_block_cache_node_t* _dalvik_block_cache[DALVIK_BLOCK_CACHE_SIZE];

/* allocate a node refer to the block */
static inline dalvik_block_cache_node_t* _dalvik_block_cache_node_alloc(const char* class, const char* method, dalvik_block_t* block)
{
    if(NULL == block ||
       NULL == class ||
       NULL == block )
    {
        return NULL;
    }
    dalvik_block_cache_node_t* ret = (dalvik_block_cache_node_t*) malloc(sizeof(dalvik_block_cache_node_t));
    if(NULL == ret) 
    {
        LOG_ERROR("can not allocate memory");
        return NULL;
    }
    ret->block = block;
    ret->next = NULL;
    return ret;
}
/* tranverse the graph */
static inline void _dalvik_block_tranverse_graph(dalvik_block_t* node, vector_t* result)
{
    if(NULL == node ||
       NULL == result ||
       DALVIK_BLOCK_INDEX_INVALID == node->index) 
        return;
    vector_pushback(result, &node); 
    node->index = DALVIK_BLOCK_INDEX_INVALID;  /* mark the node visited */
    int i;
    for(i = 0; i < node->nbranches; i ++)
        _dalvik_block_tranverse_graph(node->branches[i].block, result);
}
/* from the entry point, free the block graph */
static inline void _dalvik_block_graph_free(dalvik_block_t* entry)
{
    
    vector_t* vec = vector_new(sizeof(dalvik_block_t*));
    _dalvik_block_tranverse_graph(entry, vec);
    int i;
    for(i = 0; i < vector_size(vec); i ++)
    {
        dalvik_block_cache_node_t* node = *(dalvik_block_cache_node_t**)vector_get(vec, i);
        //TODO: free node->block
        LOG_INFO("fixme: free node->block");
        free(node);
    }
    vector_free(vec);
}
static inline hashval_t _dalvik_block_hash(const char* class, const char* method)
{
    return 
            (((uintptr_t)class & 0xffffffffull) * MH_MULTIPLY) ^ 
            (((uintptr_t)method & 0xffff) * MH_MULTIPLY) ^ 
            ((uintptr_t)method >> 16) ^ 
            ~((uintptr_t)class>>((sizeof(uintptr_t)/2)));
}
void dalvik_block_init()
{
    memset(_dalvik_block_cache, 0, sizeof(_dalvik_block_cache));
}
void dalvik_block_finalize()
{
    int i;
    for(i = 0; i < DALVIK_BLOCK_CACHE_SIZE; i ++)
    {
        dalvik_block_cache_node_t* p;
        dalvik_block_t* graph_entry;
        for(p = _dalvik_block_cache[i]; NULL != p;)
        {
            dalvik_block_cache_node_t* tmp = p;
            p = p->next;
            graph_entry = tmp->block;
            _dalvik_block_graph_free(graph_entry);
            free(tmp);
        }
    }
}
/* Sort the instruction by the offset. this order is equavalient to the 
 * executing order without branch. Because parser always parse the instruction
 * one by one 
 */
static int _dalvik_block_key_comp(const void* this, const void* that)
{
    return (*(int*)this) - (*(int*)that);
}
/* Tranverse all instructions in this method, figure out which is key instruction that we really take care
 * and return the offset of the address in the array.
 */
static inline int _dalvik_block_get_key_instruction_list(uint32_t entry_point, uint32_t* key, size_t size)
{
    uint32_t kcnt = 0;
    uint32_t inst;
    dalvik_instruction_t * current_inst = NULL;
#define __PUSH(value) do{\
    if(kcnt >= size)\
    {\
        LOG_ERROR("excepting %d key instructions, got more than that. Try to adjust constant DALVIK_BLOCK_MAX_KEYS");\
        return 0;\
    }\
    key[kcnt ++] = (value);\
    LOG_DEBUG("new branch %d --> %d", inst, key[kcnt - 1]);\
}while(0)
#define __PUSH_LABEL(label) __PUSH(dalvik_label_jump_table[(label)])
    for(inst = entry_point; DALVIK_INSTRUCTION_INVALID != inst; inst = current_inst->next)
    {
         current_inst = dalvik_instruction_get(inst);
         switch(current_inst->opcode)
         {
             static char buf[1024];
             LOG_DEBUG("current instruction: %s", dalvik_instruction_to_string(current_inst, buf, sizeof(buf)));
             case DVM_GOTO:
                 LOG_DEBUG("key instruction: #%d: opcode=GOTO flag=%x num_operands=%d", 
                            inst, 
                            current_inst->flags, 
                            current_inst->num_operands);
                 /* append the instruction to the key list */
                 __PUSH(inst);
                 __PUSH_LABEL(current_inst->operands[0].payload.labelid);
                 break;
             case DVM_IF:
                 LOG_DEBUG("key instruction: #%d: opcode=IF flag=%x num_operands=%d",
                           inst,
                           current_inst->flags,
                           current_inst->num_operands);
                 __PUSH(inst);
                 /* if it's not the last instruction, this is one possible routine */
                 if(current_inst->next != DALVIK_INSTRUCTION_INVALID)
                     __PUSH(current_inst->next);
                 __PUSH(current_inst->operands[2].payload.labelid);
                 break;
             case DVM_SWITCH:
                 LOG_DEBUG("key instruction: #%d: opcode=SWITCH flag=%x num_operands=%d",
                           inst,
                           current_inst->flags,
                           current_inst->num_operands);
                 __PUSH(inst);
                 if(DVM_FLAG_SWITCH_PACKED == current_inst->flags)
                 {
                     /* packed switch, use a label vector */
                     int i;
                     vector_t* lv = current_inst->operands[2].payload.branches;
                     if(NULL == lv)
                     {
                         LOG_ERROR("invalid operand");
                         return -1;
                     }
                     size_t size = vector_size(lv);
                     
                     for(i = 0; i < size; i ++)
                         __PUSH_LABEL(*(int*)vector_get(lv,i));
                 }
                 else if(DVM_FLAG_SWITCH_SPARSE == current_inst->flags)
                 {
                     /* sparse swith */
                     int i;
                     vector_t* lv = current_inst->operands[2].payload.sparse;
                     if(NULL == lv)
                     {
                         LOG_ERROR("invalid operand");
                         return -1;
                     }
                     size_t size = vector_size(lv);
                     for(i = 0; i < size; i ++)
                     {
                        dalvik_sparse_switch_branch_t* branch;
                        branch = (dalvik_sparse_switch_branch_t*) vector_get(lv, i);
                        __PUSH_LABEL(branch->labelid);
                     }
                 }
                 else
                     LOG_ERROR("invalid instruction switch(flags = %d)", current_inst->flags);
                 break;
             case DVM_INVOKE:
                 LOG_DEBUG("key instruction: #%d: opcode=INVOKE, flags=%x, noperands=%d", 
                           inst,
                           current_inst->flags,
                           current_inst->num_operands);
                 __PUSH(inst);
                 if(current_inst->next != DALVIK_INSTRUCTION_INVALID)
                     __PUSH(current_inst->next);
                 break;
             case DVM_RETURN:
                 LOG_DEBUG("key instruction #%d: opcode = RETURN, flags = %d, noperands = %d",
                            inst,
                            current_inst->flags,
                            current_inst->num_operands);
                 __PUSH(inst);
                 /* there's no next instruction actually */
                 break;
             default:
                 LOG_DEBUG("not a key instruction opcode=%d, flags=%d, noperands=%d",
                           current_inst->opcode,
                           current_inst->flags,
                           current_inst->num_operands);
         }
    }

#undef __PUSH
#undef __PUSH_LABEL

    if(0 == kcnt)
    {
        LOG_WARNING("we can not find any key instruction here");
        return 0;
    }
    /* sort all key instructions */
    qsort(key, kcnt, sizeof(uint32_t), _dalvik_block_key_comp);
    /* make the instruction offset unique in the array */
    int next_pos = 1;
    int cur_pos = 1;

    for(; cur_pos < kcnt; cur_pos ++)
    {
        if(key[next_pos-1] == key[cur_pos]) continue;  /* just skip this, if it's a duplication of the prevoius one */
        key[next_pos++] = key[cur_pos];
    }
    kcnt = next_pos;
#if LOG_LEVEL >= 6
    LOG_DEBUG("%d key instructions in this function: ", kcnt);
    int i;
    for(i = 0; i < kcnt; i ++)
    {
        LOG_DEBUG("%d", key[i]);
    }
#endif  /* debuge infomation */
    return kcnt;
}
/* allocate a new code block with nbranches */
static inline dalvik_block_t* _dalvik_block_new(size_t nbranches)
{
    size_t size = sizeof(dalvik_block_t) + sizeof(dalvik_block_branch_t) * nbranches;
    dalvik_block_t* ret = (dalvik_block_t*)malloc(size);
    if(NULL == ret) return NULL;
    memset(ret, 0, size);
    ret->nbranches = nbranches;
    return ret;
}
static inline int32_t _dalvik_block_find_blockid_by_instruction(uint32_t inst, const uint32_t *key, int kcnt)
{
    int i;
    for(i = 0; i < kcnt; i ++)
        if(inst < key[i])  /* find the end point of the block */
            return i;
    return -1;
}
static inline dalvik_block_t* _dalvik_block_setup_keyinst_goto(dalvik_instruction_t* inst, const uint32_t* key, size_t kcnt, uint32_t index)
{
    dalvik_block_t* block = _dalvik_block_new(1);
    if(NULL == block)
    {
        LOG_ERROR("can not allocate memory for a goto blongck");
        return NULL;
    }
    uint32_t target = dalvik_label_jump_table[inst->operands[0].payload.labelid];
    block->index = index;
    block->branches[0].linked = 0;
    block->branches[0].conditional = 0;
    block->branches[0].block_id[0] = _dalvik_block_find_blockid_by_instruction(target, key, kcnt);
    LOG_DEBUG("possible path block %d --> block %d",  index, block->branches[0].block_id[0]);
    return block;
}
static inline dalvik_block_t* _dalvik_block_setup_keyinst_if(dalvik_instruction_t* inst, const uint32_t* key, size_t kcnt, uint32_t index)
{
    dalvik_block_t* block = _dalvik_block_new(2);
    if(NULL == block)
    {
        LOG_ERROR("can not allocate memory for a if block");
        return NULL;
    }
    block->index = index;
    /* setup the false branch */
    block->branches[1].conditional = 0;
    block->branches[1].linked = 0;
    if(inst->next == DALVIK_INSTRUCTION_INVALID)
    {
        block->branches[1].disabled = 1;
        LOG_WARNING("unexcepted goto instruction at the end of the method, disable the default branch");
    }
    else
    {
        block->branches[1].block_id[0] = _dalvik_block_find_blockid_by_instruction(inst->next, key, kcnt);
        LOG_DEBUG("possible path block %d --> block %d", index, block->branches[1].block_id[0]);
    }
    /* setup the true branch */
    uint32_t target = dalvik_label_jump_table[inst->operands[2].payload.labelid];
    block->branches[0].conditional = 1;
    block->branches[0].linked = 0;
    block->branches[0].block_id[0] = _dalvik_block_find_blockid_by_instruction(target, key, kcnt);
    block->branches[0].left = inst->operands + 0;
    block->branches[0].right = inst->operands + 1;
    
    /* set up the comparasion flags */
    if(inst->flags == DVM_FLAG_IF_EQ || 
       inst->flags == DVM_FLAG_IF_LE ||
       inst->flags == DVM_FLAG_IF_GE)
        block->branches[0].eq = 1;
    if(inst->flags == DVM_FLAG_IF_LE ||
       inst->flags == DVM_FLAG_IF_LT)
        block->branches[0].lt = 1;
    if(inst->flags == DVM_FLAG_IF_GE ||
       inst->flags == DVM_FLAG_IF_GT)
        block->branches[0].gt = 1;
    
    LOG_DEBUG("possible path block %d --> block %d",  index, block->branches[0].block_id[0]);
    return block;
}
static inline dalvik_block_t* _dalvik_block_setup_keyinst_switch(dalvik_instruction_t* inst, const uint32_t *key, size_t kcnt, uint32_t index)
{
    dalvik_block_t* block = NULL;

    if(DVM_FLAG_SWITCH_PACKED == inst->flags)
    {
        /* packed switch instruction */
        vector_t* branches = inst->operands[1].payload.branches;
        size_t nb = vector_size(branches);
        block = _dalvik_block_new(nb);
        if(NULL == block)
        {
            LOG_ERROR("can not allocate memory for a packed swith block");
            return NULL;
        }
        block->index = index;
        int32_t value_begin = inst->operands[1].payload.int32;
        int j;
        /* set up branches */
        for(j = 0; j < nb; j ++)
        {
            uint32_t target = *(uint32_t*)vector_get(branches, j);

            block->branches[j].conditional = 1;
            block->branches[j].linked      = 0;
            block->branches[j].block_id[0] = _dalvik_block_find_blockid_by_instruction(target, key, kcnt);
            block->branches[j].left_inst = 1;   /* use the instant field */
            block->branches[j].ileft[0] = j + value_begin;
            block->branches[j].right = inst->operands + 0;
            block->branches[j].eq = 1;
            LOG_DEBUG("possible path block %d --> %d", index, block->branches[j].block_id[0]);
        }
        block->branches[nb-1].conditional = 0;
    }
    else if(DVM_FLAG_SWITCH_SPARSE == inst->flags)
    {
        /* sparse switch instruction */
        vector_t* branches = inst->operands[1].payload.sparse;
        size_t nb = vector_size(branches);
        block = _dalvik_block_new(nb);
        if(NULL == block)
        {
            LOG_ERROR("can not allocate memory for sparse switch instruction");
            return 0;
        }
        block->index = index;
        int j;
        for(j = 0; j < nb; j ++)
        {
            dalvik_sparse_switch_branch_t* branch;
            branch = (dalvik_sparse_switch_branch_t*)vector_get(branches, j);
            block->branches[j].conditional = 1;
            block->branches[j].linked      = 0;
            block->branches[j].block_id[0] = _dalvik_block_find_blockid_by_instruction(branch->labelid, key, kcnt);
            if(branch->is_default)
            {
                /* this is default branch */
                block->branches[j].conditional = 0;
                block->branches[j].linked = 0;
            }
            else
            {
                /* conditionlal branch */
                block->branches[j].left_inst = 1;
                block->branches[j].ileft[0] = branch->cond;
                block->branches[j].right    = inst->operands + 1;
                block->branches[j].eq       = 1;
            }
        }
    }
    else 
    {
        LOG_ERROR("unknown flags %x for switch instruction", inst->flags);
        return NULL;
    }
    return block;
}
static inline dalvik_block_t* _dalvik_block_setup_keyinst(dalvik_instruction_t* inst, const uint32_t *key, size_t kcnt, uint32_t index)
{
    dalvik_block_t* block = _dalvik_block_new(1);  /* only 1 path is possible */
    if(NULL == block)
    {
        LOG_ERROR("can not allocate memory for the instruction");
        return NULL;
    }
    block->index = index;
    block->branches[0].conditional = 0;
    if(inst->next != DALVIK_INSTRUCTION_INVALID)
        block->branches[0].block_id[0] = _dalvik_block_find_blockid_by_instruction(inst->next, key, kcnt);
    else
    {
        LOG_ERROR("unexcepted instruction at the end of the method, branches disabled");
        block->branches[0].disabled = 1;
    }
    return block;
}
static inline int _dalvik_block_build_graph(uint32_t entry, dalvik_block_t** blocks, size_t sz, const uint32_t* key, size_t kcnt)
{
    uint32_t block_begin = entry;

    uint32_t i;
    for(i = 0; i < kcnt; i ++)
    {
        dalvik_instruction_t* inst = dalvik_instruction_get(key[i]);  /*get the key instruction */
        dalvik_block_t* block = NULL;
        uint32_t block_end;
        switch(inst->opcode)
        {
            case DVM_GOTO:
                block_end = key[i];   /* do not include the goto instruction */
                block = _dalvik_block_setup_keyinst_goto(inst, key, kcnt, i);
                break;
            case DVM_IF:
                /* if statement have two possible executing pathes */
                block_end = key[i];  /* the instruction also do not be counted as a instruction in te block */
                block = _dalvik_block_setup_keyinst_if(inst, key, kcnt, i);
                break;
            case DVM_SWITCH:
                block_end = key[i]; 
                block = _dalvik_block_setup_keyinst_switch(inst, key, kcnt, i);
                break;
            case DVM_INVOKE:  /* acutally invoke instruction is not a jump instruction */
            default:
                block_end = inst->next;  /* also incuding current instruction, cuz it does more than a jump instruction */
                block = _dalvik_block_setup_keyinst(inst, key, kcnt, i);
        }
        if(NULL == block)
        {
            LOG_ERROR("can not create block for instruction from %d to %d", block_begin, block_end);
            goto ERROR;
        }
        
        /* set up the range of the code block */
        
        block->begin = block_begin;
        block->end   = block_end;
        blocks[i] = block;

        /* prepare for next block */
        block_begin = inst->next;

        /* the first key >= block_begin is actually the begnining of next block.
         * So after this loop, i ++, so that key[i] will be the end of next block
         */
        for(; block_begin < key[i]; i ++);
    }
    return 0;
    //TODO: link all graph
ERROR:
    for(i = 0; i < kcnt; i ++)
        if(blocks[i] == NULL)
            free(blocks[i]);
    return -1;
}
dalvik_block_t* dalvik_block_from_method(const char* classpath, const char* methodname, dalvik_type_t * const * typelist)
{
    if(NULL == classpath || NULL == methodname)
    {
        LOG_ERROR("class path and method name can not be true");
        return NULL;
    }
    LOG_DEBUG("get block graph of method %s/%s", classpath, methodname);
    hashval_t h = _dalvik_block_hash(classpath, methodname) % DALVIK_BLOCK_CACHE_SIZE;
    /* try to find the block graph in the cache */
    dalvik_block_cache_node_t* p;
    for(p = _dalvik_block_cache[h]; NULL != p; p = p->next)
    {
        if(p->methodname == methodname &&
           p->classpath  == classpath)
        {
            LOG_DEBUG("found the block graph in cache!");
            return p->block;
        }
    }
    /* there's no graph for this method in the cache, genterate one */
    dalvik_method_t* method = dalvik_memberdict_get_method(classpath, methodname, typelist);
    if(NULL == method) 
    {
        LOG_ERROR("can not find method %s/%s", classpath, methodname);
        return NULL;
    }
    LOG_DEBUG("find method %s/%s, entry point@%x", classpath, methodname, method->entry);

    uint32_t key[DALVIK_BLOCK_MAX_KEYS];     /* the maximum key instruction we are expecting */
    dalvik_block_t* blocks[DALVIK_BLOCK_MAX_KEYS] = {}; /* block list */ 
    
    /* Get a list of key instructions */
    uint32_t kcnt = _dalvik_block_get_key_instruction_list(method->entry, key, sizeof(key)/sizeof(*key));

    if(_dalvik_block_build_graph(method->entry, blocks, DALVIK_BLOCK_MAX_KEYS, key, kcnt) < 0)
    {
        LOG_ERROR("can not build block graph for method %s/%s", classpath, methodname);
        return NULL;
    }

    /* Ok, link the program */
    int i;
    for(i = 0; i < kcnt; i ++)
    {
        if(blocks[i] != NULL)
        {
            int j;
            for(j = 0; j < blocks[i]->nbranches; j ++)
                blocks[i]->branches[j].block = blocks[blocks[i]->branches[j].block_id[0]];
        }
    }

    return blocks[0];
}

