#include "tree_parser.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

char* string(char* in) {
    char* out = calloc(strlen(in) + 1, sizeof(char));
    strcpy(out, in);
    return out;
}

TreeNode* newTreeNode(char* label, TreeNode* parent, int numChildren, TreeNode** children) {
    TreeNode* out = malloc(sizeof(TreeNode));
    out->label = label;
    out->parent = parent;
    out->numChildren = numChildren;
    out->children = children;
    return out;
}

char* addChar(char* in, char c) {
    int length = strlen(in);
    char* out = realloc(in, (length + 2) * sizeof(char));
    out[length] = c;
    out[length + 1] = (char) 0;
    return out;
}

TreeNode* getNode(int src, char* label, TreeNode* parent) {
    TreeNode* out = newTreeNode(label, parent, 0, NULL);
    out->children = malloc((out->numChildren) * sizeof(TreeNode*));
    bool whitespace = true;
    char c;
    while (read(src, &c, 1) > 0) {
        switch (c) {
            case '\t' :
            case '\n' :
            case '\r' :
            case ' ' : {
                whitespace = true;
                break;
            }
            case '"' : {
                whitespace = false;
                out->children = realloc(out->children, (++ out->numChildren) * sizeof(TreeNode*));
                out->children[out->numChildren - 1] = newTreeNode(string("\""), out, 0, NULL);
                while (read(src, &c, 1) > 0 && c != '"') {
                    out->children[out->numChildren - 1]->label = addChar(out->children[out->numChildren - 1]->label, c);
                }
                out->children[out->numChildren - 1]->label = addChar(out->children[out->numChildren - 1]->label, c);
                break;
            }
            case '{' : {
                whitespace = true;
                out->children = realloc(out->children, (++ out->numChildren) * sizeof(TreeNode*));
                out->children[out->numChildren - 1] = getNode(src, string("{}"), out);
                break;
            }
            case '}' : {
                return out;
                break;
            }
            case '(' : {
                whitespace = true;
                out->children = realloc(out->children, (++ out->numChildren) * sizeof(TreeNode*));
                out->children[out->numChildren - 1] = getNode(src, string("()"), out);
                break;
            }
            case ')' : {
                return out;
                break;
            }
            default : {
                if (whitespace) {
                    whitespace = false;
                    out->children = realloc(out->children, (++ out->numChildren) * sizeof(TreeNode*));
                    out->children[out->numChildren - 1] = newTreeNode(string(""), out, 0, NULL);
                }
                out->children[out->numChildren - 1]->label = addChar(out->children[out->numChildren - 1]->label, c);
                break;
            }
        }
    }
    return out;
}

void indentLine(int indent) {
    for (int i = 0 ; i < indent ; i ++) {
        printf("\t");
    }
}

void printNodes(TreeNode* root, int indent) {
    printf("\n");
    indentLine(indent);
    if (root->label == NULL) {
        printf("NULL");
    } else {
        printf("%s", root->label);
    }
    if (root->numChildren > 0) {
        printf(" : {");
        for (int i = 0 ; i < root->numChildren ; i ++) {
            printNodes(root->children[i], indent + 1);
            if (i < root->numChildren - 1) {
                printf(",");
            }
        }
        printf("\n");
        indentLine(indent);
        printf("}");
    }
}