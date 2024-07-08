#include <filesystem>
#include <iostream>
#include <BRepPrimAPI_MakeBox.hxx>

// CJT.h is required for the reading of CityJSON data and the editing of attributes 
#include <CJT.h>

// CJToKernel is required for the conversion between the CityJSON format and the OpenCASCADE format
#include <CJToKernel.h>


int main()
{
	/*
	A TopoDS_Shape is required to convert to the CityJSON format.
	In this case a simple box shape is made.
	*/
	gp_Pnt anchor(0, 0, 0);
	Standard_Real size = 10;
	TopoDS_Shape boxShape = BRepPrimAPI_MakeBox(anchor, size, size, size).Shape();

	/*
	A new city collection has to be created to be able to store the shape in the CityJSON format
	*/
	std::shared_ptr<CJT::CityCollection> collection = std::make_shared<CJT::CityCollection>();
	
	/*
	This city collection requires a transformation object. (if not supplied it defaults to scale = uniform 1, translation = {0, 0, 0})
	An transformation object can be intinalized in multiple ways. 
	The easiest way is by supplying a double for the scaling of the object, this will set a uniform scale and set the origin at {0, 0, 0}.
	A translation can be added after by using the setTranslation() function.
	*/
	CJT::ObjectTransformation objectTransformation(0.001);
	objectTransformation.setTranslation(-size / 2, -size / 2, 0);
	collection->setTransformation(objectTransformation);
	collection->setVersion("1.1");

	/*
	Optionally metadata can be added to the collection via a metadata object.
	A metadata can have all its variables initialized in its constructor. 
	However to keep it clear we do for some variables step by step with the supplied methods. 
	Note that by not filling in all the metadata info (like in this example), this is not creating a valid CityJSON file. For more info: https://www.cityjson.org/specs
	*/
	CJT::metaDataObject metaData;
	metaData.setTitle("OpenCASCADE box");
	metaData.setExtend(CJT::CJTPoint(-5,-5,0), CJT::CJTPoint(5,5,0));

	/*
	The point of contact information can be added with a point of contact object.
	*/
	CJT::PointOfContactObject contactObject;
	contactObject.setConactName("User Name");
	contactObject.setConactType(CJT::PoC_type::individual);
	contactObject.setRole(CJT::PoC_role::user);
	contactObject.setWebsite("https://github.com/jaspervdv/CJT");

	metaData.setPointOfcContact(contactObject);
	collection->setMetaData(metaData);

	/*
	The collection now has a transformation and metadata, but it does not yet have the shape included.
	It can be converted to a geoobject with the help of the Kernel class.
	There is currently no way to set semantic data of the surfaces of a geo object
	*/
	CJT::Kernel kernel(collection);
	CJT::GeoObject geoObject = kernel.convertToJSON(boxShape, "1");

	/*
	A new city object has to be created to which the geo object can be added to.
	*/
	CJT::CityObject cityObject;
	cityObject.setName("Box");
	cityObject.addGeoObject(geoObject);
	cityObject.setType(CJT::Building_Type::Building);

	/*
	This city object can be added to the collection. and the collection can be dumped to a JSON.
	*/
	collection->addCityObject(cityObject);

	std::string filepath = std::filesystem::current_path().remove_filename().string();
	std::string exportPath = filepath + "/box_export.city.json";
	collection->dumpJson(exportPath);
}
