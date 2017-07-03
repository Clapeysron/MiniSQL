//
// Created by Melody on 2017/5/24 0024.
//

#include "RecordManager.h"
#include <sstream>

int RecordManager::getRecordLen(TableStruct &ts) {
	vector<AttrInfo> &attrs = ts.attrs;
	int length = 0;
	for (size_t i = 0; i < attrs.size(); ++i) {
		length += attrs[i].length;
	}
	return length;
}

bool RecordManager::isConditionSatisfied(TableStruct &ts, Condition &con, char *str) {

	// calculate the position of the named attribute in the binary data
	// this can be done before this main loop for each conditions
	// i will leave it for the sake of simplicity
	vector<AttrInfo> &attrs = ts.attrs;
	int start = 0;
	int length = 0;
	int type = 0;
	int pointer = 0;
	for (size_t i = 0; i < attrs.size(); i++) {
		if (attrs[i].name == con.column) {
			start = pointer;
			length = attrs[i].length;
			type = attrs[i].type;
			break;
		}
		pointer = pointer + attrs[i].length;
	}

	// make sure that the API module will leave n+1 bytes for char(n)
	// and fill the unused bytes with 0, so no extra check here
	char *bufarea = new char[length];
	std::memcpy(bufarea, str + start, (size_t)length);

	// if the comprasion on particular type failed,
	// the COMPRASION macro returns false
	// so if it pass through, the condition holds
	switch (type) {
	case STR: {
		string &valb = con._str;
		string vala = bufarea;
		COMPRASION;
		break;
	}
	case INT: {
		int valb = con._int;
		int vala = atoi(bufarea);
		COMPRASION;
		break;
	}
	case DOUBLE: {
		std::stringstream stream;
		std::string temp(bufarea);
		stream << bufarea;
		float valb = con._flo;
		float vala;
		stream >> vala;
		COMPRASION;
		break;
	}
	default:
		break;
	}
	delete[] bufarea;

	// it must be a long way to get here, but everything is fine
	return true;
}

bool RecordManager::isDup(TableStruct &ts, char *record) {
	int recordLen = getRecordLen(ts) + 1;
	int recordAmountInOneBlock = blockSize / recordLen;
	int blockAmount = (ts.recordAmount - 1) / recordAmountInOneBlock + 1;
	string filename = GET_FILENAME(ts.name);

	vector<AttrInfo> &attrs = ts.attrs;
	vector<int> uniqueList;

	int pos = 0;
	for (size_t i = 0; i < attrs.size(); ++i) {
		if (attrs[i].unique == 1) {
			uniqueList.push_back(pos);
			uniqueList.push_back(pos + attrs[i].length);
		}

		pos = pos + attrs[i].length;
	}

	char *block = new char[blockSize];
	for (int j = 0; j < blockAmount; ++j) {
		bm.readDataFromFile(filename, j, block);
		for (int k = 0; k < recordAmountInOneBlock; ++k) {
			if (isDupEntry(uniqueList, block + k * recordLen + 1, record)) {
				return true;
			}
		}
	}

	delete[] block;
	return false;
}

bool RecordManager::isDupEntry(vector<int> &pos, char *src, char *dst) {
	for (size_t i = 0; i < pos.size(); i += 2) {
		int start = pos[i];
		int stop = pos[i + 1];
		if (!memcmp(src + start, dst + start, (size_t)(stop - start))) {
			return true;
		}
	}

	return false;
}

bool RecordManager::createTable(string tableName) {
	string filename = GET_FILENAME(tableName);
	bm.createFile(filename);
	return true;
}

bool RecordManager::dropTable(string tableName) {
	string filename = GET_FILENAME(tableName);
	bm.deleteFile(filename);
	return true;
}

