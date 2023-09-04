#include "inc/CJT.h"

namespace CJT
{
	using json = nlohmann::json;

	json updateVerts(json* boundaries, std::map<int, int>* correctingIdxMap, int depth) {
		if (depth != 0) { 
			for (json::iterator obb = boundaries->begin(); obb != boundaries->end(); ++obb)
			{
				json* subvalue = &obb.value();
				updateVerts(subvalue, correctingIdxMap, depth - 1);
			}
			return *boundaries;
		}
		else {
				*boundaries = json(correctingIdxMap->at((int)*boundaries));
				return *boundaries;
		}
		return {};
	}

	std::vector<int> getFlatVerts(json* boundaries) {
		std::vector<int> collection;
		if (boundaries[0].type() == json::value_t::number_unsigned || boundaries[0].type() == json::value_t::number_integer)
		{
			for (size_t i = 0; i < boundaries->size(); i++)
			{
				collection.emplace_back(boundaries[i]);
			}
		}
		else
		{
			for (json::iterator obb = boundaries->begin(); obb != boundaries->end(); ++obb)
			{
				json* subvalue = &obb.value();
				
				std::vector<int> outputboundaries = getFlatVerts(subvalue);
				for (size_t i = 0; i < outputboundaries.size(); i++)
				{
					collection.emplace_back(outputboundaries[i]);
				}
			}
		}
		return collection;
	}
	
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

	int findIdxVector(std::vector<std::string> searchVector, std::string name) {
		for (int i = 0; i < searchVector.size(); i++)
		{
			if (searchVector[i] == name)
			{
				return i;
			}
		}
		return -1;
	}


	std::string buildingTypeToString(Building_Type bType) {
		switch (bType)
		{
		case CJT::Building_Type::none:
			return "none";
		case CJT::Building_Type::Bridge:
			return "Bridge";
		case CJT::Building_Type::Building:
			return "Building";
		case CJT::Building_Type::CityFurniture:
			return "CityFurniture";
		case CJT::Building_Type::CityObjectGroup:
			return "CityObjectGroup";
		case CJT::Building_Type::LandUse:
			return "LandUse";
		case CJT::Building_Type::OtherConstruction:
			return "OtherConstruction";
		case CJT::Building_Type::PlantCover:
			return "PlantCover";
		case CJT::Building_Type::SolitaryVegetationObject:
			return "SolitaryVegetationObject";
		case CJT::Building_Type::TINRelief:
			return "TINRelief";
		case CJT::Building_Type::TransporationSquare:
			return "TransporationSquare";
		case CJT::Building_Type::Railway:
			return "Railway";
		case CJT::Building_Type::Road:
			return "Road";
		case CJT::Building_Type::Tunnel:
			return "Tunnel";
		case CJT::Building_Type::WaterBody:
			return "WaterBody";
		case CJT::Building_Type::Waterway:
			return "Waterway";
		case CJT::Building_Type::p_Extenstion:
			return "+Extenstion";
		case CJT::Building_Type::BridgePart:
			return "BridgePart";
		case CJT::Building_Type::BridgeInstallation:
			return "BridgeInstallation";
		case CJT::Building_Type::BridgeConstructiveElement:
			return "BridgeConstructiveElement";
		case CJT::Building_Type::BridgeRoom:
			return "BridgeRoom";
		case CJT::Building_Type::BridgeFurniture:
			return "BridgeFurniture";
		case CJT::Building_Type::BuildingPart:
			return "BuildingPart";
		case CJT::Building_Type::BuildingInstallation:
			return "BuildingInstallation";
		case CJT::Building_Type::BuildingConstructiveElement:
			return "BuildingConstructiveElement";
		case CJT::Building_Type::BuildingFurniture:
			return "BuildingFurniture";
		case CJT::Building_Type::BuildingStorey:
			return "BuildingStorey";
		case CJT::Building_Type::BuildingRoom:
			return "BuildingRoom";
		case CJT::Building_Type::BuildingUnit:
			return "BuildingUnit";
		case CJT::Building_Type::TunnelPart:
			return "TunnelPart";
		case CJT::Building_Type::TunnelInstallation:
			return "TunnelInstallation";
		case CJT::Building_Type::TunnelConstructiveElement:
			return "TunnelConstructiveElement";
		case CJT::Building_Type::TunnelHollowSpace:
			return "TunnelHollowSpace";
		case CJT::Building_Type::TunnelFurniture:
			return "TunnelFurniture";
		default:
			return "none";
		}
	}

	Building_Type stringToBuildingType(std::string stringType) {
		return Building_Type_Map()[stringType];
	}

	std::string roleToString(PoC_role roleEnum) {
		switch (roleEnum)
		{
		case CJT::PoC_role::none:
			return "none";
		case CJT::PoC_role::resourceProvider:
			return "resourceProvider";
		case CJT::PoC_role::custodian:
			return "custodian";
		case CJT::PoC_role::owner:
			return "owner";
		case CJT::PoC_role::user:
			return "user";
		case CJT::PoC_role::distributor:
			return "distributor";
		case CJT::PoC_role::originator:
			return "originator";
		case CJT::PoC_role::pointOfContact:
			return "pointOfContact";
		case CJT::PoC_role::principalInvestigator:
			return "principalInvestigator";
		case CJT::PoC_role::processor:
			return "processor";
		case CJT::PoC_role::publisher:
			return "publisher";
		case CJT::PoC_role::author:
			return "author";
		case CJT::PoC_role::sponsor:
			return "sponsor";
		case CJT::PoC_role::coauthor:
			return "coauthor";
		case CJT::PoC_role::collaborator:
			return "collaborator";
		case CJT::PoC_role::editor:
			return "editor";
		case CJT::PoC_role::mediator:
			return "mediator";
		case CJT::PoC_role::rightsHolder:
			return "rightsHolder";
		case CJT::PoC_role::contributor:
			return "contributor";
		case CJT::PoC_role::funder:
			return "funder";
		case CJT::PoC_role::stakeholder:
			return "stakeholder";
		default:
			return "none";
		}
	}

