# Circular Queue Character Device Driver

Minimal Linux character device implementing a FIFO circular queue (no locking)

A simple Linux kernel character device that implements a fixed-size circular queue i.e. ring bufferwith no locking.

This module allows user-space programs to write bytes into a kernel buffer and read them back in FIFO order.

---

## Features

- Character device driver
- Circular buffer of 256 bytes
- FIFO read/write behavior
- No locking (single-producer/single-consumer or non-concurrent use only)
- Minimal and easy to understand

---

## Build

Make sure you have kernel headers installed.

```bash
make
```

Load the Module
```bash
sudo insmod circular_queue.ko
dmesg | tail
```
Create a Device Node
```bash
sudo mknod /dev/circular_queue c <MAJOR_NUMBER> 0
sudo chmod 666 /dev/circular_queue
```
Usage
Write to device
```bash
echo "hello world" > /dev/circular_queue
```
Read from device
```bash
cat /dev/circular_queue
```
OUTPUT:
hello world

