#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  // TODO: initialize struct if needed;
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  nil->left = nil->right = nil;

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

  // if (x->parent == t->root)
  // {
  //   t->root = x;
  // }
  // else
  // {

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
  // }
}

void right_rotate(rbtree *t, node_t *x)
{
  // if (x->parent == t->root)
  // {
  //   t->root = x;
  // }
  // else
  // {

  node_t *y = x->left;

  x->left = y->right;
  if (y->right != t->nil)
    y->right->parent = x;

  y->parent = x->parent;

  if (x->parent == t->nil)
    t->root = y;

  else if (x == x->parent->right)
    x->parent->right = y;

  else
    x->parent->left = y;

  y->right = x;
  x->parent = y;
  // }
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
        if (z != t->root && z->parent != t->root)
        {
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          right_rotate(t, z->parent->parent);
        }
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
        if (z != t->root && z->parent != t->root)
        {
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          left_rotate(t, z->parent->parent);
        }
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

  while (cur != t->nil)
  {
    if (cur->key == key)
      return cur;

    else if (cur->key < key)
      cur = cur->right;
    else
      cur = cur->left;
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  // rbtree 중 최소 값을 가진 node pointer 반환
  node_t *cur = t->root;

  if (cur == t->nil)
  {
    return cur;
  }

  while (cur->left != t->nil)
  {
    cur = cur->left;
  }

  return cur;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  // 최댓값을 가진 node pointer 반환
  node_t *cur = t->root;
  if (cur == t->nil)
  {
    return cur;
  }

  while (cur->right != t->nil)
  {
    cur = cur->right;
  }

  return cur;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{

  // 타겟 노드가 루트 노드인 경우
  if (u->parent == t->nil)
    t->root = v;

  // 타겟 노드가 왼쪽 자식인 경우
  else if (u == u->parent->left)
    u->parent->left = v;

  // 타겟 노드가 오른쪽 자식인 경우
  else
    u->parent->right = v;
  v->parent = u->parent;
}

node_t *rbtree_minimum(rbtree *t, node_t *node)
{
  node_t *successor = node;

  while (successor->left != t->nil)
  {
    successor = successor->left;
  }

  return successor;
}

void rbtree_delete_fixup(rbtree *t, node_t *x)
{
  node_t *sibling;
  // 타겟이 루트아님 && 타겟 == 블랙
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    // 타겟이 부모의 왼쪽 자식인경우
    if (x == x->parent->left)
    {
      sibling = x->parent->right; // 형제는 오른쪽 자식

      // CASE 1 형제 == 레드.
      if (sibling->color == RBTREE_RED)
      {
        sibling->color = RBTREE_BLACK; // 형제-부모 색바꾸기
        x->parent->color = RBTREE_RED; // 형제-부모 색바꾸기
        left_rotate(t, x->parent);     // 부모기준 타겟이 내려가도록 회전
        sibling = x->parent->right;
      }

      // CASE 2. 형제 == 블랙, 형제의 자식 둘다 블랙
      // 형제/자신의 블랙을 부모로 올리기
      if (sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK)
      {
        sibling->color = RBTREE_RED; // 형제를 레드로 변경
        x = x->parent;               // 부모에서 다시 fixup
      }
      else
      {
        // CASE 3. 형제 == 블랙, 형제의 꺾인 자식 == 레드
        // 자식 레드와 형제 서로 색바꾸기, 펴지게 회전 (레드가 바깥에 오게)
        if (sibling->right->color == RBTREE_BLACK)
        {
          sibling->left->color = RBTREE_BLACK; // 자식레드-형제 색바꾸기
          sibling->color = RBTREE_RED;         // 자식레드-형제 색바꾸기
          right_rotate(t, sibling);            // 레드가 바깥에 오게 회전
          sibling = x->parent->right;
          // -> case 4가 됨
        }
        // CASE 4. 형제 == 블랙, 형제의 펴진 자식 == 레드
        sibling->color = x->parent->color;    // 부모-형제 서로 색바꾸기
        x->parent->color = RBTREE_BLACK;      // 부모-형제 서로 색바꾸기
        sibling->right->color = RBTREE_BLACK; // 형제의 펴진 자식 블랙
        left_rotate(t, x->parent);            // 부모기준 회전 (타겟이 내려가도록)
        x = t->root;                          // 타겟을 루트로 설정
      }
    }
    // 타겟이 부모의 오른쪽 자식인경우
    else
    {
      sibling = x->parent->left; // 형제는 왼쪽 자식

      // CASE 1 형제 == 레드.
      if (sibling->color == RBTREE_RED)
      {
        sibling->color = RBTREE_BLACK; // 형제-부모 색바꾸기
        x->parent->color = RBTREE_RED; // 형제-부모 색바꾸기
        right_rotate(t, x->parent);    // 부모기준 타겟이 내려가도록 회전
        sibling = x->parent->left;
      }

      // CASE 2. 형제 == 블랙, 형제의 자식 둘다 블랙
      // 형제/자신의 블랙을 부모로 올리기
      if (sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK)
      {
        sibling->color = RBTREE_RED; // 형제를 레드로 변경
        x = x->parent;               // 부모에서 다시 fixup
      }
      else
      {
        // CASE 3. 형제 == 블랙, 형제의 꺾인 자식 == 레드
        // 자식 레드와 형제 서로 색바꾸기, 펴지게 회전 (레드가 바깥에 오게)
        if (sibling->left->color == RBTREE_BLACK)
        {
          sibling->right->color = RBTREE_BLACK; // 자식레드-형제 색바꾸기
          sibling->color = RBTREE_RED;          // 자식레드-형제 색바꾸기
          left_rotate(t, sibling);              // 레드가 바깥에 오게 회전
          sibling = x->parent->left;
          // -> case 4가 됨
        }
        // CASE 4. 형제 == 블랙, 형제의 펴진 자식 == 레드
        sibling->color = x->parent->color;   // 부모-형제 서로 색바꾸기
        x->parent->color = RBTREE_BLACK;     // 부모-형제 서로 색바꾸기
        sibling->left->color = RBTREE_BLACK; // 형제의 펴진 자식 블랙
        right_rotate(t, x->parent);          // 부모기준 회전 (타겟이 내려가도록)
        x = t->root;                         // 타겟을 루트로 설정
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  node_t *y = p;
  node_t *x;
  color_t y_original_color = y->color;

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
  // 타겟 노드의 자식이 둘 있는 경우
  else
  {
    y = rbtree_minimum(t, p->right);
    y_original_color = y->color;
    x = y->right;

    if (y->parent == p)
    {
      x->parent = y;
    }
    else
    {
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  // 삭제되는 노드의 색이 블랙이면 재조정 시작
  if (y_original_color == RBTREE_BLACK)
    rbtree_delete_fixup(t, x);

  free(p);
  return 0;
}

int rbtree_inoder_travasal(const rbtree *t, node_t *node, key_t *arr, int n)
{
  if (node == t->nil)
    return n;

  n = rbtree_inoder_travasal(t, node->left, arr, n);
  arr[n] = node->key;
  n++;
  n = rbtree_inoder_travasal(t, node->right, arr, n);
  return n;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  /*rbtree의 내용을 *key 순서대로* 주어진 array로 변환
  array의 크기는 n으로 주어지며 tree의 크기가 n 보다 큰 경우에는 순서대로 n개 까지만 변환
  array의 메모리 공간은 이 함수를 부르는 쪽에서 준비하고 그 크기를 n으로 알려줍니다.*/

  if (t->root == t->nil)
    return -1;

  rbtree_inoder_travasal(t, t->root, arr, 0);

  return 0;
}

// 트리를 출력하는 함수
void print_rbtree(rbtree *t, node_t *node, int space)
{
  if (node == t->nil)
    return;

  space += 10;
  print_rbtree(t, node->right, space);

  printf("\n");
  for (int i = 10; i < space; i++)
    printf(" ");
  printf("%d(%s)\n", node->key, node->color == RBTREE_RED ? "R" : "B");

  print_rbtree(t, node->left, space);
}

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

// int main()
// {
//   // 새로운 레드-블랙 트리 생성
//   rbtree *t = new_rbtree();

//   // 삽입할 노드 키 배열
//   int keys[] = {7, 3, 18, 10, 22, 8, 11, 26, 2};

//   // 트리에 노드 삽입
//   for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
//   {
//     rbtree_insert(t, keys[i]);
//   }

//   // 트리 출력
//   printf("Inserted Nodes:\n");
//   print_rbtree(t, t->root, 0);

//   // 삭제할 노드 키 배열
//   int delete_keys[] = {18, 11, 3};

//   // 트리에서 노드 삭제
//   for (int i = 0; i < sizeof(delete_keys) / sizeof(delete_keys[0]); i++)
//   {
//     node_t *node = rbtree_find(t, delete_keys[i]);
//     if (node != NULL)
//     {
//       rbtree_erase(t, node);
//       printf("\n\nAfter deleting node with key %d:\n", delete_keys[i]);
//       print_rbtree(t, t->root, 0);
//     }
//     else
//     {
//       printf("\nNode with key %d not found!\n", delete_keys[i]);
//     }
//   }

//   // 트리 메모리 해제
//   delete_rbtree(t);

//   return 0;
// }