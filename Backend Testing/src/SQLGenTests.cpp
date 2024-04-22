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



TEST_CASE("CreateModel Single Column", "[sqlgenerator::CreateModel]") {
	sqlGenerator::CreateModel create;
	create.tableName("CreateModel");
	REQUIRE(create.str() == "");


	SECTION("Single Column int") {
		create.columns("UFID", "int");
		REQUIRE(create.str() == "create table CreateModel(UFID int)");
	}

	SECTION("Single Column int change table name") {
		create.columns("UFID", "int");
		create.tableName("ChangedCreateModel");
		REQUIRE(create.str() == "create table ChangedCreateModel(UFID int)");
	}

	SECTION("Single Column varchar") {
		create.columns("UFID", "varchar(255)");
		REQUIRE(create.str() == "create table CreateModel(UFID varchar(255))");
	}

	SECTION("Single Column int primary") {
		create.columns("UFID", "int", true);
		// not null is implied by primary key, however sqlite does not include this. Explicit for generation
		REQUIRE(create.str() == "create table CreateModel(UFID int not null, primary key(UFID))");
	}

	SECTION("Single Column int unique") {
		create.columns("UFID", "int", false, false, true);
		REQUIRE(create.str() == "create table CreateModel(UFID int unique)");
	}

	SECTION("Single Column int not null") {
		create.columns("UFID", "int", false, true);
		REQUIRE(create.str() == "create table CreateModel(UFID int not null)");
	}

	SECTION("Single Column int not null and unique") {
		create.columns("UFID", "int", false, true, true);
		REQUIRE(create.str() == "create table CreateModel(UFID int not null unique)");
	}

	SECTION("Single Column int primary key and unique") {
		create.columns("UFID", "int", true, false, true);
		// Primary implies unique so providing it is not required
		REQUIRE(create.str() == "create table CreateModel(UFID int not null, primary key(UFID))");
	}

	SECTION("Single Column int not null as foreign key") {
		create.columns("UFID", "int", false, true, false, {true, "CreateTest", "Column1"});
		REQUIRE(create.str() == "create table CreateModel(UFID int not null, foreign key (UFID) references CreateTest(Column1))");
	}

	SECTION("Single Column reset") {
		create.columns("UFID", "int", false, true, true);
		REQUIRE(create.str() == "create table CreateModel(UFID int not null unique)");
		create.reset();
		REQUIRE(create.str() == "");
	}
}



TEST_CASE("CreateModel Multiple Column", "[sqlgenerator::CreateModel]") {
	sqlGenerator::CreateModel create;
	create.tableName("CreateModel");
	REQUIRE(create.str() == "");

	SECTION("Multiple Column int") {
		create.columns("UFID", "int");
		create.columns("SID", "int");
		REQUIRE(create.str() == "create table CreateModel(UFID int, SID int)");
	}

	SECTION("Multiple Column int with interleaved addition") {
		create.columns("UFID", "int");
		create.columns("SID", "int");
		REQUIRE(create.str() == "create table CreateModel(UFID int, SID int)");
		create.columns("FName", "varchar(255)");
		REQUIRE(create.str() == "create table CreateModel(UFID int, SID int, FName varchar(255))");
	}

	SECTION("Multiple Column int primary") {
		create.columns("UFID", "int", true);
		create.columns("SID", "int");
		// not null is implied by primary key, however sqlite does not include this. Explicit for generation
		REQUIRE(create.str() == "create table CreateModel(UFID int not null, SID int, primary key(UFID))");
	}

	SECTION("Multiple Column int multiple primary") {
		create.columns("UFID", "int", true);
		create.columns("SID", "int", true);
		// not null is implied by primary key, however sqlite does not include this. Explicit for generation
		REQUIRE(create.str() == "create table CreateModel(UFID int not null, SID int not null, primary key(UFID, SID))");
	}

	SECTION("Multiple Column int unique") {
		create.columns("UFID", "int", false, false, true);
		create.columns("SID", "int");
		REQUIRE(create.str() == "create table CreateModel(UFID int unique, SID int)");
	}

	SECTION("Multiple Column int unique") {
		create.columns("UFID", "int", false, false, true);
		create.columns("SID", "int", false, false, true);
		REQUIRE(create.str() == "create table CreateModel(UFID int unique, SID int unique)");
	}

	SECTION("Multiple Column int not null") {
		create.columns("UFID", "int", false, true);
		create.columns("SID", "int");
		REQUIRE(create.str() == "create table CreateModel(UFID int not null, SID int)");
	}

	SECTION("Multiple Column int multiple not null") {
		create.columns("UFID", "int", false, true);
		create.columns("SID", "int", false, true);
		REQUIRE(create.str() == "create table CreateModel(UFID int not null, SID int not null)");
	}

	SECTION("Multiple Column int not null and unique") {
		create.columns("UFID", "int", false, true, true);
		create.columns("SID", "int");
		REQUIRE(create.str() == "create table CreateModel(UFID int not null unique, SID int)");
	}

	SECTION("Multiple Column int not null and unique") {
		create.columns("UFID", "int", false, true, true);
		create.columns("SID", "int", false, true, true);
		REQUIRE(create.str() == "create table CreateModel(UFID int not null unique, SID int not null unique)");
	}

	SECTION("Multiple Column int not null as foreign key") {
		create.columns("UFID", "int", false, true, false, { true, "CreateTest", "Column1" });
		create.columns("SID", "int", false, true);
		REQUIRE(create.str() == "create table CreateModel(UFID int not null, SID int not null, foreign key (UFID) references CreateTest(Column1))");
	}

	SECTION("Multiple Column int not null multiple foreign key") {
		create.columns("UFID", "int", false, true, false, { true, "CreateTest", "Column1" });
		create.columns("SID", "int", false, true, false, { true, "CreateTest", "Column2" });
		REQUIRE(create.str() == "create table CreateModel(UFID int not null, SID int not null, foreign key (UFID) references CreateTest(Column1), foreign key (SID) references CreateTest(Column2))");
	}

	SECTION("Reset after lots of operations") {
		create.columns("UFID", "int", true, true, false, { true, "CreateTest", "Column1" });
		create.columns("SID", "int", false, true, false, { true, "CreateTest", "Column2" });
		create.columns("FName", "varchar(255)", false, true);
		create.columns("LName", "varchar(255)", false, true);
		REQUIRE(create.str() == "create table CreateModel(UFID int not null, SID int not null, FName varchar(255) not null, LName varchar(255) not null, primary key(UFID), foreign key (UFID) references CreateTest(Column1), foreign key (SID) references CreateTest(Column2))");
		create.reset();
		REQUIRE(create.str() == "");
	}
}


TEST_CASE("DropModel", "[sqlgenerator::DropModel]") {
	sqlGenerator::DropModel drop;
	REQUIRE(drop.str() == "");
	drop.tableName("DropModel");
	REQUIRE(drop.str() == "drop table DropModel");
	drop.reset();
	REQUIRE(drop.str() == "");
}

