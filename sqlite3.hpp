#pragma once
#include <string>

#include <stdexcept>

#include <vector>
#include <functional>

namespace sqlt3 {

extern const unsigned open_nomutex;
extern const unsigned open_fullmutex;
extern const unsigned open_sharedcache;
extern const unsigned open_privatecache;
extern const unsigned open_uri;
extern const unsigned open_readonly;
extern const unsigned open_readwrite;
extern const unsigned open_create;

enum outcome {
	row, done
};

namespace detail {
	struct impl;
}

using std::string;
using std::size_t;

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

database open(const char* filename, unsigned flags);
database open(const char* filename);
void close(database& database);

statement prepare(database& database, const char* sql_begin, const char* sql_end, const char*& tail);
statement prepare(database& database, const char* sql, const char*& tail);
statement prepare(database& database, string::const_iterator sql_begin, string::const_iterator sql_end, string::const_iterator& tail);
outcome step(statement& statement);
void finalize(statement& statement);

void bind(statement& statement, size_t index, nullptr_t value);
void bind(statement& statement, size_t index, char value);
void bind(statement& statement, size_t index, signed char value);
void bind(statement& statement, size_t index, unsigned char value);
void bind(statement& statement, size_t index, wchar_t value);
// void bind(statement& statement, size_t index, char16_t value);
// void bind(statement& statement, size_t index, char32_t value);
void bind(statement& statement, size_t index, short value);
void bind(statement& statement, size_t index, unsigned short value);
void bind(statement& statement, size_t index, int value);
void bind(statement& statement, size_t index, unsigned int value);
void bind(statement& statement, size_t index, long value);
void bind(statement& statement, size_t index, unsigned long value);
void bind(statement& statement, size_t index, long long value);
void bind(statement& statement, size_t index, unsigned long long value);
void bind(statement& statement, size_t index, float value);
void bind(statement& statement, size_t index, double value);
void bind(statement& statement, size_t index, long double value);
void bind(statement& statement, size_t index, const string& value);
void bind(statement& statement, size_t index, const char* value);

void clear_bindings(statement& statement);
size_t bind_parameter_count(statement& statement);
size_t bind_parameter_index(statement& statement, const char* name);
size_t bind_parameter_index(statement& statement, const string& name);
const char* bind_parameter_name(statement& statement, size_t index);

char column_char(statement& statement, size_t index);
signed char column_schar(statement& statement, size_t index);
unsigned char column_uchar(statement& statement, size_t index);
wchar_t column_wchar_t(statement& statement, size_t index);
char16_t column_char16(statement& statement, size_t index);
char32_t column_char32(statement& statement, size_t index);
short column_short(statement& statement, size_t index);
unsigned short column_ushort(statement& statement, size_t index);
int column_int(statement& statement, size_t index);
unsigned int column_uint(statement& statement, size_t index);
long column_long(statement& statement, size_t index);
unsigned long column_ulong(statement& statement, size_t index);
long long column_llong(statement& statement, size_t index);
unsigned long long column_ullong(statement& statement, size_t index);
float column_float(statement& statement, size_t index);
double column_double(statement& statement, size_t index);
long double column_ldouble(statement& statement, size_t index);
const char* column_string(statement& statement, size_t index);
size_t column_bytes(statement& statement, size_t index);

size_t column_count(statement& statement);
size_t data_count(statement& statement);
const char* column_type(statement& statement, size_t index);
const char* column_decltype(statement& statement, size_t index);
const char* column_name(statement& statement, size_t index);
const char* column_origin_name(statement& statement, size_t index);
const char* column_table_name(statement& statement, size_t index);
const char* column_database_name(statement& statement, size_t index);

template <class T> T column(statement& statement, size_t index);

template <> inline char column<char>(statement& statement, size_t index) {
	return column_char(statement, index);
}

template <> inline signed char column<signed char>(statement& statement, size_t index) {
	return column_schar(statement, index);
}

template <> inline unsigned char column<unsigned char>(statement& statement, size_t index) {
	return column_uchar(statement, index);
}

template <> inline wchar_t column<wchar_t>(statement& statement, size_t index) {
	return column_wchar_t(statement, index);
}
/*
template <> inline char16_t column<char16_t>(statement& statement, size_t index) {
	return column_char16(statement, index);
}

template <> inline char32_t column<char32_t>(statement& statement, size_t index) {
	return column_char32(statement, index);
}
*/
template <> inline short column<short>(statement& statement, size_t index) {
	return column_short(statement, index);
}

template <> inline unsigned short column<unsigned short>(statement& statement, size_t index) {
	return column_ushort(statement, index);
}

template <> inline int column<int>(statement& statement, size_t index) {
	return column_int(statement, index);
}

template <> inline unsigned int column<unsigned int>(statement& statement, size_t index) {
	return column_uint(statement, index);
}

template <> inline long column<long>(statement& statement, size_t index) {
	return column_long(statement, index);
}

template <> inline unsigned long column<unsigned long>(statement& statement, size_t index) {
	return column_ulong(statement, index);
}

template <> inline long long column<long long>(statement& statement, size_t index) {
	return column_llong(statement, index);
}

template <> inline unsigned long long column<unsigned long long>(statement& statement, size_t index) {
	return column_ullong(statement, index);
}

template <> inline float column<float>(statement& statement, size_t index) {
	return column_float(statement, index);
}

template <> inline double column<double>(statement& statement, size_t index) {
	return column_double(statement, index);
}

template <> inline long double column<long double>(statement& statement, size_t index) {
	return column_ldouble(statement, index);
}

template <> inline const char* column<const char*>(statement& statement, size_t index) {
	return column_string(statement, index);
}

template <> inline string column<string>(statement& statement, size_t index) {
	return column_string(statement, index);
}

namespace detail {

enum tag {
	tag_void,
	tag_nullptr_t,
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
	tag_unsupported
};

template <class T> struct type_tag { static const tag value = tag_unsupported; };
template <> struct type_tag<void> { static const tag value = tag_void; };
template <> struct type_tag<nullptr_t> { static const tag value = tag_nullptr_t; };
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
template <> struct type_tag<long long> { static const tag value = tag_longlong; };
template <> struct type_tag<unsigned long long> { static const tag value = tag_ulonglong; };
template <> struct type_tag<float> { static const tag value = tag_float; };
template <> struct type_tag<double> { static const tag value = tag_double; };
template <> struct type_tag<std::string> { static const tag value = tag_string; };
template <> struct type_tag<const char*> { static const tag value = tag_cstring; };
template <std::size_t N> struct type_tag<const char [N]> { static const tag value = tag_cstring; };

struct param_info {
	template <class T> param_info(const T& param)
		: ptr(&param)
		, tag(type_tag<T>::value) {
	}
	
