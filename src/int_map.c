#include "int_map.h"


/*
typedef struct s_int {
	size_t	id;
	size_t	x;
	size_t	y;
}	t_int;
*/


int_pair make_int_pair(int first, int second) {
    int_pair p;
    p.first = first;
    p.second = second;
    return p;
}

int_node *new_int_node(int_pair value, int col) {
    int_node *n = malloc(sizeof(int_node));
    if (!n)
        return NULL;
    n->value = value;
    n->col = col;
    n->par = NULL;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void init_int_map(int_map *mp, int (*cmp)(int, int)) {
    mp->root = NULL;
    mp->size = 0;
    mp->cmp = cmp;
}

void clear_int_node(int_node *n) {
    if (!n)
        return ;
    clear_int_node(n->left);
    clear_int_node(n->right);
    free(n);
}

void clear_int_map(int_map *mp) {
    clear_int_node(mp->root);
    mp->root = NULL;
    mp->size = 0;
}

int_node *top_int(int_map *mp) {
    if (!mp->root)
        return NULL;
    int_node *cur = mp->root;
    while (cur->left)
        cur = cur->left; 
    return cur;
}

void pop_int(int_map *pq) {
    int_node *cur = top_int(pq);

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



void	leftRotate_int(int_map *mp, int_node *node) {
	if (!node || !node->right)
		return ;
	int_node *y = node->right;
	node->right = y->left;
	if (y->left)
		y->left->par = node;
	if (node == mp->root)
		mp->root = y;
	else if (node == node->par->left)
		node->par->left = y;
	else
		node->par->right = y;
	y->par = node->par;
	node->par = y;
	y->left = node;
}

void	rightRotate_int(int_map *mp, int_node *node) {
	if (!node || !node->left)
		return ;
	int_node *y = node->left;
	node->left = y->right;
	if (y->right)
		y->right->par = node;
	if (node == mp->root)
		mp->root = y;
	else if (node == node->par->left)
		node->par->left = y;
	else
		node->par->right = y;
	y->par = node->par;
	node->par = y;
	y->right = node;
}

void leftRightRotate_int(int_map *mp, int_node *node) {
	if (!node || !node->left || !node->left->right)
		return ;
	leftRotate_int(mp, node->left);
}

void rightLeftRotate_int(int_map *mp, int_node *node) {
	if (!node || !node->right || !node->right->left)
		return ;
	rightRotate_int(mp, node->right);
}

void insertFix_int(int_map *mp, int_node *node) {
	if (!node->par || !node->par->par) 
		return;
	while (node->par && node->par->col == RED && node->par->par) {
		int_node *p = node->par, *gp = node->par->par;
		if (p == gp->left)
			if (gp->right && gp->right->col == RED) {
				gp->right->col = BLACK;
				gp->left->col = BLACK;
				gp->col = RED;
				node = gp;
			} else {
				if (node == p->right) {
					node = p;
					leftRotate_int(mp, node);
				}
				if (node->par->par) { 
					node->par->col = BLACK;
					node->par->par->col = RED;
					rightRotate_int(mp, node->par->par);
				}
			} else {
				if (gp->left && gp->left->col == RED) {
					gp->left->col = BLACK;
					gp->right->col = BLACK;
					gp->col = RED;
					node = gp;
				} else {
					if (node == p->left) {
						node = p;
						rightRotate_int(mp, node);
					}
					if (node->par->par) {
						node->par->col = BLACK;
						node->par->par->col = RED;
						leftRotate_int(mp, node->par->par);
					}
				}
			}
			if (node == mp->root)
				break;
	}
	mp->root->col = BLACK;
}

void insert_int(int_map *mp, int_pair elem) {
    int_node *new = new_int_node(elem, 1);
    if (!new) {
		printf("PROBLEMO\n");
        clear_int_map(mp);
        return ;
    }
    if (!mp->root) {
        new->col = 0;
        mp->root = new;
    } else {
        int_node *cur = mp->root;
        while (cur) {
            if (mp->cmp(new->value.first, cur->value.first)) {
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
	//printf("a\n");
    insertFix_int(mp, new);
	//printf("b\n");
    mp->size++;
}
					

int_node *predecessor(int_node *node) {
	if (!node || !node->left)
		return (NULL);
	node = node->left;
	while (node->right)
		node = node->right;
	return node;
}

void transplant(int_map *mp, int_node *u, int_node *v) {
	if (!u->par)
		mp->root = v;
	else if (u == u->par->left)
		u->par->left = v;
	else
		u->par->right = v;
	if (v)
		v->par = u->par;
}

void erase_int(int_map *mp, int val) {
	int_node *node = mp->root;

	while (node && node->value.first != val)
		node = (mp->cmp(val, node->value.first) ? node->left : node->right);

	if (!node)
		return ;

	int_node *x, *y, *dummy = NULL;
	int original_col = node->col;

	if (!node->left && !node->right) {
		dummy = new_int_node(make_int_pair(0, 0), 0);
		transplant(mp, node, dummy);
		x = dummy;
	} else if (!node->left) {
		transplant(mp, node, node->right);
		x = node->right;
	} else if (!node->right) {
		transplant(mp, node, node->left);
		x = node->left;
	} else {
		y = predecessor(node);
		x = y->left;
		if (!x) {
			dummy = new_int_node(make_int_pair(0, 0), 0);
			dummy->col = BLACK;
			dummy->par = y;
			y->left = dummy;
			x = dummy;
		}
		original_col = y->col;
		if (y->par != node) {
			transplant(mp, y, x);
			y->left = node->left;
			if (y->left)
				y->left->par = y;
		}
		transplant(mp, node, y);
		y->col = node->col;
		y->right = node->right;
		if (y->right)
			y->right->par = y;
	}

	if (dummy) {
		transplant(mp, dummy, NULL);
		//free(dummy);
	}

	//free(node);

	mp->size--;
}


int_node *upper_bound_int(int_map *mp, int value) {
    int_node *cur = mp->root;
	int_node *ret = NULL;

	while (cur) {
		if (mp->cmp(cur->value.first, value) || value == cur->value.first)
			cur = cur->right;
		else
			ret = cur, cur = cur->left;
	}
    return ret;

}

int_node *inverse_lower_bound_int(int_map *mp, int value) {
    int_node *cur = mp->root;
	int_node *ret = NULL;

	while (cur) {
		if (mp->cmp(cur->value.first, value) || value == cur->value.first)
			ret = cur, cur = cur->right;
		else
			cur = cur->left;
	}
    return ret;
}

int_node *lower_bound_int(int_map *mp, int value) {
    int_node *cur = mp->root;
	int_node *ret = NULL;

	while (cur) {
		if (mp->cmp(cur->value.first, value))
			cur = cur->right;
		else
			ret = cur, cur = cur->left;
	}
    return ret;

}

int_node *get_int(int_map *mp, int value) {
    int_node *cur = mp->root;

    while (cur) {
	    //printf("[%zu] x=%zu y=%zu\n", cur->value.second.id, cur->value.second.x, cur->value.second.y);
		if (cur->value.first == value)
            break;
        if (mp->cmp(value, cur->value.first))
            cur = cur->left;
        else
            cur = cur->right;
    }
    return cur;
}
