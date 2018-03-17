#include <jv_rbtree.h>

static void jv_rbtree_left_rotate(jv_rbtree_t *tree, jv_rbtree_node_t *node);

static void jv_rbtree_right_rotate(jv_rbtree_t *tree, jv_rbtree_node_t *node);

static void jv_rbtree_insert_fixup(jv_rbtree_t *tree, jv_rbtree_node_t *node);

static void jv_rbtree_remove_fixup(jv_rbtree_t *tree, jv_rbtree_node_t *node);

/**
 * destroy tree's all nodes by preorder traversal, but except tree structure
 */
static jv_int_t jv_rbtree_destroy_node(jv_rbtree_t *tree, jv_rbtree_node_t *root);

static jv_int_t jv_rbtree_destroy_node(jv_rbtree_t *tree, jv_rbtree_node_t *node) {
  if (node != tree->sentinel) {
    printf("destroy node's data:%lu\n", node->data->key);
    jv_rbtree_destroy_node(tree, node->left);
    jv_rbtree_destroy_node(tree, node->right);
    free(node);
  }
  return JV_OK;
}

jv_rbtree_t *jv_rbtree_create() {
  jv_rbtree_t *tree = malloc(sizeof(jv_rbtree_t) + sizeof(jv_rbtree_node_t));
  jv_rbtree_node_t *sentinel = (jv_rbtree_node_t *) ((u_char *) tree + sizeof(jv_rbtree_t));

  sentinel->color = JV_RBTREE_BLACK;
  sentinel->data = NULL;
  sentinel->left = NULL;
  sentinel->right = NULL;
  sentinel->parent = NULL;

  tree->sentinel = sentinel;
  tree->root = sentinel;
  tree->length = 0;
  return tree;
}

void jv_rbtree_inorder(jv_rbtree_node_t *node, jv_rbtree_node_t *sentinel) {
  jv_rbtree_node_t *t = node;
  if (node != sentinel) {
    jv_rbtree_inorder(t->left, sentinel);
    /*if (t->data->key != 0) { */
    printf("node's data:%-2ld, left:%-2ld, right:%-2ld, parent:%-2ld, color:%s \n", (long) t->data->key,
           t->left != NULL && t->left->data != NULL ? (long) t->left->data->key : -1,
           t->right != NULL && t->right->data != NULL ? (long) t->right->data->key : -1,
           t->parent != NULL && t->parent->data != NULL ? (long) t->parent->data->key : -1, t->color == 0 ? "red" : "black");
    /* } */
    jv_rbtree_inorder(t->right, sentinel);
  }
}

static void jv_rbtree_left_rotate(jv_rbtree_t *tree, jv_rbtree_node_t *node) {
  jv_rbtree_node_t *y = node->right;
  node->right = y->left;
  /* printf("left rotate\n"); */
  if (y->left != tree->sentinel) {
    y->left->parent = node;
  }
  y->parent = node->parent;
  if (node->parent == tree->sentinel) { /* node == tree->root */
    tree->root = y;
  } else if (node == node->parent->left) {
    node->parent->left = y;
  } else {
    node->parent->right = y;
  }
  y->left = node;
  node->parent = y;
}

static void jv_rbtree_right_rotate(jv_rbtree_t *tree, jv_rbtree_node_t *node) {
  jv_rbtree_node_t *y = node->left;
  node->left = y->right;
  /* printf("right rotate\n"); */
  if (y->right != tree->sentinel) {
    y->right->parent = node;
  }
  y->parent = node->parent;
  if (node->parent == tree->sentinel) { /* node==tree->root */
    tree->root = y;
  } else if (node == node->parent->right) {
    node->parent->right = y;
  } else {
    node->parent->left = y;
  }
  y->right = node;
  node->parent = y;
}

