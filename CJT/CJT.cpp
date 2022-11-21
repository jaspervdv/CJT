#include "CJT.h"

namespace CJT
{
	using json = nlohmann::json;
	
	void CJTPoint::print()
	{
		std::cout << "{" << x_ << ", " << y_ << ", " << z_ << "}" << std::endl;
	}

	ObjectTransformation::ObjectTransformation(json* transformationJson)
	{
		auto scaler = transformationJson->at("scale");
		auto translater = transformationJson->at("translate");

		xScale_ = scaler[0];
		yScale_ = scaler[1];
		zScale_ = scaler[2];

		xTrans_ = translater[0];
		yTrans_ = translater[1];
		zTrans_ = translater[2];
	}

	double* ObjectTransformation::getTranslation()
	{
		static double trans[3] = { xTrans_ , yTrans_, zTrans_ };
		return trans;
	}

	double* ObjectTransformation::getScale()
	{
		static double scaler[3] = { xScale_ , yScale_, zScale_ };
		return scaler;
	}

	GeoObject::GeoObject(json boundaries, float lod, json surfaceData, json surfaceTypeValues, std::string type)
	{
		boundaries_ = boundaries;
		lod_ = lod;
		surfaceData_ = surfaceData;
		surfaceTypeValues_ = surfaceTypeValues;
		type_ = type;
	}	


	bool CityCollection::isValid(json jsonData) {
		// TODO add validator library
		if (!jsonData.contains("CityObjects") || !jsonData.contains("version") || !jsonData.contains("transform") || !jsonData.contains("vertices"))
		{
			return false;
		};

		auto transformData = jsonData["transform"];
		if (!transformData.contains("scale") || !transformData.contains("translate"))
		{
			return false;
		}

		auto version = jsonData["version"];
		if (version != "1.1" && version != "1.0")
		{
			return false;
		}
		return true;
	}

	ObjectTransformation CityCollection::fetchTransformation(json* transJson)
	{		
		ObjectTransformation transformation(transJson);

		return transformation;
	}

	CityObject::CityObject(std::string name, std::string type)
	{
		name_ = name;
		type_ = type;
	}

	CityObject::CityObject(std::string name, std::string type, json attributes, std::vector<std::string> parentList, std::vector<std::string> childList)
	{
		name_ = name;
		type_ = type;

		if (attributes.size() > 0)
		{
			hasAttributes_ = true;
			attributes_ = attributes;
		}

		if (parentList.size() > 0)
		{
			isChild_ = true;
			parentList_ = parentList;
		}

		if (childList.size() > 0)
		{
			isParent_ = true;
			childList_ = childList;
		}
	}

	std::vector<CJTPoint> CityCollection::fetchPoints(json* pointJson)
	{
		std::vector<CJTPoint> vertList;
		double* scaler = objectTransformation_.getScale();

		for (auto point = pointJson->begin(); point != pointJson->end(); ++point)
		{
			auto coord = point.value();
			vertList.emplace_back(CJTPoint(coord[0] * scaler[0], coord[1] * scaler[1], coord[2] * scaler[2]));
		}
		return vertList;
	}

	std::map<std::string, CityObject> CityCollection::fetchCityObjects(json* cityObjects)
	{
		std::map<std::string, CityObject> collection;

		for (auto cityObject = cityObjects->begin(); cityObject != cityObjects->end(); ++cityObject)
		{
			std::string objectName = cityObject.key();
 
			CityObject CCityObject(objectName, cityObject.value()["type"]);
			auto geoObjects = cityObject.value()["geometry"];

			if (geoObjects.size() == 0)
			{
				continue;
			}

			for (auto geoObject = geoObjects.begin(); geoObject != geoObjects.end(); ++geoObject)
			{
				auto geoValue = geoObject.value();

				std::string geoType = "";
				json boundaries = {};
				float lod = -1 ;
				json semanticValues = {};
				json surfaceData = {};
				std::vector<std::string> parentList = {};
				std::vector<std::string> childList = {};
				
				if (geoValue.contains("type")) { geoType = geoValue["type"]; }
				if (geoValue.contains("boundaries")) { boundaries = geoValue["boundaries"]; }
				
				if (geoValue.contains("lod")) 
				{ 
					if (geoValue["lod"].type() == json::value_t::string)
					{
						std::string stringLod = geoValue["lod"];
						lod = stof(stringLod);
					}
					else
					{
						lod = geoValue["lod"];
					}
				}
				if (geoValue.contains("semantics")) 
				{ 
					json semanticData = geoValue["semantics"];
					if (semanticData.contains("values")) { semanticValues = semanticData["values"]; }
					if (semanticData.contains("surfaces")) { surfaceData = semanticData["surfaces"]; }

				}
				
				CCityObject.addGeoObject(lod, GeoObject(
					boundaries,
					lod,
					surfaceData,
					semanticValues,
					geoType
				));
			}

			collection.emplace(objectName, CCityObject);
		}
		return collection;
	}

	bool CityCollection::parseJSON(std::string filePath, bool silent)
	{
		// set initial booleans
		isSilent_ = silent;
		bool success = true;

		// parse json 
		std::ifstream f(filePath);
		json completeData = json::parse(f);

		// check if valid
		if (!isValid(completeData))
		{
			if (!isSilent_)
			{
				std::cout << "File: '" + filePath + "' is invalid!" << std::endl;
			}
			return false;
		}
		else if (!isSilent_)
		{
			std::cout << "File: '" + filePath + "' is valid!" << std::endl;
		}

		// get complete list of all the present vertices
		objectTransformation_ = fetchTransformation(&completeData["transform"]);
		vertices_ = fetchPoints(&completeData["vertices"]);

		if (!isSilent_)
		{
			std::cout << "Loaded " << vertices_.size() << " vertices" << std::endl;
		}

		cityObjects_ = fetchCityObjects(&completeData["CityObjects"]);

		if (!isSilent_)
		{
			std::cout << "Loaded " << cityObjects_.size() << " City Objects" << std::endl;
		}

		if (!isSilent_)
		{
			if (vertices_.size() != 0 && cityObjects_.size() != 0)
			{
				std::cout << "JSON Loaded succesfully" << std::endl;
			}
			else {
				std::cout << "JSON has not been loaded succesfully" << std::endl;
			}
		}

		return success;
	}
}


