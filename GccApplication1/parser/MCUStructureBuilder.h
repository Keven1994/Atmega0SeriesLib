#include <utility>

//
// Created by keven on 13.03.2019.
//

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cassert>

namespace utils {

    [[nodiscard]] static inline bool contains(const std::string& container,const char* sub){
        return container.find(sub) != std::string::npos;
    }

    template<typename T1 = std::string, typename T2 = std::string>
    struct tuple {
        tuple() = default;

        T1 str1;
        T2 str2;
        tuple(T1&& t1,T2&& t2) : str1(t1), str2(t2) {}
        tuple(const T1& t1,const T2& t2) : str1(t1), str2(t2) {}
    };

    template<typename T1 = std::string, typename T2 = std::string, typename T3 = std::string>
    struct triple {
        T1 str1;
        T2 str2;
        T3 str3;
    };

    [[nodiscard]] static inline std::string toLowerCase(std::string trans) {
        std::transform(std::begin(trans), std::end(trans), std::begin(trans), tolower);
        return trans;
    }

    [[nodiscard]] static inline std::string toHigherCase(std::string trans) {
        std::transform(std::begin(trans), std::end(trans), std::begin(trans), toupper);
        return trans;
    }

    [[nodiscard]] static inline std::string toCamelCase(std::string trans) {
        trans = toLowerCase(trans);
        std::transform(std::begin(trans), ++std::begin(trans), std::begin(trans), toupper);
        return trans;
    }

    [[nodiscard]] int find(const std::vector<std::string>& container, std::string str){
        for(int i = 0; static_cast<unsigned long long>(i) < container.size(); i++){
            if(utils::toLowerCase(container[i]) == utils::toLowerCase(str)) return i;
        }
        return -1;
    }

    [[nodiscard]] bool contains(const std::vector<std::string>& container, std::string str){
        return find(container,str) >= 0;
    }
}


namespace details {

    struct generatable {

        virtual std::string generate() noexcept = 0;
        virtual void style(std::string& val) noexcept {
            val += '\n';
        }
        virtual ~generatable() = default;
    };

    enum special {
        Data, Flag, Toggle, Control
    };

    class portAlias : public  generatable {

    public:

        portAlias() noexcept {

        }

        //using pin ## number = AVR::port::details::PortPin<P,number>
        [[nodiscard]] std::string generate() noexcept override {
           return "template<typename P>\nusing port = AVR::port::details::Port<P,portComponent::registers>;";
        }
    };

    class Reg : public generatable {
        static inline constexpr auto reg_str = "utils::Pair<reg::Register<";
        static inline constexpr auto R_str = "reg::accessmode::ReadOnly";
        static inline constexpr auto RW_str = "reg::accessmode::RW";
        static inline constexpr auto data_str = "reg::specialization::Data";
        static inline constexpr auto toggle_str = "reg::specialization::Toggle";
        static inline constexpr auto flag_str = "reg::specialization::Flag";
        static inline constexpr auto ctrl_str = "reg::specialization::Control";
        std::string name, protection, offset, values, specStr;
        int size;
        special spec;

        [[nodiscard]] std::string resolveString(special spec) {
            switch (spec) {
                case Data :
                    return data_str;
                case Flag :
                    return flag_str;
                case Toggle :
                    return toggle_str;
                case Control :
                    return ctrl_str;
                default:
                    return "";
            }
        }

        [[nodiscard]] std::string resolveString(int size) noexcept {
            switch (size) {
                case 1 :
                    return "uint8_t";
                case 2 :
                    return "uint16_t";
                case 4 :
                    return "uint32_t";
                case 8 :
                    return "uint64_t";
                default:
                    return "";
            }
        }

    public:


