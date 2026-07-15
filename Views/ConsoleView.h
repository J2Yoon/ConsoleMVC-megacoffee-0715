#pragma once

#include <string>

// 콘솔 입출력을 담당하는 공용 View 헬퍼.
// Controller/Model은 화면 형식을 알지 못하며, 모든 출력 형식은 이 클래스가 담당한다.
class ConsoleView
{
public:
    static void PrintTitle(const std::string& title);
    static void PrintLine(const std::string& text = "");
    static void PrintDivider();
    static void PrintError(const std::string& message);

    static std::string ReadLine(const std::string& prompt);
    static int ReadInt(const std::string& prompt);
    static double ReadDouble(const std::string& prompt);
};
