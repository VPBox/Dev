/*
 * include/linux/drv_namespace.h
 *
 *
 * Driver namespaces:
 *
 * The driver namespace allows a device driver to register itself and pass a
 * pointer to its driver specific namespace structure and register notifiers
 * which are called when the active namepace becomes inactive and when an
 * inactive namespace becomes active.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GPL 2 license for more details.
 * The full GPL 2 License is included in this distribution in the file called
 * COPYING
 */
#include <linux/module.h>
#include <linux/err.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/pid_namespace.h>
#include <linux/drv_namespace.h>
#include <linux/kernel.h>
#include <linux/atomic.h>
#include <linux/uaccess.h>
#include "../fs/proc/internal.h"

/* protects active namespace and switches */
static DECLARE_RWSEM(global_drv_ns_lock);

static void drv_ns_lock(struct drv_namespace *drv_ns)
{
	mutex_lock(&drv_ns->mutex);
}

static void drv_ns_unlock(struct drv_namespace *drv_ns)
{
	mutex_unlock(&drv_ns->mutex);
}

static struct drv_namespace *create_drv_ns(struct task_struct *task,
					   struct pid_namespace *new_pidns)
{
	struct drv_namespace *drv_ns;

	static int s_ndev;

	drv_ns = kzalloc(sizeof(struct drv_namespace), GFP_KERNEL);
	if (!drv_ns)
		return ERR_PTR(-ENOMEM);

	atomic_set(&drv_ns->count, 1);
	BLOCKING_INIT_NOTIFIER_HEAD(&drv_ns->notifiers);
	mutex_init(&drv_ns->mutex);
	/* all new namespaces get a default tag */
	snprintf(drv_ns->tag, DRV_NS_TAG_LEN, "drv_ns.%d", ++s_ndev);
	drv_ns->tag[DRV_NS_TAG_LEN-1] = '\0';

	drv_ns->pid_ns = get_pid_ns(new_pidns);

	return drv_ns;
}

struct drv_namespace *copy_drv_ns(unsigned long flags,
				  struct task_struct *task,
				  struct pid_namespace *new_pidns)
{
	struct drv_namespace *drv_ns = task->nsproxy->drv_ns;

	/*
	 * Couple driver namespace semantics with pid-namespace.
	 * It's convenient, and we ran out of clone flags anyway.
	 */
	if (!(flags & CLONE_NEWPID))
		return get_drv_ns(drv_ns);
	else
		return create_drv_ns(task, new_pidns);
}

void __put_drv_ns(struct drv_namespace *drv_ns)
{
	int n;

	if (!drv_ns || drv_ns == &init_drv_ns)
		return;

	for (n = 0; n < DRV_NS_DESC_MAX; n++) {
		if (drv_ns->info[n])
			put_drv_ns_info(n, drv_ns->info[n], 0);
	}

	put_pid_ns(drv_ns->pid_ns);
	kfree(drv_ns);
}

struct drv_namespace *get_drv_ns_by_task(struct task_struct *task)
{
	struct drv_namespace *drv_ns = NULL;
	struct nsproxy *nsproxy;

	rcu_read_lock();
	nsproxy = task_nsproxy(task);
	if (nsproxy)
		drv_ns = get_drv_ns(nsproxy->drv_ns);
	rcu_read_unlock();

	return drv_ns;
}

struct drv_namespace *get_drv_ns_by_vpid(pid_t vpid)
{
	struct drv_namespace *drv_ns = NULL;
	struct task_struct *task;
	struct nsproxy *nsproxy;

	rcu_read_lock();
	task = find_task_by_pid_ns(vpid, &init_pid_ns);
	if (task) {
		nsproxy = task_nsproxy(task);
		if (nsproxy)
			drv_ns = get_drv_ns(nsproxy->drv_ns);
	}
	rcu_read_unlock();

	return drv_ns;
}

/**
 * notifications: activate/deactive driver namespace
 */
