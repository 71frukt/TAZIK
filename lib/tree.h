#ifndef DIFF_TREE_H
#define DIFF_TREE_H

#include <stdio.h>
#include <uchar.h>

#define LANG_DEBUG

#include "tree_elem_t_info.h"

#include "operations.h"

const int TREE_ALLOC_MARKS_NUM = 20;
const int START_TREE_SIZE      = 100;
const int TOKEN_LEN            = 50;

const int START_NAMES_TABLE_CAPA = 10;

const TreeElem_t POISON_VAL = 0xDEB11;

#define LEFT_MARK      "L"
#define RIGHT_MARK     "R"
#define UNIQ_ARG_MARK  "U"
#define EOT_SYMBOL     "$"
#define POISON_SYMBOL  "POISON"

#define BASE_INPUT_FILE_NAME   "source_file.txt"

#ifdef LANG_DEBUG
#define ON_LANG_DEBUG(...)  __VA_ARGS__
#else
#define ON_LANG_DEBUG(...)
#endif


enum NodeType
{
    NUM,
    VAR,
    OP,
    INIT,
    MANAGER,
    NEW_EXPR,
    NAMED_NODE_TYPE,

    POISON_TYPE,
    EOT
};


union NodeVal
{
    TreeElem_t         num;
    char              *prop_name;
    const Operation   *op;
    const Initializer *init;
    const ManageElem  *manager;
    const char        *new_expr;
    NamedNode          named_node_type;
};

struct Node
{
    NodeType type;

    NodeVal  val;

    Node    *left;
    Node    *right;
};

struct ProperName
{
    char name[TOKEN_LEN];
};

struct NamesTable
{
    ProperName *names;
    size_t size;
    size_t capacity;
};

struct TreeAllocMarks
{
    Node   *data[TREE_ALLOC_MARKS_NUM];
    size_t  size;
};

struct Tree
{
    Node **node_ptrs;
    Node *root_ptr;

    size_t capacity;
    size_t size;

    NamesTable names_table;

    TreeAllocMarks alloc_marks;

    ON_LANG_DEBUG(const char *name);
};

void    TreeCtor      (Tree *tree, size_t start_capacity ON_LANG_DEBUG(, const char *name));
void    TreeDtor      (Tree *tree);
void    TreeRecalloc  (Tree *tree, size_t new_capacity);
Node   *NewNode       (Tree *tree, NodeType type, NodeVal val, Node *left, Node *right);
void    RemoveNode    (Tree *tree, Node **node);
void    RemoveSubtree (Tree *tree, Node **start_node);
Node   *TreeCopyPaste (Tree *source_tree, Tree *dest_tree, Node *coping_node);
size_t  GetTreeHeight (Node *cur_node);
char   *NodeValToStr  (Node *node);

ProperName *FindNameInTable (NamesTable *table, char *name);
ProperName *NewNameInTable  (NamesTable *table, char *name);

void    NamesTableCtor  (size_t start_capa, NamesTable *table);
void    NamesTableDtor  (NamesTable *table);

bool    SubtreeContainsType(Node *cur_node, NodeType type);
bool    OpNodeIsCommutativity(Node *op_node);

#endif