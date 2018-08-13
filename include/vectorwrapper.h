#include <type_traits>
#include <vector>

template<typename T>
struct extendablevector;

template<typename T, typename = typename T::extends_t>
struct vectorextension
{
    using size_type = typename std::vector<T>::size_type;
    using pointer = typename std::vector<T>::pointer;
    using reference = typename std::vector<T>::reference;
    using const_reference = typename std::vector<T>::const_reference;
    using const_pointer = typename std::vector<T>::const_pointer;
    using value_type = typename std::vector<T>::value_type;
    using difference_type = typename std::vector<T>::difference_type;
    using allocator_type = typename std::vector<T>::allocator_type;
    using iterator = typename std::vector<T>::iterator;
    using reverse_iterator = typename std::vector<T>::reverse_iterator;
    using const_reverse_iterator =
        typename std::vector<T>::const_reverse_iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    iterator begin() noexcept
    {
        return m_vector.begin();
    }
    const_iterator begin() const noexcept
    {
        return m_vector.begin();
    }
    iterator end() noexcept
    {
        return m_vector.end();
    }
    const_iterator end() const noexcept
    {
        return m_vector.end();
    }
    iterator rend() noexcept
    {
        return m_vector.rend();
    }
    const_iterator rend() const noexcept
    {
        return m_vector.rend();
    }
    iterator rbegin() noexcept
    {
        return m_vector.rbegin();
    }
    const_iterator rbegin() const noexcept
    {
        return m_vector.rbegin();
    }

    const_iterator cbegin() const noexcept
    {
        return m_vector.cbegin();
    }
    const_iterator cend() const noexcept
    {
        return m_vector.cend();
    }
    const_iterator crend() const noexcept
    {
        return m_vector.crend();
    }
    const_iterator crbegin() const noexcept
    {
        return m_vector.crbegin();
    }
    size_type size() const noexcept
    {
        return m_vector.size();
    }
    size_type max_size() const noexcept
    {
        return m_vector.max_size();
    }
    void resize(size_type n)
    {
        return m_vector.resize(n);
    }
    void resize(size_type n, const value_type& val)
    {
        return m_vector.resize(n, val);
    }
    size_type capacity() const noexcept
    {
        return m_vector.capacity();
    }
    bool empty() const noexcept
    {
        return m_vector.empty();
    }
    void reserve(size_type n)
    {
        return m_vector.reserve(n);
    }
    void shrink_to_fit()
    {
        m_vector.shrink_to_fit();
    }

    reference operator[](size_type n)
    {
        return m_vector[n];
    }
    const_reference operator[](size_type n) const
    {
        return m_vector[n];
    }

    reference at(size_type n)
    {
        return m_vector.at(n);
    }
    const_reference at(size_type n) const
    {
        return m_vector.at(n);
    }

    reference front()
    {
        return m_vector.front();
    }
    const_reference front() const
    {
        return m_vector.front();
    }
    reference back()
    {
        return m_vector.back();
    }
    const_reference back() const
    {
        return m_vector.back();
    }
    value_type* data() noexcept
    {
        return m_vector.data();
    }
    const value_type* data() const noexcept
    {
        return m_vector.data();
    }

    template<class InputIterator>
    void assign(InputIterator first, InputIterator last)
    {
        return m_vector.template assign<InputIterator>(first, last);
    }

    void assign(size_type n, const value_type& val)
    {
        return m_vector.assign(n, val);
    }

    void assign(std::initializer_list<value_type> il)
    {
        return m_vector.assign(il);
    }

    void push_back(const value_type& val)
    {
        m_vector.push_back(val);
    }
    void push_back(value_type&& val)
    {
        m_vector.push_back(val);
    }
    void pop_back()
    {
        return m_vector.pop_back();
    }

    iterator insert(const_iterator position, const value_type& val)
    {
        return m_vector.insert(position, val);
    }

    iterator insert(const_iterator position, size_type n, const value_type& val)
    {
        return m_vector.insert(position, n, val);
    }

