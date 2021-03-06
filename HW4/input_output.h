// ======================================================================
// PROVIDED CODE - DO NOT EDIT THIS FILE
// ======================================================================

#ifndef _INPUT_OUTPUT_H_
#define _INPUT_OUTPUT_H_

#include <string>
#include <list>
#include <cassert>


//
// The PARSE enum type indicates how the input will file will be split
// into elements for difference comparison.
//
enum PARSE { CHAR, WORD, LINE };


//
// A helper function that converts a command line string into a PARSE
// enum.
//
PARSE parse_type(const std::string &type);


//
// Helper functions to read input text (or code) files.  And write out
// text (or code) files reconstructed after applying diff edit
// operations to an input file.
//
// Input files are always stored as a list of strings.  The strings
// will be single letters (for the CHAR parse type), a WORD of one or
// more characters with no white space, or a LINE ending in a newline
// character as indicated.
//
std::list<std::string> read_text(PARSE type, const std::string &filename);
void write_text(PARSE type, const std::list<std::string> &text, const std::string &filename);


//
// Helper functions to read and write difference files.  Difference
// files store the insert, erase, and replace operations.
//
std::list<Diff> read_diff(PARSE type, const std::string &filename);
void write_diff(PARSE type, const std::list<Diff> &diff, const std::string &filename);

#endif
