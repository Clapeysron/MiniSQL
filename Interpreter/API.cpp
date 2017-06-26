//
//  API.cpp
//  Interpreter
//
//  Created by Clapeysron on 03/06/2017.
//  Copyright © 2017 Clapeysron. All rights reserved.
//

#include "API.hpp"
#include <algorithm>


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
	//    while(sql_stream >> buf)
	//    {
	//        cout << buf << ' ';
	//    }
	//    cout << endl;
	//    return "abc";
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
                sql_stream >> buf; // colname
                col_list.push_back(buf);
            }
            ret_string = select_all(table_name, col_list);
            return ret_string;
        }
        else if (buf == "WHERE")
        {
            while (buf != "81")
            {
                sql_stream >> buf;
                vector<vector<int>> indexs_list;
                if (buf == "AND")
                {
                    sql_stream >> buf;
                    if (buf == "CMP")
                    {
                        int comparison_type, type_1, type_2;
                        string comp_1, comp_2;
                        sql_stream >> comparison_type >> type_1 >> comp_1 >> type_2 >> comp_2;
                        indexs_list.push_back(search_where(table_name, comparison_type, type_1, comp_1, type_2, comp_2));
                        continue;
                    }
                    else if (buf == "BETWEEN")
                    {
                        int type_1, type_2, type_3;
                        string comp, between_1, between_2;
                        sql_stream >> type_1 >> between_1 >> type_2 >> between_2 >> type_3 >> comp;
                        indexs_list.push_back(search_between(table_name, type_1, comp, type_2, between_1, type_3, between_2));
                        continue;
                    }
                }
                else if (buf == "CMP")
                {
                    int comparison_type, type_1, type_2;
                    string comp_1, comp_2;
                    sql_stream >> comparison_type >> type_1 >> comp_1 >> type_2 >> comp_2;
                    indexs_list.push_back(search_where(table_name, comparison_type, type_1, comp_1, type_2, comp_2));
                }
                else if (buf == "BETWEEN")
                {
                    int type_1, type_2, type_3;
                    string comp, between_1, between_2;
                    sql_stream >> type_1 >> between_1 >> type_2 >> between_2 >> type_3 >> comp;
                    indexs_list.push_back(search_between(table_name, type_1, comp, type_2, between_1, type_3, between_2));
                }
                vector<string> col_list;
                for (int i = 0; i < select_list_num; i++)
                {
                    sql_stream >> buf; // NAME
                    sql_stream >> buf; // colname
                    col_list.push_back(buf);
                }
                ret_indexs=indexs_list[0];
                for (int i=0; i<indexs_list.size(); i++)
                {
                    ret_indexs = and_indexs(ret_indexs, indexs_list[i]);
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
                
                ret_indexs=indexs_list[0];
                for (int i=0; i<indexs_list.size(); i++)
                {
                    ret_indexs = and_indexs(ret_indexs, indexs_list[i]);
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
                ret_indexs=indexs_list[0];
                for (int i=0; i<indexs_list.size(); i++)
                {
                    ret_indexs = and_indexs(ret_indexs, indexs_list[i]);
                }
                ret_string = delete_part(table_name, ret_indexs);
                return ret_string;
            }
        }
    else if (buf == "CREATEINDEX") {
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
	// TODO: API select_all
	// bool CM.have_table(table_name);
	// bool CM.have_column(col_list);
	// string RM.select_all(table_name);
	// string > out_string
	ret_string += "+-------+-------+\n";
	ret_string += "| s_ID  | i_ID  |\n";
	ret_string += "+-------+-------+\n";
	ret_string += "| 12345 | 10101 |\n";
	ret_string += "+-------+-------+\n";
	ret_string += "9 rows in set (0.00 sec)\n";
	return ret_string;
}

string API::select(string table_name, vector<string> col_list, vector<int> indexs) {
	string ret_string;
	// TODO: API select_all
	// bool CM.have_table(table_name);
	// bool CM.have_column(col_list);
	// string RM.select_part(table_name, indexs);
	// string > out_string
	ret_string += "+-------+-------+\n";
	ret_string += "| s_ID  | i_ID  |\n";
	ret_string += "+-------+-------+\n";
	ret_string += "| 12345 | 10101 |\n";
	ret_string += "+-------+-------+\n";
	ret_string += "9 rows in set (0.00 sec)\n";
	return ret_string;
}

vector<int> API::search_between(string table_name, int type_1, string comp, int type_2, string between_1, int type_3, string between_2) {
	// TODO: API search_between
	vector<int> ret_indexs;
	if (!CM.have_table(table_name)) {
		std::cerr << "no such table" << std::endl;
		ret_indexs.push_back(-1); // -1 for no table_name
		return ret_indexs;
	} else {
		TableInfo& temp_table = CM.find_table(table_name);
		if (type_1 == 81) {
			if (!temp_table.have_column(comp)) {
				std::cerr << "no such column" << std::endl;
				ret_indexs.push_back(-1); // -1 for no ret_indexs and no such column;
				return ret_indexs;
			} else {
				FieldInfo temp_field = temp_table.get_column(comp);
				std::stringstream ss;
				ss << between_1 << between_2;

				//std::string temp_index = temp_table.find_index(comp);
				if (type_2 == temp_field.get_type_magic_num() && type_3 == temp_field.get_type_magic_num()) {
					if (temp_table.have_index(comp)) {
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
							std::cout << "no such type_2" << std::endl;
							ret_indexs.push_back(-2); // -2 for no such type;
							return ret_indexs;
						}
					} else {
						//TODO: to be implemented by melody;
						std::cout << "dfz have no index, to be fixed by melody" << std::endl;
					}
				} else {
					std::cout << "type not match" << std::endl;
					ret_indexs.push_back(-3); // -3 for type not match
					return ret_indexs;
				}

			}
		} else {
			//TODO: to implement 3=3, 3=a 
			ret_indexs.push_back(-2); // -2 for not a column;
			return ret_indexs;
		}
	}

	return ret_indexs;
}



vector<int> API::search_where(string table_name, int comparison_type, int type_1, string comp_1, int type_2, string comp_2) {
	vector<int> ret_indexs;
	// TODO: API search_where
	//std::vector<int> result;
	if (!CM.have_table(table_name)) {
		std::cerr << "no such table" << std::endl;
		ret_indexs.push_back(-1); // -1 for no result
		return ret_indexs;
	} else {
		TableInfo& temp_table = CM.find_table(table_name);
		if (type_1 == 81) {
			if (!temp_table.have_column(comp_1)) {
				std::cout << "no such column" << std::endl;
				ret_indexs.push_back(-2); // -1 for no such column 
				return ret_indexs;
			} else {
				FieldInfo temp_field = temp_table.get_column(comp_1);
				if (type_2 == temp_field.get_type_magic_num()) {
					if (temp_table.have_index(comp_1)) {

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
							std::cerr << "no such type_2" << std::endl;
							ret_indexs.push_back(-3); // -1 for no result
							return ret_indexs;
						}
					} else {
						// TODO: to be fixed by melody
						return ret_indexs;
					}
				} else {
					std::cout << "type not match" << std::endl;
					ret_indexs.push_back(-3); // -3 for no type
					return ret_indexs;
				}

			}
		} else {
			//TODO: to implement 3=3, 3=a
			std::cout << "no such table" << std::endl;
			ret_indexs.push_back(-1); // -1 for no type
			return ret_indexs;
		}
	}
	return ret_indexs;
}

