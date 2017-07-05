//
//  API.cpp
//  Interpreter
//
//  Created by Clapeysron on 03/06/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include "API.hpp"
#include <algorithm>


#define CHARS 82
#define INT 83
#define FLOAT 84

RecordManager API::RM = RecordManager(BufferManager::Instance(), 4096);
CatalogManager& CM = CatalogManager::Instance();
IndexManager& IM = IndexManager::Instance();


void API::set_sql(string sql) {
	sql_query.clear();
	sql_query = sql;
}

string API::exec() {
	string sql = get_sql_from_bison();
	string buf;
	string ret_string;
	vector<int> ret_indexs;
	stringstream sql_stream(sql);
	//while(sql_stream >> buf)
	//{
	//    cout << buf << ' ';
	//}
	//cout << endl;
	//return "abc";
	sql_stream >> buf; // STMT
	sql_stream >> buf; // SELECT INSERT
	if (buf == "SELECT") {
		int select_list_num;
		sql_stream >> select_list_num; // LISTNUM

		string table_name;
		sql_stream >> buf; // TABLE
		sql_stream >> table_name;

		sql_stream >> buf; // WHERE or 81
		if (buf == "81") {
			vector<string> col_list;
			for (int i = 0; i < select_list_num; i++) {
				sql_stream >> buf; // NAME
				//sql_stream >> buf; // colname
				col_list.push_back(buf);
			}
			ret_string = select_all(table_name, col_list);
			return ret_string;
		} else if (buf == "WHERE") {
			vector<vector<int>> indexs_list;
			while (buf != "81") {
				sql_stream >> buf;

				if (buf == "AND") {
					sql_stream >> buf;
					if (buf == "CMP") {
						int comparison_type, type_1, type_2;
						string comp_1, comp_2;
						sql_stream >> comparison_type >> type_1 >> comp_1 >> type_2 >> comp_2;
						indexs_list.push_back(search_where(table_name, comparison_type, type_1, comp_1, type_2, comp_2));
						continue;
					} else if (buf == "BETWEEN") {
						int type_1, type_2, type_3;
						string comp, between_1, between_2;
						sql_stream >> type_1 >> between_1 >> type_2 >> between_2 >> type_3 >> comp;
						indexs_list.push_back(search_between(table_name, type_1, comp, type_2, between_1, type_3, between_2));
						continue;
					}
				} else if (buf == "CMP") {
					int comparison_type, type_1, type_2;
					string comp_1, comp_2;
					sql_stream >> comparison_type >> type_1 >> comp_1 >> type_2 >> comp_2;
					indexs_list.push_back(search_where(table_name, comparison_type, type_1, comp_1, type_2, comp_2));
				} else if (buf == "BETWEEN") {
					int type_1, type_2, type_3;
					string comp, between_1, between_2;
					sql_stream >> type_1 >> between_1 >> type_2 >> between_2 >> type_3 >> comp;
					indexs_list.push_back(search_between(table_name, type_1, comp, type_2, between_1, type_3, between_2));
				}
				vector<string> col_list;
				for (int i = 0; i < select_list_num; i++) {
					sql_stream >> buf; // NAME
					sql_stream >> buf; // colname
					col_list.push_back(buf);
				}
				ret_indexs = indexs_list[0];
				for (size_t i = 0; i < indexs_list.size(); i++) {
					ret_indexs = and_indexs(ret_indexs, indexs_list[i]);
					if (indexs_list[i].size() == 1 && indexs_list[i][0] < 0) {
						switch (indexs_list[i][0]) {
						case -1:
							ret_string += "Error: have no such table\n";
							return ret_string;
							break;
						case -2:
							ret_string += "Error: have no such column\n";
							return ret_string;
							break;
						case -3:
							ret_string += "Error: type doesn't match\n";
							return ret_string;
							break;
						}
					}
				}
				ret_string = select(table_name, col_list, ret_indexs);
				return ret_string;
			}
		}
	} else if (buf == "INSERT") {
		string table_name;
		sql_stream >> table_name;

		sql_stream >> buf; // VALUES
		int insert_list_num;
		sql_stream >> insert_list_num; // LISTNUM

		vector<int> type_list;
		vector<string> value_list;
		for (int i = 0; i < insert_list_num; i++) {
			int temp_type;
			string temp_value;
			sql_stream >> temp_type;
			sql_stream >> temp_value;
			type_list.push_back(temp_type);
			value_list.push_back(temp_value);
		}
		ret_string = insert(table_name, type_list, value_list);
		return ret_string;
	} else if (buf == "UPDATE") {
		sql_stream >> buf; // TABLE
		string table_name;
		sql_stream >> table_name;
		sql_stream >> buf;
		if (buf == "ASSIGN") {
			string col_name;
			int update_type;
			string update_value;
			sql_stream >> col_name;
			sql_stream >> update_type;
			sql_stream >> update_value;
			ret_string = update_all(table_name, col_name, update_type, update_value);
			return ret_string;
		} else if (buf == "WHERE") {
			while (buf != "ASSIGN") {
				vector<vector<int>> indexs_list;
				sql_stream >> buf;
				if (buf == "AND") {
					sql_stream >> buf;
					if (buf == "CMP") {
						int comparison_type, type_1, type_2;
						string comp_1, comp_2;
						sql_stream >> comparison_type >> type_1 >> comp_1 >> type_2 >> comp_2;
						indexs_list.push_back(search_where(table_name, comparison_type, type_1, comp_1, type_2, comp_2));
						continue;
					} else if (buf == "BETWEEN") {
						int type_1, type_2, type_3;
						string comp, between_1, between_2;
						sql_stream >> type_1 >> between_1 >> type_2 >> between_2 >> type_3 >> comp;
						indexs_list.push_back(search_between(table_name, type_1, comp, type_2, between_1, type_3, between_2));
						continue;
					}
				} else if (buf == "CMP") {
					int comparison_type, type_1, type_2;
					string comp_1, comp_2;
					sql_stream >> comparison_type >> type_1 >> comp_1 >> type_2 >> comp_2;
					indexs_list.push_back(search_where(table_name, comparison_type, type_1, comp_1, type_2, comp_2));
				} else if (buf == "BETWEEN") {
					int type_1, type_2, type_3;
					string comp, between_1, between_2;
					sql_stream >> type_1 >> between_1 >> type_2 >> between_2 >> type_3 >> comp;
					indexs_list.push_back(search_between(table_name, type_1, comp, type_2, between_1, type_3, between_2));
				}
				sql_stream >> buf; // ASSIGN
				string col_name;
				int update_type;
				string update_value;
				sql_stream >> col_name;
				sql_stream >> update_type;
				sql_stream >> update_value;

				ret_indexs = indexs_list[0];
				for (size_t i = 0; i < indexs_list.size(); i++) {
					ret_indexs = and_indexs(ret_indexs, indexs_list[i]);
					if (indexs_list[i].size() == 1 && indexs_list[i][0] < 0) {
						switch (indexs_list[i][0]) {
						case -1:
							ret_string += "Error: have no such table\n";
							return ret_string;
							break;
						case -2:
							ret_string += "Error: have no such column\n";
							return ret_string;
							break;
						case -3:
							ret_string += "Error: type doesn't match\n";
							return ret_string;
							break;
						}
					}
				}
				ret_string = update_part(table_name, col_name, update_type, update_value, ret_indexs);
				return ret_string;
			}
		}
	} else if (buf == "DELETE") {
		string table_name;
		sql_stream >> buf; // TABLE
		sql_stream >> table_name;

		sql_stream >> buf; // WHERE or $
		if (buf == "$") {
			ret_string = delete_all(table_name);
			return ret_string;
		} else if (buf == "WHERE") {
			while (buf != "$") {
				vector<vector<int>> indexs_list;
				sql_stream >> buf;
				if (buf == "AND") {
					sql_stream >> buf;
					if (buf == "CMP") {
						int comparison_type, type_1, type_2;
						string comp_1, comp_2;
						sql_stream >> comparison_type >> type_1 >> comp_1 >> type_2 >> comp_2;
						indexs_list.push_back(search_where(table_name, comparison_type, type_1, comp_1, type_2, comp_2));
						continue;
					} else if (buf == "BETWEEN") {
						int type_1, type_2, type_3;
						string comp, between_1, between_2;
						sql_stream >> type_1 >> between_1 >> type_2 >> between_2 >> type_3 >> comp;
						indexs_list.push_back(search_between(table_name, type_1, comp, type_2, between_1, type_3, between_2));
						continue;
					}
				} else if (buf == "CMP") {
					int comparison_type, type_1, type_2;
					string comp_1, comp_2;
					sql_stream >> comparison_type >> type_1 >> comp_1 >> type_2 >> comp_2;
					indexs_list.push_back(search_where(table_name, comparison_type, type_1, comp_1, type_2, comp_2));
				} else if (buf == "BETWEEN") {
					int type_1, type_2, type_3;
					string comp, between_1, between_2;
					sql_stream >> type_1 >> between_1 >> type_2 >> between_2 >> type_3 >> comp;
					indexs_list.push_back(search_between(table_name, type_1, comp, type_2, between_1, type_3, between_2));
				}
				ret_indexs = indexs_list[0];
				for (size_t i = 0; i < indexs_list.size(); i++) {
					ret_indexs = and_indexs(ret_indexs, indexs_list[i]);
					if (indexs_list[i].size() == 1 && indexs_list[i][0] < 0) {
						switch (indexs_list[i][0]) {
						case -1:
							ret_string += "Error: have no such table\n";
							return ret_string;
							break;
						case -2:
							ret_string += "Error: have no such column\n";
							return ret_string;
							break;
						case -3:
							ret_string += "Error: type doesn't match\n";
							return ret_string;
							break;
						}
					}

				}
				ret_string = delete_part(table_name, ret_indexs);
				return ret_string;
			}
		}
	} else if (buf == "CREATEINDEX") {
		string index_name;
		sql_stream >> index_name;
		string table_name;
		sql_stream >> table_name;
		int col_num;
		sql_stream >> col_num;
		vector<string> col_list;
		for (int i = 0; i < col_num; i++) {
			sql_stream >> buf;
			col_list.push_back(buf);
		}
		ret_string = create_index(table_name, index_name, col_list);
		return ret_string;
	} else if (buf == "CREATE") {
		string table_name;
		sql_stream >> table_name; // TABLE
		int create_num;
		sql_stream >> create_num; // CREATE_NUM
		vector<string> name_list;
		vector<int> type_list;
		vector<int> length_list;
		vector<int> primary_flag;
		vector<int> unique_flag;
		vector<int> nnull_flag;
		for (int i = 0; i < create_num; i++) {
			int type_len;
			sql_stream >> type_len; // type_len

			if (type_len >= 2000 && type_len < 2257) // string
			{
				type_list.push_back(82);
				length_list.push_back(type_len - 2000);
			} else if (type_len >= 3000 && type_len < 3257) {
				type_list.push_back(83);
				length_list.push_back(type_len - 3000);
			} else if (type_len >= 4000 && type_len < 4257) {
				type_list.push_back(84);
				length_list.push_back(type_len - 4000);
			}

			string col_name;
			sql_stream >> col_name;
			name_list.push_back(col_name);

			primary_flag.push_back(0);
			unique_flag.push_back(0);
			nnull_flag.push_back(0);
			sql_stream >> buf;
			while (buf != "ENDCOL") {
				if (buf == "ATTR_UNIQUE") {
					unique_flag.pop_back();
					unique_flag.push_back(1);
				} else if (buf == "ATTR_NOTNULL") {
					nnull_flag.pop_back();
					nnull_flag.push_back(1);
				} else if (buf == "ATTR_PRIKEY") {
					primary_flag.pop_back();
					primary_flag.push_back(1);
				}
				sql_stream >> buf;
			}
		}
		ret_string = create(table_name, name_list, type_list, length_list, primary_flag, unique_flag, nnull_flag);
		return ret_string;
	} else if (buf == "DROP") {
		string table_name;
		sql_stream >> table_name;
		ret_string = drop_table(table_name);
		return ret_string;
	} else if (buf == "DROPINDEX") {
		string index_name;
		sql_stream >> index_name;
		string table_name;
		sql_stream >> table_name;
		ret_string = drop_index(table_name, index_name);
		return ret_string;
	} else if (buf == "SHOWTABLE") {
		ret_string = show_tables();
		return ret_string;
	} else if (buf == "SHOWSTATUS") {
		ret_string = show_status();
		return ret_string;
	} else if (buf == "EXEC") {
		string file_name;
		sql_stream >> file_name;
		ifstream file;
		file.open(file_name);
		if (file.is_open() == 0) {
			ret_string = "Can't open file " + file_name;
			return ret_string;
		}
		Interpreter file_exe;
		file_exe.exe_loop(file);
	} else if (buf == "EXIT") {
		cout << "Bye" << endl;
		exit(0);
	} else {
		cout << "Error with" << buf << endl;
	}
	return ret_string;
}

