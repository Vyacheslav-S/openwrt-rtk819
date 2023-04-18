/*
 * Copyright (c) 2013  Hauke Mehrtens <hauke@hauke-m.de>
 * Copyright (c) 2013  Hannes Frederic Sowa <hannes@stressinduktion.org>
 * Copyright (c) 2014  Luis R. Rodriguez <mcgrof@do-not-panic.com>
 *
 * Backport functionality introduced in Linux 3.13.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/version.h>
#include <linux/kernel.h>
#include <net/genetlink.h>

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0))
#ifdef CPTCFG_REGULATOR
#include <linux/module.h>
#include <linux/regulator/driver.h>
#include <linux/device.h>
#include <linux/static_key.h>

static void devm_rdev_release(struct device *dev, void *res)
{
	regulator_unregister(*(struct regulator_dev **)res);
}

/**
 * devm_regulator_register - Resource managed regulator_register()
 * @regulator_desc: regulator to register
 * @config: runtime configuration for regulator
 *
 * Called by regulator drivers to register a regulator.  Returns a
 * valid pointer to struct regulator_dev on success or an ERR_PTR() on
 * error.  The regulator will automatically be released when the device
 * is unbound.
 */
struct regulator_dev *devm_regulator_register(struct device *dev,
				  const struct regulator_desc *regulator_desc,
				  const struct regulator_config *config)
{
	struct regulator_dev **ptr, *rdev;

	ptr = devres_alloc(devm_rdev_release, sizeof(*ptr),
			   GFP_KERNEL);
	if (!ptr)
		return ERR_PTR(-ENOMEM);

	rdev = regulator_register(regulator_desc, config);
	if (!IS_ERR(rdev)) {
		*ptr = rdev;
		devres_add(dev, ptr);
	} else {
		devres_free(ptr);
	}

	return rdev;
}
EXPORT_SYMBOL_GPL(devm_regulator_register);

static int devm_rdev_match(struct device *dev, void *res, void *data)
{
	struct regulator_dev **r = res;
	if (!r || !*r) {
		WARN_ON(!r || !*r);
		return 0;
	}
	return *r == data;
}

/**
 * devm_regulator_unregister - Resource managed regulator_unregister()
 * @regulator: regulator to free
 *
 * Unregister a regulator registered with devm_regulator_register().
 * Normally this function will not need to be called and the resource
 * management code will ensure that the resource is freed.
 */
void devm_regulator_unregister(struct device *dev, struct regulator_dev *rdev)
{
	int rc;

	rc = devres_release(dev, devm_rdev_release, devm_rdev_match, rdev);
	if (rc != 0)
		WARN_ON(rc);
}
EXPORT_SYMBOL_GPL(devm_regulator_unregister);
#endif /* CPTCFG_REGULATOR */
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)) */

/************* generic netlink backport *****************/

#undef genl_register_family
#undef genl_unregister_family

int __backport_genl_register_family(struct genl_family *family)
{
	int i, ret;

#define __copy(_field) family->family._field = family->_field
	__copy(id);
	__copy(hdrsize);
	__copy(version);
	__copy(maxattr);
	strncpy(family->family.name, family->name, sizeof(family->family.name));
	__copy(netnsok);
	__copy(pre_doit);
	__copy(post_doit);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	__copy(parallel_ops);
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,11,0)
	__copy(module);
#endif
#undef __copy

	ret = genl_register_family(&family->family);
	if (ret < 0)
		return ret;

	family->attrbuf = family->family.attrbuf;
	family->id = family->family.id;

	for (i = 0; i < family->n_ops; i++) {
		ret = genl_register_ops(&family->family, &family->ops[i]);
		if (ret < 0)
			goto error;
	}

	for (i = 0; i < family->n_mcgrps; i++) {
		ret = genl_register_mc_group(&family->family,
					     &family->mcgrps[i]);
		if (ret)
			goto error;
	}

	return 0;

 error:
	backport_genl_unregister_family(family);
	return ret;
}
EXPORT_SYMBOL_GPL(__backport_genl_register_family);

int backport_genl_unregister_family(struct genl_family *family)
{
	int err;
	err = genl_unregister_family(&family->family);
	return err;
}
EXPORT_SYMBOL_GPL(backport_genl_unregister_family);

#ifdef __BACKPORT_NET_GET_RANDOM_ONCE
struct __net_random_once_work {
	struct work_struct work;
	struct static_key *key;
};

static void __net_random_once_deferred(struct work_struct *w)
{
	struct __net_random_once_work *work =
		container_of(w, struct __net_random_once_work, work);
	if (!static_key_enabled(work->key))
		static_key_slow_inc(work->key);
	kfree(work);
}

static void __net_random_once_disable_jump(struct static_key *key)
{
	struct __net_random_once_work *w;

	w = kmalloc(sizeof(*w), GFP_ATOMIC);
	if (!w)
		return;

	INIT_WORK(&w->work, __net_random_once_deferred);
	w->key = key;
	schedule_work(&w->work);
}

bool __net_get_random_once(void *buf, int nbytes, bool *done,
			   struct static_key *done_key)
{
	static DEFINE_SPINLOCK(lock);
	unsigned long flags;

	spin_lock_irqsave(&lock, flags);
	if (*done) {
		spin_unlock_irqrestore(&lock, flags);
		return false;
	}

	get_random_bytes(buf, nbytes);
	*done = true;
	spin_unlock_irqrestore(&lock, flags);

	__net_random_once_disable_jump(done_key);

	return true;
}
EXPORT_SYMBOL_GPL(__net_get_random_once);
#endif /* __BACKPORT_NET_GET_RANDOM_ONCE */
