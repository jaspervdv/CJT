#pragma once
#include <iostream>
#include <map>
#include <fstream>
#include <string>

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
		
		double* getCoordinates();
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
		float lod_;
		json surfaceData_;
		json surfaceTypeValues_;
		std::string type_;

	public:
		GeoObject(
			json boundaries,
			float lod,
			json surfaceData,
			json surfaceTypeValues,
			std::string type
		);
	};


	class CityObject
	{
	private:
		std::string name_;
		std::string type_;

		std::map<float, GeoObject> geometry_;

		bool isParent_ = false;
		bool isChild_ = false;
		bool hasAttributes_ = false;

		json attributes_ = {};
		std::vector<std::string> parentList_ = {};
		std::vector<std::string> childList_ = {};

	public:
		CityObject
		(
			std::string name,
			std::string type
		);

		CityObject
		(
			std::string name,
			std::string type,
			json attributes,
			std::vector<std::string> parentList,
			std::vector<std::string> childList
		);

		void addGeoObject(float lod, GeoObject geom) { geometry_.emplace(lod, geom); }
	};


	class CityCollection 
	{
	private:
		std::map<std::string, CityObject> cityObjects_;
		std::vector<CJTPoint> vertices_;
		ObjectTransformation objectTransformation_;

		bool isSilent_ = true;

		bool isValid(json jsonData);
		ObjectTransformation fetchTransformation(json* transJson);
		std::vector<CJTPoint> fetchPoints(json* pointJson);
		std::map<std::string, CityObject> fetchCityObjects(json* cityObjects);

	public:
		// read file
		bool parseJSON(std::string filePath, bool silent);

		// dump to file
		bool dumpJson(std::string filePath, bool silent);

		// get city objects
		CityObject getCityObject(std::string obName);
		std::vector<CityObject> getCityObject(std::vector<std::string> obNameList);
	};
}