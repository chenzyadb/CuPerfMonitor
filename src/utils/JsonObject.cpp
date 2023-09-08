#include "JsonObject.h"

JsonItem::JsonItem() : 
	valueRaw_(""),
	valueDouble_(0.0), 
	valueInt_(0), 
	valueLong_(0), 
	valueString_(""),
	valueBoolean_(false), 
	valueArray_(), 
	valueJson_(nullptr) { }

JsonItem::JsonItem(const JsonItem &other) : 
	valueRaw_(""),
	valueDouble_(0.0),
	valueInt_(0),
	valueLong_(0),
	valueString_(""),
	valueBoolean_(false),
	valueArray_(),
	valueJson_(nullptr)
{
	if (this != &other) {
		valueRaw_ = other.ValueRaw();
		valueDouble_ = other.ValueDouble();
		valueInt_ = other.ValueInt();
		valueLong_ = other.ValueLong();
		valueString_ = other.ValueString();
		valueBoolean_ = other.ValueBoolean();
		valueArray_ = other.ValueArray();
		valueJson_ = new JsonObject(other.ValueJson());
	}
}

JsonItem::JsonItem(const std::string &raw) :
	valueRaw_(raw),
	valueDouble_(0.0),
	valueInt_(0),
	valueLong_(0),
	valueString_(""),
	valueBoolean_(false),
	valueArray_(),
	valueJson_(nullptr)
{
	constexpr int TYPE_NUM = 0, TYPE_JSON = 1, TYPE_ARRAY = 2, TYPE_STRING = 3;
	const auto getValueType = [](const std::string &raw) -> int {
		int type = TYPE_NUM;
		switch (raw[0]) {
			case '{':
				type = TYPE_JSON;
				break;
			case '[':
				type = TYPE_ARRAY;
				break;
			case '\"':
				type = TYPE_STRING;
				break;
			default:
				type = TYPE_NUM;
				break;
		}
		return type;
	};
	const auto getDecimal = [](int decimalPlaces) -> double {
		double dec = 1.0;
		for (int i = 0; i < decimalPlaces; i++) {
			dec *= 0.1;
		}
		return dec;
	};
	const auto rawToDouble = [&](const std::string &raw) -> double {
		double valueDouble = 0.0;
		bool atDecimal = false;
		int decimalPlaces = 0;
		for (const auto &c : raw) {
			switch (c) {
				case '0':
					if (!atDecimal) {
						valueDouble = valueDouble * 10;
					}
					break;
				case '1':
					if (!atDecimal) {
						valueDouble = valueDouble * 10 + 1;
					} else {
						valueDouble = valueDouble + 1 * getDecimal(decimalPlaces);
					}
					break;
				case '2':
					if (!atDecimal) {
						valueDouble = valueDouble * 10 + 2;
					} else {
						valueDouble = valueDouble + 2 * getDecimal(decimalPlaces);
					}
					break;
				case '3':
					if (!atDecimal) {
						valueDouble = valueDouble * 10 + 3;
					} else {
						valueDouble = valueDouble + 3 * getDecimal(decimalPlaces);
					}
					break;
				case '4':
					if (!atDecimal) {
						valueDouble = valueDouble * 10 + 4;
					} else {
						valueDouble = valueDouble + 4 * getDecimal(decimalPlaces);
					}
					break;
				case '5':
					if (!atDecimal) {
						valueDouble = valueDouble * 10 + 5;
					} else {
						valueDouble = valueDouble + 5 * getDecimal(decimalPlaces);
					}
					break;
				case '6':
					if (!atDecimal) {
						valueDouble = valueDouble * 10 + 6;
					} else {
						valueDouble = valueDouble + 6 * getDecimal(decimalPlaces);
					}
					break;
				case '7':
					if (!atDecimal) {
						valueDouble = valueDouble * 10 + 7;
					} else {
						valueDouble = valueDouble + 7 * getDecimal(decimalPlaces);
					}
					break;
				case '8':
					if (!atDecimal) {
						valueDouble = valueDouble * 10 + 8;
					} else {
						valueDouble = valueDouble + 8 * getDecimal(decimalPlaces);
					}
					break;
				case '9':
					if (!atDecimal) {
						valueDouble = valueDouble * 10 + 9;
					} else {
						valueDouble = valueDouble + 9 * getDecimal(decimalPlaces);
					}
					break;
				case '.':
					atDecimal = true;
					break;
				default:
					break;
			}
			if (atDecimal) {
				decimalPlaces++;
			}
		}
		if (raw[0] == '-') {
			valueDouble *= -1;
		}
		return valueDouble;
	};
	const auto rawToString = [](const std::string &raw) -> std::string {
		size_t end_pos = raw.rfind("\"");
		if (end_pos == 0 || end_pos == std::string::npos) {
			return std::string();
		}
		return raw.substr(1, end_pos - 1);
	};
	const auto rawToBoolean = [](const std::string &raw) -> bool {
		return (raw == "true");
	};

	int type = getValueType(valueRaw_);
	if (type == TYPE_NUM) {
		valueDouble_ = rawToDouble(valueRaw_);
		valueInt_ = static_cast<int>(valueDouble_);
		valueLong_ = static_cast<int64_t>(valueDouble_);
		valueBoolean_ = rawToBoolean(valueRaw_);
	} else if (type == TYPE_JSON) {
		valueJson_ = new JsonObject(valueRaw_);
	} else if (type == TYPE_ARRAY) {
		valueArray_ = ParseArray_(valueRaw_);
	} else if (type == TYPE_STRING) {
		valueString_ = rawToString(valueRaw_);
	}
}

