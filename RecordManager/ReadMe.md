# Record Manager

Author @Melody

# 主要功能

1. 表的创建与删除
2. 记录的插入
3. 记录的查找（考虑多条件查找）
4. 实现从 BufferManager 返回的内容中记录的提取与保存

# 对外接口

## createTable

```
    bool createTable(string tableName);
```

createTable 传入一个表名，会对应的创建一个空的文件，文件名为 tableName.MYD

## dropTable

```
    bool dropTable(string tableName);
```

dropTable 是 createTable 的反操作

## insertIntoTable

插入一条记录

```
    int insertIntoTable(TableStruct &ts, char* data);
```

传入一个表的结构，这个我在 TableStruct 里面定义过了。

```
class TableStruct{
public:
    string name; // table name
    int recordAmount; // record's amount
    vector<AttrInfo> attrs; // the information of columns;

    TableStruct(){

    };

    TableStruct(string name, int recordAmount, vector<AttrInfo> attrs):name(name), recordAmount(recordAmount), attrs(attrs){

    };
};

```

其中对于 AttrInfo 的定义如下

```
class AttrInfo{
public:
    string name; // attribute name
    int type; // attribute type(int, float, varchar)
    int length;
    bool unique; // is it unique
    bool index; // is it in index column.

    AttrInfo(){

    };

    AttrInfo(string name, int type, int length , bool unique, bool index):name(name), type(type), length(length) , unique(unique), index(index){

    };
};
```

而 data 这块内存需要按照 一条记录的顺序排好。再调用 insertIntoTable 即可，下面是一个 demo。

```
    BufferManager bm = BufferManager(32, 4);
    RecordManager rm = RecordManager(bm, 32);
    rm.createTable("lalala");
    vector<AttrInfo> ats;
    ats.push_back(AttrInfo("username", 1, 11, 0, 0));
    ats.push_back(AttrInfo("password", 1, 11, 0, 0));
    ats.push_back(AttrInfo("id", 0, 3, 1, 1));
    TableStruct ts("lalala", 3, ats);
    char *data = new char[26];

    memset(data, 0, 26);
    strcpy(data, "melody");
    strcpy(data + 11, "melody123");
    *(int *)(data + 22) = 0;
    if(rm.insertIntoTable(ts, data) != -1){
        cout << "Insert successfully" << endl;
    }
```

## deleteRecord

```
    int deleteRecord(TableStruct &ts, vector<int> &scope, vector<int> &moved);
```

ts 的说明同上，关于 scpoe 则是一个存储了需要删除的元素的 index 的数组， index 从 0 开始的噢！ moved 会返回需要更新的索引。

moved 的存在的原因是，删除记录的原理为把最后一条记录移动到当前要删除的记录位置，这样就能既没有空洞又不需要更新大量的索引了。

moved 中两个为一对，比如 moved[0] 为要更新的索引位置， moved[1] 为新的索引位置，以此类推，

## selectRecord

选择记录

```
	bool selectRecord(TableStruct &ts, vector<int> &scope, vector<char *> &result);
```

ts 说明同上上， scope 为需要选择的记录的 index， index 从 0 开始。

result 会返回一个包含了结果集的字符串指针数组，请务必在使用了结果之后将这些指针一一 delete[]。

## selectAttribute

选择一整列属性。

```
    bool selectAttribute(TableStruct &ts, string col, vector<char *> &values);
```

这是为建立索引提供便利的一个方法， ts 的说明同上上上， col 为字段名， values 为查询结果，查询结果是按照文件中第几条记录的顺序排列的，请务必在使用了结果之后将指针 delete[]

顺带构造函数可以定义 blockSize~。在使用 RecordManager 之前要先初始化 BufferManager ，再将其传入 RecordManager 的构造函数中。