int RecordManager::insertIntoTable(TableStruct &ts, char *data) {
	if (isDup(ts, data)) {
		cout << "Dup" << endl;
		return -1;
	}

	int recordLen = getRecordLen(ts);
	int recordAmountInOneBlock = blockSize / recordLen;
	char* newData = new char[recordLen + 1];
	newData[0] = 0; //The delete flag, if the value of this flag is 1 we consider this record is deleted
	memcpy(newData + 1, data, recordLen);
	recordLen++;

	string filename = GET_FILENAME(ts.name);

	if (ts.recordAmount % recordAmountInOneBlock == 0) { // It's full, write it to another new block;
		char *block = new char[blockSize];
		memcpy(block, newData, (size_t)recordLen);

		bm.writeDataToFile(filename, ts.recordAmount / recordAmountInOneBlock, block);

		delete[] block;
	} else {
		char *block = new char[blockSize];
		bm.readDataFromFile(filename, ts.recordAmount / recordAmountInOneBlock, block);
		memcpy(block + ((ts.recordAmount % recordAmountInOneBlock) * recordLen), newData, (size_t)recordLen);
		bm.writeDataToFile(filename, ts.recordAmount / recordAmountInOneBlock, block);

		//delete[] block;
	}

	return ts.recordAmount++; // the record amount will add one and the index of this record will be returned.
}

//int RecordManager::deleteRecord(TableStruct &ts, vector<int> &scope, vector<int> &moved){
//    // I assume that the scope is the final list of records which will be deleted. So Index Manager can update the
//    // index using the vector<int> scope. I just need to delete these index and I will return the moved records.
//    // My strategy of delete records is swap it to the last block and I will delete the last records.
//    int recordLen = getRecordLen(ts);
//    int recordAmountInOneBlock = blockSize / recordLen;
//    int blockAmount = (ts.recordAmount - 1) / recordAmountInOneBlock + 1;
//    int currentAmount = ts.recordAmount;
//    string filename = GET_FILENAME(ts.name);
//
//    char *block = new char[blockSize];
//
//    int currentBlock = -1;
//    bool isModified = false;
//
//    for(int i = (int)scope.size() - 1; i >= 0 ; i--){
//        if(scope[i] / recordAmountInOneBlock != currentAmount){
//            if(isModified && currentAmount > currentBlock * recordAmountInOneBlock){
//                bm.writeDataToFile(filename, currentBlock, block);
//            }
//
//            currentBlock = scope[i] / recordAmountInOneBlock;
//            bm.readDataFromFile(filename, currentBlock, block);
//            isModified = false;
//        }
//
//        int j = scope[i] % recordAmountInOneBlock;
//        char *buf = new char[blockSize];
//        bm.readDataFromFile(filename, (currentAmount - 1) / recordAmountInOneBlock, buf);
//        int index = (currentAmount - 1) % recordAmountInOneBlock;
//
//        memcpy(block + j * recordLen, buf + index * recordLen, (size_t)recordLen);
//
//
//        currentAmount--;
//        if(currentAmount % recordAmountInOneBlock == 0){
//            bm.deleteLastBlockOfFile(filename);
//        }
//        delete[] buf;
//        isModified = true;
//        if(currentAmount != scope[i]){
//            moved.push_back(currentAmount);
//            moved.push_back(scope[i]);
//        }
//
//    }
//
//    if(isModified && currentAmount > currentBlock * recordAmountInOneBlock){
//        bm.writeDataToFile(filename, currentBlock, block);
//    }
//
//    delete[] block;
//
//    int tmp = ts.recordAmount - currentAmount;
//
//    ts.recordAmount = currentAmount;
//
//    return tmp;
//}

