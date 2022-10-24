#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <ctype.h>
#include <stdbool.h>



enum
{
  TK_NOTYPE = 1,
  TK_U, // to keep unsigned calculation
  TK_EQ,
  TK_NOTEQ,
  TK_AND,
  TK_ADD,
  TK_SUB,
  TK_MUL,
  TK_DIV,
  // TK_DECIMAL_NUM,
  TK_NUM,
  TK_REGNAME,
  TK_DEPOINTER,
  TK_LBRAKETS,
  TK_RBRAKETS,

  /* TODO: Add more token types */

};

static struct rule
{
  const char *regex;
  int token_type;
} rules[] = {

    /* TODO: Add more rules.
     * Pay attention to the precedence level of different rules.
     */

    {" +", TK_NOTYPE}, // spaces
    {"u", TK_U},       // unsigned flag
    {"\\+", TK_ADD},   // add
    {"-", TK_SUB},     // sub
    {"==", TK_EQ},     // equal
    {"!=", TK_NOTEQ},  // not equal
    {"&&", TK_AND},
    {"\\*", TK_MUL}, // mul
    {"/", TK_DIV},
    {"(0x)?[0-9a-fA_F]*|^\\$[0-9a-zA-Z]*", TK_NUM}, // advanced match than devimal
    {"\\(", TK_LBRAKETS},
    {"\\)", TK_RBRAKETS}};

