/*
 * include/linux/drv_namespace.h
 *
 *
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GPL 2 license for more details.
 * The full GPL 2 License is included in this distribution in the file called
 * COPYING
 */

#ifndef _LINUX_DRV_NS_H
#define _LINUX_DRV_NS_H

#include <linux/pid_namespace.h>
#include <linux/nsproxy.h>
#include <linux/notifier.h>
#include <linux/hardirq.h>
#include <linux/err.h>

#ifdef __KERNEL__

#define DRV_NS_TAG_LEN 64
#define DRV_NS_DESC_MAX 32
#define INIT_DRV_NS_IDX 0
#define MAX_CONTAINER	8

struct drv_namespace;
struct drv_ns_info;

struct drv_namespace {
	bool active;
	atomic_t count;
	struct pid_namespace *pid_ns;
	char tag[DRV_NS_TAG_LEN + 1];
	struct blocking_notifier_head notifiers;
	unsigned long timestamp; /* jiffies */

	struct mutex mutex;
	struct drv_ns_info *info[DRV_NS_DESC_MAX];
};

struct drv_ns_info {
	struct drv_namespace *drv_ns;
	struct list_head list;
	struct notifier_block nb;
	atomic_t count;
};

extern struct drv_namespace init_drv_ns;
extern struct net init_net;
extern struct drv_namespace *active_drv_ns;
#ifdef CONFIG_DRV_NS

struct drv_ns_ops {
	struct drv_ns_info* (*create)(struct drv_namespace *drv_ns);
	void (*release)(struct drv_ns_info *drv_ns_info);
};

/* driver namespace notifications */
#define DRV_NS_EVENT_ACTIVATE		0x1
#define DRV_NS_EVENT_DEACTIVATE		0x2

extern void __put_drv_ns(struct drv_namespace *drv_ns);
extern int get_drv_ns_idx(void);
static inline void put_drv_ns(struct drv_namespace *drv_ns)
{
	if (atomic_dec_and_test(&drv_ns->count))
		__put_drv_ns(drv_ns);
}

static inline struct drv_namespace *get_drv_ns(struct drv_namespace *drv_ns)
{
	atomic_inc(&drv_ns->count);
	return drv_ns;
}

/* return the driver namespaces of the current process */
static inline struct drv_namespace *current_drv_ns(void)
{
	if (in_interrupt())
		return &init_drv_ns;
	return current->nsproxy->drv_ns;
}

/* return the net namespaces of the current process */
static inline struct net *current_net_ns(void)
{
	if (in_interrupt())
		return &init_net;
	return current->nsproxy->net_ns;
}

/* return whether given driver namespace is active */
static inline bool is_active_drv_ns(struct drv_namespace *drv_ns)
{
	return drv_ns->active;
}

/* return and get the driver namespace of a given task */
extern struct drv_namespace *get_drv_ns_by_task(struct task_struct *task);
extern struct drv_namespace *get_drv_ns_by_vpid(pid_t vpid);

/*
 * set_active_drv_ns() will lock and unlock drv_namespace_lock
 * and call all registered activate and inactivate notifiers.
 */
extern void set_active_drv_ns(struct drv_namespace *drv_ns);

/* return root pid of the init process in a device namespace */
static inline pid_t drv_ns_init_pid(struct drv_namespace *drv_ns)
{
	return drv_ns->pid_ns->child_reaper->pid;
}

/* driver namespaces: notifiers (de)registration */
extern void drv_ns_register_notify(struct drv_namespace *drv_ns,
				   struct notifier_block *nb);
extern void drv_ns_unregister_notify(struct drv_namespace *drv_ns,
				     struct notifier_block *nb);

extern struct drv_namespace *copy_drv_ns(unsigned long flags,
					 struct task_struct *task,
					 struct pid_namespace *new_pidns);

/* helpers for per-driver logic of device namespace */

extern int register_drv_ns_ops(char *name, struct drv_ns_ops *ops);
extern void unregister_drv_ns_ops(int ns_id);
extern struct drv_ns_info *get_drv_ns_info(int ns_id,
					   struct drv_namespace *drv_ns,
					   bool lock, bool create);
extern struct drv_ns_info *get_drv_ns_info_task(int ns_id,
						struct task_struct *task);
extern void put_drv_ns_info(int ns_id,
			    struct drv_ns_info *drv_ns_info,
			    int lock);
extern void loop_drv_ns_info(int ns_id, void *ptr,
			     void (*func)(struct drv_ns_info *drv_ns_info,
					  void *ptr));

/* macro-mania to reduce repetitive code - not for the faint of heart */

#define i_to_x_drv_ns(i, x) container_of(i, struct x ## _drv_ns, drv_ns_info)

#define _drv_ns_id(X) \
	static int X ## _ns_id;

