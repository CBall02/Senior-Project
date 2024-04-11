#include "catch_amalgamated.hpp"
#include "sqlGenerator.h"



TEST_CASE("InsertModel", "[InsertModel]") {
	sqlGenerator::InsertModel insert;
	insert.into("InsertModel");
	REQUIRE(insert.str() == "");

	SECTION("Single column entry") {
		insert.insert("UFID", 29909937);
		REQUIRE(insert.str() == "insert into InsertModel(UFID) values(29909937)");
	}

	SECTION("Multiple column entry single type") {
		insert.insert("UFID", 29909937);
		insert.insert("EmployeeID", 33619);
		REQUIRE(insert.str() == "insert into InsertModel(UFID, EmployeeID) values(29909937, 33619)");
	}

	SECTION("Multiple column entry multiple type") {
		insert.insert("UFID", 29909937);
		insert.insert("Name", "John Doe");
		REQUIRE(insert.str() == "insert into InsertModel(UFID, Name) values(29909937, 'John Doe')");
	}

	SECTION("Reset Insert Model") {
		insert.insert("UFID", 29909937);
		insert.insert("Name", "John Doe");
		REQUIRE(insert.str() == "insert into InsertModel(UFID, Name) values(29909937, 'John Doe')");
		insert.reset();
		REQUIRE(insert.str() == "");
	}

	SECTION("Multiple column entry with replace, multiple type") {
		insert.replace(true);
		insert.insert("UFID", 29909937);
		insert.insert("Name", "John Doe");
		REQUIRE(insert.str() == "insert or replace into InsertModel(UFID, Name) values(29909937, 'John Doe')");
	}

	SECTION("Change Table Name") {
		insert.insert("UFID", 29909937);
		insert.insert("Name", "John Doe");
		insert.into("ChangedInsertModel");
		REQUIRE(insert.str() == "insert into ChangedInsertModel(UFID, Name) values(29909937, 'John Doe')");
	}
}


TEST_CASE("CreateModel", "[CreateModel]") {

}