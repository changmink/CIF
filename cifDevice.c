#include <linux/config.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>     /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/mm.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>

#include<cif.h>
#define CIF_NAME		"cif"

/** Global Variables **/
static int cif_major = 0;
static unsigned long *cif_addr;

MODULE_AUTHOR("speark(speark0438@gmail.com)");
MODULE_LICENSE("Dual BSD/GPL");

static struct file_operations cif_fops = {
	.owner 	= THIS_MODULE,
   .open 	= cif_open,
   .release = cif_release,
   //.write = cif_write,
	//.read 	= cif_read,
	.ioctl 	= cif_ioctl
};

/** Open the device */
static int cif_open(struct inode *inode, struct file *filp){
	cif_init_camera();
     return 0;
}

/** Closing the device */
static int cif_release(struct inode *inode, struct file *filp){
     return 0;
}

/** ioctl function */
int cif_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg){
	unsigned short x,y,sx,sy;
	switch(cmd){
		case CIF_CAPTURE :
				cif_capture();
			break;
		case CIF_CROP :
				sx = (arg & 0xff0000) >> 32;
				sy = (arg & 0xff00000) >> 48;
				x = arg & 0xff;
				y = (arg&0xff00) >> 16;
				cif_crop(sx,sy,x,y);
			break;
		case CIF_SET_SIZE :
			x = arg&0x00ff;
			y = (arg&0xff00) >> 16;
			cif_crop(0,0,x,y);
			break;
		case 
	}
	return cmd; 
}
static void cif_setup_cdev(struct cdev *dev, int minor, struct file_operations *fops){
     int err, devno = MKDEV(cif_major,minor);

     cdev_init(dev, fops);
     dev->owner = THIS_MODULE;
     dev->ops = fops;
     err = cdev_add(dev, devno, 1);

     if (err)
          printk(KERN_WARNING "Error %d adding CIFs%d", err, minor);
}
static struct cdev CIFDevs[1];//커널에 디바이스 드라이버의 동록 정보를 저장할 구조체 
/** Module housekeeping.*/
static int cif_init(void){
     int result;
     dev_t dev = MKDEV(cif_major,0);
     
     /* Figure out our device number. */
     if (cif_major)
          result = register_chrdev_region(dev, 1, cif_NAME);//정적할당
     else {
          result = alloc_chrdev_region(&dev, 0, 1, cif_NAME);//동적할당
			 cif_major = MAJOR(dev);
	     }
     if (result < 0) {
          printk(KERN_WARNING "%s: unable to get major %d\n", CIF_NAME, cif_major);
          return result;
              }
     if (cif_major == 0)
          cif_major = result;

     /* Now set up two cdevs. */
     cif_setup_cdev(CIFDevs,0,&cif_fops);

     /* Allocate I/O Memory */
     cif_addr = ioremap(CIF_ADDRESS,0x1000);

     if (!cif_addr){
          printk(KERN_WARNING "%s:memory allocation failed.\n", CIF_NAME);
          iounmap(cif_addr);
          return -EIO;
     }

     return 0;
}

static void cif_cleanup(void){
	cdev_del(CIFDevs);
	iounmap(cif_addr);    
	unregister_chrdev_region(MKDEV(cif_major,0),1);

}
module_init(cif_init);
module_exit(cif_cleanup);
