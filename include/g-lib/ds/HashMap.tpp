#include <stdexcept>

namespace GLib::DS {

template <typename K, typename V>
Hashmap<K, V>::Hashmap() : m_Size(0) {}

template <typename K, typename V>
Hashmap<K, V>::~Hashmap() { Clear(); }

template <typename K, typename V>
Hashmap<K, V>::Hashmap(Hashmap &&other) noexcept
    : m_Size(other.m_Size), m_Data(std::move(other.m_Data)) {
  other.m_Size = 0;
}

template <typename K, typename V>
void Hashmap<K, V>::Insert(const K &key, const V &value) {
  for (auto &pair : m_Data) {
    if (pair.first == key) {
      pair.second = value;
      return;
    }
  }
  m_Data.emplace_back(key, value);
  ++m_Size;
}

template <typename K, typename V>
bool Hashmap<K, V>::Contains(const K &key) const {
  for (const auto &pair : m_Data) {
    if (pair.first == key) return true;
  }
  return false;
}

template <typename K, typename V>
V Hashmap<K, V>::Get(const K &key) const {
  for (const auto &pair : m_Data) {
    if (pair.first == key) return pair.second;
  }
  throw std::runtime_error("Key not found");
}

template <typename K, typename V>
unsigned int Hashmap<K, V>::Size() const { return m_Size; }

template <typename K, typename V>
bool Hashmap<K, V>::IsEmpty() const { return m_Size == 0; }

template <typename K, typename V>
void Hashmap<K, V>::Clear() {
  m_Data.clear();
  m_Size = 0;
}

} // namespace GLib::DS
