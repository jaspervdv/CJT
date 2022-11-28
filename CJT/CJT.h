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
	class CityCollection;

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

		bool operator== (CJTPoint other);
		bool operator!= (CJTPoint other);
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
		MaterialObject() {};

		MaterialObject(json materialJson);

		MaterialObject(
			std::string name,
			float ambientIntensity,
			std::array<float, 3> diffuseColor,
			std::array<float, 3> emissiveColor,
			std::array<float, 3> specularColor,
			float shininess,
			float transparency,
			bool isSmooth
		);

		MaterialObject(
			std::string name,
			float ambientIntensity,
			std::array<float, 3> diffuseColor
		);

		MaterialObject(MaterialObject* other);

		/// @brief get name of the materal object, "" if no name
		std::string getName() { return name_; }
		/// @brief returns true if material object has a name
		bool hasName();

		/// @brief get the ambient intensity of the materal object, -1 if no data is present
		float getAmbientIntensity() { return ambientIntensity_; }
		/// @brief returns true if material object has an ambient intensity
		bool hasAmbientIntensity();

		/// @brief get the diffuse color of the materal object, populated with -1 if no data is present
		std::array<float, 3> getDiffuseColor(){ return diffuseColor_; }
		/// @brief returns true if material object has a diffuse color
		bool hasDiffuseColor();

		/// @brief get the emmisve color of the materal object, populated with -1 if no data is present
		std::array<float, 3> getEmissiveColor() { return emissiveColor_; }
		/// @brief returns true if material object has an emmisve color
		bool hasEmissiveColor();

		/// @brief get the specular color of the materal object, populated with -1 if no data is present
		std::array<float, 3> getSpecularColor() { return specularColor_; }
		/// @brief returns true if material object has a specular color
		bool hasSpecularColor();

		/// @brief get the shininess of the material object, -1 if no data is present
		float getShininess() {return shininess_;}
		/// @brief returns true if material object has a shininess value
		bool hasShininess();

		/// @brief get the transparancy of the material object, -1 if no data is present
		float getTransparency() { return transparency_; }
		/// @brief returns true if material object has a Transparency value
		bool hasTransparency();

		/// @brief get if material object is smooth
		bool getIsSmooth() { return isSmooth_; }

		/// @brief get the index of the object (location in list), -1 is no data is present
		int getIdx() { return idx_; }

		/// @brief set the index of the object
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
		TextureObject() {};

		TextureObject(json textureJson);

		void setIdx(int idx) { idx_ = idx; }

		/// @brief get name of the texture object, "" if no name
		std::string getName() { return name_; }
		/// @brief returns true if texture object has a name
		bool hasName();

		/// @brief get type of the texture object, "" if no name
		std::string getType() { return type_; }
		/// @brief returns true if texture object has a type
		bool hasType();

		/// @brief get image path of the texture object, "" if no name
		std::string getImage() { return image_; }
		/// @brief returns true if texture object has a image path
		bool hasImage();

		/// @brief get wrepmode of the texture object, "" if no name
		std::string getWrapmode() { return wrapMode_; }
		/// @brief returns true if texture object has a wrapmode
		bool hasWrapmode();

		/// @brief get the border color of the texture object, populated with -1 if no data is present
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
		void addMaterial(MaterialObject obb);
		void addTexture(TextureObject obb);

		std::vector<MaterialObject> getMaterials() { return materials_; }
		std::vector<TextureObject> getTexures() { return textures_; }
		MaterialObject getMaterial(int idx) { return materials_[idx]; }
		MaterialObject getMaterial(std::string name);
		size_t getMaterialSize() { return materials_.size(); }

		size_t getTextureSize() { return textures_.size(); }
	};


	class PointOfContactObject
	{
	private:
		std::string contactName_ = "";
		std::string contactType_ = "";
		std::string role_ = "";
		std::string phone_ = "";
		std::string website_ = "";
		std::string address_ = "";
		json additionalData_ = {};
	public:
		PointOfContactObject() {};

		PointOfContactObject(
			std::string contactName,
			std::string contactType,
			std::string role,
			std::string phone,
			std::string website,
			std::string address
			);

		PointOfContactObject(json pointOfContact);

		/// @brief get data in CityJSON format
		json getData();
		/// @brief get contactName data
		std::string getContactName() { return contactName_; }
		/// @brief get contactType data
		std::string getContactType() { return contactType_; }
		/// @brief get role data
		std::string getRole() { return role_; }
		/// @brief get phone data
		std::string getPhone() { return phone_; }
		/// @brief get website data
		std::string getWebsite() { return website_; }
		/// @brief get address data
		std::string getAddress() { return address_; }
		/// @brief get additional stored data
		json getAdditionalData() { return additionalData_; }
	};


	class metaDataObject 
	{
	private:
		std::tuple<CJTPoint, CJTPoint> geographicalExtent_ = { CJTPoint(0,0,0), CJTPoint(0,0,0) };
		std::string identifier_ = "";
		PointOfContactObject pointOfContact_ ;
		std::string referenceDate_ = "";
		std::string referenceSystem_ = "";
		std::string title_ = "";
		json additionalData_ = {};

	public:
		metaDataObject() {};

		metaDataObject(
			std::tuple<CJTPoint, CJTPoint> geographicalExtent,
			std::string identifier,
			PointOfContactObject pointOfContact,
			std::string referenceDate,
			std::string referenceSystem,
			std::string title,
			std::map<std::string, std::string> additionalData = {}
		);

		metaDataObject(json metaData);

		/// @brief get data in CityJSON format
		json getData();

		/// @brief get geographicalExtent data
		std::tuple<CJTPoint, CJTPoint> getExtend() { return geographicalExtent_; }
		/// @brief get point of geographicalExtent, 0 == lll, 1 == rrr
		CJTPoint getExtend(int idx);
		/// @brief get identifier data
		std::string getIdentifier() { return identifier_; }
		/// @brief get contact information object
		PointOfContactObject* getPointOfContact() { return &pointOfContact_; }
		/// @brief get referenceDate information
		std::string getReferenceDate() { return referenceDate_; }
		/// @brief get referenceSystem information
		std::string getReferenceSystem() { return referenceSystem_; }
		/// @brief get title information
		std::string getTitle() { return title_; }
		/// @brief get additional stored data
		json getAdditionalData() { return additionalData_; }
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
		void setBoundaries(json boundaries) { boundaries_ = boundaries; }
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
		void addAttribute(std::map<std::string, T> keyValueList);
		/// @brief removes attribute with given keyName
		void removeAttribute(std::string keyName);
		/// @brief removes attribute with given keyName
		void removeAttribute(std::vector<std::string> keyNames);
		/// @brief removes all attributes
		void clearAttributes();

		/// @brief returns true if object has geometry
		bool hasGeo() { return hasGeo_; }
		/// @brief add geo object to the cityObject
		void addGeoObject(GeoObject geom) { geometry_.emplace_back(geom); hasGeo_ = true; }
		/// @brief get all geoObjects
		std::vector< GeoObject> getGeoObjects() { return geometry_; }

		/// @brief add a parent relationship to the CityObject, second input is optional validation of name
		void addParent(std::string parentName, CityCollection* citycoll = nullptr);
		/// @brief get a list of all the parents
		std::vector<std::string> getParents() { return parentList_; }
		/// @brief add a child reationship to the CityObject, second input is optional validation of name
		void addChild(std::string childName, CityCollection* citycoll = nullptr);
		/// @brief get a list of all the children
		std::vector<std::string> getChildren() { return childList_; }
		/// @brief set object geometry
		void setGeo(std::vector<GeoObject> geometry) { geometry_ = geometry; }
	};


	class CityCollection 
	{
	private:
		std::map<std::string, CityObject> cityObjects_;
		std::vector<CJTPoint> vertices_;
		std::string version_ = "";

		metaDataObject metaData_;
		ObjectTransformation objectTransformation_;
		AppearanceObject appearance_;

		bool isSilent_ = true;

		bool isValid(json jsonData);

		ObjectTransformation fetchTransformation(json* transJson);
		std::vector<CJTPoint> fetchPoints(json* pointJson);
		std::map<std::string, CityObject> fetchCityObjects(json* cityObjects);
		AppearanceObject fetchAppearance(json* AppearanceJson);

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
		std::vector<CityObject*> getCityObjectTyped(std::string typeName);
		/// @brief get all materials
		std::vector<MaterialObject> getMaterials() { return appearance_.getMaterials(); }
		/// @brief get Materialobject based on idx
		MaterialObject getMaterial(int idx);
		/// @brief get a list of materialobjects based on material name
		//std::vector<MaterialObject> getMaterial(std::string);

		/// @brief get all the textures
		std::vector<TextureObject> getTextures() { return appearance_.getTexures(); }
 
		/// @brief returns all the vertsices that are in the collection
		std::vector<CJTPoint> getVerices();
		/// @brief remove duplicate vertices in collection, only use this completely before or completely after geometry altering
		void cullDuplicatedVerices();
		/// @brief remove unreferenced vertices in collection, only use this completely before or completely after geometry altering
		void cullUnreferencedVerices();
		/// @brief clean the vertex list in collection, only use this completely before or completely after geometry altering
		void CleanVertices();

		/// @brief get the full metadata
		metaDataObject* getMetaData() { return &metaData_; }
			
		bool setMetaData(metaDataObject metaData) { metaData_ = metaData; }
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
	inline void CityObject::addAttribute(std::map<std::string, T> keyValueList)
	{
		for (auto pair = keyValueList.begin(); pair != keyValueList.end(); ++pair)
		{			
			addAttribute(pair->first, pair->second);
		}
	}
}