#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum
{
  RBTREE_RED,
  RBTREE_BLACK
} color_t;

typedef int key_t;

typedef struct node_t
{
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct
{
  node_t *root;
  node_t *nil; // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif // _RBTREE_H_

while (타겟이 루트아님 && 타겟 == BLACK)
{

  // CASE 1.
  if
    **형제 == RED **
                      형제 /
                  부모 서로 색바꾸기,
        부모기준 회전(타겟이 내려가도록)

            // CASE 2.
            if **형제 == BLACK,
        형제의 자식 둘다 블랙 **
                형제 /
            자신의 블랙을 부모로 올리기->형제를 RED로 변경,
        부모에서 다시 fixup

        else
    {
      // CASE 3.
      if 형제
        == BLACK, (**형제의 꺾인 자식 **) == RED 자식 RED와 형제 서로 색바꾸기, 펴지게 회전(RED가 바깥쪽에 오게) CASE 4가 됨

                                                                                        // CASE 4. 형제 == BLACK, (**형제의 펴진 자식**) == RED
                                                                                        부모 /
                                                                                    형제 서로 색바꾸기 형제의(펴진 자식) = BLACK 부모기준 회전(타겟이 내려가도록) 타겟을 루트로 설정->while 종료
    }