jv_int_t jv_rbtree_insert(jv_rbtree_t *tree, jv_rbtree_data_t *data) {
  jv_rbtree_node_t *p, *sentinel;
  jv_rbtree_data_t *d;

  p = malloc(sizeof(jv_rbtree_node_t) + sizeof(jv_rbtree_data_t));

  if (p == NULL) {
    return JV_ERROR;
  }

  d = (jv_rbtree_data_t *) ((u_char *) p + sizeof(jv_rbtree_node_t));
  sentinel = tree->sentinel;

  d->key = data->key;
  d->value = data->value;
  p->data = d;

  if (tree->root == sentinel) {
    p->left = sentinel;
    p->right = sentinel;
    p->parent = sentinel;
    p->color = JV_RBTREE_BLACK;
    tree->root = p;
  } else {
    jv_rbtree_node_t *y = sentinel;
    jv_rbtree_node_t *x = tree->root;
    while (x != sentinel) {
      y = x;
      if (p->data->key < x->data->key) {
        x = x->left;
      } else {
        x = x->right;
      }
    }

    p->parent = y;

    if (y == sentinel) {
      tree->root = p;
    } else if (p->data->key < y->data->key) {
      y->left = p;
    } else {
      y->right = p;
    }

    p->left = sentinel;
    p->right = sentinel;
    p->color = JV_RBTREE_RED;
    jv_rbtree_insert_fixup(tree, p);
  }
  tree->length++;
  return JV_OK;
}

static void jv_rbtree_insert_fixup(jv_rbtree_t *tree, jv_rbtree_node_t *z) {
  /* printf("insert fixup\n"); */
  while (z->parent->color == JV_RBTREE_RED) {
    if (z->parent == z->parent->parent->left) {
      jv_rbtree_node_t *y = z->parent->parent->right;
      if (y->color == JV_RBTREE_RED) {
        z->parent->color = JV_RBTREE_BLACK;
        y->color = JV_RBTREE_BLACK;
        z->parent->parent->color = JV_RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          z = z->parent;
          jv_rbtree_left_rotate(tree, z);
        }
        z->parent->color = JV_RBTREE_BLACK;
        z->parent->parent->color = JV_RBTREE_RED;
        jv_rbtree_right_rotate(tree, z->parent->parent);
      }
      /* printf("insert fixup 1 \n"); */
    } else {
      jv_rbtree_node_t *y = z->parent->parent->left;
      if (y->color == JV_RBTREE_RED) {
        z->parent->color = JV_RBTREE_BLACK;
        y->color = JV_RBTREE_BLACK;
        z->parent->parent->color = JV_RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          jv_rbtree_right_rotate(tree, z);
        }
        z->parent->color = JV_RBTREE_BLACK;
        z->parent->parent->color = JV_RBTREE_RED;
        jv_rbtree_left_rotate(tree, z->parent->parent);
      }
      /* printf("insert fixup 2 \n"); */
    }
  }
  tree->root->color = JV_RBTREE_BLACK;
}

jv_rbtree_node_t *jv_rbtree_min(jv_rbtree_node_t *node, jv_rbtree_node_t *sentinel) {
  /* if (node->left == NULL) {
   return NULL;
   }*/
  while (node->left != sentinel) {
    node = node->left;
  }
  return node;
}

jv_rbtree_node_t *jv_rbtree_max(jv_rbtree_node_t *node, jv_rbtree_node_t *sentinel) {
  /* if (node->left == NULL) {
   return NULL;
   } */
  while (node->right != sentinel) {
    node = node->right;
  }
  return node;
}

jv_rbtree_node_t *jv_rbtree_predecessor(jv_rbtree_node_t *node, jv_rbtree_node_t *sentinel) {
  jv_rbtree_node_t *y;
  if (node->left != sentinel) {
    return jv_rbtree_max(node->left, sentinel);
  }
  y = node->parent;
  while (y != sentinel && node == y->left) {
    node = y;
    y = y->parent;
  }
  return y;
}

jv_rbtree_node_t *jv_rbtree_successor(jv_rbtree_node_t *node, jv_rbtree_node_t *sentinel) {
  jv_rbtree_node_t *y;
  if (node->right != sentinel) {
    return jv_rbtree_min(node->right, sentinel);
  }
  y = node->parent;
  while (y != sentinel && node == y->right) {
    node = y;
    y = y->parent;
  }
  return y;
}

jv_rbtree_node_t *jv_rbtree_get(jv_rbtree_t *tree, jv_uint_t key) {
  jv_rbtree_node_t *node = NULL;
  node = tree->root;
  while (node != tree->sentinel) {
    if (key > node->data->key)
      node = node->right;
    else if (key < node->data->key)
      node = node->left;
    else if (key == node->data->key)
      return node;
  }
  return NULL;
}

