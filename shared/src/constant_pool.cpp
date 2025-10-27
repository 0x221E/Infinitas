#include "shared/constant_pool.h"
#include <type_traits>
#include <utility>
#include "shared/object_visitors.h"

namespace shared
{

    ConstantPool::ConstantPool(std::vector<Types> &&constantPool)
        : m_Storage(std::move(constantPool))
    {
    }

    ConstantPool::ConstantPool(const ConstantPool& other) : m_Storage(other.CloneStorage())
    {
    }

    ConstantPool::ConstantPool(ConstantPool &&constantPool)
        : m_Storage(std::move(constantPool.m_Storage))
    {
    }

    ConstantPool::ConstantPool()
    {
    }

    ConstantPool &ConstantPool::operator=(ConstantPool &&other)
    {
        if (this == &other) return *this;
        m_Storage = std::move(other.m_Storage);
        return *this;
    }

    ConstantPool &ConstantPool::operator=(const ConstantPool& other)
    {
        if (this == &other) return *this;
        std::vector<Types> temp = other.CloneStorage();
        m_Storage.swap(temp);
        return *this;
    }

    Types ConstantPool::GetConstant(std::size_t index)
    {
        if (index >= m_Storage.size())
        {
            throw std::runtime_error("[Constant Pool] Get() Failed: Index out of bounds.");
        }

        return std::visit(CopyObject{}, m_Storage[index]);
    }

    [[nodiscard]] std::vector<Types> ConstantPool::MoveConstants() noexcept
    {
        return std::move(m_Storage);
    }

    std::size_t ConstantPool::Size() const noexcept
    {
        return m_Storage.size();
    }

    void ConstantPool::InsertConstants(std::vector<Types> &&constants)
    {
        m_Storage.reserve(m_Storage.capacity() + m_Storage.capacity() * 2); /** @todo Implement better optimization. */
        m_Storage.insert(m_Storage.end(), std::make_move_iterator(constants.begin()), std::make_move_iterator(constants.end()));
    }

    void ConstantPool::InsertConstant(Types &&constant)
    {
        m_Storage.emplace_back(std::move(constant));
    }

    void ConstantPool::ReplaceConstants(std::vector<Types> &&constants)
    {
        m_Storage = std::move(constants);
    }

    std::vector<Types> ConstantPool::CloneStorage() const
    {
        std::vector<Types> temp;
        temp.reserve(m_Storage.size());
        for(const auto& elem : m_Storage)
        {
            temp.emplace_back(std::visit(CopyObject{}, elem));
        }
        return temp;    
    }
}
