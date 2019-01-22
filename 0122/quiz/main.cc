#include "debug.h"
#include "aries.h"
#define LOG_FILE "LogFile"
#define MAX_TRANXACT (1000)

OBJ TmpObj[MAX_OBJ]; // tmp obj for redo and undo

void
dispTxTable(void)
{
  for( auto itr = TxTable.begin(); itr != TxTable.end(); itr++ ){
    printf("%4d:%4d:%d\n", itr->txid, itr->llsn, itr->stat);
  }
}

void
updateTxTable(int txid, int lsn, TX_STAT stat)
{
  for (auto itr=TxTable.begin(); itr!=TxTable.end(); itr++) {
    if (itr->txid == txid) {
      itr->llsn = lsn; 
      return;
    }
  }

  TX_TABLE tt;
  tt.txid = txid;
  tt.llsn = lsn;
  tt.stat = stat;
  TxTable.push_back(tt);
}

void
finalizeTxTable(int txid, TX_STAT stat)
{
  for( auto itr=TxTable.begin(); itr!=TxTable.end(); itr++ ){
    if (itr->txid == txid) {
      itr->stat = stat;
      return;
    }
  }
  ERR;
}

void
redo_test()
{
  LOG l;
  for (int i = 0; i < MAX_OBJ; i++) {
    TmpObj[i].lsn = -1;
  }

  // Redo
  int fd = open(LOG_FILE, O_RDONLY); if( fd == -1 )ERR;
  while (true){
    int ret = read(fd, &l, sizeof(LOG));
    if (ret == 0) break; else if (ret<0) ERR;

    // Redo core
    if (l.type == UPDATE) {
      TmpObj[l.oid].val = l.after;
      TmpObj[l.oid].lsn = l.lsn;
      updateTxTable(l.txid, l.lsn, INPROGRESS);
    }
    else if (l.type == COMMIT) {
      finalizeTxTable(l.txid, COMMITING);
    }
    else if (l.type == ABORT) {
      finalizeTxTable(l.txid, ABORTING);
    }
  }
  int ret = close(fd); if( ret<0 )ERR;

  // State check for REDO
  for (int i = 0; i < MAX_OBJ; i++) {
    if (Obj[i].lsn != -1) {
      if (Obj[i].lsn == TmpObj[i].lsn && Obj[i].val == TmpObj[i].val) /* ok */;
      else ERR;
    }
  }
}

void
safe_terminate(void)
{
  int ret = close(Logfd);
  if( ret<0 ) ERR;
}

void
undo_core(int lsn)
{
  int fd;
  int n;
  LOG l;

  fd = open(LOG_FILE, O_RDONLY); if( fd == -1 )ERR;
  while (true) {
    n = lseek(fd, lsn * sizeof(LOG), SEEK_SET);
    n = read(fd, &l, sizeof(LOG)); if( n<0 )ERR;
    TmpObj[l.oid].val = l.before;
    if (l.plsn == -1) break;
    lsn = l.plsn;
  }
  n = close(fd); if (n<0) ERR;
  printf("Undo Success\n");
}

void
undo_test()
{
  dispTxTable();
  for (auto itr=TxTable.begin(); itr!=TxTable.end(); itr++){
    if (itr->stat != COMMITING) {
      undo_core(itr->llsn);
    }
  }
}

void 
sig_handler(int sig)
{
  printf("Bye bye---, your database is inconsistent ^_^/ \n");
  exit(0);

  printf("Received Signal\n");
  safe_terminate();
  printf("Redo starts\n");
  redo_test();
  printf("Redo ends\n");
  printf("Undo starts\n");
  undo_test();
  printf("Undo ends\n");
  exit(0);
}

void
init(void)
{
  // Signal
  if (signal(SIGINT, sig_handler) == SIG_ERR) ERR;

  // File
  Logfd = open(LOG_FILE, O_WRONLY|O_CREAT|O_APPEND|O_TRUNC, 0644);
  if (Logfd == -1) ERR;

  // Init database
  for (int i = 0; i < MAX_OBJ; i++) {
    Obj[i].lsn = -1;
    Obj[i].val = i * 10;
  }
}

int
getLsn(void)
{
  // lock
  static int lsn = 0;
  lsn++;
  //unlock
  return lsn;
}

LOG
getCommitLog(int txid)
{
  LOG l;
  l.txid = txid;
  l.type = COMMIT;

  return l;
}

LOG
getUpdateLog(int txid, int oid, int after)
{
  LOG l;

  l.plsn = Obj[oid].lsn;
  l.lsn = getLsn();
  l.txid = txid;
  l.type = UPDATE;
  l.oid = oid;
  l.before = Obj[oid].val;
  l.after = after;

  return l;
}

void
normal(const int txid)
{
  LOG l;
  int nupdate = rand() % 100 + 1;

  for (int i = 0; i < nupdate; i++) {
    usleep(1000);

    // Update an object
    int oid = rand() % MAX_OBJ;
    int after = rand() % 10;

    // Make a log
    l = getUpdateLog(txid, oid, after);
    int ret = write(Logfd, &l, sizeof(l)); if(ret<0)ERR;

    // Update database
    Obj[oid].val = after;
    Obj[oid].lsn = l.lsn;

    // Modify Transaction Table
    updateTxTable(l.txid, l.lsn, INPROGRESS);
  }
  l = getCommitLog(txid);
  int ret = write(Logfd, &l, sizeof(l)); if(ret<0)ERR;
  finalizeTxTable(l.txid, COMMITING);
}

int
main(void)
{
  init();
  std::cout << "Init Done" << std::endl;

  for( int txid = 1; txid <= MAX_TRANXACT; txid++ ){
    printf("Trans: %d\n", txid);
    normal(txid);
  }
  dispTxTable();

  return 0;
}
