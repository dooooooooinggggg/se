#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "debug.h"
#define DATA_SIZE 1000
#define MYHASH_SIZE (DATA_SIZE * 10)
#define INVALID (-1)
#define SKIP (3)

typedef struct _LIST
{
  struct _LIST *next;
  int key;
  int value;
} LIST;

typedef struct _MYHASH
{
  int key;
  LIST *pointer;
} MYHASH;

LIST Head;

int main()
{
  LIST *p = &Head;
  MYHASH myhash[MYHASH_SIZE];

  // Generate data
  int prev = 0;
  for (int i = 0; i < DATA_SIZE; i++)
  {
    p->next = (LIST *)malloc(sizeof(LIST));
    p = p->next;
    p->key = prev + rand() % 5 + 1;
    p->value = rand() % 1000;
    p->next = NULL;
    prev = p->key;
  }

  NNN;
  // Init myhash
  for (int i = 0; i < MYHASH_SIZE; i++)
  {
    myhash[i].key = INVALID;
  }

  NNN;
  // Generate myhash
  for (p = Head.next; p != NULL; p = p->next)
  {
    int address = p->key % MYHASH_SIZE;
    while (true)
    {
      if (myhash[address].key == INVALID)
      {
        myhash[address].key = p->key;
        myhash[address].pointer = p;
        break;
      }
      else
      {
        address = address + SKIP;
        if (address >= MYHASH_SIZE)
          address = address - MYHASH_SIZE;
      }
    }
  }

  // Print keys
  for (p = Head.next; p != NULL; p = p->next)
  {
    printf("%d\t", p->key);
  }
  printf("\n");

  // Search
  int key;
  printf("Key>");
  scanf("%d", &key);

  int address = key % MYHASH_SIZE;
  int distance = 0;
  while (true)
  {
    if (myhash[address].key == key)
    {
      printf("%d:%d\n", key, myhash[address].pointer->value);
      break;
    }
    else
    {
      distance += SKIP;
      address += SKIP;
      if (address >= MYHASH_SIZE)
      {
        // address = address - MYHASH_SIZE;
        printf("Not Found\n");
        break;
      }
    }
  }

  exit(0);
}
