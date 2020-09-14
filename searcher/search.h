/*================================================================
*   
*   文件名称：search.h
*   创 建 者：Angela
*   创建日期：2020年08月22日
*   描    述：
*
================================================================*/
#pragma once
#include "cppjieba/jieba.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdint.h>

using std::string;
using std::cout;
using std::endl;
using std::unordered_map;
using std::vector;

namespace searcher{
// 接下来的代码时索引木块的内容
// 先定义个一个基本的索引中需要用到的结构
// 这个结构是正排索引的基础
// 正排索引时给定doc_id映射到文档内容(DocInfo 对象)
struct DocInfo{
    int64_t doc_id;
    string title;
    string url;
    string content;
};
// 倒排索引时给定次, 映射到包含该词的文档id类表, (此处不光要有文档id, 还得有权重信息, 以及该词的内容)
struct Weight{
    //该词在哪个文档中宏出现
    int64_t doc_id;
    // 对应的权重是多少
    int wieght;
    // 词是啥
    string word;
};

// 这个东西叫做“倒排拉链”
typedef vector<Weight> InvertedList;

// Index类用于整个索引结构, 并且提供一些供外部调用的API
class Index{
    private:
        // 索引结构
        // 正排索引, 数组下标就对应到doc_id
        vector<DocInfo> forword_index;
        // 倒排所用, 使用一个 hash 表来表示这个映射关系
        unordered_map<string, InvertedList> inverted_index;

    public:
        Index();
        // 提供一些对外调用的函数
        // 1. 查正排的函数, 返回指针就可以使用 NULL 表示无效结果的情况(引用没有表示无效的情况)
        const DocInfo* GetDocInfo(int64_t doc_id);
        // 2. 查倒排的函数
        const InvertedList* GetInvertedList(const string &key);
        // 3. 构建索引
        bool Build(const string &input_path);

    private:
        DocInfo* BuildForward(const string& line);
        void BuildInverted(const DocInfo& doc_info);
        void CutWord(const string& input, vector<string>* output);
        cppjieba::Jieba jieba;
};

// 接下来是搜索模块的内容
}// namespace searcher


