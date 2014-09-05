#ifndef __DALVIK_TOKENS_H__
#define __DALVIK_TOKENS_H__
/** @file dalvik_tokens.h
 *  @brief lexer tokens 
 */

/** @brief the maximum keyword */
#define DALVIK_MAX_NUM_KEYWORDS 256
/** @brief keyword array */
extern const char* dalvik_keywords[DALVIK_MAX_NUM_KEYWORDS];

/** @brief return a token table entity */
#define DALVIK_TOKEN_TABLE_ENTITY(id) ((const char* const)dalvik_keywords[(id)])

#define DALVIK_TOKEN_MOVE       DALVIK_TOKEN_TABLE_ENTITY(0)
#define DALVIK_TOKEN_RETURN     DALVIK_TOKEN_TABLE_ENTITY(1)
#define DALVIK_TOKEN_CONST      DALVIK_TOKEN_TABLE_ENTITY(2)
#define DALVIK_TOKEN_MONITOR    DALVIK_TOKEN_TABLE_ENTITY(3)
#define DALVIK_TOKEN_CHECK      DALVIK_TOKEN_TABLE_ENTITY(4)
#define DALVIK_TOKEN_INSTANCE   DALVIK_TOKEN_TABLE_ENTITY(5)
#define DALVIK_TOKEN_ARRAY      DALVIK_TOKEN_TABLE_ENTITY(6)
#define DALVIK_TOKEN_NEW        DALVIK_TOKEN_TABLE_ENTITY(7)
#define DALVIK_TOKEN_FILLED     DALVIK_TOKEN_TABLE_ENTITY(8)
#define DALVIK_TOKEN_16         DALVIK_TOKEN_TABLE_ENTITY(9)
#define DALVIK_TOKEN_FROM16     DALVIK_TOKEN_TABLE_ENTITY(10)
#define DALVIK_TOKEN_WIDE       DALVIK_TOKEN_TABLE_ENTITY(11)
#define DALVIK_TOKEN_OBJECT     DALVIK_TOKEN_TABLE_ENTITY(12)
#define DALVIK_TOKEN_RESULT     DALVIK_TOKEN_TABLE_ENTITY(13)
#define DALVIK_TOKEN_EXCEPTION  DALVIK_TOKEN_TABLE_ENTITY(14)
#define DALVIK_TOKEN_VOID       DALVIK_TOKEN_TABLE_ENTITY(15)
#define DALVIK_TOKEN_4          DALVIK_TOKEN_TABLE_ENTITY(16)
#define DALVIK_TOKEN_HIGH16     DALVIK_TOKEN_TABLE_ENTITY(17)
#define DALVIK_TOKEN_32         DALVIK_TOKEN_TABLE_ENTITY(18)
#define DALVIK_TOKEN_CLASS      DALVIK_TOKEN_TABLE_ENTITY(19)
#define DALVIK_TOKEN_JUMBO      DALVIK_TOKEN_TABLE_ENTITY(20)
#define DALVIK_TOKEN_ENTER      DALVIK_TOKEN_TABLE_ENTITY(21)
#define DALVIK_TOKEN_EXIT       DALVIK_TOKEN_TABLE_ENTITY(22)
#define DALVIK_TOKEN_CAST       DALVIK_TOKEN_TABLE_ENTITY(23)
#define DALVIK_TOKEN_OF         DALVIK_TOKEN_TABLE_ENTITY(24)
#define DALVIK_TOKEN_LENGTH     DALVIK_TOKEN_TABLE_ENTITY(25)
#define DALVIK_TOKEN_RANGE      DALVIK_TOKEN_TABLE_ENTITY(26)
#define DALVIK_TOKEN_THROW      DALVIK_TOKEN_TABLE_ENTITY(27)
#define DALVIK_TOKEN_GOTO       DALVIK_TOKEN_TABLE_ENTITY(28)
#define DALVIK_TOKEN_PACKED     DALVIK_TOKEN_TABLE_ENTITY(29)
#define DALVIK_TOKEN_SWITCH     DALVIK_TOKEN_TABLE_ENTITY(30)
#define DALVIK_TOKEN_SPARSE     DALVIK_TOKEN_TABLE_ENTITY(31)
#define DALVIK_TOKEN_CMPL       DALVIK_TOKEN_TABLE_ENTITY(32)
#define DALVIK_TOKEN_CMPG       DALVIK_TOKEN_TABLE_ENTITY(33)
#define DALVIK_TOKEN_CMP        DALVIK_TOKEN_TABLE_ENTITY(34)
#define DALVIK_TOKEN_FLOAT      DALVIK_TOKEN_TABLE_ENTITY(35)
#define DALVIK_TOKEN_DOUBLE     DALVIK_TOKEN_TABLE_ENTITY(36)
#define DALVIK_TOKEN_LONG       DALVIK_TOKEN_TABLE_ENTITY(37)
#define DALVIK_TOKEN_IF         DALVIK_TOKEN_TABLE_ENTITY(38)
#define DALVIK_TOKEN_EQ         DALVIK_TOKEN_TABLE_ENTITY(39)
#define DALVIK_TOKEN_NE         DALVIK_TOKEN_TABLE_ENTITY(40)
#define DALVIK_TOKEN_LE         DALVIK_TOKEN_TABLE_ENTITY(41)
#define DALVIK_TOKEN_GE         DALVIK_TOKEN_TABLE_ENTITY(42)
#define DALVIK_TOKEN_GT         DALVIK_TOKEN_TABLE_ENTITY(43)
#define DALVIK_TOKEN_LT         DALVIK_TOKEN_TABLE_ENTITY(44)
#define DALVIK_TOKEN_EQZ        DALVIK_TOKEN_TABLE_ENTITY(45)
#define DALVIK_TOKEN_NEZ        DALVIK_TOKEN_TABLE_ENTITY(46)
#define DALVIK_TOKEN_LEZ        DALVIK_TOKEN_TABLE_ENTITY(47)
#define DALVIK_TOKEN_GEZ        DALVIK_TOKEN_TABLE_ENTITY(48)
#define DALVIK_TOKEN_GTZ        DALVIK_TOKEN_TABLE_ENTITY(49)
#define DALVIK_TOKEN_LTZ        DALVIK_TOKEN_TABLE_ENTITY(50)
#define DALVIK_TOKEN_BOOLEAN    DALVIK_TOKEN_TABLE_ENTITY(51)
#define DALVIK_TOKEN_BYTE       DALVIK_TOKEN_TABLE_ENTITY(52)
#define DALVIK_TOKEN_CHAR       DALVIK_TOKEN_TABLE_ENTITY(53)
#define DALVIK_TOKEN_SHORT      DALVIK_TOKEN_TABLE_ENTITY(54)
#define DALVIK_TOKEN_AGET       DALVIK_TOKEN_TABLE_ENTITY(55)
#define DALVIK_TOKEN_APUT       DALVIK_TOKEN_TABLE_ENTITY(56)
#define DALVIK_TOKEN_SGET       DALVIK_TOKEN_TABLE_ENTITY(57)
#define DALVIK_TOKEN_SPUT       DALVIK_TOKEN_TABLE_ENTITY(58)
#define DALVIK_TOKEN_IGET       DALVIK_TOKEN_TABLE_ENTITY(59)
#define DALVIK_TOKEN_IPUT       DALVIK_TOKEN_TABLE_ENTITY(60)
#define DALVIK_TOKEN_INVOKE     DALVIK_TOKEN_TABLE_ENTITY(61)
#define DALVIK_TOKEN_VIRTUAL    DALVIK_TOKEN_TABLE_ENTITY(62)
#define DALVIK_TOKEN_SUPER      DALVIK_TOKEN_TABLE_ENTITY(63)
#define DALVIK_TOKEN_DIRECT     DALVIK_TOKEN_TABLE_ENTITY(64)
#define DALVIK_TOKEN_STATIC     DALVIK_TOKEN_TABLE_ENTITY(65)
#define DALVIK_TOKEN_INTERFACE  DALVIK_TOKEN_TABLE_ENTITY(66)
#define DALVIK_TOKEN_INT        DALVIK_TOKEN_TABLE_ENTITY(67)
#define DALVIK_TOKEN_TO         DALVIK_TOKEN_TABLE_ENTITY(68)
#define DALVIK_TOKEN_NEG        DALVIK_TOKEN_TABLE_ENTITY(69)
#define DALVIK_TOKEN_NOT        DALVIK_TOKEN_TABLE_ENTITY(70)
#define DALVIK_TOKEN_ADD        DALVIK_TOKEN_TABLE_ENTITY(71)
#define DALVIK_TOKEN_SUB        DALVIK_TOKEN_TABLE_ENTITY(72)
#define DALVIK_TOKEN_MUL        DALVIK_TOKEN_TABLE_ENTITY(73)
#define DALVIK_TOKEN_DIV        DALVIK_TOKEN_TABLE_ENTITY(74)
#define DALVIK_TOKEN_REM        DALVIK_TOKEN_TABLE_ENTITY(75)
#define DALVIK_TOKEN_AND        DALVIK_TOKEN_TABLE_ENTITY(76)
#define DALVIK_TOKEN_OR         DALVIK_TOKEN_TABLE_ENTITY(77)
#define DALVIK_TOKEN_XOR        DALVIK_TOKEN_TABLE_ENTITY(78)
#define DALVIK_TOKEN_SHL        DALVIK_TOKEN_TABLE_ENTITY(79)
#define DALVIK_TOKEN_SHR        DALVIK_TOKEN_TABLE_ENTITY(80)
#define DALVIK_TOKEN_USHL       DALVIK_TOKEN_TABLE_ENTITY(81)
#define DALVIK_TOKEN_USHR       DALVIK_TOKEN_TABLE_ENTITY(82)
#define DALVIK_TOKEN_2ADDR      DALVIK_TOKEN_TABLE_ENTITY(83)
#define DALVIK_TOKEN_LIT8       DALVIK_TOKEN_TABLE_ENTITY(84)
#define DALVIK_TOKEN_LIT16      DALVIK_TOKEN_TABLE_ENTITY(85)
#define DALVIK_TOKEN_NOP        DALVIK_TOKEN_TABLE_ENTITY(86)
#define DALVIK_TOKEN_STRING     DALVIK_TOKEN_TABLE_ENTITY(87)
#define DALVIK_TOKEN_DEFAULT    DALVIK_TOKEN_TABLE_ENTITY(88)
#define DALVIK_TOKEN_RSUB       DALVIK_TOKEN_TABLE_ENTITY(89)
#define DALVIK_TOKEN_METHOD     DALVIK_TOKEN_TABLE_ENTITY(90)
#define DALVIK_TOKEN_ATTRS      DALVIK_TOKEN_TABLE_ENTITY(91)
#define DALVIK_TOKEN_ABSTRACT   DALVIK_TOKEN_TABLE_ENTITY(92)
#define DALVIK_TOKEN_ANNOTATION DALVIK_TOKEN_TABLE_ENTITY(93)
#define DALVIK_TOKEN_FINAL      DALVIK_TOKEN_TABLE_ENTITY(94)
#define DALVIK_TOKEN_PRIVATE    DALVIK_TOKEN_TABLE_ENTITY(95)
#define DALVIK_TOKEN_PROTECTED  DALVIK_TOKEN_TABLE_ENTITY(96)
#define DALVIK_TOKEN_PUBLIC     DALVIK_TOKEN_TABLE_ENTITY(97)
#define DALVIK_TOKEN_SYNCHRNIZED DALVIK_TOKEN_TABLE_ENTITY(98)
#define DALVIK_TOKEN_TRANSIENT  DALVIK_TOKEN_TABLE_ENTITY(99)
#define DALVIK_TOKEN_LINE       DALVIK_TOKEN_TABLE_ENTITY(100)
#define DALVIK_TOKEN_LIMIT      DALVIK_TOKEN_TABLE_ENTITY(101)
#define DALVIK_TOKEN_REGISTERS  DALVIK_TOKEN_TABLE_ENTITY(102)
#define DALVIK_TOKEN_LABEL      DALVIK_TOKEN_TABLE_ENTITY(103)
#define DALVIK_TOKEN_FIELD      DALVIK_TOKEN_TABLE_ENTITY(104)
#define DALVIK_TOKEN_SOURCE     DALVIK_TOKEN_TABLE_ENTITY(105)
#define DALVIK_TOKEN_IMPLEMENTS DALVIK_TOKEN_TABLE_ENTITY(106)
#define DALVIK_TOKEN_DATA       DALVIK_TOKEN_TABLE_ENTITY(107)
#define DALVIK_TOKEN_CATCH      DALVIK_TOKEN_TABLE_ENTITY(108)
#define DALVIK_TOKEN_CATCHALL   DALVIK_TOKEN_TABLE_ENTITY(109)
#define DALVIK_TOKEN_FILL       DALVIK_TOKEN_TABLE_ENTITY(110)
#define DALVIK_TOKEN_USING      DALVIK_TOKEN_TABLE_ENTITY(111)
#define DALVIK_TOKEN_FROM       DALVIK_TOKEN_TABLE_ENTITY(112)
#define DALVIK_TOKEN_DEBUG      DALVIK_TOKEN_TABLE_ENTITY(113)
#define DALVIK_TOKEN_TRUE       DALVIK_TOKEN_TABLE_ENTITY(114)
#define DALVIK_TOKEN_FALSE      DALVIK_TOKEN_TABLE_ENTITY(115)
#define DALVIK_TOKEN_UINVOKE    DALVIK_TOKEN_TABLE_ENTITY(116) /* this is the _invoke (used to emliminate instructions like array-new */
#define DALVIK_TOKEN_VOLATILE   DALVIK_TOKEN_TABLE_ENTITY(117)
/** @brief initialize the token table. no need to finalize, because stringpool can dealing with this */
int dalvik_tokens_init(void);

#endif