string API::delete_all(string table_name) {
	// TODO: API delete_all
	string ret_string;
	if (CM.have_table(table_name)) {
		// int IM.delete_all(table_name);
		// string RM.delete_all(table_name);
		ret_string += "Query OK, 1 row affected (0.01 sec)\n";
	} else {
		ret_string += "Error: have no such table\n";
	}

	return ret_string;
}

string API::delete_part(string table_name, vector<int> indexs) {
	// TODO: API delete_part

	string ret_string;
	if (CM.have_table(table_name)) {
		// int IM.delete_part(table_name, indexs);
		// string RM.delete_part(table_name, indexs);
		ret_string += "Query OK, 3 row affected (0.02 sec)\n";
	} else {
		ret_string += "Error: have no such table\n";
	}

	return ret_string;
}

string API::insert(string table_name, vector<int> type_list, vector<string> value_list) {
	std::reverse(type_list.begin(), type_list.end());
	std::reverse(value_list.begin(), value_list.end());


	// TODO: API insert
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
					ret_string += "'" + value_list[i] + "' 's length is larger than maximum";
					return ret_string;
				}
				bool index = fields[i].get_index() == "" ? false : true;
				ats.push_back(AttrInfo(fields[i].get_name(), type_list[i], length, fields[i].get_unique(), index));
				buffsize += length;
				length_list.push_back(length);
			}
			TableStruct ts(table_name, IM.get_record_size(table_name), ats);

			char * data = new char[buffsize];
			int offset = 0;
			for (size_t i = 0; i < value_list.size(); i++) {
				memcpy(data + offset, value_list[i].data(), value_list.size());
				offset += length_list[i];
			}

			RM.insertIntoTable(ts, data);
			//TODO: implement delete indexManager 
			//IM.insert("", "", std::vector<int>(), std::vector<int>());

			//No need for delete[]data because insertIntoData have done this;
		} else {
			ret_string += "Error: values' type doesn't match table'" + table_name + "'\n";
			return ret_string;
		}

	} else {
		ret_string += "Error: have no such table\n";
		return ret_string;
	}

	ret_string += "Query OK, 3 row affected (0.02 sec)\n";
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
	ret_string += "Query OK, 3 row affected (0.02 sec)\n";
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
	ret_string += "Query OK, 3 row affected (0.02 sec)\n";
	return ret_string;
}

