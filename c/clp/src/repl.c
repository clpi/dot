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


void add_vertex (graph_t *g, int i) {
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
void add_edge (graph_t *g, int a, int b, int w) {
    a = a - 'a';
    b = b - 'a';
    add_vertex(g, a);
    add_vertex(g, b);
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
void printWelcome(char* version) {
    printf("\n\033[1;32mdito REPL: \033[0;34mVersion \033[0;34m%s\033[0m\n", version);
    puts("\033[0;34m[1] \033[0mPress \033[0;33mCtrl+c\033[0m to exit.");
    puts("\033[0;34m[2] \033[0mEnter \033[0;33mhelp\033[0m for instructions.");
    puts("\033[0;34m[3] \033[0mMore info can be found at \033[0;33mdevisa.io/dito\033[0m.\n");
}
void ls() {
    printf("\033[0;34m[+] \033[1;32mLS:\n");
}
void print_help() {
    printf("\033[0;34m[+] \033[1;32mHelp:\n");
    printf("\033[0;34m[+] \033[0;33m\tAbout\033[0m The \033[0;35mdito\033[0m environment REPL.\n");
    printf("\033[0;34m[+] \033[0;33m\tVersion\033[0m 0.1.0\n");
    printf("\033[0;34m[+] \033[0;33m\tWebsite\033[0m https://dito.devisa.io/docs\n");
    printf("\033[0;34m[+] \033[0m");
    printf("\033[1;32mCommands:\n");
    printf("\033[0;34m[+] \033[0;33m\tls\t\033[0m- Lists current dir's files.\n");
    printf("\033[0;34m[+] \033[0;33m\tevol\t\033[0m- Runs an evolutionary algo.\n");
    printf("\033[0;34m[+] \033[0;33m\thelp\t\033[0m- Prints this help message.\n");
    printf("\033[0;34m[+] \033[0;33m\tquit\t\033[0m- Quits the dito shell.\n");
    printf("\033[0;34m[+] \033[0m");
    printf("\033[1;32mOptions:\n");
    printf("\033[0;34m[+] \033[0;33m\t-v\t\033[0m- Sets output to verbose level 1.\n");
    printf("\033[0;34m[+] \033[0;33m\t-d\t\033[0m- Sets output to debug logs.\n");
}

int main(int argc, char** argv) {
    printWelcome("0.1.0");
    char* prompt = "\n\033 [1;32m[dito] \033[1;34m~ >\033[0m ";
    while(1) {
        char* inp = readln(prompt);
        addHistory(inp);
        if (strcmp(inp, "evol") == 0) evol();
        else if (strcmp(inp, "ls") == 0) ls();
        else if ((strcmp(inp, "help") == 0) | (strcmp(inp, "h") == 0)) print_help();
        else printf("\033[1;31mUnrecognized command:\033[0m %s\n", inp);
        free(inp);
    }
    return 0;
}

