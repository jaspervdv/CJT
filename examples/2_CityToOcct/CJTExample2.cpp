#include <filesystem>
#include <iostream>
#include <STEPControl_Writer.hxx>

// CJT.h is required for the reading of CityJSON data and the editing of attributes 
#include <CJT.h>

// CJToKernel is required for the conversion between the CityJSON format and the OpenCASCADE format
#include <CJToKernel.h>

int main()
{
    // As explained in example 1 a city collection has to be created.
    CJT::CityCollection collection;

    /*
    As explained in example 1 the city collection can be populated with the parseJSON method. 
    */
    std::string filepath = std::filesystem::current_path().remove_filename().string();
    collection.parseJSON(filepath + "/twobuildings.city.json", false);

    /*
    A kernel object has to be created to be able to convert OpenCASCADE geometry to CityJSON geometry.
    The kernel constructor requires a reference to the city collection object. 
    */
    CJT::Kernel kernel(&collection);

    /*
    An object that is desired for conversion to the OpenCASCADE format has to be isolated, this is explained in example 1
    */
    CJT::CityObject* cityObject1 = collection.getCityObject("Building_1");

    /*
    The object can be converted to the OpenCASCADE format with the convertToCascade function. This function accepts both a CJT city object or a geo object.
    The output is a TopoDS shape object. It will attempt to create a TopoDS_Solid, if this is not succesfull it will output a TopoDS_Composit. 
    */

    std::vector<TopoDS_Shape*> shapeList =  kernel.convertToCascade(*cityObject1);

    /*
    To visualize the output will be written to a STEP file
    */
    STEPControl_Writer writer;
    
    for (std::vector<TopoDS_Shape*>::iterator ptr = shapeList.begin(); ptr < shapeList.end(); ptr++)
    {
        TopoDS_Shape* geoObject = *ptr;
        writer.Transfer(*geoObject, STEPControl_AsIs);
    }

    std::string exportPath = filepath + "/twobuildings_step_export.step";
    writer.Write(exportPath.c_str());

}

