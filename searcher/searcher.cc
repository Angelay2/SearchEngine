#include "search.h"
#include "../common/util.hpp"
#include <boost/algorithm/string/case_conv.hpp>
#include <fstream>
#include <string>

namespace searcher{

const char* const DICT_PATH = "../jieba_dict.utf8";
const char* const HMM_PATH = "../jiaba_dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../jieba_dict/idf.utf8";
const char* const IDF_PATH = "../jieba_dict/idf.utf8";
const char* const STOP_WORD_PATH = "../jieba_dict/stop_words.utf8";

Index::Index()
    :jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH){}
    const DocInfo*Index::GetDocInfo(int64_t doc_id){
        if(doc_id < 0 || doc_id >= forword_index.size()){
            return nullptr;
        }
        return &forword_index[doc_id];
    }
    const InvertedList* Index::GetInvertedList(const string &key){
        //unordered_map<string, InvertedList>::iterator it = inverted_index.find(key);
        auto it = inverted_index.find(key);
        if(it == inverted_index.end()){
            return nullptr;
        }
        return &it->second;
    }
    // 上面的两个函数实现起来都比较容易, 接下来的这个函数(构建索引),实现起来就要更复杂
    bool Index::Build(const string& input_path){
        // 1. 按行读取输入文件内容(上个环节预处理模块生产的raw_input文件)
        //  raw_input 的结构: 是一个行文本文件, 每一行对应一个问的那个
        //  每一行有分成三个部分, 使用\3来切分, 分别是标题, url, 正文
        std::cout << "开始构建索引" << std::endl;
        std::ifstream file(input_path.c_str());
        if(!file.is_open()){
            std::cout << "raw_input 文件打开失败" << std::endl;
            return false;
        }
        string line;
        while(std::getline(file,line)){
        //  2. 针对当前行, 解析成 DocInfo 对象, 并构造为正排索引
            DocInfo* doc_info = BuildForward(line);
            if(doc_info == nullptr){
                // 当前文档构造正排出现问题
                std::cout << "构建正排失败！" << std::endl;
                continue;
            }
        }
        //  3. 根据当前的 DocInfo 对象, 进行解析, 构造成倒序索引
        BuildInverted(*doc_info);
    }
    std::cout << "结束构建索引" << std::endl;
    file.close();
    return true;
}

// 核心操作： 按照 \3 对 line 进行切分，第一个部分就是标题， 第二个部分就是 url， 第三个部分就是正文
// C++标准库中， 也没有字符串切分的操作
DocInfo* Index::BuildForward(const string &line){
    // 1. 先把 line 按照 \3 切分成 3 个部分
    vector<string> tokens;
    common::Util::Split(line, "\3", &tokens);
    if(tokens.size() != 3){
        // 如果切分结果不是 3 份，就认为当前这一行是存在问题的
        // 认为该文档构造失败
        return nullptr;
    }
    // 2. 把切分结果填充到 DocInfo 对象中
    DocInfo doc_info;
    doc_info.doc_id  = forward_index.size();
    doc_info.title = tokens[0];
    doc_info.url = tokens[1];
    doc_info.content = tokens[2];
    forward_index.push_back(std::move(doc_info));
    // 3. 返回结果
    // 注意体会这里可能存在的野指针问题， C++中的经典错误
    // 也是面试中的红药考点 return &doc_info;
    return &forward_index.back();
}

// 倒排是一个 hash 表
// key 是词（针对文档的分词结果）
// value 是倒排拉链（包含若干个 Weight 对象）
// 每次遍历到一个文档， 就要分析这个文档， 并且把相关信息， 更新到倒排结构当中
void Index::BuildInverted(const DocInfo &doc_info){
    // 0.创建专门用于统计词频的结构
    struct WordCnt{
        int title_cnt;
        int content_cnt;
        WordCnt():title_cnt(0), content_cnt(0){}
    };
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
