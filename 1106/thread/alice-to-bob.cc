#include "alice-to-bob.h"
#include <iostream>

using namespace std;

typedef struct _DATA_OBJ{
  string line;
  bool wroteAlice;
} DATA_OBJ;

pthread_mutex_t Lock;
DATA_OBJ DataObj;

void 
init(void)
{
  if (pthread_mutex_init(&Lock, NULL)) ERR;
  DataObj.wroteAlice = false;
}

void *
alice(void *a)
{
  string line;

  while (true) {
    if (pthread_mutex_lock(&Lock)) ERR;
    if (DataObj.wroteAlice == false) {
      printf("[I am Alice] message? ");
      getline(cin, line);
      //cout << endl;
      DataObj.line = line;
      DataObj.wroteAlice = true;
      if (pthread_mutex_unlock(&Lock)) ERR;
    }
    else {
      if (pthread_mutex_unlock(&Lock)) ERR;
      sleep(1);
    }
  }

  return NULL;
}

void *
bob(void *a)
{
  while (true) {
    if (pthread_mutex_lock(&Lock)) ERR;    
    if (DataObj.wroteAlice == true) {
      cout << " [I am Bob ] message: " << DataObj.line << endl;
      DataObj.wroteAlice = false;
    }
    if (pthread_mutex_unlock(&Lock)) ERR; 
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
