#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "ctemplate/template.h"
#include <boost/algorithm/string.hpp> 
#include <unistd.h>
#include <unordered_map>//无符号的map来保存多个问
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;
class Question{
public:
    Question(string id,string title, string star, string desc, string code){
        id_ = id;
        title_ = title;
        star_ = star;
        desc_ = desc;
        code_ = code;
    }
	string id_;//题目ID
	string title_;//题目标题
	string star_;//题目的难易程度
    string desc_;
    string code_;
};

int main(){
    string html;
    vector<Question> ques;
    for(int i = 0; i<10; i++){
        Question question(to_string(i), "哈哈", "简单", "it is so easy", "coding");
        ques.push_back(question);
    }
    ctemplate::TemplateDictionary dict("all_questions");
    for(auto question : ques)
    {
        ctemplate::TemplateDictionary* sub_dict = dict.AddSectionDictionary("question");
        sub_dict->SetValue("id", question.id_); 
        sub_dict->SetValue("id", question.id_);
        sub_dict->SetValue("title", question.title_);
        sub_dict->SetValue("star", question.star_);
       
    }
    ctemplate::Template* tl = ctemplate::Template::GetTemplate("./all_questions.html", ctemplate::DO_NOT_STRIP); 
    tl->Expand(&html, &dict);
    cout<<html<<endl;
   
    int destfd = open("./demo.html", O_WRONLY | O_CREAT, 0777);
       if(destfd == -1) {
        perror("open");
        return -1;
    }

    // 3.频繁的读写操作
    //char buf[1024] = {0};
    write(destfd, html.c_str(), html.size());

    // 4.关闭文件
    close(destfd);

}