#define _drv_ns_get(X) \
	static inline \
	struct X ## _drv_ns *get_ ## X ## _ns(struct drv_namespace *drv_ns) \
	{ \
		struct drv_ns_info *info; \
		info = get_drv_ns_info(X ## _ns_id, drv_ns, 1, 1); \
		return info ? i_to_x_drv_ns(info, X) : NULL; \
	}

#define _drv_ns_find(X) \
	static inline \
	struct X ## _drv_ns *find_ ## X ## _ns(struct drv_namespace *drv_ns) \
	{ \
		struct drv_ns_info *info; \
		info = get_drv_ns_info(X ## _ns_id, drv_ns, 0, 0); \
		return info ? i_to_x_drv_ns(info, X) : NULL; \
	}


#define _drv_ns_get_cur(X) \
	static inline struct X ## _drv_ns *get_ ## X ## _ns_cur(void) \
	{ \
		struct drv_ns_info *info; \
		info = get_drv_ns_info_task(X ## _ns_id, current); \
		return info ? i_to_x_drv_ns(info, X) : NULL; \
	}

#define _drv_ns_put(X) \
	static inline void put_ ## X ## _ns(struct X ## _drv_ns *X ## _ns) \
	{ \
		put_drv_ns_info(X ## _ns_id, &X ## _ns->drv_ns_info, 1); \
	}

#define _drv_ns_active(X) \
	static inline bool \
	is_active_ ## X ## _drv_ns(struct X ## _drv_ns *ns) \
	{ \
		return is_active_drv_ns(ns->drv_ns_info.drv_ns); \
	}

#define _drv_nb_self(X) \
	static inline struct X ## _drv_ns * \
	get_ ## X ## _nb_self(struct notifier_block *self) \
	{ \
		struct drv_ns_info *drv_ns_info; \
		struct X ## _drv_ns *ns; \
		drv_ns_info = container_of(self, struct drv_ns_info, nb); \
		ns = container_of(drv_ns_info, \
				  struct X ## _drv_ns, drv_ns_info); \
		return ns; \
	}

/*
 * Finally, this is what a driver author really needs to use:
 * DEFINE_DRV_NS_INFO(X): X_ns_id, put_X_ns(), get_X_ns(), get_X_ns_cur()
 *                        is_active_X_ns(), get_X_nb_self()
 * DRV_NS_REGISTER(X): will register X with driver namespace
 * DRV_NS_UNREGISTER(X): will unregister X from driver namespace
 */

#define DEFINE_DRV_NS_INFO(X) \
	_drv_ns_id(X) \
	_drv_ns_find(X) \
	_drv_ns_get(X) \
	_drv_ns_get_cur(X) \
	_drv_ns_put(X) \
	_drv_ns_active(X) \
	_drv_nb_self(X)

#define DRV_NS_REGISTER(X, s) \
	(X ## _ns_id = register_drv_ns_ops(s, &X ## _ns_ops))

#define DRV_NS_UNREGISTER(X) \
	unregister_drv_ns_ops(X ## _ns_id)


#else  /* !CONFIG_DEV_NS */

/*
 * Driver authors should use this macro instead if !CONFIG_DRV_NS:
 * DEFINE_DRV_NS_INIT(X): find_X_ns(), get_X_ns(), get_X_ns_cur(), put_X_ns(),
 *                        is_active_X_ns()
 */
#define DEFINE_DRV_NS_INIT(x) \
	struct x ## _drv_ns init_ ## x ## _ns = { \
		.drv_ns_info = { \
			.drv_ns = &init_drv_ns, \
			.list = LIST_HEAD_INIT(init_ ## x ## _ns.drv_ns_info.list),\
			.count = ATOMIC_INIT(1), \
		}, \
	}; \
	static inline \
	struct x ## _drv_ns *find_ ## x ## _ns(struct drv_namespace *drv_ns) \
	{ return &init_ ## x ## _ns; } \
	struct x ## _drv_ns *get_ ## x ## _ns(struct drv_namespace *drv_ns) \
	{ return &init_ ## x ## _ns; } \
	static inline struct x ## _drv_ns *get_ ## x ## _ns_cur(void) \
	{ return &init_ ## x ## _ns; } \
	static inline void put_ ## x ## _ns(struct x ## _drv_ns *x ## _ns) \
	{ /* */ } \
	static inline bool is_active_ ## x ##_ns(struct x ## _drv_ns *ns) \
	{ return true; }

static inline void put_drv_ns(struct drv_namespace *drv_ns)
{ /* */ }

static inline struct drv_namespace *get_drv_ns(struct drv_namespace *drv_ns)
{ return drv_ns; }

static inline bool is_active_drv_ns(struct drv_namespace *drv_ns)
{ return true; }

static inline struct drv_namespace *current_drv_ns(void)
{ return &init_drv_ns; }

static inline struct drv_namespace *copy_drv_ns(unsigned long flags,
						struct task_struct *task,
						struct pid_namespace *new_pidns)
{
	if (flags & CLONE_NEWPID)
		return ERR_PTR(-EINVAL);
	return task->nsproxy->drv_ns;
}

static inline pid_t drv_ns_init_pid(struct drv_namespace *drv_ns)
{
	return init_task.pid;
}

#endif /* CONFIG_DEV_NS */

static inline int is_init_ns(struct drv_namespace *ns)
{
	if (ns == &init_drv_ns)
		return 1;
	return 0;
}

static inline struct nsproxy *drv_ns_nsproxy(struct drv_namespace *drv_ns)
{
	/*
	 * a device namespace has a one-to-one relationship with a
	 * PID namespace, so we know that the init task in the PID
	 * namespace will also share our device namespace. Therefore,
	 * we can take the nsproxy pointer from the child_reaper of
	 * our associated PID namespace.
	 */
	return task_nsproxy(drv_ns->pid_ns->child_reaper);
}


#endif /* __KERNEL__ */
#endif /* _LINUX_DEV_NS_H */
