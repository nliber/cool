#ifndef COOL_PREFIX_MAP_H_
#define COOL_PREFIX_MAP_H_

#include <boost/algorithm/string/predicate.hpp>
#include <boost/container/flat_map.hpp>
#include <cool/iless_range.h>
#include <type_traits>

namespace cool
{
    ///////////////////////////////////////////////////////////////////////////
    // prefix_map is a map that looks for either caseless exact key match or
    //  a unique prefix match.
    //
    //  This is similar to gdb commands and is useful when parsing things
    //  users directly type in (command line, input, etc.)
    ///////////////////////////////////////////////////////////////////////////
    template<typename Key, typename Value>
    class prefix_map : public boost::container::flat_map<Key, Value, iless_range>
    {
        using map_type = boost::container::flat_map<Key, Value, iless_range>;

    public:
        using typename map_type::allocator_type;
        using typename map_type::key_type;
        using typename map_type::value_type;
        using typename map_type::key_compare;
        using typename map_type::iterator;
        using typename map_type::const_iterator;

        template<typename... Us, typename = std::enable_if_t<std::is_constructible_v<map_type, Us...>>>
        prefix_map(Us&&... us)
        : map_type(std::forward<Us>(us)...)
        {}

        prefix_map(std::initializer_list<value_type> il)
        : map_type(il)
        {}

        template<typename... Us>
        prefix_map(std::initializer_list<value_type> il, Us&&... us)
        : map_type(il, std::forward<Us>(us)...)
        {}

        template<typename... Us>
        prefix_map(boost::container::ordered_unique_range_t, std::initializer_list<value_type> il, Us&&... us)
        : map_type(boost::container::ordered_unique_range, il, std::forward<Us>(us)...)
        {}

        prefix_map& operator=(std::initializer_list<value_type> il)
        {
            map_type::operator=(il);
            return *this;
        }

        const_iterator find_by_prefix(key_type const& key) const
        { return find_by_prefix(*this, key); }

              iterator find_by_prefix(key_type const& key)
        { return find_by_prefix(*this, key); }

    private:
        // Helper function that abstracts away const vs non-const iterators
        // with templates and auto type deduction
        //
        // TODO explore making this O(log n):
        // I think lower_bound returns either an exact match or the element
        // just before the two prefixes to check
        //
        // TODO handle is_transparent
        template<typename PrefixMap>
        static auto /* [const_]iterator */ find_by_prefix(PrefixMap& that, key_type const& key)
        {
            // If there is an exact match, return it
            auto found = that.find(key);
            if (that.end() != found)
                return found;

            key_compare comp = that.key_comp();
            auto is_prefix = [&](value_type const& kv)
            { return boost::istarts_with(kv.first, key, comp.get_locale()); };

            // If there is exactly one prefix match, return it
            // The empty() check is for the degenerate case when size() == 1, as an empty key prefix-matches everything
            found = std::find_if(that.begin(), that.end(), is_prefix);
            if (that.end() != found && that.end() == std::find_if(found + 1, that.end(), is_prefix) && !std::empty(key))
                return found;

            return that.end();
        }
    };
} // cool namespace

#endif /* COOL_PREFIX_MAP_H_ */

