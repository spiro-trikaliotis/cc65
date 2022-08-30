// vim: set et ts=2 sw=2:

const std::string  sc_struct_string_prefix = "SC_STRUCT: ";
const unsigned int sc_struct_string_prefix_len = sc_struct_string_prefix.size();

static void collect_symbols(void)
{
  std::ifstream ifs(names_and_parameters::filename_debugtables);

  if (ifs.fail()) {
    std::cerr << "Error opening output file " << names_and_parameters::filename_output << std::endl;
    exit(1);
  }

  enum symbol_type_state {
    STS_UNKNOWN,
    STS_GLOBAL_SYMBOL_TABLE,
    STS_GLOBAL_TAG_TABLE,
    STS_GLOBAL_STRUCT_UNION_DEF
  };

  symbol_type_state sts = STS_UNKNOWN;

  std::string inputline;
  symbol_entry * current_sc_struct = NULL;

  while (std::getline(ifs, inputline) && !ifs.eof()) {
    if (inputline.length() > 0) {
      if (inputline == "Global symbol table") {
        sts = STS_GLOBAL_SYMBOL_TABLE;
        continue;
      }
      else if (inputline == "Global tag table") {
        sts = STS_GLOBAL_TAG_TABLE;
        continue;
      }
      else if (inputline == "Global struct and union definitions") {
        sts = STS_GLOBAL_STRUCT_UNION_DEF;
        continue;
      }
      else if (sts == STS_GLOBAL_STRUCT_UNION_DEF && std::string(inputline, 0, sc_struct_string_prefix_len) == sc_struct_string_prefix) {
        // special processing for SC_STRUCT in "Global struct and union definition" area

        auto identifier = inputline.substr(sc_struct_string_prefix_len);

        current_sc_struct = tag_table.find(identifier);
        if (current_sc_struct == NULL) {
          std::cerr << "identifier " << identifier << " not found for SC_STRUCT, aborting.\n";
          exit(1);
        }
        continue;
      }
      else if (inputline[0] == '=') {
        // skip a header marker, if it is one (consists only of equal signs)
        if (std::find_if(inputline.begin(), inputline.end(), no_equal_char) != inputline.end()) {
          std::cerr << "I do not understand the line: " << inputline << ", aborting.\n";
          exit(1);
        }
        continue;
      }
      else if (inputline[0] == ' ') {
        auto start_type_pos = std::find_if(inputline.begin(), inputline.end(), no_space);
        auto colon_type_pos = std::find   (start_type_pos, inputline.end(), ':');

        auto start_text_pos = std::find_if(colon_type_pos, inputline.end(), no_space);
        start_text_pos      = std::find_if(start_text_pos, inputline.end(), space);
        start_text_pos      = std::find_if(start_text_pos, inputline.end(), no_space);

        if (start_text_pos != inputline.end()) {
          auto type = std::string(start_type_pos, colon_type_pos);
          auto text = std::string(start_text_pos, inputline.end());

          if (type == "AsmName") {
            switch (sts) {
              case STS_GLOBAL_SYMBOL_TABLE:
                symbol_table.back().setAsmName(text);
                break;
              case STS_GLOBAL_TAG_TABLE:
                tag_table.back().setAsmName(text);
                break;
              case STS_GLOBAL_STRUCT_UNION_DEF:
                if (current_sc_struct) {
                  current_sc_struct->getSubSymbolTable().back().setAsmName(text);
                }
                break;
            }
          }
          else if (type == "Flags") {
            switch (sts) {
              case STS_GLOBAL_SYMBOL_TABLE:
                symbol_table.back().setFlags(text);
                break;
              case STS_GLOBAL_TAG_TABLE:
                tag_table.back().setFlags(text);
                break;
              case STS_GLOBAL_STRUCT_UNION_DEF:
                if (current_sc_struct) {
                  current_sc_struct->getSubSymbolTable().back().setFlags(text);
                }
                break;
            }
          }
          else if (type == "Type") {
            switch (sts) {
              case STS_GLOBAL_SYMBOL_TABLE:
                symbol_table.back().setType(text);
                break;
              case STS_GLOBAL_TAG_TABLE:
                tag_table.back().setType(text);
                break;
              case STS_GLOBAL_STRUCT_UNION_DEF:
                if (current_sc_struct) {
                  current_sc_struct->getSubSymbolTable().back().setType(text);
                }
                break;
            }
          }
        }
        continue;
      }
      else {
        auto colon_pos = std::find(inputline.begin(), inputline.end(), ':');
        auto symbol = std::string(inputline.begin(), colon_pos);

        switch (sts) {
          case STS_UNKNOWN:
            std::cerr << "Symbol definition before a header, aborting\n";
            exit(1);
            break;

          case STS_GLOBAL_SYMBOL_TABLE:
            symbol_table.push_back(symbol_entry(symbol));
            break;
          case STS_GLOBAL_TAG_TABLE:
            tag_table.push_back(symbol_entry(symbol));
            break;
          case STS_GLOBAL_STRUCT_UNION_DEF:
            if (current_sc_struct) {
              current_sc_struct->getSubSymbolTable().push_back(symbol_entry(symbol));
            }
            break;
        }
      }
    }
  }


  std::sort(symbol_table.begin(), symbol_table.end(), symbol_entry_compare);
  std::sort(tag_table.begin(),    tag_table.end(),    symbol_entry_compare);

  file_dump << "\n\nSorted symbol table:\n--------------------\n";
  symbol_table.dump();
  file_dump << "===========================\n\n\n";

  file_dump << "\n\nSorted tag table:\n-----------------\n";
  tag_table.dump();

  file_dump << "===========================\n\n\n";
}


