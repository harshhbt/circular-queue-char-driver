#include<linux/module.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/cdev.h>

#define DEVICE_NAME "circular_queue"
#define BUF_SIZE 256

struct queue_buffer{
    char buf[BUF_SIZE];
    size_t head;
    size_t tail;
};

 static dev_t dev_num;
 static struct cdev queue_cdev;
 static struct queue_buffer qb;

static inline bool qb_empty(struct queue_buffer *r)
{
    return r->head == r->tail;
}
static inline bool qb_full(struct queue_buffer *r)
{
    return((r->head + 1)% BUF_SIZE) == r->tail;
}
//read
static ssize_t queue_read(struct file *file, 
                          char __user *ubuf,
                          size_t count,
                          loff_t *ppos)
{ size_t read = 0;
  char c;

  while(read<count){
    if(qb_empty(&qb))
    break;
c = qb.buf[qb.tail];
qb.tail = (qb.tail + 1)%BUF_SIZE;

if (copy_to_user(ubuf + read ,&c, 1))
    return read;
    
    read++;  
}
    return read;
}
//write
static ssize_t queue_write(struct file *file, 
                          const char __user *ubuf,
                          size_t count,
                          loff_t *ppos)
{ size_t written = 0;
  char c;
    while(written<count){
        if (copy_from_user(&c, ubuf + written, 1))
        return written;
    if(qb_full(&qb))
    break;
    qb.buf[qb.head] = c;
    qb.head = (qb.head +1)% BUF_SIZE;
    written++;
    }
  return written;
}
static struct file_operations fops ={
    .owner =THIS_MODULE,
    .read = queue_read,
    .write = queue_write,
};


static int __init queue_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if(ret)
    return ret;
    
    cdev_init(&queue_cdev, &fops);
    ret = cdev_add(&queue_cdev, dev_num, 1);
    if(ret)
    goto unregister;
   
    qb.head = qb.tail =0;

    pr_info("char device loaded: major = %d\n", MAJOR(dev_num));
    return 0;

unregister:
   unregister_chrdev_region(dev_num, 1);
   return ret;
}

static void __exit queue_exit(void)
{
    cdev_del(&queue_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("circular_queue unloaded\n");
}

module_init(queue_init);
module_exit(queue_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HARSH KUMAR");
MODULE_DESCRIPTION("CHAR DEVICE WITH CIRCULAR QUEUE - NO LOCKING");
