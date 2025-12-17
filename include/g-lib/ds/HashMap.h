#ifndef GLIB_HASH_MAP_H
#define GLIB_HASH_MAP_H

namespace GLib::DS {

  template <typename K, typename V> class Hashmap {
    private:
      unsigned int m_Size;
      // Need a vector data struct

    public:

      Hashmap();
      ~Hashmap();
      Hashmap(Hashmap &&other) noexcept;
      Hashmap(const Hashmap &) = delete;
      Hashmap &operator=(const Hashmap &) = delete;

      void           Insert(const K& key, const V& value);
      bool           Contains(const K& key) const;
      V              Get(const K& key) const;
      unsigned int   Size() const;
      void           IsEmpty() const;
      void           Clear();

  }
};

#endif