        Reg(std::string &&name, std::string &&protection, std::string &&offset, std::string &&size,
            std::string &&values, special spec) noexcept :
                name(name), protection((protection == "R" ? R_str : RW_str)), offset(offset),
                values((values.empty() ? "void" : values)), specStr(resolveString(spec)), spec(spec) {
            try {
                this->size = std::stoi(size);
            } catch (std::exception &e) {
                std::cerr << __PRETTY_FUNCTION__ << " exception: " << e.what() << '\n';
                size = 1;
            }
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
        void style(std::string& val) noexcept override {

        }
#pragma GCC diagnostic pop

        [[nodiscard]] std::string generate() noexcept override {
            std::string tmp = "using " + utils::toLowerCase(name) + " = " + reg_str + protection + "," + specStr;
            std::string size_str = resolveString(size);
            if (spec == Data || spec == Toggle) {
                if (size == 1) {
                    tmp += ">";
                } else {
                    tmp += ",void," + size_str + ">";
                }
            } else {
                if (size == 1) {
                    tmp += "," + values + ">";
                } else {
                    tmp += "," + values + "," + size_str + ">";
                }
            }
            tmp += "," + offset + ">;";
            return tmp;
        }

    };

    class TypePin : public generatable {

        std::string port, number, count;

    public:

        TypePin(std::string &&port, std::string &&number, std::string&& count) noexcept : port(utils::toHigherCase(port)), number(number), count(count) {

        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
        void style(std::string& val) noexcept override {

        }
#pragma GCC diagnostic pop
        //using pin ## number = AVR::port::details::PortPin<P,number>
        [[nodiscard]] std::string generate() noexcept override {
            std::string tmp = "using pin" + count + " = AVR::port::details::PortPin<port_details::port<port_details::ports::port"+utils::toLowerCase(port)+">,"+number+">;";
            return tmp;
        }

    };

    class Function : public generatable {
        std::string attributes, returnType, name, params, cvQualifier, content;
    public:
        Function(std::string &&attributes, std::string &&returnType, std::string &&name,
                 std::string &&params, std::string &&cvQualifier, std::string &&content) : attributes(
                std::move(attributes)),
                                                                                           returnType(returnType),
                                                                                           name(name),
                                                                                           params(params),
                                                                                           cvQualifier(cvQualifier),
                                                                                           content(content) {}

        void changeContent(const std::string &content) {
            this->content = content;
        }

        std::string generate() noexcept override {
            return attributes + " " + returnType + " " + name + "(" + params + ") " + cvQualifier + " { " + content +
                   "}";
        }

    };

    class Enum : public generatable {
        std::string name, type;
        std::string entrys{};

    public:
        Enum(std::string &&name, std::string &&type) noexcept : name(name), type(type) {}

        void addEntry(const std::string &name, const std::string &value) noexcept {

            if (!entrys.empty())
                entrys += ",\n";
            entrys += "    " + name;
            entrys += " = " + value;
        }

        [[nodiscard]] std::string generate() noexcept override {
            std::string tmp = "enum class " + name + " : " + type + " {\n";
            tmp += entrys;
            tmp += "\n};";
            return tmp;
        }
    };

    class Struct : public generatable {

        std::string mname;
        std::string mentrys{};

        void addEntry(const std::string &name, const std::string &value) noexcept {

            if (!mentrys.empty())
                mentrys += "\n";
            mentrys += "    " + name;
            mentrys += " = " + value + ";";
        }

    public:
        explicit Struct(std::string &&name) noexcept : mname(name) {}

        void addStatic(std::string &&type, std::string &&name, std::string &&value, bool constex = false) {
            std::string prefix;
            if (!constex)
                prefix = "static inline ";
            else {
                prefix = "static constexpr ";
            }
            addEntry((prefix + type + " " + name), value);
        }

        virtual void addTypeAlias(std::string&& name, std::string&& type){
            addEntry("using "+name,type);
        }

