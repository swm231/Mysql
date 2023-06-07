# 常用命令

## 帮助命令

```shell
docker version 		 # 显示docker的版本信息
docker info			 # 显示docker的系统信息，包括镜像和容器的数量
docker 命令 --help	# 帮助命令
```

帮助文档地址:https://docs.docker.com/engine/reference/commandline/docker/

## 镜像命令

**docker images 查看所有本地主机上的镜像**

```shell
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker images
REPOSITORY   TAG       IMAGE ID       CREATED         SIZE
centos       latest    5d0da3dc9764   20 months ago   231MB

# 解释
REPOSITORY	镜像的仓库源
TAG			镜像的标签
IMAGE ID	镜像的id
CREATED		镜像的创建时间
SIZE		镜像的大小

# 可选项
-a, --all 		# 列出所有镜像
-q, --quiet		# 只显示镜像的id
```

**docker search 搜索镜像**

```shell
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker search mysql
NAME                            DESCRIPTION                                     STARS     OFFICIAL   AUTOMATED
mysql                           MySQL is a widely used, open-source relation…   14201     [OK]       
mariadb                         MariaDB Server is a high performing open sou…   5422      [OK]       
percona                         Percona Server is a fork of the MySQL relati…   614       [OK]       

# 可选项，通过搜索来过滤
--filter=STARS=3000  	# 搜索出来的镜像就是STARS大于3000的
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker search mysql --filter=STARS=3000
NAME      DESCRIPTION                                     STARS     OFFICIAL   AUTOMATED
mysql     MySQL is a widely used, open-source relation…   14201     [OK]       
mariadb   MariaDB Server is a high performing open sou…   5422      [OK]       
```

**docker pull下载镜像**

```shell
docker pull 镜像名[:tag]  # 下载镜像
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker pull mysql
Using default tag: latest 		# 如果不写tag，默认是latest
latest: Pulling from library/mysql
72a69066d2fe: Pull complete  	# 分层下载，docker iamge的核心 联合文件系统
93619dbc5b36: Pull complete 
99da31dd6142: Pull complete 
626033c43d70: Pull complete 
37d5d7efb64e: Pull complete 
ac563158d721: Pull complete 
d2ba16033dad: Pull complete 
688ba7d5c01a: Pull complete 
00e060b6d11d: Pull complete 
1c04857f594f: Pull complete 
4d7cfa90e6ea: Pull complete 
e0431212d27d: Pull complete 
Digest: sha256:e9027fe4d91c0153429607251656806cc784e914937271037f7738bd5b8e7709	# 签名
Status: Downloaded newer image for mysql:latest
docker.io/library/mysql:latest	# 真实地址

# 下面两个命令等价
docker pull mysql
docker pull docker.io/library/mysql:lates

# 指定版本下载
docker pull mysql:5.7

swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker pull mysql:5.7
5.7: Pulling from library/mysql
72a69066d2fe: Already exists 
93619dbc5b36: Already exists 
99da31dd6142: Already exists 
626033c43d70: Already exists 
37d5d7efb64e: Already exists 
ac563158d721: Already exists 
d2ba16033dad: Already exists 
0ceb82207cd7: Pull complete 
37f2405cae96: Pull complete 
e2482e017e53: Pull complete 
70deed891d42: Pull complete 
Digest: sha256:f2ad209efe9c67104167fc609cca6973c8422939491c9345270175a300419f94
Status: Downloaded newer image for mysql:5.7
docker.io/library/mysql:5.7
```

![image-20230607183307003](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230607183307003.png)

**docker rmi 删除镜像**

```shell
docker rmi -f 镜像id 								# 删除镜像
docker rmi -f 镜像id 镜像id 镜像id 镜像id		    # 删除多个镜像

swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker rmi -f c20987f18b13
Untagged: mysql:5.7
Untagged: mysql@sha256:f2ad209efe9c67104167fc609cca6973c8422939491c9345270175a300419f94
Deleted: sha256:c20987f18b130f9d144c9828df630417e2a9523148930dc3963e9d0dab302a76
Deleted: sha256:6567396b065ee734fb2dbb80c8923324a778426dfd01969f091f1ab2d52c7989
Deleted: sha256:0910f12649d514b471f1583a16f672ab67e3d29d9833a15dc2df50dd5536e40f
Deleted: sha256:6682af2fb40555c448b84711c7302d0f86fc716bbe9c7dc7dbd739ef9d757150
Deleted: sha256:5c062c3ac20f576d24454e74781511a5f96739f289edaadf2de934d06e910b92

docker rmi -f $(docker images -aq)  			 # 删除所有镜像
```

