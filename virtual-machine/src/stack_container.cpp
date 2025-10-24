#include "virtual-machine/stack_container.h"
#include <type_traits>

namespace vm
{

    StackContainer::StackContainer(std::size_t capacity) :  m_Size(0)
    {
        m_Storage.reserve(capacity); /** @todo Add optimization. */
    }

    void StackContainer::Push(Types&& element)
    {
        if (m_Size < m_Storage.size())
        {
            m_Storage[m_Size] = std::move(element);
        }
        else 
        {
            m_Storage.emplace_back(std::move(element));
        }
        m_Size++;
    }

    const Types& StackContainer::Back() const
    {
        if(m_Size <= 0){
            throw std::runtime_error("[STACK] Stack size cannot be smaller than 0. Cannot Back().");
        }
        return m_Storage.at(m_Size - 1);
    }

    Types& StackContainer::ReferenceBack()
    {
        if(m_Size <= 0){
            throw std::runtime_error("[STACK] Stack size cannot be smaller than 0. Cannot Back().");
        }

        return m_Storage.at(m_Size - 1);
    }

    void StackContainer::Pop()
    {
        if(m_Size <= 0){
            throw std::runtime_error("[STACK] Stack size cannot be smaller than 0. Cannot pop.");
        }

        m_Size--;
    }
    
    Types StackContainer::PopMove()
    {
        if(m_Size <= 0){
            throw std::runtime_error("[STACK] Stack size cannot be smaller than 0. Cannot pop.");
        }

        auto result = std::visit([](auto&& obj) -> Types 
        {
            using type = std::remove_cvref_t<decltype(obj)>;

            if constexpr (std::same_as<type, ObjectPtr>)
            {
                return std::move(obj); 
            }
            else 
            {
                return obj;
            }
          
        }, m_Storage.at(m_Size - 1)); 
        
        
        m_Size--;

        return result;
    }

    std::size_t StackContainer::Size() const
    {
        return m_Size;
    }

}
