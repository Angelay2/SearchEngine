#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

// 这个变量表示从哪个目录当中读取 boost 文档的 html
string g_input_path = "../data/input";
// 这个变量对应预处理模块输出结果放到哪里
string g_output_path = "../data/tmp/raw_input";

// 创建爱你一个重要的结构体, 表示一个文档(一个HTML) 
struct DocInfo{
   // 文档的标题
   string title;
   // 文档的url
   string url;
   // 文档的正文
   string content;
};

bool Enumfile(const string &input_path, vector<int> *iter){
// 预处理过程的核心流程
// 1. 把 input 目录中所有的 html 路径都枚举出来
    namespace fs = boost::filesystem;
    fs::path root_path(input_path);
    if(!fs::exits(root_path)){
        std::cout << "当前的目录不存在" << std::endl;
        return false;
    }
    // 递归遍历的时候使用到一个核心的类
    // 迭代器使用循环实现的时候可以在完全递归
    // C++ 常用的一种做法, 把迭代器的默认构造函数生成的迭代器作为一个"哨兵"
    fs::recursive_directory_iterator end_iter;
    for(fs::recursive_directory_iterator iter(root_path); iter != end_iter; ++iter){
        // 当前的路径对于的是不是一个普通文件, 如果是目录,直接跳过
        if(!fs::is_regular_file(*iter)){
            continue;
        }       
        // 当前路径对于的文件是不是一个Html文件, 如果是其他文件也跳过
        if(!iter->path().extension() != ".html"){
            continue;
        }
        // 把得到的路径加入到最终结果的vector中
        file_list->push_back(iter->path.string());
    }
}

bool ParserFile(const string& file_path, DocInfo* doc_info){
    // 先读取文件的内容
    //  一股脑的吧整个文件内容都读取出来
    //  Read 这样的函数是一个比较底层比较通用的函数, 各个模块都可以用到
    string html;
    bool ret = Read(file_path, &html);
}
bool ParserTitle(const string& html, string* title){


}
// 根据本地路径来获取到在线文档的路径
// 本地路径形如: ../data/input/html/
// 在线路径如: https://www.boost.org/doc.libs.
bool ParserUrl(const string& file_path, string* url){
   string url_head = ; 
   string url_tail = ;
   *url = url_head + url_tail;
    return true;
}

bool ParserContent(const string& html, string* content){
  bool id_content = true;
  for(auto c : html){
      if(is_content){
          if(c == '<'){
              // 说明当前遇到了标签, 
              is_content = false;
          }
          else{
              // 这里还要单独处理下'\n', 预期的输出结果是一个行文本文件
              // 最终结果raw_input中每一行对应到一个原始的HTML文档
              // 此时就需要把html文档中原来的\n都干掉
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













