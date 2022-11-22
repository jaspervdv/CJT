#include "CJT.h"

namespace CJT
{
	using json = nlohmann::json;
	
	std::array<double, 3> CJTPoint::getCoordinates()
	{
		return { x_ , y_, z_ };
	}

	double CJTPoint::getX()
	{
		return x_;
	}

	double CJTPoint::getY()
	{
		return y_;
	}

	double CJTPoint::getZ()
	{
		return z_;
	}

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

	GeoObject::GeoObject(json boundaries, std::string lod, json surfaceData, json surfaceTypeValues, std::string type)
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

	std::vector<float> CityObject::getLoD()
	{
		if (geometry_.size() < 1)
		{
			return {};
		}

		std::vector<float> loDList = {};
		for (size_t i = 0; i < geometry_.size(); i++)
		{
			float lod = std::stof(geometry_[i].getLoD());

			if (!std::count(loDList.begin(), loDList.end(), lod))
			{
				loDList.emplace_back(lod);
			}
		}
		std::sort(loDList.begin(), loDList.end());

		return loDList;
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
			json attributes = {};
			std::vector<std::string> parents = {};
			std::vector<std::string> children = {};

			json objectValue = cityObject.value();
			if (objectValue.contains("attributes")) { attributes = objectValue["attributes"]; }
			if (objectValue.contains("parents")) { parents = objectValue["parents"]; }
			if (objectValue.contains("children")) { children = objectValue["children"]; }

			CityObject CCityObject(
				objectName, 
				objectValue["type"],
				attributes,
				parents,
				children
			);
			auto geoObjects = objectValue["geometry"];

			if (geoObjects.size() == 0)
			{
				collection.emplace(objectName, CCityObject);
				continue;
			}

			for (auto geoObject = geoObjects.begin(); geoObject != geoObjects.end(); ++geoObject)
			{
				auto geoValue = geoObject.value();

				std::string geoType = "";
				json boundaries = {};
				std::string lod = "" ;
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
						lod = geoValue["lod"];
					}
					else
					{
						double doubleLoD = geoValue["lod"];

						if (doubleLoD == 0) { lod = "0"; }
						else if (doubleLoD == 0.1) { lod = "0.1"; }
						else if (doubleLoD == 0.2) { lod = "0.2"; }
						else if (doubleLoD == 0.3) { lod = "0.3"; }
						else if (doubleLoD == 0.4) { lod = "0.4"; }
						else if (doubleLoD == 1) { lod = "1"; }
						else if (doubleLoD == 1.1) { lod = "1.1"; }
						else if (doubleLoD == 1.2) { lod = "1.2"; }
						else if (doubleLoD == 1.3) { lod = "1.3"; }
						else if (doubleLoD == 1.4) { lod = "1.4"; }
						else if (doubleLoD == 2) { lod = "2"; }
						else if (doubleLoD == 2.1) { lod = "2.1"; }
						else if (doubleLoD == 2.2) { lod = "2.2"; }
						else if (doubleLoD == 2.3) { lod = "2.3"; }
						else if (doubleLoD == 2.4) { lod = "2.4"; }
					}
				}
				if (geoValue.contains("semantics")) 
				{ 
					json semanticData = geoValue["semantics"];
					if (semanticData.contains("values")) { semanticValues = semanticData["values"]; }
					if (semanticData.contains("surfaces")) { surfaceData = semanticData["surfaces"]; }

				}

