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

namespace utils {

    [[nodiscard]] static inline bool contains(const std::string& container,const char* sub){
        return container.find(sub) != std::string::npos;
    }

    template<typename T1 = std::string, typename T2 = std::string>
    struct tuple {
        T1 str1;
        T2 str2;
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
            std::string tmp = "using pin" + count + " = AVR::port::details::PortPin<port<ports::port"+utils::toLowerCase(port)+">,"+number+">;";
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

        void addMember(generatable &gen) noexcept {
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
}


class MCUStructureBuilder {
    static inline constexpr auto header = "#pragma once\n #include \"../../hw_abstractions/Port.hpp\"\n\n";
    static inline auto f = details::Function("[[nodiscard,gnu::always_inline]] static inline","auto&","value","","","" );

    std::string deviceName;
    std::string compname;
    details::Struct compStruct, regs;
    details::Namespace nameSpace;
    std::vector<details::Enum> enums;
    std::vector<details::Struct> instances;

    [[nodiscard]] int find(const std::vector<details::Struct>& container, std::string str){
        for(int i = 0; static_cast<unsigned long long>(i) < container.size(); i++){
            if(utils::toLowerCase(container[i].name()) == utils::toLowerCase(str)) return i;
        }
        return -1;
    }

    [[nodiscard]] int countDistinct(const std::vector<utils::triple<>>& container){
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
               nameSpace(deviceName.substr(2)), enums(),instances() {}

    void addRegister(std::string &&name, std::string &&protection, std::string &&offset, std::string &&size,
                     std::string &&values, reg_type type) noexcept {
        auto tmp = details::Reg(std::move(name), std::move(protection), std::move(offset), std::move(size),
                                std::move(values), type);
        regs.addMember(tmp);
    }

    void addInstance(std::string &&name) {
        f.changeContent("return "+name+";");
        auto tmp = details::Struct(utils::toLowerCase(name));
        tmp.addMember(f);
        instances.push_back(tmp);
    }

    void addEnum(std::string &&name, std::string &&type = "mem_width") noexcept {
        auto tmp = details::Enum(std::move(name), std::move(type));
        enums.push_back(tmp);
    }

    void addEnumEntry(std::string &&name, std::string &&value) {
        enums.back().addEntry(name, value);
    }

    void addSignal(std::vector<utils::triple<>>& fgps, std::string&& modName) noexcept {
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


        auto funcctr = countDistinct(fgps);
        if(funcctr == 1){ //special case when there is only 1 function (e.g. ports)
            std::vector<details::Struct> toAdd{};
            for(auto& elem : groups){
                details::Struct temp = details::Struct(utils::toCamelCase(elem.str1));
                auto i = 0;
                for(auto& ele : elem.str2){
                    auto tmp = details::TypePin(std::string{ele[1]}, std::string{ele[2]}, std::to_string(i++));
                    temp.addMember(tmp);
                }
                toAdd.push_back(temp);
            }
            for(auto& elem: toAdd){
                auto i = find(instances,modName);
                if(i >= 0) {
                    instances[i].addMember(elem);
                }
            }
        } else if(funcctr > 1){

            std::vector<std::string> processed;
            std::vector<std::string> temp{};
            std::vector<details::Struct> funcs{};
            for (auto &ele : fgps) {
                const auto &search = ele.str1;
                if (!utils::contains(processed, search)) {
                    processed.push_back(search);
                    funcs.emplace_back(utils::toCamelCase(search+""));
                }
            }

            std::vector<details::Struct> toAdd{};
            for(auto& elem : funcs){

                for(auto& ele : groups){
                    details::Struct group = details::Struct(utils::toCamelCase(ele.str1));
                    bool filled = false;
                    auto i = 0;
                    for(auto& el : ele.str2) {
                        if (sameRow13(fgps, elem.name(), el)) {
                            auto tmp = details::TypePin(std::string{el[1]}, std::string{el[2]}, std::to_string(i++));
                            group.addMember(tmp);
                            filled = true;
                        }
                    }
                    if(filled)
                    elem.addMember(group);
                }
                toAdd.push_back(elem);
            }

            for(auto& func : funcs) {
                auto i = find(instances,modName);
                if(i >= 0) {
                    instances[i].addMember(func);
                }
            }

        } else std::cerr << "no function found, sth went wrong\n";
    }

    void parse(const std::string& path) noexcept {
        std::string tmp = header;
        for (auto &elem: enums) {
            compStruct.addMember(elem);
        }
        compStruct.addMember(regs);
        nameSpace.addMember(compStruct);
        if(aliasGenerate){
            auto alias = details::portAlias{};
            nameSpace.addMember(alias);
        }
        auto ins = details::Struct(utils::toLowerCase(compname)+"s");
        for (auto &elem : instances) {
            ins.addMember(elem);
        }
        nameSpace.addMember(ins);
        tmp += nameSpace.generate();

        std::string outName = utils::toHigherCase(path)+ deviceName + compname + ".hpp";
        try {
            std::ofstream of(outName);
            of << tmp;
        } catch (std::exception &e) {
            std::cerr << __FILE__ << " " << __LINE__ << " could not parse: " << e.what();
        }
    }
};
