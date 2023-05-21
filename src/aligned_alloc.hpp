#ifndef ALIGNED_ALLOC_HPP
#define ALIGNED_ALLOC_HPP

#include <new>
#include <stdexcept>
#include <cstddef>

template<typename T, std::size_t Align>
class aligned_allocator
{
public:
    static_assert(Align >= alignof(T), "The alignment can't be less than the minimum required alignment for T.");

    constexpr static std::align_val_t alignment{ Align };

    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = const T*;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

    template<typename U>
    struct rebind
    {
        using other = aligned_allocator<U, Align>;
    };

    constexpr aligned_allocator()                               = default;
    constexpr aligned_allocator(const aligned_allocator& other) = default;

    template<typename U>
    constexpr aligned_allocator(const aligned_allocator<U, Align>& other) noexcept {}


    [[nodiscard]]
    constexpr pointer allocate(size_type size) const
    {
        if (size > max_size()) throw std::bad_array_new_length{};

        if (size == 0) return nullptr;

        const size_type bytes_needed = size * sizeof(value_type);
        void* const storage = ::operator new[](bytes_needed, alignment);

        return static_cast<pointer>(storage);
    }

    constexpr void deallocate(pointer storage, size_type size) const noexcept
    {
        ::operator delete[](storage, size, alignment);
    }

    [[nodiscard]]
    constexpr size_type max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }
};

template<typename T, typename U, std::size_t Align>
inline constexpr bool operator==(const aligned_allocator<T, Align>&, const aligned_allocator<U, Align>&) noexcept
{
    return true;
}

#endif // !ALIGNED_ALLOC_HPP
