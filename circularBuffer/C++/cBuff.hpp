#ifndef CBUFF_HPP
#define CBUFF_HPP

/* includes */
#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <memory>

/* Macros */
#define CIRCULAR_BUFF_DEFAULT_SIZE  (4*1024)

template <typename T>
class CircularBuff
{
    public:
        CircularBuff(size_t size);
        CircularBuff();

        void put(T item);
        T get();
        void reset();
        bool empty() const;
        bool full() const;
        size_t capacity() const;
        size_t size() const;

        void write(T buff[], size_t size);
        size_t read(T* buff, size_t size);

    private:
        std::mutex m_mutex;
        std::unique_ptr<T[]> m_buf;
        size_t m_head = 0;
        size_t m_tail = 0;
        const size_t m_maxSize;
        bool m_full = 0;
};

/*-----------------------------------------------------------
 * Purpose  :   Constructor with buffer size as argument
 * Inputs   :   @param size : max sixe of circular buffer
 * Outputs  :   None
 * Returns  :   None
 * Notes    :   None
 * ---------------------------------------------------------*/
template <typename T>
CircularBuff<T>::CircularBuff(size_t size) :
m_buf(std::unique_ptr<T[]>(new T[size])),
m_maxSize(size)
{
}

/*-----------------------------------------------------------
 * Purpose  :   Constructor (set default buffer size)
 * Inputs   :   None
 * Outputs  :   None
 * Returns  :   None
 * Notes    :   None
 * ---------------------------------------------------------*/
template <typename T>
CircularBuff<T>::CircularBuff() :
CircularBuff::CircularBuff(CIRCULAR_BUFF_DEFAULT_SIZE)
{
}

/*-----------------------------------------------------------
 * Purpose  :   Reset circular buffer
 * Inputs   :   None
 * Outputs  :   None
 * Returns  :   None
 * Notes    :   None
 * ---------------------------------------------------------*/
template <typename T>
void CircularBuff<T>::reset()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_head = m_tail;
    m_full = false;
}

/*-----------------------------------------------------------
 * Purpose  :   Check if buffer is empty
 * Inputs   :   None
 * Outputs  :   None
 * Returns  :   bool
 * Notes    :   None
 * ---------------------------------------------------------*/
template <typename T>
bool CircularBuff<T>::empty() const
{
    // Empty if head and tail are equal
    if((true != m_full) && (m_head == m_tail))
    {
        return true;
    }

    return false;
}

/*-----------------------------------------------------------
 * Purpose  :   Check if buffer is full
 * Inputs   :   None
 * Outputs  :   None
 * Returns  :   bool
 * Notes    :   None
 * ---------------------------------------------------------*/
template <typename T>
bool CircularBuff<T>::full() const
{
    return m_full;
}

/*-----------------------------------------------------------
 * Purpose  :   Get maximum size of buffer
 * Inputs   :   None
 * Outputs  :   None
 * Returns  :   size_t
 * Notes    :   None
 * ---------------------------------------------------------*/
template <typename T>
size_t CircularBuff<T>::capacity() const
{
    return m_maxSize;
}

/*-----------------------------------------------------------
 * Purpose  :   Current size of buffer
 * Inputs   :   None
 * Outputs  :   None
 * Returns  :   size_t
 * Notes    :   None
 * ---------------------------------------------------------*/
template <typename T>
size_t CircularBuff<T>::size() const
{
    size_t size = m_maxSize;

    if (true != m_full)
    {
        if (m_head >= m_tail)
        {
            size = m_head - m_tail;
        }
        else
        {
            size = m_maxSize + m_head - m_tail;
        }
    }

    return size;
}

/*-----------------------------------------------------------
 * Purpose  :   Push an object to tail of buffer
 * Inputs   :   item
 * Outputs  :   None
 * Returns  :   None
 * Notes    :   None
 * ---------------------------------------------------------*/
template <typename T>
void CircularBuff<T>::put(T item)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    // Push data to tail
    m_buf[m_head] = item;

    if (m_full)
    {
        m_tail = (m_tail + 1) % m_maxSize;
    }

    m_head = (m_head + 1) % m_maxSize;
    m_full = (m_head == m_tail);
}

/*-----------------------------------------------------------
 * Purpose  :   Pop an object from head of buffer
 * Inputs   :   None
 * Outputs  :   None
 * Returns  :   T
 * Notes    :   None
 * ---------------------------------------------------------*/
template <typename T>
T CircularBuff<T>::get()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (true == empty())
    {
        return T();
    }

    // Pop data from head
    auto val = m_buf[m_tail];
    m_full = false;
    m_tail = (m_tail + 1) % m_maxSize;

    return val;
}

/*-----------------------------------------------------------
 * Purpose  :   Push an array of objects to tail
 * Inputs   :   @param buff : array of objects to push
 *              @param size : size of object array
 * Outputs  :   None
 * Returns  :   None
 * Notes    :   None
 * ---------------------------------------------------------*/
template <typename T>
void CircularBuff<T>::write(T* buff, size_t size)
{
    for(size_t i = 0; i < size; ++i)
    {
        put(buff[i]);
    }
}

/*-----------------------------------------------------------
 * Purpose  :   Pop an array of objects from head
 * Inputs   :   @param buff : array to save objects popped
 *              @param size : size of object array
 * Outputs  :   None
 * Returns  :   size_t
 * Notes    :   None
 * ---------------------------------------------------------*/
template <typename T>
size_t CircularBuff<T>::read(T* buff, size_t size)
{
    size_t i = 0;
    for(i = 0; i < size; ++i)
    {
        if(true == empty())
        {
            break;
        }
        buff[i] = get();
    }

    // Returns number of objects read
    return i;
}

#endif /* CBUFF_HPP */
