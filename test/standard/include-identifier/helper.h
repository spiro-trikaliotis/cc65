// vim: set et ts=2 sw=2:

static inline bool no_space(char c)
{
  return !isspace(c);
}

static inline bool space(char c)
{
  return isspace(c);
}

static inline bool no_equal_char(char c)
{
  return c != '=';
}

static inline void rtrim(std::string &s)
{
  s.erase(std::find_if(s.rbegin(), s.rend(), no_space).base(), s.end());
}

