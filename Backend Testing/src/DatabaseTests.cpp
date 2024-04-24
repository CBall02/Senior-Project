#include "catch_amalgamated.hpp"
#include <string>
#include <vector>
#include <iostream>
#include "database.h"
#include "sqlGenerator.h"



TEST_CASE("Database 1") {
	Database::instance()->openDatabase(":memory:");
	sqlGenerator::CreateModel create;
	create.tableName("company");
	create.columns("id", "int", true, true, true)
		("name", "varchar(30)", false, true)
		("age", "int", false, true)
		("address", "varchar(50)", false, true)
		("salary", "float", false, true);

	sqlGenerator::InsertModel insert;
	insert.into("company");
	insert.insert("id", 1)
				 ("name", "Paul")
				 ("salary", 20000)
				 ("age", 32)
				 ("address", "California");


	std::string select = "SELECT * from COMPANY";

	auto created = Database::instance()->sqlExec(create.str());
	REQUIRE(created);
	REQUIRE(created.result == true);

	auto inserted = Database::instance()->sqlExec(insert.str());
	REQUIRE(inserted);
	REQUIRE(inserted.result == true);

	auto query = Database::instance()->queryDatabase(select);
	REQUIRE(query);
	REQUIRE(query->numFields() == 5);
	REQUIRE(query->getIntField("id") == 1);
	REQUIRE(query->getIntField("age") == 32);
	REQUIRE(query->getFloatField("salary") == 20000);
	REQUIRE(query->getStringField("name") == std::string("Paul"));
	REQUIRE(query->getStringField("address") == std::string("California"));
}

TEST_CASE("Database 2") {
	Database::instance()->openDatabase(":memory:");
	sqlGenerator::CreateModel create;
	create.tableName("company");
	create.columns("id", "int", true, true, true)
		("name", "varchar(30)", false, true)
		("age", "int", false, true)
		("address", "varchar(50)", false, true)
		("salary", "float", false, true);



	std::vector<int> ids = { 1, 2, 3, 4 };
	std::vector<std::string> names = { "Paul", "Allen", "Teddy", "Mark" };
	std::vector<int> ages = { 32, 25, 23, 25 };
	std::vector<std::string> addresses = { "California", "Texas", "Norway", "Rich-mond" };
	std::vector<float> salaries = { 20000.0, 15000.0, 20000.0, 65000.0 };


	auto created = Database::instance()->sqlExec(create.str());

	REQUIRE(created);
	REQUIRE(created.result == true);


	for (int i = 0; i < 4; i++) {
		sqlGenerator::InsertModel insert;
		insert.into("company");
		insert.insert("id", ids[i])
					 ("name", names[i])
					 ("salary", salaries[i])
					 ("age", ages[i])
					 ("address", addresses[i]);
		auto inserted = Database::instance()->sqlExec(insert.str());
		REQUIRE(inserted);
		REQUIRE(inserted.result == true);
	}


	std::string select = "SELECT * from COMPANY";
	auto query = Database::instance()->queryDatabase(select);
	REQUIRE(query);
	REQUIRE(query->numFields() == 5);
	for (int i = 0; !query->eof(); i++) {
		REQUIRE(query->getIntField("id") == ids[i]);
		REQUIRE(query->getIntField("age") == ages[i]);
		REQUIRE(query->getFloatField("salary") == salaries[i]);
		REQUIRE(query->getStringField("name") == names[i]);
		REQUIRE(query->getStringField("address") == addresses[i]);
		query->nextRow();
	}
}

TEST_CASE("Database 3") {
	Database::instance()->openDatabase(":memory:");
	sqlGenerator::CreateModel create;
	create.tableName("company");
	create.columns("id", "int", true, true, true)
				("name", "varchar(30)", false, true)
				("age", "int", false, true)
				("address", "varchar(50)", false, true)
				("salary", "float", false, true);
	sqlGenerator::InsertModel insert;
	insert.into("company");
	insert.insert("id", 1)
				("name", "Paul")
				("salary", 20000)
				("age", 32)
				("address", "California");
	std::string select = "select * from company";

	sqlGenerator::UpdateModel update;
	update.set("salary", 25000.23);
	update.where(sqlGenerator::column("id") == 1);
	update.update("company");

	auto created = Database::instance()->sqlExec(create.str());
	REQUIRE(created);
	REQUIRE(created.result == true);

	auto inserted = Database::instance()->sqlExec(insert.str());
	REQUIRE(inserted);
	REQUIRE(inserted.result == true);

	auto updated = Database::instance()->sqlExec(update.str());
	REQUIRE(updated);
	REQUIRE(updated.result == true);
	auto query = Database::instance()->queryDatabase(select);
	REQUIRE(query);
	REQUIRE(query->numFields() == 5);
	REQUIRE(query->getIntField("id") == 1);
	REQUIRE(query->getIntField("age") == 32);
	REQUIRE(query->getFloatField("salary") == 25000.23);
	REQUIRE(query->getStringField("name") == std::string("Paul"));
	REQUIRE(query->getStringField("address") == std::string("California"));
}

