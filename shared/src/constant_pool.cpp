#include "shared/constant_pool.h"
#include <type_traits>

namespace shared
{

    ConstantPool::ConstantPool(std::vector<Types> &&constantPool)
        : m_Storage(std::move(constantPool))
    {
    }

    ConstantPool::ConstantPool(const ConstantPool& constantPool)
    {
        m_Storage.reserve(constantPool.m_Storage.size());

        for (const auto& object : constantPool.m_Storage)
        {
            auto result = std::visit([](auto&& obj) ->Types
            {
                using type = std::remove_cvref_t<decltype(obj)>;

                if constexpr (std::is_same_v<type, ObjectPtr>)
                {
                    return obj->Clone();
                }
                else
                {
                    return obj;        
                }
            }, object);

            m_Storage.emplace_back(std::move(result));
        }
    }

    ConstantPool::ConstantPool(ConstantPool &&constantPool)
        : m_Storage(std::move(constantPool.m_Storage))
    {
    }

    ConstantPool::ConstantPool()
    {
    }

    /**
     * @todo Is moving the right approach here?
     */
    ConstantPool &ConstantPool::operator=(ConstantPool &&other)
    {
        if (this != &other)
        {
            m_Storage = std::move(other.m_Storage);
        }
        
        return *this;
    }

    Types ConstantPool::GetConstant(std::size_t index)
    {
        if (index >= m_Storage.size())
        {
            throw std::runtime_error("[Constant Pool] Get() Failed: Index out of bounds.");
        }

        return std::visit([](auto&& obj) -> Types
          {
              using type = std::remove_reference_t<decltype(obj)>;

              if constexpr (std::is_same_v<type, ObjectPtr>)
              {
                  return obj->Clone();
              }
              else 
              {
                  return obj;
              }
          }, m_Storage[index]);
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
}
