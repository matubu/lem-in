#include "room_map.h"


/*
typedef struct s_room {
	size_t	id;
	size_t	x;
	size_t	y;
}	t_room;
*/


room_pair make_room_pair(char *first, t_room second) {
    room_pair p;
    p.first = first;
    p.second = second;
    return p;
}

room_node *new_room_node(room_pair value, int col) {
    room_node *n = malloc(sizeof(room_node));
    n->value = value;
    n->col = col;
    n->par = NULL;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void init_room_map(room_map *mp, int (*cmp)(char *, char *)) {
    mp->root = NULL;
    mp->size = 0;
    mp->cmp = cmp;
}

void clear_room_node(room_node *n) {
    if (!n)
        return ;
    clear_room_node(n->left);
    clear_room_node(n->right);
    free(n);
}

void clear_room_map(room_map *mp) {
    clear_room_node(mp->root);
    mp->root = NULL;
    mp->size = 0;
}

room_node *front_room(room_map *mp) {
    if (!mp->root)
        return NULL;
    room_node *cur = mp->root;
    while (cur->left)
        cur = cur->left; 
    return cur;
}



void	leftRotate(room_map *mp, room_node *node) {
	if (!node || !node->right)
		return ;
	room_node *y = node->right;
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

void	rightRotate(room_map *mp, room_node *node) {
	if (!node || !node->left)
		return ;
	room_node *y = node->left;
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

void leftRightRotate(room_map *mp, room_node *node) {
	if (!node || !node->left || !node->left->right)
		return ;
	leftRotate(mp, node->left);
}

void rightLeftRotate(room_map *mp, room_node *node) {
	if (!node || !node->right || !node->right->left)
		return ;
	rightRotate(mp, node->right);
}

void insertFix(room_map *mp, room_node *node) {
	if (!node->par || !node->par->par) 
		return;
	while (node->par && node->par->col == RED) {
		room_node *p = node->par, *gp = node->par->par;
		if (p == gp->left)
			if (gp->right && gp->right->col == RED) {
				gp->right->col = BLACK;
				gp->left->col = BLACK;
				gp->col = RED;
				node = gp;
			} else {
				if (node == p->right) {
					node = p;
					leftRotate(mp, node);
				}
				if (node->par->par) { 
					node->par->col = BLACK;
					node->par->par->col = RED;
					rightRotate(mp, node->par->par);
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
						rightRotate(mp, node);
					}
					if (node->par->par) {
						node->par->col = BLACK;
						node->par->par->col = RED;
						leftRotate(mp, node->par->par);
					}
				}
			}
			if (node == mp->root)
				break;
	}
	mp->root->col = BLACK;
}

room_node *insert_room(room_map *mp, room_pair elem) {
    room_node *new = new_room_node(elem, 1);

    if (!mp->root) {
        new->col = 0;
        mp->root = new;
    } else {
        room_node *cur = mp->root;
        while (1) {
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
    insertFix(mp, new);
    mp->size++;
	return (new);
}

room_node *get_room(room_map *mp, char *value) {
    room_node *cur = mp->root;

    while (cur) {
	    //printf("[%zu] x=%zu y=%zu\n", cur->value.second.id, cur->value.second.x, cur->value.second.y);
        if (equal_str(cur->value.first, value))
            break;
        if (mp->cmp(value, cur->value.first))
            cur = cur->left;
        else
            cur = cur->right;
    }
    return cur;
}
