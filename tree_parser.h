#ifndef TREE_PARSER_H
#define TREE_PARSER_H

typedef int bool;
#define true 1
#define false 0

typedef struct TreeNode TreeNode;
struct TreeNode {
    char* label;
    TreeNode* parent;
    int numChildren;
    TreeNode** children;
};

char* string(char* in);

TreeNode* newTreeNode(char* label, TreeNode* parent, int numChildren, TreeNode** children);

char* addChar(char* in, char c);

TreeNode* getNode(int src, char* label, TreeNode* parent);

void indentLine(int indent);

void printNodes(TreeNode* root, int indent);

#endif//TREE_PARSER_H