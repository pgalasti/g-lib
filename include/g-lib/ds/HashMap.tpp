#include <stdexcept>

namespace GLib::DS {

template <typename K, typename V>
Hashmap<K, V>::Hashmap(const unsigned int capacity)
    : m_Size(0), m_Capacity(capacity), m_Buckets(capacity) {}

template <typename K, typename V>
Hashmap<K, V>::~Hashmap() { Clear(); }

template <typename K, typename V>
Hashmap<K, V>::Hashmap(Hashmap &&other) noexcept
    : m_Size(other.m_Size), m_Capacity(other.m_Capacity), m_Buckets(std::move(other.m_Buckets)) {
  other.m_Size = 0;
  other.m_Capacity = 0;
}

template <typename K, typename V>
void Hashmap<K, V>::Insert(const K &key, const V &value) {
  const unsigned int index = std::hash<K>{}(key) % m_Capacity;
  for (auto &pair : m_Buckets[index]) {
    if (pair.first == key) {
      pair.second = value;
      return;
    }
  }
  m_Buckets[index].emplace_back(key, value);
  ++m_Size;
}

template <typename K, typename V>
bool Hashmap<K, V>::Contains(const K &key) const {
  const unsigned int index = std::hash<K>{}(key) % m_Capacity;
  for (const auto &pair : m_Buckets[index]) {
    if (pair.first == key) return true;
  }
  return false;
}

template <typename K, typename V>
V Hashmap<K, V>::Get(const K &key) const {
  const unsigned int index = std::hash<K>{}(key) % m_Capacity;
  for (const auto &pair : m_Buckets[index]) {
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
  for (auto &bucket : m_Buckets) bucket.clear();
  m_Size = 0;
}

} // namespace GLib::DS
