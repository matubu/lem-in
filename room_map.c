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

room_node *front(room_map *mp) {
    if (!mp->root)
        return NULL;
    room_node *cur = mp->root;
    while (cur->left)
        cur = cur->left; 
    return cur;
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
