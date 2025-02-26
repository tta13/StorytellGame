#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define G_LEDS         3
#define BUTTONS        6

static uint32_t info = 0;
static uint32_t old_info_b = 0;
static uint32_t new = 0;

static void *green_leds;
static void *buttons;

static int   access_count =  0;
static int   MAJOR_NUMBER = 91;

static int     char_device_open    ( struct inode * , struct file *);
static int     char_device_release ( struct inode * , struct file *);
static ssize_t char_device_read    ( struct file * , char *,       size_t , loff_t *);
static ssize_t char_device_write   ( struct file * , const char *, size_t , loff_t *);

static struct file_operations file_opts = {
      .read = char_device_read,
      .open = char_device_open,
     .write = char_device_write,
   .release = char_device_release
};

static int char_device_open(struct inode *inodep, struct file *filep) {
   access_count++;
   printk(KERN_ALERT "altera_driver: opened %d time(s)\n", access_count);
   return 0;
}

static int char_device_release(struct inode *inodep, struct file *filep) {
   printk(KERN_ALERT "altera_driver: device closed.\n");
   return 0;
}

static ssize_t char_device_read(struct file *filep, char *buf, size_t driver, loff_t *off) {
  	info = 0;
  	switch(driver){
 	case BUTTONS:
      info = ioread32(buttons);
      if(info != old_info_b){
        new = 1;
      }
      old_info_b = info;
      break;
    default:
      printk(KERN_ALERT "Invalid Option from Read().\n");
      return -1;
	}

	if(new == 1){
    copy_to_user(buf, &info, sizeof(uint32_t));
    printk("Info lida: %d\n", info);
    new = 0; 
    return 4;
  } else{
    printk("Nao Info lida: %d\n", info);
    return 0;
  }
}

static ssize_t char_device_write(struct file *filep, const char *buf, size_t driver, loff_t *off) {
  	info = 0;
  	copy_from_user(&info, buf, sizeof(uint32_t));
  
  	switch(driver){
	case G_LEDS:
      iowrite32(info, green_leds);
      break;
 	default:
      printk(KERN_ALERT "Invalid Option from Read().\n");
      return -1;
  	}
  	printk(KERN_ALERT "WROTE: %d", info);
  	return 4;
}

//-- PCI Device Interface

static struct pci_device_id pci_ids[] = {
  { PCI_DEVICE(0x1172, 0x0004), },
  { 0, }
};
MODULE_DEVICE_TABLE(pci, pci_ids);

static int pci_probe(struct pci_dev *dev, const struct pci_device_id *id);
static void pci_remove(struct pci_dev *dev);

static struct pci_driver pci_driver = {
  .name     = "alterahello",
  .id_table = pci_ids,
  .probe    = pci_probe,
  .remove   = pci_remove,
};

static unsigned char pci_get_revision(struct pci_dev *dev) {
  u8 revision;

  pci_read_config_byte(dev, PCI_REVISION_ID, &revision);
  return revision;
}

static int pci_probe(struct pci_dev *dev, const struct pci_device_id *id) {
  int vendor;
  int retval;
  unsigned long resource;

  retval = pci_enable_device(dev);
  
  if (pci_get_revision(dev) != 0x01) {
    printk(KERN_ALERT "altera_driver: cannot find pci device\n");
    return -ENODEV;
  }

  pci_read_config_dword(dev, 0, &vendor);
  printk(KERN_ALERT "altera_driver: Found Vendor id: %x\n", vendor);

  resource = pci_resource_start(dev, 0);
  printk(KERN_ALERT "altera_driver: Resource start at bar 0: %lx\n", resource);

 green_leds = ioremap_nocache(resource + 0XC080, 0x20);
  buttons = ioremap_nocache(resource + 0XC100, 0x20);
 return 0;
}

static void pci_remove(struct pci_dev *dev) {
iounmap(green_leds);
 iounmap(buttons);
}


//-- Global module registration

static int __init altera_driver_init(void) {
   int t = register_chrdev(MAJOR_NUMBER, "de2i150_altera", &file_opts);
   t = t | pci_register_driver(&pci_driver);

   if(t<0)
      printk(KERN_ALERT "altera_driver: error: cannot register char or pci.\n");
   else
     printk(KERN_ALERT "altera_driver: char+pci drivers registered.\n");

   return t;
}

static void __exit altera_driver_exit(void) {
  printk(KERN_ALERT "Goodbye from de2i150_altera.\n");

  unregister_chrdev(MAJOR_NUMBER, "de2i150_altera");
  pci_unregister_driver(&pci_driver);
}

module_init(altera_driver_init);
module_exit(altera_driver_exit);

