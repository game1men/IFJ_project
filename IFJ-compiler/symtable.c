/*********************************************************************
 * \file   symtable.c
 * \brief  Main file for symbol table. Implemented by using binary tree.
 * Some functions for binary tree operations have already been used in IAL second project and were written by me.
 * 
 * 
 * \author Matej Smida xsmida06
 * \date   November 2022
 *********************************************************************/

#include "symtable.h"


void BTinit(T_BTnode** tree) {
    *tree = NULL;
}

int BTinsert(T_BTnode** tree, String* key, void* data) {
    T_BTnode* tmp = *tree;

    // we are inserting root of the tree
    if (tmp == NULL) {
        T_BTnode* newNode = malloc(sizeof(T_BTnode));

        if (newNode == NULL) return INVALID_ALLOCATION_EXCEPTION;

        newNode->key = key;
        newNode->data = data;
        newNode->leftPtr = NULL;
        newNode->rightPtr = NULL;

        *tree = newNode;

        return OK;
    }

    // tree already contains some elements
    while (tmp != NULL) {

        if (CompareVarNames(tmp->key, key) == -1) {

            // insert element to the left
            if (tmp->leftPtr == NULL) {
                T_BTnode* newNode = malloc(sizeof(T_BTnode));

                if (newNode == NULL) return INVALID_ALLOCATION_EXCEPTION;

                newNode->key = key;
                newNode->data = data;
                newNode->leftPtr = NULL;
                newNode->rightPtr = NULL;

                tmp->leftPtr = newNode;
                return OK;
            }
            // parent has left son so we continue left
            else {
                tmp = tmp->leftPtr;
            }
        } else if (CompareVarNames(tmp->key, key) == 1) {

            // insert element to the right
            if (tmp->rightPtr == NULL) {
                T_BTnode* newNode = malloc(sizeof(T_BTnode));

                if (newNode == NULL) return INVALID_ALLOCATION_EXCEPTION;

                newNode->key = key;
                newNode->data = data;
                newNode->leftPtr = NULL;
                newNode->rightPtr = NULL;

                tmp->rightPtr = newNode;
                return OK;
            }
            // parent has right son so we continue right
            else {
                tmp = tmp->rightPtr;
            }
        }
        // element is already in the tree => overwrite data
        else {
            tmp->data = data;
            return OK;
        }
    }
    return OK;
}

void* BTsearch(T_BTnode* tree, String* key, int* err) {
    while (tree != NULL) {

        if (CompareVarNames(tree->key, key) == -1) {

            // we go to the left (if we can, otherwise return NULL)
            if (tree->leftPtr == NULL) {
                *err = NULL_POINTER_EXCEPTION;
                return NULL;
            }
            tree = tree->leftPtr;
        } else if (CompareVarNames(tree->key, key) == 1) {

            // we go to the right (if we can, otherwise return NULL)
            if (tree->rightPtr == NULL) {
                *err = NULL_POINTER_EXCEPTION;
                return NULL;
            }
            tree = tree->rightPtr;
        }
        // found the element
        else {
            *err = OK;
            return tree->data;
        }
    }
    // tree is NULL
    *err = NULL_POINTER_EXCEPTION;
    return NULL;
}

int BTdelete(T_BTnode* tree, String* key) {

    if (tree == NULL) return NULL_POINTER_EXCEPTION;

    T_BTnode* child = tree;
    T_BTnode* parent = NULL;

    while (child != NULL) {

        if (CompareVarNames(child->key, key) == -1) {

            // we are going to the left (if doesnt have left subtree return nothing)
            if (child->leftPtr == NULL) {
                return OK;
            }
            parent = child;
            child = child->leftPtr;
        } else if (CompareVarNames(child->key, key) == 1) {

            // we are going to the right (if doesnt have right subtree return nothing)
            if (child->rightPtr == NULL) {
                return OK;
            }
            parent = child;
            child = child->rightPtr;
        }
        // we have found the element
        else {

            // element doesnt have subtrees
            if (child->leftPtr == NULL && child->rightPtr == NULL) {

                // we are deleting the only element in the tree (root)
                if (parent == NULL) {
                    free(child);
                    return OK;
                }
                // element is in fathers LEFT subtree
                if (parent->leftPtr == child) {
                    parent->leftPtr = NULL;
                }
                // element is in fathers RIGHT subtree
                else if (parent->rightPtr == child) {
                    parent->rightPtr = NULL;
                }

                free(child);
                return OK;
            }
            // element has ONE subtree
            else if ((child->leftPtr != NULL && child->rightPtr == NULL) ||
                     (child->leftPtr == NULL && child->rightPtr != NULL)) {

                // element has LEFT subtree
                if (child->leftPtr != NULL) {

                    if (parent->leftPtr == child) {
                        parent->leftPtr = child->leftPtr;
                        free(child);
                        return OK;
                    } else if (parent->rightPtr == child) {
                        parent->rightPtr = child->leftPtr;
                        free(child);
                        return OK;
                    }
                }
                // element has RIGHT subtree
                else if (child->rightPtr != NULL) {

                    if (parent->leftPtr == child) {
                        parent->leftPtr = child->rightPtr;
                        free(child);
                        return OK;
                    } else if (parent->rightPtr == child) {
                        parent->rightPtr = child->rightPtr;
                        free(child);
                        return OK;
                    }
                }
            }
            // element has both subtrees
            else {
                int err = BTreplace_by_rightmost(child->leftPtr, child);

                if (err != OK) return err;
            }
        }
    }
    return OK;
}

int BTdispose(T_BTnode* tree, void (*ItemDisposer)(void *)) {

    if (tree == NULL) return NULL_POINTER_EXCEPTION;

    Stack stack;
    InitStack(&stack);

    int err;

    // push root of the tree into stack
    err = Push(&stack, tree);

    if (err != OK) return err;

    while (!IsEmpty(&stack, &err)) {

        if (err != OK) return err;

        T_BTnode* tmp = Pop(&stack, &err);

        if (err != OK) return err;

        // if the element has left subtree we push it into stack
        if (tmp->leftPtr != NULL) {
           err = Push(&stack, tmp->leftPtr);

           if (err != OK) return err;
        }
        // if the element has right subtree we push it into stack
        if (tmp->rightPtr != NULL) {
           err = Push(&stack, tmp->rightPtr);

           if (err != 0) return err;
        }

        // free the element
        free(tmp);
    }
    // everything is freed, set pointer to tree to NULL
    tree = NULL;

    DisposeStack(&stack, ItemDisposer);
    
    return OK;
}

int BTreplace_by_rightmost(T_BTnode* tree, T_BTnode* target) {

    if (tree == NULL || target == NULL) return NULL_POINTER_EXCEPTION;

    T_BTnode* tmp = tree;
    T_BTnode* parent;

    while (tmp != NULL) {

        // we have found the most right element
        if (tmp->rightPtr == NULL) {

            // element doesnt have left subtree
            if (tmp->leftPtr == NULL) {
                target->key = tmp->key;
                target->data = tmp->data;

                parent->rightPtr = NULL;
                free(tmp);
                return OK;
            }
            // element has left subtree
            else {
                target->key = tmp->key;
                target->data = tmp->data;

                // so we will give it to his father
                parent->rightPtr = tmp->leftPtr;
                free(tmp);
                return OK;
            }
        }

        //continue to the right
        parent = tmp;
        tmp = tmp->rightPtr;
    }
    return OK;
}
