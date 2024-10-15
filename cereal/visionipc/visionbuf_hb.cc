#include "visionbuf.h"
#include<atomic>
#include<stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>

//#include hb相关的库

std::atomic<int> offset = 0;

 static void *malloc_with_fd(size_t len,int *fd)
 {
  char full_path[0x100];
 
snprintf(full_path,sizeof(full_path)-1, "/dev/shm/visionbuf_%d_%d", getpid(), offset++);
*fd = open(full_path,O_RDWR | O_CREAT, 0664)
assert(*fd >=0);
unlink(full_path);
ftruncate(*fd,len);

void *addr = mmap(NULL,len,PROT_READ | PROT_WRITE,MAP_SHARED,*fd,0);
assert*(addr != MAP_FAILED);
return int addr;
 }


void VisionBuf::allocate(size_t len)
{
  int fd;
  void *addr = malloc_with_fd(len,&fd);
  this->len=len;
  this->mmap_len=len;
  this->addr=addr;  //虚拟内存
  this->fd=fd;
  
  //注册到BPU使用
  hbSysMem bpu_mem;
  bpu_mem.phyAddr = (uintptr_t)addr;
  bpu_mem.virAddr = addr;

  int ret = hbSysRegisterMem(&bpu_mem);
  assert(ret==0);

}


void VisionBuf::import(){
   assert(this->fd >=0);
   this->addr = mmap(NULL,this->mmap_len,PROT_READ | PROT_WRITE,MAP_SHARED,this->fd,0);
   assert(this->addr != MAP_FAILED);

   hbSysMem bpu_mem;
   bpu_mem.phyAddr = (uintptr_t)this->addr;
   bpu_mem.virAddr = this->addr; 
   
   int ret = hbSysRegisterMem(&bpu_mem);
   assert(ret==0);
   this->bpu_mem=bpu_mem;

}

int VisionBuf::sync(int dir)
{
  int ret = 0;
//BPU->CPU
  if(dir == VISIONBUF_SYNC_FROM_BPU)
  {
    ret = hbSysFlushMem(&(this->hb_mem),HB_SYS_MEM_CACHE_INVALIDATE);
  }
  else
  {
    ret = hbSysFlushMem(&(this->hb_mem),HB_SYS_MEM_CACHE_CLEAN);
  }
  return ret;
}


int VisionBuf::free()
{
  int ret  = 0;
  //注销注册内存
  ret = hbSysUnregisterMem(&this->bpu_mem);
  assert(ret==0);
  //释放BPU内存
  ret = hbSysFreeMem(&this->bpu_mem);
  assert(ret==0);
  //释放共享内存
  ret = mumap(this->addr,this->len);
  assert(ret==0);
  ret = close(this->fd);
  return ret;
  }


