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

TEST_CASE("DeleteModel", "[sqlgenerator::DeleteModel]") {
	sqlGenerator::DeleteModel del;
	del.from("DeleteModel");
	REQUIRE(del.str() == "");

	SECTION("Where single condition") {
		del.where("col1 = 3");
		REQUIRE(del.str() == "delete from DeleteModel where col1 = 3");
	}

	SECTION("Where multiple condition") {
		del.where("col1 = 3");
		del.where("col2 = 'john'");
		REQUIRE(del.str() == "delete from DeleteModel where col1 = 3 and col2 = 'john'");
	}

	SECTION("Where single condition column") {
		del.where(sqlGenerator::column("col1") == 3);
		REQUIRE(del.str() == "delete from DeleteModel where col1 = 3");
	}

	SECTION("Where multiple condition column") {
		del.where(sqlGenerator::column("col1") == 3);
		del.where(sqlGenerator::column("col2") == "john");
		REQUIRE(del.str() == "delete from DeleteModel where col1 = 3 and col2 = 'john'");
	}


	SECTION("Where single condition multiple table") {
		del.from("SecondModel");
		del.where("col1 = 3");
		REQUIRE(del.str() == "delete from DeleteModel, SecondModel where col1 = 3");
	}

	SECTION("Where multiple condition multiple table") {
		del.from("SecondModel");
		del.where("col1 = 3");
		del.where("col2 = 'john'");
		REQUIRE(del.str() == "delete from DeleteModel, SecondModel where col1 = 3 and col2 = 'john'");
	}

	SECTION("Where single condition column multiple table") {
		del.from("SecondModel");
		del.where(sqlGenerator::column("col1") == 3);
		REQUIRE(del.str() == "delete from DeleteModel, SecondModel where col1 = 3");
	}

	SECTION("Where multiple condition column multiple table") {
		del.from("SecondModel");
		del.where(sqlGenerator::column("col1") == 3);
		del.where(sqlGenerator::column("col2") == "john");
		REQUIRE(del.str() == "delete from DeleteModel, SecondModel where col1 = 3 and col2 = 'john'");
	}


	SECTION("Reset") {
		del.where(sqlGenerator::column("col1") == 3);
		del.where(sqlGenerator::column("col2") == "john");
		REQUIRE(del.str() == "delete from DeleteModel where col1 = 3 and col2 = 'john'");
		del.reset();
		REQUIRE(del.str() == "");
	}
}

TEST_CASE("UpdateModel", "[sqlgenerator::UpdateModel]") {
	sqlGenerator::UpdateModel update;
	update.update("UpdateModel");
	REQUIRE(update.str() == "");

	SECTION("Single set") {
		update.set("col1", 3);
		REQUIRE(update.str() == "update UpdateModel set col1 = 3");
	}

	SECTION("Multiple set") {
		update.set("col1", 3);
		update.set("col2", 4);
		REQUIRE(update.str() == "update UpdateModel set col1 = 3, col2 = 4");
	}

	SECTION("Single set with where single condition") {
		update.set("col1", 3);
		update.where("col1 = 7");
		REQUIRE(update.str() == "update UpdateModel set col1 = 3 where col1 = 7");
	}

	SECTION("Multiple set with where single condition") {
		update.set("col1", 3);
		update.where("col1 = 7");

		update.set("col2", "john");
		REQUIRE(update.str() == "update UpdateModel set col1 = 3, col2 = 'john' where col1 = 7");
	}

	SECTION("Single set with where multiple condition") {
		update.set("col1", 3);
		update.where("col1 = 7");
		update.where("col2 = 'dan'");
		REQUIRE(update.str() == "update UpdateModel set col1 = 3 where col1 = 7 and col2 = 'dan'");
	}

	SECTION("Multiple set with where multiple condition") {
		update.set("col1", 3);
		update.where("col1 = 7");

		update.set("col2", "john");
		update.where("col2 = 'dan'");
		REQUIRE(update.str() == "update UpdateModel set col1 = 3, col2 = 'john' where col1 = 7 and col2 = 'dan'");
	}


	SECTION("Single set with where single condition column") {
		update.set("col1", 3);
		update.where(sqlGenerator::column("col1") == 7);
		REQUIRE(update.str() == "update UpdateModel set col1 = 3 where col1 = 7");
	}

	SECTION("Multiple set with where single condition column") {
		update.set("col1", 3);
		update.where(sqlGenerator::column("col1") == 7);

		update.set("col2", "john");
		REQUIRE(update.str() == "update UpdateModel set col1 = 3, col2 = 'john' where col1 = 7");
	}

	SECTION("Single set with where multiple condition") {
		update.set("col1", 3);
		update.where(sqlGenerator::column("col1") == 7);
		update.where(sqlGenerator::column("col2") == "dan");
		REQUIRE(update.str() == "update UpdateModel set col1 = 3 where col1 = 7 and col2 = 'dan'");
	}

	SECTION("Multiple set with where multiple condition column") {
		update.set("col1", 3);
		update.where(sqlGenerator::column("col1") == 7);

		update.set("col2", "john");
		update.where(sqlGenerator::column("col2") == "dan");
		REQUIRE(update.str() == "update UpdateModel set col1 = 3, col2 = 'john' where col1 = 7 and col2 = 'dan'");
	}
}

