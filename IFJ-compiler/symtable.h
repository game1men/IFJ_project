/*********************************************************************
 * \file   symtable.h
 * \brief  Header file for symbol table
 * 
 * \author Matej Smida xsmida06
 * \date   November 2022
 *********************************************************************/

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "utils.h"
#include "stack.h"

/**
 * \brief Structure of a binary tree node
 */
typedef struct BTnode {
    String* key;
    void* data;
    struct BTnode* leftPtr;
    struct BTnode* rightPtr;
} T_BTnode;

/*
* BINARY SEARCH TREE FUNCTIONS
*/

/**
 * \brief Initializes pointer to the root of the tree
 * 
 * \param tree Pointer to the root of the tree
 */
void BTinit(T_BTnode** tree);

/**
 * \brief Inserts a new node to the correct place in the tree or rewrite its content if it already exists.
 * 
 * \param tree Pointer to the root of the tree
 * \param key Key that tells us where to insert the node
 * \param data Data of the node
 * 
 * \return OK if succesfull or error message
 */
int BTinsert(T_BTnode** tree, String* key, void* data);

/**
 * \brief Searches the tree for specific node
 * 
 * \param tree Pointer to the root of the tree
 * \param key Key of node we are looking for
 * \param err Error holder for error messages
 * 
 * \return Data of node if found or NULL
 */
void* BTgetData(T_BTnode* tree, String* key, int* err);

/**
* \brief Searches the tree for specific node
* 
*  \param tree Pointer to the root of the tree
 * \param key Key of node we are looking for
 * \param err Boolean value for error (true if error occurs)
 * 
 * \return true if node was found, false otherwise
*/
bool BTsearch(T_BTnode* tree, String* key);
/**
 * \brief Deletes specific node in tree
 * 
 * \param tree Pointer to the root of the tree
 * \param key Key of node we want to delete
 * 
 * \return OK if succesfull or exception
 */
int BTdelete(T_BTnode* tree, String* key);

/**
 * \brief Frees the whole tree
 * 
 * \param tree Pointer to the root of the tree
 * 
 * \return OK if succesfull or error message
 */
int BTdispose(T_BTnode* tree, void (*ItemDisposer)(void *));

/**
 * \brief Replaces node with tho most righteous node in the tree/subtree. Used in delete function
 * 
 * \param tree Pointer to the root of the tree/subtree where we look for the most righteous node
 * \param target Target node
 * 
 * \return OK if succesfull or exception
 */
int BTreplace_by_rightmost(T_BTnode* tree, T_BTnode* target);

#endif // !SYMTABLE_H
