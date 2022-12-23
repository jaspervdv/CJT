#include <filesystem>
#include <iostream>


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
    std::string filepath = std::filesystem::current_path().remove_filename().string();
    collection.parseJSON(filepath + "/twobuildings.city.json", false);

    /*
    A collection is populated by city objects, to change an attribute a city object has to be accessed. 
    A city object can be selected by the name
    */
    CJT::CityObject* cityObject = collection.getCityObject("Building_1");
    
    /*
    Attributes can be easily added with the addAttribute() function. 
    In this case we set the key to "CJT Edited?" and the value associated with this key to "Yes!".
    Instead of two strings a std::pair can also be used 
    */
    cityObject->addAttribute("CJT Edited?", "Yes!");

    /*
    The updated collection now has to be written back to a file.
    */
    collection.dumpJson(filepath + "/twobuildings_exported.city.json");
}