JsonItem::~JsonItem()
{
	if (valueJson_ != nullptr) {
		delete valueJson_;
	}
}

JsonItem &JsonItem::operator=(const JsonItem &other)
{
	if (this != &other) {
		valueRaw_ = other.ValueRaw();
		valueDouble_ = other.ValueDouble();
		valueInt_ = other.ValueInt();
		valueLong_ = other.ValueLong();
		valueString_ = other.ValueString();
		valueBoolean_ = other.ValueBoolean();
		valueArray_ = other.ValueArray();
		valueJson_ = new JsonObject(other.ValueJson());
	}

	return *this;
}

bool JsonItem::operator==(const JsonItem &other) const
{
	return (valueRaw_ == other.ValueRaw());
}

bool JsonItem::operator!=(const JsonItem &other) const
{
	return (valueRaw_ != other.ValueRaw());
}

std::string JsonItem::ValueRaw() const
{
	return valueRaw_;
}

double JsonItem::ValueDouble() const
{
	return valueDouble_;
}

int JsonItem::ValueInt() const
{
	return valueInt_;
}

int64_t JsonItem::ValueLong() const
{
	return valueLong_;
}

std::string JsonItem::ValueString() const
{
	return valueString_;
}

bool JsonItem::ValueBoolean() const
{
	return valueBoolean_;
}

std::vector<JsonItem> JsonItem::ValueArray() const
{
	return valueArray_;
}

JsonObject JsonItem::ValueJson() const
{
	JsonObject json{};
	if (valueJson_ != nullptr) {
		json = *valueJson_;
	}

	return json;
}

