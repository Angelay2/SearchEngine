#include "search.h"

namespace searcher{
    const DocInfo*Index::GetDocInfo(int64_t doc_id){
        if(doc_id < 0 || doc_id >= forword_index.size()){
            return nullptr;
        }
        return &forword_index[doc_id];
    }
    const InvertedList* Index::GetInvertedList(const string &key){
        unordered_map<string, InvertedList>::iterator it = inverted_index.find(key);
        auto it = inverted_index.find(key);
        if(it == inverted_index.end()){
            return nullptr;
        }
        return it->second;
    }
    // 上面的两个函数实现起来都比价哦容易, 接下来的这个个函数(构建索引),实现起来就要更复杂
    bool Index::Build(const string& input_path){
        // 1. 按行读取输入文件内容(上个环节预处理模块生产的raw_input文件)
        //  raw_input 的结构: 是一个行文本文件, 每一行对应一个问的那个
        //  每一行有分寸三个部分, 使用\3来切分, 分别是标题, url, 正文
        std::cout << "开始构建索引" << std::endl;
        std::ifstream file(input_path.c_str());
        if(!file.is_open()){
            std::cout << "raw_input 文件袋打开失败" << std::endl;
            return false;
        }

        //  2. 针对当前行, 解析成 DocInfo 对象, 并构造为正排索引
        //  3. 根据当前的 DocInfo 对象, 进行解析, 构造成倒序索引
    }

    // 内存泄漏会导致一个进程持有的内存持续增长,(如果增长一定程度, 不再增长 也就不会产生内存泄漏)
    class Searcher{ // 和Index 的生命周期很长 跟随整个进程 不需要delete, 这两个对象不会被重复实例化多次
        private:
            // 搜索过程依赖索引, 就需要在Searcher类中有一个 Index 的指针
            Index* index;
        public:
            Searcher() : index(new Index()){

            }
            bool Init(const string& input_path);
            bool Search(const string& query, string* results);

    };


}// namespace searcher