## 容器命令

**说明：有了镜像才可以创建容器**

```shell
docker pull centos
```

**创建容器并启动**

```shell
docker run [可选参数] image

# 参数说明
--name="Name"	# 容器名字，用来区分容器
-d				# 后台方式运行
-it				# 使用交互方式运行，进入容器，查看内容
-p				# 指定容器端口
	-p ip:主机端口:容器端口
	-p 主机端口:容器端口
	-p 容器端口
	容器端口
-P				# 随机指定端口

# 测试，启动并进入容器
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker run -it centos /bin/bash
[root@709dfa8cb2a7 /]# ls
bin  dev  etc  home  lib  lib64  lost+found  media  mnt  opt  proc  root  run  sbin  srv  sys  tmp  usr  var

# 从容器中退出
exit 		 	# 停止容器并退出
ctrl + p + q 	# 后台挂起并退出
```

**查看所有运行的容器**

```shell
# docker ps 命令
		# 列出所有当前正在运行的容器
-a 		# 列出所有当前正在运行的容器+历史运行过的容器
-n=?	# 显示最近创建的容器
-q 		# 只显示容器编号
CONTAINER ID   IMAGE     COMMAND                  CREATED          STATUS          PORTS     NAMES
34a07c09bb72   centos    "/bin/bash"              8 minutes ago    Up 8 minutes              cranky_ardinghelli
e276bad80a68   centos    "/bin/bash -c 'while…"   27 minutes ago   Up 27 minutes             boring_torvalds
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker ps -a
CONTAINER ID   IMAGE     COMMAND                  CREATED          STATUS                      PORTS     NAMES
34a07c09bb72   centos    "/bin/bash"              8 minutes ago    Up 8 minutes                          cranky_ardinghelli
e276bad80a68   centos    "/bin/bash -c 'while…"   27 minutes ago   Up 27 minutes                         boring_torvalds
55fc92576e44   centos    "/bin/bash"              30 minutes ago   Exited (0) 30 minutes ago             youthful_rosalind
e976cecfbec9   centos    "/bin/bash"              2 hours ago      Exited (0) 2 hours ago                charming_brattain
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker ps -aq
34a07c09bb72
e276bad80a68
55fc92576e44
e976cecfbec9
```

**删除容器**

```shell
docker rm 容器id			        # 删除指定容器
docker rm -f 容器id			    # 强制删除
docker rm $(docker ps -aq) 	 	 # 删除所有容器
docker ps -a -q|xargs docker rm	 # 删除所有容器
```

**启动和停止容器**

```shell
docker start 容器id		# 启动容器
docker restart 容器id		# 重启容器
docker stop	容器id		# 停止容器
docker kill 容器id		# 强制停止
```



## 其他命令

**后台启动容器**

```shell
# 命令 docker run -d 镜像名

swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker run -d centos
55fc92576e44792e60f68134e6ec4fff461018583201b0e249cd3da45d7c1a64
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker ps
CONTAINER ID   IMAGE     COMMAND   CREATED   STATUS    PORTS     NAMES

# 问题docker ps后，发现centos停止了
# 原因：docker容器使用后台运行，就必须要有一个前台进程，docker发现没有应用，就会自动停止
# nginx容器启动后，发现自己没有提供服务，就会立刻停止
```

**查看日志**

```shell
docker logs -f -t --tail 容器
# 没有日志

# 自己编写一段shell脚本
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker run -d centos /bin/bash -c "while true; do echo swm_231;sleep 1; done"
e276bad80a68f25127bb791375e04eb808122aee576ae498de8fc89164ce3f10
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker ps
CONTAINER ID   IMAGE     COMMAND                  CREATED         STATUS         PORTS     NAMES
e276bad80a68   centos    "/bin/bash -c 'while…"   5 seconds ago   Up 4 seconds             boring_torvalds

# 显示日志
-tf 			# 显示日志
--tail number 	# 要显示的日志条数
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker logs -tf --tail 10 e276bad80a68
2023-06-07T10:52:51.078389526Z swm_231
2023-06-07T10:52:52.080230038Z swm_231
2023-06-07T10:52:53.081882805Z swm_231
...

```

**查看容器中的进程信息**

```shell
docker top 容器id

swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker top e276bad80a68
UID                 PID                 PPID                C                   STIME               TTY                 TIME                CMD
root                22885               22865               0                   18:22               ?    
root                25514               22885               0                   18:54               ?  
```

**查看容器源数据**

