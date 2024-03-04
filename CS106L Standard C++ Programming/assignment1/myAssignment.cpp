#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;
using std::string;          
using std::unordered_set;

class html_fun{
    private:
        bool is_link_valid(const string& string_input){
            for(auto ite=string_input.begin();ite!=string_input.end();ite++){
                if(*ite=='#'||*ite==':'){
                    return false;
                }
            }
            return true;
        }
    public:
        unordered_set<string> createWikiLinkSet(const string& target_html){
            unordered_set<string> result_set;
            const string start_pattern="<a href=\"/wiki/",end_pattern="\"";
            std::string::const_iterator search_start_pos=target_html.begin();
            std::string::const_iterator search_end_pos=target_html.end();
            while(search_start_pos!=search_end_pos){
                std::string::const_iterator link_start=std::search(search_start_pos,search_end_pos,start_pattern.begin(),start_pattern.end());
                if(link_start==search_end_pos){
                    break;
                }
                std::string::const_iterator link_end=std::search(link_start+start_pattern.length(),search_end_pos,end_pattern.begin(),end_pattern.end());
                auto target_name=string(link_start+start_pattern.length(),link_end);
                if(is_link_valid(target_name)){
                   result_set.insert(target_name);
                }
                search_start_pos=link_end+1;
            }
            return result_set;
        }
};

class file_fun{
    public:
    string file_output_fun(){
        string filename = "quokka.txt";
        cout<< "Enter a file name: " << filename << endl;
        string target_html;
        std::ifstream fin(filename, std::ifstream::in);
        string partial_content;
        while (getline(fin, partial_content)) {
            target_html += partial_content;
        }
        return target_html;
    }
};

int main(){
    string target_html;
    html_fun fun_html;
    file_fun fun_file;
    target_html=fun_file.file_output_fun();
    unordered_set<string> validLinks=fun_html.createWikiLinkSet(target_html);
    for (auto& link : validLinks) {
        cout << link << endl;
    }
    return 0;
}
