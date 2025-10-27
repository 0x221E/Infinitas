#include "virtual-machine/stack_container.h"
#include <shared/object_visitors.h>

namespace vm
{

    StackContainer::StackContainer(std::size_t capacity) :  m_Size(0)
    {
        m_Storage.reserve(capacity); /** @todo Add optimization. */
    }

    StackContainer::StackContainer(const StackContainer& other) : m_Storage(other.CloneStorage()), m_Size(other.m_Size)
    {
        
    }
    
    StackContainer& StackContainer::operator=(const StackContainer& other)
    {
        if(this == &other) return *this;
        m_Size = other.m_Size; 
        std::vector<Types> temp = other.CloneStorage();
        m_Storage.swap(temp);  
        return *this;
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
        m_Size--;
        return std::move(m_Storage.at(m_Size));
    }

    std::size_t StackContainer::Size() const
    {
        return m_Size;
    }

    std::vector<Types> StackContainer::CloneStorage() const
    {
        std::vector<Types> temp;
        temp.reserve(m_Storage.size());
        for(const auto& elem : m_Storage)
        {
            temp.emplace_back(std::visit(shared::CopyObject{}, elem));
        }
        return temp;
    }

}
