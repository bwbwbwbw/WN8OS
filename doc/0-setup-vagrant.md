## 配置 Vagrant

1. 下载安装 VirtualBox 5.0.0，下载安装 Vagrant

2. 下载 precise64 镜像：http://files.vagrantup.com/precise64.box

3. 导入镜像
   ```bash
   vagrant box add precise64 ~/Downloads/precise64.box  # 最后参数为镜像下载位置
   ```

4. 安装 Guest Additions 自动升级插件
   ```bash
   vagrant plugin install vagrant-vbguest
   ```

5. 启动
   ```bash
   #cd WN8OS
   vagrant up
   ```

启动后，宿主机中当前目录（`WN8OS`）会挂载到客户机中的 `/vagrant` 目录下。

## 常用命令

`vagrant ssh`：连接进入客户机

`vagrant halt`：关闭客户机

`vagrant up`：启动客户机