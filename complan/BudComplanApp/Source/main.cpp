//! @file
//! @brief ����� ����� main() � ��������� BudComplan � ������������� ������ ����������.
#include "precomp.h"
//
#include <amgApplication.h>
#include <JsonCpp/json.h>
#include "Application.h"
//
// ������������� ������
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