#ifndef GLIB_HASH_MAP_H
#define GLIB_HASH_MAP_H

#include <functional>
#include <vector>
#include "Seek.h"

namespace GLib::DS {

  template <typename K, typename V> class Hashmap : public Seek<K> {
    private:
      static constexpr unsigned int DEFAULT_CAPACITY = 128;

      unsigned int m_Size;
      unsigned int m_Capacity;

      // I may write my own vector structure later for fun
      std::vector<std::vector<std::pair<K, V>>> m_Buckets;

    public:

      Hashmap(const unsigned int capacity = DEFAULT_CAPACITY);
      ~Hashmap();
      Hashmap(Hashmap &&other) noexcept;
      Hashmap(const Hashmap &) = delete;
      Hashmap &operator=(const Hashmap &) = delete;

      void           Insert(const K& key, const V& value);
      bool           Contains(const K& key) const override;
      V              Get(const K& key) const;
      unsigned int   Size() const;
      bool           IsEmpty() const;
      void           Clear();

  };
}

#include "HashMap.tpp"
#endif
