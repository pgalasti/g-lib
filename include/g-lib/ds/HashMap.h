#ifndef GLIB_HASH_MAP_H
#define GLIB_HASH_MAP_H

#include <vector>
#include "Seek.h"

namespace GLib::DS {

  template <typename K, typename V> class Hashmap : public Seek<K> {
    private:
      unsigned int m_Size;
      
      // I may write my own vector structure later for fun
      std::vector<std::pair<K, V>> m_Data;

    public:

      Hashmap();
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