string API::select_all(string table_name, vector<string> col_list) {
	string ret_string;
	//std::cout << "select_all" << std::endl;
	if (CM.have_table(table_name)) {

		if (!((col_list.size() == 1 && col_list.front() == "*") || CM.find_table(table_name).have_columns(col_list))) {
			ret_string += "Error: have no such columns\n";
		} else {
			std::vector<AttrInfo> ats;
			const std::vector<FieldInfo>& fields = CM.find_table(table_name).get_columns();
			for (size_t i = 0; i < fields.size(); i++) {
				int length = fields[i].get_type_magic_num() == 82 ? fields[i].cget_type().get_size() : 11;   //11 is the length of int max
				bool index = fields[i].get_index() == "" ? false : true;
				ats.push_back(AttrInfo(fields[i].get_name(), fields[i].get_type_magic_num(), length, fields[i].get_unique(), index));
			}
			TableStruct ts(table_name, IM.get_record_size(table_name), ats);

			std::vector<std::string> buff;
			RM.selectAll(ts, buff, col_list);
			for (size_t i = 0; i < buff.size(); i++) {
				ret_string += buff[i];
			}
			if (buff.size() < 4) {
				ret_string += "0 rows in set\n";
			} else {
				ret_string += std::to_string(buff.size() - 4) + " rows in set\n";
			}
		}



	} else {
		ret_string += "Error: have no such table " + table_name + "\n";
	}

	return ret_string;
}

