#pragma once
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <variant>
#include <vector>

#include <nlohmann/json.hpp>

namespace CJT {

	using json = nlohmann::json;
	class CJTPoint
	{
	private:
		double x_;
		double y_;
		double z_;

	public:
		CJTPoint(double x, double y, double z) { x_ = x; y_ = y; z_ = z; }
		
		std::array<double, 3> getCoordinates();
		double getX();
		double getY();
		double getZ();

		void print();
	};

	class ObjectTransformation
	{
	private:
		double xTrans_ = 0;
		double yTrans_ = 0;
		double zTrans_ = 0;

		double xScale_ = 1;
		double yScale_ = 1;
		double zScale_ = 1;

	public:
		ObjectTransformation() {};
		ObjectTransformation(double scale) { xScale_ = scale; yScale_ = scale; zScale_ = scale; }
		ObjectTransformation(double xScale, double yScale, double zScale) { xScale_ = xScale; yScale = yScale; zScale = zScale; }
		ObjectTransformation(json *transformationJson);

		double* getTranslation();
		double* getScale();
	};


	class GeoObject
	{
	private:
		json boundaries_;
		std::string lod_;
		json surfaceData_;
		json surfaceTypeValues_;
		std::string type_;

	public:
		GeoObject(
			json boundaries,
			std::string lod,
			json surfaceData,
			json surfaceTypeValues,
			std::string type
		);

		std::string getLoD() { return lod_; }
		std::string getType() { return type_; }

		json getBoundaries() { return boundaries_; };
		json getSurfaceData() { return surfaceData_; }
		json getSurfaceTypeValues() { return surfaceTypeValues_; }
	};


	class CityObject
	{
	private:
		std::string name_;
		std::string type_;

		std::vector<GeoObject> geometry_;

		bool hasGeo_ = false;
		bool isParent_ = false;
		bool isChild_ = false;
		bool hasAttributes_ = false;

		json attributes_ = {};
		std::vector<std::string> parentList_ = {};
		std::vector<std::string> childList_ = {};

	public:
		CityObject() {}

		/// @brief construct a simple cityobject
		CityObject
		(
			std::string name,
			std::string type
		);

		/// @brief construct a cityobject 
		CityObject
		(
			std::string name,
			std::string type,
			json attributes,
			std::vector<std::string> parentList,
			std::vector<std::string> childList
		);

		/// @brief get cityObject's name
		std::string getName() { return name_; }
		/// @brief get cityObject's type
		std::string getType() { return type_; }

		/// @brief returns true if the cityobject has one or more attributes
		bool hasAttributes() { return hasAttributes_; }
		/// @brief returns the attributes of the cityobject in a json format
		json getAttributes() { return attributes_; }

		/// @brief returns the LoD 
		std::vector<float> getLoD();
		
		/// @brief add an attribute to the cityObject
		template <typename T>
		void addAttribute(std::string key, T value);
		/// @brief add multiple attributes of the same type to the cityObject
		template <typename T>
		void addAttributes(std::map<std::string, T> keyValueList);
		/// @brief removes attribute with given keyName, if no keyName is given, all attributes are removed
		void removeAttribute(std::string keyName = "");
		/// @brief removes attribute with given keyName, if no keyName is given, all attributes are removed
		void removeAttribute(std::vector<std::string> keyName = {});

		/// @brief returns true if object has geometry
		bool hasGeo() { return hasGeo_; }
		/// @brief add geo object to the cityObject
		void addGeoObject(GeoObject geom) { geometry_.emplace_back(geom); hasGeo_ = true; }
		/// @brief get all geoObjects
		std::vector< GeoObject> getGeoObjects() { return geometry_; }

		/// @brief add a parent relationship to the CityObject
		void addParent(std::string parentName);
		/// @brief get a list of all the parents
		std::vector<std::string> getParents() { return parentList_; }
		/// @brief add a child reationship to the CityObject
		void addChild(std::string childName);
		/// @brief get a list of all the children
		std::vector<std::string> getChildren() { return childList_; }
	};


	class CityCollection 
	{
	private:
		std::map<std::string, CityObject> cityObjects_;
		std::vector<CJTPoint> vertices_;
		std::string version_ = "";
		json metaData_ = {};

		ObjectTransformation objectTransformation_;

		bool isSilent_ = true;

		bool isValid(json jsonData);

		ObjectTransformation fetchTransformation(json* transJson);
		std::vector<CJTPoint> fetchPoints(json* pointJson);
		std::map<std::string, CityObject> fetchCityObjects(json* cityObjects);

	public:
		/// @brief read a cityJSON file
		bool parseJSON(std::string filePath, bool silent);

		/// @brief dump to a cityJSON file
		bool dumpJson(std::string filePath, bool silent);

		/// @brief get all cityObjects
		std::vector<CityObject*> getCityObject();
		/// @brief get cityObject based on name
		CityObject* getCityObject(std::string obName);
		/// @brief get cityObjects based on name
		std::vector<CityObject*> getCityObject(std::vector<std::string> obNameList);
		/// @brief get cityObjects based on type
		//std::vector<CityObject> getCityObject(std::string typeName);

		// @brief returns all the vertsices that are in the collection
		std::vector<CJTPoint> getVerices();


	};

	template<typename T>
	inline void CityObject::addAttribute(std::string key, T value)
	{
		if (attributes_.contains(key))
		{
			std::cout << "key: " + key << " already in attributes of object " + name_ << std::endl;
			return;
		}
		attributes_.emplace(key, value);
		hasAttributes_ = true;
	}
	template<typename T>
	inline void CityObject::addAttributes(std::map<std::string, T> keyValueList)
	{
		for (auto pair = keyValueList.begin(); pair != keyValueList.end(); ++pair)
		{			
			addAttribute(pair->first, pair->second);
		}
	}
}