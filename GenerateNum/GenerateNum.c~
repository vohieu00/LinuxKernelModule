//https://tldp.org/LDP/lkmpg/2.6/lkmpg.pdf


#include <linux/init.h> //for __init and __exit
#include <linux/module.h> //loading LKM into kernel
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>
#define DEVICE_NAME	"generateDevice"
#define CLASS_NAME	"generate"
#define SIZE	5
static int major_num; //device number 
static char generated_num[SIZE]; //Number that module generate
static int number_opens = 0; //Number of time that devide is opened
static struct class* generate_class = NULL;

/*static int device_open(void);
static int device_read(char* buff, size_t length, loff_t *offset);
static int device_release(void);*/
//static ssize_t device_write(void);

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
//static ssize_t device_write(struct file *, const char *, size_t, loff_t *);


//static int device_open(void)
static int device_open(struct inode *inode, struct file *file)
{
 	//static int counter = 0;
	if (number_opens)
	{
		return -EBUSY;
	}
	number_opens++;
	printk(KERN_INFO "Device has been open %d time(s).\n", number_opens);
	try_module_get(THIS_MODULE);
	return number_opens;
}
static int device_release(struct inode * inode, struct file *file)
{
	number_opens--;
	module_put(THIS_MODULE);
	printk(KERN_INFO "Device sucessfully closed\n");
	return 0;	
}
static ssize_t device_read(struct file *pfile, char *buff, size_t len, loff_t *offset)
{
	int error_count = 0;
	get_random_bytes(generated_num, sizeof(int));
	error_count = copy_to_user(buff, generated_num, sizeof(int));
	if (error_count == 0)
	{
		printk(KERN_INFO "Sent a number to user.\n");
		return 0;
	}
	else
	{
		printk(KERN_INFO " Faild to send a number to user.\n");
		return -EFAULT;
	}
}
struct file_operations fops = 
{
	.read = device_read,
	//.write = device_write,
	.open = device_open,
	.release = device_release
};
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
