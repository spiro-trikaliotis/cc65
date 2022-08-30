// vim: set et ts=2 sw=2:

#include <algorithm>
#include <fstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "helper.h"

static const bool debugoutput = false;
static std::ofstream file_dump;

#include "namesandparameters.h"

#include "symboltables.h"
#include "reference_identifier.h"

static std::vector<std::string>          macro_table;
static Symboltable                       symbol_table;
static Symboltable                       tag_table;
static std::vector<reference_identifier> reference_table;

#include "macros.h"
#include "symbols.h"
#include "exec_cc65_ll.h"
#include "exec_cc65.h"
#include "collect_reference.h"

// command-line:
// $1 - path to cc65
// $2 - file which contains the identifiers
// $3 - header file to test
// $4 - options (c89, c99, cc65)
// $5 - working path (where testwrk/ resides)
//
int main(int argc, char **argv)
{

  if (argc != 1 + 5) {
    std::cerr << "You must provide 5 parameters, aborting..." << std::endl;
    return 1;
  }

  names_and_parameters::set(std::string(argv[1]),
                            std::string(argv[2]),
                            std::string(argv[3]),
                            std::string(argv[4]),
                            std::string(argv[5]));

  file_dump.open(names_and_parameters::filename_dumpfile);

  if (debugoutput) {
    std::cout << "Calling cc65 at " << names_and_parameters::filename_cc65exe
      << "\nwith temporary dir at " << names_and_parameters::pathname_temporary
      << ",\nthe identifiers are in the file " << names_and_parameters::filename_identifiers
      << ",\nthe header file is " << names_and_parameters::filename_header
      << "\nand I will use the cc65 standard " << names_and_parameters::cc65_standard << std::endl;
  }

  call_cc65_for_test();
  collect_reference_data();

  return 0;
}
