// vim: set et ts=2 sw=2:

class symbol_entry {
  private:
    std::string name;
    std::string asmname;
    std::string flags;
    std::string type;
    std::vector<symbol_entry> sub_symbols;

  public:
    symbol_entry(const std::string & newName)
      : name(newName)
    {
    }

    void setAsmName(const std::string & newAsmName)
    {
      this->asmname = newAsmName;
    }

    void setFlags(const std::string & newFlags)
    {
      this->flags = newFlags;
    }

    void setType(const std::string & newType)
    {
      if (newType == "(none)") {
        this->type = "";
      }
      else {
        this->type = newType;
      }
    }

    const std::string & getName() const
    {
      return this->name;
    }

    std::vector<symbol_entry> & getSubSymbolTable(void)
    {
      return sub_symbols;
    }

    std::string dump() const
    {
      return this->name + "," + this->asmname + "," + this->type + "," + this->flags;
    }
};

bool symbol_entry_compare(const symbol_entry &a, const symbol_entry &b)
{
  return a.getName() < b.getName();
}

class Symboltable {
  private:
    std::vector<symbol_entry> entries;

  public:
    symbol_entry & back()
    {
      return entries.back();
    }

    void push_back(const symbol_entry & entry)
    {
      entries.push_back(entry);
    }

    std::vector<symbol_entry>::iterator begin()
    {
      return entries.begin();
    }

    std::vector<symbol_entry>::iterator end()
    {
      return entries.end();
    }

    symbol_entry * find(const std::string & identifier)
    {
      for ( auto & it : entries) {
        if (it.getName() == identifier) {
          return &it;
        }
      }
      return NULL;
    }

    void dump(void)
    {
      for (auto & symbol: entries) {
        file_dump << symbol.dump() << '\n';

        auto & sub_symbol_table = symbol.getSubSymbolTable();

        if (sub_symbol_table.size() > 0) {
          for (auto & it: sub_symbol_table) {
            file_dump << ' ' << it.dump() << '\n';
          }
        }
      }
    }
};
