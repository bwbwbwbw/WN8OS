## 如何调试

由于某些神奇的 bug，我们需要自己下载编译一份 gdb 才能正常调试。

```bash
wget http://ftp.gnu.org/gnu/gdb/gdb-7.8.tar.gz
tar xzf gdb-7.8.tar.gz
```

编辑 `gdb-7.8/gdb/remote.c`:

```cpp
static void
process_g_packet (struct regcache *regcache)
{
  struct gdbarch *gdbarch = get_regcache_arch (regcache);
  struct remote_state *rs = get_remote_state ();
  struct remote_arch_state *rsa = get_remote_arch_state ();
  int i, buf_len;
  char *p;
  char *regs;

  buf_len = strlen (rs->buf);

  /* Further sanity checks, with knowledge of the architecture.  */
  // 从此处开始
  // 以下代码需要被注释掉
/*
  if (buf_len > 2 * rsa->sizeof_g_packet)
    error (_("Remote 'g' packet reply is too long: %s"), rs->buf);
*/
  // 以下代码需要新增
  if (buf_len > 2 * rsa->sizeof_g_packet) {
    rsa->sizeof_g_packet = buf_len;
    for (i = 0; i < gdbarch_num_regs(gdbarch); i++) {
        if (rsa->regs[i].pnum == -1)    
            continue;
        if (rsa->regs[i].offset >= rsa->sizeof_g_packet)
            rsa->regs[i].in_g_packet = 0;
        else
            rsa->regs[i].in_g_packet = 1;
    }
  }
  // 到地处结束
  //...
}
```

最后编译：

```bash
./configure
make
sudo make install
```