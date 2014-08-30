#pragma once
#include <stdexcept>
#include <string>
#include <vector>
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

extern const unsigned open_nomutex;
extern const unsigned open_fullmutex;
extern const unsigned open_sharedcache;
extern const unsigned open_privatecache;
extern const unsigned open_uri;
extern const unsigned open_readonly;
extern const unsigned open_readwrite;
extern const unsigned open_create;

database open(const char* filename, unsigned flags = open_readwrite | open_create);
void close(database& database);

statement prepare(database& database, std::string::const_iterator begin, std::string::const_iterator end, std::string::const_iterator& tail);
statement prepare(database& database, const char* sql, const char*& tail);
statement prepare(database& database, const char* sql, std::size_t bytes, const char*& tail);

result_t step(statement& statement);
void finalize(statement& statement);

void clear_bindings(statement& statement);


template <class T> inline void bind(statement& statement, std::size_t index, const T& value) {
	static_assert("Requested type T is invalid.", false);
}

template <> void bind<nullptr_t>(statement& statement, std::size_t index, const nullptr_t& value);
template <> void bind<char>(statement& statement, std::size_t index, const char& value);
template <> void bind<signed char>(statement& statement, std::size_t index, const signed char& value);
template <> void bind<unsigned char>(statement& statement, std::size_t index, const unsigned char& value);
template <> void bind<wchar_t>(statement& statement, std::size_t index, const wchar_t& value);
// template <> void bind<char16_t>(statement& statement, std::size_t index, const char16_t& value);
// template <> void bind<char32_t>(statement& statement, std::size_t index, const char32_t& value);
template <> void bind<short>(statement& statement, std::size_t index, const short& value);
template <> void bind<unsigned short>(statement& statement, std::size_t index, const unsigned short& value);
template <> void bind<int>(statement& statement, std::size_t index, const int& value);
template <> void bind<unsigned int>(statement& statement, std::size_t index, const unsigned int& value);
template <> void bind<long>(statement& statement, std::size_t index, const long& value);
template <> void bind<unsigned long>(statement& statement, std::size_t index, const unsigned long& value);
template <> void bind<long long>(statement& statement, std::size_t index, const long long& value);
template <> void bind<unsigned long long>(statement& statement, std::size_t index, const unsigned long long& value);
template <> void bind<float>(statement& statement, std::size_t index, const float& value);
template <> void bind<double>(statement& statement, std::size_t index, const double& value);
template <> void bind<std::string>(statement& statement, std::size_t index, const std::string& value);
template <> void bind<const char*>(statement& statement, std::size_t index, char const* const& value);

std::size_t bind_parameter_count(statement& statement);
std::size_t bind_parameter_index(statement& statement, const char* name);
std::size_t bind_parameter_index(statement& statement, const std::string& name);
std::string bind_parameter_name_str(statement& statement, std::size_t index);
const char* bind_parameter_name_cstr(statement& statement, std::size_t index);

template <class T> inline T column(statement& statement, std::size_t index) {
	static_assert("Requested type T is invalid.", false);
}

template <> float column<float>(statement& statement, std::size_t index);
template <> double column<double>(statement& statement, std::size_t index);
template <> signed char column<signed char>(statement& statement, std::size_t index);
template <> unsigned char column<unsigned char>(statement& statement, std::size_t index);
template <> short column<short>(statement& statement, std::size_t index);
template <> unsigned short column<unsigned short>(statement& statement, std::size_t index);
template <> int column<int>(statement& statement, std::size_t index);
template <> unsigned int column<unsigned int>(statement& statement, std::size_t index);
template <> long column<long>(statement& statement, std::size_t index);
template <> unsigned long column<unsigned long>(statement& statement, std::size_t index);
template <> long long column<long long>(statement& statement, std::size_t index);
template <> unsigned long long column<unsigned long long>(statement& statement, std::size_t index);
template <> std::string column<std::string>(statement& statement, std::size_t index);

std::string column_str(statement& statement, std::size_t index);
const char* column_cstr(statement& statement, std::size_t index);
std::size_t column_bytes(statement& statement, std::size_t index);

std::size_t column_count(statement& statement);
std::size_t data_count(statement& statement);
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

class database {
public:
	database();
	database(database&& that);
	~database();
	database& operator=(database&& that);
	explicit operator bool() const;
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
	explicit operator bool() const;
private:
	void* _impl;
	friend struct detail::impl;
	statement(const statement&);
	statement& operator=(const statement&);
};

bool exec(database& database, const char* sql, std::size_t size, std::function<bool(std::size_t, const char* const*, const char* const*)> functor);
bool exec(database& database, const char* sql, std::function<bool(std::size_t, const char* const*, const char* const*)> functor);
bool exec(database& database, const std::string& sql, std::function<bool(std::size_t, const char* const*, const char* const*)> functor);

namespace detail {

template <std::size_t NumArgs, std::size_t Index, class... Args> struct column_t {
	static void invoke(std::tuple<Args...>& dest, statement& statement, std::size_t num_cols) {
		if (Index < num_cols) {
			typedef std::remove_reference<decltype(std::get < Index >(dest))>::type type;
			std::get < Index >(dest) = column<type>(statement, Index);
			column_t<NumArgs, Index + 1, Args...>::invoke(dest, statement, num_cols);
		}
	}
};

template <std::size_t NumArgs, class... Args> struct column_t<NumArgs, NumArgs, Args...> {
	static void invoke(std::tuple<Args...>& dest, statement& statement, std::size_t num_cols) {
	}
};

}


