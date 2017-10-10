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