string API::select(string table_name, vector<string> col_list, vector<int> indexs) {
	string ret_string;
	std::reverse(col_list.begin(), col_list.end());
	//std::cout << "select" << std::endl;
	
	if (CM.have_table(table_name)) {
		if (col_list.size() == 1 && col_list.front() == "*" || CM.have_column(table_name, col_list)) {



			std::vector<AttrInfo> ats;
			const std::vector<FieldInfo>& fields = CM.find_table(table_name).get_columns();
			for (size_t i = 0; i < fields.size(); i++) {
				int length = fields[i].get_type_magic_num() == 82 ? fields[i].cget_type().get_size() : 11;   //11 is the length of int max
				bool index = fields[i].get_index() == "" ? false : true;
				ats.push_back(AttrInfo(fields[i].get_name(), fields[i].get_type_magic_num(), length, fields[i].get_unique(), index));
			}
			TableStruct ts(table_name, IM.get_record_size(table_name), ats);
			std::vector<string> buff;
			RM.selectRecord(ts, col_list, indexs, buff);  // real version, applied after melody fixed;



			for (size_t i = 0; i < buff.size(); i++) {
				ret_string += buff[i];

			}
			if (buff.size() < 4) {
				ret_string += "0 rows in set\n";
			} else {
				ret_string += std::to_string(buff.size() - 4) + " rows in set\n";
			}

		} else {
			ret_string += "Error: have no such column\n";
		}
	} else {
		ret_string += "Error: have no such table\n";
	}


	return ret_string;
}

