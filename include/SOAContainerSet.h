#include "SOAField.h"
#include "SOASkin.h"
#include "SOAUtils.h"
#include "SOAView.h"
#include <string>

class IncompatibleZip
{
    std::string m_message{"unspecified failure"};

public:
    IncompatibleZip() {}
    IncompatibleZip(std::string s)
      : m_message(s)
    {}
    std::string message()
    {
        return m_message;
    }
};

template<typename T>
struct is_IDed
{
    bool value = false;
};

template<typename CONTAINER>
class IDedSOAContainer;

template<typename VIEW>
struct is_IDed<IDedSOAContainer<VIEW>>
{
    static constexpr bool value =
        SOA::Utils::is_view<std::decay_t<VIEW>>::value;
};

template<typename FIRST, typename SECOND, typename... OTHERS>
bool allsameid(FIRST& first, SECOND& second, OTHERS&... others);

template<typename FIRST, typename... OTHERS>
int
firstid(FIRST& first, OTHERS&...)
{
    return first.ZipIdentifier();
}

template<typename FIRST>
bool allsameid(FIRST& /*unused*/);

template<typename CONTAINER>
class IDedSOAContainer : public CONTAINER
{
public:
    using view = CONTAINER;

    template<typename... Args>
    IDedSOAContainer(int ID, Args&&... args)
      : CONTAINER(args...)
      , m_identifier(ID)
    {}

    template<typename T>
    auto push_back(T&& t) -> std::enable_if_t<
        view::self_type::fields_typelist::size() == 1 &&
        std::is_same_v<std::remove_reference_t<T>,
                       typename view::self_type::fields_typelist::template at<
                           0>::type::type>>
    {
        CONTAINER::emplace_back(std::forward<T>(t));
    }
    template<typename T>
    auto push_back(const T& t) -> std::enable_if_t<
        view::self_type::fields_typelist::size() == 1 &&
        std::is_same_v<std::remove_reference_t<T>,
                       typename view::self_type::fields_typelist::template at<
                           0>::type::type>>
    {
        CONTAINER::emplace_back(std::forward<const T>(t));
    }

private:
    int m_identifier;

public:
    int ZipIdentifier() const
    {
        return m_identifier;
    }
};

template<template<class> class SKIN, typename... IDeds,
         typename = typename std::enable_if_t<SOA::Utils::ALL(
             SOA::impl::is_skin<SKIN>(),
             is_IDed<typename std::remove_cv_t<
                 typename std::remove_reference_t<IDeds>>>::value...)>>
auto
myzip(IDeds&&... views) -> IDedSOAContainer<
    decltype(zip(std::forward<typename std::remove_reference_t<IDeds>::view>(
                     static_cast<typename std::remove_reference_t<IDeds>::view>(
                         views))...)
                 .template view<SKIN>())>
{
    /// maybe assert
    assert(allsameid(views...));
/// or throw
#ifndef NDEBUG
    if(!allsameid(views...))
        throw IncompatibleZip("zipping from different sets");
#endif

    auto encapsulated_zip =
        zip(std::forward<typename std::remove_reference_t<IDeds>::view>(
                static_cast<typename std::remove_reference_t<IDeds>::view>(
                    views))...)
            .template view<SKIN>();
    return IDedSOAContainer<decltype(encapsulated_zip)>(firstid(views...),
                                                        encapsulated_zip);
}

template<typename FIRST, typename SECOND, typename... OTHERS>
bool
allsameid(FIRST& first, SECOND& second, OTHERS&... others)
{
    return (first.ZipIdentifier() == second.ZipIdentifier()) &&
           (allsameid(first, others...));
}

template<typename FIRST>
bool
allsameid(FIRST& /*unused*/)
{
    return true;
}