bool RecordManager::selectAll(TableStruct &ts, vector<string> &result, vector<string> &col_list) {
	int recordLen = getRecordLen(ts) + 1;
	int recordAmountInOneBlock = blockSize / recordLen;
	int blockAmount = (ts.recordAmount - 1) / recordAmountInOneBlock + 1;

	if (col_list.size() == 1 && col_list[0] == "*") {
		col_list.clear();
		for (size_t i = 0; i < ts.attrs.size(); i++) {
			col_list.push_back(ts.attrs[i].name);
		}
	}

	vector<int> pointers;

	std::string filename = GET_FILENAME(ts.name);
	result.clear();

	char* block = new char[blockSize];
	std::vector<int> width;
	string newline = "+";
	string secondline = "|";

	for (int j = 0; j < col_list.size(); ++j) {
		int start = 0;
		for (int i = 0; i < ts.attrs.size(); ++i) {
			if (ts.attrs[i].name != col_list[j]) {
				start += ts.attrs[i].length;
				continue;
			}
			pointers.push_back(start);
			pointers.push_back(ts.attrs[i].length);
			width.push_back(ts.attrs[i].length);
			string space((unsigned long)ts.attrs[i].length, '-');
			newline = newline + space + "+";
			secondline += ts.attrs[i].name;
			string columnSpace(ts.attrs[i].length - ts.attrs[i].name.length(), ' ');
			secondline += columnSpace + "|";
		}
	}

	newline += "\n";
	secondline += "\n";
	result.push_back(newline);
	result.push_back(secondline);
	result.push_back(newline);
	int currentBlock = -1;
	if (ts.recordAmount != 0) {
		for (int i = 0; i < ts.recordAmount; ++i) {
			if (i / recordAmountInOneBlock != currentBlock) {
				currentBlock = i / recordAmountInOneBlock;
				bm.readDataFromFile(filename, currentBlock, block);
			}

			int j = i % recordAmountInOneBlock;

			char *target = new char[recordLen - 1]; // this target is also needed to be freed in the higher place.
			char * target_begin = target;
			if (*(block + j * recordLen) == 1) {
				continue;
			}
			memcpy(target, block + j * recordLen + 1, (size_t)(recordLen - 1));
			string then = "|";
			for (int k = 0; k < (pointers.size() / 2); ++k) {
				string this_line(target + pointers[k * 2]);
				string this_space(pointers[k * 2 + 1] - this_line.length(), ' ');
				then = then + this_line + this_space + "|";
			}
			then += "\n";
			result.push_back(then);
			//result.push_back(newline);
			delete[] target_begin;
		}

		delete[] block;
		result.push_back(newline);
	}
	return true;

}
bool RecordManager::selectRecordWithCondition(TableStruct &ts, vector<int> &results, int comparison_type, int type_1, string &comp_1, int type_2, string comp_2) {
	int recordLen = getRecordLen(ts) + 1;
	int recordAmountInOneBlock = blockSize / recordLen;
	int blockAmount = (ts.recordAmount - 1) / recordAmountInOneBlock + 1;
	string filename = GET_FILENAME(ts.name);

	results.clear();
	Condition co;
	co.op = comparison_type;
	if (type_1 != COL) {
		return false;
	} else {
		co.column = comp_1;
		switch (type_2) {
		case INT: {
			std::stringstream buff;
			buff << comp_2;
			buff >> co._int;
			break;
		}
		case DOUBLE: {
			std::stringstream buff;
			buff << comp_2;
			buff >> co._flo;
			break;
		}
		case STR:
			co._str = comp_2;
			break;
		default:
			return false;
		}
	}

	char* block = new char[blockSize];

	int currentBlock = -1;
	for (int i = 0; i < blockAmount; ++i) {
		bm.readDataFromFile(filename, i, block);

		for (int j = 0; (j < recordAmountInOneBlock) && (i * recordAmountInOneBlock + j < ts.recordAmount); ++j) {
			if (*(block + j * recordLen) == 1) {
				continue;
			}
			if (isConditionSatisfied(ts, co, block + j * recordLen + 1)) {
				results.push_back(i * recordAmountInOneBlock + j);
			}
		}
	}

	delete[] block;
	return true;
}

int RecordManager::deleteRecord(TableStruct &ts, vector<int> &scope) {
	int recordLen = getRecordLen(ts) + 1;
	int recordAmountInOneBlock = blockSize / recordLen;
	int blockAmount = (ts.recordAmount - 1) / recordAmountInOneBlock + 1;
	string filename = GET_FILENAME(ts.name);



	char* block = new char[blockSize];

	int currentBlock = -1;
	for (int i = 0; i < scope.size(); ++i) {
		if (scope[i] / recordAmountInOneBlock != currentBlock) {
			currentBlock = scope[i] / recordAmountInOneBlock;
			bm.readDataFromFile(filename, currentBlock, block);
		}

		int j = scope[i] % recordAmountInOneBlock;

		block[j * recordLen] = 1;
		bm.writeDataToFile(filename, currentBlock, block);
		// Remeber to delete the buf.
	}
	delete[] block;
	return (int)scope.size();
}

