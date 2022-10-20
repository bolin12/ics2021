#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <ctype.h>
#include <stdbool.h>

enum
{
  TK_NOTYPE,
  TK_U,// to keep unsigned calculation
  TK_EQ,
  TK_ADD,
  TK_SUB,
  TK_MUL,
  TK_DIV,
  TK_NUM,
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
    {"\\*", TK_MUL},   // mul
    {"/", TK_DIV},
    {"[1-9][0-9]*|0", TK_NUM},
    {"\\(", TK_LBRAKETS},
    {"\\)", TK_RBRAKETS}};

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

static bool check_legalparen(int p, int q);
static bool check_parentheses(int p, int q);
static int found_main_op_token(int p, int q);

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
        case TK_U:  // to eliminate fool generator to generate some signed calcculation.
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
        case TK_MUL:
          tokens[nr_token].type = TK_MUL;
          break;
        case TK_DIV:
          tokens[nr_token].type = TK_DIV;
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
    for (int i = 0; tokens[p].str[i] != '\0'; i++) // judge if it is a number one by one
    {

      if (!isdigit(tokens[p].str[i]))
      {
        Log("Not a digit of char%c!", tokens[p].str[i]);
        EXPR_FLAG = false;
        return 0;
      }
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

    word_t val1 = eval(p, op_pos - 1);
    word_t val2 = eval(op_pos + 1, q);
    // Log("val1 and val2 are:%u,%u", val1,val2);
    word_t ret_val=0;
    switch (tokens[op_pos].type)
    {
    case TK_ADD:
      ret_val =  val1 + val2;
      break;
    case TK_SUB:
      ret_val =  val1 - val2;
      break;
    case TK_MUL:
      ret_val =  val1 * val2;
      break;
    case TK_DIV:
      if (val2 == 0)
      {
        Log("Div zero error!");
        EXPR_FLAG = false;
        return 0;
      }
      ret_val =  val1 / val2;
      break;
    default:
      Assert(0, "????\n");
    }

    
    return ret_val;
  }
  
}

static int found_main_op_token(int p, int q)
{
  int ri_paren_count = 0;
  bool ADD_SUB_FOUND_FLAG = false;
  int op_pos;
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
    if (tokens[op_pos].type == TK_ADD || tokens[op_pos].type == TK_SUB)
    {
      if (ri_paren_count == 0)
      {
        ADD_SUB_FOUND_FLAG = true;
        break;
      }
    }
  }

  if (ADD_SUB_FOUND_FLAG == false)
  {
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
      if (tokens[op_pos].type == TK_MUL || tokens[op_pos].type == TK_DIV)
      {
        if (ri_paren_count == 0)
        {
          break;
        }
      }
    }
  }
  if (op_pos < p)
  {
    return -1;
  }
  return op_pos;
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