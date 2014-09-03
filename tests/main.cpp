#include <stdio.h>
#include "gtest/gtest.h"
#include <sqlite3.hpp>

struct sqlt3cpp_test : public ::testing::Test {
	sqlt3cpp_test() {
		database = sqlt3::open("test.db");
	}

	~sqlt3cpp_test() {
		close(database);
	}

	sqlt3::database database;

	static const long long children = 161;
	static const long long equator = 40071875498;
	static const long long meridian = 40007862917;
	static const long long area = 510072675965685;
};

/*TEST_F(sqlt3cpp_test, open) {
	std::remove("dummy.db");
	database = sqlt3::open("dummy.db");
	EXPECT_TRUE(database);
	sqlt3::close(database);
	std::remove("dummy.db");
}

TEST_F(sqlt3cpp_test, close) {
	close(database);
	EXPECT_FALSE(database);
}

TEST_F(sqlt3cpp_test, exec_should_execute_statement) {
	std::vector<std::string> values;

	auto functor = [&](const std::string& value) -> bool {
		values.push_back(value);
		return false;
	};

	sqlt3::execf<std::string>(database, "SELECT value FROM \"table\" WHERE key = 'twin' ORDER BY value;", functor);
	ASSERT_EQ(2, values.size());
	EXPECT_TRUE(std::to_string(1) == values[0]);
	EXPECT_TRUE(std::to_string(2) == values[1]);
}

TEST_F(sqlt3cpp_test, exec_should_throw_exception) {
	std::vector<std::size_t> num_cols_vec;
	std::vector<std::string> values_vec;
	std::vector<std::string> names_vec;

	auto functor = [&]() -> bool {
		throw std::string("today is the day");
		return true;
	};

	ASSERT_THROW(sqlt3::execf(database, "SELECT value FROM \"table\" WHERE key = 'twin';", functor), std::string);
}

TEST_F(sqlt3cpp_test, tuple_exec_should_execute_statement) {
	auto tuple = sqlt3::exect<int, std::string, double, std::string>(database, "SELECT first, second, third, fourth FROM \"numbers\" WHERE first = 1;");

	EXPECT_EQ(1, std::get<0>(tuple));
	EXPECT_EQ("one", std::get<1>(tuple));
	EXPECT_GT(0.0001, std::abs(11.11 - std::get<2>(tuple)));
	EXPECT_EQ("first", std::get<3>(tuple));
}

TEST_F(sqlt3cpp_test, vexec_should_execute_statement) {
	auto vector = sqlt3::execv<int, std::string, double, std::string>(database, "SELECT first, second, third, fourth FROM \"numbers\" ORDER BY first;");

	ASSERT_EQ(10, vector.size());

	EXPECT_EQ(0, std::get<0>(vector[0]));
	EXPECT_EQ("zero", std::get<1>(vector[0]));
	EXPECT_GT(0.0001, std::abs(0.0 - std::get<2>(vector[0])));
	EXPECT_EQ("null", std::get<3>(vector[0]));

	EXPECT_EQ(5, std::get<0>(vector[5]));
	EXPECT_EQ("five", std::get<1>(vector[5]));
	EXPECT_GT(0.0001, std::abs(55.55 - std::get<2>(vector[5])));
	EXPECT_EQ("fifth", std::get<3>(vector[5]));

	EXPECT_EQ(9, std::get<0>(vector[9]));
	EXPECT_EQ("nine", std::get<1>(vector[9]));
	EXPECT_GT(0.0001, std::abs(99.99 - std::get<2>(vector[9])));
	EXPECT_EQ("ninth", std::get<3>(vector[9]));
}

TEST_F(sqlt3cpp_test, tuple_exec_should_execute_statement_with_binding) {
	auto tuple = sqlt3::exect<int, std::string, double, std::string>(
		database, 
		"SELECT first, second, third, fourth FROM \"numbers\" WHERE first = ?;",
		1ull);

	EXPECT_EQ(1, std::get<0>(tuple));
	EXPECT_EQ("one", std::get<1>(tuple));
	EXPECT_GT(0.0001, std::abs(11.11 - std::get<2>(tuple)));
	EXPECT_EQ("first", std::get<3>(tuple));
}*/

TEST_F(sqlt3cpp_test, void_value_exec) {
	sqlt3::exec<void>(
		database,
		"SELECT first FROM \"numbers\" WHERE fourth = ?;",
		""
		);
}

TEST_F(sqlt3cpp_test, single_value_exec) {
	auto value = sqlt3::exec<int>(
		database,
		"SELECT first FROM \"numbers\" WHERE fourth = ?;",
		"first"
		);

	EXPECT_EQ(1, value);
}

TEST_F(sqlt3cpp_test, tuple_value_exec) {
	auto value = sqlt3::exec< std::tuple<int, std::string> >(
		database,
		"SELECT first, second FROM \"numbers\" WHERE fourth = ?;",
		"first"
		);

	EXPECT_EQ(1, std::get<0>(value));
	EXPECT_EQ("one", std::get<1>(value));
}

TEST_F(sqlt3cpp_test, detail_is_bindable) {
	struct dummy_t { };
	EXPECT_TRUE((sqlt3::detail::are_supported<int, int>::value));
	EXPECT_FALSE((sqlt3::detail::are_supported<dummy_t, int>::value));
}

TEST_F(sqlt3cpp_test, column_std_string_nullptr_bug) {
	auto value = sqlt3::exec<std::string>(
		database,
		"SELECT value FROM \"table\" WHERE key = ?;",
		"null"
		);
	EXPECT_EQ("", value);
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	auto result = RUN_ALL_TESTS();
	std::system("pause");
	return result;
}