        virtual void addMember(generatable &gen) noexcept {
            if (!mentrys.empty())
                mentrys += "\n";
            auto tmp = "    " + gen.generate();

            for (uint32_t i = 0; i < tmp.size(); i++) {
                if (tmp[i] == '\n')
                    tmp.insert(i + 1, "    ");
            }
            gen.style(tmp);
            mentrys += tmp;
        }

        virtual void addMember(std::string &&gen) noexcept {
            if (!mentrys.empty())
                mentrys += "\n";
            auto tmp = "    " + gen;

            for (uint32_t i = 0; i < tmp.size(); i++) {
                if (tmp[i] == '\n')
                    tmp.insert(i + 1, "    ");
            }
            mentrys += tmp;
        }

        [[nodiscard]] std::string generate() noexcept override {
            std::string tmp = "struct " + mname + " {\n";
            tmp += mentrys;
            tmp += "\n};";
            return tmp;
        }

        [[nodiscard]] inline const std::string & name() const noexcept { return mname; }

        [[nodiscard]] inline const std::string &entrys() const noexcept { return mentrys; }
    };

    class Namespace : public Struct {

    public:
        Namespace(std::string &&name) : Struct(std::move(name)) {}

        [[nodiscard]] std::string generate() noexcept override {
            std::string tmp = "namespace " + name() + " {\n";
            tmp += entrys();
            tmp += "\n}";
            return tmp;
        }
    };

    class Declaraction : public generatable {
        std::string str = "template<";

    public:
        Declaraction(std::string&& name, std::string&& paramtype, std::string&& paramname, std::string&& dummyname, std::string paramVal = ""){
            str += paramtype;
            str += " " +paramname;
            str += " " +paramVal +",bool " + dummyname + " = true>\nstruct ";
            str += name;
            str += ";";
        }

        [[nodiscard]] std::string generate() noexcept override {
            return str;
        }
    };

    class TemplateStruct : public Struct {
        std::string templatePraefix = "template<bool ";
        std::string special ="<";
    public:
        TemplateStruct(std::string&& name, std::string paramVal, std::string&& dummyname) : Struct(std::move(name)) {
            special += paramVal + ", " + dummyname + ">\n";
            templatePraefix+= dummyname +">\n";
        }

        [[nodiscard]] std::string generate() noexcept override {
            std::string tmp = templatePraefix+"struct " + Struct::name()+ special  + " {\n";
            tmp += Struct::entrys();
            tmp += "\n};";
            return tmp;
        }
    };
}


class MCUStructureBuilder {
    static inline constexpr auto header = "#pragma once\n #include \"../../hw_abstractions/Port.hpp\"\n #include \"../../tools/meta.h\"\n\n";
    static inline auto f = details::Function("[[nodiscard,gnu::always_inline]] static inline","auto&","value","","","" );

    std::string deviceName;
    std::string compname;
    std::vector<std::string> registers{};
    details::Struct compStruct, regs;
    unsigned int highestOffset = 0;
    details::Namespace nameSpace;
    std::vector<details::Enum> enums;
    std::vector<details::Struct*> instances;
    bool multifunction = false;

    [[nodiscard]] int find(const std::vector<details::Struct>& container, std::string str){
        for(int i = 0; static_cast<unsigned long long>(i) < container.size(); i++){
            if(utils::toLowerCase(container[i].name()) == utils::toLowerCase(str)) return i;
        }
        return -1;
    }

    [[nodiscard]] int find(const std::vector<details::TemplateStruct>& container, std::string str){
        for(int i = 0; static_cast<unsigned long long>(i) < container.size(); i++){
            if(utils::toLowerCase(container[i].name()) == utils::toLowerCase(str)) return i;
        }
        return -1;
    }

    [[nodiscard]] int countDistinctFunctions(const std::vector<utils::triple<>>& container){
        int tmp = 0;
        std::vector<std::string> processed{};
        for (const auto &elem : container) {
            auto& search = elem.str1;
            if(!utils::contains(processed,search)) {
                tmp++;
                processed.push_back(search);
            }
        }
        return tmp;
    }

