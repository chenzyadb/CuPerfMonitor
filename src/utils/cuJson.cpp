#include "cuJson.h"

cuJson::cuJson() { }
cuJson::~cuJson() { }

cuJson::Json cuJson::ParseJson(const std::string &text)
{
	Json json = jsonList.size();
	jsonList.emplace_back(FormatString(text));

	return json;
}

bool cuJson::IsItemExist(const Json &json, const std::string &itemName)
{
	if (json >= jsonList.size()) {
		throw std::runtime_error("cuJson: Json does not exist.");
	}

	bool itemExist = true;
	if (GetNotNestedItem(jsonList[json], itemName).empty()) {
		itemExist = false;
	}

	return itemExist;
}

std::string cuJson::GetItemString(const Json &json, const std::string &itemName)
{
	if (json >= jsonList.size()) {
		throw std::runtime_error("cuJson: Json does not exist.");
	}

	std::string item = GetNotNestedItem(jsonList[json], itemName);
	if (!item.empty()) {
		item = GetPrevString(GetPostString(item, ':'), ',');
	} else {
		throw std::runtime_error("cuJson: Item does not exist.");
	}

	std::string itemString = GetPrevString(GetPostString(item, '\"'), '\"');

	return itemString;
}

int cuJson::GetItemInt(const Json &json, const std::string &itemName)
{
	if (json >= jsonList.size()) {
		throw std::runtime_error("cuJson: Json does not exist.");
	}

	std::string item = GetNotNestedItem(jsonList[json], itemName);
	if (!item.empty()) {
		item = GetPrevString(GetPostString(item, ':'), ',');
	} else {
		throw std::runtime_error("cuJson: Item does not exist.");
	}

	int itemInt = std::stoi(item);

	return itemInt;
}

double cuJson::GetItemDouble(const Json &json, const std::string &itemName)
{
	if (json >= jsonList.size()) {
		throw std::runtime_error("cuJson: Json does not exist.");
	}

	std::string item = GetNotNestedItem(jsonList[json], itemName);
	if (!item.empty()) {
		item = GetPrevString(GetPostString(item, ':'), ',');
	} else {
		throw std::runtime_error("cuJson: Item does not exist.");
	}

	double itemDouble = std::stof(item);

	return itemDouble;
}

bool cuJson::GetItemBool(const Json &json, const std::string &itemName)
{
	if (json >= jsonList.size()) {
		throw std::runtime_error("cuJson: Json does not exist.");
	}

	std::string item = GetNotNestedItem(jsonList[json], itemName);
	if (!item.empty()) {
		item = GetPrevString(GetPostString(item, ':'), ',');
	} else {
		throw std::runtime_error("cuJson: Item does not exist.");
	}

	bool itemBool = false;
	if (item == "true") {
		itemBool = true;
	}

	return itemBool;
}

cuJson::Json cuJson::GetItemJson(const Json &json, const std::string &itemName)
{
	if (json >= jsonList.size()) {
		throw std::runtime_error("cuJson: Json does not exist.");
	}

	std::string itemJsonText = "";
	std::string item = GetNotNestedItem(jsonList[json], itemName);
	if (!item.empty()) {
		itemJsonText = GetSingleJson(GetPostString(item, ':'));
	} else {
		throw std::runtime_error("cuJson: Item does not exist.");
	}

	Json itemJson = ParseJson(itemJsonText);

	return itemJson;
}

cuJson::ArrayString cuJson::GetArrayString(const Json &json, const std::string &itemName)
{
	if (json >= jsonList.size()) {
		throw std::runtime_error("cuJson: Json does not exist.");
	}

	std::string arrayText = "";
	std::string item = GetNotNestedItem(jsonList[json], itemName);
	if (!item.empty()) {
		arrayText = GetSingleArray(GetPostString(item, ':'));
	} else {
		throw std::runtime_error("cuJson: Array does not exist.");
	}

	ArrayString array;
	std::vector<std::string> genericArray = GetGenericArray(arrayText);
	for (const std::string &genericItem : genericArray) {
		array.emplace_back(GetPrevString(GetPostString(genericItem, '\"'), '\"'));
	}
	
	return array;
}

cuJson::ArrayInt cuJson::GetArrayInt(const Json &json, const std::string &itemName)
{
	if (json >= jsonList.size()) {
		throw std::runtime_error("cuJson: Json does not exist.");
	}

	std::string arrayText = "";
	std::string item = GetNotNestedItem(jsonList[json], itemName);
	if (!item.empty()) {
		arrayText = GetSingleArray(GetPostString(item, ':'));
	} else {
		throw std::runtime_error("cuJson: Array does not exist.");
	}

	ArrayInt array;
	std::vector<std::string> genericArray = GetGenericArray(arrayText);
	for (const std::string &genericItem : genericArray) {
		array.emplace_back(std::stoi(genericItem));
	}

	return array;
}

