#include <dalvik/sexp.h>
const char* sexpr_keywords[SEXPR_MAX_NUM_KEYWORDS];

const char* _sexpr_keywords_defs[SEXPR_MAX_NUM_KEYWORDS] = {
    NULL
}; 

static inline sexpression_t* _sexpr_alloc(int type)
{
    size_t size = sizeof(sexpression_t);
    switch(type)
    {
        case SEXP_TYPE_LIT:
            size += sizeof(sexp_lit_t);
            break;
        case SEXP_TYPE_STR:
            size += sizeof(sexp_str_t);
            break;
        case SEXP_TYPE_CONS:
            size += sizeof(sexp_cons_t);
            break;
        default:
            return NULL;
    }
    return (sexpression_t*) malloc(sizeof(sexpression_t));
}
void sexp_free(sexpression_t* buf)
{
    sexp_cons_t* cons_data;
    if(SEXP_NIL == buf)  return; /* A empty S-Expression */
    switch(buf->type)
    {
        case SEXP_TYPE_CONS:     /* A Cons S-Expression, free the memory recursively */
            cons_data = (sexp_cons_t*) buf->data;
            sexp_free(cons_data->first);
            sexp_free(cons_data->second);
        default:
            free(buf);
    }
}
int sexp_init(void)
{
    int i;
    for(i = 0; _sexpr_keywords_defs[i]; i ++)
        sexpr_keywords[i] = stringpool_query(_sexpr_keywords_defs[i]);
    return 0;
}
const char* sexp_parse(const char* str, sexpression_t** buf)
{

}