vector<int> API::search_between(string table_name, int type_1, string comp, int type_2, string between_1, int type_3, string between_2) {
	int temp = type_3;
	type_3 = type_1;
	type_1 = temp;
	std::string temp_str = between_2;
	between_2 = between_1;
	between_1 = temp_str;

	for (size_t j = 0; j < between_1.size(); j++) {
		if (between_1[j] == '#') {
			between_1[j] = ' ';
		}
	}

	for (size_t j = 0; j < between_2.size(); j++) {
		if (between_2[j] == '#') {
			between_2[j] = ' ';
		}
	}


	//std::cout << "search between" << std::endl;

	vector<int> ret_indexs;
	if (!CM.have_table(table_name)) {

		ret_indexs.push_back(-1); // -1 for no table_name
		return ret_indexs;
	} else {
		TableInfo& temp_table = CM.find_table(table_name);
		if (type_1 == 81) {
			if (!temp_table.have_column(comp)) {

				ret_indexs.push_back(-1); // -1 for no ret_indexs and no such column;
				return ret_indexs;
			} else {
				FieldInfo temp_field = temp_table.get_column(comp);
				std::stringstream ss;
				ss << between_1 << between_2;

				//std::string temp_index = temp_table.find_index(comp);
				if (type_2 == temp_field.get_type_magic_num() && type_3 == temp_field.get_type_magic_num()) {
					if (false) {
						std::string temp_index = temp_table.find_index(comp).second;
						switch (type_2) {
						case 82: {
							std::string real_between_1;
							std::string real_between_2;
							ss >> real_between_1 >> real_between_2;
							ret_indexs = IM.search_range(temp_index, &real_between_1, &real_between_2);
							return ret_indexs;
							break;
						}

						case 83: {
							int real_between_1;
							int real_between_2;
							ss >> real_between_1 >> real_between_2;
							ret_indexs = IM.search_range(temp_index, &real_between_1, &real_between_2);
							return ret_indexs;
							break;
						}

						case 84: {
							float real_between_1;
							float real_between_2;
							ss >> real_between_1 >> real_between_2;
							ret_indexs = IM.search_range(temp_index, &real_between_1, &real_between_2);
							return ret_indexs;
							break;
						}
						default:
							//std::cout << "no such type_2" << std::endl;

							ret_indexs.push_back(-2); // -2 for no such type;
							return ret_indexs;
						}
					} else {



						std::vector<AttrInfo> ats;
						const std::vector<FieldInfo>& fields = CM.find_table(table_name).get_columns();
						for (size_t i = 0; i < fields.size(); i++) {
							int length = fields[i].get_type_magic_num() == 82 ? fields[i].cget_type().get_size() : 11;   //11 is the length of int max
							bool index = fields[i].get_index() == "" ? false : true;
							ats.push_back(AttrInfo(fields[i].get_name(), fields[i].get_type_magic_num(), length, fields[i].get_unique(), index));
						}
						TableStruct ts(table_name, IM.get_record_size(table_name), ats);
						std::vector<int> result1;
						std::vector<int> result2;

						RM.selectRecordWithCondition(ts, result1, 6, type_1, comp, type_2, between_1);
						RM.selectRecordWithCondition(ts, result2, 5, type_1, comp, type_3, between_2);
						ret_indexs = and_indexs(result1, result2);
						return ret_indexs;

					}
				} else {
					//std::cout << "type not match" << std::endl;
					ret_indexs.push_back(-3); // -3 for type not match
					return ret_indexs;
				}

			}
		} else {
			//std::cout << "does not support" << std::endl;
			ret_indexs.push_back(-2); // -2 for not a column;
			return ret_indexs;
		}
	}

	return ret_indexs;
}


