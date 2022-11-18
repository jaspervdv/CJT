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
		else if (isSilent_)
		{
			std::cout << "File: '" + filePath + "' is valid!" << std::endl;
		}

		// get complete list of all the present vertices
		objectTransformation_ = fetchTransformation(&completeData["transform"]);
		vertices_ = fetchPoints(&completeData["vertices"]);

		// generate geo
		auto cityObjects = completeData["CityObjects"];

		for (auto cityObject = cityObjects.begin(); cityObject != cityObjects.end(); ++cityObject)
		{
			CityObject CCityObject(cityObject.key(), cityObject.value()["type"]);

			auto geoObjects = cityObject.value()["geometry"];
			
			if (geoObjects.size() == 0)
			{
				continue;
			}


			for (auto geoObject = geoObjects.begin(); geoObject != geoObjects.end(); ++geoObject)
			{
				auto geoType = geoObject.value()["type"];
				std::string lod = geoObject.value()["lod"];
				
				if (geoType == "MultiSurface")
				{
					
				}
				else if (geoType == "Solid")
				{

				}
				else if (geoType == "CompositeSolid")
				{

				}

				
			}
		}
		return success;
	}

}