bool RecordManager::selectRecord(TableStruct &ts, vector<string> &col_list, vector<int> &scope, vector<string> &result) {
	int recordLen = getRecordLen(ts) + 1;
	int recordAmountInOneBlock = blockSize / recordLen;
	int blockAmount = (ts.recordAmount - 1) / recordAmountInOneBlock + 1;
	string filename = GET_FILENAME(ts.name);

	result.clear();

	if (col_list.size() == 1 && col_list[0] == "*") {
		col_list.clear();
		for (size_t i = 0; i < ts.attrs.size(); i++) {
			col_list.push_back(ts.attrs[i].name);
		}
	}

	std::vector<int> width;
	string newline = "+";
	string secondline = "|";
	vector<int> pointers;
	for (int j = 0; j < col_list.size(); ++j) {
		int start = 0;
		for (int i = 0; i < ts.attrs.size(); ++i) {
			if (ts.attrs[i].name != col_list[j]) {
				start += ts.attrs[i].length;
				continue;
			}
			pointers.push_back(start);
			pointers.push_back(ts.attrs[i].length);
			width.push_back(ts.attrs[i].length);
			string space((unsigned long)ts.attrs[i].length, '-');
			newline = newline + space + "+";
			secondline += ts.attrs[i].name;
			string columnSpace(ts.attrs[i].length - ts.attrs[i].name.length(), ' ');
			secondline += columnSpace + "|";
		}
	}

	newline += "\n";
	secondline += "\n";
	result.push_back(newline);
	result.push_back(secondline);
	result.push_back(newline);

	char* block = new char[blockSize];

	int currentBlock = -1;
	for (int i = 0; i < scope.size(); ++i) {
		if (scope[i] / recordAmountInOneBlock != currentBlock) {
			currentBlock = scope[i] / recordAmountInOneBlock;
			bm.readDataFromFile(filename, currentBlock, block);
		}

		int j = scope[i] % recordAmountInOneBlock;
		if (*(block + j * recordLen) == 1) {
			continue;
		}
		char *target = new char[recordLen - 1]; // this target is also needed to be freed in the higher place.
		memcpy(target, block + j * recordLen + 1, (size_t)(recordLen - 1));
		string then = "|";
		for (int k = 0; k < (pointers.size() / 2); ++k) {
			string this_line(target + pointers[k * 2]);
			string this_space(pointers[k * 2 + 1] - this_line.length(), ' ');
			then = then + this_line + this_space + "|";
		}
		then += "\n";
		result.push_back(then);
		delete[] target;
	}

	delete[] block;

	result.push_back(newline);

	return true;
}

bool RecordManager::selectAttribute(TableStruct &ts, string col, vector<string> &values) {
	int recordLen = getRecordLen(ts) + 1;
	int recordAmountInOneBlock = blockSize / recordLen;
	int blockAmount = (ts.recordAmount - 1) / recordAmountInOneBlock + 1;
	string filename = GET_FILENAME(ts.name);

	values.clear();
	int start = 0, length = 0, pointer = 0;
	vector<AttrInfo> &attrs = ts.attrs;
	for (size_t i = 0; i < attrs.size(); ++i) {
		if (attrs[i].name == col) {
			start = pointer;
			length = attrs[i].length;
			break;
		}
		pointer += attrs[i].length;
	}

	char *block = new char[blockSize];

	for (int j = 0; j < blockAmount; ++j) {
		bm.readDataFromFile(filename, j, block);
		for (int k = 0; k < recordAmountInOneBlock && j * recordAmountInOneBlock + k < ts.recordAmount; ++k) {
			if (block[k * recordLen] == 1) {
				continue;
			}

			char *buf = new char[length]; // this buf will be pushed into the vector, in the higher place you should free all of them.
			memcpy(buf, block + k * recordLen + start + 1, (size_t)length);
			string buffer(buf);
			values.push_back(buffer);
		}
	}

	delete[] block;
	return true;
}


