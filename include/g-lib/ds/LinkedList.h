#ifndef GLIB_LINKED_LIST_H
#define GLIB_LINKED_LIST_H

#include "Node.h"

namespace GLib::DS {
  enum class LinkedListEnd {
    BEGINNING   = 0,
    END         = 1
  };

  template<typename T> 
  class LinkedList {
    private:
      unsigned int   m_Size;
      SNode<T>*      m_pHead;
      SNode<T>*      m_pTail;
      
      // May setup an optional initial pool of nodes already allocated by memory

    public:  
      LinkedList     ();
      ~LinkedList    ();
      LinkedList     (LinkedList&& other) noexcept;
      LinkedList     (const LinkedList&) = delete;
      LinkedList&    operator=(const LinkedList&) = delete;
      LinkedList&    operator=(LinkedList&& other) noexcept;
      
      void           Push(const T& value, LinkedListEnd end = LinkedListEnd::END);
      T              Pop(LinkedListEnd end = LinkedListEnd::END); 
      void           Clear();
      
      T              Peek(LinkedListEnd end = LinkedListEnd::END) const;
      unsigned int   size() const;

  };

} // namespce GLib::DS
#endif