	void const* const ptr;
	const tag tag;
};

void exec_base(
	database& database, 
	const char* sql_begin, 
	const char* sql_end,
	const std::function<void(statement&)>& callback,
	std::size_t num_params,
	...
	);

template<size_t...>
struct seq { };

template<int N, size_t... S>
struct genseq : genseq<N - 1, N - 1, S...> {
};

template<size_t... S>
struct genseq<0, S...> {
	typedef seq<S...> type;
};

template <class... Args, std::size_t... S, class F>
void _apply(
	seq<S...>,
	statement& statement, 
	const F& functor
	) {
	functor(column<typename std::tuple_element<S, std::tuple<Args...> >::type>(statement, S)...);
}

template <class... Args, class F>
void apply(
	statement& statement,
	const F& functor
	) {
	_apply<Args...>(genseq<sizeof...(Args)>::type(), statement, functor);
}

template <class... Results, class F, class... Params>
inline void exec(
	database& database, 
	const char* sql_begin, 
	const char* sql_end,
	const F& functor,
	const Params&... params
	) {
	auto callback = [&](statement& statement) {
		auto state = done;
		do {
			state = step(statement);
			auto count = data_count(statement);
			if (count != 0) {
				apply<Results...>(statement, functor);
			}
		} while (state != done);
	};

	detail::exec_base(
		database,
		sql_begin,
		sql_end,
		callback,
		sizeof...(params),
		&detail::param_info(params)...
		);
}

template <class... List> struct are_supported;

template <class... List> struct are_supported < std::tuple<List...> > {
	static const bool value = are_supported<List...>::value;
};

template <class T> struct are_supported < std::vector<T> > {
	static const bool value = are_supported<T>::value;
};

template <class Head, class... Tail>
struct are_supported < Head, Tail... > {
	static const bool value = type_tag<Head>::value != tag_unsupported && are_supported<Tail...>::value;
};
	
template <>
struct are_supported<> {
	static const bool value = true;
};

template <class F, class... Args>
struct args_matches {
	struct dummy_t {};
	template <class T = F> static auto test(F functor) -> decltype(functor(*(Args*)(0)...), std::true_type());
	template <class T = F> static auto test(...) -> std::false_type;


