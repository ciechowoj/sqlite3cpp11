#include <stdexcept>

namespace sqlite3cpp {
namespace detail {

void throw_exception(int, const char*);

}

enum result_t {
	ok,
	row,
	done
};

class sqlite_error : public std::runtime_error {
	sqlite_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class abort_error : public sqlite_error {
	abort_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class auth_error : public sqlite_error {
	auth_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class busy_error : public sqlite_error {
	busy_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class cantopen_error : public sqlite_error {
	cantopen_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_error : public sqlite_error {
	constraint_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class corrupt_error : public sqlite_error {
	corrupt_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class empty_error : public sqlite_error {
	empty_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class format_error : public sqlite_error {
	format_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class full_error : public sqlite_error {
	full_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class internal_error : public sqlite_error {
	internal_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class interrupt_error : public sqlite_error {
	interrupt_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_error : public sqlite_error {
	ioerr_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class locked_error : public sqlite_error {
	locked_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class mismatch_error : public sqlite_error {
	mismatch_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class misuse_error : public sqlite_error {
	misuse_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class nolfs_error : public sqlite_error {
	nolfs_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class nomem_error : public sqlite_error {
	nomem_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class notadb_error : public sqlite_error {
	notadb_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class notfound_error : public sqlite_error {
	notfound_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class perm_error : public sqlite_error {
	perm_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class protocol_error : public sqlite_error {
	protocol_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class range_error : public sqlite_error {
	range_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class readonly_error : public sqlite_error {
	readonly_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class row_error : public sqlite_error {
	row_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class schema_error : public sqlite_error {
	schema_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class toobig_error : public sqlite_error {
	toobig_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class warning_error : public sqlite_error {
	warning_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class abort_rollback_error : public ioerr_error {
	abort_rollback_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class busy_recovery_error : public ioerr_error {
	busy_recovery_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class busy_snapshot_error : public ioerr_error {
	busy_snapshot_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class cantopen_convpath_error : public ioerr_error {
	cantopen_convpath_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class cantopen_fullpath_error : public ioerr_error {
	cantopen_fullpath_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class cantopen_isdir_error : public ioerr_error {
	cantopen_isdir_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_check_error : public ioerr_error {
	constraint_check_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_commithook_error : public ioerr_error {
	constraint_commithook_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_foreignkey_error : public ioerr_error {
	constraint_foreignkey_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_function_error : public ioerr_error {
	constraint_function_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_notnull_error : public ioerr_error {
	constraint_notnull_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_primarykey_error : public ioerr_error {
	constraint_primarykey_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_rowid_error : public ioerr_error {
	constraint_rowid_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_trigger_error : public ioerr_error {
	constraint_trigger_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_unique_error : public ioerr_error {
	constraint_unique_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_vtab_error : public ioerr_error {
	constraint_vtab_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class corrupt_vtab_error : public ioerr_error {
	corrupt_vtab_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_access_error : public ioerr_error {
	ioerr_access_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_checkreservedlock_error : public ioerr_error {
	ioerr_checkreservedlock_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_close_error : public ioerr_error {
	ioerr_close_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_convpath_error : public ioerr_error {
	ioerr_convpath_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_delete_error : public ioerr_error {
	ioerr_delete_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_delete_noent_error : public ioerr_error {
	ioerr_delete_noent_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_dir_fsync_error : public ioerr_error {
	ioerr_dir_fsync_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_fstat_error : public ioerr_error {
	ioerr_fstat_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_fsync_error : public ioerr_error {
	ioerr_fsync_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_gettemppath_error : public ioerr_error {
	ioerr_gettemppath_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_lock_error : public ioerr_error {
	ioerr_lock_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_mmap_error : public ioerr_error {
	ioerr_mmap_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_nomem_error : public ioerr_error {
	ioerr_nomem_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_rdlock_error : public ioerr_error {
	ioerr_rdlock_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_read_error : public ioerr_error {
	ioerr_read_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_seek_error : public ioerr_error {
	ioerr_seek_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_shmmap_error : public ioerr_error {
	ioerr_shmmap_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_shmopen_error : public ioerr_error {
	ioerr_shmopen_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_shmsize_error : public ioerr_error {
	ioerr_shmsize_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_short_read_error : public ioerr_error {
	ioerr_short_read_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_truncate_error : public ioerr_error {
	ioerr_truncate_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_unlock_error : public ioerr_error {
	ioerr_unlock_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class ioerr_write_error : public ioerr_error {
	ioerr_write_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class locked_sharedcache_error : public ioerr_error {
	locked_sharedcache_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class readonly_cantlock_error : public ioerr_error {
	readonly_cantlock_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class readonly_dbmoved_error : public ioerr_error {
	readonly_dbmoved_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class readonly_recovery_error : public ioerr_error {
	readonly_recovery_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class readonly_rollback_error : public ioerr_error {
	readonly_rollback_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

}