std::vector<JsonItem> JsonItem::ParseArray_(const std::string &valueRaw)
{
	const auto getArrayLenth = [&](size_t start_pos) -> size_t {
		size_t lenth = 1, bracketsNum = 1;
		while (bracketsNum > 0) {
			if (valueRaw[start_pos + lenth] == '[') {
				bracketsNum++;
			} else if (valueRaw[start_pos + lenth] == ']') {
				bracketsNum--;
			} else if (valueRaw[start_pos + lenth] == '\0') {
				throw std::runtime_error("Invalid JSON structure.");
			}
			lenth++;
		}
		return lenth;
	};
	const auto getJsonLenth = [&](size_t start_pos) -> size_t {
		size_t lenth = 1, bracesNum = 1;
		while (bracesNum > 0) {
			if (valueRaw[start_pos + lenth] == '{') {
				bracesNum++;
			} else if (valueRaw[start_pos + lenth] == '}') {
				bracesNum--;
			} else if (valueRaw[start_pos + lenth] == '\0') {
				throw std::runtime_error("Invalid JSON structure.");
			}
			lenth++;
		}
		return lenth;
	};
	const auto getStringLenth = [&](size_t start_pos) -> size_t {
		size_t end_pos = valueRaw.find('\"', start_pos + 1);
		if (end_pos == std::string::npos) {
			throw std::runtime_error("Invalid JSON structure.");
		}
		return end_pos - start_pos + 1;
	};
	const auto getLenth = [&](size_t start_pos) -> size_t {
		size_t end_pos = valueRaw.find(',', start_pos);
		if (end_pos == std::string::npos) {
			end_pos = valueRaw.find(']', start_pos);
		}
		return end_pos - start_pos;
	};

	std::vector<std::string> arrayRaw{};
	size_t pos = 1, end_pos = valueRaw.rfind(']');
	if (end_pos == std::string::npos) {
		throw std::runtime_error("Invalid JSON structure.");
	}
	while (pos < end_pos) {
		const auto &ch = valueRaw[pos];
		switch (ch) {
			case ',':
				pos++;
				break;
			case '\"':
				{
					auto lenth = getStringLenth(pos);
					arrayRaw.emplace_back(valueRaw.substr(pos, lenth));
					pos += lenth;
				}
				break;
			case '[':
				{
					auto lenth = getArrayLenth(pos);
					arrayRaw.emplace_back(valueRaw.substr(pos, lenth));
					pos += lenth;
				}
				break;
			case '{':
				{
					auto lenth = getJsonLenth(pos);
					arrayRaw.emplace_back(valueRaw.substr(pos, lenth));
					pos += lenth;
				}
				break;
			default:
				{
					auto lenth = getLenth(pos);
					arrayRaw.emplace_back(valueRaw.substr(pos, lenth));
					pos += lenth;
				}
				break;
		}
	}

	std::vector<JsonItem> array{};
	for (const auto &raw : arrayRaw) {
		array.emplace_back(JsonItem(raw));
	}

	return array;
}

JsonObject::JsonObject() : jsonMap_(), jsonOrder_() { }

