#include <iostream>
#include <string>
#include <ostream>
#include <vector>
#include <fstream>
#include "pugixml.hpp"
#include "MCUStructureBuilder.h"

std::string generateSomething(std::string&& lineStart, std::vector<std::string>&& member, std::string&& lineEnd){
    std::string tmp =lineStart;
    for(auto& elem : member){
        tmp+="    ";
        //auto linecounter = 0;
        for(uint32_t i = 0; i < elem.size(); i++){
            if(elem[i] == '\n')
                elem.insert(i+1,"    ");
        }
        tmp+=elem+'\n';
    }
    tmp += lineEnd;
    return tmp;
}

std::string generateNameSpace(std::string&& name,std::vector<std::string>&& member){
    return generateSomething("namespace "+name+" {\n\n", std::move(member),"}");
}

std::string generateStruct(std::string&& name,std::vector<std::string>&& member){
    return generateSomething("struct "+name+" {\n",std::move(member),"\n};");
}

std::string generateStaticVariable(std::string&& name,std::string&& type,std::string&& value){
    std::string tmp = "static inline ";
    tmp += type;
    tmp += (" " + name + " = " + value +";");
    return tmp;
}



int main() {
    /*
    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file("C:\\Users\\keven\\OneDrive\\Desktop\\ATmega4809.xml");
    if (!result)
        return -1;
    auto module = doc.select_nodes("/modules/module");
    auto reg = doc.select_nodes("/modules/module/register-group/register");
    for (auto node: module)
    {
        pugi::xml_node tool = node.node();
        std::cout << "Module Name " << tool.attribute("name").value() << "\n";
        for(auto node1: tool.child("register-group").children("register")){
            std::cout << "  name: " << node1.attribute("name").as_string() << " offset: "
            << node1.attribute("offset").as_string() << " protection: " << node1.attribute("rw").as_string() << " size: "
            << node1.attribute("size").as_string() <<"\n";

        }
    }*/

    MCUStructureBuilder builder("ATmega4808","Port");

    builder.addEnum("testEnum");
    builder.addEnumEntry("entry1","42");
    builder.addEnumEntry("entry2","21");
    builder.addEnum("testEnum2");
    builder.addEnumEntry("entry1","21");
    builder.addEnumEntry("entry2","42");
    using reg_type = typename MCUStructureBuilder::reg_type;
    builder.addRegister("reg1","R","0x42","1","testEnum",reg_type::Control);
    builder.addRegister("reg2","R","0x22","1","",reg_type::Data);
    builder.addInstance("PORTA");
    builder.addInstance("PORTC");
    builder.parse();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}