//
// Created by TehnokraT on 09.05.2024.
//
#pragma once

#ifndef GASFLOWCONTROLMONITORINGAPP_MAINMENU_H
#define GASFLOWCONTROLMONITORINGAPP_MAINMENU_H
#include <string>
#include <vector>
#include <include/csvfile.h>


class mainMenu {
public:
    static void Render();
    static std::vector<std::string> files_;
    static csvfile plot_file;

    static std::vector<std::string> ListFilesInDirectory(const std::string& directory_path);
    static std::string file_to_open;
    static void ShowChosenPlot();
private:
    static void ShowMenuFile();
    static void ShowEditMenu();
    static std::vector<double> time_;
    static std::vector<double> input_;
    static std::vector<double> pid_;

    static bool open_saved_;

};


#endif //GASFLOWCONTROLMONITORINGAPP_MAINMENU_H