TEST_CASE("Database 4") {
	Database::instance()->openDatabase(":memory:");
	sqlGenerator::CreateModel create;
	create.tableName("company");
	create.columns("id", "int", true, true, true)
		("name", "varchar(30)", false, true)
		("age", "int", false, true)
		("address", "varchar(50)", false, true)
		("salary", "float", false, true);
	sqlGenerator::InsertModel insert;
	insert.into("company");
	insert.insert("id", 1)
		("name", "Paul")
		("salary", 20000)
		("age", 32)
		("address", "California");
	std::string select = "select * from company";

	sqlGenerator::DeleteModel del;
	del.from("company");
	del.where(sqlGenerator::column("id") == 1);

	auto created = Database::instance()->sqlExec(create.str());
	REQUIRE(created);
	REQUIRE(created.result == true);

	auto inserted = Database::instance()->sqlExec(insert.str());
	REQUIRE(inserted);
	REQUIRE(inserted.result == true);

	auto deleted = Database::instance()->sqlExec(del.str());
	REQUIRE(deleted);
	REQUIRE(deleted.result == true);

	auto query = Database::instance()->queryDatabase(select);
	REQUIRE(query);
	REQUIRE(query->numFields() == 5);
	REQUIRE(query->eof());
}

TEST_CASE("Database::parseSchema Test") {
	Database::instance()->openDatabase(":memory:");
	sqlGenerator::CreateModel create;
	create.tableName("company");
	create.columns("id", "int", true, true, true)
		("name", "varchar(30)", false, true)
		("age", "int", false, true)
		("address", "varchar(50)", false, true)
		("salary", "float", false, true);

	Database::Column idInfo;
	idInfo.name = "id";
	idInfo.type = "int";
	idInfo.isPrimary = true;
	idInfo.isNotNull = true;
	idInfo.isUnique = true;

	Database::Column nameInfo;
	nameInfo.name = "name";
	nameInfo.type = "varchar(30)";
	nameInfo.isPrimary = false;
	nameInfo.isNotNull = true;
	nameInfo.isUnique = false;

	Database::Column ageInfo;
	ageInfo.name = "age";
	ageInfo.type = "int";
	ageInfo.isPrimary = false;
	ageInfo.isNotNull = true;
	ageInfo.isUnique = false;

	Database::Column addressInfo;
	addressInfo.name = "address";
	addressInfo.type = "varchar(50)";
	addressInfo.isPrimary = false;
	addressInfo.isNotNull = true;
	addressInfo.isUnique = false;

	Database::Column salaryInfo;
	salaryInfo.name = "salary";
	salaryInfo.type = "float";
	salaryInfo.isPrimary = false;
	salaryInfo.isNotNull = true;
	salaryInfo.isUnique = false;

	auto created = Database::instance()->sqlExec(create.str());
	REQUIRE(created);
	REQUIRE(created.result == true);
	auto schema = Database::instance()->getTableSchema("company");
	REQUIRE(schema.size() == 5);

	
	REQUIRE(schema[0].name == idInfo.name);
	REQUIRE(schema[0].type == idInfo.type);
	REQUIRE(schema[0].isPrimary == idInfo.isPrimary);
	REQUIRE(schema[0].isNotNull == idInfo.isNotNull);
	REQUIRE(schema[0].isUnique == idInfo.isUnique);
	
	
	REQUIRE(schema[1].name == nameInfo.name);
	REQUIRE(schema[1].type == nameInfo.type);
	REQUIRE(schema[1].isPrimary == nameInfo.isPrimary);
	REQUIRE(schema[1].isNotNull == nameInfo.isNotNull);
	REQUIRE(schema[1].isUnique == nameInfo.isUnique);

	REQUIRE(schema[2].name == ageInfo.name);
	REQUIRE(schema[2].type == ageInfo.type);
	REQUIRE(schema[2].isPrimary == ageInfo.isPrimary);
	REQUIRE(schema[2].isNotNull == ageInfo.isNotNull);
	REQUIRE(schema[2].isUnique == ageInfo.isUnique);

	REQUIRE(schema[3].name == addressInfo.name);
	REQUIRE(schema[3].type == addressInfo.type);
	REQUIRE(schema[3].isPrimary == addressInfo.isPrimary);
	REQUIRE(schema[3].isNotNull == addressInfo.isNotNull);
	REQUIRE(schema[3].isUnique == addressInfo.isUnique);

	REQUIRE(schema[4].name == salaryInfo.name);
	REQUIRE(schema[4].type == salaryInfo.type);
	REQUIRE(schema[4].isPrimary == salaryInfo.isPrimary);
	REQUIRE(schema[4].isNotNull == salaryInfo.isNotNull);
	REQUIRE(schema[4].isUnique == salaryInfo.isUnique);
}

TEST_CASE("GetTable") {
	Database::instance()->openDatabase(":memory:");
	sqlGenerator::CreateModel create;
	create.tableName("company");
	create.columns("id", "int", true, true, true)
		("name", "varchar(30)", false, true)
		("age", "int", false, true)
		("address", "varchar(50)", false, true)
		("salary", "float", false, true);
	auto table = Database::instance()->getTable("company");
	REQUIRE_FALSE(table);

	auto created = Database::instance()->sqlExec(create.str());
	REQUIRE(created);
	REQUIRE(created.result == true);
	table = Database::instance()->getTable("company");
	REQUIRE(table);	 
}

