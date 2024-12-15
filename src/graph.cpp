#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree.h"
#include "graph.h"
#include "operations.h"
#include "reader.h"

void DrawGraph(Tree *tree, char *dest_picture_path)
{
    assert(tree);
    assert(dest_picture_path);

    FILE *dot_file = fopen(TMP_DOTFILE_NAME, "w");

    fprintf(dot_file, "digraph G{           \n" 
                        "bgcolor = \"%s\";  \n" , BACKGROUND_COLOR);

    InitNodesInDot(tree, dot_file);
    MakeLinksInDot(tree, dot_file);

    fprintf(dot_file, "} \n");

    fclose(dot_file);

    MakeGraphPicture(TMP_DOTFILE_NAME, dest_picture_path);

    fclose(dot_file);
}

// void InitNodesInDot(Tree *tree, FILE *dot_file)
// {
//     assert(tree);
//     assert(dot_file);
//     assert(tree->node_ptrs);

//     for (size_t i = 0; i < tree->size; i++)
//     {
//         Node *cur_node = tree->node_ptrs[i];

//         if (cur_node == NULL)
//             continue;

//         char *node_val_str = NodeValToStr(cur_node);

//         if (cur_node->type == OP)
//         {
//             if (cur_node->val.op->type == BINARY)
//                 fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"{%s | { <%s> %s | <%s> %s } }\"]\n",
//                     NODE_NAME_PREFIX, cur_node, OP_NODE_SHAPE, OP_NODE_COLOR, node_val_str, LEFT_MARK, LEFT_MARK, RIGHT_MARK, RIGHT_MARK);
            
//             else
//                 fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"{%s | { <%s> %s } }\"]\n",
//                     NODE_NAME_PREFIX, cur_node, OP_NODE_SHAPE, OP_NODE_COLOR, node_val_str, LEFT_MARK, UNIQ_ARG_MARK);
//         }

//         // else if (cur_node->type == NEW_EXPR)
//         //     fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"{%s | { <%s> %s | <%s> %s } }\"]\n",
//         //             NODE_NAME_PREFIX, cur_node, OP_NODE_SHAPE, NEW_EXPR_NODE_COLOR, "new_expr", LEFT_MARK, LEFT_MARK, RIGHT_MARK, RIGHT_MARK);

//         else if (cur_node->type == KEY_WORD)
//             fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"{%s | { <%s> %s | <%s> %s } }\"]\n",
//                     NODE_NAME_PREFIX, cur_node, KEY_WORD_SHAPE, KEY_WORD_COLOR, node_val_str, LEFT_MARK, LEFT_MARK, RIGHT_MARK, RIGHT_MARK);
                
//         else if (cur_node->type == VAR || cur_node->type == FUNC || cur_node->type == VAR_OR_FUNC)
//             fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"%s\"]\n",
//                 NODE_NAME_PREFIX, cur_node, VAR_NODE_SHAPE, VAR_NODE_COLOR, node_val_str);
                
//         else if (cur_node->type == NUM)
//         {
//             fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"%s\"]\n",
//                 NODE_NAME_PREFIX, cur_node, NUM_NODE_SHAPE, NUM_NODE_COLOR, node_val_str);
//         }

//         else if (cur_node->type == TYPE_INDICATOR)
//             fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"{%s | { <%s> %s | <%s> %s } }\"]\n",
//                     NODE_NAME_PREFIX, cur_node, TYPEOF_NAMED_NODE_SHAPE, TYPEOF_NAMED_NODE_COLOR, node_val_str, LEFT_MARK, LEFT_MARK, RIGHT_MARK, RIGHT_MARK);
//             // fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"{ <%s> %s}\"]\n",
//                     // NODE_NAME_PREFIX, cur_node, TYPEOF_NAMED_NODE_SHAPE, TYPEOF_NAMED_NODE_COLOR, LEFT_MARK, node_val_str);

//         else if (cur_node->type == MANAGER)
//             fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"%s\"]\n",
//                 NODE_NAME_PREFIX, cur_node, MANAGER_NODE_SHAPE, MANAGER_NODE_COLOR, node_val_str);

//         // else if (cur_node->type == EOT)
//             // fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"%s\"]\n",
//                 // NODE_NAME_PREFIX, cur_node, EOT_NODE_SHAPE, EOT_NODE_COLOR, node_val_str);

