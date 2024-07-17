#include <filesystem>
#include <iostream>

// Only CJT.h is required for the editing of attributes 
#include <CJT.h>


int main()
{
    // A city collection has to be created. This collection stores all the City(JSON) data in a easily accessable format
    CJT::CityCollection collection;

    /*
    The city collection can be populated with the parseJSON method. This function takes a path to the CityJSON fileand a boolean.
    The boolean dictates if the collection is silenced or not. If the function is silenced it will not communicate in any way.
    In this case the collection is not silenced, so information will be supplied by methods related to the collection.
    This can be disabled halfway by calling the silence() method, and reverted by the unsilence() method
    */
  
#ifdef _WIN32 
    std::string filepath = std::filesystem::current_path().remove_filename().string();
#endif // _WIN32

#ifdef __linux__
    std::string filepath = std::filesystem::current_path().string();
#endif // linux

    collection.parseJSON(filepath + "/twobuildings.city.json", false);


    /*
    A collection is populated by city objects, to change an attribute a city object has to be accessed.
    A city object can be selected by the name
    */
    std::shared_ptr<CJT::CityObject> cityObject1 = collection.getCityObjectPtr("Building_1");
    std::shared_ptr<CJT::CityObject> cityObject2 = collection.getCityObjectPtr("Building_2");

    /*
    Names can be easily be changed with the setName() function.
    The types can also be changed easily, make sure you look at the valid types in the standard: https://www.cityjson.org/spec
    The libary does not validate the used types
    */
    cityObject1->setName("Bridge_Building_1");
    cityObject1->setType(CJT::Building_Type::Bridge);

    /*
    Attributes can be easily added with the addAttribute() function.
    In this case we set the key to "CJT Edited?" and the value associated with this key to "Yes!".
    Instead of two strings a std::pair can also be used
    */
    cityObject1->addAttribute("CJT Edited?", "Yes!");
    cityObject2->addAttribute("CJT Edited?", "Yes!");

    /*
    The value can not be accidentally overwritten by adding another attribute with the same key.
    However if this is desirable behavior it can be force overwritten by adding a boolean true
    */
    cityObject2->addAttribute("CJT Edited?", "Naturally!", true);

    /*
    To see the results thefile can be written to json with the dumpJSON function
    */

    collection.dumpJson(filepath + "/twobuildings_exported_1.city.json");

    /*
    An attribute can be removed with the removeAttribute() function
    */
    cityObject1->removeAttribute("CJT Edited?");
    collection.dumpJson(filepath + "/twobuildings_exported_2.city.json");

    /*
    A parent child relationship can be created with the addParent() or addChild() function. 
    This will create the relationship in both the city object. 
    In this case cityObject1 will receive cityObject2 as parent and cityObject2 will receive cityObject1 as child
    */
    cityObject1->addParent(cityObject2);
    collection.dumpJson(filepath + "/twobuildings_exported_3.city.json");

    /*
    A parent child relation can be removed with the removeParent() or removeChild() function.
    This will remove the relationship in both the city object.
    In this case the child is removed from cityObject2 to show that cityObject1 also is updated even though the parent was originally added to cityObject1
    */
    cityObject2->removeChild(cityObject1);
    collection.dumpJson(filepath + "/twobuildings_exported_4.city.json");

    /*
    Changing the LoD of a city object is slightly more challeging because the LoD is not a city object attribute but an attribute of a geometry object.
    Every city object that has visable geometry has one, or more, geometry objects internalized.
    These can be accessed with the getGeoObjects() function, this function als takes a variable, allowing for filtering of the the geoobjects based on their LoD
    For every Geoobject the LoD can be easily edited with the setLoD() function
    */
    std::vector<std::shared_ptr<CJT::GeoObject>> geoObjectList = cityObject1->getGeoObjectsPtr();

    for (std::shared_ptr<CJT::GeoObject> geoObject : geoObjectList)
    {
        geoObject->setLoD("2.2");
    }
    collection.dumpJson(filepath + "/twobuildings_exported_5.city.json");
}

