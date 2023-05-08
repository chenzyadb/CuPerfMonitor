#pragma once

/*
     =======  /     /   ======  ======   |    /
    /        /     /   /       /         |   /
   /        /     /   /        ======    |  /
  /        /     /   /              /    | /
  =======  ======    ======   ======      /
  - A simple CSV parser based on C++ STL.
  - Author: chenzyyzd@github.com
  - Example: 
   You can use this library to parse csv as easy as Excel.
   Original:
    A,B,C,D,E,F,G
    , , , , , ,
    , , ,:-), , ,
    , , , , , ,
   Use CuCsv:
     0 | A | B | C | D | E | F | G |
     1 |   |   |   |   |   |   |   |
     2 |   |   |   |:-)|   |   |   |
     3 |   |   |   |   |   |   |   |
   CuCsv.GetCsvCell(myCsv, "D", 2) == ":-)"
*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

class CuCsv {
    public:
        CuCsv();
        ~CuCsv();

        using Csv = std::unordered_map<std::string, std::vector<std::string>>;
        using CsvColumn = std::vector<std::string>;
        using CsvRow = std::unordered_map<std::string, std::string>;

        Csv ParseCsv(const std::string &csvText, const std::string &delimiter);
        Csv CreateCsv(const std::vector<std::string> &columnNames);
        std::string PrintCsv(const Csv &csv, const std::string &delimiter);
        CsvColumn GetCsvColumn(const Csv &csv, const std::string &columnName);
        CsvRow GetCsvRow(const Csv &csv, const size_t &rowIdx);
        std::string GetCsvCell(const Csv &csv, const std::string &columnName, const size_t &rowIdx);
        Csv SetCsvColumn(const Csv &csv, const std::string &columnName, const CsvColumn &newColumn);
        Csv AppendCsvColumn(const Csv &csv, const CsvColumn &newColumn);
        Csv DeleteCsvColumn(const Csv &csv, const std::string &columnName);
        Csv SetCsvRow(const Csv &csv, const size_t &rowIdx, const CsvRow &newRow);
        Csv InsertCsvRow(const Csv &csv, const size_t &rowIdx, const CsvRow &newRow);
        Csv AppendCsvRow(const Csv &csv, const CsvRow &newRow);
        Csv DeleteCsvRow(const Csv &csv, const size_t &rowIdx);
        Csv SetCsvCell(const Csv &csv, const std::string &columnName, const size_t &rowIdx, const std::string &newCell);

    private:
        std::vector<std::string> StrSplit(const std::string &str, const std::string &delimiter);
        std::string FormatString(const std::string &str);
};