JsonObject::JsonObject(const std::string &jsonText) : jsonMap_(), jsonOrder_()
{
	const auto getEscapeChar = [](char ch) -> char {
		char ret = ch;
		switch (ch) {
			case '\\':
				ret = '\\';
				break;
			case '\"':
				ret = '\"';
				break;
			case '\'':
				ret = '\'';
				break;
			case 'n':
				ret = '\n';
				break;
			case 'r':
				ret = '\r';
				break;
			case 't':
				ret = '\t';
				break;
			case 'b':
				ret = '\b';
				break;
			case 'f':
				ret = '\f';
				break;
			case 'a':
				ret = '\a';
				break;
			case '/':
				ret = '/';
				break;
			default:
				break;
		}
		return ret;
	};

	size_t pos = 0;
	bool atJson = false, atKey = false, atValue = false, atString = false, atItemJson = false, atArray = false;
	int bracesNum = 0, bracketsNum = 0;
	std::string key = "", value = "";
	while (pos < jsonText.size()) {
		switch (jsonText[pos]) {
			case '{':
				if (atJson) {
					if (atValue) {
						if (atArray) {
							value += jsonText[pos];
						} else if (atString) {
							value += jsonText[pos];
						} else if (atItemJson) {
							value += jsonText[pos];
							bracesNum++;
						} else if (!atItemJson) {
							value += jsonText[pos];
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
						value += jsonText[pos];
					} else if (atString) {
						value += jsonText[pos];
					} else if (atItemJson) {
						value += jsonText[pos];
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
							value += jsonText[pos];
						} else if (atItemJson) {
							value += jsonText[pos];
						} else if (atString) {
							value += jsonText[pos];
							atString = false;
						} else {
							value += jsonText[pos];
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
							value += jsonText[pos];
						} else if (atItemJson) {
							value += jsonText[pos];
						} else if (atString) {
							value += jsonText[pos];
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
							value += jsonText[pos];
						} else if (atItemJson) {
							value += jsonText[pos];
						} else if (atString) {
							value += jsonText[pos];
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
							value += jsonText[pos];
							bracketsNum++;
						} else if (atItemJson) {
							value += jsonText[pos];
						} else if (atString) {
							value += jsonText[pos];
						} else {
							value += jsonText[pos];
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
							value += jsonText[pos];
							bracketsNum--;
							if (bracketsNum == 0) {
								atArray = false;
							}
						} else if (atItemJson) {
							value += jsonText[pos];
						} else if (atString) {
							value += jsonText[pos];
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
			case '\\':
				pos++;
				if (atKey) {
					key += getEscapeChar(jsonText[pos]);
				} else if (atValue) {
					value += getEscapeChar(jsonText[pos]);
				} else {
					throw std::runtime_error("Invalid JSON structure.");
				}
				break;
			default:
				if (atKey) {
					key += jsonText[pos];
				} else if (atValue) {
					value += jsonText[pos];
				} else {
					throw std::runtime_error("Invalid JSON structure.");
				}
				break;
		}
		if (!atKey && !atValue && !key.empty() && !value.empty()) {
			jsonMap_[key] = JsonItem(value);
			jsonOrder_.emplace_back(key);
			key = "", value = "";
		}
		pos++;
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

std::unordered_map<std::string, JsonItem> JsonObject::_Get_JsonMap() const
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
	const auto &otherJsonMap = other._Get_JsonMap();
	const auto &otherJsonOrder = other._Get_JsonOrder();
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
	return (jsonOrder_ == other._Get_JsonOrder() && jsonMap_ == other._Get_JsonMap());
}

bool JsonObject::operator!=(const JsonObject &other) const
{
	return (jsonOrder_ != other._Get_JsonOrder() || jsonMap_ != other._Get_JsonMap());
}

std::string JsonObject::GetValueRaw(const std::string &key) const
{
	return GetValue_(key).ValueRaw();
}

std::string JsonObject::GetValueString(const std::string &key) const
{
	return GetValue_(key).ValueString();
}

int JsonObject::GetValueInt(const std::string &key) const
{
	return GetValue_(key).ValueInt();
}

int64_t JsonObject::GetValueLong(const std::string &key) const
{
	return GetValue_(key).ValueLong();
}

double JsonObject::GetValueDouble(const std::string &key) const
{
	return GetValue_(key).ValueDouble();
}

bool JsonObject::GetValueBoolean(const std::string &key) const
{
	return GetValue_(key).ValueBoolean();
}

JsonObject JsonObject::GetValueJson(const std::string &key) const
{
	return GetValue_(key).ValueJson();
}

JsonItem JsonObject::GetValueItem(const std::string &key) const
{
	return GetValue_(key);
}

std::vector<std::string> JsonObject::GetArrayRaw(const std::string &key) const
{
	std::vector<std::string> arrayRaw{};
	const auto &array = GetValue_(key).ValueArray();
	for (const auto &item : array) {
		arrayRaw.emplace_back(item.ValueRaw());
	}

	return arrayRaw;
}

std::vector<std::string> JsonObject::GetArrayString(const std::string &key) const
{
	std::vector<std::string> arrayString{};
	const auto &array = GetValue_(key).ValueArray();
	for (const auto &item : array) {
		arrayString.emplace_back(item.ValueString());
	}

	return arrayString;
}

std::vector<int> JsonObject::GetArrayInt(const std::string &key) const
{
	std::vector<int> arrayInt{};
	const auto &array = GetValue_(key).ValueArray();
	for (const auto &item : array) {
		arrayInt.emplace_back(item.ValueInt());
	}

	return arrayInt;
}

std::vector<int64_t> JsonObject::GetArrayLong(const std::string &key) const
{
	std::vector<int64_t> arrayLong{};
	const auto &array = GetValue_(key).ValueArray();
	for (const auto &item : array) {
		arrayLong.emplace_back(item.ValueLong());
	}

	return arrayLong;
}

std::vector<double> JsonObject::GetArrayDouble(const std::string &key) const
{
	std::vector<double> arrayDouble{};
	const auto &array = GetValue_(key).ValueArray();
	for (const auto &item : array) {
		arrayDouble.emplace_back(item.ValueDouble());
	}

	return arrayDouble;
}

std::vector<bool> JsonObject::GetArrayBoolean(const std::string &key) const
{
	std::vector<bool> arrayBoolean{};
	const auto &array = GetValue_(key).ValueArray();
	for (const auto &item : array) {
		arrayBoolean.emplace_back(item.ValueBoolean());
	}

	return arrayBoolean;
}

std::vector<JsonObject> JsonObject::GetArrayJson(const std::string &key) const
{
	std::vector<JsonObject> arrayJson{};
	const auto &array = GetValue_(key).ValueArray();
	for (const auto &item : array) {
		arrayJson.emplace_back(item.ValueJson());
	}

	return arrayJson;
}

std::vector<JsonItem> JsonObject::GetArrayItem(const std::string &key) const
{
	return GetValue_(key).ValueArray();
}

void JsonObject::PutValueRaw(const std::string &key, const std::string &value)
{
	PutValue_(key, JsonItem(value));
}

void JsonObject::PutValueString(const std::string &key, const std::string &value)
{
	std::string valueRaw = "\"" + value + "\"";
	PutValue_(key, JsonItem(valueRaw));
}

void JsonObject::PutValueInt(const std::string &key, const int &value)
{
	std::string valueRaw = std::to_string(value);
	PutValue_(key, JsonItem(valueRaw));
}

void JsonObject::PutValueLong(const std::string &key, const int64_t &value)
{
	std::string valueRaw = std::to_string(value);
	PutValue_(key, JsonItem(valueRaw));
}

void JsonObject::PutValueDouble(const std::string &key, const double &value)
{
	std::string valueRaw = std::to_string(value);
	PutValue_(key, JsonItem(valueRaw));
}

void JsonObject::PutValueBoolean(const std::string &key, const bool &value)
{
	std::string valueRaw = "false";
	if (value) {
		valueRaw = "true";
	}
	PutValue_(key, JsonItem(valueRaw));
}

void JsonObject::PutValueJson(const std::string &key, const JsonObject &value)
{
	std::string valueRaw = value.PrintToString(false);
	PutValue_(key, JsonItem(valueRaw));
}

void JsonObject::PutValueItem(const std::string &key, const JsonItem &value)
{
	PutValue_(key, value);
}

void JsonObject::PutArrayRaw(const std::string &key, const std::vector<std::string> &array)
{
	std::string valueRaw = "[";
	for (const auto &value : array) {
		valueRaw += value + ",";
	}
	valueRaw = valueRaw.substr(0, valueRaw.rfind(",")) + "]";
	PutValue_(key, JsonItem(valueRaw));
}

void JsonObject::PutArrayString(const std::string &key, const std::vector<std::string> &array)
{
	std::string valueRaw = "[";
	for (const auto &value : array) {
		valueRaw += "\"" + value + "\",";
	}
	valueRaw = valueRaw.substr(0, valueRaw.rfind(",")) + "]";
	PutValue_(key, JsonItem(valueRaw));
}

void JsonObject::PutArrayInt(const std::string &key, const std::vector<int> &array)
{
	std::string valueRaw = "[";
	for (const auto &value : array) {
		valueRaw += std::to_string(value) + ",";
	}
	valueRaw = valueRaw.substr(0, valueRaw.rfind(",")) + "]";
	PutValue_(key, JsonItem(valueRaw));
}

void JsonObject::PutArrayLong(const std::string &key, const std::vector<int64_t> &array)
{
	std::string valueRaw = "[";
	for (const auto &value : array) {
		valueRaw += std::to_string(value) + ",";
	}
	valueRaw = valueRaw.substr(0, valueRaw.rfind(",")) + "]";
	PutValue_(key, JsonItem(valueRaw));
}

void JsonObject::PutArrayDouble(const std::string &key, const std::vector<double> &array)
{
	std::string valueRaw = "[";
	for (const auto &value : array) {
		valueRaw += std::to_string(value) + ",";
	}
	valueRaw = valueRaw.substr(0, valueRaw.rfind(",")) + "]";
	PutValue_(key, JsonItem(valueRaw));
}

void JsonObject::PutArrayBoolean(const std::string &key, const std::vector<bool> &array)
{
	std::string valueRaw = "[";
	for (const auto &value : array) {
		if (value) {
			valueRaw += "true,";
		} else {
			valueRaw += "false,";
		}
	}
	valueRaw = valueRaw.substr(0, valueRaw.rfind(",")) + "]";
	PutValue_(key, JsonItem(valueRaw));
}

void JsonObject::PutArrayJson(const std::string &key, const std::vector<JsonObject> &array)
{
	std::string valueRaw = "[";
	for (const JsonObject &value : array) {
		valueRaw += value.PrintToString(false) + ",";
	}
	valueRaw = valueRaw.substr(0, valueRaw.rfind(",")) + "]";
	PutValue_(key, JsonItem(valueRaw));
}

void JsonObject::PutArrayItem(const std::string &key, const std::vector<JsonItem> &array)
{
	std::string valueRaw = "[";
	for (const auto &value : array) {
		valueRaw += value.ValueRaw() + ",";
	}
	valueRaw = valueRaw.substr(0, valueRaw.rfind(",")) + "]";
	PutValue_(key, JsonItem(valueRaw));
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
		} else {
			throw std::runtime_error("Failed to find JsonKey.");
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
	return (jsonMap_.count(key) == 1);
}

size_t JsonObject::Size() const
{
	return jsonMap_.size();
}

size_t JsonObject::GetKeyPos(const std::string &key) const
{
	const auto &iter = std::find(jsonOrder_.begin(), jsonOrder_.end(), key);
	if (iter == jsonOrder_.end()) {
		throw std::runtime_error("Failed to find JsonKey.");
	}

	return static_cast<size_t>(iter - jsonOrder_.begin());
}

JsonItem JsonObject::GetItemAtPos(size_t pos) const
{
	if (pos >= jsonMap_.size()) {
		throw std::runtime_error("Position out of range.");
	}

	return jsonMap_.at(jsonOrder_[pos]);
}

std::string JsonObject::PrintToString(bool format) const
{
	std::string jsonText = "{";
	if (format) {
		for (const std::string &key : jsonOrder_) {
			jsonText += "\n\t\"" + key + "\" :" + GetValue_(key).ValueRaw() + ",";
		}
		jsonText = jsonText.substr(0, jsonText.rfind(",")) + "\n}";
	} else {
		for (const std::string &key : jsonOrder_) {
			jsonText += "\"" + key + "\":" + GetValue_(key).ValueRaw() + ",";
		}
		jsonText = jsonText.substr(0, jsonText.rfind(",")) + "}";
	}
	
	return jsonText;
}

JsonItem JsonObject::GetValue_(const std::string &key) const
{
	auto iter = jsonMap_.find(key);
	if (iter == jsonMap_.end()) {
		throw std::runtime_error("Failed to get JsonValue.");
	}

	return iter->second;
}

void JsonObject::PutValue_(const std::string &key, const JsonItem &value)
{
	if (jsonMap_.count(key) == 0) {
		jsonOrder_.emplace_back(key);
		jsonMap_[key] = value;
	} else {
		jsonMap_[key] = value;
	}
}
