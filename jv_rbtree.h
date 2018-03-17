#ifndef _JV_RBTREE_H_INCLUDE_
#define _JV_RBTREE_H_INCLUDE_

#include <jv_core.h>

#define JV_RBTREE_BLACK 1
#define JV_RBTREE_RED 0
#define JV_RBTREE_NIL NULL

typedef struct jv_rbtree_data_s jv_rbtree_data_t;
typedef struct jv_rbtree_node_s jv_rbtree_node_t;
typedef struct jv_rbtree_s jv_rbtree_t;

struct jv_rbtree_s {
  jv_rbtree_node_t *root;
  jv_rbtree_node_t *sentinel; /* nil */
  jv_uint_t length;
};

struct jv_rbtree_node_s {
  jv_rbtree_data_t *data;
  jv_rbtree_node_t *parent;
  jv_rbtree_node_t *left;
  jv_rbtree_node_t *right;
  u_char color;
};

struct jv_rbtree_data_s {
  jv_uint_t key;
  void *value;
};

jv_rbtree_t *jv_rbtree_create();

jv_int_t jv_rbtree_insert(jv_rbtree_t *tree, jv_rbtree_data_t *data);

jv_int_t jv_rbtree_remove(jv_rbtree_t *tree, jv_rbtree_node_t *node);

jv_rbtree_node_t *jv_rbtree_get(jv_rbtree_t *tree, jv_uint_t key);

jv_rbtree_node_t *jv_rbtree_min(jv_rbtree_node_t *root, jv_rbtree_node_t *sentinel);

jv_rbtree_node_t *jv_rbtree_max(jv_rbtree_node_t *node, jv_rbtree_node_t *sentinel);

void jv_rbtree_inorder(jv_rbtree_node_t *node, jv_rbtree_node_t *sentinel);

jv_rbtree_node_t *jv_rbtree_predecessor(jv_rbtree_node_t *node, jv_rbtree_node_t *sentinel);

/**
 *  search node's successor by inorder traversal
 */
jv_rbtree_node_t *jv_rbtree_successor(jv_rbtree_node_t *node, jv_rbtree_node_t *sentinel);

jv_int_t jv_rbtree_destroy(jv_rbtree_t *tree);

jv_uint_t jv_rbtree_depth(jv_rbtree_node_t *node, jv_rbtree_node_t *sentinel);

#endif /* _JV_RBTREE_H_INCLUDE_ */
