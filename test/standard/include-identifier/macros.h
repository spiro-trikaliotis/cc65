// vim: set et ts=2 sw=2:

static void handle_macro_inputline(std::string & inputline)
{
  // skip until the first colon (':')
  auto start_macro_name = std::find(inputline.begin(), inputline.end(), ':');

  if (start_macro_name == inputline.end()) {
    return;
  }

  // now, split the remaining on space and use them as macro names

  while (start_macro_name != inputline.end()) {
    start_macro_name = std::find_if(start_macro_name+1, inputline.end(), no_space);

    auto end_macro_name = std::find_if(start_macro_name, inputline.end(), space);

    if (start_macro_name != inputline.end()) {
      auto remaining = std::string(start_macro_name, end_macro_name);

      if (remaining != "[empty]") {
        macro_table.push_back(remaining);
      }
    }

    start_macro_name = end_macro_name;
  }
}

static void collect_macros(void)
{
  std::ifstream ifs(names_and_parameters::filename_output);

  if (ifs.fail()) {
    std::cerr << "Error opening output file " << names_and_parameters::filename_output << std::endl;
    exit(1);
  }

  std::string inputline;
  while (std::getline(ifs, inputline) && !ifs.eof()) {
    if (inputline.length() > 0) {

      switch (inputline[0]) {
        case ' ':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
          handle_macro_inputline(inputline);
          break;

        case 'M':
          if (inputline == "Macro Hash Table Summary") {
            break;
          }
          // FALL THROUGH

        default:
          std::cerr << "Unknown macro definition line: " << inputline << std::endl;
          exit(1);
      }
    }
  }

  std::sort(macro_table.begin(), macro_table.end());

  file_dump << "\n\nSorted macro table:\n-------------------\n";
  for (auto & macro: macro_table) {
    file_dump << macro << '\n';
  }

  file_dump << "===========================\n\n\n";
}