static int level_token[5][2] = {
    {TK_EQ, TK_NOTEQ},
    {TK_ADD, TK_SUB},
    {TK_MUL, TK_DIV},
    {TK_DEPOINTER},

};
#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i++)
  {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0)
    {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token
{
  int type;
  char str[32];
} Token;
// larger for test
static Token tokens[512] __attribute__((used)) = {};
static int nr_token __attribute__((used)) = 0;
bool EXPR_FLAG = true;
static uint32_t eval(int p, int q);

word_t paddr_read(paddr_t addr, int len);
static bool check_legalparen(int p, int q);
static bool check_parentheses(int p, int q);
static int found_main_op_token(int p, int q);
static int found_specific_level_op(int p, int q, int level);

static bool make_token(char *e)
{
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0')
  {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i++)
    {
      // printf("here loop in?\n");
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
      {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        // Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
        //     i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        switch (rules[i].token_type)
        {
        case TK_NOTYPE:
          goto NEXT; // just jump out of this
                     //  tokens[nr_token].type = TK_NOTYPE; no need to record TK_NOTYPE
        case TK_U:   // to eliminate fool generator to generate some signed calcculation.
          goto NEXT;
        case TK_ADD:
          tokens[nr_token].type = TK_ADD;
          break;
        case TK_SUB:
          tokens[nr_token].type = TK_SUB;
          break;
        case TK_EQ:
          tokens[nr_token].type = TK_EQ;
          break;
        case TK_NOTEQ:
          tokens[nr_token].type = TK_NOTEQ;
          break;
        case TK_MUL:
          tokens[nr_token].type = TK_MUL;
          break;
        case TK_DIV:
          tokens[nr_token].type = TK_DIV;
          break;
        case TK_AND:
          tokens[nr_token].type = TK_AND;
          break;
        case TK_NUM:
          tokens[nr_token].type = TK_NUM;
          break;
        case TK_LBRAKETS:
          tokens[nr_token].type = TK_LBRAKETS;
          break;
        case TK_RBRAKETS:
          tokens[nr_token].type = TK_RBRAKETS;
          break;
        default:
          Log("??? no match??\n");
          break;
        }
        strncpy(tokens[nr_token].str, substr_start, substr_len);
        tokens[nr_token].str[substr_len] = '\0'; // set end str flag
        nr_token++;
      NEXT:
        break; // match ended no need for loop else wrong.
      }
    }

    if (i == NR_REGEX)
    {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

word_t expr(char *e, bool *success)
{

  if (!make_token(e))
  {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  // TODO();
  EXPR_FLAG = true;

  if (check_legalparen(0, nr_token - 1) == false)
  {
    *success = false;
    Log("Not legal parentheses!");
    return 0;
  }

  for (int i = 0; i < nr_token; i++)
  {

    if (tokens[i].type == TK_MUL)
    {
      if (i == 0)
      {
        tokens[i].type = TK_DEPOINTER;
        continue;
      }
      switch (tokens[i - 1].type)
      {
      case TK_ADD:
        tokens[i].type = TK_DEPOINTER;
        break;
      case TK_SUB:
        tokens[i].type = TK_DEPOINTER;
        break;
      case TK_DIV:
        tokens[i].type = TK_DEPOINTER;
        break;
      case TK_MUL:
        tokens[i].type = TK_DEPOINTER;
        break;
      case TK_EQ:
        tokens[i].type = TK_DEPOINTER;
        break;
      case TK_NOTEQ:
        tokens[i].type = TK_DEPOINTER;
        break;
      default:
        break;
      }
    }
  }

  word_t value = eval(0, nr_token - 1);
  if (EXPR_FLAG == false)
  {
    *success = false;
  }
  return value;
}

static uint32_t eval(int p, int q)
{
  Assert(p <= q, "Bad Expression!");
  if (EXPR_FLAG == false)
  {
    return 0;
  }
  if (p == q)
  {
    if (tokens[p].str[0] == '$')
    {
      bool *success = malloc(1);
      word_t reg_val = isa_reg_str2val(tokens[p].str + 1, success);
      if (*success == false)
      {
        Log("Register error!");
        EXPR_FLAG = false;
        return 0;
      }
      return reg_val;
    }
    if (tokens[p].str[0] == '0' && tokens[p].str[1] == 'x')
    {
      return strtol(tokens[p].str, NULL, 16);
    }
    return atoi(tokens[p].str);
  }
  else if (check_parentheses(p, q))
  {
    return eval(p + 1, q - 1);
  }
  else
  {
    int op_pos = found_main_op_token(p, q);
    if (op_pos == -1)
    {
      Log("No legal operator from p%d:%s to q%d:%s!", p, tokens[p].str, q, tokens[q].str);
      EXPR_FLAG = false;
      return 0;
    }

    word_t ret_val = 0;
    if (tokens[op_pos].type == TK_DEPOINTER)
    {

      word_t to_execute = eval(op_pos + 1, q);
      word_t val = paddr_read(to_execute, 4);
      return val;
    }
    word_t val1 = eval(p, op_pos - 1);
    word_t val2 = eval(op_pos + 1, q);
    // Log("val1 and val2 are:%u,%u", val1,val2);

    switch (tokens[op_pos].type)
    {
    case TK_EQ:
      ret_val = val1 == val2;
    case TK_NOTEQ:
      ret_val = val1 != val2;
    case TK_ADD:
      ret_val = val1 + val2;
      break;
    case TK_SUB:
      ret_val = val1 - val2;
      break;
    case TK_MUL:
      ret_val = val1 * val2;
      break;
    case TK_DIV:
      if (val2 == 0)
      {
        Log("Div zero error!");
        EXPR_FLAG = false;
        return 0;
      }
      ret_val = val1 / val2;
      break;
    default:
      Assert(0, "????\n");
    }

    return ret_val;
  }
}

static int found_main_op_token(int p, int q)
{
  int op_pos = -1;
  for (int i = 0; i < 4; i++)
  {
    op_pos = found_specific_level_op(p, q, i);
    if (op_pos >= 0)
      return op_pos;
  }
  return -1;
}

static int found_specific_level_op(int p, int q, int level)
{
  int op_pos;
  int ri_paren_count = 0;

  int cur_level_op_num = sizeof(level_token[level]) / sizeof(level_token[level][0]);
  for (op_pos = q; op_pos >= p; op_pos--)
  {
    if (!strcmp(tokens[op_pos].str, ")"))
    {
      ri_paren_count++;
    }
    if (!strcmp(tokens[op_pos].str, "("))
    {
      ri_paren_count--;
    }

    for (int i = 0; i < cur_level_op_num; i++)
    {
      if (tokens[op_pos].type == level_token[level][i])
      {
        if (ri_paren_count == 0)
        {
          return op_pos;
        }
      }
    }
  }
  return -1;
}

static bool check_parentheses(int p, int q)
{
  if (!(!strcmp(tokens[p].str, "(") && !strcmp(tokens[q].str, ")")))
  {
    return false;
  }
  int le_count = 1;
  for (int i = p + 1; i <= q; i++)
  {
    if (!strcmp(tokens[i].str, "("))
    {
      le_count++;
    }
    if (!strcmp(tokens[i].str, ")"))
    {
      le_count--;
      if (le_count == 0 && i < q)
      {
        return false;
      }
    }
  }
  return true;
}

static bool check_legalparen(int p, int q)
{
  int le_count = 0;
  for (int i = p; i <= q; i++)
  {
    if (!strcmp(tokens[i].str, "("))
    {
      le_count++;
    }
    if (!strcmp(tokens[i].str, ")"))
    {
      le_count--;
      if (le_count < 0)
      {
        return false;
      }
    }
  }
  if (le_count > 0)
  {
    return false;
  }
  return true;
}