static BLOCKING_NOTIFIER_HEAD(drv_ns_notifiers);
void drv_ns_register_notify(struct drv_namespace *drv_ns,
			    struct notifier_block *nb)
{
	if (drv_ns != NULL)
		blocking_notifier_chain_register(&drv_ns->notifiers, nb);
	else
		blocking_notifier_chain_register(&drv_ns_notifiers, nb);
}

void drv_ns_unregister_notify(struct drv_namespace *drv_ns,
			      struct notifier_block *nb)
{
	if (drv_ns != NULL)
		blocking_notifier_chain_unregister(&drv_ns->notifiers, nb);
	else
		blocking_notifier_chain_unregister(&drv_ns_notifiers, nb);
}

/*
 * Helpers for per-driver logic of driver-namepace
 *
 * Drivers should embed 'struct drv_ns_info' in driver-specific,
 * per-driver-namespace data, e.g.:
 *
 *   struct xxxx_namespace {
 *     ... (data specific to xxxx)
 *     struct drv_ns_info drvns_info;
 *   };
 *
 * Drivers should register a 'struct drv_ns_ops' with ->create()
 * and ->release() methods, and keep an identifier (drv_ns_xxx_id),
 * for use by driver namespace generic code
 *
 * Drivers can iterate over per-driver data in all namespaces:
 *   void loop_drv_ns_info(int drv_ns_id, void *ptr,
 *              void (*func)(struct drv_ns_info *drv_ns_info, void *ptr))
 *
 * See include/linux/drv_namespace.h for helper macros to hide these details.
 */

struct drv_ns_desc {
	char *name;
	struct drv_ns_ops *ops;
	struct mutex mutex;
	struct list_head head;
};

static struct drv_ns_desc drv_ns_desc[DRV_NS_DESC_MAX];
static DEFINE_SPINLOCK(drv_ns_desc_lock);

int register_drv_ns_ops(char *name, struct drv_ns_ops *ops)
{
	struct drv_ns_desc *desc;
	int n, ret = -ENOMEM;

	if (!name)
		return -EINVAL;

	spin_lock(&drv_ns_desc_lock);
	for (n = 0; n < DRV_NS_DESC_MAX; n++) {
		desc = &drv_ns_desc[n];
		if (!desc->name && ret < 0)
			ret = n;
		else if (desc->name && !strcmp(desc->name, name)) {
			ret = -EBUSY;
			break;
		}
	}
	if (ret >= 0) {
		pr_info("drv_ns: register info %s\n", name);
		desc = &drv_ns_desc[ret];
		desc->name = name;
		desc->ops = ops;
		INIT_LIST_HEAD(&desc->head);
		mutex_init(&desc->mutex);
	}
	spin_unlock(&drv_ns_desc_lock);

	return ret;
}

void unregister_drv_ns_ops(int drv_ns_id)
{
	struct drv_ns_desc *desc = &drv_ns_desc[drv_ns_id];

	spin_lock(&drv_ns_desc_lock);
	pr_info("drv_ns: unregister desc %s\n", desc->name);
	memset(&drv_ns_desc[drv_ns_id], 0, sizeof(*desc));
	spin_unlock(&drv_ns_desc_lock);
}

/* this function is called with drv_ns_lock(drv_ns) held */
static struct drv_ns_info *new_drv_ns_info(int drv_ns_id,
					   struct drv_namespace *drv_ns)
{
	struct drv_ns_desc *desc = &drv_ns_desc[drv_ns_id];
	struct drv_ns_info *drv_ns_info;

	pr_debug("drv_ns: [0x%p] new info %s\n", drv_ns, desc->name);

	drv_ns_info = desc->ops->create(drv_ns);
	if (IS_ERR_OR_NULL(drv_ns_info))
		return NULL;

	pr_debug("drv_ns: [0x%p] got info 0x%p\n", drv_ns, drv_ns_info);

