#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

static char *int_str;

/* [X1: point 1]
 Module's metadata information.
 These arent mandatory, but useful for documentation and ownership.
 MODULE_DESCRIPTION: Synopsis of the module's functionality
 MODULE_AUTHOR: Author's Name 
 MODULE_LICENCE: License for the program, GPL is used here.
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Purushothaman Palani");
MODULE_DESCRIPTION("LKP Exercise 3");

/* [X2: point 1]
 command line arguments for kernel modules. 
 module_param macro takes 3 arguments. name of the variable, its type, and permissions for the corresponding file in sysfs.
 here, we are specifying variable int_str of char pointer type with 
 S_IRUSR - Read permission bit for owner of the file
 S_IRGRP - Read permission bit for group owner of the file
 S_IROTH - Read permission bit for other users
 */
module_param(int_str, charp, S_IRUSR | S_IRGRP | S_IROTH);

/* [X3: point 1]
MODULE_PARM_DESC is a macro used to document the arguments a module can take.
It has two arguments variable name and a string about its purpose. 
The docstring mentions int_str is a comma separated list of integers
 */
MODULE_PARM_DESC(int_str, "A comma-separated list of integers");

/* [X4: point 1]
LIST_HEAD is a macro for initializing the struct list_head(in include/linux/list.h) as a static variable.
the name of the list_head structure is 'mylist' 
 */
static LIST_HEAD(mylist);

/* [X5: point 1]
 Defining a struct named entry with 'val' of type int and 'list' of type list_head(struct)
 */
struct entry {
	int val;
	struct list_head list;
};

static int store_value(int val)
{
	/* [X6: point 10]
	 * Allocate a struct entry of which val is val
	 * and add it to the tail of mylist.
	 * Return 0 if everything is successful.
	 * Otherwise (e.g., memory allocation failure),
	 * return corresponding error code in error.h (e.g., -ENOMEM).
	 */
	struct entry *new_val = kmalloc(sizeof(*new_val), GFP_KERNEL);
	if (!new_val){
		return -ENOMEM;
	}
	else{
		new_val -> val = val;
		INIT_LIST_HEAD(&new_val -> list);
		list_add_tail(&new_val -> list, &mylist);
		printk(KERN_INFO"Added element: %d\n", val);
		return 0;
	}
}

static void test_linked_list(void)
{
	/* [X7: point 10]
	 * Print out value of all entries in mylist.
	 */
	struct entry *print_all_entries;
	list_for_each_entry(print_all_entries, &mylist, list){
		printk(KERN_INFO "List Element: %d\n", print_all_entries->val);
	}
}


static void destroy_linked_list_and_free(void)
{
	/* [X8: point 10]
	 * Free all entries in mylist.
	 */
	 struct entry *delete_entry, *next;
	list_for_each_entry_safe(delete_entry, next, &mylist, list){
		printk(KERN_INFO "Deleting %d\n", delete_entry->val);
		list_del(&delete_entry->val);
		kfree(delete_entry);
	}
}


static int parse_params(void)
{
	int val, err = 0;
	char *p, *orig, *params;


	/* [X9: point 1]
	the kstrdup function allocates space and copies existing string. String duplication function
	in this case, it copies the 'int_str' string with GFP_KERNEL flag. 
	GFP - get free page flag, specifies which memory zones can be used, how hard the allocator should
	try to find free memory, whether it can be accessed by userspace etc.
	'params' is the pointer to the newly copied string. If unallocated, returns NULL,
	and the program exits with ENOMEM, meaning out of memory.
	 */
	params = kstrdup(int_str, GFP_KERNEL);
	if (!params)
		return -ENOMEM;
	orig = params;

	/* [X10: point 1]
	strsep function splits the string into tokens based upon the chatracter to split by. 
	takes in string 'params' and separator ',' returns split character string to char pointer p
	This process repeats till the last ',' 
	 */
	while ((p = strsep(&params, ",")) != NULL) {
		if (!*p)
			continue;
		/* [X11: point 1]
		kstrtoint function converts 'p' string into integer and store it to pointer 'val'.
		the '0' is for the base argument, which automatically detects the string format as bin/hex/oct or decimal 
		 */
		err = kstrtoint(p, 0, &val);
		if (err)
			break;

		/* [X12: point 1]
		creates a node and allots memory for a new element to be inserted at the tail of the linked list.
		if storing returns a non-zero value, meaning that insertion failed, then it breaks out of the loop. 
		 */
		err = store_value(val);
		if (err)
			break;
	}

	/* [X13: point 1]
	 Remove 'orig' variable and free up the memory used by it.
	 */
	kfree(orig);
	return err;
}

static void run_tests(void)
{
	/* [X14: point 1]
	Invoke the test_linked_list function to display the elements of the linked list.
	 */
	test_linked_list();
}

static void cleanup(void)
{
	/* [X15: point 1]
	Print the string as a kernel info message;
	Delete the linked list from scope and free-up the memory 
	*/
	printk(KERN_INFO "\nCleaning up...\n");

	destroy_linked_list_and_free();
}

static int __init ex3_init(void)
{
	int err = 0;

	/* [X16: point 1]
	Checks if int_str argument/parameter is missing. If missing, prints the string as kernel info message
	 */
	if (!int_str) {
		printk(KERN_INFO "Missing \'int_str\' parameter, exiting\n");
		return -1;
	}

	/* [X17: point 1]
	Invoke the previously defined parsing function and checks for non-zero return value. 
	If successful, then executes the run_tests function. else exits to the cleanup and returns non-zero value.
	 */
	err = parse_params();
	if (err)
		goto out;

	/* [X18: point 1]
	Invokes the run_tests function and displays the contents of the linked list.
	 */
	run_tests();
out:
	/* [X19: point 1]
	Invokes the cleanup function which removes the variables from scope and frees the used memory.
	*/
	cleanup();
	return err;
}

static void __exit ex3_exit(void)
{
	/* [X20: point 1]
	the __exit macro in includes/linux/init.h used to hint the kernel that the ex3_exit function marks
	the end of the code and kernel optimizes memory usage by clearing function/variable memory usage. 
	Just a wrapper to end the program.
	 */
	return;
}

/* [X21: point 1]
	module_init will be called during insertion time(insmod) and there is only one module_init per module.
	here the entry point is ex3_init function.
*/
module_init(ex3_init);

/* [X22: point 1]
	module_exit will clean up the module's memory usage and remove variables from scope and there is only one 
	module_exit per module. here it invokes the ex3_exit function.
 */
module_exit(ex3_exit);
