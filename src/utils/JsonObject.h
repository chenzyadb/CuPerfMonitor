// Simple-JsonObject V2 by chenzyadb.

#ifndef _JsonObject_V2_H
#define _JsonObject_V2_H

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>

class JsonObject;
class JsonItem
{
	public:
		JsonItem();
		JsonItem(const JsonItem &other);
		JsonItem(const std::string &raw);
		~JsonItem();
		JsonItem &operator=(const JsonItem &other);
		bool operator==(const JsonItem &other) const;
		bool operator!=(const JsonItem &other) const;

		std::string ValueRaw() const;
		double ValueDouble() const;
		int ValueInt() const;
		int64_t ValueLong() const;
		std::string ValueString() const;
		bool ValueBoolean() const;
		std::vector<JsonItem> ValueArray() const;
		JsonObject ValueJson() const;

	private:
		std::string valueRaw_;
		double valueDouble_;
		int valueInt_;
		int64_t valueLong_;
		std::string valueString_;
		bool valueBoolean_;
		std::vector<JsonItem> valueArray_;
		JsonObject* valueJson_;

		std::vector<JsonItem> ParseArray_(const std::string &valueRaw);
};

class JsonObject
{
	public:
		JsonObject();
		JsonObject(const std::string &jsonText);
		JsonObject(const JsonObject &other);
		~JsonObject();
		
		std::unordered_map<std::string, JsonItem> _Get_JsonMap() const;
		std::vector<std::string> _Get_JsonOrder() const;
		JsonObject &operator=(const JsonObject &other);
		JsonObject &operator+=(const JsonObject &other);
		bool operator==(const JsonObject &other) const;
		bool operator!=(const JsonObject &other) const;

		std::string GetValueRaw(const std::string &key) const;
		std::string GetValueString(const std::string &key) const;
		int GetValueInt(const std::string &key) const;
		int64_t GetValueLong(const std::string &key) const;
		double GetValueDouble(const std::string &key) const;
		bool GetValueBoolean(const std::string &key) const;
		JsonObject GetValueJson(const std::string &key) const;
		JsonItem GetValueItem(const std::string &key) const;

		std::vector<std::string> GetArrayRaw(const std::string &key) const;
		std::vector<std::string> GetArrayString(const std::string &key) const;
		std::vector<int> GetArrayInt(const std::string &key) const;
		std::vector<int64_t> GetArrayLong(const std::string &key) const;
		std::vector<double> GetArrayDouble(const std::string &key) const;
		std::vector<bool> GetArrayBoolean(const std::string &key) const;
		std::vector<JsonObject> GetArrayJson(const std::string &key) const;
		std::vector<JsonItem> GetArrayItem(const std::string &key) const;

		void PutValueRaw(const std::string &key, const std::string &value);
		void PutValueString(const std::string &key, const std::string &value);
		void PutValueInt(const std::string &key, const int &value);
		void PutValueLong(const std::string &key, const int64_t &value);
		void PutValueDouble(const std::string &key, const double &value);
		void PutValueBoolean(const std::string &key, const bool &value);
		void PutValueJson(const std::string &key, const JsonObject &value);
		void PutValueItem(const std::string &key, const JsonItem &value);

		void PutArrayRaw(const std::string &key, const std::vector<std::string> &array);
		void PutArrayString(const std::string &key, const std::vector<std::string> &array);
		void PutArrayInt(const std::string &key, const std::vector<int> &array);
		void PutArrayLong(const std::string &key, const std::vector<int64_t> &array);
		void PutArrayDouble(const std::string &key, const std::vector<double> &array);
		void PutArrayBoolean(const std::string &key, const std::vector<bool> &array);
		void PutArrayJson(const std::string &key, const std::vector<JsonObject> &array);
		void PutArrayItem(const std::string &key, const std::vector<JsonItem> &array);

		void Clear();
		void Remove(const std::string &key);
		bool Contains(const std::string &key) const;
		size_t Size() const;
		size_t GetKeyPos(const std::string &key) const;
		JsonItem GetItemAtPos(size_t pos) const;
		std::string PrintToString(bool format) const;

	private:
		std::unordered_map<std::string, JsonItem> jsonMap_;
		std::vector<std::string> jsonOrder_;

		JsonItem GetValue_(const std::string &key) const;
		void PutValue_(const std::string &key, const JsonItem &value);
};

#endif
