// vim: set et ts=2 sw=2:

class reference_identifier {
  public:

    enum standards {
      STD_C89,
      STD_C99,
      STD_CC65,
      STD_LAST
    };

    enum type {
      TYPE_MACRO,
      TYPE_SYMBOL,
      TYPE_TAG,
      TYPE_STRUCT_ELEMENT,
      TYPE_LAST
    };

  private:
    std::string identifier;
    bool        type[TYPE_LAST] = { };
    bool        standard[STD_LAST] = { };

    void setType(const std::string & TypeString)
    {
      if (TypeString.size() == 0) {
        std::cerr << "*** empty type string, aborting...\n";
        exit(1);
      }

      for (auto & it : TypeString) {
        switch (it) {
          case 'M':
            this->type[TYPE_MACRO] = true;
            break;

          case 'S':
            this->type[TYPE_SYMBOL] = true;
            break;

          case 'T':
            this->type[TYPE_TAG] = true;
            break;

          case 'E':
            this->type[TYPE_STRUCT_ELEMENT] = true;
            break;


          default:
            std::cerr << "*** unrecognized type string " << TypeString << ", aborting...\n";
            exit(1);
        }
      }
    }

    void setStandards(const std::string & StandardsString)
    {
      auto start_standard_name = StandardsString.begin();

      while (start_standard_name != StandardsString.end()) {

        start_standard_name    = std::find_if(start_standard_name, StandardsString.end(), no_space);

        auto end_standard_name = std::find(start_standard_name, StandardsString.end(), ' ');

        auto standard = std::string(start_standard_name, end_standard_name);

        if (standard == "c89") {
          this->standard[STD_C89] = true;
        }
        else if (standard == "c99") {
          this->standard[STD_C99] = true;
        }
        else if (standard == "cc65") {
          this->standard[STD_CC65] = true;
        }
        else {
          std::cerr << "*** unrecognized standard string " << standard << ", aborting...\n";
          exit(1);
        }

        start_standard_name = end_standard_name;
      }
    }

    std::string getTypeString(void)
    {
      std::stringstream st;

      if (this->type[TYPE_MACRO])          st << "MACRO ";
      if (this->type[TYPE_SYMBOL])         st << "SYMBOL ";
      if (this->type[TYPE_TAG])            st << "TAG ";
      if (this->type[TYPE_STRUCT_ELEMENT]) st << "STRUCTELEMENT ";

      std::string str = st.str();
      rtrim(str);

      return str;
    }

    std::string getStandardsString(void)
    {
      std::string standards;

      if (this->standard[STD_C89]) {
        standards.append(" C89");
      }
      if (this->standard[STD_C99]) {
        standards.append(" C99");
      }
      if (this->standard[STD_CC65]) {
        standards.append(" CC65");
      }

      return standards;
    }


  public:
    reference_identifier(const std::string & newIdentifier, const std::string & newType, const std::string & newStandards)
      : identifier(newIdentifier)
    {
      this->setType(newType);
      this->setStandards(newStandards);
    }

    const std::string & getName(void) const
    {
      return this->identifier;
    }

    std::string dump(void)
    {
      return identifier + ',' + this->getTypeString() + ',' + this->getStandardsString();
    }
};

bool reference_identifier_compare(const reference_identifier &a, const reference_identifier &b)
{
  return a.getName() < b.getName();
}
