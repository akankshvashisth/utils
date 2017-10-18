#include <string>
#include <vector>
#include <iostream>
#include <tuple>
#include <type_traits>

template<typename C0>
auto inner(C0 const& c0) -> std::vector<std::tuple<typename C0::value_type>>
{
    std::vector<std::tuple<typename C0::value_type>> ret;
    for (auto const& e0 : c0){    
        ret.emplace_back(std::make_tuple(e0));    
    }
    return ret;
}

template<class C0, class... Cs>
auto inner(C0 const& c0, Cs const&... cs) -> std::vector<std::tuple<typename C0::value_type, typename Cs::value_type...>>
{
    std::vector<std::tuple<typename C0::value_type, typename std::decay<Cs>::type::value_type...>> ret;
    for (auto const& e0 : c0){
        for (auto const& e1 : inner(cs...)){
            ret.emplace_back(std::tuple_cat(std::make_tuple(e0), e1));
        }
    }
    return ret;
}

int main(int argc, char** argv)
{
    {
        std::vector<int> v = { 1, 2, 3 };
        std::vector<float> f = { 1, 2, 3, 4, 5 };
        std::vector<std::string> s = { "this", "is", "string" };
        for (auto const& e : inner(s, v, s)){
            std::string s1;
            int s2;
            std::string s3;
            std::tie(s1, s2, s3) = e;
            std::cout << s1 << " " << s2 << " " << s3 << std::endl;
        }                   
    }
    return 0;
}
