#include <stdlib.h>
#include <stdio.h>
#include "skiplist.h"

void defaultHandler(zskiplistNode *node)
{
}

/* Create a skiplist node with the specified number of levels. */
zskiplistNode *zslCreateNode(int level, unsigned long score, handler_pt func)
{
    zskiplistNode *zn =
        malloc(sizeof(*zn) + level * sizeof(struct zskiplistLevel));
    zn->score = score;
    zn->handler = func;
    return zn;
}

zskiplist *zslCreate(void)
{
    int j;
    zskiplist *zsl;

    zsl = malloc(sizeof(*zsl));
    zsl->level = 1;
    zsl->length = 0;
    zsl->header = zslCreateNode(ZSKIPLIST_MAXLEVEL, 0, defaultHandler);
    for (j = 0; j < ZSKIPLIST_MAXLEVEL; j++)
    {
        zsl->header->level[j].forward = NULL;
    }
    return zsl;
}

/* Free a whole skiplist. */
void zslFree(zskiplist *zsl)
{
    zskiplistNode *node = zsl->header->level[0].forward, *next;

    free(zsl->header);
    while (node)
    {
        next = node->level[0].forward;
        free(node);
        node = next;
    }
    free(zsl);
}

int zslRandomLevel(void)
{
    int level = 1;
    while ((arc4random() & 0xFFFF) < (ZSKIPLIST_P * 0xFFFF))
        level += 1;
    return (level < ZSKIPLIST_MAXLEVEL) ? level : ZSKIPLIST_MAXLEVEL;
}

zskiplistNode *zslInsert(zskiplist *zsl, unsigned long score, handler_pt func)
{
    zskiplistNode *update[ZSKIPLIST_MAXLEVEL], *x;
    int i, level;

    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--)
    {
        while (x->level[i].forward &&
               x->level[i].forward->score < score)
        {
            x = x->level[i].forward;
        }
        update[i] = x;
    }
    level = zslRandomLevel();
    printf("zskiplist add node level = %d\n", level);
    if (level > zsl->level)
    {
        for (i = zsl->level; i < level; i++)
        {
            update[i] = zsl->header;
        }
        zsl->level = level;
    }
    x = zslCreateNode(level, score, func);
    for (i = 0; i < level; i++)
    {
        x->level[i].forward = update[i]->level[i].forward;
        update[i]->level[i].forward = x;
    }

    zsl->length++;
    return x;
}

zskiplistNode *zslMin(zskiplist *zsl)
{
    zskiplistNode *x;
    x = zsl->header;
    return x->level[0].forward;
}

void zslDeleteHead(zskiplist *zsl)
{
    zskiplistNode *update[ZSKIPLIST_MAXLEVEL];
    zskiplistNode *x = zslMin(zsl);
    if (!x)
        return;
    int i;
    for (i = zsl->level - 1; i >= 0; i--)
    {
        if (zsl->header->level[i].forward == x)
        {
            zsl->header->level[i].forward = x->level[i].forward;
        }
    }
    while (zsl->level > 1 && zsl->header->level[zsl->level - 1].forward == NULL)
        zsl->level--;
    zsl->length--;
}

void zslDeleteNode(zskiplist *zsl, zskiplistNode *x, zskiplistNode **update)
{
    int i;
    for (i = 0; i < zsl->level; i++)
    {
        if (update[i]->level[i].forward == x)
        {
            update[i]->level[i].forward = x->level[i].forward;
        }
    }
    while (zsl->level > 1 && zsl->header->level[zsl->level - 1].forward == NULL)
        zsl->level--;
    zsl->length--;
}

void zslDelete(zskiplist *zsl, zskiplistNode *zn)
{
    zskiplistNode *update[ZSKIPLIST_MAXLEVEL], *x;
    int i;

    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--)
    {
        while (x->level[i].forward &&
               x->level[i].forward->score < zn->score)
        {
            x = x->level[i].forward;
        }
        update[i] = x;
    }
    x = x->level[0].forward;
    if (x && zn->score == x->score)
    {
        zslDeleteNode(zsl, x, update);
        free(x);
    }
}

void zslPrint(zskiplist *zsl)
{
    zskiplistNode *x;
    x = zsl->header;
    x = x->level[0].forward;
    printf("start print skiplist level = %d\n", zsl->level);
    int i;
    for (i = 0; i < zsl->length; i++)
    {
        printf("skiplist ele %d: score = %lu\n", i + 1, x->score);
        x = x->level[0].forward;
    }
}
