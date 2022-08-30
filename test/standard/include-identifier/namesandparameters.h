// vim: set et ts=2 sw=2:

class names_and_parameters {
  public:
    static std::string filename_cc65exe;
    static std::string filename_identifiers;
    static std::string filename_header;
    static std::string cc65_standard;
    static std::string pathname_temporary;

    static std::string target_c_filename;
    static std::string filename_output;
    static std::string filename_debugtables;
    static std::string filename_dumpfile;

    static void setpaths(void)
    {
      names_and_parameters::filename_output      = names_and_parameters::pathname_temporary + "/macros_"  + names_and_parameters::filename_header + "-" + names_and_parameters::cc65_standard + ".list";
      names_and_parameters::filename_debugtables = names_and_parameters::pathname_temporary + "/symbols_" + names_and_parameters::filename_header + "-" + names_and_parameters::cc65_standard + ".list";
      names_and_parameters::target_c_filename    = names_and_parameters::pathname_temporary + "/_test_"   + names_and_parameters::filename_header                       + ".c";
      names_and_parameters::filename_dumpfile    = names_and_parameters::pathname_temporary + "/"         + names_and_parameters::filename_header + "-" + names_and_parameters::cc65_standard + ".dump";
    }

  public:
    static void set(const std::string & newFilename_cc65exe,
                    const std::string & newFilename_identifiers,
                    const std::string & newFilename_header,
                    const std::string & newCc65_standard,
                    const std::string & newPathname_temporary)
    {
      filename_cc65exe     = newFilename_cc65exe;
      filename_identifiers = newFilename_identifiers;
      filename_header      = newFilename_header;
      cc65_standard        = newCc65_standard;
      pathname_temporary   = newPathname_temporary;

      setpaths();
    }
};

std::string names_and_parameters::filename_cc65exe;
std::string names_and_parameters::filename_identifiers;
std::string names_and_parameters::filename_header;
std::string names_and_parameters::cc65_standard;
std::string names_and_parameters::pathname_temporary;

std::string names_and_parameters::target_c_filename;
std::string names_and_parameters::filename_output;
std::string names_and_parameters::filename_debugtables;
std::string names_and_parameters::filename_dumpfile;