	typedef decltype(test(*(F*)(0))) type;
	static const bool value = type::value;
};

template <class... Results> struct exec_t { };

template <> struct exec_t < void > {
	template <class... Params> static void invoke(
		database& database,
		const char* sql,
		const Params&... params
		) {

		detail::exec<>(
			database,
			sql,
			sql + std::strlen(sql),
			[]() { },
			params...
			);
	}

	template <class... Params> static void invoke(
		database& database,
		const string& sql,
		const Params&... params
		) {

		detail::exec<>(
			database,
			sql.data(),
			sql.data() + sql.size(),
			[]() {},
			params...
			);
	}
};

template <class T> struct exec_t < T > {
	template <class... Params> static T invoke(
		database& database, 
		const char* sql, 
		const Params&... params
		) {
		T result;

		detail::exec<T>(
			database, 
			sql,
			sql + std::strlen(sql),
			[&](const T& result2) {
				result = result2;
			},
			params...
			);

		return result;
	}

	template <class... Params> static T invoke(
		database& database,
		const string& sql,
		const Params&... params
		) {
		T result;

		detail::exec<T>(
			database,
			sql.data(),
			sql.data() + sql.size(),
			[&](const T& result2) {
				result = result2;
			},
			params...
			);

		return result;
	}
};

template <class... List> struct exec_t < std::tuple<List...> > {
	template <class... Params> static std::tuple<List...> invoke(
		database& database,
		const char* sql,
		const Params&... params
		) {
		std::tuple<List...> results;

		exec<List...>(
			database,
			sql,
			sql + std::strlen(sql),
			[&](const List&... results2) {
				results = std::tuple<List...>(results2...);
			},
			params...
			);

		return results;
	}
};

template <class T> struct exec_t < std::vector<T> > {
	template <class... Params> static std::vector<T> invoke(
		database& database,
		const char* sql,
		const Params&... params
		) {
		std::vector<T> results;

		exec<T>(
			database,
			sql,
			sql + std::strlen(sql),
			[&](const T& result) {
				results.push_back(result);
			},
			params...
			);

		return results;
	}
};

}

template <class... Results, class... Params, class F>
inline void execf(
	database& database,
	const char* sql,
	F functor,
	const Params&... params
	) {
	static_assert(detail::are_supported<Results...>::value, "Types of result (Results) must by fundamental types, std::string or const char*.");
	static_assert(detail::are_supported<Params...>::value, "Types of parameters (Params) must by fundamental types, std::string or const char*.");
	static_assert(detail::args_matches<F, Results...>::value, "The given parameter list of functor doesn't math with the Results parameter pack.");

	detail::exec<Results...>(
		database, 
		sql, 
		sql + std::strlen(sql), 
		functor, 
		params...
		);
}

template <class... Results, class... Params, class F>
inline void execf(
	database& database,
	const string& sql,
	F functor,
	const Params&... params
	) {
	static_assert(detail::are_supported<Results...>::value, "Types of result (Results) must by fundamental types, std::string or const char*.");
	static_assert(detail::are_supported<Params...>::value, "Types of parameters (Params) must by fundamental types, std::string or const char*.");
	static_assert(detail::args_matches<F, Results...>::value, "The given parameter list of functor doesn't math with the Results parameter pack.");

	detail::exec<Results...>(
		database,
		sql.data(),
		sql.data() + sql.size(),
		functor,
		params...
		);
}

template <class Result, class... Params>
inline Result exec(
	database& database,
	const char* sql,
	const Params&... params) {
	static_assert(detail::are_supported<Result>::value, "Type of result must by fundamental type, std::string or const char*.");
	static_assert(detail::are_supported<Params...>::value, "Types of parameters (Params) must by fundamental types, std::string or const char*.");

	return detail::exec_t<Result>::invoke<Params...>(
		database,
		sql,
		params...
		);
}

template <class Result, class... Params>
inline Result exec(
	database& database,
	const string sql,
	const Params&... params) {
	static_assert(detail::are_supported<Result>::value, "Type of result must by fundamental type, std::string or const char*.");
	static_assert(detail::are_supported<Params...>::value, "Types of parameters (Params) must by fundamental types, std::string or const char*.");

	return detail::exec_t<Result>::invoke<Params...>(
		database,
		sql,
		params...
		);
}

class sqlite_error : public std::runtime_error {
protected:
	sqlite_error(const char*);
	friend struct detail::impl;
};

class abort_error : public sqlite_error {
protected:
	abort_error(const char*);
	friend struct detail::impl;
};

class auth_error : public sqlite_error {
	auth_error(const char*);
	friend struct detail::impl;
};

class busy_error : public sqlite_error {
protected:
	busy_error(const char*);
	friend struct detail::impl;
};

class cantopen_error : public sqlite_error {
protected:
	cantopen_error(const char*);
	friend struct detail::impl;
};

class constraint_error : public sqlite_error {
protected:
	constraint_error(const char*);
	friend struct detail::impl;
};

class corrupt_error : public sqlite_error {
protected:
	corrupt_error(const char*);
	friend struct detail::impl;
};

class empty_error : public sqlite_error {
	empty_error(const char*);
	friend struct detail::impl;
};

class format_error : public sqlite_error {
	format_error(const char*);
	friend struct detail::impl;
};

class full_error : public sqlite_error {
	full_error(const char*);
	friend struct detail::impl;
};

class internal_error : public sqlite_error {
	internal_error(const char*);
	friend struct detail::impl;
};

class interrupt_error : public sqlite_error {
	interrupt_error(const char*);
	friend struct detail::impl;
};

class ioerr_error : public sqlite_error {
protected:
	ioerr_error(const char*);
	friend struct detail::impl;
};

class locked_error : public sqlite_error {
protected:
	locked_error(const char*);
	friend struct detail::impl;
};

class mismatch_error : public sqlite_error {
	mismatch_error(const char*);
	friend struct detail::impl;
};

class misuse_error : public sqlite_error {
	misuse_error(const char*);
	friend struct detail::impl;
};

class nolfs_error : public sqlite_error {
	nolfs_error(const char*);
	friend struct detail::impl;
};

class nomem_error : public sqlite_error {
	nomem_error(const char*);
	friend struct detail::impl;
};

class notadb_error : public sqlite_error {
	notadb_error(const char*);
	friend struct detail::impl;
};

class notfound_error : public sqlite_error {
	notfound_error(const char*);
	friend struct detail::impl;
};

class perm_error : public sqlite_error {
	perm_error(const char*);
	friend struct detail::impl;
};

class protocol_error : public sqlite_error {
	protocol_error(const char*);
	friend struct detail::impl;
};

class range_error : public sqlite_error {
	range_error(const char*);
	friend struct detail::impl;
};

class readonly_error : public sqlite_error {
protected:
	readonly_error(const char*);
	friend struct detail::impl;
};

class row_error : public sqlite_error {
	row_error(const char*);
	friend struct detail::impl;
};

class schema_error : public sqlite_error {
	schema_error(const char*);
	friend struct detail::impl;
};

class toobig_error : public sqlite_error {
	toobig_error(const char*);
	friend struct detail::impl;
};

class warning_error : public sqlite_error {
	warning_error(const char*);
	friend struct detail::impl;
};

class abort_rollback_error : public abort_error {
	abort_rollback_error(const char*);
	friend struct detail::impl;
};

class busy_recovery_error : public busy_error {
	busy_recovery_error(const char*);
	friend struct detail::impl;
};

class busy_snapshot_error : public busy_error {
	busy_snapshot_error(const char*);
	friend struct detail::impl;
};

class cantopen_convpath_error : public cantopen_error {
	cantopen_convpath_error(const char*);
	friend struct detail::impl;
};

class cantopen_fullpath_error : public cantopen_error {
	cantopen_fullpath_error(const char*);
	friend struct detail::impl;
};

class cantopen_isdir_error : public cantopen_error {
	cantopen_isdir_error(const char*);
	friend struct detail::impl;
};

class constraint_check_error : public constraint_error {
	constraint_check_error(const char*);
	friend struct detail::impl;
};

class constraint_commithook_error : public constraint_error {
	constraint_commithook_error(const char*);
	friend struct detail::impl;
};

class constraint_foreignkey_error : public constraint_error {
	constraint_foreignkey_error(const char*);
	friend struct detail::impl;
};

class constraint_function_error : public constraint_error {
	constraint_function_error(const char*);
	friend struct detail::impl;
};

class constraint_notnull_error : public constraint_error {
	constraint_notnull_error(const char*);
	friend struct detail::impl;
};

class constraint_primarykey_error : public constraint_error {
	constraint_primarykey_error(const char*);
	friend struct detail::impl;
};

class constraint_rowid_error : public constraint_error {
	constraint_rowid_error(const char*);
	friend struct detail::impl;
};

class constraint_trigger_error : public constraint_error {
	constraint_trigger_error(const char*);
	friend struct detail::impl;
};

class constraint_unique_error : public constraint_error {
	constraint_unique_error(const char*);
	friend struct detail::impl;
};

class constraint_vtab_error : public constraint_error {
	constraint_vtab_error(const char*);
	friend struct detail::impl;
};

class corrupt_vtab_error : public corrupt_error {
	corrupt_vtab_error(const char*);
	friend struct detail::impl;
};

class ioerr_access_error : public ioerr_error {
	ioerr_access_error(const char*);
	friend struct detail::impl;
};

class ioerr_checkreservedlock_error : public ioerr_error {
	ioerr_checkreservedlock_error(const char*);
	friend struct detail::impl;
};

class ioerr_close_error : public ioerr_error {
	ioerr_close_error(const char*);
	friend struct detail::impl;
};

class ioerr_convpath_error : public ioerr_error {
	ioerr_convpath_error(const char*);
	friend struct detail::impl;
};

class ioerr_delete_error : public ioerr_error {
	ioerr_delete_error(const char*);
	friend struct detail::impl;
};

class ioerr_delete_noent_error : public ioerr_error {
	ioerr_delete_noent_error(const char*);
	friend struct detail::impl;
};

class ioerr_dir_fsync_error : public ioerr_error {
	ioerr_dir_fsync_error(const char*);
	friend struct detail::impl;
};

class ioerr_fstat_error : public ioerr_error {
	ioerr_fstat_error(const char*);
	friend struct detail::impl;
};

class ioerr_fsync_error : public ioerr_error {
	ioerr_fsync_error(const char*);
	friend struct detail::impl;
};

class ioerr_gettemppath_error : public ioerr_error {
	ioerr_gettemppath_error(const char*);
	friend struct detail::impl;
};

class ioerr_lock_error : public ioerr_error {
	ioerr_lock_error(const char*);
	friend struct detail::impl;
};

class ioerr_mmap_error : public ioerr_error {
	ioerr_mmap_error(const char*);
	friend struct detail::impl;
};

class ioerr_nomem_error : public ioerr_error {
	ioerr_nomem_error(const char*);
	friend struct detail::impl;
};

class ioerr_rdlock_error : public ioerr_error {
	ioerr_rdlock_error(const char*);
	friend struct detail::impl;
};

class ioerr_read_error : public ioerr_error {
	ioerr_read_error(const char*);
	friend struct detail::impl;
};

class ioerr_seek_error : public ioerr_error {
	ioerr_seek_error(const char*);
	friend struct detail::impl;
};

class ioerr_shmmap_error : public ioerr_error {
	ioerr_shmmap_error(const char*);
	friend struct detail::impl;
};

class ioerr_shmopen_error : public ioerr_error {
	ioerr_shmopen_error(const char*);
	friend struct detail::impl;
};

class ioerr_shmsize_error : public ioerr_error {
	ioerr_shmsize_error(const char*);
	friend struct detail::impl;
};

class ioerr_short_read_error : public ioerr_error {
	ioerr_short_read_error(const char*);
	friend struct detail::impl;
};

class ioerr_truncate_error : public ioerr_error {
	ioerr_truncate_error(const char*);
	friend struct detail::impl;
};

class ioerr_unlock_error : public ioerr_error {
	ioerr_unlock_error(const char*);
	friend struct detail::impl;
};

class ioerr_write_error : public ioerr_error {
	ioerr_write_error(const char*);
	friend struct detail::impl;
};

class locked_sharedcache_error : public locked_error {
	locked_sharedcache_error(const char*);
	friend struct detail::impl;
};

class readonly_cantlock_error : public readonly_error {
	readonly_cantlock_error(const char*);
	friend struct detail::impl;
};

class readonly_dbmoved_error : public readonly_error {
	readonly_dbmoved_error(const char*);
	friend struct detail::impl;
};

class readonly_recovery_error : public readonly_error {
	readonly_recovery_error(const char*);
	friend struct detail::impl;
};

class readonly_rollback_error : public readonly_error {
	readonly_rollback_error(const char*);
	friend struct detail::impl;
};

}
