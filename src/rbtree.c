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
  return t->root;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}

// 트리를 출력하는 함수
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