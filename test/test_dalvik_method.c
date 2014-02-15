#include <dalvik/dalvik_method.h>
#include <sexp.h>
#include <assert.h>
#include <adam.h>
sexpression_t* sexp;
dalvik_method_t* method;

int main()
{
    adam_init();
    const char* method_code =
"(method (attrs public ) visit([object antlr/collections/AST] )void\n"
"  (limit registers 7)\n"
"  ; this: v5 ([object antlr/DumpASTVisitor])\n"
"  ; parameter[0] : v6 ([object antlr/collections/AST])\n"
"  (line 27)\n"
"  (const/4 v0 0)\n"
"  (line 29)\n"
"  (move-object v1 v6)\n"
"  (label l36f28)\n"
"  (if-nez v1 l36f46)\n"
"  (label l36f2c)\n"
"  (line 36)\n"
"  (move-object v1 v6)\n"
"  (label l36f2e)\n"
"  (if-nez v1 l36f60)\n"
"  (line 63)\n"
"  (if-eqz v0 l36f44)\n"
"  (line 64)\n"
"  (sget-object v2 java/lang/System.out [object java/io/PrintStream])\n"
"  (const-string v3 \"\")\n"
"  (invoke-virtual {v2 v3} java/io/PrintStream/println [object java/lang/String] )\n"
"  (label l36f44)\n"
"  (line 66)\n"
"  (return-void )\n"
"  (label l36f46)\n"
"  (line 30)\n"
"  (invoke-interface {v1} antlr/collections/AST/getFirstChild )\n"
"  (move-result-object v2)\n"
"  (if-eqz v2 l36f56)\n"
"  (line 31)\n"
"  (const/4 v0 0)\n"
"  (line 32)\n"
"  (goto l36f2c)\n"
"  (label l36f56)\n"
"  (line 29)\n"
"  (invoke-interface {v1} antlr/collections/AST/getNextSibling )\n"
"  (move-result-object v1)\n"
"  (goto l36f28)\n"
"  (label l36f60)\n"
"  (line 37)\n"
"  (if-eqz v0 l36f68)\n"
"  (if-ne v1 v6 l36f6e)\n"
"  (label l36f68)\n"
"  (line 38)\n"
"  (invoke-direct {v5} antlr/DumpASTVisitor/tabs )\n"
"  (label l36f6e)\n"
"  (line 40)\n"
"  (invoke-interface {v1} antlr/collections/AST/getText )\n"
"  (move-result-object v2)\n"
"  (if-nez v2 l37012)\n"
"  (line 41)\n"
"  (sget-object v2 java/lang/System.out [object java/io/PrintStream])\n"
"  (const-string v3 \"nil\")\n"
"  (invoke-virtual {v2 v3} java/io/PrintStream/print [object java/lang/String] )\n"
"  (label l36f88)\n"
"  (line 47)\n"
"  (sget-object v2 java/lang/System.out [object java/io/PrintStream])\n"
"  (new-instance v3 java/lang/StringBuilder)\n"
"  (const-string v4 \" [\")\n"
"  (invoke-direct {v3 v4} java/lang/StringBuilder/<init> [object java/lang/String] )\n"
"  (invoke-interface {v1} antlr/collections/AST/getType )\n"
"  (move-result v4)\n"
"  (invoke-virtual {v3 v4} java/lang/StringBuilder/append int )\n"
"  (move-result-object v3)\n"
"  (const-string v4 \"] \")\n"
"  (invoke-virtual {v3 v4} java/lang/StringBuilder/append [object java/lang/String] )\n"
"  (move-result-object v3)\n"
"  (invoke-virtual {v3} java/lang/StringBuilder/toString )\n"
"  (move-result-object v3)\n"
"  (invoke-virtual {v2 v3} java/io/PrintStream/print [object java/lang/String] )\n"
"  (line 49)\n"
"  (if-eqz v0 l37026)\n"
"  (line 50)\n"
"  (sget-object v2 java/lang/System.out [object java/io/PrintStream])\n"
"  (const-string v3 \" \")\n"
"  (invoke-virtual {v2 v3} java/io/PrintStream/print [object java/lang/String] )\n"
"  (label l36fd6)\n"
"  (line 56)\n"
"  (invoke-interface {v1} antlr/collections/AST/getFirstChild )\n"
"  (move-result-object v2)\n"
"  (if-eqz v2 l37008)\n"
"  (line 57)\n"
"  (iget v2 v5 antlr/DumpASTVisitor.level int)\n"
"  (add-int/lit8 v2 v2 1)\n"
"  (iput v2 v5 antlr/DumpASTVisitor.level int)\n"
"  (line 58)\n"
"  (invoke-interface {v1} antlr/collections/AST/getFirstChild )\n"
"  (move-result-object v2)\n"
"  (invoke-virtual {v5 v2} antlr/DumpASTVisitor/visit [object antlr/collections/AST] )\n"
"  (line 59)\n"
"  (iget v2 v5 antlr/DumpASTVisitor.level int)\n"
"  (add-int/lit8 v2 v2 255)\n"
"  (iput v2 v5 antlr/DumpASTVisitor.level int)\n"
"  (label l37008)\n"
"  (line 36)\n"
"  (invoke-interface {v1} antlr/collections/AST/getNextSibling )\n"
"  (move-result-object v1)\n"
"  (goto l36f2e)\n"
"  (label l37012)\n"
"  (line 44)\n"
"  (sget-object v2 java/lang/System.out [object java/io/PrintStream])\n"
"  (invoke-interface {v1} antlr/collections/AST/getText )\n"
"  (move-result-object v3)\n"
"  (invoke-virtual {v2 v3} java/io/PrintStream/print [object java/lang/String] )\n"
"  (goto l36f88)\n"
"  (label l37026)\n"
"  (line 53)\n"
"  (sget-object v2 java/lang/System.out [object java/io/PrintStream])\n"
"  (const-string v3 \"\")\n"
"  (invoke-virtual {v2 v3} java/io/PrintStream/println [object java/lang/String] )\n"
"  (goto l36fd6)\n)";
    assert(NULL != sexp_parse(method_code, &sexp));
    assert(NULL != (method = dalvik_method_from_sexp(sexp, NULL, NULL)));
    sexp_free(sexp);
    dalvik_method_free(method);
    char input2[] = "(method (attrs public abstract ) beginTree([object antlr/Token] )void (annotation systemVisibility [object dalvik/annotation/Throws] (value [array [object java/lang/String]] [object antlr/SemanticException]) ) )";
    //TODO: TEST IT. ISSUE : should not be a failure
    assert(NULL != sexp_parse(input2, &sexp));
    assert(NULL != (method = dalvik_method_from_sexp(sexp, NULL, NULL)));
    sexp_free(sexp);
    dalvik_method_free(method);
    adam_finalize();
    return 0;
}