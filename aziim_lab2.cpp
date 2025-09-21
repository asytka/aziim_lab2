#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <locale>
#include <codecvt>

std::map<wchar_t, std::vector<std::wstring>> homophonicTable = {
    {L'А', {L"73", L"77", L"93", L"88"}}, {L'а', {L"73", L"77", L"93", L"88"}},
    {L'Б', {L"37", L"02"}}, {L'б', {L"37", L"02"}},
    {L'В', {L"20", L"56", L"97", L"31"}}, {L'в', {L"20", L"56", L"97", L"31"}},
    {L'Г', {L"98", L"89"}}, {L'г', {L"98", L"89"}},
    {L'Ґ', {L"18"}}, {L'ґ', {L"18"}},
    {L'Д', {L"70", L"11", L"99"}}, {L'д', {L"70", L"11", L"99"}},
    {L'Е', {L"28", L"66", L"33", L"46"}}, {L'е', {L"28", L"66", L"33", L"46"}},
    {L'Є', {L"94"}}, {L'є', {L"94"}},
    {L'Ж', {L"43"}}, {L'ж', {L"43"}},
    {L'З', {L"26", L"79"}}, {L'з', {L"26", L"79"}},
    {L'И', {L"64", L"19", L"36", L"29", L"52", L"83"}}, {L'и', {L"64", L"19", L"36", L"29", L"52", L"83"}},
    {L'І', {L"45", L"75", L"27", L"23"}}, {L'і', {L"45", L"75", L"27", L"23"}},
    {L'Ї', {L"71"}}, {L'ї', {L"71"}},
    {L'Й', {L"96"}}, {L'й', {L"96"}},
    {L'К', {L"63", L"54", L"39", L"50"}}, {L'к', {L"63", L"54", L"39", L"50"}},
    {L'Л', {L"58", L"47", L"55"}}, {L'л', {L"58", L"47", L"55"}},
    {L'М', {L"22", L"59", L"80"}}, {L'м', {L"22", L"59", L"80"}},
    {L'Н', {L"32", L"17", L"67", L"40", L"74", L"53"}}, {L'н', {L"32", L"17", L"67", L"40", L"74", L"53"}},
    {L'О', {L"38", L"91", L"05", L"07", L"14", L"60", L"41"}}, {L'о', {L"38", L"91", L"05", L"07", L"14", L"60", L"41"}},
    {L'П', {L"25", L"86"}}, {L'п', {L"25", L"86"}},
    {L'Р', {L"72", L"92", L"82"}}, {L'р', {L"72", L"92", L"82"}},
    {L'С', {L"09", L"03", L"06", L"87"}}, {L'с', {L"09", L"03", L"06", L"87"}},
    {L'Т', {L"35", L"01", L"08", L"49"}}, {L'т', {L"35", L"01", L"08", L"49"}},
    {L'У', {L"12", L"78", L"04"}}, {L'у', {L"12", L"78", L"04"}},
    {L'Ф', {L"42"}}, {L'ф', {L"42"}},
    {L'Х', {L"57"}}, {L'х', {L"57"}},
    {L'Ц', {L"65"}}, {L'ц', {L"65"}},
    {L'Ч', {L"84"}}, {L'ч', {L"84"}},
    {L'Ш', {L"16"}}, {L'ш', {L"16"}},
    {L'Щ', {L"95"}}, {L'щ', {L"95"}},
    {L'Ь', {L"85", L"10"}}, {L'ь', {L"85", L"10"}},
    {L'Ю', {L"62"}}, {L'ю', {L"62"}},
    {L'Я', {L"24", L"90"}}, {L'я', {L"24", L"90"}},
    {L' ', {L"21", L"30", L"51", L"81", L"68", L"34", L"15", L"13", L"76", L"44", L"69", L"48", L"61"}}
};


std::wstring encrypt(const std::wstring& text) {
    std::wstring result;
    std::mt19937 gen(static_cast<unsigned>(time(nullptr)));

    for (wchar_t ch : text) {
        auto it = homophonicTable.find(ch);
        if (it != homophonicTable.end()) {
            const auto& codes = it->second;
            std::uniform_int_distribution<> dist(0, codes.size() - 1);
            result += codes[dist(gen)]; 
        }
    }

    return result;
}

int main() {
    std::locale::global(std::locale(""));

    std::wifstream fin("open_text.txt");
    fin.imbue(std::locale(fin.getloc(), new std::codecvt_utf8<wchar_t>));

    if (!fin) {
        std::wcerr << L"Не вдалось відкрити файл open_text.txt" << std::endl;
        return 1;
    }

    std::wstring openText((std::istreambuf_iterator<wchar_t>(fin)), std::istreambuf_iterator<wchar_t>());
    fin.close();

    std::wstring cipher = encrypt(openText);

    std::wofstream fout("cipher_text.txt");
    fout.imbue(std::locale(fout.getloc(), new std::codecvt_utf8<wchar_t>));
    fout << cipher;
    fout.close();

    std::wcout << L"Шифрування завершено. Результат записано у cipher_text.txt" << std::endl;
    return 0;
}
