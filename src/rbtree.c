#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  // TODO: initialize struct if needed;
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;

  // tree의 nil과 root를 nil 노드로 설정
  p->nil = p->root = nil;

  return p;
}

void delete_node(node_t *node, rbtree *t)
{
  // base case node가 nil인 경우
  if (node == t->nil)
    return;

  delete_node(node->left, t);  // 왼쪽서브트리 삭제
  delete_node(node->right, t); // 오른쪽서브트리 삭제
  free(node);                  // 현재 노드 메모리 해제
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  delete_node(t->root, t); // 재귀적으로 메모리 반납하는 함수 사용
  free(t->nil);            // nil 노드 메모리 해제
  free(t);                 // 트리 자체를 메모리에서 해제
}

void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right; // y를 설정한다.
  x->right = y->left;   // y의 왼쪽 서브트리를 newNode의 오른쪽 서브 트리로 옮긴다.
  if (y->left != t->nil)
    y->left->parent = x;

  y->parent = x->parent; // x의 부모를 y로 연결한다.

  if (x->parent == t->nil)
    t->root = y;

  else if (x == x->parent->left)
    x->parent->left = y;

  else
    x->parent->right = y;

  y->left = x; // x를 y의 왼쪽으로 놓는다
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left;

  x->left = y->right;
  if (y->right != t->nil)
    y->right->parent = x;

  y->parent = x->parent;
  if (x->parent == t->nil)
    t->root = y;

  else if (x == x->parent->left)
    x->parent->left = y;

  else
    x->parent->right = y;

  y->right = x;
  x->parent = y;
}

void insert_fixup(rbtree *t, node_t *z)
{
  // 삽입할 노드의 부모가 레드인 경우
  while (z->parent->color == RBTREE_RED)
  {
    node_t *uncle;
    // 노드의 부모가 할아버지의 왼쪽 자식인 경우
    if (z->parent == z->parent->parent->left)
    {
      uncle = z->parent->parent->right;
      // 삼촌이 레드인 경우
      if (uncle->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }

      // 삼촌이 블랙이고 삽입할 노드가 부모의 오른쪽인 경우
      // LR 인 경우 (부모가 왼쪽, 삽입할 노드가 오른쪽)
      else
      {
        if (z == z->parent->right)
        {
          // 왼쪽으로 회전 실행
          z = z->parent;
          left_rotate(t, z);
        }

        // LL인 경우 오른쪽으로 회전 (부모가 왼쪽, 삽입할 노드도 왼쪽)
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
        right_rotate(t, z);
      }
    }

    // 노드의 부모가 할아버지의 오른쪽 자식인 경우
    else
    {
      uncle = z->parent->parent->left;
      // 삼촌이 레드인 경우
      if (uncle->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }

      // 삼촌이 블랙이고 삽입할 노드가 부모의 오른쪽인 경우
      else
      {
        // RL인 경우 (부모가 왼쪽, 삽입할 노드가 오른쪽)
        if (z == z->parent->left)
        {
          // 오른쪽으로 회전 실행
          z = z->parent;
          right_rotate(t, z);
        }

        // RR인 경우 왼쪽으로 회전
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
        left_rotate(t, z);
      }
    }
  }

  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert

  /////////////////////////////////////////////////////////
  // 삽입할 새 노드 생성
  node_t *newNode = (node_t *)calloc(1, sizeof(node_t));
  newNode->key = key;
  newNode->color = RBTREE_RED;
  newNode->left = newNode->right = t->nil;

  node_t *y = t->nil;
  node_t *x = t->root;

  // 노드를 삽입할 곳 탐색 (이진 탐색 트리)
  while (x != t->nil)
  {
    y = x;
    if (newNode->key < x->key)
      x = x->left;

    else
      x = x->right;
  }

  newNode->parent = y;
  if (y == t->nil)
    t->root = newNode;

  else if (newNode->key < y->key)
    y->left = newNode;

  else
    y->right = newNode;

  newNode->left = t->nil;
  newNode->right = t->nil;

  // 규칙위반하면 재조정하기
  insert_fixup(t, newNode);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  // rbtree내에 해당 key가 있으면 해당 노드 반환
  // 해당하는 노드가 없으면 null 반환
  node_t *cur = t->root;

  // cur가 nil이 아니고 key가 아니면 계속 탐색 진행
  while (cur != t->nil && cur->key != key)
  {
    if (cur->key > key) // cur.key가 key보다 크면 왼쪽 서브트리로 탐색진행
      cur = cur->left;

    else if (cur->key < key) // cur.key가 key보다 작으면 오른쪽 서브트리로 탐색진행
      cur = cur->right;
  }

  if (cur == t->nil)
    return NULL;

  return cur;
}