vector<int> API::search_where(string table_name, int comparison_type, int type_2, string comp_2, int type_1, string comp_1) {

	//std::cout << "search where" << std::endl;
	vector<int> ret_indexs;


	for (size_t j = 0; j < comp_2.size(); j++) {
		if (comp_2[j] == '#') {
			comp_2[j] = ' ';
		}
	}


	if (!CM.have_table(table_name)) {

		ret_indexs.push_back(-1); // -1 for no result
		return ret_indexs;
	} else {
		TableInfo& temp_table = CM.find_table(table_name);
		if (type_1 == 81) {
			if (!temp_table.have_column(comp_1)) {
				//std::cout << "no such column" << std::endl;
				ret_indexs.push_back(-2); // -1 for no such column 
				return ret_indexs;
			} else {
				FieldInfo temp_field = temp_table.get_column(comp_1);
				if (type_2 == temp_field.get_type_magic_num()) {
					if (false) {

						std::stringstream ss;
						ss << comp_2;

						std::string temp_index = temp_table.find_index(comp_1).second;
						switch (type_2) {
						case 82: {
							std::string real_comp_2;
							ss >> real_comp_2;
							ret_indexs = IM.search_where(temp_index, comparison_type, real_comp_2);
							return ret_indexs;
							break;
						}

						case 83: {
							int real_comp_2;
							ss >> real_comp_2;
							ret_indexs = IM.search_where(temp_index, comparison_type, real_comp_2);
							return ret_indexs;
							break;
						}

						case 84: {
							float real_comp_2;
							ss >> real_comp_2;
							ret_indexs = IM.search_where(temp_index, comparison_type, real_comp_2);
							return ret_indexs;
							break;
						}

						default:
							std::cout << "no such type_2" << std::endl;
							ret_indexs.push_back(-3); // -1 for no result
							return ret_indexs;
						}
					} else {

						std::vector<AttrInfo> ats;
						const std::vector<FieldInfo>& fields = CM.find_table(table_name).get_columns();
						for (size_t i = 0; i < fields.size(); i++) {
							int length = fields[i].get_type_magic_num() == 82 ? fields[i].cget_type().get_size() : 11;   //11 is the length of int max
							bool index = fields[i].get_index() == "" ? false : true;
							ats.push_back(AttrInfo(fields[i].get_name(), fields[i].get_type_magic_num(), length, fields[i].get_unique(), index));
						}
						TableStruct ts(table_name, IM.get_record_size(table_name), ats);
						std::vector<char*> buff;
						RM.selectRecordWithCondition(ts, ret_indexs, comparison_type, type_1, comp_1, type_2, comp_2);
						return ret_indexs;
					}
				} else {
					//std::cout << "type not match" << std::endl;
					ret_indexs.push_back(-3); // -3 for no type
					return ret_indexs;
				}

			}
		} else {

			std::cout << "Error: The first Comparator is not a column" << std::endl;
			ret_indexs.push_back(-1); // -1 for no type
			return ret_indexs;
		}
	}
	return ret_indexs;
}

