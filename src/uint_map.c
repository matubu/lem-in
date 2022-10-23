#include "uint_map.h"

uint_pair make_uint_pair(uint first, uint second) {
	uint_pair	p;
	p.first = first;
	p.second = second;
	return p;
}

uint_node	*new_uint_node(uint_pair value, uint col) {
	uint_node	*n = malloc(sizeof(uint_node));
	n->value = value;
	n->col = col;
	n->par = NULL;
	n->left = NULL;
	n->right = NULL;
	return n;
}

void	init_uint_map(uint_map *mp, bool (*cmp)(uint, uint)) {
	mp->root = NULL;
	mp->size = 0;
	mp->cmp = cmp;
}

void	clear_uint_node(uint_node *n) {
	if (!n)
		return ;
	clear_uint_node(n->left);
	clear_uint_node(n->right);
	free(n);
}

void	clear_uint_map(uint_map *mp) {
	clear_uint_node(mp->root);
	mp->root = NULL;
	mp->size = 0;
}

uint_node	*top_uint(uint_map *mp) {
	if (!mp->root)
		return NULL;
	uint_node	*cur = mp->root;
	while (cur->left)
		cur = cur->left; 
	return cur;
}

void	pop_uint(uint_map *pq) {
	uint_node	*cur = top_uint(pq);

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

void	leftRotate_uint(uint_map *mp, uint_node *node) {
	if (!node || !node->right)
		return ;
	uint_node *y = node->right;
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

void	rightRotate_uint(uint_map *mp, uint_node *node) {
	if (!node || !node->left)
		return ;
	uint_node	*y = node->left;
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

void	leftRightRotate_uint(uint_map *mp, uint_node *node) {
	if (!node || !node->left || !node->left->right)
		return ;
	leftRotate_uint(mp, node->left);
}

void	rightLeftRotate_uint(uint_map *mp, uint_node *node) {
	if (!node || !node->right || !node->right->left)
		return ;
	rightRotate_uint(mp, node->right);
}

void	insertFix_uint(uint_map *mp, uint_node *node) {
	if (!node->par || !node->par->par) 
		return;
	while (node->par && node->par->col == RED && node->par->par) {
		uint_node	*p = node->par, *gp = node->par->par;
		if (p == gp->left)
			if (gp->right && gp->right->col == RED) {
				gp->right->col = BLACK;
				gp->left->col = BLACK;
				gp->col = RED;
				node = gp;
			} else {
				if (node == p->right) {
					node = p;
					leftRotate_uint(mp, node);
				}
				if (node->par->par) { 
					node->par->col = BLACK;
					node->par->par->col = RED;
					rightRotate_uint(mp, node->par->par);
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
						rightRotate_uint(mp, node);
					}
					if (node->par->par) {
						node->par->col = BLACK;
						node->par->par->col = RED;
						leftRotate_uint(mp, node->par->par);
					}
				}
			}
			if (node == mp->root)
				break;
	}
	mp->root->col = BLACK;
}

void	insert_uint(uint_map *mp, uint_pair elem) {
	uint_node	*new = new_uint_node(elem, 1);
	if (!mp->root) {
		new->col = 0;
		mp->root = new;
	} else {
		uint_node *cur = mp->root;
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
	insertFix_uint(mp, new);
	mp->size++;
}
					

uint_node	*predecessor(uint_node *node) {
	if (!node || !node->left)
		return (NULL);
	node = node->left;
	while (node->right)
		node = node->right;
	return node;
}

void	transplant(uint_map *mp, uint_node *u, uint_node *v) {
	if (!u->par)
		mp->root = v;
	else if (u == u->par->left)
		u->par->left = v;
	else
		u->par->right = v;
	if (v)
		v->par = u->par;
}

void	erase_uint(uint_map *mp, uint val) {
	uint_node	*node = mp->root;

	while (node && node->value.first != val)
		node = (mp->cmp(val, node->value.first) ? node->left : node->right);

	if (!node)
		return ;

	uint_node	*x,
				*y,
				*dummy = NULL;

	if (!node->left && !node->right) {
		dummy = new_uint_node(make_uint_pair(0, 0), 0);
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
			dummy = new_uint_node(make_uint_pair(0, 0), 0);
			dummy->col = BLACK;
			dummy->par = y;
			y->left = dummy;
			x = dummy;
		}
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
		free(dummy);
	}

	free(node);

	mp->size--;
}


uint_node	*upper_bound_uint(uint_map *mp, uint value) {
	uint_node	*cur = mp->root;
	uint_node	*ret = NULL;

	while (cur) {
		if (mp->cmp(cur->value.first, value) || value == cur->value.first)
			cur = cur->right;
		else
			ret = cur, cur = cur->left;
	}
	return ret;
}

uint_node	*get_uint(uint_map *mp, uint value) {
	uint_node *cur = mp->root;

	while (cur) {
		if (cur->value.first == value)
			break;
		if (mp->cmp(value, cur->value.first))
			cur = cur->left;
		else
			cur = cur->right;
	}
	return cur;
}
