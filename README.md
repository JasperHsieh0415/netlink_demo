# Simple Netlink Communication Demo

This is a simple demo for netlink communication between kernel space and user space.

```app/netlink_test``` is for **netlink client**

```kernel/ast_netlink``` is for **netlink server**

## Usage

1. ```insmod ast_netlink.ko```

    module starts to send data (unsigned int num) every 1s

2. Run ```netlink_test```

    ```netlink_test``` in userspace starts to receive data via netlink

## Output

```console
[AST /]$ kernel send 5
netlink_test: recv:5
kernel send 6
netlink_test: recv:6
kernel send 7
netlink_test: recv:7
kernel send 8
netlink_test: recv:8
kernel send 9
netlink_test: recv:9
kernel send 10
netlink_test: recv:10
kernel send 11
netlink_test: recv:11
kernel send 12
netlink_test: recv:12
kernel send 13
netlink_test: recv:13
kernel send 14
netlink_test: recv:14
kernel send 15
netlink_test: recv:15
kernel send 16
netlink_test: recv:16
kernel send 17
netlink_test: recv:17
kernel send 18
...
```