				CCityObject.addGeoObject(
					GeoObject(
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

		version_ = completeData["version"];
		metaData_ = completeData["metadata"];

		return success;
	}


	bool CityCollection::dumpJson(std::string filePath, bool silent)
	{
		json newFile;

		// metdata collection
		std::pair fileType = { "type", "CityJSON" };
		std::pair version = { "version", version_ };

		newFile.emplace(fileType);
		newFile.emplace(version);

		if (metaData_.size() != 0)
		{
			newFile.emplace("metadata", metaData_);
		}

		// transformation data collection
		double* scalingdata = objectTransformation_.getScale();
		double* translatordata = objectTransformation_.getTranslation();
		std::pair scaler { "scale", std::vector<double>(
			{
				scalingdata[0],
				scalingdata[1],
				scalingdata[2]
			}
		) };

		std::pair translator{ "translate", std::vector<double>(
			{
				translatordata[0],
				translatordata[1],
				translatordata[2]
			}
		) };

		std::map transform {scaler, translator};
		std::pair transformer{ "transform", transform };
		newFile.emplace(transformer);

		// offload vertices
		std::vector<std::array<double, 3>> vertList;

		for (size_t i = 0; i < vertices_.size(); i++)
		{
			auto unscaledCoords = vertices_[i].getCoordinates();

			for (size_t j = 0; j < 3; j++)
			{
				unscaledCoords[j] = unscaledCoords[j] / scalingdata[j];
			}

			vertList.emplace_back(unscaledCoords);
		}
		newFile.emplace("vertices", vertList);

		// offload cityobjects
		json cityGeoCollection;
		std::map<std::string, json> objectCollection;
		for (std::pair<std::string, CityObject> cityObject : cityObjects_)
		{
			auto currentObject = cityObject.second;
			std::string objectName = currentObject.getName();
			std::string objectType = currentObject.getType();
			std::vector<GeoObject> geoObjectList = currentObject.getGeoObjects();
			std::map<std::string, json> cityObject;

			cityObject.emplace("type", objectType);

			// get geometry
			if (currentObject.hasGeo())
			{
				std::list<json> geoGroup;

				for (size_t i = 0; i < geoObjectList.size(); i++)
				{
					std::map<std::string, json> geoCollection;
					geoCollection.emplace("boundaries", geoObjectList[i].getBoundaries());
					geoCollection.emplace("lod", geoObjectList[i].getLoD());
					geoCollection.emplace("type", geoObjectList[i].getType());

					auto surfaceSemData = geoObjectList[i].getSurfaceData();
					if (!surfaceSemData.is_null()) // if no surface semantic data is supplied
					{
						std::pair surfaceTypePair{ "surfaces", surfaceSemData };
						std::pair surfaceValuePair{ "values", geoObjectList[i].getSurfaceTypeValues() };
						geoCollection.emplace("semantics", std::pair{ surfaceTypePair, surfaceValuePair });
					}
					geoGroup.emplace_back(geoCollection);
				}
				cityObject.emplace("geometry", geoGroup);
			}

			// get parents/children
			std::vector<std::string> parentList = currentObject.getParents();
			std::vector<std::string> childList = currentObject.getChildren();

			if (parentList.size())
			{
				cityObject.emplace("parents", parentList);
			}
			if (childList.size())
			{
				cityObject.emplace("children", childList);
			}

			objectCollection.emplace(objectName, cityObject);
		}
		newFile.emplace("CityObjects", objectCollection);

		std::ofstream fileLoc;

		fileLoc.open(filePath);
		fileLoc << newFile;
		fileLoc.close();

		return true;
	}
	std::vector<CityObject*> CityCollection::getCityObject()
	{
		std::vector<CityObject*> outputList;
		for (std::pair<std::string, CityObject> data : cityObjects_)
		{
			outputList.emplace_back(&data.second);
		}
		return outputList;
	}
	CityObject* CityCollection::getCityObject(std::string obName)
	{
		if (cityObjects_.find(obName) == cityObjects_.end())
		{
			std::cout << "Object Name not found" << std::endl;
			return nullptr;
		}

		return &cityObjects_[obName];
	}
	

	std::vector<CityObject*> CityCollection::getCityObject(std::vector<std::string> obNameList)
	{
		std::vector<CityObject*> cityObjectList;
		for (size_t i = 0; i < obNameList.size(); i++)
		{
			auto tempCityObject = getCityObject(obNameList[i]);

			if (tempCityObject == nullptr)
			{
				std::cout << obNameList[i] + " not found" << std::endl;
				continue;
			}
			cityObjectList.emplace_back(tempCityObject);
		}

		return cityObjectList;
	}


	std::vector<CJTPoint> CityCollection::getVerices()
	{
		return vertices_;
	}
}