jv_int_t jv_rbtree_remove(jv_rbtree_t *tree, jv_rbtree_node_t *node) {
  jv_rbtree_node_t *x, *y;
  if (node == NULL || node->data == NULL) {
    return JV_OK;
  }
  if (node->left == tree->sentinel || node->right == tree->sentinel) {
    y = node;
  } else {
    y = jv_rbtree_successor(node, tree->sentinel);
  }
  if (y->left != tree->sentinel) {
    x = y->left;
  } else {
    x = y->right;
  }
  /*
   if (x != tree->sentinel) {
   x->parent = y->parent;
   }
   */
  x->parent = y->parent;

  if (y->parent == tree->sentinel) {
    tree->root = x;
  } else {
    if (y == y->parent->left) {
      y->parent->left = x;
    } else {
      y->parent->right = x;
    }
  }
  if (y != node) {
    node->data->key = y->data->key;
  }
  /* if(y->data->key != 0) { */
  printf("remove node:%-2ld, left:%-2ld, right:%-2ld, parent:%-2ld, color:%s \n", (long) y->data->key,
         y->left != NULL && y->left->data != NULL ? (long) y->left->data->key : -1,
         y->right != NULL && y->right->data != NULL ? (long) y->right->data->key : -1,
         y->parent != NULL && y->parent->data != NULL ? (long) y->parent->data->key : -1, y->color == 0 ? "red" : "black");
  /* } */
  if (y->color == JV_RBTREE_BLACK) {
    jv_rbtree_remove_fixup(tree, x);
  }
  tree->length--;
  free(y);
  return JV_OK;
}

static void jv_rbtree_remove_fixup(jv_rbtree_t *tree, jv_rbtree_node_t *node) {
  if (node->data != NULL) {
    printf("remove fixup, data:%ld, color:%d\n", (long) node->data->key, node->color);
  }
  while (node != tree->root && node->color == JV_RBTREE_BLACK) {
    if (node == node->parent->left) { /* node is left node */
      jv_rbtree_node_t *w = node->parent->right;
      if (w->color == JV_RBTREE_RED) {
        w->color = JV_RBTREE_BLACK;
        node->parent->color = JV_RBTREE_RED;
        jv_rbtree_left_rotate(tree, node->parent);
        w = node->parent->right;
      }
      /* w is black */
      if (w->left->color == JV_RBTREE_BLACK && w->right->color == JV_RBTREE_BLACK) {
        w->color = JV_RBTREE_RED;
        node = node->parent;
      } else {
        if (w->right->color == JV_RBTREE_BLACK) {
          w->left->color = JV_RBTREE_BLACK;
          w->color = JV_RBTREE_RED;
          jv_rbtree_right_rotate(tree, w);
          w = node->parent->right;
        }

        w->color = w->parent->color;
        node->parent->color = JV_RBTREE_BLACK;
        w->right->color = JV_RBTREE_BLACK;
        jv_rbtree_left_rotate(tree, node->parent);
        node = tree->root;
      }
      /* printf("remove fixup 1 \n"); */
    } else { /* node is right node */
      jv_rbtree_node_t *w = node->parent->left;
      if (w->color == JV_RBTREE_RED) {
        w->color = JV_RBTREE_BLACK;
        node->parent->color = JV_RBTREE_RED;
        jv_rbtree_right_rotate(tree, node->parent);
        w = node->parent->left;
      }
      if (w->left->color == JV_RBTREE_BLACK && w->right->color == JV_RBTREE_BLACK) {
        w->color = JV_RBTREE_RED;
        node = node->parent;
      } else {
        if (w->left->color == JV_RBTREE_BLACK) {
          w->right->color = JV_RBTREE_BLACK;
          w->color = JV_RBTREE_RED;
          jv_rbtree_left_rotate(tree, w);
          w = node->parent->left;
        }
        w->color = w->parent->color;
        node->parent->color = JV_RBTREE_BLACK;
        w->left->color = JV_RBTREE_BLACK;
        jv_rbtree_right_rotate(tree, node->parent);
        node = tree->root;
      }
      /* printf("remove fixup 2 \n"); */
    }
  }
  node->color = JV_RBTREE_BLACK;
}

jv_int_t jv_rbtree_destroy(jv_rbtree_t *tree) {
  if (jv_rbtree_destroy_node(tree, tree->root) == JV_OK) {
    free(tree);
  }
  return JV_ERROR;
}

jv_uint_t jv_rbtree_depth(jv_rbtree_node_t *node, jv_rbtree_node_t *sentinel) {
  jv_uint_t hl, hr;
  jv_rbtree_node_t *t = node;
  if (t == sentinel) {
    return 0;
  }
  hl = jv_rbtree_depth(t->left, sentinel);
  hr = jv_rbtree_depth(t->right, sentinel);
  return (hl > hr ? hl : hr) + 1;
}