    template<class InputIterator>
    iterator insert(const_iterator position, InputIterator first,
                    InputIterator last)
    {
        return m_vector.template insert<InputIterator>(position, first, last);
    }

    iterator insert(const_iterator position, value_type&& val)
    {
        return m_vector.insert(position, val);
    }

    iterator insert(const_iterator position,
                    std::initializer_list<value_type> il)
    {
        return m_vector.insert(position, il);
    }

    iterator erase(const_iterator position)
    {
        return m_vector.erase(position);
    }
    iterator erase(const_iterator first, const_iterator last)
    {
        return m_vector.erase(first, last);
    }

    void swap(vectorextension& x)
    {
        m_vector.swap(x.m_vector);
        std::swap(m_ref, x.m_ref);
    }
    void clear() noexcept
    {
        m_vector.clear();
    }

    template<class... Args>
    iterator emplace(const_iterator position, Args&&... args)
    {
        m_vector.template emplace<Args...>(position, args...);
    }

    template<class... Args>
    void emplace_back(Args&&... args)
    {
        m_vector.template emplace_back<Args...>(args...);
    }

    allocator_type get_allocator() const noexcept
    {
        return m_vector.get_allocator();
    }

    vectorextension& operator=(const std::vector<T>& x)
    {
        m_vector = x;
        m_ref = nullptr;
        return *this;
    }
    vectorextension& operator=(std::vector<T>&& x)
    {
        m_vector = std::move(x);
        m_ref = nullptr;
        return *this;
    }
    vectorextension& operator=(const vectorextension& x)
    {
        m_vector = x.m_vector;
        m_ref = x.m_ref;
        return *this;
    }
    vectorextension& operator=(vectorextension&& x)
    {
        m_vector = std::move(x.m_vector);
        m_ref = x.m_ref;
        return *this;
    }
    vectorextension& operator=(std::initializer_list<value_type> il)
    {
        m_vector = il;
        m_ref = nullptr;
        return *this;
    }

    vectorextension()
      : m_vector()
    {}
    explicit vectorextension(const allocator_type& alloc)
      : m_vector(alloc)
    {}

    explicit vectorextension(size_type n,
                           const allocator_type& alloc = allocator_type())
      : m_vector(n, alloc)
    {}
    vectorextension(size_type n, const value_type& val,
                  const allocator_type& alloc = allocator_type())
      : m_vector(n, val, alloc)
    {}

    template<class InputIterator>
    vectorextension(InputIterator first, InputIterator last,
                  const allocator_type& alloc = allocator_type())
      : m_vector(first, last, alloc)
    {}

    vectorextension(const std::vector<T>& x)
      : m_vector(x)
    {}
    vectorextension(const vectorextension& x)
      : m_vector(x.m_vector), m_ref(x.m_ref)
    {}

    vectorextension(const std::vector<T>& x, const allocator_type& alloc)
      : m_vector(x, alloc)
    {}
    vectorextension(const vectorextension& x, const allocator_type& alloc)
      : m_vector(x.m_vector, alloc), m_ref(x.m_ref)
    {}

    vectorextension(vectorextension&& x)
      : m_vector(std::move(x.m_vector)), m_ref(x.m_ref)
    {}
    vectorextension(std::vector<T>&& x)
      : m_vector(std::move(x))
    {}

    vectorextension(std::vector<T>&& x, const allocator_type& alloc)
      : m_vector(std::move(x), alloc)
    {}
    vectorextension(vectorextension&& x, const allocator_type& alloc)
      : m_vector(std::move(x.m_vector), alloc), m_ref(x.m_ref)
    {}

    vectorextension(std::initializer_list<value_type> il,
                  const allocator_type& alloc = allocator_type())
      : m_vector(il, alloc)
    {}

    extendablevector<typename T::extends_t>* base() const
    {
        return m_ref;
    }

    bool validate_ref(extendablevector<typename T::extends_t>& base) const
    {
        return &m_ref == base;
    }

    bool validate_ref(extendablevector<typename T::extends_t>* base) const
    {
        return m_ref == base;
    }

