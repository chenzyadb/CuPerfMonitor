#include "JsonObject.h"

JsonObject::JsonObject() : jsonMap_(), jsonOrder_() { }

JsonObject::JsonObject(const std::string &jsonText) : jsonMap_(), jsonOrder_()
{
	bool atJson = false;
	bool atKey = false;
	bool atValue = false;
	bool atString = false;
	bool atItemJson = false;
	bool atArray = false;
	int bracesNum = 0;
	int bracketsNum = 0;
	std::string key = "";
	std::string value = "";
	for (const char &c : jsonText) {
		switch (c) {
			case '{':
				if (atJson) {
					if (atValue) {
						if (atArray) {
							value += c;
						} else if (atString) {
							value += c;
						} else if (atItemJson) {
							value += c;
							bracesNum++;
						} else if (!atItemJson) {
							value += c;
							atItemJson = true;
							bracesNum++;
						}
					} else {
						throw std::runtime_error("Invalid JSON structure.");
					}
				} else {
					atJson = true;
				}
				break;
			case '}':
				if (atValue) {
					if (atArray) {
						value += c;
					} else if (atString) {
						value += c;
					} else if (atItemJson) {
						value += c;
						bracesNum--;
						if (bracesNum == 0) {
							atItemJson = false;
						}
					} else {
						atValue = false;
						atJson = false;
					}
				} else {
					throw std::runtime_error("Invalid JSON structure.");
				}
				break;
			case '\"':
				if (atJson) {
					if (atValue) {
						if (atArray) {
							value += c;
						} else if (atItemJson) {
							value += c;
						} else if (atString) {
							value += c;
							atString = false;
						} else {
							value += c;
							atString = true;
						}
					} else if (atKey) {
						atKey = false;
					} else {
						atKey = true;
					}
				} else {
					throw std::runtime_error("Invalid JSON structure.");
				}
				break;
			case ':':
				if (atJson) {
					if (!atKey && !atValue) {
						atValue = true;
					} else if (atValue) {
						if (atArray) {
							value += c;
						} else if (atItemJson) {
							value += c;
						} else if (atString) {
							value += c;
						} else {
							throw std::runtime_error("Invalid JSON structure.");
						}
					}
				} else {
					throw std::runtime_error("Invalid JSON structure.");
				}
				break;
			case ',':
				if (atJson) {
					if (atValue) {
						if (atArray) {
							value += c;
						} else if (atItemJson) {
							value += c;
						} else if (atString) {
							value += c;
						} else {
							atValue = false;
						}
					} else {
						throw std::runtime_error("Invalid JSON structure.");
					}
				} else {
					throw std::runtime_error("Invalid JSON structure.");
				}
				break;
			case '[':
				if (atJson) {
					if (atValue) {
						if (atArray) {
							value += c;
							bracketsNum++;
						} else if (atItemJson) {
							value += c;
						} else if (atString) {
							value += c;
						} else {
							value += c;
							atArray = true;
							bracketsNum++;
						}
					}
				} else {
					throw std::runtime_error("Invalid JSON structure.");
				}
				break;
			case ']':
				if (atJson) {
					if (atValue) {
						if (atArray) {
							value += c;
							bracketsNum--;
							if (bracketsNum == 0) {
								atArray = false;
							}
						} else if (atItemJson) {
							value += c;
						} else if (atString) {
							value += c;
						} else {
							throw std::runtime_error("Invalid JSON structure.");
						}
					} else {
						throw std::runtime_error("Invalid JSON structure.");
					}
				} else {
					throw std::runtime_error("Invalid JSON structure.");
				}
				break;
			case ' ':
				break;
			case '\n':
				break;
			case '\t':
				break;
			case '\r':
				break;
			default:
				if (atKey) {
					key += c;
				} else if (atValue) {
					value += c;
				} else {
					throw std::runtime_error("Invalid JSON structure.");
				}
				break;
		}
		if (!atKey && !atValue && !key.empty() && !value.empty()) {
			PutValue_(key, value);
			key = "";
			value = "";
		}
	}
	if (atKey || atValue) {
		throw std::runtime_error("Invalid JSON structure.");
	}
}

JsonObject::JsonObject(const JsonObject &other) : jsonMap_(), jsonOrder_()
{
	if (this != &other) {
		jsonMap_ = other._Get_JsonMap();
		jsonOrder_ = other._Get_JsonOrder();
	}
}

JsonObject::~JsonObject() { }

std::unordered_map<std::string, std::string> JsonObject::_Get_JsonMap() const
{
	return jsonMap_;
}

