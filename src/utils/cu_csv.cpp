#include "cu_csv.h"

CuCsv::CuCsv() { }

CuCsv::~CuCsv() { }

CuCsv::Csv CuCsv::ParseCsv(const std::string &csvText, const std::string &delimiter)
{
    std::string formattedText = FormatString(csvText);
    if (formattedText.empty()) {
        throw std::runtime_error("CuCsv: CsvText is empty.");
    }
    if (delimiter.empty()) {
        throw std::runtime_error("CuCsv: Delimiter is empty.");
    }

    std::vector<std::string> lines = StrSplit(formattedText, "\n");
    size_t columnNum = StrSplit(lines[0], delimiter).size();
    if (columnNum == 0) {
        throw std::runtime_error("CuCsv: Invalid CSV format.");
    }

    std::vector<std::vector<std::string>> csvBase{};
    csvBase.resize(columnNum);
    for (const std::string &line : lines) {
        std::vector<std::string> row = StrSplit(line, delimiter);
        if (row.size() == columnNum) {
            for (size_t idx = 0; idx < columnNum; idx++) {
                csvBase[idx].emplace_back(row[idx]);
            }
        } else if (row.size() == columnNum - 1) {
            for (size_t idx = 0; idx < row.size(); idx++) {
                csvBase[idx].emplace_back(row[idx]);
            }
            csvBase[columnNum - 1].emplace_back("");
        } else {
            throw std::runtime_error("CuCsv: Invalid CSV format.");
        }
    }

    Csv csv{};
    for (const auto &column : csvBase) {
        csv[column[0]] = column;
    }

    return csv;
}

CuCsv::Csv CuCsv::CreateCsv(const std::vector<std::string> &columnNames)
{
    if (columnNames.size() == 0) {
        throw std::runtime_error("CuCsv: columnNames is empty.");
    }

    Csv newCsv{};
    for (const std::string &columnName : columnNames) {
        newCsv[columnName] = std::vector<std::string>(1, columnName);
    }

    return newCsv;
}

std::string CuCsv::PrintCsv(const Csv &csv, const std::string &delimiter)
{
    if (delimiter.empty()) {
        throw std::runtime_error("CuCsv: Delimiter is empty.");
    }

    std::vector<std::vector<std::string>> csvBase{};
    for (const auto &[colunmName, column] : csv) {
        csvBase.emplace_back(column);
    }

    std::string csvText = "";
    for (size_t rowIdx = 0; rowIdx < csvBase[0].size(); rowIdx++) {
        for (size_t columnIdx = 0; columnIdx < (csvBase.size() - 1); columnIdx++) {
            csvText += csvBase[columnIdx][rowIdx] + delimiter;
        }
        csvText += csvBase[csvBase.size() - 1][rowIdx] + "\n";
    }

    return csvText;
}

CuCsv::CsvColumn CuCsv::GetCsvColumn(const Csv &csv, const std::string &columnName)
{
    if (csv.count(columnName) == 0) {
        throw std::runtime_error("CuCsv: Column does not exist.");
    }

    return csv.at(columnName);
}

CuCsv::CsvRow CuCsv::GetCsvRow(const Csv &csv, const size_t &rowIdx)
{
    CsvRow row{};
    for (const auto &[columnName, column] : csv) {
        if (rowIdx >= column.size() || rowIdx < 0) {
            throw std::runtime_error("CuCsv: Row does not exist.");
        }
        row[columnName] = column[rowIdx];
    }

    return row;
}

std::string CuCsv::GetCsvCell(const Csv &csv, const std::string &columnName, const size_t &rowIdx)
{
    if (csv.count(columnName) == 0) {
        throw std::runtime_error("CuCsv: Column does not exist.");
    }
    const auto &column = csv.at(columnName);
    if (rowIdx >= column.size() || rowIdx < 0) {
        throw std::runtime_error("CuCsv: Row does not exist.");
    }
    std::string cellText = column[rowIdx];

    return cellText;
}

CuCsv::Csv CuCsv::SetCsvColumn(const Csv &csv, const std::string &columnName, const CsvColumn &newColumn)
{
    if (newColumn.size() == 0) {
        throw std::runtime_error("CuCsv: NewColumn is empty.");
    }

    Csv newCsv = csv;
    if (csv.count(columnName) == 1) {
        newCsv[columnName] = newColumn;
    } else {
        throw std::runtime_error("CuCsv: Column does not exist.");
    }

    for (auto &[_columnName, column] : newCsv) {
        if (column.size() < newColumn.size()) {
            column.resize(newColumn.size());
        } else if (newColumn.size() < column.size()) {
            newCsv[newColumn[0]].resize(column.size());
            break;
        }
    }

    return newCsv;
}