    template<typename TT,
             typename = typename std::enable_if<std::is_same<
                 typename T::extends_t, typename TT::extends_t>::value>>
    void set_ref(vectorextension<TT>& intermediate)
    {
        set_ref(intermediate.base());
    }

    template<typename TT,
             typename = typename std::enable_if<std::is_same<
                 typename T::extends_t, typename TT::extends_t>::value>>
    void set_ref(vectorextension<TT>* intermediate)
    {
        set_ref(intermediate->base());
    }

    void set_ref(extendablevector<typename T::extends_t>& base)
    {
        m_ref = &base;
    }

    void set_ref(extendablevector<typename T::extends_t>* base)
    {
        m_ref = base;
    }

private:
    std::vector<T> m_vector;

    extendablevector<typename T::extends_t>* m_ref{nullptr};
};



template<typename T>
struct extendablevector
{
    using size_type = typename std::vector<T>::size_type;
    using pointer = typename std::vector<T>::pointer;
    using reference = typename std::vector<T>::reference;
    using const_reference = typename std::vector<T>::const_reference;
    using const_pointer = typename std::vector<T>::const_pointer;
    using value_type = typename std::vector<T>::value_type;
    using difference_type = typename std::vector<T>::difference_type;
    using allocator_type = typename std::vector<T>::allocator_type;
    using iterator = typename std::vector<T>::iterator;
    using reverse_iterator = typename std::vector<T>::reverse_iterator;
    using const_reverse_iterator =
        typename std::vector<T>::const_reverse_iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    iterator begin() noexcept
    {
        return m_vector.begin();
    }
    const_iterator begin() const noexcept
    {
        return m_vector.begin();
    }
    iterator end() noexcept
    {
        return m_vector.end();
    }
    const_iterator end() const noexcept
    {
        return m_vector.end();
    }
    iterator rend() noexcept
    {
        return m_vector.rend();
    }
    const_iterator rend() const noexcept
    {
        return m_vector.rend();
    }
    iterator rbegin() noexcept
    {
        return m_vector.rbegin();
    }
    const_iterator rbegin() const noexcept
    {
        return m_vector.rbegin();
    }

    const_iterator cbegin() const noexcept
    {
        return m_vector.cbegin();
    }
    const_iterator cend() const noexcept
    {
        return m_vector.cend();
    }
    const_iterator crend() const noexcept
    {
        return m_vector.crend();
    }
    const_iterator crbegin() const noexcept
    {
        return m_vector.crbegin();
    }
    size_type size() const noexcept
    {
        return m_vector.size();
    }
    size_type max_size() const noexcept
    {
        return m_vector.max_size();
    }
    void resize(size_type n)
    {
        return m_vector.resize(n);
    }
    void resize(size_type n, const value_type& val)
    {
        return m_vector.resize(n, val);
    }
    size_type capacity() const noexcept
    {
        return m_vector.capacity();
    }
    bool empty() const noexcept
    {
        return m_vector.empty();
    }
    void reserve(size_type n)
    {
        return m_vector.reserve(n);
    }
    void shrink_to_fit()
    {
        m_vector.shrink_to_fit();
    }

    reference operator[](size_type n)
    {
        return m_vector[n];
    }
    const_reference operator[](size_type n) const
    {
        return m_vector[n];
    }

    reference at(size_type n)
    {
        return m_vector.at(n);
    }
    const_reference at(size_type n) const
    {
        return m_vector.at(n);
    }

    reference front()
    {
        return m_vector.front();
    }
    const_reference front() const
    {
        return m_vector.front();
    }
    reference back()
    {
        return m_vector.back();
    }
    const_reference back() const
    {
        return m_vector.back();
    }
    value_type* data() noexcept
    {
        return m_vector.data();
    }
    const value_type* data() const noexcept
    {
        return m_vector.data();
    }

    template<class InputIterator>
    void assign(InputIterator first, InputIterator last)
    {
        return m_vector.template assign<InputIterator>(first, last);
    }

    void assign(size_type n, const value_type& val)
    {
        return m_vector.assign(n, val);
    }

