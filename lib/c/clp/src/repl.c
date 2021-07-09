#include <stdio.h>
#include <stdlib.h>
#include <termio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int vertex;
    int weight;
} edge_t;

typedef struct {
    edge_t **edges;
    int edges_len;
    int edges_size;
    int dist;
    int prev;
    int visited;
} vertex_t;

typedef struct {
    vertex_t **vertices;
    int vertices_len;
    int vertices_size;
} graph_t;

const char target[] = "METHINKS IT IS LIKE A WEASEL";
const char tbl[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

#define CHOICE (sizeof(tbl) - 1)
#define MUTATE 15
#define COPIES 30
/* returns random integer from 0 to n - 1 */
int irand(int n)
{
	int r, rand_max = RAND_MAX - (RAND_MAX % n);
	while((r = rand()) >= rand_max);
	return r / (rand_max / n);
}
/* number of different chars between a and b */
int unfitness(const char *a, const char *b)
{
	int i, sum = 0;
	for (i = 0; a[i]; i++)
		sum += (a[i] != b[i]);
	return sum;
}
/* each char of b has 1/MUTATE chance of differing from a */
void mutate(const char *a, char *b)
{
	int i;
	for (i = 0; a[i]; i++)
		b[i] = irand(MUTATE) ? a[i] : tbl[irand(CHOICE)];

	b[i] = '\0';
}
int evol() {
	int i, best_i, unfit, best, iters = 0;
	char specimen[COPIES][sizeof(target) / sizeof(char)];

	/* init rand string */
	for (i = 0; target[i]; i++)
		specimen[0][i] = tbl[irand(CHOICE)];
	specimen[0][i] = 0;

	do {
		for (i = 1; i < COPIES; i++)
			mutate(specimen[0], specimen[i]);

		/* find best fitting string */
		for (best_i = i = 0; i < COPIES; i++) {
			unfit = unfitness(target, specimen[i]);
			if(unfit < best || !i) {
				best = unfit;
				best_i = i;
			}
		}

		if (best_i) strcpy(specimen[0], specimen[best_i]);
        printf("\033[0;34m[+] \033[0;33mIter\033[0m %d, \033[0;32mScore\033[0m %d \033[0;36m[%s]\033[0m\n", iters++, best, specimen[0]);
	} while (best);

	return 0;
}


void addVertex (graph_t *g, int i) {
    if (g->vertices_size < i + 1) {
        int size = g->vertices_size * 2 > i ? g->vertices_size * 2 : i + 4;
        g->vertices = realloc(g->vertices, size * sizeof (vertex_t *));
        for (int j = g->vertices_size; j < size; j++)
            g->vertices[j] = 0;
        g->vertices_size = size;
    }
    if (!g->vertices[i]) {
        g->vertices[i] = calloc(1, sizeof (vertex_t));
        g->vertices_len++;
    }
}
void addEdge (graph_t *g, int a, int b, int w) {
    a = a - 'a';
    b = b - 'a';
    addVertex(g, a);
    addVertex(g, b);
    vertex_t *v = g->vertices[a];
    if (v->edges_len >= v->edges_size) {
        v->edges_size = v->edges_size ? v->edges_size * 2 : 4;
        v->edges = realloc(v->edges, v->edges_size * sizeof (edge_t *));
    }
    edge_t *e = calloc(1, sizeof (edge_t));
    e->vertex = b;
    e->weight = w;
    v->edges[v->edges_len++] = e;
}
#define TRUE 1
#define FALSE 0
#define STACK_SIZE 80
#define BUFFER_SIZE 100
typedef int bool;

typedef struct {
    char name;
    bool val;
} var;
typedef struct {
    int top;
    bool els[STACK_SIZE];
} stack_of_bool;
char expr[BUFFER_SIZE];
int expr_len;
var vars[24];
int vars_len;

int elems_count(stack_of_bool *sp) { return (sp->top) + 1;   }

bool is_operator(const char c) { return c == '&' || c == '|' || c == '!' || c == '^'; }

int vars_index(const char c) {
   int i;
   for (i = 0; i < vars_len; ++i) {
       if (vars[i].name == c) return i;
   }
   return -1;
}
bool is_full(stack_of_bool *sp) { return sp->top == STACK_SIZE - 1; }

bool is_empty(stack_of_bool *sp) { return sp->top == -1; }

bool peek(stack_of_bool *sp) {
    if (!is_empty(sp))
        return sp->els[sp->top];
    else {
        printf("Stack is empty.\n");
        exit(1);
    }
}

void push(stack_of_bool *sp, bool val) {
    if (!is_full(sp)) {
        sp->els[++(sp->top)] = val;
    }
    else {
        printf("Stack is full.\n");
        exit(1);
    }
}

bool pop(stack_of_bool *sp) {
    if (!is_empty(sp))
        return sp->els[(sp->top)--];
    else {
        printf("\nStack is empty.\n");
        exit(1);
    }
}
void make_empty(stack_of_bool *sp) { sp->top = -1;   }
/* END STACK*/
char* textBetween(char* thisText, char* startText, char* endText, char* returnText)
{
    char* startPointer = 0;
    int stringLength = 0;
    char* endPointer = 0;
    int endLength = 0;
	if (strstr(startText, "start") != 0) {
		startPointer = thisText;
	} else {
		startPointer = strstr(thisText, startText);
    	if (startPointer != 0) {
        	startPointer = startPointer + strlen(startText);
        }
	}
    if (startPointer != 0){
		if (strstr(endText, "end") != 0) {
			endPointer = thisText;
			endLength = 0;
		} else {
			endPointer = strstr(startPointer, endText);
			endLength = (int)strlen(endPointer);
		}
        stringLength = strlen(startPointer) - endLength;
        if (stringLength == 0) {
		    returnText = "";
		    startPointer = 0;
        } else {
    	    strncpy(returnText,startPointer, stringLength);
	        returnText[stringLength++] = '\0';
		}
    } else {
	    returnText = "";
    }
    return startPointer;
}

int startsWith(const char* container, const char* target) {
  if (strlen(container) < strlen(target)) return 0;
  return strncmp(container, target, strlen(target)) == 0;
}

int endsWith(const char* container, const char* target) {
  if (strlen(container) < strlen(target)) return 0;
  return strncmp(
          container + strlen(container)
          - strlen(target), target, strlen(target)
          ) == 0;
}
int contains(const char* container, const char* target) {
  return strstr(container, target) != 0;
}
typedef struct {
    int *data;
    int *prio;
    int *index;
    int len;
    int size;
} heap_t;
typedef struct Tree Tree;
typedef enum {
    nd_Ident, nd_String, nd_Integer, nd_Sequence, nd_If, nd_Prtc, nd_Prts, nd_Prti, nd_While,
    nd_Assign, nd_Negate, nd_Not, nd_Mul, nd_Div, nd_Mod, nd_Add, nd_Sub, nd_Lss, nd_Leq,
    nd_Gtr, nd_Geq, nd_Eql, nd_Neq, nd_And, nd_Or
} NodeType;
struct Tree {
    NodeType node_type;
    Tree *left;
    Tree *right;
    int value;
};
struct {
    char       *enum_text;
    NodeType   node_type;
} atr[] = {
    {"Identifier"  , nd_Ident,  },  {"String"      , nd_String,  },
    {"Integer"     , nd_Integer,},  {"Sequence"    , nd_Sequence,},
    {"If"          , nd_If,     },  {"Prtc"        , nd_Prtc,    },
    {"Prts"        , nd_Prts,   },  {"Prti"        , nd_Prti,    },
    {"While"       , nd_While,  },  {"Assign"      , nd_Assign,  },
    {"Negate"      , nd_Negate, },  {"Not"         , nd_Not,     },
    {"Multiply"    , nd_Mul,    },  {"Divide"      , nd_Div,     },
    {"Mod"         , nd_Mod,    },  {"Add"         , nd_Add,     },
    {"Subtract"    , nd_Sub,    },  {"Less"        , nd_Lss,     },
    {"LessEqual"   , nd_Leq,    },  {"Greater"     , nd_Gtr,     },
    {"GreaterEqual", nd_Geq,    },  {"Equal"       , nd_Eql,     },
    {"NotEqual"    , nd_Neq,    },  {"And"         , nd_And,     },
    {"Or"          , nd_Or,     },
};
Tree *make_node(NodeType node_type, Tree *left, Tree *right) {
    Tree *t = calloc(sizeof(Tree), 1);
    t->node_type = node_type;
    t->left = left;
    t->right = right;
    return t;
}
Tree *make_leaf(NodeType node_type, int value) {
    Tree *t = calloc(sizeof(Tree), 1);
    t->node_type = node_type;
    t->value = value;
    return t;
}
static char buf[2048];

/* Adds param to history history */
void addHistory(char* unused) {}

/* Requests new input from user */
char* readln(char* prompt) {
    fputs(prompt, stdout);
    fgets(buf, 2048, stdin);
    char* cpy = malloc(strlen(buf + 1));
    strcpy(cpy, buf);
    cpy[strlen(cpy)-1] = '\0';
    return cpy;
}
// ☘ 	 𝚫⛬  ⛶  ᐃ  	𝛥 𝚫
void printWelcome(char* version) {
    puts("\033[0;36m  \033[1;34m｜\033[0;33m 𝚫 \033[1;34mdito\033[1;35m+ \033[1;33mREPL\033[1;35m \033[0m by \033[0;34mDevisa \033[0;36m(2021)\033[0m");
    puts("\033[0;36m  \033[1;34m｜\033[1;32m Version \033[0m0.1.0-development\033[0m");
    puts("\033[0;36m  \033[1;34m｜\033[1;32m Authors \033[0mChris Pecunies\033[0m");
    puts("\033[0;36m  \033[1;34m｜");
    puts("\033[0;36m  \033[1;34m｜\033[1;32m About \033[0m");
    puts("\033[0;36m  \033[1;34m｜\033[0;34m\033[1;34m \033[0;33m∙\033[0m Press \033[0;33mCtrl+c\033[0m or run \033[0;34mexit\033[0m to exit.");
    puts("\033[0;36m  \033[1;34m｜\033[0;34m\033[1;34m \033[0;33m∙\033[0m Enter \033[0;33mhelp\033[0m for instructions.");
    puts("\033[0;36m  \033[1;34m｜\033[0;34m\033[1;34m \033[0;33m∙\033[0m More info can be found at \033[0;33mdevisa.io/dito\033[0m.");
    puts("\033[0;36m  \033[1;34m｜");
}
void ls(char* inp) {
    printf("\033[0;36m 1\033[1;34m｜\033[1;34minput: \033[0;32m[ls]\n");
    printf("\033[0;36m 2\033[1;34m｜\033[0;33m ∙ \033[0;33mcommand: \033[0mls\n");
    printf("\033[0;36m 2\033[1;34m｜\033[0;33m ∙ \033[0;33mexact: \033[0m%s\n", inp);
    printf("\033[0;36m 2\033[1;34m｜\n");
    printf("\033[0;36m 3\033[1;34m｜\033[1;34moutput: \033[0;32mls\n");
    printf("\033[0;36m 4\033[1;34m｜\033[0m");
    system(inp);
}
void cat(char* inp) {
    printf("\033[0;34m[+] \033[1;32mCAT:\033[0m Running cat %s\n", inp);
    system(inp);
}
void edit(char* inp) {
    printf("\033[0;34m[+] \033[1;32mEDIT:\033[0m Running edit cmd %s\n", inp);
    system(inp);
}
void echo(char* inp) {
    printf("\033[0;34m[+] \033[1;32mECHO:\033[0m Running echo cmd %s\n", inp);
    system(inp);
}
void sub() {
    printf("\033[0;34m[+] \033[1;32msub:    beween %s\n", "");
}
void find() {
    printf("\033[0;34m[+] \033[1;32mfind:\n");
}
void print_help() {
    printf("\033[0;36m 1\033[1;34m｜\033[1;32mHelp:\n");
    printf("\033[0;36m 2\033[1;34m｜\033[0;33m  ∙ about  \033[0m\t  The \033[0;34mdito\033[0m environment REPL.\n");
    printf("\033[0;36m 3\033[1;34m｜\033[0;33m  ∙ version\033[0m\t  Version 0.1.0\n");
    printf("\033[0;36m 4\033[1;34m｜\033[0;33m  ∙ website\033[0m   https://dito.devisa.io/docs\n");
    printf("\033[0;36m 4\033[1;34m｜\033[0;33m  ∙ repo   \033[0m   https://github.com/devisa/dito\n");
    printf("\033[0;36m 5\033[1;34m｜\033[0m\033[1;32mCommands:\n");
    printf("\033[0;36m 6\033[1;34m｜\033[0;33m  ∙ ls\t\033[0m  Lists current dir's files.\n");
    printf("\033[0;36m 7\033[1;34m｜\033[0;33m  ∙ evol\t\033[0m  Runs an evolutionary algo.\n");
    printf("\033[0;36m 8\033[1;34m｜\033[0;33m  ∙ help\t\033[0m  Prints this help message.\n");
    printf("\033[0;36m 9\033[1;34m｜\033[0;33m  ∙ quit\t\033[0m  Quits the dito shell.\n");
    printf("\033[0;36m10\033[1;34m｜\033[0m\033[1;32mOptions:\n");
    printf("\033[0;36m11\033[1;34m｜\033[0;33m  ∘ -v\t\033[0m  Sets output to verbose level 1.\n");
    printf("\033[0;36m12\033[1;34m｜\033[0;33m  ∘ -d\t\033[0m  Sets output to debug logs.\n");
}
// Db: ⛁
//  ∴
// \033[1;34m+
char* prompt() {
    char* prompt = "\033[1;34m \033[1;34m｜\033[0;32m\033[0;32m𝚫\033[1;34m dito \033[0;33mat\033[0;34m ~ \033[0;32m\033[1;33m ∴\033[0m ";
    return prompt;
}
void open() {
    FILE *infile, *outfile;
    int lineNo = 0;
    char line[512];
    const char *ifilename = "readings.txt";
    outfile = fopen("V0.txt", "w");
    infile = fopen(ifilename, "rb");
    if (!infile) {
        printf("Can't open %s\n", ifilename);
        exit(1);
    }
}
void exitDito() {
    exit(1);
}

int main(int argc, char** argv) {
    printWelcome("0.1.0");
    while(1) {
        char* inp = readln(prompt());
        addHistory(inp);
        if (startsWith(inp, "evol")) evol();
        else if (startsWith(inp, "ls")) ls(inp);
        else if (startsWith(inp, "echo")) echo(inp);
        else if (startsWith(inp, "cat")) cat(inp);
        else if ((startsWith(inp, "quit")) | (startsWith(inp, "exit"))) exitDito();
        else if (startsWith(inp, "open")) open();
        else if (startsWith(inp, "edit")) edit(inp);
        else if (startsWith(inp, "find")) find();
        else if (startsWith(inp, "sub")) sub();
        else if ((startsWith(inp, "help")) | (startsWith(inp, "h"))) print_help();
        else {
            char * inp_tok = strtok(inp, " ");
            char * inp_str = strstr(inp, " ");
            printf("\033[1;31mUnrecognized command:\033[0m %s\n", inp);
            printf("%s %s \n", inp_tok, inp_str);
        }
        free(inp);
    }
    return 0;
}