```shell
docker inspect 容器id

swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker inspect e276bad80a68
[
    {
        "Id": "e276bad80a68f25127bb791375e04eb808122aee576ae498de8fc89164ce3f10",
        "Created": "2023-06-07T10:22:06.994210904Z",
        "Path": "/bin/bash",
        "Args": [
            "-c",
            "while true; do echo swm_231;sleep 1; done"
        ],
        "State": {
            "Status": "running",
            "Running": true,
            "Paused": false,
            "Restarting": false,
            "OOMKilled": false,
            "Dead": false,
            "Pid": 22885,
            "ExitCode": 0,
            "Error": "",
            "StartedAt": "2023-06-07T10:22:07.251938326Z",
            "FinishedAt": "0001-01-01T00:00:00Z"
        },
        "Image": "sha256:5d0da3dc976460b72c77d94c8a1ad043720b0416bfc16c52c45d4847e53fadb6",
        "ResolvConfPath": "/var/lib/docker/containers/e276bad80a68f25127bb791375e04eb808122aee576ae498de8fc89164ce3f10/resolv.conf",
        "HostnamePath": "/var/lib/docker/containers/e276bad80a68f25127bb791375e04eb808122aee576ae498de8fc89164ce3f10/hostname",
        "HostsPath": "/var/lib/docker/containers/e276bad80a68f25127bb791375e04eb808122aee576ae498de8fc89164ce3f10/hosts",
        "LogPath": "/var/lib/docker/containers/e276bad80a68f25127bb791375e04eb808122aee576ae498de8fc89164ce3f10/e276bad80a68f25127bb791375e04eb808122aee576ae498de8fc89164ce3f10-json.log",
        "Name": "/boring_torvalds",
        "RestartCount": 0,
        "Driver": "overlay2",
        "Platform": "linux",
        "MountLabel": "",
        "ProcessLabel": "",
        "AppArmorProfile": "docker-default",
        "ExecIDs": null,
        "HostConfig": {
            "Binds": null,
            "ContainerIDFile": "",
            "LogConfig": {
                "Type": "json-file",
                "Config": {}
            },
            "NetworkMode": "default",
            "PortBindings": {},
            "RestartPolicy": {
                "Name": "no",
                "MaximumRetryCount": 0
            },
            "AutoRemove": false,
            "VolumeDriver": "",
            "VolumesFrom": null,
            "ConsoleSize": [
                37,
                90
            ],
            "CapAdd": null,
            "CapDrop": null,
            "CgroupnsMode": "host",
            "Dns": [],
            "DnsOptions": [],
            "DnsSearch": [],
            "ExtraHosts": null,
            "GroupAdd": null,
            "IpcMode": "private",
            "Cgroup": "",
            "Links": null,
            "OomScoreAdj": 0,
            "PidMode": "",
            "Privileged": false,
            "PublishAllPorts": false,
            "ReadonlyRootfs": false,
            "SecurityOpt": null,
            "UTSMode": "",
            "UsernsMode": "",
            "ShmSize": 67108864,
            "Runtime": "runc",
            "Isolation": "",
            "CpuShares": 0,
            "Memory": 0,
            "NanoCpus": 0,
            "CgroupParent": "",
            "BlkioWeight": 0,
            "BlkioWeightDevice": [],
            "BlkioDeviceReadBps": [],
            "BlkioDeviceWriteBps": [],
            "BlkioDeviceReadIOps": [],
            "BlkioDeviceWriteIOps": [],
            "CpuPeriod": 0,
            "CpuQuota": 0,
            "CpuRealtimePeriod": 0,
            "CpuRealtimeRuntime": 0,
            "CpusetCpus": "",
            "CpusetMems": "",
            "Devices": [],
            "DeviceCgroupRules": null,
            "DeviceRequests": null,
            "MemoryReservation": 0,
            "MemorySwap": 0,
            "MemorySwappiness": null,
            "OomKillDisable": false,
            "PidsLimit": null,
            "Ulimits": null,
            "CpuCount": 0,
            "CpuPercent": 0,
            "IOMaximumIOps": 0,
            "IOMaximumBandwidth": 0,
            "MaskedPaths": [
                "/proc/asound",
                "/proc/acpi",
                "/proc/kcore",
                "/proc/keys",
                "/proc/latency_stats",
                "/proc/timer_list",
                "/proc/timer_stats",
                "/proc/sched_debug",
                "/proc/scsi",
                "/sys/firmware"
            ],
            "ReadonlyPaths": [
                "/proc/bus",
                "/proc/fs",
                "/proc/irq",
                "/proc/sys",
                "/proc/sysrq-trigger"
            ]
        },
        "GraphDriver": {
            "Data": {
                "LowerDir": "/var/lib/docker/overlay2/eec8f425354622da88b4448fbb67de0d99e8586a28aa73bb8e82a636adca0ab2-init/diff:/var/lib/docker/overlay2/e15e667fe15b41a7ba2b435136658472fea8f3b109577034fc1fd15c222f68f8/diff",
                "MergedDir": "/var/lib/docker/overlay2/eec8f425354622da88b4448fbb67de0d99e8586a28aa73bb8e82a636adca0ab2/merged",
                "UpperDir": "/var/lib/docker/overlay2/eec8f425354622da88b4448fbb67de0d99e8586a28aa73bb8e82a636adca0ab2/diff",
                "WorkDir": "/var/lib/docker/overlay2/eec8f425354622da88b4448fbb67de0d99e8586a28aa73bb8e82a636adca0ab2/work"
            },
            "Name": "overlay2"
        },
        "Mounts": [],
        "Config": {
            "Hostname": "e276bad80a68",
            "Domainname": "",
            "User": "",
            "AttachStdin": false,
            "AttachStdout": false,
            "AttachStderr": false,
            "Tty": false,
            "OpenStdin": false,
            "StdinOnce": false,
            "Env": [
                "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
            ],
            "Cmd": [
                "/bin/bash",
                "-c",
                "while true; do echo swm_231;sleep 1; done"
            ],
            "Image": "centos",
            "Volumes": null,
            "WorkingDir": "",
            "Entrypoint": null,
            "OnBuild": null,
            "Labels": {
                "org.label-schema.build-date": "20210915",
                "org.label-schema.license": "GPLv2",
                "org.label-schema.name": "CentOS Base Image",
                "org.label-schema.schema-version": "1.0",
                "org.label-schema.vendor": "CentOS"
            }
        },
        "NetworkSettings": {
            "Bridge": "",
            "SandboxID": "eb09defef60619dab7987986294afa368c6f768118a08c097fb00b00b5be3f5d",
            "HairpinMode": false,
            "LinkLocalIPv6Address": "",
            "LinkLocalIPv6PrefixLen": 0,
            "Ports": {},
            "SandboxKey": "/var/run/docker/netns/eb09defef606",
            "SecondaryIPAddresses": null,
            "SecondaryIPv6Addresses": null,
            "EndpointID": "fd0a8613e39d2d066113a1b3a52528a7f9c919899d431c8040f96f59b9e1a753",
            "Gateway": "172.17.0.1",
            "GlobalIPv6Address": "",
            "GlobalIPv6PrefixLen": 0,
            "IPAddress": "172.17.0.2",
            "IPPrefixLen": 16,
            "IPv6Gateway": "",
            "MacAddress": "02:42:ac:11:00:02",
            "Networks": {
                "bridge": {
                    "IPAMConfig": null,
                    "Links": null,
                    "Aliases": null,
                    "NetworkID": "8f81bc3fe6952cd3c1779d46ca6f12f114750c275ce821ccf2817f77a6584f16",
                    "EndpointID": "fd0a8613e39d2d066113a1b3a52528a7f9c919899d431c8040f96f59b9e1a753",
                    "Gateway": "172.17.0.1",
                    "IPAddress": "172.17.0.2",
                    "IPPrefixLen": 16,
                    "IPv6Gateway": "",
                    "GlobalIPv6Address": "",
                    "GlobalIPv6PrefixLen": 0,
                    "MacAddress": "02:42:ac:11:00:02",
                    "DriverOpts": null
                }
            }
        }
    }
]
```

