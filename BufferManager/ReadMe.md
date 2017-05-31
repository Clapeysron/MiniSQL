# Buffer Manager

Author：@Melody

# 主要功能

1. 按照上层提供的 index 访问指定 block ，并把多余内容处理掉直接返回。

2. 一个文件一个 BufferManager， 每一个block有其对应的一个lock标志位，当 lock flag 为 1 的时候直接返回读取失败

3. 将读取到的内容按照 LRU 算法缓存，提高命中率、加快访问速度。

4. 在写文件时不先写文件，而是操作内存以加快速度， 设置一个 edited 标志位，若 edited flag 为 1 则将该 block 重新写回指定位置。

5. 因为无法做到全面的考虑 Index 的情况，暂时不考虑 Index 文件的读取方法。希望这部分由写 Index 的同学完成。

# 主要架构

主模块 BufferManager 管理多个 BufferUnit ，BufferUnitManager 管理多个 Block。每个文件都有一个 BufferUnit，这样可以加快多个表读写时的效率。


# BufferManager

对外接口如下。


```
	bool createFile(string filename); // Create a file.
    
	bool deleteFile(string filename); // Delete a file.
    
	bool readDataFromFile(string filename, int blockIndex, char *readBuffer); // Read the specific blockIndex data in the file to the memory.
    
	bool writeDataToFile(string filename, int blockIndex, char *writeBuffer); // Write the data to the specific blockIndex to the file from the memory.
    
	bool lockBlock(string filename, int blockIndex); // Lock or Unlock the Block in the specific file.
```

