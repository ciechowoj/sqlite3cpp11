#include <stdexcept>
#include <string>
#include <functional>

namespace sqlite3cpp {

class database;
class statement;

namespace detail {

void throw_exception(int, const char*);

struct impl {
	template <class T> static void*& get(T& x) {
		return x._impl;
	}

	template <class T> static const void* get(const T& x) {
		return x._impl;
	}
};

}

enum result_t {
	row,
	done
};

void clear_bindings(statement& statement);
void bind(statement& statement, std::size_t index, double value);
void bind(statement& statement, std::size_t index, long long value);
void bind(statement& statement, std::size_t index, unsigned long long value);
void bind(statement& statement, std::size_t index, const std::string& value);
void bind(statement& statement, std::size_t index, const char* value);
void bind(statement& statement, std::size_t index, const char* value, std::size_t bytes);
void bind_null(statement& statement, std::size_t index);
void bind_zeros(statement& statement, std::size_t index, std::size_t bytes);

std::size_t bind_parameter_count(statement& statement);
std::size_t bind_parameter_index(statement& statement, const char* name);
std::size_t bind_parameter_index(statement& statement, const std::string& name);
std::string bind_parameter_name_str(statement& statement, std::size_t index);
const char* bind_parameter_name_cstr(statement& statement, std::size_t index);

void close(database& database);

double column_double(statement& statement, std::size_t index);
signed char column_byte(statement& statement, std::size_t index);
unsigned char column_ubyte(statement& statement, std::size_t index);
short column_short(statement& statement, std::size_t index);
unsigned short column_ushort(statement& statement, std::size_t index);
int column_int(statement& statement, std::size_t index);
unsigned int column_uint(statement& statement, std::size_t index);
long column_long(statement& statement, std::size_t index);
unsigned long column_ulong(statement& statement, std::size_t index);
long long column_llong(statement& statement, std::size_t index);
unsigned long long column_ullong(statement& statement, std::size_t index);
std::string column_str(statement& statement, std::size_t index);
const char* column_cstr(statement& statement, std::size_t index);
std::size_t column_bytes(statement& statement, std::size_t index);

template <class T> inline T column(statement& statement, std::size_t index) {
	static_assert("Requested type T is invalid.", false);
}

template <> inline double column<double>(statement& statement, std::size_t index) {
	return column_double(statement, index);
}

std::size_t column_count(statement& statement);
std::string column_type_str(statement& statement, std::size_t index);
const char* column_type_cstr(statement& statement, std::size_t index);
std::string column_decltype_str(statement& statement, std::size_t index);
const char* column_decltype_cstr(statement& statement, std::size_t index);
std::string column_name_str(statement& statement, std::size_t index);
const char* column_name_cstr(statement& statement, std::size_t index);
std::string column_origin_name_str(statement& statement, std::size_t index);
const char* column_origin_name_cstr(statement& statement, std::size_t index);
std::string column_table_name_str(statement& statement, std::size_t index);
const char* column_table_name_cstr(statement& statement, std::size_t index);
std::string column_database_name_str(statement& statement, std::size_t index);
const char* column_database_name_cstr(statement& statement, std::size_t index);

statement prepare(database& database, std::string::const_iterator begin, std::string::const_iterator end, std::string::const_iterator& tail);
statement prepare(database& database, const char* sql, const char*& tail);
statement prepare(database& database, const char* sql, std::size_t bytes, const char*& tail);
result_t step(statement& statement);
void finalize(statement& statement);



void exec(database& database, const char* sql);
void exec(database& database, const char* sql, std::function<bool(int, const char* const*, const char* const*)> callback);



class database {
public:
	database();
	database(database&& that);
	~database();
	database& operator=(database&& that);
	explicit operator bool();
private:
	void* _impl;
	friend struct detail::impl;
	database(const database&);
	database& operator=(const database&);
};

class statement {
public:
	statement();
	statement(statement&& that);
	~statement();
	statement& operator=(statement&& that);
	explicit operator bool();
private:
	void* _impl;
	friend struct detail::impl;
	statement(const statement&);
	statement& operator=(const statement&);
};

template <class T> inline std::pair<bool, T> exec(database& database, const char* sql, std::size_t bytes) {
	static_assert("Requested type T is invalid.", false);
}

template <> inline std::pair<bool, long long> exec<long long>(database& database, const char* sql, std::size_t bytes) {
	const char* itr = sql;
	const char* end = sql + bytes;
	sqlite3cpp::statement statement;
	long long func_result = 0;
	bool initialized = false;
	while (itr != end) {
		statement = prepare(database, itr, end - itr, itr);
		result_t step_result = done;
		do {
			step_result = step(statement);
			if (!initialized && column_count(statement) != 0) {
				func_result = column<long long>(statement, 0);
				initialized = true;
			}
		} while (step_result != done);
		finalize(statement);
	}

	return std::make_pair(initialized, func_result);
}

template <> inline std::pair<bool, unsigned long long> exec<unsigned long long>(database& database, const char* sql, std::size_t bytes) {
	const char* itr = sql;
	const char* end = sql + bytes;
	sqlite3cpp::statement statement;
	long long func_result = 0;
	bool initialized = false;
	while (itr != end) {
		statement = prepare(database, itr, end - itr, itr);
		result_t step_result = done;
		do {
			step_result = step(statement);
			if (!initialized && column_count(statement) != 0) {
				func_result = column<long long>(statement, 0);
				initialized = true;
			}
		} while (step_result != done);
		finalize(statement);
	}

	return std::make_pair(initialized, func_result);
}




class sqlite_error : public std::runtime_error {
protected:
	sqlite_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class abort_error : public sqlite_error {
protected:
	abort_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class auth_error : public sqlite_error {
	auth_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class busy_error : public sqlite_error {
protected:
	busy_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class cantopen_error : public sqlite_error {
protected:
	cantopen_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_error : public sqlite_error {
protected:
	constraint_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class corrupt_error : public sqlite_error {
protected:
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
protected:
	ioerr_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class locked_error : public sqlite_error {
protected:
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
protected:
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

class abort_rollback_error : public abort_error {
	abort_rollback_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class busy_recovery_error : public busy_error {
	busy_recovery_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class busy_snapshot_error : public busy_error {
	busy_snapshot_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class cantopen_convpath_error : public cantopen_error {
	cantopen_convpath_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class cantopen_fullpath_error : public cantopen_error {
	cantopen_fullpath_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class cantopen_isdir_error : public cantopen_error {
	cantopen_isdir_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_check_error : public constraint_error {
	constraint_check_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_commithook_error : public constraint_error {
	constraint_commithook_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_foreignkey_error : public constraint_error {
	constraint_foreignkey_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_function_error : public constraint_error {
	constraint_function_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_notnull_error : public constraint_error {
	constraint_notnull_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_primarykey_error : public constraint_error {
	constraint_primarykey_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_rowid_error : public constraint_error {
	constraint_rowid_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_trigger_error : public constraint_error {
	constraint_trigger_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_unique_error : public constraint_error {
	constraint_unique_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class constraint_vtab_error : public constraint_error {
	constraint_vtab_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class corrupt_vtab_error : public corrupt_error {
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

class locked_sharedcache_error : public locked_error {
	locked_sharedcache_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class readonly_cantlock_error : public readonly_error {
	readonly_cantlock_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class readonly_dbmoved_error : public readonly_error {
	readonly_dbmoved_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class readonly_recovery_error : public readonly_error {
	readonly_recovery_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

class readonly_rollback_error : public readonly_error {
	readonly_rollback_error(const char*);
	friend void detail::throw_exception(int, const char*);
};

}

namespace sqlite = sqlite3cpp;