namespace detail {

enum tag {
	tag_void,
	tag_nullptr_t,
	tag_bool,
	tag_char,
	tag_schar,
	tag_uchar,
	tag_wchar_t,
	tag_char16_t,
	tag_char32_t,
	tag_short,
	tag_ushort,
	tag_int,
	tag_uint,
	tag_long,
	tag_ulong,
	tag_longlong,
	tag_ulonglong,
	tag_float,
	tag_double,
	tag_string,
	tag_cstring,
	tag_generic
};

template <class T> struct type_tag { static const tag value = tag_generic; };
template <> struct type_tag<void> { static const tag value = tag_void; };
template <> struct type_tag<nullptr_t> { static const tag value = tag_nullptr_t; };
template <> struct type_tag<bool> { static const tag value = tag_bool; };
template <> struct type_tag<char> { static const tag value = tag_char; };
template <> struct type_tag<signed char> { static const tag value = tag_schar; };
template <> struct type_tag<unsigned char> { static const tag value = tag_uchar; };
template <> struct type_tag<wchar_t> { static const tag value = tag_wchar_t; };
// template <> struct type_tag<char16_t> { static const tag value = tag_char16_t; };
// template <> struct type_tag<char32_t> { static const tag value = tag_char32_t; };
template <> struct type_tag<short> { static const tag value = tag_short; };
template <> struct type_tag<unsigned short> { static const tag value = tag_ushort; };
template <> struct type_tag<int> { static const tag value = tag_int; };
template <> struct type_tag<unsigned int> { static const tag value = tag_uint; };
template <> struct type_tag<long> { static const tag value = tag_long; };
template <> struct type_tag<unsigned long> { static const tag value = tag_ulong; };
template <> struct type_tag<long long> { static const tag value = tag_long; };
template <> struct type_tag<unsigned long long> { static const tag value = tag_ulong; };
template <> struct type_tag<float> { static const tag value = tag_float; };
template <> struct type_tag<double> { static const tag value = tag_double; };
template <> struct type_tag<std::string> { static const tag value = tag_string; };
template <> struct type_tag<const char*> { static const tag value = tag_cstring; };

struct param_info {
	template <class T> param_info(const T& param)
		: ptr(&param)
		, tag(type_tag<T>::value)
		, info(typeid(T)) {
	}
	
	void const* const ptr;
	const tag tag;
	const std::type_info& info;
};

template <class... Args> struct bind_t;

template <class T, class... Args> struct bind_t<T, Args...> {
	static void invoke(statement& statement, std::size_t index, const void* param, const std::type_info& info) {
		if (typeid(T) == info) {
			bind<T>(statement, index, *static_cast<const T*>(param));
		}
		else {
			bind_t<Args...>::invoke(statement, index, param, info);
		}
	}
};

template <> struct bind_t<> {
	static void invoke(statement& statement, std::size_t index, const void* param, const std::type_info& info) {
	}
};

void exec1(
	database& database, 
	const char* sql, 
	std::size_t sql_size, 
	const std::function<void(statement&)>& column_callback,
	const std::function<void(statement&, std::size_t, const param_info&)>& bind_callback,
	std::size_t num_params,
	...
	);

template <class F, class... Params>
inline void exec2(
	database& database, 
	const char* sql, 
	std::size_t sql_size, 
	const F& functor,
	const Params&... params
	) {
	auto column_callback = [&](statement& statement) {
		auto state = done;
		do {
			state = step(statement);
			auto count = data_count(statement);
			if (count != 0) {
				functor(statement, count);
			}
		} while (state != done);
	};

	auto bind_callback = [&](statement& statement, std::size_t index, const detail::param_info& param) {
		detail::bind_t<Params...>::invoke(statement, index, param.ptr, param.info);
	};

	detail::exec1(
		database,
		sql,
		sql_size,
		column_callback,
		bind_callback,
		sizeof...(params),
		&detail::param_info(params)...
		);
}

}

template <class... Results, class... Params> 
inline std::tuple<Results...> exec(
	database& database, 
	const char* sql, 
	const Params&... params
	) {
	std::tuple<Results...> result;

	auto callback = [&](statement& statement, std::size_t count) {
		detail::column_t<sizeof...(Results), 0, Results...>::invoke(result, statement, count);
	};

	detail::exec2<decltype(callback), Params...>(
		database,
		sql,
		std::strlen(sql),
		callback,
		params...
		);

	return result;
}

template <class... Results, class... Params>
inline std::tuple<Results...> exec(
	database& database,
	const std::string& sql,
	const Params&... params
	) {
	std::tuple<Results...> result;

	auto callback = [&](statement& statement, std::size_t count) {
		detail::column_t<sizeof...(Results), 0, Results...>::invoke(result, statement, count);
	};

	detail::exec2<decltype(callback), Params...>(
		database,
		sql,
		sql.size(),
		callback,
		params...
		);

	return result;
}

template <class... Results, class... Params>
inline std::vector<std::tuple<Results...> > vexec(
	database& database,
	const char* sql,
	const Params&... params
	) {
	std::vector<std::tuple<Results...> > result;

	auto callback = [&](statement& statement, std::size_t count) {
		result.emplace_back();
		detail::column_t<sizeof...(Results), 0, Results...>::invoke(result.back(), statement, count);
	};

	detail::exec2<decltype(callback), Params...>(
		database,
		sql,
		std::strlen(sql),
		callback,
		params...
		);

	return result;
}

template <class... Results, class... Params>
inline std::vector<std::tuple<Results...> > vexec(
	database& database,
	const std::string& sql,
	const Params&... params
	) {
	std::vector<std::tuple<Results...> > result;

	auto callback = [&](statement& statement, std::size_t count) {
		result.emplace_back();
		detail::column_t<sizeof...(Results), 0, Results...>::invoke(result.back(), statement, count);
	};

	detail::exec2<decltype(callback), Params...>(
		database,
		sql,
		sql.size(),
		callback,
		params...
		);

	return result;
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

namespace sqlt3 = sqlite3cpp;