    [[nodiscard]] int countDistinctGroups(const std::vector<utils::triple<>>& container){
        int tmp = 0;
        std::vector<std::string> processed{};
        for (const auto &elem : container) {
            auto& search1 = elem.str2;
            if( ! utils::contains(processed,search1)) {
                tmp++;
                processed.push_back(search1);
            }
        }
        return tmp;
    }

    [[nodiscard]] bool sameRow13(const std::vector<utils::triple<>>& fgps, const std::string& str,const std::string& str2){
        for(auto& elem: fgps){
            if(utils::toLowerCase(elem.str1) == utils::toLowerCase(str) && utils::toLowerCase(elem.str3) == utils::toLowerCase(str2)) return true;
        }
        return false;
    }
    bool aliasGenerate = false;
public:
    using reg_type = details::special;

    explicit MCUStructureBuilder(std::string &&deviceName, std::string &&ComponentName)
    noexcept : deviceName(deviceName), compname(ComponentName), compStruct(utils::toLowerCase(compname) + "Component"),regs("registers"),
               nameSpace(deviceName.substr(2)), enums(),instances() {registers.reserve(500); for(auto i = 0 ; i < 500; i++)registers.push_back("");}

    void addRegister(std::string &&name, std::string &&protection, std::string &&offset, std::string &&size,
                     std::string &&values, reg_type type) noexcept {

        try {
            auto withoutX = "0"+offset.substr(2,offset.size());

            unsigned int temp = std::stoi(withoutX, 0, 16);
            assert(temp < 500);

            registers[temp] = name;
            highestOffset = (highestOffset < temp ? temp : highestOffset);

        } catch (std::exception& e){

            std::cerr << "offset string was not numeric! '\n";
        }

        auto tmp = details::Reg(std::move(name), std::move(protection), std::move(offset), std::move(size),
                                std::move(values), type);
        regs.addMember(tmp);
    }

    void addInstance(std::string& mname,std::string &&number) {

        f.changeContent("return "+mname+";");
        details::Struct* tmp = new details::TemplateStruct("inst",number, "dummy");
        auto decl = details::Declaraction("alt","auto","N","dummy1");
        tmp->addMember(decl);
        tmp->addMember(f);
        instances.push_back(tmp);
    }

    void addInstance(std::string& name) {

        f.changeContent("return "+name+";");
        details::Struct* tmp = new details::Struct(utils::toLowerCase(name));
        tmp->addMember(f);
        instances.push_back(tmp);
    }

    void addEnum(std::string &&name, std::string &&type = "mem_width") noexcept {
        auto tmp = details::Enum(std::move(name), std::move(type));
        enums.push_back(tmp);
    }

    void addEnumEntry(std::string &&name, std::string &&value) {
        enums.back().addEntry(name, value);
    }

