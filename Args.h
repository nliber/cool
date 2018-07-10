#ifndef COOL_ARGS_H_
#define COOL_ARGS_H_

#include <algorithm>
#include <iterator>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace cool
{

    class Args
    {
    public:
        explicit Args(std::vector<std::string> vs)
            : args{ std::move(vs) }
        {
            argvs.reserve(args.size() + 1);
            std::transform(std::begin(args), std::end(args), std::back_inserter(argvs), [](std::string const& s) { return s.c_str(); });
            argvs.emplace_back();
        }

        template<typename InputIterator>
        Args(InputIterator first, InputIterator last)
            : Args{ std::vector<std::string>(std::move(first), std::move(last)) }
        {
        }

        explicit Args(std::initializer_list<const char*> il)
            : Args{ std::vector<std::string>{il.begin(), il.end()} }
        {
        }

        explicit Args(std::initializer_list<std::string> il)
            : Args{ std::vector<std::string>{il.begin(), il.end()} }
        {
        }

        explicit Args(int c, const char*const v[])
            : Args{ v, v + c }
        {
        }

        explicit Args(const char*const v[])
            : Args{ v, [v]() mutable { while(*v) ++v; return v; }() }
        {
        }

        Args(Args const& that)
            : Args{ that.args }
        {
        }

        Args(Args&& that)
            : args{ std::move(that.args) }
            , argvs{ std::move(that.argvs) }
        {
            that = Args{ std::vector<std::string>{} };
        }

        ~Args() = default;

        friend void swap(Args& lhs, Args& rhs) noexcept
        {
            using namespace std;
            swap(lhs.args, rhs.args);
            swap(lhs.argvs, rhs.argvs);
        }

        Args& operator=(Args that) noexcept
        {
            swap(*this, that);
            return *this;
        }

        int argc()               const noexcept { return static_cast<int>(args.size()); }
        const char*const* argv() const noexcept { return argvs.data(); }

        friend std::ostream& operator<<(std::ostream& os, Args const& that)
        {
            const char* space = "";
            for (auto&& a : that.args)
            {
                os << space << a;
                space = " ";
            }
            return os;
        }

    private:
        std::vector<std::string> args;
        std::vector<const char*> argvs;
    };

} // namespace cool

#endif /* COOL_ARGS_H_ */