	PoC_role stringToPoCRole(std::string stringRole) {
		return PoC_role_Map()[stringRole];
	}

	std::string	contactTypeToString(PoC_type roleType) {
		switch (roleType)
		{
		case CJT::PoC_type::none:
			return "none";
		case CJT::PoC_type::individual:
			return "individual";
		case CJT::PoC_type::organization:
			return "organization";
		default:
			return "none";
		}
	}

	PoC_type stringToContactType(std::string stringType) {
		return PoC_type_Map()[stringType];
	}

	ObjectTransformation::ObjectTransformation(const json& transformationJson)
	{
		auto scaler = transformationJson.at("scale");
		auto translater = transformationJson.at("translate");

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

	bool MaterialObject::checkArrayValidity(std::array<float, 3> a)
	{
		for (size_t i = 0; i < a.size(); i++)
		{
			if (a[i] == -1)
			{
				return false;
			}
		}
		return true;
	}

	MaterialObject::MaterialObject(const json& materialJson)
	{
		if (materialJson.contains("name")) { name_ = materialJson["name"]; }
		if (materialJson.contains("ambientIntensity")) { ambientIntensity_ = materialJson["ambientIntensity"]; }
		if (materialJson.contains("diffuseColor")) { diffuseColor_ = materialJson["diffuseColor"]; }
		if (materialJson.contains("emissiveColor")) { emissiveColor_ = materialJson["emissiveColor"]; }
		if (materialJson.contains("specularColor")) { specularColor_ = materialJson["specularColor"]; }
		if (materialJson.contains("shininess")) { shininess_ = materialJson["shininess"]; }
		if (materialJson.contains("transparency")) { transparency_ = materialJson["transparency"]; }
		if (materialJson.contains("isSmooth")) { isSmooth_ = materialJson["isSmooth"]; }
	}

	MaterialObject::MaterialObject(
		const std::string& name,
		float ambientIntensity,
		const std::array<float, 3>& diffuseColor,
		const std::array<float, 3>& emissiveColor,
		const std::array<float, 3>& specularColor,
		float shininess,
		float transparency,
		bool isSmooth
	)
	{
		name_ = name;
		ambientIntensity_ = ambientIntensity;
		diffuseColor_ = diffuseColor;
		emissiveColor_ = emissiveColor;
		specularColor_ = specularColor;
		shininess_ = shininess;
		transparency_ = transparency;
		isSmooth_ = isSmooth;
	}

	MaterialObject::MaterialObject(
		std::string name,
		float ambientIntensity,
		std::array<float, 3> diffuseColor
	)
	{
		name_ = name;
		ambientIntensity_ = ambientIntensity;
		diffuseColor_ = diffuseColor;
	}

	MaterialObject::MaterialObject(const MaterialObject& other)
	{
		MaterialObject tempOther = other;

		name_ = tempOther.getName();
		ambientIntensity_ = tempOther.getAmbientIntensity();
		diffuseColor_ = tempOther.getDiffuseColor();
		emissiveColor_ = tempOther.getEmissiveColor();
		specularColor_ = tempOther.getSpecularColor();
		shininess_ = tempOther.getShininess();
		transparency_ = tempOther.getTransparency();
		isSmooth_ = tempOther.isSmooth_;
	}

	bool MaterialObject::hasName()
	{
		if (name_ == ""){ return false; }
		return true;
	}

	bool MaterialObject::hasAmbientIntensity()
	{
		if (ambientIntensity_ == -1) { return false; }
		return true;
	}

	bool MaterialObject::hasDiffuseColor()
	{
		return checkArrayValidity(diffuseColor_);
	}

	bool MaterialObject::hasEmissiveColor()
	{
		return checkArrayValidity(emissiveColor_);
	}

	bool MaterialObject::hasSpecularColor()
	{
		return checkArrayValidity(specularColor_);
	}

	bool MaterialObject::hasShininess()
	{
		if (shininess_ == -1) { return false; };
		return true;
	}

	bool MaterialObject::hasTransparency()
	{
		if (transparency_ == -1) { return false; };
		return true;
	}

	bool TextureObject::checkStringValidity(std::string a) 
	{
		if (a == "") { return false; }
		return true;
	}

	TextureObject::TextureObject(const json& textureJson)
	{
		if (textureJson.contains("type")) { type_ = textureJson["type"]; }
		if (textureJson.contains("image")) 
		{ 
			image_ = textureJson["image"]; 
			// create a name for easy access. 
			std::filesystem::path imagePath = image_;
			name_ = imagePath.filename().string();
		}
		if (textureJson.contains("wrapMode")) { wrapMode_ = textureJson["wrapMode"]; }
		if (textureJson.contains("textureType")) { type_ = textureJson["textureType"]; }
		if (textureJson.contains("borderColor")) { borderColor_ = textureJson["borderColor"]; }
	}

	bool TextureObject::hasName() 
	{
		return checkStringValidity(name_);
	}

	bool TextureObject::hasType() {
		return checkStringValidity(type_);
	}

	bool TextureObject::hasImage() {
		return checkStringValidity(image_);
	}

	bool TextureObject::hasWrapmode() {
		return checkStringValidity(wrapMode_);
	}

	bool TextureObject::hasBorderColor() {
		for (size_t i = 0; i < borderColor_.size(); i++)
		{
			if (borderColor_[i] == -1)
			{
				return false;
			}
		}
		return true;
	}


	void AppearanceObject::addMaterial(MaterialObject obb) {
		int matIdx = (int) materials_.size();
		obb.setIdx(matIdx);
		materials_.emplace_back(obb);
	}

	void AppearanceObject::addTexture(TextureObject obb) {
		int textIdx = (int) textures_.size();
		obb.setIdx(textIdx);
		textures_.emplace_back(obb);
	}

	std::vector<MaterialObject*> AppearanceObject::getMaterialsPtr()
	{
		std::vector<MaterialObject*> materialptr;
		for (size_t i = 0; i < materials_.size(); i++) { materialptr.emplace_back(&materials_[i]); }
		return materialptr;
	}

	std::vector<TextureObject*> AppearanceObject::getTexuresPtr()
	{
		std::vector<TextureObject*> texturePtr;
		for (size_t i = 0; i < textures_.size(); i++) { texturePtr.emplace_back(&textures_[i]); }
		return texturePtr;
	}

	bool CJTPoint::operator!=(CJTPoint other)
	{
		if (x_ == other.x_ && y_ == other.y_ && z_ == other.z_)
		{
			return false;
		}
		return true;
	}

	bool CJTPoint::operator<(CJTPoint other)
	{
		if (x_ + y_ + z_ < other.x_ + other.y_ + other.z_)
		{
			return true;
		}
		return false;
	}


	PointOfContactObject::PointOfContactObject(
		const std::string& contactName,
		const PoC_type& contactType,
		const PoC_role& role,
		const std::string& phone,
		const std::string& website,
		const std::string& address)
	{
		contactName_ = contactName;
		contactType_ = contactType;
		role_ = role;
		phone_ = phone;
		website_ = website;
		address_ = address;
	}

	PointOfContactObject::PointOfContactObject(const json& pointOfContact)
	{
		for (auto obb = pointOfContact.begin(); obb != pointOfContact.end(); ++obb)
		{
			std::string k = obb.key();

			if (k == "contactName") { contactName_ = obb.value(); }
			else if (k == "contactType") { contactType_ = stringToContactType(obb.value()); } // TODO: make less crash prone
			else if (k == "contactType") { role_ = stringToPoCRole(obb.value()); }
			else if (k == "phone") { phone_ = obb.value(); }
			else if (k == "website") { website_ = obb.value(); }
			else if (k == "address") { address_ = obb.value(); }
			else { additionalData_.emplace(k, obb.value()); }
		}
	}

	json PointOfContactObject::getData()
	{
		json outputJson;
		if (contactName_.size() != 0) { outputJson.emplace("contactName", contactName_); }
		if (contactType_ != PoC_type::none) { outputJson.emplace("contactType", contactTypeToString(contactType_)); }
		if (role_ != PoC_role::none) { outputJson.emplace("role", roleToString(role_)); }
		if (phone_.size() != 0) { outputJson.emplace("phone", phone_); }
		if (website_.size() != 0) { outputJson.emplace("website", website_); }
		if (address_.size() != 0) { outputJson.emplace("address", address_); }
		if (additionalData_.size() != 0) {
			for (auto obb = additionalData_.begin(); obb != additionalData_.end(); ++obb)
			{
				outputJson.emplace(obb.key(), obb.value());
			}
		}

		return outputJson;
	}
	metaDataObject::metaDataObject(
		const std::tuple<CJTPoint, CJTPoint>& geographicalExtent,
		const std::string& identifier,
		const PointOfContactObject& pointOfContact,
		const std::string& referenceDate,
		const std::string& referenceSystem,
		const std::string& title,
		const std::map<std::string, std::string>& additionalData
	)
	{
		geographicalExtent_ = geographicalExtent;
		identifier_ = identifier;
		pointOfContact_ = pointOfContact;
		referenceDate_ = referenceDate;
		referenceSystem_ = referenceSystem;
		title_ = title;
		additionalData_ = additionalData;
	}

	metaDataObject::metaDataObject(const json& metaData)
	{
		for (auto obb = metaData.begin(); obb != metaData.end(); ++obb)
		{
			std::string k = obb.key();

			if (k == "geographicalExtent")
			{
				auto geoExtend = obb.value();
				geographicalExtent_ = {
					CJTPoint(
						geoExtend[0],
						geoExtend[1],
						geoExtend[2]),
					CJTPoint(
						geoExtend[3],
						geoExtend[4],
						geoExtend[5]) };
			}
			else if (k == "identifier") { identifier_ = obb.value(); }
			else if (k == "referenceSystem") { referenceSystem_ = obb.value(); }
			else if (k == "referenceDate") { referenceDate_ = obb.value(); }
			else if (k == "referenceSystem") { referenceSystem_ = obb.value(); }
			else if (k == "title") { title_ = obb.value(); }
			else if (k == "datasetPointOfContact") { 
				PointOfContactObject pointofcontact(obb.value());
				pointOfContact_ = pointofcontact;
			}
			else { additionalData_.emplace(k, obb.value()); }
		}
	}

	json metaDataObject::getData()
	{
		json outputJson;

		if (std::get<0>(geographicalExtent_) != std::get<1>(geographicalExtent_))
		{
			std::vector<double> formExtent;
			CJTPoint p1 = std::get<0>(geographicalExtent_);
			CJTPoint p2 = std::get<1>(geographicalExtent_);

			formExtent.emplace_back(p1.getX());
			formExtent.emplace_back(p1.getY());
			formExtent.emplace_back(p1.getZ());

			formExtent.emplace_back(p2.getX());
			formExtent.emplace_back(p2.getY());
			formExtent.emplace_back(p2.getZ());

			outputJson.emplace("geographicalExtent", formExtent);
		}
		if (identifier_.size() != 0) { outputJson.emplace("identifier", identifier_); }
		if (referenceDate_.size() != 0) { outputJson.emplace("referenceDate", referenceDate_); }
		if (referenceSystem_.size() != 0) { outputJson.emplace("referenceSystem", referenceSystem_); }
		if (title_.size() != 0) { outputJson.emplace("title", title_); }
		if (additionalData_.size() != 0) { 
			for (auto obb = additionalData_.begin(); obb != additionalData_.end(); ++obb)
			{
				outputJson.emplace(obb.key(), obb.value());
			}
		}
		
		json pJson = pointOfContact_.getData();
		if (pJson.size() != 0)
		{
			outputJson.emplace("pointOfContact", pJson);
		}

		return outputJson;
	}

	CJTPoint metaDataObject::getExtend(int idx)
	{
		if (idx == 0) { return std::get<0>(geographicalExtent_); }
		else if (idx == 1) { return std::get<1>(geographicalExtent_); }
		else { 
			std::cout << "no point available with idx: " << idx << std::endl; 
			return CJTPoint(0, 0, 0);
		}
	}

	bool metaDataObject::checkInfo()
	{
		if (std::get<0>(geographicalExtent_) != std::get<1>(geographicalExtent_)) { return true; }
		if (identifier_.size() > 0) { return true; };
		if (referenceDate_.size() > 0) { return true; };
		if (referenceSystem_.size() > 0) { return true; };
		if (title_.size() > 0) { return true; };
		if (additionalData_.size() > 0) { return true; };
		//if (getPointOfContactPtr() != nullptr)
		//{	
		//	if (!getPointOfContactPtr()->getData().is_null()) { return true; }
		//}
		// TODO: check this
		return false;
	}

	void metaDataObject::setExtend(CJTPoint minPoint, CJTPoint maxPoint)
	{
		auto minPointVec = minPoint.getCoordinates();
		auto maxPointVec = maxPoint.getCoordinates();
		for (size_t i = 0; i < 3; i++)
		{
			if (minPointVec[i] > maxPointVec[i])
			{
				std::cout << "minpoint that does not behave like a minpoint is ignored" << std::endl;
				return;
			}
		}

		geographicalExtent_ = std::tuple<CJTPoint, CJTPoint>(minPoint, maxPoint);
	}

	void metaDataObject::addAdditionalData(json addData, bool overRide)
	{
		for (auto obb = addData.begin(); obb != addData.end(); ++obb)
		{
			std::string k = obb.key();

			if (additionalData_.contains(k) && !overRide)
			{
				continue;
			}
			if (additionalData_.contains(k))
			{
				additionalData_[k] = obb.value();
				continue;
			}
			additionalData_.emplace(k, obb.value());
		}
	}

	void metaDataObject::removeAdditionalData(std::string keyName)
	{
		if (additionalData_.contains(keyName))
		{
			additionalData_.erase(keyName);
		}
	}

	GeoObject::GeoObject(json boundaries, std::string lod, json surfaceData, json surfaceTypeValues, std::string type)
	{
		boundaries_ = boundaries;
		lod_ = lod;
		surfaceData_ = surfaceData;
		surfaceTypeValues_ = surfaceTypeValues;
		type_ = type;
	}

	GeoObject::GeoObject(json boundaries, std::string lod, std::string type)
	{
		boundaries_ = boundaries;
		lod_ = lod;
		type_ = type;
	}


	void GeoObject::clearSurfaceData(bool clearTypes) {
		surfaceData_.clear();
		if (!clearTypes) { return; }
		surfaceTypeValues_.clear();
	}

	void GeoObject::appendSurfaceData(std::map<std::string, std::string> surfaceData) {
		surfaceData_.emplace_back(surfaceData);
	}

	void GeoObject::appendSurfaceData(std::map<std::string, int> surfaceData) {
		surfaceData_.emplace_back(surfaceData);
	}

	void GeoObject::appendSurfaceData(std::map<std::string, double> surfaceData) {
		surfaceData_.emplace_back(surfaceData);
	}

	void GeoObject::setSurfaceTypeValue(int idx, int ref) {
		if (type_ == "Solid")
		{
			if (surfaceTypeValues_[0].size() <= idx ) { return; }
			surfaceTypeValues_[0][idx] = ref;
			return;
		}
		
		if (surfaceTypeValues_.size() < idx - 1) { return; }
		surfaceTypeValues_[idx] = ref;
	}


	void GeoObject::setSurfaceTypeValues(std::vector<int> references) {
		if (type_ == "Solid")
		{
			if (surfaceTypeValues_.size() == 0)
			{
				surfaceTypeValues_.emplace_back(json());
			}
			surfaceTypeValues_[0] = references;
		}
		else {
			surfaceTypeValues_ = references;
		}
	}

	void GeoObject::appendSurfaceTypeValue(int ref) {
		if (type_ == "Solid")
		{
			if (surfaceTypeValues_.size() == 0)
			{
				surfaceTypeValues_.emplace_back(json());
			}
			surfaceTypeValues_[0].emplace_back(ref);
		}
		else {
			surfaceTypeValues_.emplace_back(ref);
		}
	}


	bool CityCollection::isValid(const json& jsonData) {
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

	ObjectTransformation CityCollection::fetchTransformation(const json& transJson)
	{		
		ObjectTransformation transformation(transJson);

		return transformation;
	}

	CityObject::CityObject()
	{
		name_ = "";
		type_ = Building_Type::none;
	}

	CityObject::CityObject(
		const std::string& name,
		const Building_Type& type
	)
	{
		name_ = name;
		type_ = type;
	}

	CityObject::CityObject(
		const std::string& name,
		const Building_Type& type,
		const json& attributes,
		const std::vector<std::string>& parentList,
		const std::vector<std::string>& childList)
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

	void CityObject::removeAttribute(const std::string& keyName)
	{
		if (!attributes_.contains(keyName))
		{
			std::cout << "key: " + keyName << " cannot be found in object " + name_ << std::endl;
			return;
		}
		attributes_.erase(keyName);
	}

	void CityObject::removeAttribute(const std::vector<std::string>& keyNames)
	{		
		for (size_t i = 0; i < keyNames.size(); i++)
		{
			removeAttribute(keyNames[i]);
		}
	}

	void CityObject::clearAttributes()
	{
		attributes_ = {}; 
		hasAttributes_ = false;
	}

	void CityObject::addGeoObject(const GeoObject& geom)
	{
		geometry_.emplace_back(geom);
		hasGeo_ = true;
	}

	std::vector<GeoObject*> CityObject::getGeoObjectsPtr()
	{
		std::vector<GeoObject*> ptrVector;
		for (size_t i = 0; i < geometry_.size(); i++) { ptrVector.emplace_back(&geometry_[i]); }
		return ptrVector;
	}

	std::vector<GeoObject> CityObject::getGeoObjects(const std::string& lod)
	{
		std::vector<GeoObject> geoObjectList;
		for (size_t i = 0; i < geometry_.size(); i++)
		{
			if (geometry_[i].getLoD() == lod)
			{
				geoObjectList.emplace_back(geometry_[i]);
			}
		}

		return geoObjectList;
	}

	std::vector<GeoObject*> CityObject::getGeoObjectsPtr(const std::string& lod)
	{
		std::vector<GeoObject*> geoObjectList;
		for (size_t i = 0; i < geometry_.size(); i++)
		{
			if (geometry_[i].getLoD() == lod)
			{
				geoObjectList.emplace_back(&geometry_[i]);
			}
		}
		return geoObjectList;
	}

	void CityObject::addParent(CityObject* parentObject)
	{
		std::string parentName = parentObject->getName();

		if(std::find(parentList_.begin(), parentList_.end(), parentName) == parentList_.end()) {
			parentList_.emplace_back(parentName);
			parentObject->childList_.emplace_back(getName());
		}
	}

	void CityObject::setParent(const std::vector<std::string>& parentNames)
	{
		parentList_ = parentNames;
	}

	void CityObject::removeParent(CityObject* removeableObject)
	{
		std::string parentName = removeableObject->getName();
		int loc = findIdxVector(parentList_, parentName);
		if (loc != -1) { parentList_.erase(parentList_.begin() + loc); }
		else { return; }

		std::vector<std::string>* otherChildList = removeableObject->getChildrenPtr();
		loc = findIdxVector(*otherChildList, getName());
		if (loc != -1) { otherChildList->erase(otherChildList->begin() + loc); }

	}

	void CityObject::addChild(CityObject* childObject)
	{
		std::string childName = childObject->getName();

		if (std::find(childList_.begin(), childList_.end(), childName) == childList_.end()) {
			childList_.emplace_back(childObject->getName());
			childObject->parentList_.emplace_back(getName());
		}
	}

	void CityObject::setChild(const std::vector<std::string>& childNames)
	{
		childList_ = childNames;
	}

	void CityObject::removeChild(CityObject* removeableObject)
	{
		std::string childName = removeableObject->getName();
		int loc = findIdxVector(childList_, childName);
		if (loc != -1) { childList_.erase(childList_.begin() + loc); }
		else { return; }

		std::vector<std::string>* otherParentList = removeableObject->getParentsPtr();
		loc = findIdxVector(*otherParentList, getName());
		if (loc != -1) { otherParentList->erase(otherParentList->begin() + loc); }
	}

	std::vector<CJTPoint> CityCollection::fetchPoints(const json& pointJson)
	{
		std::vector<CJTPoint> vertList;
		double* scaler = objectTransformation_.getScale();

		for (auto point = pointJson.begin(); point != pointJson.end(); ++point)
		{
			auto coord = point.value();
			vertList.emplace_back(CJTPoint(coord[0] * scaler[0], coord[1] * scaler[1], coord[2] * scaler[2]));
		}
		return vertList;
	}

	std::map<std::string, CityObject> CityCollection::fetchCityObjects(const json& cityObjects)
	{
		std::map<std::string, CityObject> collection;

		for (auto cityObject = cityObjects.begin(); cityObject != cityObjects.end(); ++cityObject)
		{
			std::string objectName = cityObject.key();
			json attributes = {};
			std::vector<std::string> parents = {};
			std::vector<std::string> children = {};

			json objectValue = cityObject.value();
			if (objectValue.contains("attributes")) { attributes = objectValue["attributes"]; }
			if (objectValue.contains("parents")) { parents = objectValue["parents"]; }
			if (objectValue.contains("children")) { children = objectValue["children"]; }

			CityObject CCityObject = CityObject(
				objectName, 
				stringToBuildingType(objectValue["type"]), //TODO: make less crash prone and more effective
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
				GeoObject geoObjectP = GeoObject(
					boundaries,
					lod,
					surfaceData,
					semanticValues,
					geoType
				);

				CCityObject.addGeoObject(geoObjectP);
			}
			collection.emplace(objectName, CCityObject);
		}
		return collection;
	}

	AppearanceObject CityCollection::fetchAppearance(const json& AppearanceJson)
	{
		AppearanceObject newAppearanceObject;
		if (AppearanceJson.contains("materials"))
		{
			json materials = AppearanceJson.at("materials");
			for (size_t i = 0; i < materials.size(); i++)
			{
				newAppearanceObject.addMaterial(MaterialObject(materials[i]));
			}
		}

		if (AppearanceJson.contains("textures"))
		{
			json textures = AppearanceJson.at("textures");
			for (size_t i = 0; i < textures.size(); i++)
			{
				newAppearanceObject.addTexture(TextureObject(textures[i]));
			}
		}


		return newAppearanceObject;
	}
	
	bool CityCollection::parseJSON(const std::string& filePath, bool silent)
	{
		// set initial booleans
		isSilent_ = silent;
		bool success = true;

		if (!isSilent_)
		{
			std::cout << "===========================" << std::endl;
			std::cout << "Parsing file : " << filePath << std::endl;
		}

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
		objectTransformation_ = fetchTransformation(completeData["transform"]);
		vertices_ = fetchPoints(completeData["vertices"]);

		if (!isSilent_)
		{
			std::cout << "Loaded " << vertices_.size() << " vertices" << std::endl;
		}

		cityObjects_ = fetchCityObjects(completeData["CityObjects"]);

		if (!isSilent_)
		{
			std::cout << "Loaded " << cityObjects_.size() << " City Objects" << std::endl;
		}

		std::cout << "click" << std::endl;

		appearance_ = fetchAppearance(completeData["appearance"]);

		if (!isSilent_)
		{
			std::cout << "Loaded " << appearance_.getMaterialSize() << " material objects" << std::endl;
			std::cout << "Loaded " << appearance_.getTextureSize() << " texture objects" << std::endl;
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

		appearance_ = fetchAppearance(completeData["appearance"]);


		version_ = completeData["version"];
		metaData_ = metaDataObject(completeData["metadata"]);

		if (!isSilent_)
		{
			std::cout << "Success" << std::endl;
		}

		return success;
	}


	bool CityCollection::dumpJson(const std::string& filePath)
	{
		if (!isSilent_)
		{
			std::cout << "===========================" << std::endl;
			std::cout << "Writing data to: " << filePath << std::endl;
		}

		if (version_ != "1.1" && version_ != "1.0") { std::cout << "only cityJSON 1.0 and 1.1 supported" << std::endl; }

		json newFile;
		// metdata collection
		std::pair fileType = { "type", "CityJSON" };
		std::pair version = { "version", version_ };

		bool stringLoD = true;

		if (version_ == "1.0") { stringLoD = false; }

		newFile.emplace(fileType);
		newFile.emplace(version);

		if (metaData_.checkInfo())
		{
			newFile.emplace("metadata", metaData_.getData());
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
		std::vector<std::array<int, 3>> vertList;

		for (size_t i = 0; i < vertices_.size(); i++)
		{
			CJT::CJTPoint currentVert = vertices_[i];
			std::array<double, 3> unscaledCoords = currentVert.getCoordinates();
			std::array<int, 3> scaledCoords;

			for (size_t j = 0; j < 3; j++)
			{
				scaledCoords[j] = round(unscaledCoords[j] / scalingdata[j]);
			}

			vertList.emplace_back(scaledCoords);
		}
		newFile.emplace("vertices", vertList);

		// offload cityobjects
		json cityGeoCollection;
		std::map<std::string, json> objectCollection;
		for (std::pair<std::string, CityObject> cityObject : cityObjects_)
		{
			auto currentObject = cityObject.second;
			std::string objectName = currentObject.getName();
			std::string objectType = buildingTypeToString(currentObject.getType());
			std::vector<GeoObject> geoObjectList = currentObject.getGeoObjects();
			std::map<std::string, json> cityObject;

			cityObject.emplace("type", objectType);

			if (currentObject.hasAttributes())
			{
				cityObject.emplace("attributes", currentObject.getAttributes());
			}

			// get geometry
			if (currentObject.hasGeo())
			{
				std::list<json> geoGroup;

				for (size_t i = 0; i < geoObjectList.size(); i++)
				{
					std::map<std::string, json> geoCollection;
					geoCollection.emplace("boundaries", geoObjectList[i].getBoundaries());

					if (stringLoD)
					{
						geoCollection.emplace("lod", geoObjectList[i].getLoD());
					}
					else
					{
						geoCollection.emplace("lod", std::stod(geoObjectList[i].getLoD()));
					}


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

		// get materials
		std::vector<json> materialCollection;
		int c = 0;
		for (MaterialObject materialObject : getMaterials())
		{
			json materialJson;

			if (!materialObject.hasName())
			{
				std::cout << "material at idx: " << c << " has no name!" << std::endl;
				materialJson.emplace("name", "");
			}
			else {
				materialJson.emplace("name", materialObject.getName());
			}
			if (materialObject.hasAmbientIntensity())
			{
				materialJson.emplace("ambientIntensity", materialObject.getAmbientIntensity());
			}
			if (materialObject.hasDiffuseColor())
			{
				materialJson.emplace("diffuseColor", materialObject.getDiffuseColor());
			}
			if (materialObject.hasEmissiveColor())
			{
				materialJson.emplace("emissiveColor", materialObject.getEmissiveColor());
			}
			if (materialObject.hasSpecularColor())
			{
				materialJson.emplace("specularColor", materialObject.getSpecularColor());
			}
			if (materialObject.hasShininess())
			{
				materialJson.emplace("shininess", materialObject.getShininess());
			}
			if (materialObject.hasTransparency())
			{
				materialJson.emplace("transparency", materialObject.getTransparency());
			}

			materialJson.emplace("isSmooth", materialObject.getIsSmooth());

			materialCollection.emplace_back(materialJson);
			c++;
		}

		// get textures
		std::vector<json> textureCollection;
		c = 0;
		for (TextureObject textureObject : getTextures())
		{
			json textureJson;
			if (!textureObject.hasType())
			{
				std::cout << "material at idx: " << c << " has no type!" << std::endl;
				textureJson.emplace("type", "");
			}
			else
			{
				textureJson.emplace("type", textureObject.getType());
			}
			if (!textureObject.hasImage())
			{
				std::cout << "material at idx: " << c << " has no image!" << std::endl;
				textureJson.emplace("image", "");
			}
			else
			{
				textureJson.emplace("image", textureObject.getImage());
			}
			if (textureObject.hasWrapmode())
			{
				textureJson.emplace("wrapMode", textureObject.getWrapmode());
			}
			if (textureObject.hasBorderColor())
			{
				textureJson.emplace("borderColor", textureObject.getBorderColor());
			}

			textureCollection.emplace_back(textureJson);
		}

		std::map<std::string, json> appearanceCollecton;

		if (materialCollection.size() > 0) { appearanceCollecton.emplace("materials", materialCollection);}
		if (textureCollection.size() > 0) { appearanceCollecton.emplace("textures", textureCollection); }
		if (appearanceCollecton.size() > 0) { newFile.emplace("appearance", appearanceCollecton); }

		std::ofstream fileLoc;

		fileLoc.open(filePath);
		fileLoc << newFile;
		fileLoc.close();

		if (!isSilent_)
		{
			std::cout << "Success" << std::endl;
		}

		return true;
	}


	std::vector<CityObject> CityCollection::getAllCityObject()
	{
		std::vector<CityObject> outputList;
		for (std::pair<std::string, CityObject> data : cityObjects_)
		{
			outputList.emplace_back(data.second);
		}
		return outputList;
	}

	bool CityCollection::containsCityObject(const std::string& obName)
	{
		if (cityObjects_.find(obName) == cityObjects_.end()) { return false; }
		return true;
	}

	CityObject CityCollection::getCityObject(const std::string& obName)
	{
		return cityObjects_[obName];
	}

	CityObject* CityCollection::getCityObjectPtr(const std::string& obName)
	{
		return &cityObjects_[obName];
	}

	std::vector<CityObject> CityCollection::getCityObject(const std::vector<std::string>& obNameList)
	{
		std::vector<CityObject> cityObjectList;
		for (size_t i = 0; i < obNameList.size(); i++)
		{
			if (!containsCityObject(obNameList[i])) { continue; }
			auto tempCityObject = getCityObject(obNameList[i]);
			cityObjectList.emplace_back(tempCityObject);
		}
		return cityObjectList;
	}

	std::vector<CityObject*> CityCollection::getCityObjectPtr(const std::vector<std::string>& obNameList)
	{
		std::vector<CityObject*> cityObjectList;
		for (size_t i = 0; i < obNameList.size(); i++)
		{
			if (!containsCityObject(obNameList[i])) { continue; }
			auto tempCityObject = getCityObjectPtr(obNameList[i]);
			cityObjectList.emplace_back(tempCityObject);
		}
		return cityObjectList;
	}

	
	std::vector<CityObject> CityCollection::getCityObjectTyped(const Building_Type& typeName)
	{
		std::vector<CityObject> cityObjectList;
		for (auto obb = cityObjects_.begin(); obb != cityObjects_.end(); ++obb)
		{
			if (obb->second.getType() == typeName)
			{
				cityObjectList.emplace_back(obb->second);
			}
		}
		return cityObjectList;
	}

	std::vector<CityObject*> CityCollection::getCityObjectTypedPtr(const Building_Type& typeName)
	{
		std::vector<CityObject*> cityObjectList;
		for (auto obb = cityObjects_.begin(); obb != cityObjects_.end(); ++obb)
		{
			if (obb->second.getType() == typeName)
			{
				cityObjectList.emplace_back(&obb->second);
			}
		}
		return cityObjectList;
	}


	void CityCollection::addCityObject(const CityObject& cityObject)
	{
		CityObject tempObject = cityObject;
		cityObjects_.emplace(tempObject.getName(), tempObject);
	}

	MaterialObject CityCollection::getMaterial(int idx)
	{
		int buffer = 1;
		if (appearance_.getMaterialSize() >= idx + buffer)
		{
			return appearance_.getMaterial(idx);
		}	
		std::cout << "no material present with idx: " << idx << std::endl;
		return MaterialObject();
	}
	
	void CityCollection::removeCityObject(const std::string& obName)
	{
		if (cityObjects_.find(obName) != cityObjects_.end())
		{
			cityObjects_.erase(obName);
		}
	}


	const std::vector<CJTPoint>& CityCollection::getVerices()
	{
		return vertices_;
	}


	int CityCollection::addVertex(const CJTPoint& point, bool unique)
	{
		double scaleX = objectTransformation_.getScale()[0];
		double scaleY = objectTransformation_.getScale()[1];
		double scaleZ = objectTransformation_.getScale()[2];

		CJTPoint p = point;

		int location = -1;
		if (unique)
		{
			int i = 0;
			for (std::vector<CJT::CJTPoint>::iterator it = vertices_.begin(); it != vertices_.end(); ++it) 
			{
				CJTPoint otherPoint = *it;

				if (std::abs(p.getX() - otherPoint.getX()) <= scaleX &&
					std::abs(p.getY() - otherPoint.getY()) <= scaleY &&
					std::abs(p.getZ() - otherPoint.getZ()) <= scaleZ)
				{
					location = i;
					break;
				}
				i++;
			}
		}
		if (location == -1)
		{
			location = static_cast<int>(vertices_.size());
			vertices_.emplace_back(point);
			return location;
		}
		return location;
	}


	const std::vector<int>& CityCollection::addVertex(const std::vector<CJTPoint>& pointList, bool checkUnique)
	{
		std::vector<int> locationList;
		for (size_t i = 0; i < pointList.size(); i++)
		{
			locationList.emplace_back(addVertex(pointList[i]));
		}
		return locationList;
	}

	void CityCollection::cullDuplicatedVerices() 
	{
		if (!isSilent_)
		{	
			std::cout << "===========================" << std::endl;
			std::cout << "Culling duplicated vertices" << std::endl;
		}

		// remove dups and make map that maps the correcting locations
		std::map<int, int> correctingIdxMap;
		std::vector<CJTPoint> correctedvertices;
		int correctionAmount = 0;
		int i = 0;

		double scaleX = objectTransformation_.getScale()[0];
		double scaleY = objectTransformation_.getScale()[1];;
		double scaleZ = objectTransformation_.getScale()[2];;

		for (std::vector<CJT::CJTPoint>::iterator it = vertices_.begin(); it != vertices_.end(); ++it)
		{
			int j = 0;
			int doubleIdx = -1;
			bool found = false;
			CJTPoint currentPoint =*it;

			for (std::vector<CJTPoint>::iterator it = correctedvertices.begin(); it != correctedvertices.end(); ++it )
			{
				CJTPoint otherPoint = *it;

				if (std::abs(currentPoint.getX() - otherPoint.getX()) <= scaleX &&
					std::abs(currentPoint.getY() - otherPoint.getY()) <= scaleY &&
					std::abs(currentPoint.getZ() - otherPoint.getZ()) <= scaleZ)
				{
					found = true;
					doubleIdx = j;
					break;
				}
				j++;
			}
			if (!found)
			{
				correctedvertices.emplace_back(currentPoint);
				correctingIdxMap.emplace(i, i - correctionAmount);
				
			}
			else {
				
				correctingIdxMap.emplace(i, doubleIdx);
				correctionAmount++;
			}
			i++;
		}

		// correct geo references
		if (correctedvertices.size() == vertices_.size()) 
		{ 
			std::cout << "No duplicate vertices found" << std::endl;
			return; 
		}

		if (!isSilent_)
		{
			std::cout << "Reduced vertices count from: " << vertices_.size() << ", to: " << correctedvertices.size() << std::endl;
			std::cout << "Correcting vertices referencing" << std::endl;
		}

		//delete vertices_;
		vertices_ = correctedvertices;
		//correctedvertices = nullptr;

		for (auto obb = cityObjects_.begin(); obb != cityObjects_.end(); ++obb)
		{
			CityObject currentCityObject = obb->second;

			if (!currentCityObject.hasGeo()) { continue; }

			std::vector<GeoObject> curentGeoObjects = currentCityObject.getGeoObjects();

			for (size_t i = 0; i < curentGeoObjects.size(); i++)
			{
				GeoObject currentGeoObject = curentGeoObjects[i];
				json boundaries = currentGeoObject.getBoundaries();

				std::string geoType = currentGeoObject.getType();

				if (geoType == "MultiSurface")
				{
					currentGeoObject.setBoundaries(updateVerts(&boundaries, &correctingIdxMap, 3));
				}	

				if (geoType == "Solid")
				{
					currentGeoObject.setBoundaries(updateVerts(&boundaries, &correctingIdxMap, 4));
				}
			}
			currentCityObject.setGeo(curentGeoObjects);
			cityObjects_[obb->first] = currentCityObject;
		}

		if (!isSilent_)
		{
			std::cout << "Succesfully corrected" << std::endl;
		}
	}

	void CityCollection::cullUnreferencedVerices()
	{

		if (!isSilent_)
		{
			std::cout << "===========================" << std::endl;
			std::cout << "Culling unreferenced vertices" << std::endl;
		}

		std::vector<bool> vertreference;

		for (std::vector<CJT::CJTPoint>::iterator it = vertices_.begin(); it != vertices_.end(); ++it)
		{
			vertreference.emplace_back(false);
		}

		for (auto obb = cityObjects_.begin(); obb != cityObjects_.end(); ++obb)
		{
			CityObject currentCityObject = obb->second;

			if (!currentCityObject.hasGeo()) { continue; }

			std::vector<GeoObject> curentGeoObjects = currentCityObject.getGeoObjects();

			for (std::vector<GeoObject>::iterator it = curentGeoObjects.begin(); it != curentGeoObjects.end(); ++it)
			{
				GeoObject currentGeoObject = *it;
				json boundaries = currentGeoObject.getBoundaries();

				std::string geoType = currentGeoObject.getType();
				std::vector<int> referencesIdx;

				referencesIdx = getFlatVerts(&boundaries);

				for (size_t j = 0; j < referencesIdx.size(); j++)
				{
					vertreference[referencesIdx[j]] = true;
				}
			}
		}

		std::map<int, int> correctingIdxMap;;
		int correctionAmount = 0;
		int currentAmount = 0;
		std::vector<CJTPoint> correctedvertices;

		for (std::vector<bool>::iterator it = vertreference.begin(); it != vertreference.end(); ++it)
		{
			if (*it == false)
			{
				correctionAmount++;
			}
			else
			{
				correctedvertices.emplace_back(vertices_[currentAmount]);
				correctingIdxMap.emplace(currentAmount, currentAmount - correctionAmount);
			}
			currentAmount++;
		}

		if (correctingIdxMap.size() == 0)
		{
			return;
		}

		if (!isSilent_)
		{
			if (correctionAmount == 0)
			{
				std::cout << "No unreferenced vertices found" << std::endl;
			}
			else {
				std::cout << correctionAmount << " unreferenced vertices found" << std::endl;
				std::cout << "Correcting vertices referencing" << std::endl;
			}
			
		}

		// correct geo references
		if (correctedvertices.size() == vertices_.size()) { return; }

		vertices_ = correctedvertices;
		//correctedvertices = nullptr;

		std::cout << cityObjects_.size() << std::endl;

		for (auto obb = cityObjects_.begin(); obb != cityObjects_.end(); ++obb)
		{
			CityObject currentCityObject = obb->second;

			if (!currentCityObject.hasGeo()) { continue; }

			std::vector<GeoObject> curentGeoObjects = currentCityObject.getGeoObjects();

			for (size_t i = 0; i < curentGeoObjects.size(); i++)
			{
				std::cout << i << std::endl;
				GeoObject currentGeoObject = curentGeoObjects[i];
				json boundaries = currentGeoObject.getBoundaries();

				std::string geoType = currentGeoObject.getType();

				if (geoType == "MultiSurface")
				{
					currentGeoObject.setBoundaries(updateVerts(&boundaries, &correctingIdxMap, 3));
				}

				if (geoType == "Solid")
				{
					updateVerts(&boundaries, &correctingIdxMap, 4);
					currentGeoObject.setBoundaries(updateVerts(&boundaries, &correctingIdxMap, 4));
				}
			}
			currentCityObject.setGeo(curentGeoObjects);
			std::cout << obb->first << std::endl;
			cityObjects_[obb->first] = currentCityObject;
		}
		std::cout << "end" << std::endl;

		if (!isSilent_)
		{
			std::cout << "Succesfully corrected" << std::endl;
		}
	}

	void CityCollection::CleanVertices()
	{
		cullUnreferencedVerices();
		cullDuplicatedVerices();
	}

	//*/
}