    void addSignal(std::vector<utils::triple<>>& fgps, std::string&& modName, int inst) noexcept {

        if(utils::contains(modName,"PORT")) {
            aliasGenerate = true;
        }
        std::vector<utils::tuple<std::string,std::vector<std::string>>> groups;
        {
            std::vector<std::string> processed;
            //sorting pads in groups
            for (auto &ele : fgps) {
                const auto &search = ele.str2;
                if (!utils::contains(processed, search)) {
                    processed.push_back(search);
                    std::vector<std::string> temp{};
                    for (auto &elem : fgps) {
                        if (elem.str2 == search) {
                            temp.push_back(elem.str3);
                        }
                    }
                    groups.push_back(utils::tuple<std::string, std::vector<std::string>>{search, temp});
                }
            }
        }


        auto funcctr = countDistinctFunctions(fgps);
        auto grpc=countDistinctGroups(fgps);

        if(utils::contains(modName,"PORT")){ //special case when there is only 1 function (e.g. ports)
            auto tmpstr = modName;
            addInstance(tmpstr);
            std::vector<details::Struct> toAdd{};

            std::string typestr = "Meta::List<";
            for(auto& elem : groups){
                details::Struct temp = details::Struct("Pin");
                auto i = 0;
                for(auto& ele : elem.str2){
                    typestr += (i == 0 ? "" : ", ");
                    typestr += "pin"+std::to_string(i);
                    auto tmp = details::TypePin(std::string{ele[1]}, std::string{ele[2]}, std::to_string(i++));
                    temp.addMember(tmp);
                }
                toAdd.push_back(temp);
            }
            typestr+=">";
            for(auto& elem: toAdd){
                elem.addTypeAlias("list",typestr+"");
                instances[inst]->addMember(elem);
            }
        } else {
            multifunction = true;
            auto tmpstr = modName;
            addInstance(tmpstr,std::to_string(inst));
            std::vector<std::string> processed;
            std::vector<std::string> temp{};
            std::vector<utils::tuple<  details::TemplateStruct, std::string>> funcs{};
            for (auto &ele : fgps) {
                const auto &search = ele.str1;
                if (!utils::contains(processed, search)) {
                    processed.push_back(search);
                    funcs.emplace_back(details::TemplateStruct("alt",std::to_string(--funcctr), "dummy1"),search);
                }
            }
            std::vector<details::Struct> toAdd{};
            for(auto& elem : funcs){
                std::string typestr = "Meta::List<typename ";
                bool first = true;
                for(auto& ele : groups){
                    details::Struct group = details::Struct(utils::toCamelCase(ele.str1));
                    bool filled = false;
                    auto i = 0;
                    for(auto& el : ele.str2) {
                        if (sameRow13(fgps, elem.str2, el)) {
                            typestr += (first ? "" : ", typename ");
                            first = false;
                            typestr += group.name()+"::";
                            typestr += "pin"+std::to_string(i);
                            auto tmp = details::TypePin(std::string{el[1]}, std::string{el[2]}, std::to_string(i++));
                            group.addMember(tmp);
                            filled = true;
                        }
                    }
                    if(filled)
                        elem.str1.addMember(group);
                }
                typestr += ">";
                elem.str1.addTypeAlias("list",typestr+"");
                toAdd.push_back(elem.str1);
            }

            for(auto& func : funcs) {

                    instances[inst]->addMember(func.str1);

            }

        }
        //assert(funcctr >= 1);
    }

    void parse(const std::string& path) noexcept {
        std::string tmp = header;
        for (auto &elem: enums) {
            compStruct.addMember(elem);
        }
        for(unsigned int i = 0; i <= highestOffset; i++){
            if(registers[i].empty()){

                regs.addMember(std::string("volatile mem_width reserved")+std::to_string(i)+";");
            } else {

                regs.addMember(utils::toLowerCase(registers[i])+ "::type " +utils::toCamelCase(registers[i]) +";");
            }
        }
        compStruct.addMember(regs);
        details::Namespace ns{utils::toLowerCase(compname)+"_details"};
ns.addMember(compStruct);
        if(aliasGenerate){
            auto alias = details::portAlias{};
            ns.addMember(alias);
        }
        auto ins = details::Struct(utils::toLowerCase(compname)+"s");
        if(multifunction) {
            auto inst_decl = details::Declaraction("inst", "auto", "N", "dummy");
            ins.addMember(inst_decl);
        }
        for (details::Struct* elem : instances) {
            ins.addMember(*elem);
        }

        ns.addMember(ins);
        nameSpace.addMember(ns);
        tmp += nameSpace.generate();

        std::string outName = path + deviceName + compname + ".hpp";
        try {
            std::ofstream of(outName);
            of << tmp;
        } catch (std::exception &e) {
            std::cerr << __FILE__ << " " << __LINE__ << " could not parse: " << e.what();
        }
    }
};
