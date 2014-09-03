#include <sqlite3.hpp>
#include <sqlite3.h>
#include <limits>
#include <cstdio>
#include <cstdarg>

namespace sqlt3 {
namespace detail {

struct impl {
	template <class T> static void*& get(T& x) {
		return x._impl;
	}

	template <class T> static const void* get(const T& x) {
		return x._impl;
	}

	static void throw_exception(int code, const char* message) {
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
	detail::impl::throw_exception(sqlite3_extended_errcode(database), sqlite3_errmsg(database));
}

inline void throw_exception(database& database) {
	throw_exception(impl(database));
}

inline void throw_exception(statement& statement) {
	throw_exception(sqlite3_db_handle(impl(statement)));
}

template <class T, class S> inline T safe_downcast(const S& s) {
	if (s <= static_cast<S>(std::numeric_limits<T>::max())) {
		return static_cast<T>(s);
	}
	else {
		throw std::overflow_error(std::to_string(s));
	}
}

const unsigned open_nomutex = SQLITE_OPEN_NOMUTEX;
const unsigned open_fullmutex = SQLITE_OPEN_FULLMUTEX;
const unsigned open_sharedcache = SQLITE_OPEN_SHAREDCACHE;
const unsigned open_privatecache = SQLITE_OPEN_PRIVATECACHE;
const unsigned open_uri = SQLITE_OPEN_URI;
const unsigned open_readonly = SQLITE_OPEN_READONLY;
const unsigned open_readwrite = SQLITE_OPEN_READWRITE;
const unsigned open_create = SQLITE_OPEN_CREATE;

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

database open(const char* filename, unsigned flags);
database open(const char* filename);
void close(database& database);

database open(const char* filename, unsigned flags) {
	database database;

	auto result = sqlite3_open_v2(filename, &impl(database), flags, nullptr);
	if (result != SQLITE_OK) {
		throw_exception(database);
	}

	return database;
}

database open(const char* filename) {
	return open(filename, open_readwrite | open_create);
}

void close(database& database) {
	if (database) {
		sqlite3_close_v2(impl(database));
		impl(database) = nullptr;
	}
}

statement prepare(
	database& database, 
	const char* sql_begin, 
	const char* sql_end, 
	const char*& tail
	) {
	statement statement;
	if (database) {
		auto result = sqlite3_prepare_v2(
			impl(database),
			sql_begin,
			sql_end - sql_begin,
			&impl(statement),
			&tail
			);
		if (result != SQLITE_OK) {
			throw_exception(database);
		}
	}
	else {
		throw std::invalid_argument("database");
	}
	return statement;
}

statement prepare(
	database& database,
	const char* sql,
	const char*& tail
	) {
	return prepare(
		database,
		sql,
		sql + std::strlen(sql),
		tail);
}

statement prepare(
	database& database,
	string::const_iterator sql_begin,
	string::const_iterator sql_end,
	string::const_iterator& tail
	) {

	const char* begin = &*sql_begin;
	const char* end = &*sql_end;
	const char* _tail = nullptr;

	auto result = prepare(
		database,
		begin,
		end,
		_tail);

	tail = sql_begin + (_tail - begin);

	return result;
}

outcome step(statement& statement) {
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
		impl(statement) = nullptr;
	}
}

template <class T> 
inline void bind_int(
	statement& statement, 
	size_t index, 
	T value
	) {
	if (statement) {
		auto _index = safe_downcast<int>(index);
		sqlite3_bind_int(impl(statement), _index, value);
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <class T>
inline void bind_int64(
	statement& statement,
	size_t index,
	T value
	) {
	if (statement) {
		auto _index = safe_downcast<int>(index);
		sqlite3_bind_int64(impl(statement), _index, value);
	}
	else {
		throw std::invalid_argument("statement");
	}
}

void bind(statement& statement, size_t index, nullptr_t value) {
	if (statement) {
		auto _index = safe_downcast<int>(index);
		auto result = sqlite3_bind_null(impl(statement), _index);
		if (result != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

void bind(statement& statement, size_t index, char value) {
	return bind_int(statement, index, value);
}

void bind(statement& statement, size_t index, signed char value) {
	return bind_int(statement, index, value);
}

void bind(statement& statement, size_t index, unsigned char value) {
	return bind_int(statement, index, value);
}

void bind(statement& statement, size_t index, wchar_t value) {
	return bind_int64(statement, index, value);
}

/*void bind(statement& statement, size_t index, char16_t value) {
	return bind_int(statement, index, value);
}

void bind(statement& statement, size_t index, char32_t value) {
	return bind_int64(statement, index, value);
}*/

void bind(statement& statement, size_t index, short value) {
	return bind_int(statement, index, value);
}

void bind(statement& statement, size_t index, unsigned short value) {
	return bind_int(statement, index, value);
}

void bind(statement& statement, size_t index, int value) {
	return bind_int(statement, index, value);
}

void bind(statement& statement, size_t index, unsigned int value) {
	return bind_int64(statement, index, value);
}

void bind(statement& statement, size_t index, long value) {
	return bind_int64(statement, index, value);
}

void bind(statement& statement, size_t index, unsigned long value) {
	return bind_int64(statement, index, safe_downcast<long long>(value));
}

void bind(statement& statement, size_t index, long long value) {
	return bind_int64(statement, index, value);
}

void bind(statement& statement, size_t index, unsigned long long value) {
	return bind_int64(statement, index, safe_downcast<long long>(value));
}

void bind(statement& statement, size_t index, float value) {
	if (statement) {
		auto _index = safe_downcast<int>(index);
		auto result = sqlite3_bind_double(impl(statement), _index, value);
		if (result != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

void bind(statement& statement, size_t index, double value) {
	if (statement) {
		auto _index = safe_downcast<int>(index);
		auto result = sqlite3_bind_double(impl(statement), _index, value);
		if (result != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

void bind(statement& statement, size_t index, long double value) {
	if (statement) {
		auto _index = safe_downcast<int>(index);
		auto result = sqlite3_bind_double(impl(statement), _index, double(value));
		if (result != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

void bind(statement& statement, size_t index, const string& value) {
	if (statement) {
		auto _index = safe_downcast<int>(index);
		auto result = sqlite3_bind_text(
			impl(statement),
			_index,
			value.data(),
			value.size(),
			nullptr
			);
		if (result != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

void bind(statement& statement, size_t index, const char* value) {
	if (statement) {
		auto _index = safe_downcast<int>(index);
		auto result = sqlite3_bind_text(
			impl(statement),
			_index,
			value,
			std::strlen(value),
			nullptr
			);
		if (result != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

void clear_bindings(statement& statement) {
	if (statement) {
		if (sqlite3_clear_bindings(impl(statement)) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

size_t bind_parameter_count(statement& statement) {
	if (statement) {
		auto result = sqlite3_bind_parameter_count(impl(statement));
		return static_cast<size_t>(result);
	}
	else {
		throw std::invalid_argument("statement");
	}
}

size_t bind_parameter_index(statement& statement, const char* name) {
	if (statement) {
		auto result = sqlite3_bind_parameter_index(impl(statement), name);
		return static_cast<size_t>(result);
	}
	else {
		throw std::invalid_argument("statement");
	}
}

size_t bind_parameter_index(statement& statement, const string& name) {
	return bind_parameter_index(statement, name.c_str());
}

const char* bind_parameter_name(statement& statement, size_t index) {
	if (statement) {
		auto _index = safe_downcast<int>(index);
		return sqlite3_bind_parameter_name(impl(statement), _index);
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <class T> inline T column_int64(statement& statement, size_t index) {
	if (statement) {
		auto count = sqlite3_column_count(impl(statement));
		if (index < static_cast<size_t>(count)) {
			const auto _min = static_cast<long long>(
				std::numeric_limits<T>::min()
				);

			const auto _max = static_cast<long long>(
				std::numeric_limits<T>::max()
				);

			auto result = sqlite3_column_int64(
				impl(statement),
				static_cast<int>(index)
				);

			if (result < _min) {
				throw std::underflow_error(std::to_string(result));
			}
			if (result > _max) {
				throw std::overflow_error(std::to_string(result));
			}

			return static_cast<T>(result);
		}
		else {
			throw std::invalid_argument("index");
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

char column_char(statement& statement, size_t index) {
	return column_int64<char>(statement, index);
}

signed char column_schar(statement& statement, size_t index) {
	return column_int64<signed char>(statement, index);
}

unsigned char column_uchar(statement& statement, size_t index) {
	return column_int64<unsigned char>(statement, index);
}

wchar_t column_wchar_t(statement& statement, size_t index) {
	return column_int64<wchar_t>(statement, index);
}

char16_t column_char16(statement& statement, size_t index) {
	return column_int64<char16_t>(statement, index);
}

char32_t column_char32(statement& statement, size_t index) {
	return column_int64<char32_t>(statement, index);
}

short column_short(statement& statement, size_t index) {
	return column_int64<short>(statement, index);
}

unsigned short column_ushort(statement& statement, size_t index) {
	return column_int64<unsigned short>(statement, index);
}

int column_int(statement& statement, size_t index) {
	return column_int64<int>(statement, index);
}

unsigned int column_uint(statement& statement, size_t index) {
	return column_int64<unsigned int>(statement, index);
}

long column_long(statement& statement, size_t index) {
	return column_int64<long>(statement, index);
}

unsigned long column_ulong(statement& statement, size_t index) {
	return column_int64<unsigned long>(statement, index);
}

long long column_llong(statement& statement, size_t index) {
	return column_int64<long long>(statement, index);
}

unsigned long long column_ullong(statement& statement, size_t index) {
	return column_int64<unsigned long long>(statement, index);
}

float column_float(statement& statement, size_t index) {
	auto result = column_double(statement, index);
	return static_cast<float>(result);
}

double column_double(statement& statement, size_t index) {
	if (statement) {
		auto count = sqlite3_column_count(impl(statement));
		if (index <  static_cast<size_t>(count)) {
			auto _index = static_cast<int>(index);
			return sqlite3_column_double(impl(statement), _index);
		}
		else {
			throw std::invalid_argument("index");
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

long double column_ldouble(statement& statement, size_t index) {
	return column_double(statement, index);
}

const char* column_string(statement& statement, size_t index) {
	if (statement) {
		auto count = sqlite3_column_count(impl(statement));
		if (index <  static_cast<size_t>(count)) {
			auto _index = static_cast<int>(index);
			auto result = sqlite3_column_text(impl(statement), _index);
			return reinterpret_cast<const char*>(result);
		}
		else {
			throw std::invalid_argument("index");
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

size_t column_bytes(statement& statement, size_t index) {
	if (statement) {
		if (index < column_count(statement)) {
			auto _index = static_cast<int>(index);
			return sqlite3_column_bytes(impl(statement), _index);
		}
		else {
			throw std::invalid_argument("index");
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

size_t column_count(statement& statement) {
	if (statement) {
		return sqlite3_column_count(impl(statement));
	}
	else {
		throw std::invalid_argument("statement");
	}
}

size_t data_count(statement& statement) {
	if (statement) {
		return sqlite3_data_count(impl(statement));
	}
	else {
		throw std::invalid_argument("statement");
	}
}

namespace detail {

inline void bind(
	statement& statement, 
	std::size_t index, 
	const param_info& param_info
	) {
	switch (param_info.tag) {
	case tag_nullptr_t: sqlt3::bind(statement, index, *static_cast<const nullptr_t*>(param_info.ptr)); break;
	case tag_char: sqlt3::bind(statement, index, *static_cast<const char*>(param_info.ptr)); break;
	case tag_schar: sqlt3::bind(statement, index, *static_cast<const signed char*>(param_info.ptr)); break;
	case tag_uchar: sqlt3::bind(statement, index, *static_cast<const unsigned char*>(param_info.ptr)); break;
	case tag_wchar_t: sqlt3::bind(statement, index, *static_cast<const wchar_t*>(param_info.ptr)); break;
	case tag_char16_t: sqlt3::bind(statement, index, *static_cast<const char16_t*>(param_info.ptr)); break;
	case tag_char32_t: sqlt3::bind(statement, index, *static_cast<const char32_t*>(param_info.ptr)); break;
	case tag_short: sqlt3::bind(statement, index, *static_cast<const short*>(param_info.ptr)); break;
	case tag_ushort: sqlt3::bind(statement, index, *static_cast<const unsigned short*>(param_info.ptr)); break;
	case tag_int: sqlt3::bind(statement, index, *static_cast<const int*>(param_info.ptr)); break;
	case tag_uint: sqlt3::bind(statement, index, *static_cast<const unsigned int*>(param_info.ptr)); break;
	case tag_long: sqlt3::bind(statement, index, *static_cast<const long*>(param_info.ptr)); break;
	case tag_ulong: sqlt3::bind(statement, index, *static_cast<const unsigned long*>(param_info.ptr)); break;
	case tag_longlong: sqlt3::bind(statement, index, *static_cast<const long long*>(param_info.ptr)); break;
	case tag_ulonglong: sqlt3::bind(statement, index, *static_cast<const unsigned long long*>(param_info.ptr)); break;
	case tag_float: sqlt3::bind(statement, index, *static_cast<const float*>(param_info.ptr)); break;
	case tag_double: sqlt3::bind(statement, index, *static_cast<const double*>(param_info.ptr)); break;
	case tag_string: sqlt3::bind(statement, index, *static_cast<const std::string*>(param_info.ptr)); break;
	case tag_cstring: sqlt3::bind(statement, index, static_cast<const char*>(param_info.ptr)); break;
	}
}

void exec_base(
	database& database,
	const char* sql_begin,
	const char* sql_end,
	const std::function<void(statement&)>& callback,
	std::size_t num_params,
	...
	) {
	if (database) {
		if (sql_begin != nullptr || sql_end - sql_begin == 0) {
			va_list _va_list;
			va_start(_va_list, num_params);
			std::size_t param_num = 0;
			try {
				const char* itr = sql_begin;
				const char* end = sql_end;
				while (itr < end) {
					statement statement = prepare(database, itr, end, itr);
					std::size_t bind_count = bind_parameter_count(statement);
					for (std::size_t i = 0; i < bind_count && param_num < num_params; ++i) {
						auto param_info = va_arg(_va_list, detail::param_info*);
						detail::bind(statement, i + 1, *param_info);
						++param_num;
					}

					callback(statement);
				}
			}
			catch (...) {
				va_end(_va_list);
				throw;
			}
		}
		else {
			throw std::invalid_argument("sql");
		}
	}
	else {
		throw std::invalid_argument("database");
	}
}

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
