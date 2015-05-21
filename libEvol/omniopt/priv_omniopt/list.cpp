/* A custom doubly linked OmniOptOptimizer::list implemenation */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptOptimizer.h"
#include "rand.h"

/* Insert an element X into the OmniOptOptimizer::list at location specified by NODE */
void OmniOptOptimizer::insert (OmniOptOptimizer::list *node, int x)
{
    OmniOptOptimizer::list *temp;
    if (node==NULL)
    {
        printf("\n Error!! asked to enter after a NULL pointer, hence exiting \n");
        exit(1);
    }
    temp = (OmniOptOptimizer::list *)malloc(sizeof(OmniOptOptimizer::list));
    temp->index = x;
    temp->child = node->child;
    temp->parent = node;
    if (node->child != NULL)
    {
        node->child->parent = temp;
    }
    node->child = temp;
    return;
}

/* Delte the node NODE from the OmniOptOptimizer::list */
OmniOptOptimizer::list* OmniOptOptimizer::del (OmniOptOptimizer::list *node)
{
    OmniOptOptimizer::list *temp;
    if (node==NULL)
    {
        printf("\n Error!! asked to delete a NULL pointer, hence exiting \n");
        exit(1);
    }
    temp = node->parent;
    temp->child = node->child;
    if (temp->child!=NULL)
    {
        temp->child->parent = temp;
    }
    free (node);
    return (temp);
}