string API::delete_all(string table_name) {

	string ret_string;
	int recordsnum;
	if (CM.have_table(table_name)) {
		// int IM.delete_all(table_name);
		recordsnum = IM.get_real_record_amount(table_name);
		std::vector<int> records;
		for (size_t i = 0; i < recordsnum; i++) {
			records.push_back(i);
		}
		delete_part(table_name, records);
		IM.clear_record(table_name);
		ret_string += "Query OK, " + std::to_string(recordsnum) + " row affected\n";
	} else {
		ret_string += "Error: have no such table\n";
	}

	return ret_string;
}

string API::delete_part(string table_name, vector<int> indexs) {


	string ret_string;
	if (CM.have_table(table_name)) {

		std::vector<AttrInfo> ats;
		const std::vector<FieldInfo>& fields = CM.find_table(table_name).get_columns();
		for (size_t i = 0; i < fields.size(); i++) {
			int length = fields[i].get_type_magic_num() == 82 ? fields[i].cget_type().get_size() : 11;   //11 is the length of int max
			bool index = fields[i].get_index() == "" ? false : true;
			ats.push_back(AttrInfo(fields[i].get_name(), fields[i].get_type_magic_num(), length, fields[i].get_unique(), index));
		}
		TableStruct ts(table_name, IM.get_record_size(table_name), ats);

		RM.deleteRecord(ts, indexs);
		IM.delete_part(table_name, indexs);
		// string RM.delete_part(table_name, indexs);
		ret_string += "Query OK, " + std::to_string(indexs.size()) + " row affected\n";
	} else {
		ret_string += "Error: have no such table\n";
	}

	return ret_string;
}

string API::insert(string table_name, vector<int> type_list, vector<string> value_list) {
	std::reverse(type_list.begin(), type_list.end());
	std::reverse(value_list.begin(), value_list.end());

	for (size_t i = 0; i < value_list.size(); i++) {
		for (size_t j = 0; j < value_list[i].size(); j++) {
			if (value_list[i][j] == '#'||value_list[i][j] == '$') {
				value_list[i][j] = ' ';
			}
		}
	}

	string ret_string;
	if (CM.have_table(table_name)) {
		if (CM.have_column_type(table_name, type_list)) {

			int buffsize = 0;
			std::vector<int> length_list;

			std::vector<AttrInfo> ats;
			const std::vector<FieldInfo>& fields = CM.find_table(table_name).get_columns();
			for (size_t i = 0; i < fields.size(); i++) {
				int length = type_list[i] == 82 ? fields[i].cget_type().get_size() : 11;   //11 is the length of int max
				if (value_list[i].size() > length) {
					ret_string += "'" + value_list[i] + "' 's length is larger than maximum\n";
					return ret_string;
				}
				bool index = fields[i].get_index() == "" ? false : true;
				ats.push_back(AttrInfo(fields[i].get_name(), type_list[i], length, fields[i].get_unique(), index));
				buffsize += length;
				length_list.push_back(length);
			}
			TableStruct ts(table_name, IM.get_record_size(table_name), ats);

			char * data = new char[buffsize + 1];
			int offset = 0;
			for (size_t i = 0; i < value_list.size(); i++) {
				memcpy(data + offset, value_list[i].data(), value_list[i].length() + 1);
				offset += length_list[i];
			}
			data[buffsize] = '\0';
			if (RM.insertIntoTable(ts, data) == -1) {
				ret_string += "Query fail: Duplicated keys on a unique column\n";
				return ret_string;
			}
			int startPoint = 0;
			for (size_t i = 0; i < ts.attrs.size(); i++) {
				std::string key_data(data + startPoint);
				stringstream ss(key_data);
				if (ts.attrs[i].index) {
					switch (ts.attrs[i].type) {
					case INT: {
						int key;
						ss >> key;
						IM.insert(table_name, ts.attrs[i].name, key);
						break;
					}
					case FLOAT: {
						float key;
						ss >> key;
						IM.insert(table_name, ts.attrs[i].name, key);
						break;
					}
					case CHARS: {
						std::string key;
						ss >> key;
						IM.insert(table_name, ts.attrs[i].name, key);
						break;
					}
					default:
						std::cout << "Error: insert type Error" << std::endl;
						break;
					}


				}
				startPoint += ts.attrs[i].length;

			}
			IM.insert_real_index(table_name);

			delete[] data;


		} else {
			ret_string += "Error: values' type doesn't match table'" + table_name + "'\n";
			return ret_string;
		}

	} else {
		ret_string += "Error: have no such table\n";
		return ret_string;
	}

	ret_string += "Query OK, 1 row affected\n";
	return ret_string;
}

