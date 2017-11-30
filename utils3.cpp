
#include <memory>

template<typename T>
struct resource_ref
{
    typedef T element_type;
    typedef std::shared_ptr<element_type> pointer_type;
    typedef std::weak_ptr<element_type> weak_type;

    static size_t reference_count() { return get_global_handle().use_count(); }
    static bool is_in_use() { return !get_global_handle().expired();}

    element_type* operator->() const { return m_ptr.get(); }
    element_type& operator*() const { return *m_ptr; }
    element_type* get() const { return m_ptr.get(); }
protected:    
    resource_ref() : m_ptr(get_global_handle().lock()){}

    template<typename Alloc, typename... Args>
    resource_ref(Alloc a, Args&&... xs):m_ptr(get_global_handle().lock())
    {
        if (!m_ptr){ reset(a, std::move(xs)...); }
    }

    virtual ~resource_ref(){ m_ptr.reset(); }

    void reset() { m_ptr.reset(); }

    template<typename Alloc, typename... Args>
    void reset(Alloc alloc, Args&&... xs)
    {
        m_ptr = alloc(std::move(xs)...);
        get_global_handle() = m_ptr;
    }
   
private:
    static weak_type& get_global_handle()
    {
        static weak_type handle;
        return handle;
    }

    pointer_type m_ptr;
};

/////////////////////////////////////

#include <vector>
#include <type_traits>
#include <cassert>

template<typename T>
struct resource : resource_ref<T>
{
    resource() : resource_ref<T>(std::make_shared<T>){}
};

template<typename T>
struct const_resource : resource_ref<T const>
{    
    template<typename Arg, typename... Args>
    const_resource(Arg&& arg, Args&&... data) : resource_ref<element_type>(std::make_shared<T, Arg, Args...>, arg, data...){}
    const_resource() : resource_ref<element_type>() {
        assert(is_in_use());
    }
};

template<typename T>
using vector_resource = resource<std::vector<T>>;

template<typename T>
using const_vector_resource = const_resource<std::vector<T>>;

/////////////////////////////////////

#include <iostream>

void foo()
{
    vector_resource<int> res;
    std::cout << res->size() << std::endl;
    res->push_back(5);
    std::cout << res->size() << std::endl;
}

void foo2()
{
    const_vector_resource<int> res;    
    //res->push_back(5);
    if (res.is_in_use()){
        std::cout << res->size() << ", " << res.is_in_use() << std::endl;
    }
}

int main()
{
    {
        vector_resource<int> res;
        std::cout << res->size() << std::endl;
        res->push_back(2);
        res->push_back(3);
        std::cout << res->size() << std::endl;
        foo();
        std::cout << res->size() << std::endl;
        res->push_back(3);
        std::cout << res->size() << std::endl;
        foo();
        std::cout << res->size() << std::endl;
    }    
    foo();
    {
        //std::vector<int> v = { 1, 2, 3, 4 };
        const_vector_resource<int> res(15, 6);
        std::cout << res->size() << std::endl;
        //res->push_back(3);
        foo2();
    }
    foo2();
    const_vector_resource<int> res(11,7);
    foo2();

    return 0;
}
