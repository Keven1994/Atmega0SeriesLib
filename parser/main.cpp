#include <iostream>
#include <string>
#include <ostream>
#include <vector>
#include <fstream>
#include "pugixml.hpp"
#include "MCUStructureBuilder.h"

std::string generateSomething(std::string &&lineStart, std::vector<std::string> &&member, std::string &&lineEnd) {
    std::string tmp = lineStart;
    for (auto &elem : member) {
        tmp += "    ";

        for (uint32_t i = 0; i < elem.size(); i++) {
            if (elem[i] == '\n')
                elem.insert(i + 1, "    ");
        }
        tmp += elem + '\n';
    }
    tmp += lineEnd;
    return tmp;
}

std::string generateNameSpace(std::string &&name, std::vector<std::string> &&member) {
    return generateSomething("namespace " + name + " {\n\n", std::move(member), "}");
}

std::string generateStruct(std::string &&name, std::vector<std::string> &&member) {
    return generateSomething("struct " + name + " {\n", std::move(member), "\n};");
}

std::string generateStaticVariable(std::string &&name, std::string &&type, std::string &&value) {
    std::string tmp = "static inline ";
    tmp += type;
    tmp += (" " + name + " = " + value + ";");
    return tmp;
}

#include <unordered_map>

struct stringtuple {
    std::string str1, str2;
};

int main(int argc, const char** argv) {
    using biter = uint8_t;
    using reg_type = typename MCUStructureBuilder::reg_type;
    auto map = std::unordered_map<std::string, MCUStructureBuilder>();
    pugi::xml_document doc;

    pugi::xml_parse_result result;
    if(argc > 1)
        result = doc.load_file(argv[1]);
    else result = doc.load_file("ATmega4809.atdf");

    if (!result)
        return -1;
    std::cout << "input file: " << argv[1] << '\n';
    std::string devname = doc.select_nodes("/devices/device").begin()->node().attribute("name").as_string();
    auto module = doc.select_nodes("/modules/module");
    auto reg = doc.select_nodes("/modules/module/register-group/register");
    std::string path;
#ifdef __linux__
    if(argc > 2) path = argv[2]+("/"+devname);
#elif _WIN32
    if(argc > 2) path = argv[2]+("\\"+devname);
#else
    static_assert(false,"OS not supported");
#endif
    else path = devname;
    std::cout << "output: " << path << '\n';
    for (auto node: module) {
        pugi::xml_node tool = node.node();
        std::string modName = tool.attribute("name").as_string();
        std::cout << "Module Name " << modName << "\n";
        MCUStructureBuilder mbuilder = MCUStructureBuilder(path+"", tool.attribute("name").as_string());
        for (auto node1: tool.child("register-group").children("register")) {
            std::string reg_name = node1.attribute("name").as_string();
            std::string reg_off = node1.attribute("offset").as_string();
            std::string reg_prot = node1.attribute("rw").as_string();
            std::string reg_size = node1.attribute("size").as_string();
            std::string reg_enum = reg_name + "Masks";
            reg_type reg_t;
            if (utils::contains(reg_name, "TGL")) {
                reg_t = reg_type::Toggle;
            } else if (utils::contains(reg_name, "CTRL")) {
                reg_t = reg_type::Control;
            } else if (utils::contains(reg_name, "FLAGS") || utils::contains(node1.attribute("caption").as_string(), "Status")) {
                reg_t = reg_type::Flag;
            } else {
                reg_t = reg_type::Data;
            }
            std::vector<stringtuple> val_group{};
            bool entryGenerated = false;
            for (auto node2 : node1.children("bitfield")) {
                if(reg_t == reg_type::Data){
                    if(reg_prot == "RW"){
                        reg_t = reg_type::Control;
                    } else {
                        reg_t = reg_type::Flag;
                    }
                }
                auto tempstr = std::string(node2.attribute("values").as_string());
                if (!entryGenerated) {
                    mbuilder.addEnum(reg_enum + "");
                    entryGenerated = true;
                }
                if (tempstr.empty()) {
                    if(node2.attribute("mask").as_string() == std::string("0xff")){
                        for(uint32_t i = 0; i < sizeof(biter)*8; i++){
                            mbuilder.addEnumEntry(utils::toCamelCase(node2.attribute("name").as_string()),
                                                  modName + "_" + node2.attribute("name").as_string()+ std::to_string(i) + "_bm");
                        }
                    } else
                        mbuilder.addEnumEntry(utils::toCamelCase(node2.attribute("name").as_string()),
                                              modName + "_" + node2.attribute("name").as_string() + "_bm");
                } else {
                    val_group.push_back(stringtuple{tempstr, node2.attribute("name").as_string()});
                }
            }

            for (auto &valstr : val_group) {
                std::string enumname = modName;
                enumname.append("_").append(valstr.str2).append("_enum").append("::").append(
                        modName + "_" + valstr.str2 + "_");
                for (auto node2 : tool.children("value-group")) {
                    if (node2.attribute("name").as_string() == valstr.str1) {
                        for (auto node3 : node2.children("value")) {
                            std::string valname = enumname + node3.attribute("name").as_string() + "_gc";
                            mbuilder.addEnumEntry(
                                    utils::toLowerCase(valstr.str2 + "_" + node3.attribute("name").as_string()),
                                    std::move(valname));
                        }
                    }
                }
            }
            mbuilder.addRegister(std::move(reg_name), std::move(reg_prot), std::move(reg_off), std::move(reg_size),
                                 std::move(reg_enum), reg_t);
        }
        mbuilder.parse();
    }

    std::cout << "Hello, World!" << std::endl;
    int z;
    std::cin >> z;
    return 0;
}