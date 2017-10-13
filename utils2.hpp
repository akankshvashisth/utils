#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <unordered_map>


template<typename _ContainerA, typename _ContainerB>
_ContainerA& concat(_ContainerA& a, _ContainerB const& b)
{
    a.insert(a.end(), b.cbegin(), b.cend());
    return a;
}

template<typename _Container>
bool is_palindrome(_Container const& c)
{
    return std::equal(c.begin(), c.begin() + c.size() / 2, c.rbegin());
}

//A contains all of B
template<typename _ContainerA, typename _ContainerB>
bool is_contained_in(_ContainerA const& a, _ContainerB const& b)
{
    if (a.size() < b.size()){
        return false;
    }
    typedef typename _ContainerA::value_type container_value_type;
    std::unordered_map<container_value_type, int> counter;
    typedef typename std::unordered_map<typename _ContainerA::value_type, int>::value_type counter_value_type;
    for (auto const& item : b){
        ++counter[item];
    }

    for (auto const& item : a){
        --counter[item];
    }

    return std::all_of(counter.cbegin(), counter.cend(), [](counter_value_type const& kv){ return kv.second <= 0; });
}
///////////////////////////////////////////
struct slice_tag{};

template<typename _Container>
_Container slice(_Container const& c, int b, int e)
{
    if (e <= b){
        return _Container();
    }
    auto const begin = c.cbegin() + b;
    auto const end = unsigned(e) >= c.size() ? c.end() : c.cbegin() + e;
    return _Container(begin, end);
}

template<typename _Container>
_Container slice(_Container const& c, int b, slice_tag)
{
    if (c.size() <= unsigned(b)){
        return _Container();
    }
    return _Container(c.cbegin() + b, c.cend());
}

template<typename _Container>
_Container slice(_Container const& c, slice_tag, int e)
{
    if (c.size() <= unsigned(e)){
        return _Container(c);
    }
    return _Container(c.cbegin(), c.cbegin() + e);
}

template<typename _Container>
_Container slice(_Container const& c, slice_tag, slice_tag)
{
    return _Container(c);
}

template<typename _Container, typename _Func>
void erase_if(_Container& c, _Func f)
{
    c.erase(std::remove_if(c.begin(), c.end(), f), c.end());
}

////////////////////////////
template<typename _Container>
std::string join(std::string const delimiter, _Container const& c)
{    
    if ( c.empty() ) { return std::string(); }

    auto b = c.cbegin();
    auto e = c.cend();

    std::stringstream ss;    
    ss << *b++;
    for (;b!=e;) { ss << delimiter << *b++; }

    return ss.str();
}

template<typename _Container, typename _Transform>
std::string join(std::string const delimiter, _Container const& c, _Transform f)
{
    if (c.empty()) { return std::string(); }

    auto b = c.cbegin();
    auto e = c.cend();

    std::stringstream ss;
    ss << f(*b++);
    for (; b != e;) { ss << delimiter << f(*b++); }

    return ss.str();
}

std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string>& result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}


template<class ForwardIt1, class ForwardIt2>
void iterator_swap(ForwardIt1 a, ForwardIt2 b)
{
    using std::swap;
    swap(*a, *b);
}

template <class ForwardIt>
ForwardIt rotate_left(ForwardIt first, ForwardIt n_first, ForwardIt last)
{
    if (first == n_first) return last;
    if (n_first == last) return first;

    ForwardIt next = n_first;

    do {
        iterator_swap(first++, next++);
        if (first == n_first) n_first = next;
    } while (next != last);

    ForwardIt ret = first;

    for (next = n_first; next != last;) {
        iterator_swap(first++, next++);
        if (first == n_first) n_first = next;
        else if (next == last) next = n_first;
    }

    return ret;
}

struct is_empty
{
    template<typename _Container>
    bool operator()(_Container const& c)
    { 
        return c.empty(); 
    }
};

////////////////////////

int noop(int i)
{
    return i;
}

int main(int argc, char** argv)
{
    unsigned int const n = 5;
    unsigned int const d = 3;

    std::vector<int> a = { 1, 2, 3, 4, 5 };    
    rotate_left(a.begin(), a.begin() + d, a.end());
    std::cout << join(" ", a) << std::endl;
    std::vector<std::string> tokens;
    std::string s("this.is.not.a.real.string...");
    split(s, '.', tokens);
    std::cout << join("/", tokens) << std::endl;
    erase_if(tokens, is_empty());
    std::cout << join("/", tokens) << std::endl;
    std::string b("0123456667");
    std::cout << slice(b, 0, 3) << std::endl;
    std::cout << slice(b, 0, 7) << std::endl;
    std::cout << slice(b, 0, 8) << std::endl;
    std::cout << slice(b, 0, 9) << std::endl;
    std::cout << slice(b, 1, 3) << std::endl;
    std::cout << slice(b, 3, 1) << std::endl;
    std::cout << slice(b, 1, 1) << std::endl;
    std::cout << slice(b, slice_tag(), slice_tag()) << std::endl;
    std::cout << slice(b, slice_tag(), 4) << std::endl;
    std::cout << slice(b, slice_tag(), 10) << std::endl;
    std::cout << slice(b, 10, slice_tag()) << std::endl;
    std::cout << slice(b, 5, slice_tag()) << std::endl;
    std::cout << slice(b, 5, 6) << std::endl;

    std::string c("76665421");
    std::vector<char> e = { '1', '3', '5' };
    std::cout << is_contained_in(b, c) << std::endl;
    std::cout << is_contained_in(c, b) << std::endl;
    std::cout << is_contained_in(b, b) << std::endl;
    std::cout << is_contained_in(c, e) << std::endl;
    std::cout << is_contained_in(b, e) << std::endl;

    std::string f("1234321");
    std::string g("123321");
    std::string h("123331");
    std::cout << is_palindrome(f) << is_palindrome(g) << is_palindrome(h) << std::endl;
    std::cout << concat(f, e) << std::endl;
    std::cout << concat(f, f) << std::endl;
    return 0;
}
