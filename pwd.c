#include "types.h"
#include "fcntl.h"
#include "defs.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"
#include "stat.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"

int
get_dir_name(struct inode *ip, struct inode *parent, char buf[DIRSIZ])
{
  // Look for an empty dirent.
  int off;
  struct dirent de;
  for(off = 0; off < parent->size; off += sizeof(de)){
    if(readi(parent, (char*)&de, off, sizeof(de)) != sizeof(de))
      panic("dirlink read");
    if(de.inum == ip->inum) {
      safestrcpy(buf, de.name, DIRSIZ);
      return 1;
    }
  }
  return 0;
}

int
get_path_name(struct inode *ip, char *buf, int path_length) {
  char dirname[DIRSIZ];
  struct inode *parent;
  if (ip->inum == namei("/")->inum) {
    buf[0] = '/';
    return 1;
  } else {
    if (ip->type != T_DIR)
      panic("get_path_name not directory");
    uint poff = 0;
    parent = dirlookup(ip, "..", &poff);
    ilock(parent);
    int off_set = get_path_name(parent, buf, path_length);
    if (get_dir_name(ip, parent, dirname) == 0) {
      iunlock(parent);
      panic("directory not found in parent");
    }
    iunlock(parent);
    safestrcpy(buf + off_set, dirname, path_length - off_set);
    off_set = strlen(buf);
    if (off_set == path_length - 1) {
      buf[off_set] = '\0';
      return path_length;
    } else {
      buf[off_set] = '/';
      off_set += 1;
    }
    iput(parent);
    return off_set;
  }
}

int
sys_pwd(void)
{
  int maxsize;
  char *path;
  if(argstr(0, &path) < 0 || argint(1, &maxsize) < 0)
    return -1;
  get_path_name(proc->cwd, path, maxsize);
  return 0;
}