**进入当前正在运行的容器**

```shell
# 方式一
docker exec -it 容器id bashShell

# 测试
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker exec -it e276bad80a68 /bin/bash
[root@e276bad80a68 /]# ls
bin  dev  etc  home  lib  lib64  lost+found  media  mnt  opt  proc  root  run  sbin  srv  sys  tmp  usr  var

# 方式二
docker attach 容器id
# 测试
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker attach e276bad80a68
正在执行当前的代码....

docker exec  	# 进入容器后开启一个新的终端
docker attach	# 进入容器中正在执行的终端
```



**把容器内文件拷贝到主机上**

```shell
docker cp 容器id:容器内路径 目的路径

swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker ps
CONTAINER ID   IMAGE     COMMAND       CREATED          STATUS          PORTS     NAMES
e976cecfbec9   centos    "/bin/bash"   34 minutes ago   Up 34 minutes             charming_b
# 进入容器
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker attach e976cecfbec9
[root@e976cecfbec9 /]# cd /home
[root@e976cecfbec9 home]# ls
# 新建文件
[root@e976cecfbec9 home]# touch test.cpp
[root@e976cecfbec9 home]# exit
exit
# 复制文件出来
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:/home$ docker cp e976cecfbec9:/home/test.cpp /home/swm_231/
Successfully copied 1.54kB to /home/swm_231/
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:/home$ cd
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ ls
c_client  CloudMeeting_Server-master  cmake  c_server  ds  library  Mysql  test  test.cpp

# 拷贝是一个手动的过程，未来我们使用 -v 卷的技术，可以实现自动同步
```

