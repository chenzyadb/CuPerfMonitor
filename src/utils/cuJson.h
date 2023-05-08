#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

class cuJson
{
	public:
		using Json = size_t;
		using ArrayString = std::vector<std::string>;
		using ArrayInt = std::vector<int>;
		using ArrayDouble = std::vector<double>;
		using ArrayBool = std::vector<bool>;

		cuJson();
		~cuJson();

		Json ParseJson(const std::string &text);
		bool IsItemExist(const Json &json, const std::string &itemName);
		std::string GetItemString(const Json &json, const std::string &itemName);
		int GetItemInt(const Json &json, const std::string &itemName);
		double GetItemDouble(const Json &json, const std::string &itemName);
		bool GetItemBool(const Json &json, const std::string &itemName);
		Json GetItemJson(const Json &json, const std::string &itemName);
		ArrayString GetArrayString(const Json &json, const std::string &itemName);
		ArrayInt GetArrayInt(const Json &json, const std::string &itemName);
		ArrayDouble GetArrayDouble(const Json &json, const std::string &itemName);
		ArrayBool GetArrayBool(const Json &json, const std::string &itemName);
		std::string PrintJson(const Json &json);

	private:
		std::vector<std::string> jsonList;

		std::string FormatString(const std::string &str);
		std::string GetPrevString(const std::string &str, const char &chr);
		std::string GetRePrevString(const std::string &str, const char &chr);
		std::string GetPostString(const std::string &str, const char &chr);
		std::string GetRePostString(const std::string &str, const char &chr);
		std::string GetSingleJson(const std::string &str);
		std::string GetSingleArray(const std::string &str);
		bool IsInNestJson(const std::string &str);
		std::string GetNotNestedItem(const std::string &str, const std::string &itemName);
		std::vector<std::string> GetGenericArray(const std::string &str);
};
