#include <linux/integrity.h>
#include <linux/ima.h>
#include <linux/evm.h>
/* How many LSMs were built into the kernel? */
#define LSM_COUNT (__end_lsm_info - __start_lsm_info)
struct security_hook_heads security_hook_heads __ro_after_init;
static __initdata struct lsm_info **ordered_lsms;
	if (WARN(last_lsm == LSM_COUNT, "%s: out of LSM slots!?\n", from))
	ordered_lsms = kcalloc(LSM_COUNT + 1, sizeof(*ordered_lsms),
			       GFP_KERNEL);


	kfree(ordered_lsms);
#define LSM_HOOK(RET, DEFAULT, NAME, ...) \
	INIT_HLIST_HEAD(&security_hook_heads.NAME);
#include "linux/lsm_hook_defs.h"
#undef LSM_HOOK

 * @lsm: the name of the security module
			       const char *lsm)
		hooks[i].lsm = lsm;
		hlist_add_tail_rcu(&hooks[i].list, hooks[i].head);
		if (lsm_append(lsm, &lsm_names) < 0)
 * lsm_cred_alloc - allocate a composite cred blob
 * @cred: the cred that needs a blob
 * Allocate the cred blob for all the modules
static int lsm_cred_alloc(struct cred *cred, gfp_t gfp)
	if (blob_sizes.lbs_cred == 0) {
		cred->security = NULL;
	cred->security = kzalloc(blob_sizes.lbs_cred, gfp);
	if (cred->security == NULL)
int lsm_inode_alloc(struct inode *inode)
	inode->i_security = kmem_cache_zalloc(lsm_inode_cache, GFP_NOFS);
	if (blob_sizes.lbs_task == 0) {
		task->security = NULL;
		return 0;
	}

	task->security = kzalloc(blob_sizes.lbs_task, GFP_KERNEL);
	if (task->security == NULL)
		return -ENOMEM;
	return 0;
	if (blob_sizes.lbs_ipc == 0) {
		kip->security = NULL;
		return 0;
	}
	kip->security = kzalloc(blob_sizes.lbs_ipc, GFP_KERNEL);
	if (kip->security == NULL)
		return -ENOMEM;
	return 0;
	if (blob_sizes.lbs_msg_msg == 0) {
		mp->security = NULL;
	mp->security = kzalloc(blob_sizes.lbs_msg_msg, GFP_KERNEL);
	if (mp->security == NULL)
	if (blob_sizes.lbs_superblock == 0) {
		sb->s_security = NULL;
		return 0;
	sb->s_security = kzalloc(blob_sizes.lbs_superblock, GFP_KERNEL);
	if (sb->s_security == NULL)
		return -ENOMEM;
	return 0;
#define call_void_hook(FUNC, ...)				\
	do {							\
		struct security_hook_list *P;			\
								\
		hlist_for_each_entry(P, &security_hook_heads.FUNC, list) \
			P->hook.FUNC(__VA_ARGS__);		\
#define call_int_hook(FUNC, IRC, ...) ({			\
	int RC = IRC;						\
	do {							\
		struct security_hook_list *P;			\
								\
		hlist_for_each_entry(P, &security_hook_heads.FUNC, list) { \
			RC = P->hook.FUNC(__VA_ARGS__);		\
			if (RC != 0)				\
				break;				\
		}						\
	} while (0);						\
	RC;							\
	return call_int_hook(binder_set_context_mgr, 0, mgr);
EXPORT_SYMBOL_GPL(security_binder_set_context_mgr);
	return call_int_hook(binder_transaction, 0, from, to);
EXPORT_SYMBOL_GPL(security_binder_transaction);
	return call_int_hook(binder_transfer_binder, 0, from, to);
EXPORT_SYMBOL_GPL(security_binder_transfer_binder);
	return call_int_hook(binder_transfer_file, 0, from, to, file);
EXPORT_SYMBOL_GPL(security_binder_transfer_file);
	return call_int_hook(ptrace_access_check, 0, child, mode);
	return call_int_hook(ptrace_traceme, 0, parent);
	return call_int_hook(capget, 0, target,
			     effective, inheritable, permitted);
	return call_int_hook(capset, 0, new, old,
			     effective, inheritable, permitted);
	return call_int_hook(capable, 0, cred, ns, cap, opts);
int security_quotactl(int cmds, int type, int id, struct super_block *sb)
	return call_int_hook(quotactl, 0, cmds, type, id, sb);
	return call_int_hook(quota_on, 0, dentry);
	return call_int_hook(syslog, 0, type);
	return call_int_hook(settime, 0, ts, tz);
	struct security_hook_list *hp;
	 * The module will respond with a positive value if
	 * it thinks the __vm_enough_memory() call should be
	 * made with the cap_sys_admin set. If all of the modules
	 * agree that it should be set it will. If any module
	 * thinks it should not be set it won't.
	hlist_for_each_entry(hp, &security_hook_heads.vm_enough_memory, list) {
		rc = hp->hook.vm_enough_memory(mm, pages);
		if (rc <= 0) {
	return call_int_hook(bprm_creds_for_exec, 0, bprm);
int security_bprm_creds_from_file(struct linux_binprm *bprm, struct file *file)
	return call_int_hook(bprm_creds_from_file, 0, bprm, file);
	int ret;

	ret = call_int_hook(bprm_check_security, 0, bprm);
	if (ret)
		return ret;
	return ima_bprm_check(bprm);
void security_bprm_committing_creds(struct linux_binprm *bprm)
void security_bprm_committed_creds(struct linux_binprm *bprm)
	return call_int_hook(fs_context_submount, 0, fc, reference);
	return call_int_hook(fs_context_dup, 0, fc, src_fc);
	struct security_hook_list *hp;
	hlist_for_each_entry(hp, &security_hook_heads.fs_context_parse_param,
			     list) {
		trc = hp->hook.fs_context_parse_param(fc, param);
	rc = call_int_hook(sb_alloc_security, 0, sb);
	return call_int_hook(sb_eat_lsm_opts, 0, options, mnt_opts);
	return call_int_hook(sb_mnt_opts_compat, 0, sb, mnt_opts);
	return call_int_hook(sb_remount, 0, sb, mnt_opts);
int security_sb_kern_mount(struct super_block *sb)
	return call_int_hook(sb_kern_mount, 0, sb);
	return call_int_hook(sb_show_options, 0, m, sb);
	return call_int_hook(sb_statfs, 0, dentry);
	return call_int_hook(sb_mount, 0, dev_name, path, type, flags, data);
	return call_int_hook(sb_umount, 0, mnt, flags);
	return call_int_hook(sb_pivotroot, 0, old_path, new_path);
	return call_int_hook(sb_set_mnt_opts,
			     mnt_opts ? -EOPNOTSUPP : 0, sb,
			     mnt_opts, kern_flags, set_kern_flags);
	return call_int_hook(sb_clone_mnt_opts, 0, oldsb, newsb,
	return call_int_hook(move_mount, 0, from_path, to_path);
	return call_int_hook(path_notify, 0, path, mask, obj_type);
int security_inode_alloc(struct inode *inode)
	int rc = lsm_inode_alloc(inode);
	rc = call_int_hook(inode_alloc_security, 0, inode);
	/*
	 * The rcu head is at the start of the inode blob
	 */
 * Deallocate the inode security structure and set @inode->i_security to NULL.
	integrity_inode_free(inode);
	/*
	 * The inode may still be referenced in a path walk and
	 * a call to security_inode_permission() can be made
	 * after inode_free_security() is called. Ideally, the VFS
	 * wouldn't do this, but fixing that is a much harder
	 * job. For now, simply free the i_security via RCU, and
	 * leave the current inode->i_security pointer intact.
	 * The inode will be freed after the RCU grace period too.
	 */
	if (inode->i_security)
		call_rcu((struct rcu_head *)inode->i_security,
			 inode_free_by_rcu);
	struct security_hook_list *hp;
	int rc;

	/*
	 * Only one module will provide a security context.
	 */
	hlist_for_each_entry(hp, &security_hook_heads.dentry_init_security,
			     list) {
		rc = hp->hook.dentry_init_security(dentry, mode, name,
						   xattr_name, ctx, ctxlen);
		if (rc != LSM_RET_DEFAULT(dentry_init_security))
			return rc;
	}
	return LSM_RET_DEFAULT(dentry_init_security);
	return call_int_hook(dentry_create_files_as, 0, dentry, mode,
	struct security_hook_list *hp;
		/* Allocate +1 for EVM and +1 as terminator. */
		new_xattrs = kcalloc(blob_sizes.lbs_xattr_count + 2,
	hlist_for_each_entry(hp, &security_hook_heads.inode_init_security,
			     list) {
		ret = hp->hook.inode_init_security(inode, dir, qstr, new_xattrs,
	ret = evm_inode_init_security(inode, dir, qstr, new_xattrs,
				      &xattr_count);
	if (ret)
		goto out;
	return call_int_hook(inode_init_security_anon, 0, inode, name,
	return call_int_hook(path_mknod, 0, dir, dentry, mode, dev);
	return call_int_hook(path_mkdir, 0, dir, dentry, mode);
	return call_int_hook(path_rmdir, 0, dir, dentry);
	return call_int_hook(path_unlink, 0, dir, dentry);
	return call_int_hook(path_symlink, 0, dir, dentry, old_name);
	return call_int_hook(path_link, 0, old_dentry, new_dir, new_dentry);
	return call_int_hook(path_rename, 0, old_dir, old_dentry, new_dir,
	return call_int_hook(path_truncate, 0, path);
	return call_int_hook(path_chmod, 0, path, mode);
	return call_int_hook(path_chown, 0, path, uid, gid);
	return call_int_hook(path_chroot, 0, path);
	return call_int_hook(inode_create, 0, dir, dentry, mode);
	return call_int_hook(inode_link, 0, old_dentry, dir, new_dentry);
	return call_int_hook(inode_unlink, 0, dir, dentry);
	return call_int_hook(inode_symlink, 0, dir, dentry, old_name);
	return call_int_hook(inode_mkdir, 0, dir, dentry, mode);
	return call_int_hook(inode_rmdir, 0, dir, dentry);
	return call_int_hook(inode_mknod, 0, dir, dentry, mode, dev);
		int err = call_int_hook(inode_rename, 0, new_dir, new_dentry,
	return call_int_hook(inode_rename, 0, old_dir, old_dentry,
	return call_int_hook(inode_readlink, 0, dentry);
	return call_int_hook(inode_follow_link, 0, dentry, inode, rcu);
	return call_int_hook(inode_permission, 0, inode, mask);
	int ret;

	ret = call_int_hook(inode_setattr, 0, dentry, attr);
	if (ret)
		return ret;
	return evm_inode_setattr(idmap, dentry, attr);
	return call_int_hook(inode_getattr, 0, path);
 * Check permission before setting the extended attributes.
	int ret;
	/*
	 * SELinux and Smack integrate the cap call,
	 * so assume that all LSMs supplying this call do so.
	 */
	ret = call_int_hook(inode_setxattr, 1, idmap, dentry, name, value,
			    size, flags);
	if (ret == 1)
		ret = cap_inode_setxattr(dentry, name, value, size, flags);
	if (ret)
		return ret;
	ret = ima_inode_setxattr(dentry, name, value, size);
	if (ret)
		return ret;
	return evm_inode_setxattr(idmap, dentry, name, value, size);
	int ret;
		return 0;
	ret = call_int_hook(inode_set_acl, 0, idmap, dentry, acl_name,
			    kacl);
	if (ret)
		return ret;
	ret = ima_inode_set_acl(idmap, dentry, acl_name, kacl);
	if (ret)
		return ret;
	return evm_inode_set_acl(idmap, dentry, acl_name, kacl);
	return call_int_hook(inode_get_acl, 0, idmap, dentry, acl_name);
	int ret;
		return 0;
	ret = call_int_hook(inode_remove_acl, 0, idmap, dentry, acl_name);
	if (ret)
		return ret;
	ret = ima_inode_remove_acl(idmap, dentry, acl_name);
	if (ret)
		return ret;
	return evm_inode_remove_acl(idmap, dentry, acl_name);
	evm_inode_post_setxattr(dentry, name, value, size);
	return call_int_hook(inode_getxattr, 0, dentry, name);
	return call_int_hook(inode_listxattr, 0, dentry);
 * Check permission before removing the extended attribute identified by @name
 * for @dentry.
	int ret;
	/*
	 * SELinux and Smack integrate the cap call,
	 * so assume that all LSMs supplying this call do so.
	 */
	ret = call_int_hook(inode_removexattr, 1, idmap, dentry, name);
	if (ret == 1)
		ret = cap_inode_removexattr(idmap, dentry, name);
	if (ret)
		return ret;
	ret = ima_inode_removexattr(dentry, name);
	if (ret)
		return ret;
	return evm_inode_removexattr(idmap, dentry, name);
	return call_int_hook(inode_need_killpriv, 0, dentry);
	return call_int_hook(inode_killpriv, 0, idmap, dentry);
	struct security_hook_list *hp;
	int rc;

	/*
	 * Only one module will provide an attribute with a given name.
	 */
	hlist_for_each_entry(hp, &security_hook_heads.inode_getsecurity, list) {
		rc = hp->hook.inode_getsecurity(idmap, inode, name, buffer,
						alloc);
		if (rc != LSM_RET_DEFAULT(inode_getsecurity))
			return rc;
	}
	return LSM_RET_DEFAULT(inode_getsecurity);
	struct security_hook_list *hp;
	int rc;

	/*
	 * Only one module will provide an attribute with a given name.
	 */
	hlist_for_each_entry(hp, &security_hook_heads.inode_setsecurity, list) {
		rc = hp->hook.inode_setsecurity(inode, name, value, size,
						flags);
		if (rc != LSM_RET_DEFAULT(inode_setsecurity))
			return rc;
	}
	return LSM_RET_DEFAULT(inode_setsecurity);
	return call_int_hook(inode_listsecurity, 0, inode, buffer, buffer_size);
	return call_int_hook(inode_copy_up, 0, src, new);
 * Return: Returns 0 to accept the xattr, 1 to discard the xattr, -EOPNOTSUPP
 *         if the security module does not know about attribute, or a negative
 *         error code to abort the copy up.
int security_inode_copy_up_xattr(const char *name)
	struct security_hook_list *hp;
	/*
	 * The implementation can return 0 (accept the xattr), 1 (discard the
	 * xattr), -EOPNOTSUPP if it does not know anything about the xattr or
	 * any other error code in case of an error.
	 */
	hlist_for_each_entry(hp,
			     &security_hook_heads.inode_copy_up_xattr, list) {
		rc = hp->hook.inode_copy_up_xattr(name);
		if (rc != LSM_RET_DEFAULT(inode_copy_up_xattr))
			return rc;
	}
	return call_int_hook(kernfs_init_security, 0, kn_dir, kn);
	int ret;

	ret = call_int_hook(file_permission, 0, file, mask);
	if (ret)
		return ret;

	return fsnotify_perm(file, mask);
	rc = call_int_hook(file_alloc_security, 0, file);
	return call_int_hook(file_ioctl, 0, file, cmd, arg);
	return call_int_hook(file_ioctl_compat, 0, file, cmd, arg);
	unsigned long prot_adj = mmap_prot(file, prot);
	int ret;

	ret = call_int_hook(mmap_file, 0, file, prot, prot_adj, flags);
	if (ret)
		return ret;
	return ima_file_mmap(file, prot, prot_adj, flags);
	return call_int_hook(mmap_addr, 0, addr);
	int ret;

	ret = call_int_hook(file_mprotect, 0, vma, reqprot, prot);
	if (ret)
		return ret;
	return ima_file_mprotect(vma, prot);
	return call_int_hook(file_lock, 0, file, cmd);
	return call_int_hook(file_fcntl, 0, file, cmd, arg);
	return call_int_hook(file_send_sigiotask, 0, tsk, fown, sig);
 * security_file_receive() - Check is receiving a file via IPC is allowed
	return call_int_hook(file_receive, 0, file);
	ret = call_int_hook(file_open, 0, file);
	return fsnotify_perm(file, MAY_OPEN);
	return call_int_hook(file_truncate, 0, file);
	rc = call_int_hook(task_alloc, 0, task, clone_flags);
	rc = call_int_hook(cred_alloc_blank, 0, cred, gfp);
	rc = call_int_hook(cred_prepare, 0, new, old, gfp);
	return call_int_hook(kernel_act_as, 0, new, secid);
	return call_int_hook(kernel_create_files_as, 0, new, inode);
 * security_kernel_module_request() - Check is loading a module is allowed
	int ret;

	ret = call_int_hook(kernel_module_request, 0, kmod_name);
	if (ret)
		return ret;
	return integrity_kernel_module_request(kmod_name);
	int ret;

	ret = call_int_hook(kernel_read_file, 0, file, id, contents);
	if (ret)
		return ret;
	return ima_read_file(file, id, contents);
	int ret;

	ret = call_int_hook(kernel_post_read_file, 0, file, buf, size, id);
	if (ret)
		return ret;
	return ima_post_read_file(file, buf, size, id);
	int ret;

	ret = call_int_hook(kernel_load_data, 0, id, contents);
	if (ret)
		return ret;
	return ima_load_data(id, contents);
	int ret;

	ret = call_int_hook(kernel_post_load_data, 0, buf, size, id,
			    description);
	if (ret)
		return ret;
	return ima_post_load_data(buf, size, id, description);
	return call_int_hook(task_fix_setuid, 0, new, old, flags);
	return call_int_hook(task_fix_setgid, 0, new, old, flags);
	return call_int_hook(task_fix_setgroups, 0, new, old);
	return call_int_hook(task_setpgid, 0, p, pgid);
	return call_int_hook(task_getpgid, 0, p);
	return call_int_hook(task_getsid, 0, p);
	return call_int_hook(task_setnice, 0, p, nice);
	return call_int_hook(task_setioprio, 0, p, ioprio);
	return call_int_hook(task_getioprio, 0, p);
	return call_int_hook(task_prlimit, 0, cred, tcred, flags);
	return call_int_hook(task_setrlimit, 0, p, resource, new_rlim);
	return call_int_hook(task_setscheduler, 0, p);
	return call_int_hook(task_getscheduler, 0, p);
	return call_int_hook(task_movememory, 0, p);
	return call_int_hook(task_kill, 0, p, info, sig, cred);
	struct security_hook_list *hp;
	hlist_for_each_entry(hp, &security_hook_heads.task_prctl, list) {
		thisrc = hp->hook.task_prctl(option, arg2, arg3, arg4, arg5);
	return call_int_hook(userns_create, 0, cred);
	return call_int_hook(ipc_permission, 0, ipcp, flag);
	rc = call_int_hook(msg_msg_alloc_security, 0, msg);
	rc = call_int_hook(msg_queue_alloc_security, 0, msq);
	return call_int_hook(msg_queue_associate, 0, msq, msqflg);
	return call_int_hook(msg_queue_msgctl, 0, msq, cmd);
	return call_int_hook(msg_queue_msgsnd, 0, msq, msg, msqflg);
	return call_int_hook(msg_queue_msgrcv, 0, msq, msg, target, type, mode);
	rc = call_int_hook(shm_alloc_security, 0, shp);
	return call_int_hook(shm_associate, 0, shp, shmflg);
	return call_int_hook(shm_shmctl, 0, shp, cmd);
	return call_int_hook(shm_shmat, 0, shp, shmaddr, shmflg);
	rc = call_int_hook(sem_alloc_security, 0, sma);
	return call_int_hook(sem_associate, 0, sma, semflg);
	return call_int_hook(sem_semctl, 0, sma, cmd);
	return call_int_hook(sem_semop, 0, sma, sops, nsops, alter);
 * @lsm: LSM name
int security_getprocattr(struct task_struct *p, const char *lsm,
			 const char *name, char **value)
	struct security_hook_list *hp;
	hlist_for_each_entry(hp, &security_hook_heads.getprocattr, list) {
		if (lsm != NULL && strcmp(lsm, hp->lsm))
		return hp->hook.getprocattr(p, name, value);
 * @lsm: LSM name
int security_setprocattr(const char *lsm, const char *name, void *value,
			 size_t size)
	struct security_hook_list *hp;
	hlist_for_each_entry(hp, &security_hook_heads.setprocattr, list) {
		if (lsm != NULL && strcmp(lsm, hp->lsm))
		return hp->hook.setprocattr(name, value, size);
	return call_int_hook(netlink_send, 0, sk, skb);
 * security_ismaclabel() - Check is the named attribute is a MAC label
	return call_int_hook(ismaclabel, 0, name);
	struct security_hook_list *hp;
	int rc;

	/*
	 * Currently, only one LSM can implement secid_to_secctx (i.e this
	 * LSM hook is not "stackable").
	 */
	hlist_for_each_entry(hp, &security_hook_heads.secid_to_secctx, list) {
		rc = hp->hook.secid_to_secctx(secid, secdata, seclen);
		if (rc != LSM_RET_DEFAULT(secid_to_secctx))
			return rc;
	}

	return LSM_RET_DEFAULT(secid_to_secctx);
	return call_int_hook(secctx_to_secid, 0, secdata, seclen, secid);
 * security_inode_notifysecctx() - Nofify the LSM of an inode's security label
	return call_int_hook(inode_notifysecctx, 0, inode, ctx, ctxlen);
	return call_int_hook(inode_setsecctx, 0, dentry, ctx, ctxlen);
	struct security_hook_list *hp;
	int rc;

	/*
	 * Only one module will provide a security context.
	 */
	hlist_for_each_entry(hp, &security_hook_heads.inode_getsecctx, list) {
		rc = hp->hook.inode_getsecctx(inode, ctx, ctxlen);
		if (rc != LSM_RET_DEFAULT(inode_getsecctx))
			return rc;
	}

	return LSM_RET_DEFAULT(inode_getsecctx);
	return call_int_hook(post_notification, 0, w_cred, cred, n);
	return call_int_hook(watch_key, 0, key);
	return call_int_hook(unix_stream_connect, 0, sock, other, newsk);
	return call_int_hook(unix_may_send, 0, sock, other);
	return call_int_hook(socket_create, 0, family, type, protocol, kern);
	return call_int_hook(socket_post_create, 0, sock, family, type,
	return call_int_hook(socket_socketpair, 0, socka, sockb);
	return call_int_hook(socket_bind, 0, sock, address, addrlen);
	return call_int_hook(socket_connect, 0, sock, address, addrlen);
	return call_int_hook(socket_listen, 0, sock, backlog);
	return call_int_hook(socket_accept, 0, sock, newsock);
 * security_socket_sendmsg() - Check is sending a message is allowed
	return call_int_hook(socket_sendmsg, 0, sock, msg, size);
	return call_int_hook(socket_recvmsg, 0, sock, msg, size, flags);
	return call_int_hook(socket_getsockname, 0, sock);
	return call_int_hook(socket_getpeername, 0, sock);
	return call_int_hook(socket_getsockopt, 0, sock, level, optname);
	return call_int_hook(socket_setsockopt, 0, sock, level, optname);
	return call_int_hook(socket_shutdown, 0, sock, how);
	return call_int_hook(socket_sock_rcv_skb, 0, sk, skb);
	struct security_hook_list *hp;
	int rc;

	/*
	 * Only one module will provide a security context.
	 */
	hlist_for_each_entry(hp, &security_hook_heads.socket_getpeersec_stream,
			     list) {
		rc = hp->hook.socket_getpeersec_stream(sock, optval, optlen,
						       len);
		if (rc != LSM_RET_DEFAULT(socket_getpeersec_stream))
			return rc;
	}
	return LSM_RET_DEFAULT(socket_getpeersec_stream);
	struct security_hook_list *hp;
	int rc;
	/*
	 * Only one module will provide a security context.
	 */
	hlist_for_each_entry(hp, &security_hook_heads.socket_getpeersec_dgram,
			     list) {
		rc = hp->hook.socket_getpeersec_dgram(sock, skb, secid);
		if (rc != LSM_RET_DEFAULT(socket_getpeersec_dgram))
			return rc;
	}
	return LSM_RET_DEFAULT(socket_getpeersec_dgram);
EXPORT_SYMBOL(security_socket_getpeersec_dgram);
	return call_int_hook(sk_alloc_security, 0, sk, family, priority);
	return call_int_hook(inet_conn_request, 0, sk, skb, req);
	return call_int_hook(secmark_relabel_packet, 0, secid);
	return call_int_hook(tun_dev_alloc_security, 0, security);
	call_void_hook(tun_dev_free_security, security);
	return call_int_hook(tun_dev_create, 0);
	return call_int_hook(tun_dev_attach_queue, 0, security);
	return call_int_hook(tun_dev_attach, 0, sk, security);
	return call_int_hook(tun_dev_open, 0, security);
	return call_int_hook(sctp_assoc_request, 0, asoc, skb);
	return call_int_hook(sctp_bind_connect, 0, sk, optname,
			     address, addrlen);
	return call_int_hook(sctp_assoc_established, 0, asoc, skb);
	return call_int_hook(mptcp_add_subflow, 0, sk, ssk);
	return call_int_hook(ib_pkey_access, 0, sec, subnet_prefix, pkey);
	return call_int_hook(ib_endport_manage_subnet, 0, sec,
			     dev_name, port_num);
	return call_int_hook(ib_alloc_security, 0, sec);
	call_void_hook(ib_free_security, sec);
	return call_int_hook(xfrm_policy_alloc_security, 0, ctxp, sec_ctx, gfp);
	return call_int_hook(xfrm_policy_clone_security, 0, old_ctx, new_ctxp);
	return call_int_hook(xfrm_policy_delete_security, 0, ctx);
	return call_int_hook(xfrm_state_alloc, 0, x, sec_ctx);
	return call_int_hook(xfrm_state_alloc_acquire, 0, x, polsec, secid);
	return call_int_hook(xfrm_state_delete_security, 0, x);
	return call_int_hook(xfrm_policy_lookup, 0, ctx, fl_secid);
	struct security_hook_list *hp;
	hlist_for_each_entry(hp, &security_hook_heads.xfrm_state_pol_flow_match,
			     list) {
		rc = hp->hook.xfrm_state_pol_flow_match(x, xp, flic);
	return call_int_hook(xfrm_decode_session, 0, skb, secid, 1);
	int rc = call_int_hook(xfrm_decode_session, 0, skb, &flic->flowic_secid,
	return call_int_hook(key_alloc, 0, key, cred, flags);
	call_void_hook(key_free, key);
	return call_int_hook(key_permission, 0, key_ref, cred, need_perm);
	return call_int_hook(key_getsecurity, 0, key, buffer);
int security_audit_rule_init(u32 field, u32 op, char *rulestr, void **lsmrule)
	return call_int_hook(audit_rule_init, 0, field, op, rulestr, lsmrule);
	return call_int_hook(audit_rule_known, 0, krule);
	return call_int_hook(audit_rule_match, 0, secid, field, op, lsmrule);
	return call_int_hook(bpf, 0, cmd, attr, size);
	return call_int_hook(bpf_map, 0, map, fmode);
	return call_int_hook(bpf_prog, 0, prog);
 * security_bpf_map_alloc() - Allocate a bpf map LSM blob
 * @map: bpf map
 * Initialize the security field inside bpf map.
int security_bpf_map_alloc(struct bpf_map *map)
	return call_int_hook(bpf_map_alloc_security, 0, map);
 * security_bpf_prog_alloc() - Allocate a bpf program LSM blob
 * @aux: bpf program aux info struct
 * Initialize the security field inside bpf program.
int security_bpf_prog_alloc(struct bpf_prog_aux *aux)
	return call_int_hook(bpf_prog_alloc_security, 0, aux);
	call_void_hook(bpf_map_free_security, map);
 * security_bpf_prog_free() - Free a bpf program's LSM blob
 * @aux: bpf program aux info struct
 * Clean up the security information stored inside bpf prog.
void security_bpf_prog_free(struct bpf_prog_aux *aux)
	call_void_hook(bpf_prog_free_security, aux);
	return call_int_hook(locked_down, 0, what);
	return call_int_hook(perf_event_open, 0, attr, type);
	return call_int_hook(perf_event_alloc, 0, event);
	call_void_hook(perf_event_free, event);
	return call_int_hook(perf_event_read, 0, event);
	return call_int_hook(perf_event_write, 0, event);
	return call_int_hook(uring_override_creds, 0, new);
	return call_int_hook(uring_sqpoll, 0);
	return call_int_hook(uring_cmd, 0, ioucmd);