//         else // if (cur_node->type == POISON_TYPE)
//             fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"%s\"]\n",
//                 NODE_NAME_PREFIX, cur_node, POISON_NODE_SHAPE, POISON_NODE_COLOR, node_val_str);
//     }
// }
void InitNodesInDot(Tree *tree, FILE *dot_file)
{
    assert(tree);
    assert(dot_file);
    assert(tree->node_ptrs);

    for (size_t i = 0; i < tree->size; i++)
    {
        Node *cur_node = tree->node_ptrs[i];

        if (cur_node == NULL)
            continue;

        char *node_val_str = NodeValToStr(cur_node);

        if (cur_node->type == OP)
        {
            if (cur_node->val.op->type == BINARY)
                fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"{%s | { <%s> %s | <%s> %s } }\"]\n",
                    NODE_NAME_PREFIX, cur_node, OP_NODE_SHAPE, OP_NODE_COLOR, node_val_str, LEFT_MARK, LEFT_MARK, RIGHT_MARK, RIGHT_MARK);
            
            else
                fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"{%s | { <%s> %s } }\"]\n",
                    NODE_NAME_PREFIX, cur_node, OP_NODE_SHAPE, OP_NODE_COLOR, node_val_str, LEFT_MARK, UNIQ_ARG_MARK);
        }

        // else if (cur_node->type == NEW_EXPR)
        //     fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"{%s | { <%s> %s | <%s> %s } }\"]\n",
        //             NODE_NAME_PREFIX, cur_node, OP_NODE_SHAPE, NEW_EXPR_NODE_COLOR, "new_expr", LEFT_MARK, LEFT_MARK, RIGHT_MARK, RIGHT_MARK);

        else if (cur_node->type == KEY_WORD)
            fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"{%s | { <%s> %s | <%s> %s } }\"]\n",
                    NODE_NAME_PREFIX, cur_node, KEY_WORD_SHAPE, KEY_WORD_COLOR, node_val_str, LEFT_MARK, LEFT_MARK, RIGHT_MARK, RIGHT_MARK);
                
        else if (cur_node->type == VAR || cur_node->type == FUNC || cur_node->type == VAR_OR_FUNC)
            fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"%s\"]\n",
                NODE_NAME_PREFIX, cur_node, VAR_NODE_SHAPE, VAR_NODE_COLOR, node_val_str);
                
        else if (cur_node->type == NUM)
        {
            fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"%s\"]\n",
                NODE_NAME_PREFIX, cur_node, NUM_NODE_SHAPE, NUM_NODE_COLOR, node_val_str);
        }

        // else if (cur_node->type == TYPE_INDICATOR)
        //     fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"{%s | { <%s> %s | <%s> %s } }\"]\n",
        //             NODE_NAME_PREFIX, cur_node, TYPEOF_NAMED_NODE_SHAPE, TYPEOF_NAMED_NODE_COLOR, node_val_str, LEFT_MARK, LEFT_MARK, RIGHT_MARK, RIGHT_MARK);
            // fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"{ <%s> %s}\"]\n",
                    // NODE_NAME_PREFIX, cur_node, TYPEOF_NAMED_NODE_SHAPE, TYPEOF_NAMED_NODE_COLOR, LEFT_MARK, node_val_str);

        else if (cur_node->type == MANAGER)
            fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"%s\"]\n",
                NODE_NAME_PREFIX, cur_node, MANAGER_NODE_SHAPE, MANAGER_NODE_COLOR, node_val_str);

        // else if (cur_node->type == EOT)
            // fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"%s\"]\n",
                // NODE_NAME_PREFIX, cur_node, EOT_NODE_SHAPE, EOT_NODE_COLOR, node_val_str);

        else // if (cur_node->type == POISON_TYPE)
            fprintf(dot_file, "%s%p [shape = \"%s\", style = filled, fillcolor = \"%s\", label = \"%s\"]\n",
                NODE_NAME_PREFIX, cur_node, POISON_NODE_SHAPE, POISON_NODE_COLOR, node_val_str);
    }
}

void MakeLinksInDot(Tree *tree, FILE *dot_file)
{
    assert(tree);
    assert(dot_file);

    for (size_t i = 0; i < tree->size; i++)
    {
        Node *cur_node  = tree->node_ptrs[i];
        
        if (cur_node == NULL)
            continue;

        Node *left_son  = cur_node->left;
        Node *right_son = cur_node->right;

        if (left_son  != NULL)
            fprintf(dot_file, "%s%p: <%s> -> %s%p\n", NODE_NAME_PREFIX, cur_node, LEFT_MARK,  NODE_NAME_PREFIX, left_son);

        if (right_son != NULL)
            fprintf(dot_file, "%s%p: <%s> -> %s%p\n", NODE_NAME_PREFIX, cur_node, RIGHT_MARK, NODE_NAME_PREFIX, right_son);
    }
}

void MakeGraphPicture(const char *dotfile_path, const char *picture_path)
{
    assert(dotfile_path);
    assert(picture_path);

    char cmd_command[CMD_COMMAND_LEN] = {};

    sprintf(cmd_command, "dot %s -T png -o %s\n", dotfile_path, picture_path);
    system(cmd_command);
}