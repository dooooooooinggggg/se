#include "kr.h"

bool validation(const LOCAL_DB local_db[], const int tbegin, const int tend)
{
  // Write your code here
  return true;
}

void transaction(int myid)
{
  LOCAL_DB local_db[NUM_ACCESS]; // local db
  int oidList[NUM_ACCESS];
  int tbegin = Tid;

  // Set access pattern
  for (int i = 0; i < NUM_ACCESS; i++)
  {
    oidList[i] = myid * 1000 + i;
  }

RETRY:
  // Read Phase
  for (int i = 0; i < NUM_ACCESS; i++)
  {
    int oid = oidList[i];
    local_db[oid].obj = Obj[oid];
    local_db[oid].mode = WRITE; // No conflict
  }

  // Local modification
  for (int i = 0; i < NUM_ACCESS; i++)
  {
    if (local_db[i].mode == WRITE)
    { // Modify
      local_db[i].obj.val += 3;
    }
  }

  // Validation & Write phase
  pthread_mutex_lock(&GiantLock);
  int tend = Tid;
  bool valid = validation(local_db, tbegin, tend);
  if (valid == false)
  {
    pthread_mutex_unlock(&GiantLock);
    goto RETRY;
  }

  // Write phase
  for (int i = 0; i < NUM_ACCESS; i++)
  {
    if (local_db[i].mode == WRITE)
    {
      Obj[i].val = local_db[i].obj.val;
    }
  }

  // Increment transaction ID
  Tid = Tid + 1;

  // Register myself
  XACT me;
  me.tid = Tid;
  int wid = 0;
  for (int curid = 0; curid < NUM_ACCESS; curid++)
  {
    if (local_db[curid].mode == WRITE)
    {
      me.ws[wid] = curid;
      wid++; // Increment counter for write set
    }
  }
  XactHist.push_back(me);

  pthread_mutex_unlock(&GiantLock);
}

void *
worker(void *arg)
{
  int myid = *(int *)arg;
  free(arg);
  for (int i = 0; i < NUM_TX; i++)
  {
    transaction(myid);
  }
  return NULL;
}

void init(void)
{
  pthread_mutex_init(&GiantLock, NULL);
}

void print_diff(struct timeval begin, struct timeval end)
{
  long diff = (end.tv_sec - begin.tv_sec) * 1000 * 1000 + (end.tv_usec - begin.tv_usec);
  printf("%ld us\n", diff);
}

int main(void)
{
  init();
  pthread_t thread[MAX_WORKER];
  struct timeval begin, end;

  gettimeofday(&begin, NULL);
  for (int i = 0; i < MAX_WORKER; i++)
  {
    int *id = (int *)calloc(1, sizeof(int));
    if (!id)
      ERR;
    *id = i;
    pthread_create(&thread[i], NULL, worker, id);
  }
  for (int i = 0; i < MAX_WORKER; i++)
  {
    pthread_join(thread[i], NULL);
  }
  gettimeofday(&end, NULL);
  print_diff(begin, end);

  return 0;
}