	drv_ns->info[drv_ns_id] = drv_ns_info;
	/* take a reference for our drv_ns_info array */
	atomic_set(&drv_ns_info->count, 1);
	/*
	 * don't take a reference here: we're contained by the drv_namespace
	 * structure, and an extra reference to that structure would create a
	 * circular dependecy resulting in memory that can never be free'd.
	 */
	drv_ns_info->drv_ns = drv_ns;

	mutex_lock(&desc->mutex);
	list_add(&drv_ns_info->list, &desc->head);
	mutex_unlock(&desc->mutex);

	return drv_ns_info;
}

/* this function is called with drv_ns_lock(drv_ns) held */
static void del_drv_ns_info(int drv_ns_id, struct drv_ns_info *drv_ns_info)
{
	struct drv_ns_desc *desc = &drv_ns_desc[drv_ns_id];
	struct drv_namespace *drv_ns = drv_ns_info->drv_ns;

	pr_debug("drv_ns: [0x%p] destory info 0x%p\n", drv_ns, drv_ns_info);

	drv_ns->info[drv_ns_id] = NULL;
	mutex_lock(&desc->mutex);
	list_del(&drv_ns_info->list);
	mutex_unlock(&desc->mutex);

	drv_ns_desc[drv_ns_id].ops->release(drv_ns_info);
}

/*
 * get_drv_ns_info() is intended for internal use only. It is exported only
 * to enable the helper macros in drv_namepsace.h to work properly.
 *
 * @create tells whether to create a new instance if none is found already,
 * or just return NULL.
 *
 * @lock tells whether the @drv_ns should be locked against concurrent
 * changes, or the caller is the one responsible (in which case there is
 * not even a need for an extra refefence count).
 */
struct drv_ns_info *get_drv_ns_info(int drv_ns_id,
				    struct drv_namespace *drv_ns,
				    bool lock, bool create)
{
	struct drv_ns_info *drv_ns_info;

	if (lock) {
		down_read(&global_drv_ns_lock);
		drv_ns_lock(drv_ns);
	}

	drv_ns_info = drv_ns->info[drv_ns_id];

	if (!drv_ns_info && create)
		drv_ns_info = new_drv_ns_info(drv_ns_id, drv_ns);

	if (drv_ns_info) {
		pr_debug("drv_ns: [0x%p] get info 0x%p count %d+\n", drv_ns,
			 drv_ns_info, atomic_read(&drv_ns_info->count));
	}

	if (drv_ns_info && lock)
		atomic_inc(&drv_ns_info->count);

	if (lock) {
		drv_ns_unlock(drv_ns);
		up_read(&global_drv_ns_lock);
	}

	return drv_ns_info;
}

struct drv_ns_info *get_drv_ns_info_task(int drv_ns_id, struct task_struct *tsk)
{
	struct drv_ns_info *drv_ns_info = NULL;
	struct drv_namespace *drv_ns;

	drv_ns = get_drv_ns_by_task(tsk);
	if (drv_ns) {
		drv_ns_info = get_drv_ns_info(drv_ns_id, drv_ns, 1, 1);
		put_drv_ns(drv_ns);
	}

	return drv_ns_info;
}

void put_drv_ns_info(int drv_ns_id, struct drv_ns_info *drv_ns_info, int lock)
{
	struct drv_namespace *drv_ns = drv_ns_info->drv_ns;

	if (lock) {
		down_read(&global_drv_ns_lock);
		drv_ns_lock(drv_ns);
	}

	pr_debug("drv_ns: [0x%p] put info 0x%p count %d-\n", drv_ns,
		 drv_ns_info, atomic_read(&drv_ns_info->count));
	if (atomic_dec_and_test(&drv_ns_info->count))
		del_drv_ns_info(drv_ns_id, drv_ns_info);

	if (lock) {
		drv_ns_unlock(drv_ns);
		up_read(&global_drv_ns_lock);
	}
}

