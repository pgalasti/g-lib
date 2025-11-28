#ifndef G_STRING_PARSER_H
#define G_STRING_PARSER_H

#include<list>
#include<string>

namespace GLib {  
  class StringParser
  {
    public:
	  StringParser();
      StringParser(std::string stringToParse, std::string strToken);
	  ~StringParser() {}

	  void Parse(std::string stringToParse, std::string strToken);
	  const std::string getToken() const;
	  const unsigned int getSize() const;
	  const bool isFirst() const;
	  const bool isLast() const;
	  const bool getNext();
	  const bool getBack();
	  const bool getFirst();
	  const bool getLast();
	  const bool operator++(int);
	  const bool operator--(int);

	protected:
	  std::string m_OriginalString;
	  std::list<std::string> m_TokenList;
	  std::list<std::string>::iterator m_TokenListItr;
	  std::string m_CurrentToken;

	private:
	  void Init();
  };
}

#endif
