// #pragma once
// #include <iostream>
// #include <string>
// #include <filesystem>

// namespace Utils
// {
//     class ProgressBar
//     {
//     public:
//         void GetMax(int MAXSIZEOFFIL)
//         {
//             MAXSIZEOFFILE = MAXSIZEOFFIL;
//         }
//         void update(int file);
//         void print();

//     private:
//         std::string firstPartOfpBar = "[",
//                     lastPartOfpBar = "]",
//                     pBarFiller = "|",
//                     pBarUpdater = "/-\\|";

//     private:
//         int amountOfFiller;
//         int pBarLength = 50;
//         int currUpdateVal = 0;
//         double currentProgress = 0;
//         double neededProgress = 100;
//         int MAXSIZEOFFILE = 0;
//     };
// } // !Utils