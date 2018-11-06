#include "alice-bob.h"

void *
alice(void *a)
{
  while (true) {
    printf("I am alice\n");
    sleep(1);
  }

  return NULL;
}

void *
bob(void *a)
{
  while (true) {
    printf("I am bob\n");
    sleep(1);
  }

  return NULL;
}

pthread_t
createAlice(void)
{
  pthread_t thread;

  if (pthread_create(&thread, NULL, alice, NULL)) ERR;
  
  return thread;
}

pthread_t
createBob(void)
{
  pthread_t thread;

  if (pthread_create(&thread, NULL, bob, NULL)) ERR;
  
  return thread;
}

void
createThreads(void)
{
  pthread_t thAlice, thBob;

  thAlice = createAlice();
  thBob = createBob();
  if (pthread_join(thAlice, NULL)) ERR;
  if (pthread_join(thBob, NULL)) ERR;
}

int
main(void)
{
  createThreads();

  return 0;
}