string API::update_all(string table_name, string col_name, int update_type, string update_value) {
	// TODO: API update_all
	// bool CM.have_table(table_name);
	// bool CM.have_column(col_name);
	// int CM.update_all(table_name, col_name, update_type, update_value);
	// string RM.update_all(table_name, col_name, update_type, update_value);
	// string > out_string
	string ret_string;
	ret_string += "Query OK, 3 row affected\n";
	return ret_string;
}

string API::update_part(string table_name, string col_name, int update_type, string update_value, vector<int> indexs) {
	// TODO: API update_part
	// bool CM.have_table(table_name);
	// bool CM.have_column(col_name);
	// int CM.update_part(table_name, col_name, update_type, update_value, indexs);
	// string RM.update_part(table_name, col_name, update_type, update_value, indexs);
	// string > out_string

	string ret_string;
	ret_string += "Query OK, 3 row affected\n";
	return ret_string;
}

string API::create(string table_name, vector<string> name_list, vector<int> type_list, vector<int> length_list, vector<int> primary_flag, vector<int> unique_flag, vector<int> nnull_flag) {

	std::reverse(name_list.begin(), name_list.end());
	std::reverse(type_list.begin(), type_list.end());
	std::reverse(length_list.begin(), length_list.end());
	std::reverse(primary_flag.begin(), primary_flag.end());
	std::reverse(unique_flag.begin(), unique_flag.end());
	std::reverse(nnull_flag.begin(), nnull_flag.end());

	string ret_string;

	
	//primary index should be made;
	
	if (table_name.size() > 20) {
		ret_string += "Error : Table name should be less than 20 \n";
	} else {
		if (!CM.have_table(table_name)) {
			for (size_t i = 0; i < name_list.size(); i++) {
				for (size_t j = i + 1; j < name_list.size(); j++) {
					if (name_list[i]==name_list[j]) {
						ret_string += "Error : duplicated column not allowed\n";
						return ret_string;
					}
				}
			}
			
			CM.create_table(table_name, name_list, type_list, length_list, primary_flag, unique_flag, nnull_flag);
			ret_string += "Query OK, 0 rows affected\n";

		} else {
			ret_string += "Error : Table '" + table_name + "' already exists\n";
		}
	}

	IM.init(table_name);
	RM.createTable(table_name);

	//string > out_string

	return ret_string;
}

string API::create_index(string table_name, string index_name, vector<string> col_list) {
	// TODO: API create_index
	string ret_string;
	if (col_list.size() != 1) {
		ret_string += "Error: can only create index for one col at a time!\n";
		return ret_string;
	}

	if (CM.have_table(table_name)) {
		if (!CM.have_index_with_index_name(table_name, index_name)) {



			std::vector<AttrInfo> ats;
			const std::vector<FieldInfo>& fields = CM.find_table(table_name).get_columns();
			for (size_t i = 0; i < fields.size(); i++) {
				int length = fields[i].get_type_magic_num() == 82 ? fields[i].cget_type().get_size() : 11;   //11 is the length of int max
				bool index = fields[i].get_index() == "" ? false : true;
				ats.push_back(AttrInfo(fields[i].get_name(), fields[i].get_type_magic_num(), length, fields[i].get_unique(), index));
			}
			TableStruct ts(table_name, IM.get_record_size(table_name), ats);

			std::vector<std::string> values;
			RM.selectAttribute(ts, col_list[0], values);
			std::stringstream convert;

			switch (CM.get_type(table_name, col_list[0]).get_type()) {
			case Int: {
				std::vector<int> keys;
				int temp;
				for (size_t i = 0; i < values.size(); i++) {
					convert << values[i];
					convert >> temp;
					convert.clear();
					keys.push_back(temp);
				}
				IM.create_index(index_name, table_name, col_list[0], keys);
				break;
			}

			case Float: {
				std::vector<float> keys;
				float temp;
				for (size_t i = 0; i < values.size(); i++) {
					convert << values[i];
					convert >> temp;
					convert.clear();
					keys.push_back(temp);
				}
				IM.create_index(index_name, table_name, col_list[0], keys);
				break;
			}
			case Chars: {
				std::vector<std::string> keys;
				std::string temp;
				for (size_t i = 0; i < values.size(); i++) {
					convert << values[i];
					convert >> temp;
					convert.clear();
					keys.push_back(temp);
				}
				IM.create_index(index_name, table_name, col_list[0], keys);
				break;
			}
			}

			CM.set_index(table_name, col_list[0], index_name);

			//IM.create_index(index_name, table_name, col_list[0], keys);
			/*for (size_t i = 0; i < values.size(); i++) {
				delete[] values[i];
			}*/
			//IM.create_index(index_name, table_name, col_list[0], keys);
			//ret_string += "create index : needs melody's help\n";

		} else {
			ret_string += "Error: have same index name '" + index_name + "'\n";
			return ret_string;
		}

		ret_string += "Query OK, 0 rows affected\n";
	} else {
		ret_string += "Error: have no such table\n";
	}


	return ret_string;
}

