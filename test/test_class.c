<<<<<<< HEAD
=======
#include <adam.h>
>>>>>>> rename to adam
#include <dalvik/dalvik_class.h>
const char input[] = 
"(class (attrs ) antlr/TokenRangeElement\n"
" (super antlr/AlternativeElement)\n"
" (source \"TokenRangeElement.java\")\n"
"\n"
" (field (attrs protected ) begin int )\n"
" (field (attrs protected ) beginText [object java/lang/String] )\n"
" (field (attrs protected ) end int )\n"
" (field (attrs protected ) endText [object java/lang/String] )\n"
" (field (attrs ) label [object java/lang/String] )\n"
"\n"
"(method (attrs public ) <init>([object antlr/Grammar] [object antlr/Token] [object antlr/Token] int )void\n"
"  (limit registers 7)\n"
"  ; this: v2 ([object antlr/TokenRangeElement])\n"
"  ; parameter[0] : v3 ([object antlr/Grammar])\n"
"  ; parameter[1] : v4 ([object antlr/Token])\n"
"  ; parameter[2] : v5 ([object antlr/Token])\n"
"  ; parameter[3] : v6 (int)\n"
"  (const/4 v0 0)\n"
"  (line 18)\n"
"  (invoke-direct {v2 v3 v4 v6} antlr/AlternativeElement/<init> [object antlr/Grammar] [object antlr/Token] int )\n"
"  (line 12)\n"
"  (iput v0 v2 antlr/TokenRangeElement.begin int)\n"
"  (line 13)\n"
"  (iput v0 v2 antlr/TokenRangeElement.end int)\n"
"  (line 19)\n"
"  (iget-object v0 v2 antlr/TokenRangeElement.grammar [object antlr/Grammar])\n"
"  (iget-object v0 v0 antlr/Grammar.tokenManager [object antlr/TokenManager])\n"
"  (invoke-virtual {v4} antlr/Token/getText )\n"
"  (move-result-object v1)\n"
"  (invoke-interface {v0 v1} antlr/TokenManager/getTokenSymbol [object java/lang/String] )\n"
"  (move-result-object v0)\n"
"  (invoke-virtual {v0} antlr/TokenSymbol/getTokenType )\n"
"  (move-result v0)\n"
"  (iput v0 v2 antlr/TokenRangeElement.begin int)\n"
"  (line 20)\n"
"  (invoke-virtual {v4} antlr/Token/getText )\n"
"  (move-result-object v0)\n"
"  (iput-object v0 v2 antlr/TokenRangeElement.beginText [object java/lang/String])\n"
"  (line 21)\n"
"  (iget-object v0 v2 antlr/TokenRangeElement.grammar [object antlr/Grammar])\n"
"  (iget-object v0 v0 antlr/Grammar.tokenManager [object antlr/TokenManager])\n"
"  (invoke-virtual {v5} antlr/Token/getText )\n"
"  (move-result-object v1)\n"
"  (invoke-interface {v0 v1} antlr/TokenManager/getTokenSymbol [object java/lang/String] )\n"
"  (move-result-object v0)\n"
"  (invoke-virtual {v0} antlr/TokenSymbol/getTokenType )\n"
"  (move-result v0)\n"
"  (iput v0 v2 antlr/TokenRangeElement.end int)\n"
"  (line 22)\n"
"  (invoke-virtual {v5} antlr/Token/getText )\n"
"  (move-result-object v0)\n"
"  (iput-object v0 v2 antlr/TokenRangeElement.endText [object java/lang/String])\n"
"  (line 23)\n"
"  (invoke-virtual {v4} antlr/Token/getLine )\n"
"  (move-result v0)\n"
"  (iput v0 v2 antlr/TokenRangeElement.line int)\n"
"  (line 24)\n"
"  (return-void )\n"
" )\n"
"\n"
"(method (attrs public ) generate()void\n"
"  (limit registers 2)\n"
"  ; this: v1 ([object antlr/TokenRangeElement])\n"
"  (line 27)\n"
"  (iget-object v0 v1 antlr/TokenRangeElement.grammar [object antlr/Grammar])\n"
"  (iget-object v0 v0 antlr/Grammar.generator [object antlr/CodeGenerator])\n"
"  (invoke-virtual {v0 v1} antlr/CodeGenerator/gen [object antlr/TokenRangeElement] )\n"
"  (line 28)\n"
"  (return-void )\n"
")\n"
"\n"
"(method (attrs public ) getLabel()[object java/lang/String]\n"
"  (limit registers 2)\n"
"  ; this: v1 ([object antlr/TokenRangeElement])\n"
"  (line 31)\n"
"  (iget-object v0 v1 antlr/TokenRangeElement.label [object java/lang/String])\n"
"  (return-object v0)\n"
")\n"
"\n"
"(method (attrs public ) look(int )[object antlr/Lookahead]\n"
"  (limit registers 3)\n"
"  ; this: v1 ([object antlr/TokenRangeElement])\n"
"  ; parameter[0] : v2 (int)\n"
"  (line 35)\n"
"  (iget-object v0 v1 antlr/TokenRangeElement.grammar [object antlr/Grammar])\n"
"  (iget-object v0 v0 antlr/Grammar.theLLkAnalyzer [object antlr/LLkGrammarAnalyzer])\n"
"  (invoke-interface {v0 v2 v1} antlr/LLkGrammarAnalyzer/look int [object antlr/TokenRangeElement] )\n"
"  (move-result-object v0)\n"
"  (return-object v0)\n"
")\n"
"\n"
"(method (attrs public ) setLabel([object java/lang/String] )void\n"
"  (limit registers 2)\n"
"  ; this: v0 ([object antlr/TokenRangeElement])\n"
"  ; parameter[0] : v1 ([object java/lang/String])\n"
"  (line 39)\n"
"  (iput-object v1 v0 antlr/TokenRangeElement.label [object java/lang/String])\n"
"  (line 40)\n"
"  (return-void )\n"
")\n"
"\n"
"(method (attrs public ) toString()[object java/lang/String]\n"
"  (limit registers 3)\n"
"  ; this: v2 ([object antlr/TokenRangeElement])\n"
"  (line 43)\n"
"  (iget-object v0 v2 antlr/TokenRangeElement.label [object java/lang/String])\n"
"  (if-eqz v0 l48904)\n"
"  (line 44)\n"
"  (new-instance v0 java/lang/StringBuilder)\n"
"  (const-string v1 \"\\ \")\n"
"  (invoke-direct {v0 v1} java/lang/StringBuilder/<init> [object java/lang/String] )\n"
"  (iget-object v1 v2 antlr/TokenRangeElement.label [object java/lang/String])\n"
"  (invoke-virtual {v0 v1} java/lang/StringBuilder/append [object java/lang/String] )\n"
"  (move-result-object v0)\n"
"  (const-string v1 \":\")\n"
"  (invoke-virtual {v0 v1} java/lang/StringBuilder/append [object java/lang/String] )\n"
"  (move-result-object v0)\n"
"  (iget-object v1 v2 antlr/TokenRangeElement.beginText [object java/lang/String])\n"
"  (invoke-virtual {v0 v1} java/lang/StringBuilder/append [object java/lang/String] )\n"
"  (move-result-object v0)\n"
"  (const-string v1 \"..\")\n"
"  (invoke-virtual {v0 v1} java/lang/StringBuilder/append [object java/lang/String] )\n"
"  (move-result-object v0)\n"
"  (iget-object v1 v2 antlr/TokenRangeElement.endText [object java/lang/String])\n"
"  (invoke-virtual {v0 v1} java/lang/StringBuilder/append [object java/lang/String] )\n"
"  (move-result-object v0)\n"
"  (invoke-virtual {v0} java/lang/StringBuilder/toString )\n"
"  (move-result-object v0)\n"
"  (label l48902)\n"
"  (line 47)\n"
"  (return-object v0)\n"
"  (label l48904)\n"
"  (new-instance v0 java/lang/StringBuilder)\n"
"  (const-string v1 \"\\ \")\n"
"  (invoke-direct {v0 v1} java/lang/StringBuilder/<init> [object java/lang/String] )\n"
"  (iget-object v1 v2 antlr/TokenRangeElement.beginText [object java/lang/String])\n"
"  (invoke-virtual {v0 v1} java/lang/StringBuilder/append [object java/lang/String] )\n"
"  (move-result-object v0)\n"
"  (const-string v1 \"..\")\n"
"  (invoke-virtual {v0 v1} java/lang/StringBuilder/append [object java/lang/String] )\n"
"  (move-result-object v0)\n"
"  (iget-object v1 v2 antlr/TokenRangeElement.endText [object java/lang/String])\n"
"  (invoke-virtual {v0 v1} java/lang/StringBuilder/append [object java/lang/String] )\n"
"  (move-result-object v0)\n"
"  (invoke-virtual {v0} java/lang/StringBuilder/toString )\n"
"  (move-result-object v0)\n"
"  (goto l48902)\n"
")\n"
"\n"
")\n";
#include <assert.h>
int main()
{
    adam_init();
    sexpression_t* sexp;
    dalvik_class_t* class;
    assert(sexp_parse(input, &sexp));
    assert(NULL != (class = dalvik_class_from_sexp(sexp)));
    sexp_free(sexp);
    adam_finalize();
    return 0;
}