string API::create(string table_name, vector<string> name_list, vector<int> type_list, vector<int> length_list, vector<int> primary_flag, vector<int> unique_flag, vector<int> nnull_flag) {

	std::reverse(name_list.begin(), name_list.end());
	std::reverse(type_list.begin(), type_list.end());
	std::reverse(length_list.begin(), length_list.end());
	std::reverse(primary_flag.begin(), primary_flag.end());
	std::reverse(unique_flag.begin(), unique_flag.end());
	std::reverse(nnull_flag.begin(), nnull_flag.end());


	//TODO: primary index should be made;
	string ret_string;
	if (table_name.size() > 20) {
		ret_string += "ERROR : Table name should be less than 20 \n";
	} else {
		if (!CM.have_table(table_name)) {
			CM.create_table(table_name, name_list, type_list, length_list, primary_flag, unique_flag, nnull_flag);
			ret_string += "Query OK, 0 rows affected (0.05 sec)\n";

		} else {
			ret_string += "ERROR : Table '" + table_name + "' already exists\n";
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
		ret_string += "ERROR: can only create index for one col at a time!\n";
		return ret_string;
	}

	if (CM.have_table(table_name)) {
		
		//std::string real_index_name = index_name + "_index_" + col_list[0];
		if (!CM.have_index(table_name, index_name)) {
			//TODO: get keys from RecordManger
			
			//IM.create_index(index_name, table_name, col_list[0], keys);
			ret_string += "create index : needs melody's help";
		} else {
			ret_string += "Error: have same index name '" + index_name + "'";
		}

		ret_string += "Query OK, 0 rows affected (0.03 sec)\n";
	} else {
		ret_string += "Error: have no such table\n";
	}
	

	return ret_string;
}

string API::drop_table(string table_name) {
	// TODO: API drop_table
	string ret_string;
	if (CM.have_table(table_name)) {
		CM.drop_table(table_name);
		IM.drop_table(table_name);
		RM.dropTable(table_name);
		//RM.drop_table();
		// TODO: RM drop table
		//TODO: fix 0 rows
		ret_string += "Query OK, 0 rows affected (0.01 sec)\n";
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
			Type t = CM.drop_index_with_index_name(table_name, index_name);
			IM.drop_index(t, index_name);
			ret_string += "Query OK, 0 rows affected (0.01 sec)\n";
		} else {
			ret_string += "ERROR: have no such index\n";
		}
	} else {
		ret_string += "ERROR: have no such table\n";
	}

	// notice that IM should be called first, delete indices then delete table catalog
	// why?????

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
			for (int i = 0; i < 18 - temp_string.size(); i++) ret_string += " ";
		}
		ret_string += " |\n";
	}
	ret_string += "+---------+----------+\n";
	stringstream tempstream;
	tempstream << result.size();
	ret_string += tempstream.str();
	ret_string += " tables in database (0.00 sec)\n";
	return ret_string;
}

string API::show_status() {
	std::vector<std::string> tableNames = CM.show_tables();
	for (auto i = tableNames.begin(); i != tableNames.end(); i++) {
		std::cout << *i << std::endl;
		CM.show_fields(*i);
		std::cout << std::endl;
	}
	//CM.show_fields("test");
	string ret_string;
	ret_string += "Status flag in Managers\n";
	return ret_string;
}

vector<int> API::and_indexs(vector<int> indexs_1, vector<int> indexs_2) {
	vector<int> ret_indexs;
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
