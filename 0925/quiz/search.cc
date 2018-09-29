#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct list
{
  struct list *next;
  int key;
  int value;
};

struct list Head;
int main()
{
  struct list *p = &Head;

  // Generate data
  int prev = 0;
  for (int i = 0; i < 10; i++)
  {
    p->next = (struct list *)malloc(sizeof(struct list));
    p = p->next;
    p->key = prev + rand() % 5 + 1;
    p->value = rand() % 1000;
    p->next = NULL;
    prev = p->key;
  }

  // Print
  for (p = Head.next; p != NULL; p = p->next)
  {
    printf("%d:%d\n", p->key, p->value);
  }

  // Search
  int key;
  printf("Key>");
  scanf("%d", &key);

  for (p = Head.next; p != NULL; p = p->next)
  {
    if (p->key == key)
    {
      printf("%d:%d\n", p->key, p->value);
      break;
    }
  }

  exit(0);
}
