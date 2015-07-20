#!/bin/bash
set -e

# 建立HDD磁盘镜像: 63 sectors per track, 16 heads, 50 cylinders ~= 55MB
# 建立在 /tmp 目录下.. 因为 /vagrant 目录下会因为 vbox 的 bug 而失败 :-(
echo -e "\033[32mCreating disk image /tmp/disk.img ...\033[0m"
dd if=/dev/zero of=/tmp/disk.img count=$((63*16*50))

# 创建分区表
echo -e "\033[32mCreating partition...\033[0m"
fdisk -u=sectors /tmp/disk.img << EOF
x
c
50
h
16
s
63
r
n
p
1
2048
50399
a
1
w
EOF

# 将磁盘镜像绑定到 loop0 上，分区绑定到 loop1 上
losetup /dev/loop0 /tmp/disk.img
losetup -o $((2048*512)) /dev/loop1 /tmp/disk.img

# 格式化分区为 EXT3
echo -e "\033[32mFormating partition...\033[0m"
mkfs.ext3 /dev/loop1

# 挂载分区
mount /dev/loop1 /mnt/img
mkdir -p /mnt/img/boot

# 复制内容
echo -e "\033[32mCopying kernel and boot configuration...\033[0m"
cp -R boot/* /mnt/img/boot/

# 安装 GRUB
echo -e "\033[32mInstalling GRUB...\033[0m"
cat > /mnt/img/boot/grub/device.map << EOF
(hd0) /dev/loop0
(hd0,1) /dev/loop1
EOF
grub-install --no-floppy --grub-mkdevicemap=/mnt/img/boot/grub/device.map --root-directory=/mnt/img /dev/loop0

# clean up
echo -e "\033[32mCleaning up...\033[0m"
umount /mnt/img
losetup -d /dev/loop1
losetup -d /dev/loop0
mv /tmp/disk.img ./disk.img

echo -e "\033[32mDone!\033[0m"