CuCsv::Csv CuCsv::AppendCsvColumn(const Csv &csv, const CsvColumn &newColumn)
{
    if (newColumn.size() == 0) {
        throw std::runtime_error("CuCsv: NewColumn is empty.");
    }

    Csv newCsv = csv;
    if (csv.count(newColumn[0]) == 0) {
        newCsv[newColumn[0]] = newColumn;
    } else {
        throw std::runtime_error("CuCsv: Column already exist.");
    }

    for (auto &[_columnName, column] : newCsv) {
        if (column.size() < newColumn.size()) {
            column.resize(newColumn.size());
        } else if (newColumn.size() < column.size()) {
            newCsv[newColumn[0]].resize(column.size());
            break;
        }
    }

    return newCsv;
}

CuCsv::Csv CuCsv::DeleteCsvColumn(const Csv &csv, const std::string &columnName)
{
    Csv newCsv = csv;
    newCsv.erase(columnName);

    return newCsv;
}

CuCsv::Csv CuCsv::SetCsvRow(const Csv &csv, const size_t &rowIdx, const CsvRow &newRow)
{
    if (newRow.size() == 0) {
        throw std::runtime_error("CuCsv: NewRow is empty.");
    }

    Csv newCsv = csv;
    for (auto &[columnName, column] : newCsv) {
        if (rowIdx < column.size() && rowIdx > 0) {
            if (newRow.count(columnName) == 0) {
                column[rowIdx] = "";
            } else {
                column[rowIdx] = newRow.at(columnName);
            }
        } else if (rowIdx == 0) {
            throw std::runtime_error("CuCsv: ColumnName cannot be modified.");
        } else {
            throw std::runtime_error("CuCsv: RowIdx out of bound.");
        }
    }

    return newCsv;
}

CuCsv::Csv CuCsv::InsertCsvRow(const Csv &csv, const size_t &rowIdx, const CsvRow &newRow)
{
    if (newRow.size() == 0) {
        throw std::runtime_error("CuCsv: NewRow is empty.");
    }

    Csv newCsv = csv;
    for (auto &[columnName, column] : newCsv) {
        if (rowIdx < column.size() && rowIdx > 0) {
            if (newRow.count(columnName) == 0) {
                column.insert(column.begin() + rowIdx, "");
            } else {
                column.insert(column.begin() + rowIdx, newRow.at(columnName));
            }
        } else if (rowIdx == 0) {
            throw std::runtime_error("CuCsv: ColumnName cannot be modified.");
        } else {
            throw std::runtime_error("CuCsv: RowIdx out of bound.");
        }
    }

    return newCsv;
}

CuCsv::Csv CuCsv::AppendCsvRow(const Csv &csv, const CsvRow &newRow)
{
    if (newRow.size() == 0) {
        throw std::runtime_error("CuCsv: NewRow is empty.");
    }

    Csv newCsv = csv;
    for (auto &[columnName, column] : newCsv) {
        if (newRow.count(columnName) == 0) {
            column.emplace_back("");
        } else {
            column.emplace_back(newRow.at(columnName));
        }
    }

    return newCsv;
}

CuCsv::Csv CuCsv::DeleteCsvRow(const Csv &csv, const size_t &rowIdx)
{
    Csv newCsv = csv;
    for (auto &[columnName, column] : newCsv) {
        if (rowIdx < column.size() && rowIdx > 0) {
            column.erase(column.begin() + rowIdx);
        } else if (rowIdx == 0) {
            throw std::runtime_error("CuCsv: ColumnName cannot be deleted.");
        } else {
            throw std::runtime_error("CuCsv: RowIdx out of bound.");
        }
    }

    return newCsv;
}

CuCsv::Csv CuCsv::SetCsvCell(const Csv &csv, const std::string &columnName, const size_t &rowIdx, const std::string &newCell)
{
    Csv newCsv = csv;
    if (newCsv.count(columnName) == 1) {
        CsvColumn &column = newCsv[columnName];
        if (rowIdx < column.size() && rowIdx > 0) {
            column[rowIdx] = newCell;
        } else if (rowIdx == 0) {
            throw std::runtime_error("CuCsv: ColumnName cannot be modified.");
        } else {
            throw std::runtime_error("CuCsv: RowIdx out of bound.");
        }
    } else {
        throw std::runtime_error("CuCsv: Column does not exist.");
    }

    return newCsv;
}

std::vector<std::string> CuCsv::StrSplit(const std::string &str, const std::string &delimiter)
{
    std::vector<std::string> strList{};

    size_t start_pos = 0;
    size_t pos = str.find(delimiter);
    while (pos != std::string::npos) {
        strList.emplace_back(str.substr(start_pos, pos - start_pos));
        start_pos = pos + delimiter.size();
        pos = str.find(delimiter, start_pos);
    }
    if (start_pos < str.size()) {
        strList.emplace_back(str.substr(start_pos));
    }

    return strList;
}

std::string CuCsv::FormatString(const std::string &str)
{
    std::string fstring = "";
    for (const char &c : str) {
        if (c != ' ' && c != '\"' && c != '\r') {
            fstring += c;
        }
    }

    return fstring;
}