int min_search(node_t *node, const rbtree *t)
{
  if (node == t->nil)
    return 1000000000;

  int left = 0;
  int right = 0;

  left = min_search(node->left, t);
  right = min_search(node->right, t);

  // 현재 노드의 값과 왼쪽, 오른쪽 서브트리의 최소값을 비교
  if (node->key < left)
  {
    if (node->key < right)
      return node->key; // 현재 노드의 값이 가장 작음
    else
      return right; // 오른쪽 서브트리의 값이 가장 작음
  }
  else
  {
    if (left < right)
      return left; // 왼쪽 서브트리의 값이 가장 작음

    else
      return right; // 오른쪽 서브트리의 값이 가장 작음
  }
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  // rbtree 중 최소 값을 가진 node pointer 반환
  int minKey = 0;
  node_t *minNode;
  // minKey = min_search(t->root, t);
  // minNode = rbtree_find(t, minKey);

  node_t *cur = t->root;

  // 찾는 노드가 nil노드가 아니고 루트가 아닌경우
  while (cur->parent != NULL && cur != t->nil)
  {
    cur = cur->left;
  }

  return cur;
}

int max_search(const rbtree *t, node_t *node)
{
  if (node == t->nil)
    return 0;

  int left = 0;
  int right = 0;

  left = max_search(t, node->left);
  right = max_search(t, node->right);

  // 현재 노드의 값과 왼쪽, 오른쪽 서브트리의 최대값을 비교
  if (node->key > left)
  {
    if (node->key > right)
      return node->key; // 현재 노드의 값이 가장 큼
    else
      return right; // 오른쪽 서브트리의 값이 가장 큼
  }
  else
  {
    if (left > right)
      return left; // 왼쪽 서브트리의 값이 가장 큼

    else
      return right; // 오른쪽 서브트리의 값이 가장 큼
  }
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  // 최댓값을 가진 node pointer 반환
  int maxKey = 0;
  node_t *maxNode;
  maxKey = max_search(t, t->root);
  maxNode = rbtree_find(t, maxKey);
  return maxNode;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{

  // 타겟 노드가 루트 노드인 경우
  if (u->parent == t->nil)
    t->root = v;
  // 타겟 노드가 왼쪽 자식인 경우
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  // 타겟 노드가 오른쪽 자식인 경우
  else
    u->parent->right = v;
  v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  node_t *y = p;
  node_t *x;
  int y_original_color = y->color;

  // 타겟 노드가 오른쪽 자식만 있는 경우
  if (p->left == t->nil)
  {
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  // 타겟 노드가 왼쪽 자식만 있는 경우
  else if (p->right == t->nil)
  {
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else
  {
    y = rbtree_min();
  }

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}

// // 트리를 출력하는 함수
// void print_rbtree(rbtree *t, node_t *node, int space)
// {
//   if (node == t->nil)
//     return;

//   space += 10;
//   print_rbtree(t, node->right, space);

//   printf("\n");
//   for (int i = 10; i < space; i++)
//     printf(" ");
//   printf("%d(%s)\n", node->key, node->color == RBTREE_RED ? "R" : "B");

//   print_rbtree(t, node->left, space);
// }

// int main()
// {
//   rbtree *t = new_rbtree(); // 레드-블랙 트리 생성 함수
//   int key;

//   printf("노드를 삽입하려면 키 값을 입력하세요 (음수를 입력하면 종료):\n");
//   while (scanf("%d", &key) && key >= 0)
//   {
//     rbtree_insert(t, key);
//     print_rbtree(t, t->root, 0);
//   }

//   // 트리 메모리 해제
//   delete_rbtree(t);

//   return 0;
// }