#include <sqlite3.hpp>
#include <sqlite3.h>
#include <limits>
namespace sqlite3cpp {
namespace detail {

void throw_exception(int code, const char* message) {
	switch (code) {
	case SQLITE_ABORT: throw abort_error(message);
	case SQLITE_AUTH: throw auth_error(message);
	case SQLITE_BUSY: throw busy_error(message);
	case SQLITE_CANTOPEN: throw cantopen_error(message);
	case SQLITE_CONSTRAINT: throw constraint_error(message);
	case SQLITE_CORRUPT: throw corrupt_error(message);
	case SQLITE_EMPTY: throw empty_error(message);
	case SQLITE_FORMAT: throw format_error(message);
	case SQLITE_FULL: throw full_error(message);
	case SQLITE_INTERNAL: throw internal_error(message);
	case SQLITE_INTERRUPT: throw interrupt_error(message);
	case SQLITE_IOERR: throw ioerr_error(message);
	case SQLITE_LOCKED: throw locked_error(message);
	case SQLITE_MISMATCH: throw mismatch_error(message);
	case SQLITE_MISUSE: throw misuse_error(message);
	case SQLITE_NOLFS: throw nolfs_error(message);
	case SQLITE_NOMEM: throw nomem_error(message);
	case SQLITE_NOTADB: throw notadb_error(message);
	case SQLITE_NOTFOUND: throw notfound_error(message);
	case SQLITE_PERM: throw perm_error(message);
	case SQLITE_PROTOCOL: throw protocol_error(message);
	case SQLITE_RANGE: throw range_error(message);
	case SQLITE_READONLY: throw readonly_error(message);
	case SQLITE_ROW: throw row_error(message);
	case SQLITE_SCHEMA: throw schema_error(message);
	case SQLITE_TOOBIG: throw toobig_error(message);
	case SQLITE_WARNING: throw warning_error(message);
	case SQLITE_ABORT_ROLLBACK: throw abort_rollback_error(message);
	case SQLITE_BUSY_RECOVERY: throw busy_recovery_error(message);
	case SQLITE_BUSY_SNAPSHOT: throw busy_snapshot_error(message);
	case SQLITE_CANTOPEN_CONVPATH: throw cantopen_convpath_error(message);
	case SQLITE_CANTOPEN_FULLPATH: throw cantopen_fullpath_error(message);
	case SQLITE_CANTOPEN_ISDIR: throw cantopen_isdir_error(message);
	case SQLITE_CONSTRAINT_CHECK: throw constraint_check_error(message);
	case SQLITE_CONSTRAINT_COMMITHOOK: throw constraint_commithook_error(message);
	case SQLITE_CONSTRAINT_FOREIGNKEY: throw constraint_foreignkey_error(message);
	case SQLITE_CONSTRAINT_FUNCTION: throw constraint_function_error(message);
	case SQLITE_CONSTRAINT_NOTNULL: throw constraint_notnull_error(message);
	case SQLITE_CONSTRAINT_PRIMARYKEY: throw constraint_primarykey_error(message);
	case SQLITE_CONSTRAINT_ROWID: throw constraint_rowid_error(message);
	case SQLITE_CONSTRAINT_TRIGGER: throw constraint_trigger_error(message);
	case SQLITE_CONSTRAINT_UNIQUE: throw constraint_unique_error(message);
	case SQLITE_CONSTRAINT_VTAB: throw constraint_vtab_error(message);
	case SQLITE_CORRUPT_VTAB: throw corrupt_vtab_error(message);
	case SQLITE_IOERR_ACCESS: throw ioerr_access_error(message);
	case SQLITE_IOERR_CHECKRESERVEDLOCK: throw ioerr_checkreservedlock_error(message);
	case SQLITE_IOERR_CLOSE: throw ioerr_close_error(message);
	case SQLITE_IOERR_CONVPATH: throw ioerr_convpath_error(message);
	case SQLITE_IOERR_DELETE: throw ioerr_delete_error(message);
	case SQLITE_IOERR_DELETE_NOENT: throw ioerr_delete_noent_error(message);
	case SQLITE_IOERR_DIR_FSYNC: throw ioerr_dir_fsync_error(message);
	case SQLITE_IOERR_FSTAT: throw ioerr_fstat_error(message);
	case SQLITE_IOERR_FSYNC: throw ioerr_fsync_error(message);
	case SQLITE_IOERR_GETTEMPPATH: throw ioerr_gettemppath_error(message);
	case SQLITE_IOERR_LOCK: throw ioerr_lock_error(message);
	case SQLITE_IOERR_MMAP: throw ioerr_mmap_error(message);
	case SQLITE_IOERR_NOMEM: throw ioerr_nomem_error(message);
	case SQLITE_IOERR_RDLOCK: throw ioerr_rdlock_error(message);
	case SQLITE_IOERR_READ: throw ioerr_read_error(message);
	case SQLITE_IOERR_SEEK: throw ioerr_seek_error(message);
	case SQLITE_IOERR_SHMMAP: throw ioerr_shmmap_error(message);
	case SQLITE_IOERR_SHMOPEN: throw ioerr_shmopen_error(message);
	case SQLITE_IOERR_SHMSIZE: throw ioerr_shmsize_error(message);
	case SQLITE_IOERR_SHORT_READ: throw ioerr_short_read_error(message);
	case SQLITE_IOERR_TRUNCATE: throw ioerr_truncate_error(message);
	case SQLITE_IOERR_UNLOCK: throw ioerr_unlock_error(message);
	case SQLITE_IOERR_WRITE: throw ioerr_write_error(message);
	case SQLITE_LOCKED_SHAREDCACHE: throw locked_sharedcache_error(message);
	case SQLITE_READONLY_CANTLOCK: throw readonly_cantlock_error(message);
	case SQLITE_READONLY_DBMOVED: throw readonly_dbmoved_error(message);
	case SQLITE_READONLY_RECOVERY: throw readonly_recovery_error(message);
	case SQLITE_READONLY_ROLLBACK: throw readonly_rollback_error(message);
	default: throw sqlite_error(message);
	}
}

class local_string {
public:
	local_string(const char* cstr, std::size_t size) {
		if (size < _capacity) {
			_data = _buffer;
		}
		else {
			if (size == std::numeric_limits<std::size_t>::max() || size + 1 > std::size_t(std::numeric_limits<int>::max())) {
				throw std::overflow_error("sqlite3cpp::detail::local_string");
			}
			_data = (char*)sqlite3_malloc(int(size + 1));
		}
		std::memcpy(_data, cstr, size);
		_data[size] = 0;
		_size = size;
	}

