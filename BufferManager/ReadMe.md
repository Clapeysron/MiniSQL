# Buffer Manager

@Author Melody

# 主要功能

1. 按照上层提供的 index 访问指定 block ，并把多余内容处理掉直接返回。

2. 一个文件一个 BufferManager， 不同文件设置不同的 lock 标志位，当 lock flag 为 1 的时候直接返回读取失败

3. 将读取到的内容按照 LRU 算法缓存，提高命中率、加快访问速度。

4. 在写文件时不先写文件，而是操作内存以加快速度， 设置一个 edited 标志位，若 edited flag 为 1 则将该 block 重新写回指定位置。

5. 因为无法做到全面的考虑 Index 的情况，暂时不考虑 Index 文件的读取方法。希望这部分由写 Index 的同学完成。



