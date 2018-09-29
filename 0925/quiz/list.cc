#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct list {
  struct list *next;
  int key;
};

struct list Head;
int main()
{
  struct list *p = &Head;

  for( int i = 0; i < 10; i++) {
    p->next = (struct list *)malloc(sizeof(struct list));
    p = p->next;
    p->key = i;
    p->next = NULL;
  }

  for( p = Head.next; p != NULL; p = p->next) {
    printf("%d\n", p->key);
  }

  exit(0);
}