	~local_string() {
		if (_buffer != _data) {
			sqlite3_free(_data);
		}
	}

	local_string(local_string&& that) {
		if (that._data == that._buffer) {
			std::memcpy(_buffer, that._buffer, that._size + 1);
			_data = _buffer;
		}
		else {
			_data = that._data;
		}
		_size = that._size;
		that._data = that._buffer;
		that._size = 0;
	}

	local_string& operator=(local_string&& that) {
		if (this != &that) {
			if (_data != _buffer) {
				sqlite3_free(_data);
			}
			if (that._data == that._buffer) {
				std::memcpy(_buffer, that._buffer, that._size + 1);
				_data = _buffer;
			}
			else {
				_data = that._data;
			}
			_size = that._size;
			that._data = that._buffer;
			that._size = 0;
		}
		return *this;
	}

	std::size_t size() const {
		return _size;
	}

	const char* data() const {
		return _data;
	}

	const char* c_str() const {
		return data();
	}
private:
	static const std::size_t _capacity = 1024;
	char _buffer[_capacity];
	char* _data;
	std::size_t _size;

	local_string(const local_string&);
	local_string& operator=(const local_string&);
};

}

inline sqlite3*& impl(database& database) {
	return reinterpret_cast<sqlite3*&>(detail::impl::get(database));
}

inline const sqlite3* impl(const database& database) {
	return reinterpret_cast<const sqlite3*>(detail::impl::get(database));
}

inline sqlite3_stmt*& impl(statement& statement) {
	return reinterpret_cast<sqlite3_stmt*&>(detail::impl::get(statement));
}

inline const sqlite3_stmt* impl(const statement& statement) {
	return reinterpret_cast<const sqlite3_stmt*>(detail::impl::get(statement));
}

inline void throw_exception(sqlite3* database) {
	detail::throw_exception(sqlite3_extended_errcode(database), sqlite3_errmsg(database));
}

inline void throw_exception(database& database) {
	throw_exception(impl(database));
}

inline void throw_exception(statement& statement) {
	throw_exception(sqlite3_db_handle(impl(statement)));
}

const unsigned open_nomutex = SQLITE_OPEN_NOMUTEX;
const unsigned open_fullmutex = SQLITE_OPEN_FULLMUTEX;
const unsigned open_sharedcache = SQLITE_OPEN_SHAREDCACHE;
const unsigned open_privatecache = SQLITE_OPEN_PRIVATECACHE;
const unsigned open_uri = SQLITE_OPEN_URI;
const unsigned open_readonly = SQLITE_OPEN_READONLY;
const unsigned open_readwrite = SQLITE_OPEN_READWRITE;
const unsigned open_create = SQLITE_OPEN_CREATE;

database open(const char* filename, unsigned flags) {
	database database;

	if (sqlite3_open_v2(filename, &impl(database), flags, nullptr) != SQLITE_OK) {
		try {
			throw_exception(database);
		}
		catch (...) {
			close(database);
			throw;
		}
	}

	return database;
}

void close(database& database) {
	if (database) {
		sqlite3_close(impl(database));
		impl(database) = nullptr;
	}
}

statement prepare(database& database, std::string::const_iterator begin, std::string::const_iterator end, std::string::const_iterator& tail) {
	const char* ctail = nullptr;
	statement statement = prepare(database, &*begin, end - begin, ctail);
	tail = begin + (ctail - &*begin);
	return statement;
}

statement prepare(database& database, const char* sql, const char*& tail) {
	return prepare(database, sql, std::strlen(sql), tail);
}

statement prepare(database& database, const char* sql, std::size_t bytes, const char*& tail) {
	statement statement;
	if (database) {
		if (sqlite3_prepare_v2(impl(database), sql, bytes, &impl(statement), &tail) != SQLITE_OK) {
			throw_exception(database);
		}
	}
	else {
		throw std::invalid_argument("database");
	}
	return statement;
}

result_t step(statement& statement) {
	if (statement) {
		switch (sqlite3_step(impl(statement))) {
		case SQLITE_DONE: return done;
		case SQLITE_ROW: return row;
		default: throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
	return done;
}

void finalize(statement& statement) {
	if (statement) {
		sqlite3* database = sqlite3_db_handle(impl(statement));
		if (sqlite3_finalize(impl(statement)) != SQLITE_OK) {
			throw_exception(database);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}




database::database() 
	: _impl(nullptr) {
}

database::database(database&& that)
	: _impl(that._impl) {
	that._impl = nullptr;
}

database::~database() {
	close(*this);
}

database& database::operator=(database&& that) {
	if (this != &that) {
		std::swap(_impl, that._impl);
	}
	return *this;
}

database::operator bool() const {
	return _impl != nullptr;
}

statement::statement()
	: _impl(nullptr) {
}

statement::statement(statement&& that)
	: _impl(that._impl) {
	that._impl = nullptr;
}

statement::~statement() {
	finalize(*this);
}

statement& statement::operator=(statement&& that) {
	if (this != &that) {
		std::swap(_impl, that._impl);
	}
	return *this;
}

statement::operator bool() const {
	return _impl != nullptr;
}

bool exec(database& database, const char* sql, std::size_t size, std::function<bool(std::size_t, const char* const*, const char* const*)> functor) {
	if (database) {
		if (sql == nullptr && size != 0) {
			throw std::invalid_argument("sql");
		}

		struct closure_t {
			closure_t(decltype(functor)& functor)
				: functor(functor) {
			}

			static int invoke(void* _self, int num, char** data, char** cols) {
				auto self = reinterpret_cast<closure_t*>(_self);
				try {
					return int(self->functor(num, data, cols));
				}
				catch (...) {
					self->exception = std::current_exception();
					return 1;
				}
			}

			decltype(functor)& functor;
			std::exception_ptr exception;
		};

		closure_t closure(functor);
		auto lsql = detail::local_string(sql, size);
		char* errmsg = nullptr;
		int result = SQLITE_OK;
		if (functor) {
			result = sqlite3_exec(impl(database), lsql.c_str(), closure_t::invoke, &closure, &errmsg);
		}
		else {
			result = sqlite3_exec(impl(database), lsql.c_str(), nullptr, nullptr, &errmsg);
		}

		if (closure.exception) {
			sqlite3_free(errmsg);
			std::rethrow_exception(closure.exception);
		}
		else if (result == SQLITE_ABORT) {
			return true;
		}
		else if (result != SQLITE_OK) {
			if (errmsg) {
				try {
					detail::throw_exception(SQLITE_ERROR, errmsg);
				}
				catch (...) {
					sqlite3_free(errmsg);
					throw;
				}
			}
			else {
				throw_exception(database);
			}
		}
		else {
			return false;
		}
	}
	else {
		throw std::invalid_argument("database");
	}
	return false;
}

bool exec(database& database, const char* sql, std::function<bool(std::size_t, const char* const*, const char* const*)> functor) {
	return exec(database, sql, std::strlen(sql), functor);
}

bool exec(database& database, const std::string& sql, std::function<bool(std::size_t, const char* const*, const char* const*)> functor) {
	return exec(database, sql.c_str(), sql.size(), functor);
}



sqlite_error::sqlite_error(const char* message)
	: std::runtime_error(message) {
}

abort_error::abort_error(const char* message)
	: sqlite_error(message) {
}

auth_error::auth_error(const char* message)
	: sqlite_error(message) {
}

busy_error::busy_error(const char* message)
	: sqlite_error(message) {
}

cantopen_error::cantopen_error(const char* message)
	: sqlite_error(message) {
}

constraint_error::constraint_error(const char* message)
	: sqlite_error(message) {
}

corrupt_error::corrupt_error(const char* message)
	: sqlite_error(message) {
}

empty_error::empty_error(const char* message)
	: sqlite_error(message) {
}

format_error::format_error(const char* message)
	: sqlite_error(message) {
}

full_error::full_error(const char* message)
	: sqlite_error(message) {
}

internal_error::internal_error(const char* message)
	: sqlite_error(message) {
}

interrupt_error::interrupt_error(const char* message)
	: sqlite_error(message) {
}

ioerr_error::ioerr_error(const char* message)
	: sqlite_error(message) {
}

locked_error::locked_error(const char* message)
	: sqlite_error(message) {
}

mismatch_error::mismatch_error(const char* message)
	: sqlite_error(message) {
}

misuse_error::misuse_error(const char* message)
	: sqlite_error(message) {
}

nolfs_error::nolfs_error(const char* message)
	: sqlite_error(message) {
}

nomem_error::nomem_error(const char* message)
	: sqlite_error(message) {
}

notadb_error::notadb_error(const char* message)
	: sqlite_error(message) {
}

notfound_error::notfound_error(const char* message)
	: sqlite_error(message) {
}

perm_error::perm_error(const char* message)
	: sqlite_error(message) {
}

protocol_error::protocol_error(const char* message)
	: sqlite_error(message) {
}

range_error::range_error(const char* message)
	: sqlite_error(message) {
}

readonly_error::readonly_error(const char* message)
	: sqlite_error(message) {
}

row_error::row_error(const char* message)
	: sqlite_error(message) {
}

schema_error::schema_error(const char* message)
	: sqlite_error(message) {
}

toobig_error::toobig_error(const char* message)
	: sqlite_error(message) {
}

warning_error::warning_error(const char* message)
	: sqlite_error(message) {
}

abort_rollback_error::abort_rollback_error(const char* message)
	: abort_error(message) {
}

busy_recovery_error::busy_recovery_error(const char* message)
	: busy_error(message) {
}

busy_snapshot_error::busy_snapshot_error(const char* message)
	: busy_error(message) {
}

cantopen_convpath_error::cantopen_convpath_error(const char* message)
	: cantopen_error(message) {
}

cantopen_fullpath_error::cantopen_fullpath_error(const char* message)
	: cantopen_error(message) {
}

cantopen_isdir_error::cantopen_isdir_error(const char* message)
	: cantopen_error(message) {
}

constraint_check_error::constraint_check_error(const char* message)
	: constraint_error(message) {
}

constraint_commithook_error::constraint_commithook_error(const char* message)
	: constraint_error(message) {
}

constraint_foreignkey_error::constraint_foreignkey_error(const char* message)
	: constraint_error(message) {
}

constraint_function_error::constraint_function_error(const char* message)
	: constraint_error(message) {
}

constraint_notnull_error::constraint_notnull_error(const char* message)
	: constraint_error(message) {
}

constraint_primarykey_error::constraint_primarykey_error(const char* message)
	: constraint_error(message) {
}

constraint_rowid_error::constraint_rowid_error(const char* message)
	: constraint_error(message) {
}

constraint_trigger_error::constraint_trigger_error(const char* message)
	: constraint_error(message) {
}

constraint_unique_error::constraint_unique_error(const char* message)
	: constraint_error(message) {
}

constraint_vtab_error::constraint_vtab_error(const char* message)
	: constraint_error(message) {
}

corrupt_vtab_error::corrupt_vtab_error(const char* message)
	: corrupt_error(message) {
}

ioerr_access_error::ioerr_access_error(const char* message)
	: ioerr_error(message) {
}

ioerr_checkreservedlock_error::ioerr_checkreservedlock_error(const char* message)
	: ioerr_error(message) {
}

ioerr_close_error::ioerr_close_error(const char* message)
	: ioerr_error(message) {
}

ioerr_convpath_error::ioerr_convpath_error(const char* message)
	: ioerr_error(message) {
}

ioerr_delete_error::ioerr_delete_error(const char* message)
	: ioerr_error(message) {
}

ioerr_delete_noent_error::ioerr_delete_noent_error(const char* message)
	: ioerr_error(message) {
}

ioerr_dir_fsync_error::ioerr_dir_fsync_error(const char* message)
	: ioerr_error(message) {
}

ioerr_fstat_error::ioerr_fstat_error(const char* message)
	: ioerr_error(message) {
}

ioerr_fsync_error::ioerr_fsync_error(const char* message)
	: ioerr_error(message) {
}

ioerr_gettemppath_error::ioerr_gettemppath_error(const char* message)
	: ioerr_error(message) {
}

ioerr_lock_error::ioerr_lock_error(const char* message)
	: ioerr_error(message) {
}

ioerr_mmap_error::ioerr_mmap_error(const char* message)
	: ioerr_error(message) {
}

ioerr_nomem_error::ioerr_nomem_error(const char* message)
	: ioerr_error(message) {
}

ioerr_rdlock_error::ioerr_rdlock_error(const char* message)
	: ioerr_error(message) {
}

ioerr_read_error::ioerr_read_error(const char* message)
	: ioerr_error(message) {
}

ioerr_seek_error::ioerr_seek_error(const char* message)
	: ioerr_error(message) {
}

ioerr_shmmap_error::ioerr_shmmap_error(const char* message)
	: ioerr_error(message) {
}

ioerr_shmopen_error::ioerr_shmopen_error(const char* message)
	: ioerr_error(message) {
}

ioerr_shmsize_error::ioerr_shmsize_error(const char* message)
	: ioerr_error(message) {
}

ioerr_short_read_error::ioerr_short_read_error(const char* message)
	: ioerr_error(message) {
}

ioerr_truncate_error::ioerr_truncate_error(const char* message)
	: ioerr_error(message) {
}

ioerr_unlock_error::ioerr_unlock_error(const char* message)
	: ioerr_error(message) {
}

ioerr_write_error::ioerr_write_error(const char* message)
	: ioerr_error(message) {
}

locked_sharedcache_error::locked_sharedcache_error(const char* message)
	: locked_error(message) {
}

readonly_cantlock_error::readonly_cantlock_error(const char* message)
	: readonly_error(message) {
}

readonly_dbmoved_error::readonly_dbmoved_error(const char* message)
	: readonly_error(message) {
}

readonly_recovery_error::readonly_recovery_error(const char* message)
	: readonly_error(message) {
}

readonly_rollback_error::readonly_rollback_error(const char* message)
	: readonly_error(message) {
}

}
