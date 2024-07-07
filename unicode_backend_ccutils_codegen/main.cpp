
#include <iostream>
#include <map>
#include <set>

#include <tinyxml2.h>

// struct CharInfo {}

using KEY_VALUE_SET = std::set<std::string>;
using Char_KEY_SET  = std::map<std::string, KEY_VALUE_SET>;

int work_on_char(Char_KEY_SET &char_key_set, const tinyxml2::XMLElement *chr)
{
    for (
        const tinyxml2::XMLAttribute *i = chr->FirstAttribute();
        i != nullptr;
        i = i->Next()
    )
    {
        auto name  = std::string(i->Name());
        auto value = std::string(i->Value());
        char_key_set[name].insert(value);
    }

    return 0;
}

void press_enter(std::string prompt = "")
{
    // comment return if you need this function
    return;
    std::string gap = "";
    if (prompt != "")
    {
        gap = " ";
    }
    std::cout << std::format("[ press enter to continue]{}{}:", gap, prompt);
    std::cin.get();
}

int main(int argc, char **args)
{
    press_enter("after start");

    Char_KEY_SET char_key_set;

    std::size_t processed_char_count = 0;

    {
        tinyxml2::XMLDocument doc;

        {
            press_enter("before loading");
            std::cout << "loading.." << std::endl;
            auto res = doc.LoadFile("ucd.all.flat.xml");
            press_enter("after loading");

            if (res != tinyxml2::XML_SUCCESS)
            {
                std::cout << "fail" << std::endl;
                return 1;
            }
        }

        std::cout << "scss" << std::endl;

        auto root = doc.RootElement();

        if (!root)
        {
            std::cout << "no root element" << std::endl;
            return 1;
        }

        std::cout << "  root element name: " << root->Name() << std::endl;

        auto repertoire = root->FirstChildElement("repertoire");
        if (!repertoire)
        {
            std::cout << "no repertoire element" << std::endl;
            return 1;
        }

        tinyxml2::XMLElement *c = repertoire->FirstChildElement("char");

        std::cout << std::endl;

        press_enter("before acqiring data");

        for (
            tinyxml2::XMLElement *c = repertoire->FirstChildElement();
            c != nullptr;
            c = c->NextSiblingElement()
        )
        {
            if (std::strcmp(c->Name(), "char") == 0)
            {
                work_on_char(char_key_set, c);
                processed_char_count++;
            }
        }
        press_enter("before freeng up document");
    }

    press_enter("after freeng up document");

    std ::cout << "  key count: " << char_key_set.size() << std::endl;

    for (const auto &k : char_key_set)
    {
        std ::cout << "     key: " << std::get<0>(k) << std::endl;
    }

    std ::cout << "  keys and values: " << std::endl;
    for (const auto &[k, v] : char_key_set)
    {
        std ::cout << "     key: " << k << std::endl;
        std ::cout << "     val count: " << v.size() << std::endl;
        if (v.size() > 300)
        {
            std ::cout << "        too many - not printing" << std::endl;
        }
        else
        {
            for (const auto &i : v)
            {
                std ::cout << "       val: " << i << std::endl;
            }
        }
    }

    press_enter("before exit");

    std ::cout << "processed char count: " << processed_char_count << std::endl;

    return 0;
}
