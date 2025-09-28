#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <nlohmann/json.hpp>
#include <Windows.h>
#include "source/utf8.h" 

using json = nlohmann::ordered_json;

std::unordered_map<std::string, std::vector<std::string>> homophonicTable = {
    { "А", {"173", "177", "193", "188"}}, { "а", {"273", "277", "293", "288"}},
    { "Б", {"137", "102"} }, { "б", {"237", "202"} },
    { "В", {"120", "156", "197", "131"} }, { "в", {"220", "256", "297", "231"} },
    { "Г", {"198", "189"} }, { "г", {"298", "289"} },
    { "Ґ", {"118"} }, { "ґ", {"218"} },
    { "Д", {"170", "111", "199"} }, { "д", {"270", "211", "299"} },
    { "Е", {"128", "166", "133", "146"} }, { "е", {"228", "266", "233", "246"} },
    { "Є", {"194"} }, { "є", {"294"} },
    { "Ж", {"143"} }, { "ж", {"243"} },
    { "З", {"126", "179"} }, { "з", {"226", "279"} },
    { "И", {"164", "119", "136", "129", "152", "183"} }, { "и", {"264", "219", "236", "229", "252", "283"} },
    { "І", {"145", "175", "127", "123"} }, { "і", {"245", "275", "227", "223"} },
    { "Ї", {"171"} }, { "ї", {"271"} },
    { "Й", {"196"} }, { "й", {"296"} },
    { "К", {"163", "154", "139", "150"} }, { "к", {"263", "254", "239", "250"} },
    { "Л", {"158", "147", "155"} }, { "л", {"258", "247", "255"} },
    { "М", {"122", "159", "180"} }, { "м", {"222", "259", "280"} },
    { "Н", {"132", "117", "167", "140", "174", "153"} }, { "н", {"232", "217", "267", "240", "274", "253"} },
    { "О", {"138", "191", "105", "107", "114", "160", "141"} }, { "о", {"238", "291", "205", "207", "214", "260", "241"} },
    { "П", {"125", "186"} }, { "п", {"225", "286"} },
    { "Р", {"172", "192", "182"} }, { "р", {"272", "292", "282"} },
    { "С", {"109", "103", "106", "187"} }, { "с", {"209", "203", "206", "287"} },
    { "Т", {"135", "101", "108", "149"} }, { "т", {"235", "201", "208", "249"} },
    { "У", {"112", "178", "104"} }, { "у", {"212", "278", "204"} },
    { "Ф", {"142"} }, { "ф", {"242"} },
    { "Х", {"157"} }, { "х", {"257"} },
    { "Ц", {"165"} }, { "ц", {"265"} },
    { "Ч", {"184"} }, { "ч", {"284"} },
    { "Ш", {"116"} }, { "ш", {"216"} },
    { "Щ", {"195"} }, { "щ", {"295"} },
    { "Ь", {"185", "110"} }, { "ь", {"285", "210"} },
    { "Ю", {"162"} }, { "ю", {"262"} },
    { "Я", {"124", "190"} }, { "я", {"224", "290"} },
    { " ", {"321", "330", "351", "381", "368", "334", "315", "313", "376", "344", "369", "348", "361"} }
};

std::string encrypt(const std::string& text) {
    std::string result;
    std::mt19937 gen(static_cast<unsigned>(time(nullptr)));

    using it_t = utf8::iterator<std::string::const_iterator>;
    it_t it(text.begin(), text.begin(), text.end());
    it_t end(text.end(), text.begin(), text.end());

    for (; it != end; ++it) {
        char32_t cp = *it;

        std::string ch;
        utf8::append(cp, std::back_inserter(ch));

        auto found = homophonicTable.find(ch);
        if (found != homophonicTable.end()) {
            const auto& codes = found->second;
            std::uniform_int_distribution<> dist(0, codes.size() - 1);
            result += codes[dist(gen)];
        }
    }
    return result;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    json j = homophonicTable;
    std::ofstream f("table.json");
    f << j.dump(2);
    f.close();

    std::ifstream fin("open_text.txt", std::ios::binary);
    if (!fin) {
        std::cerr << "Не вдалось відкрити файл open_text.txt\n";
        return 1;
    }
    std::string openText((std::istreambuf_iterator<char>(fin)), {});
    fin.close();

    std::string cipher = encrypt(openText);

    std::ofstream fout("cipher_text.txt", std::ios::binary);
    fout << cipher;
    fout.close();

    std::cout << "Шифрування завершено. Результат записано у cipher_text.txt\n";
    return 0;
}
