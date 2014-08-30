#include <sqlite3.hpp>
#include <sqlite3.h>
#include <limits>
#include <cstdio>
#include <cstdarg>

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
		impl(statement) = nullptr;
	}
}

template <> void bind<nullptr_t>(statement& statement, std::size_t index, const nullptr_t& value) {
	if (statement) {
		if (sqlite3_bind_null(impl(statement), safe_downcast<int>(index)) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<char>(statement& statement, std::size_t index, const char& value) {
	if (statement) {
		if (sqlite3_bind_int(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<signed char>(statement& statement, std::size_t index, const signed char& value) {
	if (statement) {
		if (sqlite3_bind_int(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<unsigned char>(statement& statement, std::size_t index, const unsigned char& value) {
	if (statement) {
		if (sqlite3_bind_int(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<wchar_t>(statement& statement, std::size_t index, const wchar_t& value) {
	if (statement) {
		if (sqlite3_bind_int64(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

/* template <> void bind<char16_t>(statement& statement, std::size_t index, const char16_t& value) {
	if (statement) {
		if (sqlite3_bind_int(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
		throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}*/

/* template <> void bind<char32_t>(statement& statement, std::size_t index, const char32_t& value) {
	if (statement) {
		if (sqlite3_bind_int64(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
		throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}*/

template <> void bind<short>(statement& statement, std::size_t index, const short& value) {
	if (statement) {
		if (sqlite3_bind_int(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<unsigned short>(statement& statement, std::size_t index, const unsigned short& value) {
	if (statement) {
		if (sqlite3_bind_int(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<int>(statement& statement, std::size_t index, const int& value) {
	if (statement) {
		if (sqlite3_bind_int(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<unsigned int>(statement& statement, std::size_t index, const unsigned int& value) {
	if (statement) {
		if (sqlite3_bind_int64(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<long>(statement& statement, std::size_t index, const long& value) {
	if (statement) {
		if (sqlite3_bind_int64(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<unsigned long>(statement& statement, std::size_t index, const unsigned long& value) {
	if (statement) {
		if (sqlite3_bind_int64(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<long long>(statement& statement, std::size_t index, const long long& value) {
	if (statement) {
		if (sqlite3_bind_int64(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<unsigned long long>(statement& statement, std::size_t index, const unsigned long long& value) {
	if (statement) {
		if (sqlite3_bind_int64(impl(statement), safe_downcast<int>(index), safe_downcast<long long>(value)) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<float>(statement& statement, std::size_t index, const float& value) {
	if (statement) {
		if (sqlite3_bind_double(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<double>(statement& statement, std::size_t index, const double& value) {
	if (statement) {
		if (sqlite3_bind_double(impl(statement), safe_downcast<int>(index), value) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<std::string>(statement& statement, std::size_t index, const std::string& value) {
	if (statement) {
		if (sqlite3_bind_text(impl(statement), safe_downcast<int>(index), value.c_str(), safe_downcast<int>(value.size()), nullptr) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> void bind<const char*>(statement& statement, std::size_t index, char const* const& value) {
	if (statement) {
		if (sqlite3_bind_text(impl(statement), safe_downcast<int>(index), value, safe_downcast<int>(std::strlen(value)), nullptr) != SQLITE_OK) {
			throw_exception(statement);
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

std::size_t bind_parameter_count(statement& statement) {
	if (statement) {
		return static_cast<std::size_t>(sqlite3_bind_parameter_count(impl(statement)));
	}
	else {
		throw std::invalid_argument("statement");
	}
}


template <> float column<float>(statement& statement, std::size_t index) {
	return static_cast<float>(column<double>(statement, index));
}

template <> double column<double>(statement& statement, std::size_t index) {
	if (!statement) {
		throw std::invalid_argument("statement");
	}
	if (column_count(statement) <= index) {
		throw std::invalid_argument("index");
	}

	return sqlite3_column_double(impl(statement), index);
}

namespace detail {

template <class T> inline T checked_column(statement& statement, std::size_t index) {
	static const std::size_t buffer_size = 20;
	char buffer[buffer_size + 1];
	auto format_message = [&](long long value) -> const char* {
		std::sprintf(buffer, "%lld", value);
		return buffer;
	};

	if (statement) {
		if (index < column_count(statement)) {
			long long result = column<long long>(statement, index);
			if (result < static_cast<long long>(std::numeric_limits<T>::min())) {
				throw std::underflow_error(format_message(result));
			}
			if (result > static_cast<long long>(std::numeric_limits<T>::max())) {
				throw std::overflow_error(format_message(result));
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

}

template <> signed char column<signed char>(statement& statement, std::size_t index) {
	return detail::checked_column<signed char>(statement, index);
}

template <> unsigned char column<unsigned char>(statement& statement, std::size_t index) {
	return detail::checked_column<unsigned char>(statement, index);
}

template <> short column<short>(statement& statement, std::size_t index) {
	return detail::checked_column<short>(statement, index);
}

template <> unsigned short column<unsigned short>(statement& statement, std::size_t index) {
	return detail::checked_column<unsigned short>(statement, index);
}

template <> int column<int>(statement& statement, std::size_t index) {
	return detail::checked_column<int>(statement, index);
}

template <> unsigned int column<unsigned int>(statement& statement, std::size_t index) {
	return detail::checked_column<unsigned int>(statement, index);
}

template <> long column<long>(statement& statement, std::size_t index) {
	return detail::checked_column<long>(statement, index);
}

template <> unsigned long column<unsigned long>(statement& statement, std::size_t index) {
	return detail::checked_column<unsigned long>(statement, index);
}

template <> long long column<long long>(statement& statement, std::size_t index) {
	if (statement) {
		if (index < column_count(statement)) {
			return sqlite3_column_int64(impl(statement), static_cast<int>(index));
		}
		else {
			throw std::invalid_argument("index");
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> unsigned long long column<unsigned long long>(statement& statement, std::size_t index) {
	static const std::size_t buffer_size = 20;
	char buffer[buffer_size + 1];
	auto format_message = [&](long long value) -> const char* {
		std::sprintf(buffer, "%lld", value);
		return buffer;
	};

	if (statement) {
		if (index < column_count(statement)) {
			long long result = column<long long>(statement, index);
			if (result < 0) {
				throw std::underflow_error(format_message(result));
			}

			return static_cast<unsigned long long>(result);
		}
		else {
			throw std::invalid_argument("index");
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

template <> std::string column<std::string>(statement& statement, std::size_t index) {
	if (statement) {
		if (index < column_count(statement)) {
			auto cstr = sqlite3_column_text(impl(statement), static_cast<int>(index));
			auto size = sqlite3_column_bytes(impl(statement), static_cast<int>(index));
			return std::string(cstr, cstr + size);
		}
		else {
			throw std::invalid_argument("index");
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

std::string column_str(statement& statement, std::size_t index) {
	return column<std::string>(statement, index);
}

const char* column_cstr(statement& statement, std::size_t index) {
	if (statement) {
		if (index < column_count(statement)) {
			auto result = sqlite3_column_text(impl(statement), static_cast<int>(index));
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

std::size_t column_bytes(statement& statement, std::size_t index) {
	if (statement) {
		if (index < column_count(statement)) {
			return sqlite3_column_bytes(impl(statement), static_cast<int>(index));
		}
		else {
			throw std::invalid_argument("index");
		}
	}
	else {
		throw std::invalid_argument("statement");
	}
}

std::size_t column_count(statement& statement) {
	if (statement) {
		return sqlite3_column_count(impl(statement));
	}
	else {
		throw std::invalid_argument("statement");
	}
}

std::size_t data_count(statement& statement) {
	if (statement) {
		return sqlite3_data_count(impl(statement));
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

namespace detail {

inline void bind(
	statement& statement, 
	std::size_t index, 
	const param_info& param_info, 
	const std::function<void(sqlt3::statement&, std::size_t, const detail::param_info&)>& bind_callback) {
	switch (param_info.tag) {
	case tag_nullptr_t: sqlt3::bind<nullptr_t>(statement, index, *static_cast<const nullptr_t*>(param_info.ptr)); break;
	case tag_char: sqlt3::bind<char>(statement, index, *static_cast<const char*>(param_info.ptr)); break;
	case tag_schar: sqlt3::bind<signed char>(statement, index, *static_cast<const signed char*>(param_info.ptr)); break;
	case tag_uchar: sqlt3::bind<unsigned char>(statement, index, *static_cast<const unsigned char*>(param_info.ptr)); break;
	case tag_wchar_t: sqlt3::bind<wchar_t>(statement, index, *static_cast<const wchar_t*>(param_info.ptr)); break;
	case tag_char16_t: sqlt3::bind<char16_t>(statement, index, *static_cast<const char16_t*>(param_info.ptr)); break;
	case tag_char32_t: sqlt3::bind<char32_t>(statement, index, *static_cast<const char32_t*>(param_info.ptr)); break;
	case tag_short: sqlt3::bind<short>(statement, index, *static_cast<const short*>(param_info.ptr)); break;
	case tag_ushort: sqlt3::bind<unsigned short>(statement, index, *static_cast<const unsigned short*>(param_info.ptr)); break;
	case tag_int: sqlt3::bind<int>(statement, index, *static_cast<const int*>(param_info.ptr)); break;
	case tag_uint: sqlt3::bind<unsigned int>(statement, index, *static_cast<const unsigned int*>(param_info.ptr)); break;
	case tag_long: sqlt3::bind<long>(statement, index, *static_cast<const long*>(param_info.ptr)); break;
	case tag_ulong: sqlt3::bind<unsigned long>(statement, index, *static_cast<const unsigned long*>(param_info.ptr)); break;
	case tag_longlong: sqlt3::bind<long long>(statement, index, *static_cast<const long long*>(param_info.ptr)); break;
	case tag_ulonglong: sqlt3::bind<unsigned long long>(statement, index, *static_cast<const unsigned long long*>(param_info.ptr)); break;
	case tag_float: sqlt3::bind<float>(statement, index, *static_cast<const float*>(param_info.ptr)); break;
	case tag_double: sqlt3::bind<double>(statement, index, *static_cast<const double*>(param_info.ptr)); break;
	case tag_string: sqlt3::bind<std::string>(statement, index, *static_cast<const std::string*>(param_info.ptr)); break;
	case tag_cstring: sqlt3::bind<const char*>(statement, index, *static_cast<const char* const*>(param_info.ptr)); break;
	case tag_generic: bind_callback(statement, index, param_info); break;
	}
}

void exec1(
	database& database,
	const char* sql,
	std::size_t sql_size,
	const std::function<void(statement&)>& column_callback,
	const std::function<void(statement&, std::size_t, const param_info&)>& bind_callback,
	std::size_t num_params,
	...
	) {
	if (database) {
		if (sql != nullptr || sql_size == 0) {
			va_list _va_list;
			va_start(_va_list, num_params);
			std::size_t param_num = 0;
			try {
				const char* itr = sql;
				const char* end = sql + sql_size;
				while (itr < end) {
					statement statement = prepare(database, itr, end - itr, itr);
					std::size_t bind_count = bind_parameter_count(statement);
					for (std::size_t i = 0; i < bind_count && param_num < num_params; ++i) {
						auto param_info = va_arg(_va_list, detail::param_info*);
						detail::bind(statement, i + 1, *param_info, bind_callback);
						++param_num;
					}

					column_callback(statement);
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