## 小结

![image-20230607190441466](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230607190441466.png)

## 练习

**docker安装nginx**

```shell
# 1 搜索镜像
# 2 下载镜像
# 3 运行测试
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker run -d --name nginx01 -p 8888:80 nginx 
580ab4b311247312c6c167088d5b4e9a47cb081111a34c27310c4aa5808433ff
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker ps
CONTAINER ID   IMAGE     COMMAND                  CREATED         STATUS         PORTS                                   NAMES
580ab4b31124   nginx     "/docker-entrypoint.…"   4 seconds ago   Up 3 seconds   0.0.0.0:8888->80/tcp, :::8888->80/tcp   nginx01
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ curl localhost:8888
<!DOCTYPE html>
<html>
<head>
<title>Welcome to nginx!</title>
<style>
html { color-scheme: light dark; }
body { width: 35em; margin: 0 auto;
font-family: Tahoma, Verdana, Arial, sans-serif; }
</style>
</head>
<body>
<h1>Welcome to nginx!</h1>
<p>If you see this page, the nginx web server is successfully installed and
working. Further configuration is required.</p>

<p>For online documentation and support please refer to
<a href="http://nginx.org/">nginx.org</a>.<br/>
Commercial support is available at
<a href="http://nginx.com/">nginx.com</a>.</p>

<p><em>Thank you for using nginx.</em></p>
</body>
</html>

# 进入容器
swm_231@iZ0jl8mpf6u9tuysfpxoorZ:~$ docker exec -it nginx01 /bin/bash
root@580ab4b31124:/# ls
bin   dev		   docker-entrypoint.sh  home  lib64  mnt  proc  run   srv  tmp  var
boot  docker-entrypoint.d  etc			 lib   media  opt  root  sbin  sys  usr
root@580ab4b31124:/# whereis nginx
nginx: /usr/sbin/nginx /usr/lib/nginx /etc/nginx /usr/share/nginx
# 查看默认html
root@580ab4b31124:/usr/share/nginx/html# cat index.html 
<!DOCTYPE html>
<html>
<head>
<title>Welcome to nginx!</title>
<style>
html { color-scheme: light dark; }
body { width: 35em; margin: 0 auto;
font-family: Tahoma, Verdana, Arial, sans-serif; }
</style>
</head>
<body>
<h1>Welcome to nginx!</h1>
<p>If you see this page, the nginx web server is successfully installed and
working. Further configuration is required.</p>

<p>For online documentation and support please refer to
<a href="http://nginx.org/">nginx.org</a>.<br/>
Commercial support is available at
<a href="http://nginx.com/">nginx.com</a>.</p>

<p><em>Thank you for using nginx.</em></p>
</body>
</html>
```

**安装 es + kibana**

``` shell
# es 暴露的端口非常多
# es 十分耗费内存
# es 的数据一般需要放置到安全目录！挂载
# --net somenetwork 网络配置

# 启动 elasticsearch
docker run -d --name elasticsearch -p 9200:9200 -p 9300:9300 -e "discovery.type=single-node" elasticsearch:7.6.2

# 启动之后就卡住了 docker stats 查看cpu状态
```

![image-20230607203234899](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230607203234899.png)

```shell
# 增加内存限制，修改配置文件 -e 环境配置修改
docker run -d --name elasticsearch -p 9200:9200 -p 9300:9300 -e "discovery.type=single-node" -e ES_JAVA_OPTS="-Xms64m -Xmx512m" elasticsearch:7.6.2
```

![image-20230607203531664](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20230607203531664.png)

## 可视化

- portainer （先用这个）

```shell
docker run -d -p 8088:9000 \
--restart=always -v /var/run/docker.sock:/var/run/docker.sock --privileged=true portainer/portainer
```

- Rancher（CI/DC再用）



**什么是portainer** 

Docker图形化界面管理工具！提供一个后台面板供我们操作！

# 镜像

## commit镜像

```shell
docker commit 提交容器成为一个新的镜像

# 命令和git原理类似
docker commit -m "提交的信息描述" -a "作者" 容器id 目标镜像名:[tag]
```

```shell
# 1 启动默认的tomcat
# 2 默认的webapps下没有文件
# 3 自己拷贝基本的文件进去
# 4 将修改后的容器提交为一个镜像
```



