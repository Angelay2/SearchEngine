// 这个的代码用于实现直接预处理模块
// 核心功能就是读取并分析boost文档的_html
// 解析出每个文档的标题、url、正文（去除html标签）
// 最终把结果输出成一个行文本文件
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include<boost/filesystem/operations.hpp>
#include <boost/fileatem/path.hpp>
#include "../common/util.hpp>"

using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::vector;

// 定义一些相关的变量和结构体

// 这个变量表示从哪个目录当中读取 boost 文档的 html
string g_input_path = "../data/input";
// 这个变量对应预处理模块输出结果放到哪里
string g_output_path = "../data/tmp/raw_input";

// 创建一个重要的结构体, 表示一个文档(一个HTML) 
struct DocInfo{
   // 文档的标题
   string title;
   // 文档的url
   string url;
   // 文档的正文
   string content;
};

// 一个函数的输入参数，用 const & 来表示
// 一个函数的输出参数，使用指针来表示
// 一个函数的输入输出参数， 使用引用来表示
bool Enumfile(const string &input_path, vector<string> *file_list){
// 预处理过程的核心流程
// 1. 把 input 目录中所有的 html 路径都枚举出来
// 枚举目录需要使用 boost 来完成
// 把 boost::filesystem 这个命名空间定义一个别名
    namespace fs = boost::filesystem;
    fs::path root_path(input_path);
    if(!fs::exits(root_path)){
        std::cout << "当前的目录不存在" << std::endl;
        return false;
    }
    // 递归遍历的时候使用到一个核心的类
    // 迭代器使用循环实现的时候可以自动完成递归
    // C++ 常用的一种做法, 把迭代器的默认构造函数生成的迭代器作为一个"哨兵"
    fs::recursive_directory_iterator end_iter;
    for(fs::recursive_directory_iterator iter(root_path); iter != end_iter; ++iter){
        // 当前的路径对应的是不是一个普通文件, 如果是目录,直接跳过
        if(!fs::is_regular_file(*iter)){
            continue;
        }       
        // 当前路径对于的文件是不是一个Html文件, 如果是，其他文件也跳过
        if(iter->path().extension() != ".html"){
            continue;
        }
        // 把得到的路径加入到最终结果的vector中
        file_list->push_back(iter->path.string());
    }
    return true;
}

// 找到 html 中的title标签
bool ParserTitle(const string &html, string* title){
    size_t beg = html.find("<title>");
    if(beg == string::npos){
        std::cout << "标题未找到" << std::endl;
        return false;
    }
    size_t end = html.find("</title");
    if(end == string::npos){
        std::cout << "标题未找到" << std::endl;
        return false;
    }
    beg += string("<title>").size();
    if(beg >= end){
        std::cout << "标题位置不合法" << std::endl;
        return false;
    }
    *title = html.substr(beg, end - beg);
    return true;
}


// 根据本地路径来获取到在线文档的路径
// 本地路径形如: ../data/input/html/
// 在线路径如: https://www.boost.org/doc.libs/1_53_0/doc/html/thread.html
// 把本路径的后半部分截取出来，再拼装上在线路径的前缀就可以了
bool ParserUrl(const string &file_path, string* url){
   string url_head = "https://www.boost.org/doc/libs/1_53_0/doc/"; 
   string url_tail = file_path.substr(g_input_path.size());
   *url = url_head + url_tail;
    return true;
}

// 针对读取出来的 html 内容进行去标签
bool ParserContent(const string& html, string* content){
  bool id_content = true;
  for(auto c : html){
      if(is_content){
          // 当前已经是正文了
          if(c == '<'){
              // 说明当前遇到了标签 
              is_content = false;
          }
          else{
              // 这里还要单独处理下'\n', 预期的输出结果是一个行文本文件
              // 最终结果 raw_input 中每一行对应到一个原始的HTML文档
              // 此时就需要把html文档中原来的\n都干掉
              if(c == '\n'){
                  c = ' ';
              }
              // 当前是普通字符, 就把结果写入到content中
              content->push_back(c);
          }   
      }
      else{
          // 当前已经是标签了
          if(c == '>'){
              // 标签结束
              id_content = true;
          }
          // 标签里的其他内容都直接忽略掉
      }
  }
  return true;
}

bool ParserFile(const string& file_path, DocInfo* doc_info){
    // 1. 先读取文件的内容
    //  一股脑的吧整个文件内容都读取出来
    //  Read 这样的函数是一个比较底层比较通用的函数, 各个模块都可能会用到
    //  直接把这个函数就放到 common 目录即可
    string html;
    bool ret = Read(file_path, &html);
    if(!ret){
        std::cout << "解析文件失败！ 读取文件失败！" << file_path << std::endl;
        return false;
    }
    // 2. 根据文件内容解析处标题，（html 中有一个 title 标签）
    ret = ParserTitle(html, &doc_info->title);
    if(!ret){
        std::cout << " 解析标题失败！" << std::endl;
        return false;
    }
    // 3. 根据文件的路径，构造处对应的在线文档的 url
    ret = ParserUrl(file_path, &doc_info->url);
    if(!ret){
        std::cout << "解析 url 失败！" << std::endl;
        return false;
    }
    // 4. 根据文件内容，进行去标签， 作为 doc_info 中的 content 字段的内容
    ret = ParserContent(html, &doc_info->content);
    if(!ret){
        std::cout << "解析正文失败！" << std::endl;
        return false;
    }
    return true;
}

int main(){
    // 1. 进行枚举路径
    vector<string> file_list;
    bool ret = Enumfile(g_input_path, );


    // 2. 遍历枚举出来的路径, 针对每个文件 ,单独进行处理
    std::ofstream output_file(g_output_path.c_str());
    if(!output_file){
        std::cout << file_path << std::endl;

    }
    return 0;
}













