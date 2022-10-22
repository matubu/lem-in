#include "lem_in.h"

pair make_pair(int first, int second) {
    pair p;
    p.first = first;
    p.second = second;
    return p;
}

node *new_node(pair value, int col) {
    node *n = malloc(sizeof(node));
    n->value = value;
    n->col = col;
    n->par = NULL;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void init_priority_queue(priority_queue *pq, int (*cmp)(int, int)) {
    pq->root = NULL;
    pq->size = 0;
    pq->cmp = cmp;
}

void clear_node(node *n) {
    if (!n)
        return ;
    clear_node(n->left);
    clear_node(n->right);
    free(n);
}

void clear_priority_queue(priority_queue *pq) {
    clear_node(pq->root);
    pq->root = NULL;
    pq->size = 0;
}

node *top(priority_queue *pq) {
    if (!pq->root)
        return NULL;
    node *cur = pq->root;
    while (cur->left)
        cur = cur->left; 
    return cur;
}

void pop(priority_queue *pq) {
    node *cur = top(pq);

    if (!cur)
        return ;
    if (cur == pq->root) {
        pq->root = pq->root->right;
        if (pq->root)
            pq->root->par = NULL;
    }
    else {
        cur->par->left = cur->right;
        if (cur->right)
            cur->right->par = cur->par;
    }
    free(cur);
    pq->size--;
}

void push(priority_queue *pq, pair elem) {
    node *new = new_node(elem, 1);
    if (!new) {
        clear_priority_queue(pq);
        return ;
    }
    if (!pq->root) {
        new->col = 0;
        pq->root = new;
    } else {
        node *cur = pq->root;
        while (1) {
            if (pq->cmp(new->value.first, cur->value.first)) {
                if (cur->left)
                    cur = cur->left;
                else {
                    cur->left = new;
                    new->par = cur;
                    break;
                }
            } else {
                if (cur->right)
                    cur = cur->right;
                else {
                    cur->right = new;
                    new->par = cur;
                    break;
                }
            }
        }
    }
    pq->size++;
}

int less(int a, int b) {
    return a < b;
}

int greater(int a, int b) {
    return a > b;
}