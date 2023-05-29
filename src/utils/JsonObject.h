// Simple-JsonObject V1 by chenzyadb.

#ifndef _JsonObject_H
#define _JsonObject_H

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>

class JsonObject
{
	public:
		JsonObject();
		JsonObject(const std::string &jsonText);
		JsonObject(const JsonObject &other);
		~JsonObject();

		std::unordered_map<std::string, std::string> _Get_JsonMap() const;
		std::vector<std::string> _Get_JsonOrder() const;
		JsonObject &operator=(const JsonObject &other);
		JsonObject &operator+=(const JsonObject &other);
		bool operator==(const JsonObject &other) const;
		bool operator!=(const JsonObject &other) const;

		std::string GetValueString(const std::string &key) const;
		int GetValueInt(const std::string &key) const;
		long GetValueLong(const std::string &key) const;
		double GetValueDouble(const std::string &key) const;
		bool GetValueBoolean(const std::string &key) const;
		JsonObject GetValueJson(const std::string &key) const;

		std::vector<std::string> GetArrayString(const std::string &key) const;
		std::vector<int> GetArrayInt(const std::string &key) const;
		std::vector<long> GetArrayLong(const std::string &key) const;
		std::vector<double> GetArrayDouble(const std::string &key) const;
		std::vector<bool> GetArrayBoolean(const std::string &key) const;
		std::vector<JsonObject> GetArrayJson(const std::string &key) const;

		void PutValueString(const std::string &key, const std::string &value);
		void PutValueInt(const std::string &key, const int &value);
		void PutValueLong(const std::string &key, const long &value);
		void PutValueDouble(const std::string &key, const double &value);
		void PutValueBoolean(const std::string &key, const bool &value);
		void PutValueJson(const std::string &key, const JsonObject &value);

		void PutArrayString(const std::string &key, const std::vector<std::string> &array);
		void PutArrayInt(const std::string &key, const std::vector<int> &array);
		void PutArrayLong(const std::string &key, const std::vector<long> &array);
		void PutArrayDouble(const std::string &key, const std::vector<double> &array);
		void PutArrayBoolean(const std::string &key, const std::vector<bool> &array);
		void PutArrayJson(const std::string &key, const std::vector<JsonObject> &array);

		void Clear();
		void Remove(const std::string &key);
		bool Contains(const std::string &key) const;
		std::string PrintToString(const bool &format) const;

	private:
		std::unordered_map<std::string, std::string> jsonMap_;
		std::vector<std::string> jsonOrder_;

		std::string GetValue_(const std::string &key) const;
		std::vector<std::string> GetArray_(const std::string &key) const;
		void PutValue_(const std::string &key, const std::string &value);
};

#endif
