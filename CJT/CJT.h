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


	class CityObject
	{
	private:
		std::string name_;
		std::string type_;

		std::map<std::string, json> geometry_;
	public:
		CityObject(std::string name, std::string type) { name_ = name, type_ = type; }

		void addGeoObject(std::string lod, json geom) { geometry_.emplace(lod, geom); }
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