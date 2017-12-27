#ifndef COOL_UNIQUE_FD_H_
#define COOL_UNIQUE_FD_H_

#include <unistd.h>

namespace cool { class unique_fd; }
int close(cool::unique_fd& that) noexcept;

namespace cool
{
    class unique_fd
    {
    public:
        constexpr explicit unique_fd(int fd = -1) noexcept : m_fd{fd} {}

        constexpr unique_fd(unique_fd&& that)            noexcept : m_fd{that.m_fd} { that.m_fd = -1; }
        constexpr unique_fd& operator=(unique_fd&& that) noexcept { swap(*this, that); return *this; }

        ~unique_fd() noexcept { if (-1 != m_fd) close(m_fd); }

        constexpr int release() noexcept { int fd{m_fd}; m_fd = -1; return fd; }

        constexpr operator int() const noexcept { return m_fd; }
        constexpr int get()      const noexcept { return m_fd; }

        friend int ::close(unique_fd& that) noexcept;

        constexpr friend void swap(unique_fd& l, unique_fd& r) noexcept
        { int fd{l.m_fd}; l.m_fd = r.m_fd; r.m_fd = fd; }

    private:
        int m_fd;
    };

} // cool namespace

inline int close(cool::unique_fd& that) noexcept
{
    int closed{close(that.m_fd)};

    if (0 == closed)
        that.m_fd = -1;

    return closed;
}

#endif  /* COOL_UNIQUE_FD_H_ */

