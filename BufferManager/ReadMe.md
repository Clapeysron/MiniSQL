# Buffer Manager

Author @Melody

# 主要功能

1. 按照上层提供的 index 访问指定 block ，并把多余内容处理掉直接返回。

2. 一个文件一个 BufferUnit， 不同 Block 设置不同的 lock 标志位，当 lock flag 为 1 的时候直接返回读取失败

3. 将读取到的内容按照 LRU 近似算法( Clock )缓存，提高命中率、加快访问速度。

4. 在写文件时不先写文件，而是操作内存以加快速度， 设置一个 edited 标志位，若 edited flag 为 1 则将该 block 重新写回指定位置。

# 对外接口

## createFile

创建一个空文件

```
    bool createFile(string filename); // Create a file.
```

创建空文件， filename 为文件名。

## deleteFile

删除文件喽

```
    bool deleteFile(string filename); // Create a file.
```

## readDataFromFile

从指定文件的指定 block 读入内存，需要注意的一点是，在 blockSize 确定之后就要一直使用相同的 blockSize 来读取相同的文件，因为 bufferManager 有个自动对齐的操作，文件大小会自动对齐到 block 的整数倍， 并且需要读取的内容不可能在两个 block 中，也就是不存在跨区段记录。所以 blockSize 很重要。

```
    bool readDataFromFile(string filename, int blockIndex, char *readBuffer);
```

filename 指定了要读取的文件名，无须担心 buffer 是否建立的问题，如果没有建立的话会自动建立。

blockIndex 是需要读取的第几个 block 的意思。

readBuffer 由上层使用该接口的提供，读取进来的数据会被放在 readBuffer 之中。

## writeDataToFile

顾名思义，向指定 blockIndex 处写入指定数据。大小就是 blockSize

```
    bool writeDataToFile(string filename, int blockIndex, char *writeBuffer); 
```

前两个参数同上， writeBuffer 指的是要写的数据对应的内存指针。

## lockBlock

需要时使用，如果 lock 一个 block 那么该 block 不可被写。

```
	bool lockBlock(string filename, int blockIndex, int lock);
```

前两个参数同上上， lock 指的是要设定的标志值， 1 的话是 lock， 0 的话 unlock。

## deleteLastBlockFromFile

该接口用于删除文件的最后一个 block。

为什么要有这么一个接口是因为我的设计中，删除一个记录是把最后一条记录给移动到前面去，但是暂时并不删除最后一条记录，只是更新记录的数量，让最后一条记录变得不会被访问而已，在文件最后一个 block 全部被移动到前面之后再删除这个 block，主要目的是减少文件 IO。

```
	bool deleteLastBlockOfFile(string filename);
```

参数同上上上。

## 构造函数

构造函数设定了 blockSize 和 bufferSize(也就是 block 的数量)

## 析构函数

什么都没做。