    void assign(std::initializer_list<value_type> il)
    {
        return m_vector.assign(il);
    }

    void push_back(const value_type& val)
    {
        m_vector.push_back(val);
    }
    void push_back(value_type&& val)
    {
        m_vector.push_back(val);
    }
    void pop_back()
    {
        return m_vector.pop_back();
    }

    iterator insert(const_iterator position, const value_type& val)
    {
        return m_vector.insert(position, val);
    }

    iterator insert(const_iterator position, size_type n, const value_type& val)
    {
        return m_vector.insert(position, n, val);
    }

    template<class InputIterator>
    iterator insert(const_iterator position, InputIterator first,
                    InputIterator last)
    {
        return m_vector.template insert<InputIterator>(position, first, last);
    }

    iterator insert(const_iterator position, value_type&& val)
    {
        return m_vector.insert(position, val);
    }

    iterator insert(const_iterator position,
                    std::initializer_list<value_type> il)
    {
        return m_vector.insert(position, il);
    }

    iterator erase(const_iterator position)
    {
        return m_vector.erase(position);
    }
    iterator erase(const_iterator first, const_iterator last)
    {
        return m_vector.erase(first, last);
    }

    void swap(extendablevector& x)
    {
        m_vector.swap(x.m_vector);
    }
    void clear() noexcept
    {
        m_vector.clear();
    }

    template<class... Args>
    iterator emplace(const_iterator position, Args&&... args)
    {
        m_vector.template emplace<Args...>(position, args...);
    }

    template<class... Args>
    void emplace_back(Args&&... args)
    {
        m_vector.template emplace_back<Args...>(args...);
    }

    allocator_type get_allocator() const noexcept
    {
        return m_vector.get_allocator();
    }

    extendablevector& operator=(const std::vector<T>& x)
    {
        m_vector = x;
        return *this;
    }
    extendablevector& operator=(std::vector<T>&& x)
    {
        m_vector = std::move(x);
        return *this;
    }
    extendablevector& operator=(const extendablevector& x)
    {
        m_vector = x.m_vector;
        return *this;
    }
    extendablevector& operator=(extendablevector&& x)
    {
        m_vector = std::move(x.m_vector);
        return *this;
    }
    extendablevector& operator=(std::initializer_list<value_type> il)
    {
        m_vector = il;
        return *this;
    }

    extendablevector()
      : m_vector()
    {}
    explicit extendablevector(const allocator_type& alloc)
      : m_vector(alloc)
    {}

    explicit extendablevector(size_type n,
                           const allocator_type& alloc = allocator_type())
      : m_vector(n, alloc)
    {}
    extendablevector(size_type n, const value_type& val,
                  const allocator_type& alloc = allocator_type())
      : m_vector(n, val, alloc)
    {}

    template<class InputIterator>
    extendablevector(InputIterator first, InputIterator last,
                  const allocator_type& alloc = allocator_type())
      : m_vector(first, last, alloc)
    {}

    extendablevector(const std::vector<T>& x)
      : m_vector(x)
    {}
    extendablevector(const extendablevector& x)
      : m_vector(x.m_vector)
    {}

    extendablevector(const std::vector<T>& x, const allocator_type& alloc)
      : m_vector(x, alloc)
    {}
    extendablevector(const extendablevector& x, const allocator_type& alloc)
      : m_vector(x.m_vector, alloc)
    {}

    extendablevector(extendablevector&& x)
      : m_vector(std::move(x.m_vector))
    {}
    extendablevector(std::vector<T>&& x)
      : m_vector(std::move(x))
    {}

    extendablevector(std::vector<T>&& x, const allocator_type& alloc)
      : m_vector(std::move(x), alloc)
    {}
    extendablevector(extendablevector&& x, const allocator_type& alloc)
      : m_vector(std::move(x.m_vector), alloc)
    {}

    extendablevector(std::initializer_list<value_type> il,
                  const allocator_type& alloc = allocator_type())
      : m_vector(il, alloc)
    {}

private:
    std::vector<T> m_vector;
};