std::vector<std::string> JsonObject::_Get_JsonOrder() const
{
	return jsonOrder_;
}

JsonObject &JsonObject::operator=(const JsonObject &other)
{
	if (this != &other) {
		jsonMap_ = other._Get_JsonMap();
		jsonOrder_ = other._Get_JsonOrder();
	}

	return *this;
}

JsonObject &JsonObject::operator+=(const JsonObject &other)
{
	std::unordered_map<std::string, std::string> otherJsonMap = other._Get_JsonMap();
	std::vector<std::string> otherJsonOrder = other._Get_JsonOrder();
	for (const std::string &key : otherJsonOrder) {
		if (jsonMap_.count(key) == 0) {
			jsonMap_[key] = otherJsonMap.at(key);
			jsonOrder_.emplace_back(key);
		} else {
			jsonMap_[key] = otherJsonMap.at(key);
		}
	}

	return *this;
}

bool JsonObject::operator==(const JsonObject &other) const
{
	bool equals = false;
	if (jsonMap_ == other._Get_JsonMap() && jsonOrder_ == other._Get_JsonOrder()) {
		equals = true;
	}

	return equals;
}

bool JsonObject::operator!=(const JsonObject &other) const
{
	bool unequal = false;
	if (jsonMap_ != other._Get_JsonMap() || jsonOrder_ != other._Get_JsonOrder()) {
		unequal = true;
	}

	return unequal;
}

std::string JsonObject::GetValueString(const std::string &key) const
{
	const std::string &value = GetValue_(key);
	size_t first_pos = value.find("\"");
	size_t last_pos = value.rfind("\"");
	if (first_pos == std::string::npos || last_pos == std::string::npos) {
		throw std::runtime_error("Failed to get ValueString.");
	}
	std::string valueString = value.substr(first_pos + 1, last_pos - first_pos - 1);

	return valueString;
}

int JsonObject::GetValueInt(const std::string &key) const
{
	const std::string &value = GetValue_(key);
	int valueInt = std::stoi(value);

	return valueInt;
}

long JsonObject::GetValueLong(const std::string &key) const
{
	const std::string &value = GetValue_(key);
	long valueLong = std::stol(value);

	return valueLong;
}

double JsonObject::GetValueDouble(const std::string &key) const
{
	const std::string &value = GetValue_(key);
	double valueDouble = std::stod(value);

	return valueDouble;
}

bool JsonObject::GetValueBoolean(const std::string &key) const
{
	const std::string &value = GetValue_(key);
	bool valueBoolean = false;
	if (value == "true") {
		valueBoolean = true;
	}

	return valueBoolean;
}

JsonObject JsonObject::GetValueJson(const std::string &key) const
{
	const std::string &value = GetValue_(key);
	JsonObject valueJson = JsonObject(value);

	return valueJson;
}

std::vector<std::string> JsonObject::GetArrayString(const std::string &key) const
{
	std::vector<std::string> arrayString{};
	const std::vector<std::string> &array = GetArray_(key);
	for (const std::string &value : array) {
		size_t first_pos = value.find("\"");
		size_t last_pos = value.rfind("\"");
		if (first_pos == std::string::npos || last_pos == std::string::npos) {
			throw std::runtime_error("Failed to get ValueString.");
		}
		std::string valueString = value.substr(first_pos + 1, last_pos - first_pos - 1);
		arrayString.emplace_back(valueString);
	}

	return arrayString;
}

std::vector<int> JsonObject::GetArrayInt(const std::string &key) const
{
	std::vector<int> arrayInt{};
	const std::vector<std::string> &array = GetArray_(key);
	for (const std::string &value : array) {
		int valueInt = std::stoi(value);
		arrayInt.emplace_back(valueInt);
	}

	return arrayInt;
}

std::vector<long> JsonObject::GetArrayLong(const std::string &key) const
{
	std::vector<long> arrayLong{};
	const std::vector<std::string> &array = GetArray_(key);
	for (const std::string &value : array) {
		int valueInt = std::stol(value);
		arrayLong.emplace_back(valueInt);
	}

	return arrayLong;
}

std::vector<double> JsonObject::GetArrayDouble(const std::string &key) const
{
	std::vector<double> arrayDouble{};
	const std::vector<std::string> &array = GetArray_(key);
	for (const std::string &value : array) {
		double valueDouble = std::stod(value);
		arrayDouble.emplace_back(valueDouble);
	}

	return arrayDouble;
}