/*
 * @drv_ns_id: id of driver namespace subsystem
 * @ptr: data pointer to be passed to callback
 * @func: callback for each device namespace (atomic, must not sleep)
 */
void loop_drv_ns_info(int drv_ns_id, void *ptr,
		      void (*func)(struct drv_ns_info *drv_ns_info, void *ptr))
{
	struct drv_ns_desc *desc = &drv_ns_desc[drv_ns_id];
	struct drv_ns_info *drv_ns_info;

	mutex_lock(&desc->mutex);
	list_for_each_entry(drv_ns_info, &desc->head, list) {
		pr_debug("drv_ns: loop info 0x%p (drv_ns 0x%p) of %s\n",
			 drv_ns_info, drv_ns_info->drv_ns, desc->name);
		(*func)(drv_ns_info, ptr);
	}
	mutex_unlock(&desc->mutex);
}

/**
 * Set the active driver namespace (will call registered notifiers to
 * allow device drivers to make device specific context store/restore)
 *
 * @dev_ns: The new active device namespace
 */
void set_active_drv_ns(struct drv_namespace *next_ns)
{
	struct drv_namespace *prev_ns;

	BUG_ON(next_ns == NULL);

	down_write(&global_drv_ns_lock);

	pr_info("drv_ns: next_ns 0x%p\n", next_ns);
	pr_info("drv_ns: active_drv_ns 0x%p\n", active_drv_ns);

	if (next_ns == active_drv_ns)
		goto done;

	prev_ns = active_drv_ns;

	/*
	 * deactivate previous drv_ns:
	 * - set active-state of previous drv_ns to false
	 * - call previous drv_ns's notifiers with deactivate event
	 * - call global notifiers with deactivate event
	 */

	drv_ns_lock(prev_ns);

	prev_ns->active = false;
	prev_ns->timestamp = jiffies;

	(void) blocking_notifier_call_chain(&prev_ns->notifiers,
					    DRV_NS_EVENT_DEACTIVATE, prev_ns);
	(void) blocking_notifier_call_chain(&drv_ns_notifiers,
					    DRV_NS_EVENT_DEACTIVATE, prev_ns);

	drv_ns_unlock(prev_ns);

	/*
	 * activate next drv_ns:
	 * - set active-state of next drv_ns to true
	 * - call next drv_ns's notifiers with activate event
	 * - call global notifiers with activate event
	 */

	drv_ns_lock(next_ns);

	next_ns->active = true;
	next_ns->timestamp = jiffies;

	/* make the switch */
	active_drv_ns = next_ns;

	(void) blocking_notifier_call_chain(&next_ns->notifiers,
					    DRV_NS_EVENT_ACTIVATE, next_ns);
	(void) blocking_notifier_call_chain(&drv_ns_notifiers,
					    DRV_NS_EVENT_ACTIVATE, next_ns);

	drv_ns_unlock(next_ns);

	get_drv_ns(next_ns);
	put_drv_ns(prev_ns);

	pr_info("dev_ns: activate 0x%p done\n", active_drv_ns);
 done:
	up_write(&global_drv_ns_lock);
}

/**
 * Setup for /proc/drv_ns
 */

static struct proc_dir_entry *proc_drv_ns_dir;

struct proc_dir_entry *
create_drv_ns_proc(const char *name, const struct file_operations *fops)
{
	struct proc_dir_entry *entry;
	entry = proc_create(name, 0, proc_drv_ns_dir, fops);
	return entry;
}

static int proc_active_ns_show(struct seq_file *seq, void *offset)
{
	down_read(&global_drv_ns_lock);
	seq_printf(seq, "%d\n", drv_ns_init_pid(active_drv_ns));
	up_read(&global_drv_ns_lock);
	return 0;
}

