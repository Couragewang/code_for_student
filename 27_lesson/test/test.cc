#include <iostream>
#include <string>
#include <unordered_map>

int main()
{
    std::unordered_map<std::string, std::string> dict;
    dict.insert(std::make_pair("hello", "你好"));
    dict.insert(std::make_pair("bit", "比特"));
    std::cout << dict["hello"] << std::endl;
    for(auto it = dict.begin(); it != dict.end(); it++){
        std::cout << it->first << " : " << it->second << std::endl;
    }
}
