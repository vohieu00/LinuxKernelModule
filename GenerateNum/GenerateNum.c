//https://tldp.org/LDP/lkmpg/2.6/lkmpg.pdf
//http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/

#include <linux/types.h>
#include <linux/init.h> //for __init and __exit
#include <linux/module.h> //loading LKM into kernel
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>

#define DEVICE_NAME	"generateDevice"
#define CLASS_NAME	"generate"
#define SIZE	4

static int major_num; //device number 
static char generated_num; //Number that module generate
//static int number_opens = 0; //Number of time that devide is opened
static struct class* generate_class = NULL;
static struct device* generate_device = NULL;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);


struct file_operations fops = 
{
	.read = device_read,
	.open = device_open,
	.release = device_release
};


static int device_open(struct inode *inode, struct file *file)
{
	//printk("Device open.\n");
	try_module_get(THIS_MODULE);
	return 0;
}
static int device_release(struct inode *inode, struct file *file)
{
	printk("Device release.\n");
	//number_opens--;
	module_put(THIS_MODULE);
	printk(KERN_INFO "Device sucessfully closed\n");
	return 0;	
}
static ssize_t device_read(struct file *pfile, char *buff, size_t len, loff_t *offset)
{
	int error_count = 0;
	//printk("Device read.\n");
	get_random_bytes(&generated_num, sizeof(char));
	error_count = copy_to_user(buff, &generated_num, sizeof(char));
	if (error_count == 0)
	{
		printk(KERN_INFO "Sent a number to user.\n");
	}
	else 
	{
		printk(KERN_INFO " Faild to send a number to user.\n");
		return -EFAULT;
	}
	return sizeof(char);
}

static int __init initGenerateDevice(void)
{
	//System provides a major number
	major_num = register_chrdev(0, DEVICE_NAME, &fops);
	
	if (major_num < 0)
	{
		printk(KERN_ALERT "Generate failed.\n");
		return major_num;
	}
	generate_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(generate_class))
	{
		unregister_chrdev(major_num, DEVICE_NAME);
		printk(KERN_ALERT "Fail to register device class.\n");
		return PTR_ERR(generate_class);
	}
	//printk("-------------%d", major_num); 
	generate_device = device_create(generate_class, NULL, MKDEV(major_num, 0), NULL, DEVICE_NAME);
	if (IS_ERR(generate_device)) 
	{
		class_destroy(generate_class);
		unregister_chrdev(major_num, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create the device.\n");
		return PTR_ERR(generate_device);
	}
	printk(KERN_INFO "Device class creation succeed");
	return 0;		
}
static void __exit exitGenrateDevice(void)
{
	device_destroy(generate_class, MKDEV(major_num, 0)); //remove device
	class_unregister(generate_class); //unregister device
	class_destroy(generate_class);    //remove class 
	unregister_chrdev(major_num, DEVICE_NAME); //unregister major number
	printk(KERN_INFO "Exit Generated Device\n");
}

module_init(initGenerateDevice);
module_exit(exitGenrateDevice);
MODULE_AUTHOR("Vo Thi Hieu - Tran Van Hieu - Nguyen Van Hiep");
MODULE_DESCRIPTION("A simple visualization for Linux char driver");
MODULE_VERSION("1.0");
MODULE_LICENSE ("GPL");