string API::drop_table(string table_name) {

	string ret_string;
	if (CM.have_table(table_name)) {
		size_t num = IM.get_real_record_amount(table_name);
		//IM.drop_table(table_name);  // IM must be used before CM for it will use CM to get index message

		CM.drop_table(table_name);

		RM.dropTable(table_name);

		//TODO: fix 0 rows
		ret_string += "Query OK, " + std::to_string(num) + " rows affected\n";
	} else {
		ret_string += "Error: have no such table\n";
	}
	return ret_string;
}

string API::drop_index(string table_name, string index_name) {
	// TODO: API drop_index

	string ret_string;
	if (CM.have_table(table_name)) {

		if (CM.have_index_with_index_name(table_name, index_name)) {
			Type t = CM.get_index_type_with_index_name(table_name, index_name);
			// notice that IM should be called first, delete indices then delete table catalog for it will use catalog manage first
			IM.drop_index(t, index_name);
			CM.drop_index_with_index_name(table_name, index_name);

			ret_string += "Query OK, 0 rows affected\n";
		} else {
			ret_string += "Error: have no such index\n";
		}
	} else {
		ret_string += "Error: have no such table\n";
	}


	return ret_string;
}

string API::show_tables() {
	vector<string> result = CM.show_tables();
	string ret_string;
	ret_string += "+---------+----------+\n";
	ret_string += "| Tables in Database |\n";
	ret_string += "+---------+----------+\n";
	for (size_t i = 0; i < result.size(); i++) {
		string temp_string = result[i];
		ret_string += "| ";
		if (temp_string.size() > 18) {
			ret_string += temp_string.substr(0, 15);
			ret_string += "...";
		} else {
			ret_string += temp_string;
			for (size_t i = 0; i < 18 - temp_string.size(); i++) ret_string += " ";
		}
		ret_string += " |\n";
	}
	ret_string += "+---------+----------+\n";
	stringstream tempstream;
	tempstream << result.size();
	ret_string += tempstream.str();
	ret_string += " tables in database\n";
	return ret_string;
}

string API::show_status() {
	std::vector<std::string> tableNames = CM.show_tables();
	for (auto i = tableNames.begin(); i != tableNames.end(); i++) {
		std::cout << *i << std::endl;
		CM.show_fields(*i);
		std::cout << IM.get_real_record_amount(*i) << std::endl;
		std::cout << std::endl;
	}


	string ret_string;
	ret_string += "Status flag in Managers\n";
	return ret_string;
}

vector<int> API::and_indexs(vector<int> indexs_1, vector<int> indexs_2) {
	vector<int> ret_indexs;
	if (indexs_1.size() == 1 && indexs_1[0] < 0) {
		switch (indexs_1[0]) {
		case -1:
			//std::cout << "Error: have no such table" << std::endl;
			return ret_indexs;
			break;
		case -2:
			//std::cout << "Error: have no such column" << std::endl;
			return ret_indexs;
			break;
		case -3:
			//std::cout << "Error: type doesn't match" << std::endl;
			return ret_indexs;
			break;
		}
	}
	if (indexs_2.size() == 1 && indexs_2[0] < 0) {
		switch (indexs_2[0]) {
		case -1:
			std::cout << "Error: have no such table" << std::endl;
			return ret_indexs;
			break;
		case -2:
			std::cout << "Error: have no such column" << std::endl;
			return ret_indexs;
			break;
		case -3:
			std::cout << "Error: type doesn't match" << std::endl;
			return ret_indexs;
			break;
		}
	}

	set_intersection(indexs_1.begin(), indexs_1.end(), indexs_2.begin(), indexs_2.end(), back_inserter(ret_indexs));
	return ret_indexs;
}

char* get_sql_from_bison() {
	return sql_from_bison;
}

void clean_sql_from_bison() {
	memset(sql_from_bison, 0, 1000 * sizeof(char));
	first_flag = 0;
}
