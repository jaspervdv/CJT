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
	
	// Forward Declaration
	//class CityCollection;

	/// @brief enum class representing the possible building types
	enum class Building_Type
	{
		none,

		// 1st level objects
		Bridge = 1000,
		Building,
		CityFurniture,
		CityObjectGroup,
		LandUse,
		OtherConstruction,
		PlantCover,
		SolitaryVegetationObject,
		TINRelief,
		TransporationSquare,
		Railway,
		Road,
		Tunnel,
		WaterBody,
		Waterway,
		p_Extenstion,

		// 2nd level objects
		BridgePart = 2000, //TODO: request parent when using 2nd lvl objects
		BridgeInstallation,
		BridgeConstructiveElement,
		BridgeRoom,
		BridgeFurniture,

		BuildingPart,
		BuildingInstallation,
		BuildingConstructiveElement,
		BuildingFurniture,
		BuildingStorey,
		BuildingRoom,
		BuildingUnit,

		TunnelPart,
		TunnelInstallation,
		TunnelConstructiveElement,
		TunnelHollowSpace,
		TunnelFurniture
	};

	/// @brief struct allowing for the translation from string to building types
	struct Building_Type_Map : public std::map<std::string, Building_Type> // TODO: find an elegant solution for this 
	{	
		Building_Type_Map()
		{
			this->operator[]("none") = Building_Type::none;
			this->operator[]("Bridge") = Building_Type::Bridge;
			this->operator[]("Building") = Building_Type::Building;
			this->operator[]("CityFurniture") = Building_Type::CityFurniture;
			this->operator[]("CityObjectGroup") = Building_Type::CityObjectGroup;
			this->operator[]("LandUse") = Building_Type::LandUse;
			this->operator[]("OtherConstruction") = Building_Type::OtherConstruction;
			this->operator[]("SolitaryVegetationObject") = Building_Type::SolitaryVegetationObject;
			this->operator[]("TINRelief") = Building_Type::TINRelief;
			this->operator[]("TransporationSquare") = Building_Type::TransporationSquare;
			this->operator[]("Railway") = Building_Type::Railway;
			this->operator[]("Road") = Building_Type::Road;
			this->operator[]("Tunnel") = Building_Type::Tunnel;
			this->operator[]("WaterBody") = Building_Type::WaterBody;
			this->operator[]("Waterway") = Building_Type::Waterway;
			this->operator[]("+Extenstion") = Building_Type::p_Extenstion;
			this->operator[]("BridgePart") = Building_Type::BridgePart;
			this->operator[]("BridgeInstallation") = Building_Type::BridgeInstallation;
			this->operator[]("BridgeConstructiveElement") = Building_Type::BridgeConstructiveElement;
			this->operator[]("BridgeRoom") = Building_Type::BridgeRoom;
			this->operator[]("BridgeFurniture") = Building_Type::BridgeFurniture;
			this->operator[]("BuildingStorey") = Building_Type::BuildingStorey;
			this->operator[]("BuildingRoom") = Building_Type::BuildingRoom;
			this->operator[]("BuildingUnit") = Building_Type::BuildingUnit;
			this->operator[]("TunnelPart") = Building_Type::TunnelPart;
			this->operator[]("TunnelInstallation") = Building_Type::TunnelInstallation;
			this->operator[]("TunnelConstructiveElement") = Building_Type::TunnelConstructiveElement;
			this->operator[]("TunnelHollowSpace") = Building_Type::TunnelHollowSpace;
			this->operator[]("TunnelFurniture") = Building_Type::TunnelFurniture;
		};
		~Building_Type_Map() {}
	};

	/// @brief enum class representing the possible point of contact roles
	enum class PoC_role 
	{
		none,
		resourceProvider,
		custodian,
		owner,
		user,
		distributor,
		originator,
		pointOfContact,
		principalInvestigator,
		processor,
		publisher,
		author,
		sponsor,
		coauthor,
		collaborator,
		editor,
		mediator,
		rightsHolder,
		contributor,
		funder,
		stakeholder
	};

	/// @brief struct allowing for the translation from string to point of contact roles
	struct PoC_role_Map : public std::map<std::string, PoC_role>
	{
		PoC_role_Map()
		{
			this->operator[]("none") = PoC_role::none;
			this->operator[]("resourceProvider") = PoC_role::resourceProvider;
			this->operator[]("custodian") = PoC_role::custodian;
			this->operator[]("owner") = PoC_role::owner;
			this->operator[]("user") = PoC_role::user;
			this->operator[]("distributor") = PoC_role::distributor;
			this->operator[]("originator") = PoC_role::originator;
			this->operator[]("pointOfContact") = PoC_role::pointOfContact;
			this->operator[]("principalInvestigator") = PoC_role::principalInvestigator;
			this->operator[]("processor") = PoC_role::processor;
			this->operator[]("publisher") = PoC_role::publisher;
			this->operator[]("author") = PoC_role::author;
			this->operator[]("sponsor") = PoC_role::sponsor;
			this->operator[]("coauthor") = PoC_role::coauthor;
			this->operator[]("collaborator") = PoC_role::collaborator;
			this->operator[]("editor") = PoC_role::editor;
			this->operator[]("mediator") = PoC_role::mediator;
			this->operator[]("rightsHolder") = PoC_role::rightsHolder;
			this->operator[]("contributor") = PoC_role::contributor;
			this->operator[]("funder") = PoC_role::funder;
			this->operator[]("stakeholder") = PoC_role::stakeholder;
		};
		~PoC_role_Map() {}
	};

	/// @brief enum class representing the possible point of contact types
	enum class PoC_type
	{
		none,
		individual,
		organization
	};

	/// @brief struct allowing for the translation from string to point of contact types
	struct PoC_type_Map : public std::map<std::string, PoC_type>
	{
		PoC_type_Map()
		{
			this->operator[]("none") = PoC_type::none;
			this->operator[]("individual") = PoC_type::individual;
			this->operator[]("organization") = PoC_type::organization;
		};
		~PoC_type_Map() {}
	};

	class CJTPoint
	{
	private:
		double x_;
		double y_;
		double z_;

	public:
		/// @brief constructs a point based on doubles representing the x, y and z coordinates
		CJTPoint(double x, double y, double z) { x_ = x; y_ = y; z_ = z; }
		
		/// @brief returns an array representing the x,y and z coordinates
		std::array<double, 3> getCoordinates();
		/// @brief returns the x coordinate
		double getX();
		/// @brief returns the y coordinate
		double getY();
		/// @brief returns the z coordinate
		double getZ();

		/// @brief couts the x, y, z coordinates
		void print();

		/// @brief evaluates the x, y, z coordinates to see if points are not equal
		bool operator!= (CJTPoint other);
		/// @brief evaluates if the sum of the x, y, z coordinates is smaller 
		bool operator< (CJTPoint other);
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
		/// @brief creates empty transformation object
		ObjectTransformation() {};
		/// @brief creates transformation object with uniform scale
		ObjectTransformation(double scale) { xScale_ = scale; yScale_ = scale; zScale_ = scale; }
		/// @brief creates transformation object with, possible, non uniform scale
		ObjectTransformation(double xScale, double yScale, double zScale) { xScale_ = xScale; yScale = yScale; zScale = zScale; }
		/// @brief creates transformation object based on CityJSON formatted transformation info
		ObjectTransformation(const json& transformationJson);

		/// @brief returns an array for size 3 representing the translation
		double* getTranslation();
		/// @brief set the translation of the transformation
		void setTranslation(double x, double y, double z) { xTrans_ = x; yTrans_ = y; zTrans_ = z; };
		/// @brief returns an array for size 3 representing the scaling
		double* getScale();
	};


	class MaterialObject 
	{
	private:
		std::string name_ = "";
		float ambientIntensity_ = -1;
		std::array<float, 3> diffuseColor_ = { -1, -1, -1 };
		std::array<float, 3> emissiveColor_ = { -1, -1, -1 };
		std::array<float, 3> specularColor_ = { -1, -1, -1 };
		float shininess_ = -1;
		float transparency_ = -1;
		bool isSmooth_ = false;
		int idx_ = -1;

		bool checkArrayValidity(std::array<float, 3> a);

	public:
		/// @brief creates an empty material object
		MaterialObject() {};
		/// @brief creates an material object based on a CityJSON formatted material object
		MaterialObject(const json& materialJson);
		/// @brief creates a complex material object
		MaterialObject(
			const std::string& name,
			float ambientIntensity,
			const std::array<float, 3>& diffuseColor,
			const std::array<float, 3>& emissiveColor,
			const std::array<float, 3>& specularColor,
			float shininess,
			float transparency,
			bool isSmooth
		);
		/// @brief creates a simple material object
		MaterialObject(
			std::string name,
			float ambientIntensity,
			std::array<float, 3> diffuseColor
		);
		/// @brief creates an copy of an other material object
		MaterialObject(const MaterialObject& other);

		/// @brief returns name of the materal object, "" if no name
		std::string getName() { return name_; }
		/// @brief returns true if material object has a name
		bool hasName();

		/// @brief returns the ambient intensity of the materal object, -1 if no data is present
		float getAmbientIntensity() { return ambientIntensity_; }
		/// @brief returns true if material object has an ambient intensity
		bool hasAmbientIntensity();

		/// @brief returns the diffuse color of the materal object, populated with -1 if no data is present
		std::array<float, 3> getDiffuseColor(){ return diffuseColor_; }
		/// @brief returns true if material object has a diffuse color
		bool hasDiffuseColor();

		/// @brief returns the emmisve color of the materal object, populated with -1 if no data is present
		std::array<float, 3> getEmissiveColor() { return emissiveColor_; }
		/// @brief returns true if material object has an emmisve color
		bool hasEmissiveColor();

		/// @brief returns the specular color of the materal object, populated with -1 if no data is present
		std::array<float, 3> getSpecularColor() { return specularColor_; }
		/// @brief returns true if material object has a specular color
		bool hasSpecularColor();

		/// @brief returns the shininess of the material object, -1 if no data is present
		float getShininess() {return shininess_;}
		/// @brief returns true if material object has a shininess value
		bool hasShininess();

		/// @brief returns the transparancy of the material object, -1 if no data is present
		float getTransparency() { return transparency_; }
		/// @brief returns true if material object has a Transparency value
		bool hasTransparency();

		/// @brief returns if material object is smooth
		bool getIsSmooth() { return isSmooth_; }

		/// @brief returns the index of the object (location in list), -1 is no data is present
		int getIdx() { return idx_; }

		/// @brief returns the index of the object
		void setIdx(int idx) { idx_ = idx; }
	};


	class TextureObject 
	{
	private:
		std::string name_ = "";
		std::string type_ = "";
		std::string image_ = "";
		std::string wrapMode_ = "";
		std::string textureType_ = "";
		std::array<float, 3> borderColor_ = { -1, -1, -1 };
		int idx_ = -1; 

		bool checkStringValidity(std::string s);

	public:
		/// @brief creates an empty texture object
		TextureObject() {};
		/// @brief creates a texture object based on a CityJSON formatted texture object
		TextureObject(const json& textureJson);

		/// @brief sets the idx of an material object (do not use)
		void setIdx(int idx) { idx_ = idx; }

		/// @brief returns name of the texture object, "" if no name
		std::string getName() { return name_; }
		/// @brief returns true if texture object has a name
		bool hasName();

		/// @brief returns type of the texture object, "" if no name
		std::string getType() { return type_; }
		/// @brief returns true if texture object has a type
		bool hasType();

		/// @brief returns image path of the texture object, "" if no name
		std::string getImage() { return image_; }
		/// @brief returns true if texture object has a image path
		bool hasImage();

		/// @brief returns wrapmode of the texture object, "" if no name
		std::string getWrapmode() { return wrapMode_; }
		/// @brief returns true if texture object has a wrapmode
		bool hasWrapmode();

		/// @brief returns the border color of the texture object, populated with -1 if no data is present
		std::array<float, 3> getBorderColor() { return borderColor_; }
		/// @brief returns true if texture object has a border color
		bool hasBorderColor();
	};

	class AppearanceObject
	{
	private:
		std::vector<MaterialObject> materials_ = {};
		std::vector<TextureObject> textures_ = {};
		std::map<std::string, TextureObject> verticesTextures_ = {};
		std::string defaultThemeTexture_ = "";
		std::string defaultThemeMaterial_ = "";
	public:
		/// @brief adds a material object to the appearance object
		void addMaterial(MaterialObject obb);
		/// @brief adds a texture object to the appearance object
		void addTexture(TextureObject obb);
		/// @brief returns all the stored material objects
		std::vector<MaterialObject> getMaterials() { return materials_; }
		std::vector<MaterialObject*> getMaterialsPtr();
		/// @brief returns all the stored texture objects
		std::vector<TextureObject> getTexures() { return textures_; }
		std::vector<TextureObject*> getTexuresPtr();
		/// @brief returns a material object based on its idx
		MaterialObject getMaterial(int idx) { return materials_[idx]; }
		MaterialObject* getMaterialPtr(int idx) { return &materials_[idx]; }
		/// @brief returns a materail object based on its name
		MaterialObject getMaterial(const std::string& name);
		/// @brief returns the total number of stored materials
		size_t getMaterialSize() { return materials_.size(); }
		/// @brief returns the total number of stored textures
		size_t getTextureSize() { return textures_.size(); }
	};


	class PointOfContactObject
	{
	private:
		std::string contactName_ = "";
		PoC_type contactType_ = PoC_type::none;
		PoC_role role_ = PoC_role::none;
		std::string phone_ = "";
		std::string website_ = "";
		std::string address_ = "";
		json additionalData_ = {};
	public:
		PointOfContactObject() {};

		PointOfContactObject(
			const std::string& contactName,
			const PoC_type& contactType,
			const PoC_role& role,
			const std::string& phone,
			const std::string& website,
			const std::string& address
			);

		PointOfContactObject(const json& pointOfContact);

		/// @brief returns data in CityJSON format
		json getData();
		/// @brief returns contactName data
		std::string getContactName() { return contactName_; }
		/// @brief sets the contactNama data
		void setConactName(std::string contactName) { contactName_ = contactName; }
		/// @brief returns contactType data
		PoC_type getContactType() { return contactType_; }
		/// @brief sets the conactType data
		void setConactType(PoC_type contactType) { contactType_ = contactType; }
		/// @brief returns role data
		PoC_role getRole() { return role_; }
		/// @brief set role data
		void setRole(PoC_role role) { role_ = role; }
		/// @brief returns phone data
		std::string getPhone() { return phone_; }
		/// @brief set phone data
		void setPhone(std::string phoneNr) { phone_ = phoneNr; }
		/// @brief returns website data
		std::string getWebsite() { return website_; }
		/// @brief set website data
		void setWebsite(std::string url) { website_ = url; }
		/// @brief returns address data
		std::string getAddress() { return address_; }
		/// @brief set address data
		void setAddress(std::string address) { address_ = address; }
		/// @brief returns additional stored data
		json getAdditionalData() { return additionalData_; }
	};


	class metaDataObject 
	{
	private:
		std::tuple<CJTPoint, CJTPoint> geographicalExtent_ = { CJTPoint(0,0,0), CJTPoint(0,0,0) };
		std::string identifier_ = "";
		PointOfContactObject pointOfContact_ = PointOfContactObject();
		std::string referenceDate_ = "";
		std::string referenceSystem_ = "";
		std::string title_ = "";
		json additionalData_ = {};

	public:
		/// creates an empty metadata object
		metaDataObject() {};
		/// creates a metadata object
		metaDataObject(
			const std::tuple<CJTPoint, CJTPoint>& geographicalExtent,
			const std::string& identifier,
			const PointOfContactObject& pointOfContact,
			const std::string& referenceDate,
			const std::string& referenceSystem,
			const std::string& title,
			const std::map<std::string, std::string>& additionalData = {}
		);
		/// @brief creates a metadata object based on a CityJSON formatted metadataObject
		metaDataObject(const json& metaData);

		/// @brief returns data in CityJSON format
		json getData();

		/// @brief returns geographicalExtent data
		std::tuple<CJTPoint, CJTPoint> getExtend() { return geographicalExtent_; }
		/// @brief returns point of geographicalExtent, 0 == lll, 1 == rrr
		CJTPoint getExtend(int idx);
		/// @brief sets geographicalExtent data
		void setExtend(CJTPoint minPoint, CJTPoint maxPoint);
		/// @brief returns identifier data
		std::string getIdentifier() { return identifier_; }
		/// @brief sets identifier data
		void setIdentifier(std::string id) { identifier_ = id; }
		/// @brief returns contact information object
		PointOfContactObject getPointOfContact() { return pointOfContact_; }
		PointOfContactObject* getPointOfContactPtr() { return &pointOfContact_; }
		/// @brief sets contact information object
		void setPointOfcContact(const PointOfContactObject& pointOfContact) { pointOfContact_ = pointOfContact; }
		/// @brief returns referenceDate information
		std::string getReferenceDate() { return referenceDate_; }
		// @brief sets referenceData informaton
		void setReferenceData(std::string date) { referenceDate_ = date; }
		/// @brief returns referenceSystem information
		std::string getReferenceSystem() { return referenceSystem_; }
		/// @brief sets referenceSystem information
		void setReferenceSystem(std::string sys) { referenceSystem_ = sys; }
		/// @brief returns title information
		std::string getTitle() { return title_; }
		/// @brief sets title information
		void setTitle(std::string title) { title_ = title; }
		/// @brief returns additional stored data
		json getAdditionalData() { return additionalData_; }
		json* getAdditionalDataPtr() { return &additionalData_; }
		/// @brief adds additional stored data
		void addAdditionalData(json addData, bool overRide);
		/// @brief removes additional stored data at key
		void removeAdditionalData(std::string keyName);
		/// @brief returns true if data is stored in object
		bool checkInfo();
	};


	class GeoObject
	{
	private:
		json boundaries_;
		std::string lod_;
		json surfaceData_;
		json surfaceTypeValues_;
		std::string type_;
		int id_ = -1;

	public:
		/// @brief creates a new geoObject
		GeoObject(
			json boundaries,
			std::string lod,
			json surfaceData,
			json surfaceTypeValues,
			std::string type
		);
		/// @brief creates a new geoObject
		GeoObject(
			json boundaries,
			std::string lod,
			std::string type
		);
		/// @brief creates a new empty geoObject
		GeoObject() {};

		/// @brief returns the LOD
		std::string getLoD() { return lod_; }
		/// @brief sets the LoD
		void setLoD(std::string lod) { lod_ = lod; }
		/// @brief returns the type of the geometry object
		std::string getType() { return type_; }
		/// @brief returns the boundaries of the geoObject in CityJSON format
		json getBoundaries() { return boundaries_; };
		/// @brief sets the boundaries of the geoObject
		void setBoundaries(json boundaries) { boundaries_ = boundaries; }
		/// @brief returns the surface data in CityJSON fomat
		json getSurfaceData() { return surfaceData_; }
		/// @brief clears the surface data objects, if clearTypes - true, also the type values will be cleared
		void clearSurfaceData(bool clearTypes = false);
		/// @brief adds surface data to geoObject
		void appendSurfaceData(std::map<std::string, std::string> surfaceData);
		/// @brief adds surface data to geoObject
		void appendSurfaceData(std::map <std::string, int> surfaceData);
		/// @brief adds surface data to geoObject
		void appendSurfaceData(std::map <std::string, double> surfaceData);
		/// @brief replaces surface data
		void setSurfaceData(std::vector<json> surfaceData);
		/// @brief replaces surface data
		void setSurfaceData(json);
		/// @brief returns the surface values in CityJSON format
		json getSurfaceTypeValues() { return surfaceTypeValues_; }
		/// @brief set the surface value at a certain indx
		void setSurfaceTypeValue(int idx, int ref);
		/// @brief set the surface values
		void setSurfaceTypeValues(std::vector<int> references);
		/// @brief append a Surface Type value to the list
		void appendSurfaceTypeValue(int ref);
		/// @brief returns the id
		int getId() { return id_; }
		/// @brief sets the id
		void setId(int id) { id_ = id; }
	};


	class CityObject
	{
	private:
		std::string name_ = "";
		Building_Type type_;

		std::vector<GeoObject> geometry_;

		bool hasGeo_ = false;
		bool isParent_ = false;
		bool isChild_ = false;
		bool hasAttributes_ = false;

		json attributes_ = {};
		std::vector<std::string> parentList_ = {};
		std::vector<std::string> childList_ = {};

	public:
		CityObject();

		/// @brief construct a simple cityobject
		CityObject
		(
			const std::string& name,
			const Building_Type& type
		);

		/// @brief construct a cityobject 
		CityObject
		(
			const std::string& name,
			const Building_Type& type,
			const json& attributes,
			const std::vector<std::string>& parentList,
			const std::vector<std::string>& childList
		);

		/// @brief returns cityObject's name
		std::string getName() { return name_; }
		/// @brief set the name of the cityObject
		void setName(const std::string& name) { name_ = name; }
		/// @brief returns cityObject's type
		Building_Type getType() { return type_; }
		/// @brief returns a cityObject's type as string
		std::string getTypeString();
		/// @brief set the type of the cityObject
		void setType(const Building_Type& type) { type_ = type; }

		/// @brief returns true if the cityobject has one or more attributes
		bool hasAttributes() { return hasAttributes_; }
		/// @brief returns the attributes of the cityobject in a json format
		json getAttributes() { return attributes_; }

		/// @brief returns the LoD 
		std::vector<float> getLoD();
		
		/// @brief adds an attribute to the cityObject
		template <typename T>
		void addAttribute(const std::string& key, const T& value, bool overwrite = false);
		/// @brief adds multiple attributes of the same type to the cityObject
		template <typename T>
		void addAttribute(const std::map<std::string, T>& keyValueList, bool overwrite = false);
		/// @brief removes attribute with given keyName
		void removeAttribute(const std::string& keyName);
		/// @brief removes attribute with given keyName
		void removeAttribute(const std::vector<std::string>& keyNames);
		/// @brief removes all attributes
		void clearAttributes();

		/// @brief returns true if object has geometry
		bool hasGeo() { return hasGeo_; }
		/// @brief adds geo object to the cityObject
		void addGeoObject(const GeoObject& geom);
		/// @brief returns all geoObjects
		std::vector<GeoObject> getGeoObjects() { return geometry_; }
		std::vector<GeoObject*> getGeoObjectsPtr();
		/// @brief returns all geoObjects with the supplied LoD
		std::vector<GeoObject> getGeoObjects(const std::string& lod);
		std::vector<GeoObject*> getGeoObjectsPtr(const std::string& lod);

		/// @brief adds parent child relationship to the CityObject, does also update the parentObject
		void addParent(CityObject* parentObject);
		/// @brief set the parent child relationships to the cityobject, does NOT update the parentObjects
		void setParent(const std::vector<std::string>& parentNames);
		/// @brief removes the parent chiled relationship from the CityObject, does also update the parentObject
		void removeParent(CityObject* removeableObject);
		/// @brief returns a list of all the parents
		const std::vector<std::string>& getParents() { return parentList_; }
		/// @brief returns a list of all the parents
		std::vector<std::string>* getParentsPtr() { return &parentList_; }
		/// @brief adds a child parent relationship to the CityObject, does also update the childObject
		void addChild(CityObject* childObject);
		/// @brief set the child parent relationships to the cityobject, does NOT update the childObjects
		void setChild(const std::vector<std::string>& childNames);
		/// @brief removes the child parent relationship from the CityObject, does also update the childObject
		void removeChild(CityObject* removeableObject);
		/// @brief returns a list of all the children
		std::vector<std::string> getChildren() { return childList_; }
		/// @brief returns a list of all the children
		std::vector<std::string>* getChildrenPtr() { return &childList_; }
		/// @brief sets object geometry
		void setGeo(std::vector<GeoObject> geometry) { geometry_ = geometry; }
	};


	class CityCollection 
	{
	private:
		std::map<std::string, CityObject> cityObjects_ = {};
		std::vector<CJTPoint> vertices_ = {};
		std::string version_ = "";

		metaDataObject metaData_ = metaDataObject();
		ObjectTransformation objectTransformation_= ObjectTransformation(1);
		AppearanceObject appearance_ = AppearanceObject();

		bool isSilent_ = true;

		bool isValid(const json& jsonData);

		ObjectTransformation fetchTransformation(const json& transJson);
		std::vector<CJTPoint> fetchPoints(const json& pointJson);
		std::map<std::string, CityObject> fetchCityObjects(const json& cityObjects);
		AppearanceObject fetchAppearance(const json& AppearanceJson);

	public:
		/// @brief read a cityJSON file
		bool parseJSON(const std::string& filePath, bool silent);

		/// @brief dump to a cityJSON file
		bool dumpJson(const std::string& filePath);

		/// @brief returns all cityObjects
		std::vector<CityObject> getAllCityObject();
		/// @brief returns cityObject based on name
		bool containsCityObject(const std::string& obName);
		CityObject getCityObject(const std::string& obName);
		CityObject* getCityObjectPtr(const std::string& obName);
		/// @brief returns cityObjects based on name
		std::vector<CityObject> getCityObject(const std::vector<std::string>& obNameList);
		std::vector<CityObject*> getCityObjectPtr(const std::vector<std::string>& obNameList);
		/// @brief returns cityObjects based on type
		std::vector<CityObject> getCityObjectTyped(const Building_Type& typeName);
		std::vector<CityObject*> getCityObjectTypedPtr(const Building_Type& typeName);
		/// @brief adds a cityObject to the collection
		void addCityObject(const CityObject& cityObject);
		/// @brief removes a cityObject from the collection
		void removeCityObject(const std::string& obName);
		/// @brief returns all materials
		std::vector<MaterialObject> getMaterials() { return appearance_.getMaterials(); }
		std::vector<MaterialObject*> getMaterialsPtr() { return appearance_.getMaterialsPtr(); }
		/// @brief returns Materialobject based on idx
		MaterialObject getMaterial(int idx);
		MaterialObject* getMaterialPtr(int idx);
		/// @brief returns a list of materialobjects based on material name
		//std::vector<MaterialObject> getMaterial(std::string);

		/// @brief returns all the textures
		std::vector<TextureObject> getTextures() { return appearance_.getTexures(); }
		std::vector<TextureObject*> getTexturesPtr() { return appearance_.getTexuresPtr(); }
 
		/// @brief get version
		std::string getVersion() { return version_; }
		/// @brief set version
		void setVersion(const std::string& version) { version_ = version; }

		/// @brief returns all the vertices that are in the collection
		const std::vector<CJTPoint>& getVerices();
		/// @brief adds a vertex to the collection, returns idx location where point is placed
		int addVertex(const CJTPoint& point, bool checkUnique = false);
		///@brief adds a collection of vertices to the collection, returns idx location where points are placed
		const std::vector<int>& addVertex(const std::vector<CJTPoint>& pointList, bool checkUnique = false);
		/// @brief removes duplicate vertices in collection, only use this completely before or completely after geometry altering
		void cullDuplicatedVerices();
		/// @brief removes unreferenced vertices in collection, only use this completely before or completely after geometry altering
		void cullUnreferencedVerices();
		/// @brief cleans the vertex list in collection, only use this completely before or completely after geometry altering
		void CleanVertices();

		/// @brief returns the full metadata
		metaDataObject getMetaData() { return metaData_; }
		metaDataObject* getMetaDataPtr() { return &metaData_; }
		/// @brief sets the metadata of the collection
		void setMetaData(const metaDataObject& metaData) { metaData_ = metaData; }

		/// @brief returns the transformation of the collection
		ObjectTransformation getTransformation() { return objectTransformation_; }
		ObjectTransformation* getTransformationPtr() { return &objectTransformation_; }
		/// @brief sets the transfomation object of the collection
		void setTransformation(ObjectTransformation transformation) { objectTransformation_ = transformation; }

		/// @brief returns object to stop printing information about processes
		void silence() { isSilent_ = true; }
		/// @brief returns object to start printing information about processes
		void unSilence() { isSilent_ = false; }
	};

	template<typename T>
	inline void CityObject::addAttribute(const std::string& key, const T& value, bool overwrite)
	{
		if (attributes_.contains(key))
		{
			if (!overwrite)
			{
				std::cout << "key: " + key << " already in attributes of object " + name_ << std::endl;
				return;
			}
			attributes_[key] = value;

		}
		attributes_.emplace(key, value);
		hasAttributes_ = true;
	}
	template<typename T>
	inline void CityObject::addAttribute(const std::map<std::string, T>& keyValueList, bool overwrite)
	{
		for (auto pair = keyValueList.begin(); pair != keyValueList.end(); ++pair)
		{			
			addAttribute(pair->first, pair->second, overwrite);
		}
	}
}