cuJson::ArrayDouble cuJson::GetArrayDouble(const Json &json, const std::string &itemName)
{
	if (json >= jsonList.size()) {
		throw std::runtime_error("cuJson: Json does not exist.");
	}

	std::string arrayText = "";
	std::string item = GetNotNestedItem(jsonList[json], itemName);
	if (!item.empty()) {
		arrayText = GetSingleArray(GetPostString(item, ':'));
	} else {
		throw std::runtime_error("cuJson: Array does not exist.");
	}

	ArrayDouble array;
	std::vector<std::string> genericArray = GetGenericArray(arrayText);
	for (const std::string &genericItem : genericArray) {
		array.emplace_back(std::stof(genericItem));
	}

	return array;
}

cuJson::ArrayBool cuJson::GetArrayBool(const Json &json, const std::string &itemName)
{
	if (json >= jsonList.size()) {
		throw std::runtime_error("cuJson: Json does not exist.");
	}

	std::string arrayText = "";
	std::string item = GetNotNestedItem(jsonList[json], itemName);
	if (!item.empty()) {
		arrayText = GetSingleArray(GetPostString(item, ':'));
	} else {
		throw std::runtime_error("cuJson: Array does not exist.");
	}

	ArrayBool array;
	std::vector<std::string> genericArray = GetGenericArray(arrayText);
	for (const std::string &genericItem : genericArray) {
		if (genericItem == "true") {
			array.emplace_back(true);
		} else {
			array.emplace_back(false);
		}
	}

	return array;
}

std::string cuJson::PrintJson(const Json &json)
{
	if (json >= jsonList.size()) {
		throw std::runtime_error("cuJson: Json does not exist.");
	}

	std::string jsonText = jsonList[json];

	return jsonText;
}

std::string cuJson::FormatString(const std::string &str)
{
	std::string fstring = "";
	for (const char &c : str) {
		if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
			fstring += c;
		}
	}

	return fstring;
}

std::string cuJson::GetPrevString(const std::string &str, const char &chr)
{
	std::string prevString = str;

	size_t pos = prevString.find(chr);
	if (pos != std::string::npos) {
		prevString = prevString.substr(0, pos);
	}

	return prevString;
}

std::string cuJson::GetRePrevString(const std::string &str, const char &chr)
{
	std::string prevString = str;

	size_t pos = prevString.rfind(chr);
	if (pos != std::string::npos) {
		prevString = prevString.substr(0, pos);
	}

	return prevString;
}

std::string cuJson::GetPostString(const std::string &str, const char &chr)
{
	std::string postString = str;

	size_t pos = postString.find(chr);
	if (pos != std::string::npos) {
		postString = postString.substr(pos + 1);
	}

	return postString;
}

std::string cuJson::GetRePostString(const std::string &str, const char &chr)
{
	std::string postString = str;

	size_t pos = postString.rfind(chr);
	if (pos != std::string::npos) {
		postString = postString.substr(pos + 1);
	}

	return postString;
}

std::string cuJson::GetSingleJson(const std::string &str)
{
	std::string jsonText = "";

	int braceNum = 0;
	for (const char &c : str) {
		jsonText += c;
		if (c == '{') {
			braceNum++;
		} else if (c == '}') {
			braceNum--;
			if (braceNum == 0) {
				break;
			}
		}
	}

	return jsonText;
}

std::string cuJson::GetSingleArray(const std::string &str)
{
	std::string arrayText = "";

	int bracketNum = 0;
	for (const char &c : str) {
		arrayText += c;
		if (c == '[') {
			bracketNum++;
		} else if (c == ']') {
			bracketNum--;
			if (bracketNum == 0) {
				break;
			}
		}
	}

	return arrayText;
}

bool cuJson::IsInNestJson(const std::string &str)
{
	int braceNum = 0;
	for (const char &c : str) {
		if (c == '{') {
			braceNum++;
		} else if (c == '}') {
			braceNum--;
		}
	}

	bool inNestJson = false;
	if (braceNum < 0) {
		inNestJson = true;
	}

	return inNestJson;
}

std::string cuJson::GetNotNestedItem(const std::string &str, const std::string &itemName)
{
	std::string item = GetRePrevString(GetPostString(str, '{'), '}');

	std::string jsonItemName = "\"" + itemName + "\"";
	size_t pos = item.find(jsonItemName);
	if (pos == std::string::npos) {
		item = "";
	}
	while (pos != std::string::npos) {
		if (IsInNestJson(item.substr(pos))) {
			item = item.substr(pos + jsonItemName.length());
			pos = item.find(jsonItemName);
			if (pos == std::string::npos) {
				item = "";
				break;
			}
		} else {
			if (pos != std::string::npos) {
				item = item.substr(pos);
			} else {
				item = "";
			}
			break;
		}
	}

	return item;
}

std::vector<std::string> cuJson::GetGenericArray(const std::string &str)
{
	std::string arrayText = GetPrevString(GetPostString(str, '['), ']');
	std::vector<std::string> array;

	size_t start_pos = 0;
	for (size_t pos = 0; pos < arrayText.size(); pos++) {
		if (arrayText.at(pos) == ',') {
			std::string item = arrayText.substr(start_pos, pos - start_pos);
			array.emplace_back(item);
			start_pos = pos + 1;
		}
	}
	if (start_pos < arrayText.size()) {
		std::string item = arrayText.substr(start_pos);
		array.emplace_back(item);
	}

	return array;
}