TEST_CASE("SelectModel", "[sqlgenerator::SelectModel]") {
	sqlGenerator::SelectModel select;
	select.from("SelectModel");
	REQUIRE(select.str() == "");

	SECTION("single select") {
		select.select("col1");
		REQUIRE(select.str() == "select col1 from SelectModel");
	}

	SECTION("multiple select") {
		select.select("col1");
		select.select("col2");
		REQUIRE(select.str() == "select col1, col2 from SelectModel");
	}

	SECTION("multiple select join") {
		select.select("col1");
		select.select("col2");
		select.join("SecondTable");
		REQUIRE(select.str() == "select col1, col2 from SelectModel join SecondTable");
	}

	SECTION("multiple select left join") {
		select.select("col1");
		select.select("col2");
		select.left_join("SecondTable");
		REQUIRE(select.str() == "select col1, col2 from SelectModel left join SecondTable");
	}

	SECTION("multiple select left outer join") {
		select.select("col1");
		select.select("col2");
		select.left_outer_join("SecondTable");
		REQUIRE(select.str() == "select col1, col2 from SelectModel left outer join SecondTable");
	}

	SECTION("multiple select right join") {
		select.select("col1");
		select.select("col2");
		select.right_join("SecondTable");
		REQUIRE(select.str() == "select col1, col2 from SelectModel right join SecondTable");
	}

	SECTION("multiple select right outer join") {
		select.select("col1");
		select.select("col2");
		select.right_outer_join("SecondTable");
		REQUIRE(select.str() == "select col1, col2 from SelectModel right outer join SecondTable");
	}

	SECTION("multiple select full join") {
		select.select("col1");
		select.select("col2");
		select.full_join("SecondTable");
		REQUIRE(select.str() == "select col1, col2 from SelectModel full join SecondTable");
	}

	SECTION("multiple select full outer join") {
		select.select("col1");
		select.select("col2");
		select.full_outer_join("SecondTable");
		REQUIRE(select.str() == "select col1, col2 from SelectModel full outer join SecondTable");
	}

	SECTION("multiple select join on condition") {
		select.select("col1");
		select.select("col2");
		select.join("SecondTable");
		select.on(sqlGenerator::column("col1") == sqlGenerator::column("col2"));
		REQUIRE(select.str() == "select col1, col2 from SelectModel join SecondTable on col1 = col2");
	}

	SECTION("multiple select left join on condition") {
		select.select("col1");
		select.select("col2");
		select.left_join("SecondTable");
		select.on(sqlGenerator::column("col1") == sqlGenerator::column("col2"));
		REQUIRE(select.str() == "select col1, col2 from SelectModel left join SecondTable on col1 = col2");
	}

	SECTION("multiple select left outer join on condition") {
		select.select("col1");
		select.select("col2");
		select.left_outer_join("SecondTable");
		select.on(sqlGenerator::column("col1") == sqlGenerator::column("col2"));
		REQUIRE(select.str() == "select col1, col2 from SelectModel left outer join SecondTable on col1 = col2");
	}

	SECTION("multiple select right join on condition") {
		select.select("col1");
		select.select("col2");
		select.right_join("SecondTable");
		select.on(sqlGenerator::column("col1") == sqlGenerator::column("col2"));
		REQUIRE(select.str() == "select col1, col2 from SelectModel right join SecondTable on col1 = col2");
	}

	SECTION("multiple select right outer join on condition") {
		select.select("col1");
		select.select("col2");
		select.right_outer_join("SecondTable");
		select.on(sqlGenerator::column("col1") == sqlGenerator::column("col2"));
		REQUIRE(select.str() == "select col1, col2 from SelectModel right outer join SecondTable on col1 = col2");
	}

	SECTION("multiple select full join on condition") {
		select.select("col1");
		select.select("col2");
		select.full_join("SecondTable");
		select.on(sqlGenerator::column("col1") == sqlGenerator::column("col2"));
		REQUIRE(select.str() == "select col1, col2 from SelectModel full join SecondTable on col1 = col2");
	}

	SECTION("multiple select full outer join on condition") {
		select.select("col1");
		select.select("col2");
		select.full_outer_join("SecondTable");
		select.on(sqlGenerator::column("col1") == sqlGenerator::column("col2"));
		REQUIRE(select.str() == "select col1, col2 from SelectModel full outer join SecondTable on col1 = col2");
	}

	SECTION("multiple select join on condition multiple") {
		select.select("col1");
		select.select("col2");
		select.join("SecondTable");
		select.on(sqlGenerator::column("col1") == sqlGenerator::column("col2"));
		select.on(sqlGenerator::column("col3") == sqlGenerator::column("col4"));
		REQUIRE(select.str() == "select col1, col2 from SelectModel join SecondTable on col1 = col2 and col3 = col4");
	}

	SECTION("multiple select left join on condition multiple") {
		select.select("col1");
		select.select("col2");
		select.left_join("SecondTable");
		select.on(sqlGenerator::column("col1") == sqlGenerator::column("col2"));
		select.on(sqlGenerator::column("col3") == sqlGenerator::column("col4"));
		REQUIRE(select.str() == "select col1, col2 from SelectModel left join SecondTable on col1 = col2 and col3 = col4");
	}

	SECTION("multiple select left outer join on condition multiple") {
		select.select("col1");
		select.select("col2");
		select.left_outer_join("SecondTable");
		select.on(sqlGenerator::column("col1") == sqlGenerator::column("col2"));
		select.on(sqlGenerator::column("col3") == sqlGenerator::column("col4"));
		REQUIRE(select.str() == "select col1, col2 from SelectModel left outer join SecondTable on col1 = col2 and col3 = col4");
	}

	SECTION("multiple select right join on condition multiple") {
		select.select("col1");
		select.select("col2");
		select.right_join("SecondTable");
		select.on(sqlGenerator::column("col1") == sqlGenerator::column("col2"));
		select.on(sqlGenerator::column("col3") == sqlGenerator::column("col4"));
		REQUIRE(select.str() == "select col1, col2 from SelectModel right join SecondTable on col1 = col2 and col3 = col4");
	}

	SECTION("multiple select right outer join on condition multiple") {
		select.select("col1");
		select.select("col2");
		select.right_outer_join("SecondTable");
		select.on(sqlGenerator::column("col1") == sqlGenerator::column("col2"));
		select.on(sqlGenerator::column("col3") == sqlGenerator::column("col4"));
		REQUIRE(select.str() == "select col1, col2 from SelectModel right outer join SecondTable on col1 = col2 and col3 = col4");
	}

	SECTION("multiple select full join on condition multiple") {
		select.select("col1");
		select.select("col2");
		select.full_join("SecondTable");
		select.on(sqlGenerator::column("col1") == sqlGenerator::column("col2"));
		select.on(sqlGenerator::column("col3") == sqlGenerator::column("col4"));
		REQUIRE(select.str() == "select col1, col2 from SelectModel full join SecondTable on col1 = col2 and col3 = col4");
	}

	SECTION("multiple select full outer join on condition multiple") {
		select.select("col1");
		select.select("col2");
		select.full_outer_join("SecondTable");
		select.on(sqlGenerator::column("col1") == sqlGenerator::column("col2"));
		select.on(sqlGenerator::column("col3") == sqlGenerator::column("col4"));
		REQUIRE(select.str() == "select col1, col2 from SelectModel full outer join SecondTable on col1 = col2 and col3 = col4");
	}

	SECTION("single distinct select") {
		select.select("col1");
		select.distinct();
		REQUIRE(select.str() == "select distinct col1 from SelectModel");
	}
}