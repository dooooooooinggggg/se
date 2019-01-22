#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <list>
#include <iostream>
using namespace std;

#define MAX_OBJ (1000)
typedef enum{UPDATE, 
             COMMIT, 
             ABORT} OPTYPE;
typedef enum{INPROGRESS, 
             COMMITING, 
             ABORTING} TX_STAT;

typedef struct _LOG{
  int plsn; // prev LSN
  int lsn;  // LSN
  int txid; // transid
  OPTYPE type;
  int oid; // Object ID
  int length;
  int before;
  int after;
} LOG;

typedef struct _OBJ{
  pthread_mutex_t lock;
  int lsn;
  int val;
} OBJ;

typedef struct _TX_TABLE {
  int txid; // TX ID
  int llsn; // Last LSN
  TX_STAT stat; // Status
} TX_TABLE;

OBJ Obj[MAX_OBJ];
int Logfd;
list<TX_TABLE> TxTable;
