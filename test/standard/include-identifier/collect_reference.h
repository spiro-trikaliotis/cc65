// vim: set et ts=2 sw=2:

static void collect_reference_data(void)
{
  std::ifstream ifs(names_and_parameters::filename_identifiers);

  if (ifs.fail()) {
    std::cerr << "Error opening output file "
              << names_and_parameters::filename_identifiers << std::endl;
    exit(1);
  }

  /*
    format of each line: identifier,type,standard
    where
    - identifier: the identifier that should be define
    - type:       M - must be a macro
                  S - must be a symbol (not a macro)
                  A - can be either a macro or a symbol (not given)
                  T - must be a tag (for a struct or union)
                  E - must be a struct or union element

    comment lines start with a hash ('#');
    comment lines and empty lines are ignored
  */
  std::string inputline;
  while (std::getline(ifs, inputline) && !ifs.eof()) {
    if (inputline.length() == 0) {
      // empty line, ignore it
      continue;
    }
    if (inputline[0] == '#') {
      // comment line, ignore it
      continue;
    }

    // skip spaces at the beginning
    auto identifier_start_pos = find_if(inputline.begin(), inputline.end(),  no_space);
    auto colon1_pos           = find   (identifier_start_pos, inputline.end(),  ',');
    auto type_start_pos       = find_if(colon1_pos + 1,    inputline.end(),  no_space);
    auto colon2_pos           = find   (type_start_pos,    inputline.end(),  ',');
    auto standard_start_pos   = find_if(colon2_pos + 1,    inputline.end(),  no_space);

    auto identifier = std::string(identifier_start_pos,  colon1_pos);
    auto type       = std::string(type_start_pos,     colon2_pos);
    auto standards  = std::string(standard_start_pos, inputline.end());

    rtrim(identifier);
    rtrim(type);
    rtrim(standards);

    reference_table.push_back(reference_identifier(identifier, type, standards));
  }

  std::sort(reference_table.begin(), reference_table.end(), reference_identifier_compare);

  file_dump << "\n\nSorted reference table:\n-----------------------\n";
  for (auto & reference: reference_table) {
    file_dump << reference.dump() << '\n';
  }

  file_dump << "===========================\n\n\n";
}