std::vector<bool> JsonObject::GetArrayBoolean(const std::string &key) const
{
	std::vector<bool> arrayBoolean{};
	const std::vector<std::string> &array = GetArray_(key);
	for (const std::string &value : array) {
		bool valueBoolean = false;
		if (value == "true") {
			valueBoolean = true;
		}
		arrayBoolean.emplace_back(valueBoolean);
	}

	return arrayBoolean;
}

std::vector<JsonObject> JsonObject::GetArrayJson(const std::string &key) const
{
	std::vector<JsonObject> arrayJson{};
	const std::vector<std::string> &array = GetArray_(key);
	for (const std::string &value : array) {
		JsonObject valueJson = JsonObject(value);
		arrayJson.emplace_back(valueJson);
	}

	return arrayJson;
}

void JsonObject::PutValueString(const std::string &key, const std::string &value)
{
	std::string valueString = "\"" + value + "\"";
	PutValue_(key, valueString);
}

void JsonObject::PutValueInt(const std::string &key, const int &value)
{
	std::string valueInt = std::to_string(value);
	PutValue_(key, valueInt);
}

void JsonObject::PutValueLong(const std::string &key, const long &value)
{
	std::string valueLong = std::to_string(value);
	PutValue_(key, valueLong);
}

void JsonObject::PutValueDouble(const std::string &key, const double &value)
{
	std::string valueDouble = std::to_string(value);
	PutValue_(key, valueDouble);
}

void JsonObject::PutValueBoolean(const std::string &key, const bool &value)
{
	std::string valueBoolean = "false";
	if (value) {
		valueBoolean = "true";
	}
	PutValue_(key, valueBoolean);
}

void JsonObject::PutValueJson(const std::string &key, const JsonObject &value)
{
	std::string valueJson = value.PrintToString(false);
	PutValue_(key, valueJson);
}

void JsonObject::PutArrayString(const std::string &key, const std::vector<std::string> &array)
{
	std::string arrayString = "[";
	for (const std::string &value : array) {
		std::string valueString = "\"" + value + "\"";
		arrayString += valueString + ",";
	}
	arrayString = arrayString.substr(0, arrayString.rfind(","));
	arrayString += "]";
	PutValue_(key, arrayString);
}

void JsonObject::PutArrayInt(const std::string &key, const std::vector<int> &array)
{
	std::string arrayInt = "[";
	for (const int &value : array) {
		std::string valueInt = std::to_string(value);
		arrayInt += valueInt + ",";
	}
	arrayInt = arrayInt.substr(0, arrayInt.rfind(","));
	arrayInt += "]";
	PutValue_(key, arrayInt);
}

void JsonObject::PutArrayLong(const std::string &key, const std::vector<long> &array)
{
	std::string arrayLong = "[";
	for (const int &value : array) {
		std::string valueLong = std::to_string(value);
		arrayLong += valueLong + ",";
	}
	arrayLong = arrayLong.substr(0, arrayLong.rfind(","));
	arrayLong += "]";
	PutValue_(key, arrayLong);
}

void JsonObject::PutArrayDouble(const std::string &key, const std::vector<double> &array)
{
	std::string arrayDouble = "[";
	for (const double &value : array) {
		std::string valueDouble = std::to_string(value);
		arrayDouble += valueDouble + ",";
	}
	arrayDouble = arrayDouble.substr(0, arrayDouble.rfind(","));
	arrayDouble += "]";
	PutValue_(key, arrayDouble);
}

void JsonObject::PutArrayBoolean(const std::string &key, const std::vector<bool> &array)
{
	std::string arrayBoolean = "[";
	for (const bool &value : array) {
		std::string valueBoolean = "false";
		if (value) {
			valueBoolean = "true";
		}
		arrayBoolean += valueBoolean + ",";
	}
	arrayBoolean = arrayBoolean.substr(0, arrayBoolean.rfind(","));
	arrayBoolean += "]";
	PutValue_(key, arrayBoolean);
}

void JsonObject::PutArrayJson(const std::string &key, const std::vector<JsonObject> &array)
{
	std::string arrayJson = "[";
	for (const JsonObject &value : array) {
		arrayJson += value.PrintToString(false) + ",";
	}
	arrayJson = arrayJson.substr(0, arrayJson.rfind(","));
	arrayJson += "]";
	PutValue_(key, arrayJson);
}

void JsonObject::Clear()
{
	jsonMap_.clear();
	jsonOrder_.clear();
}

void JsonObject::Remove(const std::string &key)
{
	{
		const auto &iter = jsonMap_.find(key);
		if (iter != jsonMap_.end()) {
			jsonMap_.erase(iter);
		}
	}
	{
		const auto &iter = std::find(jsonOrder_.begin(), jsonOrder_.end(), key);
		if (iter != jsonOrder_.end()) {
			jsonOrder_.erase(iter);
		}
	}
}

