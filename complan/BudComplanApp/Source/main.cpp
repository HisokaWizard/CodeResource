//! @file
//! @brief Точка входа main() в программу BudComplan и инициализация класса приложения.
#include "precomp.h"
//
#include <amgApplication.h>
#include <JsonCpp/json.h>
#include "Application.h"
//
// Инициализация версии
// #include "../Resources/sclRouterRC.h"
// #include <Version/amgVersionRC.h>
//
#ifdef _MSC_VER
#pragma pack(16)
#endif
//
TApplication& AmgCall Application()
{
   return(NAmg::NApplication::application<TApplication>());
}
//
int main(int argc, char* argv[])
{
   TApplication app;
   NAmg::TStringsList arguments(argc, argv);
   return app.ConsoleMainLoop(arguments);
}
//