/* The Computer Language Benchmarks Game
 * https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Kevin Carson
   compilation:
       gcc -O3 -fomit-frame-pointer -funroll-loops -static binary-trees.c -lm
       icc -O3 -ip -unroll -static binary-trees.c -lm

   *reset*
*/

/* modified by @jsinger for CHERI example allocators */
#include "freelist_allocator.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct tn
{
    struct tn *left;
    struct tn *right;
} treeNode;

treeNode *NewTreeNode(treeNode *left, treeNode *right)
{
    treeNode *new;

    new = (treeNode *) alloc(sizeof(treeNode));

    new->left = left;
    new->right = right;

    return new;
} /* NewTreeNode() */

long ItemCheck(treeNode *tree)
{
    if (tree->left == NULL)
        return 1;
    else
        return 1 + ItemCheck(tree->left) + ItemCheck(tree->right);
} /* ItemCheck() */

treeNode *BottomUpTree(unsigned depth)
{
    if (depth > 0)
        return NewTreeNode(BottomUpTree(depth - 1), BottomUpTree(depth - 1));
    else
        return NewTreeNode(NULL, NULL);
} /* BottomUpTree() */

void DeleteTree(treeNode *tree)
{
    if (tree->left != NULL)
    {
        DeleteTree(tree->left);
        DeleteTree(tree->right);
    }

    dealloc(tree);
} /* DeleteTree() */

int main(int argc, char *argv[])
{
    unsigned N, depth, minDepth, maxDepth, stretchDepth;
    treeNode *stretchTree, *longLivedTree, *tempTree;
    unsigned pages; /* mem required */

    N = (argc > 1) ? atol(argv[1]) : 0;

    minDepth = 4;

    if ((minDepth + 2) > N)
        maxDepth = minDepth + 2;
    else
        maxDepth = N;

    stretchDepth = maxDepth + 1;

    /* calculate mem requirements, with allocator-specific
     * size-class assumptions
     */
    pages = ((2 << (stretchDepth + 3)) * sizeof(treeNode)) / BYTES_IN_PAGE;
    printf("treenode size is %u bytes\n", (unsigned int) sizeof(treeNode));
    printf("we need %u pages\n", pages);

    /* allocate memory pool */
    initialize(pages);

    /* start creating tree data structures */
    stretchTree = BottomUpTree(stretchDepth);
    printf("stretch tree of depth %u\t check: %li\n", stretchDepth, ItemCheck(stretchTree));

    DeleteTree(stretchTree);

    longLivedTree = BottomUpTree(maxDepth);

    for (depth = minDepth; depth <= maxDepth; depth += 2)
    {
        long i, iterations, check;

        iterations = pow(2, maxDepth - depth + minDepth);

        check = 0;

        for (i = 1; i <= iterations; i++)
        {
            tempTree = BottomUpTree(depth);
            check += ItemCheck(tempTree);
            DeleteTree(tempTree);
        } /* for(i = 1...) */

        printf("%li\t trees of depth %u\t check: %li\n", iterations, depth, check);
    } /* for(depth = minDepth...) */

    printf("long lived tree of depth %u\t check: %li\n", maxDepth, ItemCheck(longLivedTree));

    return 0;
} /* main() */
