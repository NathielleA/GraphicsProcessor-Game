#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include "address_map_arm.h"

#define DEVICE_NAME "gpp_data_bus"
#define CLASS_NAME "gppdatabus_class"

/*Defining memory base addreses*/
#define LW_VIRTUAL 0xFF200000
#define DATA_A_OFFSET 0x70
#define DATA_B_OFFSET 0x80
#define DATA_A_PHYS_ADDR (LW_VIRTUAL + DATA_A_OFFSET)
#define DATA_B_PHYS_ADDR (LW_VIRTUAL + DATA_B_OFFSET)

// AONDE FICA O BRIDGE_SPAN?? E USA ESSA JOÇA?

#define SUCCESS 0

/*Global variables and structs for the functions of the module*/
static int majorNumber;
static struct class *dataBusClass = NULL;
static struct device *dataBusDevice = NULL;

void __iomem *data_a_mem;
void __iomem *data_b_mem;

/*Declaring default functions and the file operations*/
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {.owner = THIS_MODULE,
                                      .read = data_bus_read,
                                      .write = data_bus_write,
                                      .open = data_bus_open,
                                      .release = data_bus_release};
}

static int __init data_bus_init(void) {
  printk(KERN_INFO "DataBus: Initializing the DataBus\n");

  majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
  if (majorNumber < 0) {
    printk(KERN_ALERT "DataBus failed to register a major number\n");
    return majorNumber;
  }
  printk(KERN_INFO "DataBus: registered correctly with major number %d\n", majorNumber);

  dataBusClass = class_create(THIS_MODULE, CLASS_NAME);
  if (IS_ERR(dataBusClass)) {
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_ALERT "Failed to register device class\n");
    return PTR_ERR(dataBusClass);
  }
  printk(KERN_INFO "DataBus: device class registered correctly\n");

  dataBusDevice = device_create(dataBusClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
  if (IS_ERR(dataBusDevice)) {
    class_destroy(dataBusClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_ALERT "Failed to create the device\n");
    return PTR_ERR(dataBusDevice);
  }
  printk(KERN_INFO "DataBus: device class created correctly\n");

  /*Mapping memory address for Data A and Data B*/
  data_a_mem = ioremap(DATA_A_PHYS_ADDR, sizeof(uint32_t));
  if (!data_a_mem) {
    printk(KERN_ALERT "Failed to map memory for DATA_A\n");
    device_destroy(dataBusClass, MKDEV(majorNumber, 0));
    class_destroy(dataBusClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    return -EIO;
  }

  data_b_mem = ioremap(DATA_B_PHYS_ADDR, sizeof(uint32_t));
  if (!data_b_mem) {
    printk(KERN_ALERT "Failed to map memory for DATA_B\n");
    iounmap(data_a_mem);
    device_destroy(dataBusClass, MKDEV(majorNumber, 0));
    class_destroy(dataBusClass);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    return -EIO;
  }

  return 0;
}

static void __exit data_bus_exit(void) {
  iounmap(data_a_mem);
  iounmap(data_b_mem);
  device_destroy(dataBusClass, MKDEV(majorNumber, 0));
  class_destroy(dataBusClass);
  unregister_chrdev(majorNumber, DEVICE_NAME);
  printk(KERN_INFO "DataBus Finalized!\n");
}

static ssize_t data_bus_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
  uint32_t data_a;
  uint32_t data_b;
  uint64_t data;

  data_a = ioread32(data_a_mem);
  data_b = ioread32(data_b_mem);

  data = ((uint64_t)data_b << 32) | data_a;

  if (copy_to_user(buffer, &data, sizeof(data))) {
    return -EFAULT;
  }

  return sizeof(data);
}

static ssize_t data_bus_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
  uint64_t data;
  uint32_t data_a;
  uint32_t data_b;

  if (copy_from_user(&data, buffer, sizeof(data))) {
    return -EFAULT;
  }

  data_a = (uint32_t)(data & 0xFFFFFFFF);
  data_b = (uint32_t)(data >> 32);

  iowrite32(data_a, data_a_mem);
  iowrite32(data_b, data_b_mem);

  return sizeof(data);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Driver for the GPP Bus DATA_A and DATA_B");
MODULE_VERSION("0.1");