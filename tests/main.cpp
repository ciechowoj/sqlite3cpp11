#include <stdio.h>
#include "gtest/gtest.h"
#include <sqlite3.hpp>

namespace sqlite3 = sqlite3cpp;

struct sqlite3cpp_test : public ::testing::Test {
	sqlite3cpp_test() {
		database = sqlite3::open("test.db");
	}

	~sqlite3cpp_test() {
		close(database);
	}

	sqlite3::database database;

	static const long long children = 161;
	static const long long equator = 40071875498;
	static const long long meridian = 40007862917;
	static const long long area = 510072675965685;
};

TEST_F(sqlite3cpp_test, open) {
	std::remove("dummy.db");
	database = sqlite3::open("dummy.db");
	EXPECT_TRUE(database);
	sqlite3::close(database);
	std::remove("dummy.db");
}

TEST_F(sqlite3cpp_test, close) {
	close(database);
	EXPECT_FALSE(database);
}

TEST_F(sqlite3cpp_test, exec_should_execute_statement) {
	std::vector<std::size_t> num_cols_vec;
	std::vector<std::string> values_vec;
	std::vector<std::string> names_vec;

	auto functor = [&](std::size_t num_cols, const char* const* values, const char* const* names) -> bool {
		num_cols_vec.push_back(num_cols);
		values_vec.push_back(values[0]);
		names_vec.push_back(names[0]);
		return false;
	};

	ASSERT_EQ(false, sqlite3::exec(database, "SELECT value FROM \"table\" WHERE key = 'twin';", functor));
	ASSERT_EQ(2, num_cols_vec.size());
	EXPECT_EQ(1, num_cols_vec[0]);
	EXPECT_EQ(1, num_cols_vec[1]);
	EXPECT_TRUE(std::to_string(1) == values_vec[0] || std::to_string(2) == values_vec[0]);
	EXPECT_TRUE(std::to_string(1) == values_vec[1] || std::to_string(2) == values_vec[1]);
	EXPECT_EQ("value", names_vec[0]);
	EXPECT_EQ("value", names_vec[1]);
}

TEST_F(sqlite3cpp_test, exec_should_execute_statement_and_abort) {
	std::vector<std::size_t> num_cols_vec;
	std::vector<std::string> values_vec;
	std::vector<std::string> names_vec;

	auto functor = [&](std::size_t num_cols, const char* const* values, const char* const* names) -> bool {
		num_cols_vec.push_back(num_cols);
		values_vec.push_back(values[0]);
		names_vec.push_back(names[0]);
		return true;
	};

	ASSERT_EQ(true, sqlite3::exec(database, "SELECT value FROM \"table\" WHERE key = 'twin';", functor));
	ASSERT_EQ(1, num_cols_vec.size());
	EXPECT_EQ(1, num_cols_vec[0]);
	EXPECT_TRUE(std::to_string(1) == values_vec[0] || std::to_string(2) == values_vec[0]);
	EXPECT_EQ("value", names_vec[0]);
}

TEST_F(sqlite3cpp_test, exec_should_throw_exception) {
	std::vector<std::size_t> num_cols_vec;
	std::vector<std::string> values_vec;
	std::vector<std::string> names_vec;

	auto functor = [&](std::size_t num_cols, const char* const* values, const char* const* names) -> bool {
		throw std::string("today is the day");
		return true;
	};

	ASSERT_THROW(sqlite3::exec(database, "SELECT value FROM \"table\" WHERE key = 'twin';", functor), std::string);
}

TEST_F(sqlite3cpp_test, tuple_exec_should_execute_statement) {



	auto a = sqlite3::exec<int, std::string, double, std::string>(database, "SELECT first, second, third, fourth FROM \"numbers\" WHERE first = 1;");



}





int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	auto result = RUN_ALL_TESTS();
	std::system("pause");
	return result;
}
