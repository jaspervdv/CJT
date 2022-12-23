

#include <iostream>
#include <CJT.h>
#include <CJToKernel.h>
#include <TopoDS_Solid.hxx>
#include <TopLoc_Location.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BOPAlgo_Splitter.hxx>
#include <STEPControl_Writer.hxx>
#include <BRepBuilderAPI_Transform.hxx>

int main()
{

    std::string filepath = "twobuildings.city.json";
    CJT::CityCollection collection;
    collection.parseJSON(filepath, false);
}
