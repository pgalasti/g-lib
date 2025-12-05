#ifndef G_STRING_PARSER_H
#define G_STRING_PARSER_H

#include <list>
#include <string>

namespace GLib::Util {
class StringParser {
public:
  StringParser();
  StringParser(std::string stringToParse, std::string strToken);
  ~StringParser() {}

  void Parse(std::string stringToParse, std::string strToken);
  const std::string getToken() const;
  unsigned int getSize() const;
  bool isFirst() const;
  bool isLast() const;
  bool getNext();
  bool getBack();
  bool getFirst();
  bool getLast();
  bool operator++(int);
  bool operator--(int);

protected:
  std::string m_OriginalString;
  std::list<std::string> m_TokenList;
  std::list<std::string>::iterator m_TokenListItr;
  std::string m_CurrentToken;

private:
  void Init();
};
} // namespace GLib::Util

#endif
