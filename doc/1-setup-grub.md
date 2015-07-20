## 自动建立镜像

```bash
# cd WN8OS
make pack
```

## 手工建立一个使用 GRUB 引导的镜像

注：以下大多数操作需要 root 权限，可以在客户机中操作

```bash
# 下载编译 GRUB
$ wget http://alpha.gnu.org/gnu/grub/grub-1.98.tar.gz
$ tar xzf grub-1.98.tar.gz
$ cd grub-1.98
$ ./configure --disable-werror  # 新版 GCC 不禁用这个会编译失败
$ make

# 建立HDD磁盘镜像: 63 sectors per track, 16 heads, 50 cylinders
$ dd if=/dev/zero of=disk.img count=$((63*16*50))

# 创建分区表
$ fdisk -u=sectors disk.img
> x     # 高级模式
> c     
> 50    # 50 个柱面
> h
> 16    # 16 个磁头
> s
> 63    # 63 个扇区
> r     # 退出高级模式
> n     # 新分区
> p     # 主分区
> 1     # 1号分区
> 2048  # 从 2048 扇区起始
> 10799 # 到 10799 扇区结束
> a     # 设置启动标记
> 1     # 一号分区
> w     # 写入

# 将磁盘镜像绑定到 loop0 上，分区绑定到 loop1 上
$ losetup /dev/loop0 disk.img
$ losetup -o $((2048*512)) /dev/loop1 disk.img

# 格式化分区为 EXT3
$ mkfs.ext3 /dev/loop1

# 挂载分区
$ mkdir /mnt/img
$ mount /dev/loop1 /mnt/img

# 建立 mapping 文件，指明 GRUB 安装位置
$ mkdir -p /mnt/img/boot/grub
$ cat > /mnt/img/boot/grub/device.map << EOF
(hd0) /dev/loop0
(hd0,1) /dev/loop1
EOF

# 安装 GRUB
$ grub-install --no-floppy --grub-mkdevicemap=/mnt/img/boot/grub/device.map --root-directory=/mnt/img /dev/loop0
```