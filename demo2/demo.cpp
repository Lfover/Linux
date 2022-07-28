#include <iostream>
#include <iostream>
#include <iterator>
#include <string>
#include <regex>
using namespace std;

int main()
{
    //首先是按&分开代码和测试用例
    string s = "code=%23include+%3Ciostream%3E%0D%0A%23include+%3Cstring%3E%0D%0A%23include+%3Cvector%3E%0D%0A%23include+%3Cmap%3E%0D%0A%23include+%3Calgorithm%3E%0D%0Ausing+namespace+std%3B%0D%0A%2F%2F%E8%AF%B7%E5%AE%9E%E7%8E%B0%E8%BF%99%E4%B8%AA%E5%87%BD%E6%95%B0%0D%0Aclass+Solution+%7B%0D%0A++public%3A%0D%0A%2F%2F%E8%BF%99%E9%87%8C%E6%98%AF%E4%B8%BA%E4%BA%86+%E6%B3%A8%E9%87%8A%0D%0A++++bool+isPalindrome%28int+x%29+%7B%0D%0A++++++cout+%3C%3C+%22%E6%B5%8B%E8%AF%95%E4%B8%80%E4%B8%8B%22+%3C%3C+endl%3B%0D%0A++++++return+true%3B+++++++++%0D%0A++++%7D%0D%0A%7D%3B%0D%0A&stdin=lloll";
        std::regex self_regex("REGULAR EXPRESSIONS",
            std::regex_constants::ECMAScript | std::regex_constants::icase);
    if (std::regex_search(s, self_regex)) {
        std::cout << "Text contains the phrase 'regular expressions'\n";
    }
 
    std::regex word_regex("(\\w+)");
    auto words_begin = 
        std::sregex_iterator(s.begin(), s.end(), word_regex);
    auto words_end = std::sregex_iterator();
 
    std::cout << "Found "
              << std::distance(words_begin, words_end)
              << " words\n";
 
    const int N = 6;
    std::cout << "Words longer than " << N << " characters:\n";
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        if (match_str.size() > N) {
            std::cout << "  " << match_str << '\n';
        }
    }
 
    std::regex long_word_regex("(\\w{7,})");
    std::string new_s = std::regex_replace(s, long_word_regex, "[$&]");
    std::cout << new_s << '\n';
}


}