static int proc_ns_tag_show(struct seq_file *seq, void *offset)
{
	down_read(&global_drv_ns_lock);
	seq_printf(seq, "active: %d timestamp: %ld tag: %s\n",
		   drv_ns_init_pid(active_drv_ns),
		   active_drv_ns->timestamp,
		   active_drv_ns->tag);
	up_read(&global_drv_ns_lock);
	return 0;
}

static bool drv_ns_proc_permission(void)
{
	return current_drv_ns() == &init_drv_ns;
}

static int proc_active_ns_open(struct inode *inode, struct file *file)
{
	if (!drv_ns_proc_permission())
		return -EPERM;
	return single_open(file, proc_active_ns_show, PDE(inode)->data);
}

static int proc_ns_tag_open(struct inode *inode, struct file *file)
{
	//if (!drv_ns_proc_permission())
	//	return -EPERM;
	return single_open(file, proc_ns_tag_show, PDE(inode)->data);
}

static ssize_t proc_active_ns_write(struct file *file,
				    const char __user *buffer,
				    size_t count, loff_t *ppos)
{
	struct drv_namespace *drv_ns;
	char spid[64] = {0};
	pid_t pid = 0;

	/* only init ns may change active ns */
	if (!drv_ns_proc_permission())
		return -EPERM;

	if (count >= sizeof(spid) || count == 0)
		return -EFAULT;

	if (copy_from_user(spid, buffer, count))
		return -EFAULT;

	if (kstrtoint(spid, 10, &pid) || !pid) {
		pr_warn("drv_ns: bad PID format '%s'\n", spid);
		return -EINVAL;
	}

	drv_ns = get_drv_ns_by_vpid(pid);
	if (!drv_ns) {
		pr_warn("drv_ns: non-existing PID %d\n", pid);
		return -EINVAL;
	}

	set_active_drv_ns(drv_ns);

	put_drv_ns(drv_ns);
	return count;
}

static ssize_t proc_ns_tag_write(struct file *file,
				 const char __user *buffer,
				 size_t count, loff_t *ppos)
{
	char name[DRV_NS_TAG_LEN+1] = {0};

	if (count >= sizeof(name) || count == 0)
		return -EFAULT;

	if (copy_from_user(name, buffer, count))
		return -EFAULT;

	memset(current_drv_ns()->tag, 0, DRV_NS_TAG_LEN+1);
	strcpy(current_drv_ns()->tag, name);
	pr_info("drv_ns: tag = %s \n", current_drv_ns()->tag);
	return count;
}

static const struct file_operations active_ns_fileops = {
	.owner = THIS_MODULE,
	.open = proc_active_ns_open,
	.read = seq_read,
	.write = proc_active_ns_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static const struct file_operations ns_tag_fileops = {
	.owner = THIS_MODULE,
	.open = proc_ns_tag_open,
	.read = seq_read,
	.write = proc_ns_tag_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static __init int drv_namespace_init(void)
{
	struct proc_dir_entry *entry;

	proc_drv_ns_dir = proc_mkdir("drv_ns", NULL);
	if (!proc_drv_ns_dir) {
		pr_err("[drv_namespace_init] proc_mkdir fail!\n");
		return -ENOMEM;
	}

	entry = proc_create("active_ns_pid", 0644, proc_drv_ns_dir,
			    &active_ns_fileops);
	if (!entry) {
		pr_err("[drv_namespace_init] proc_create active_ns_pid fail!\n");
		goto out_fail_active_ns;
	}

	entry = proc_create("ns_tag", 0644, proc_drv_ns_dir,
			    &ns_tag_fileops);
	if (!entry) {
		pr_err("[drv_namespace_init] proc_create ns_tag fail!\n");
		goto out_fail_ns_tag;
	}

	pr_info("[drv_namespace_init] success exit...\n");

	return 0;

out_fail_ns_tag:
	remove_proc_entry("active_ns_pid", proc_drv_ns_dir);
out_fail_active_ns:
	remove_proc_entry("drv_ns", NULL);
	return -ENOMEM;
}

device_initcall(drv_namespace_init);