bool JsonObject::Contains(const std::string &key) const
{
	bool contains = false;
	if (jsonMap_.count(key) == 1) {
		contains = true;
	}

	return contains;
}

std::string JsonObject::PrintToString(const bool &format) const
{
	std::string jsonText = "";
	if (format) {
		jsonText += "{";
		for (const std::string &key : jsonOrder_) {
			std::string value = GetValue_(key);
			jsonText += "\n  \"" + key + "\": " + value + ",";
		}
		jsonText = jsonText.substr(0, jsonText.rfind(","));
		jsonText += "\n}";
	} else {
		jsonText += "{";
		for (const std::string &key : jsonOrder_) {
			std::string value = GetValue_(key);
			jsonText += "\"" + key + "\":" + value + ",";
		}
		jsonText = jsonText.substr(0, jsonText.rfind(","));
		jsonText += "}";
	}
	
	return jsonText;
}

std::string JsonObject::GetValue_(const std::string &key) const
{
	if (jsonMap_.count(key) != 1) {
		throw std::runtime_error("Failed to get JsonValue.");
	}

	return jsonMap_.at(key);
}

std::vector<std::string> JsonObject::GetArray_(const std::string &key) const
{
	if (jsonMap_.count(key) != 1) {
		throw std::runtime_error("Failed to get JsonArray.");
	}
	const std::string &arrayText = jsonMap_.at(key);

	std::vector<std::string> array{};
	bool atValue = false;
	bool atArray = false;
	bool atJson = false;
	bool atString = false;
	int bracesNum = 0;
	int bracketsNum = 0;
	std::string value = "";
	for (const char &c : arrayText) {
		switch (c) {
			case '[':
				if (atValue) {
					if (atArray) {
						value += c;
						bracketsNum++;
					} else if (atJson) {
						value += c;
					} else if (atString) {
						value += c;
					} else {
						value += c;
						atArray = true;
						bracketsNum++;
					}
				} else {
					atValue = true;
				}
				break;
			case ']':
				if (atValue) {
					if (atArray) {
						value += c;
						bracketsNum--;
						if (bracketsNum == 0) {
							atArray = false;
						}
					} else if (atJson) {
						value += c;
					} else if (atString) {
						value += c;
					} else {
						atValue = false;
						if (!value.empty()) {
							array.emplace_back(value);
						}
						value = "";
					}
				} else {
					throw std::runtime_error("Invalid JSONArray structure.");
				}
				break;
			case '\"':
				if (atValue) {
					if (atArray) {
						value += c;
					} else if (atJson) {
						value += c;
					} else if (atString) {
						value += c;
						atString = false;
					} else {
						value += c;
						atString = true;
					}
				} else {
					throw std::runtime_error("Invalid JSONArray structure.");
				}
				break;
			case '{':
				if (atValue) {
					if (atJson) {
						value += c;
						bracesNum++;
					} else if (atArray) {
						value += c;
					} else if (atString) {
						value += c;
					} else {
						value += c;
						atJson = true;
						bracesNum++;
					}
				} else {
					throw std::runtime_error("Invalid JSONArray structure.");
				}
				break;
			case '}':
				if (atValue) {
					if (atJson) {
						value += c;
						bracesNum--;
						if (bracesNum == 0) {
							atJson = false;
						}
					} else if (atArray) {
						value += c;
					} else if (atString) {
						value += c;
					} else {
						throw std::runtime_error("Invalid JSONArray structure.");
					}
				} else {
					throw std::runtime_error("Invalid JSONArray structure.");
				}
				break;
			case ',':
				if (atValue) {
					if (atArray) {
						value += c;
					} else if (atJson) {
						value += c;
					} else if (atString) {
						value += c;
					} else {
						array.emplace_back(value);
						value = "";
					}
				} else {
					throw std::runtime_error("Invalid JSONArray structure.");
				}
				break;
			case ' ':
				break;
			case '\n':
				break;
			case '\t':
				break;
			case '\r':
				break;
			default:
				if (atValue) {
					value += c;
				} else {
					throw std::runtime_error("Invalid JSONArray structure.");
				}
				break;
		}
		if (!atValue && !value.empty()) {
			array.emplace_back(value);
			value = "";
		}
	}
	if (atValue) {
		throw std::runtime_error("Invalid JSONArray structure.");
	}

	return array;
}

void JsonObject::PutValue_(const std::string &key, const std::string &value)
{
	jsonOrder_.emplace_back(key);
	jsonMap_[key] = value;
}
