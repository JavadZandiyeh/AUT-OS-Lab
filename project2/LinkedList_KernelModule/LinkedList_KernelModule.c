#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

/* implementation of list_head struct in types.h file :
 struct list_head {
     struct list_head *next, *prev;
 }; */

struct birthday{
    int day;
    int month;
    int year;

    /*list_head struct, put linked list among nodes of linked list
    list has prev and next pointer
    linux kernel list implementation*/
    struct list_head list;
};

/*this Macro makes birthday_list object as a pointer to first node of linked list
birthday_list type is struct list_head*/ 
static LIST_HEAD(birthday_list_head);

/* this function is called when the module is loaded */
int my_init(void){
    struct birthday *person1, *person2, *person3, *person4, *person5, *ptr1;
    /*kmalloc for kernel memory allocation
    GFP_KERNEL flag is for general purpose allocation of kernel memory*/ 
    person1 = kmalloc(sizeof(struct birthday), GFP_KERNEL);
    person1->day = 1;
    person1->month = 2;
    person1->year = 2000;
    /*this Macro initialize list struct of birthday struct*/ 
    INIT_LIST_HEAD(&person1->list);
    /*this Macro add person1 into linked list tail*/
    list_add_tail(&person1->list, &birthday_list_head);

    person2 = kmalloc(sizeof(struct birthday), GFP_KERNEL);
    person2->day = 2;
    person2->month = 3;
    person2->year = 2001;
    INIT_LIST_HEAD(&person2->list);
    list_add_tail(&person2->list, &birthday_list_head);

    person3 = kmalloc(sizeof(struct birthday), GFP_KERNEL);
    person3->day = 3;
    person3->month = 4;
    person3->year = 2002;
    INIT_LIST_HEAD(&person3->list);
    list_add_tail(&person3->list, &birthday_list_head);

    person4 = kmalloc(sizeof(struct birthday), GFP_KERNEL);
    person4->day = 4;
    person4->month = 5;
    person4->year = 2003;
    INIT_LIST_HEAD(&person4->list);
    list_add_tail(&person4->list, &birthday_list_head);

    person5 = kmalloc(sizeof(struct birthday), GFP_KERNEL);
    person5->day = 5;
    person5->month = 6;
    person5->year = 2004;
    INIT_LIST_HEAD(&person5->list);
    list_add_tail(&person5->list, &birthday_list_head);

    /*this Macro is uses for linked list iteration
    ptr : ‫‪on‬‬ ‫‪each‬‬ ‫‪iteration‬‬ ‫‪ptr‬‬ ‫‪points‬‬ ‫‪to‬‬ ‫‪the‬‬ ‫‪next‬‬ ‫‪birthday‬‬ ‫‪struct‬‬
    &birthday_list_head : pointer to list head of our linked list
    list : name of struct list_head variable in birthday struct*/
    list_for_each_entry(ptr1, &birthday_list_head, list){
        printk(KERN_INFO "day %d ,month %d ,year %d\n", ptr1->day, ptr1->month, ptr1->year);
    }
    return 0;
}

/* this function is called when the module is removed */
void my_exit(void){
    /*Go through the list and free the memory.*/
    struct birthday *ptr2, *temp;
    list_for_each_entry_safe(ptr2, temp, &birthday_list_head, list) {
        /*delete nodes of linked list*/
        list_del(&ptr2->list);
        /*free the memory which is allocated using kmalloc*/
        kfree(ptr2);
    }
    printk(KERN_INFO "Finished removing linked list\n");
}

/* Macros for registering module entry and exit points. */
module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LinkedList module");
MODULE_AUTHOR("MJZ");
