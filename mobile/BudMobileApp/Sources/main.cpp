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
NAmg::TApp& AmgCall Application()
{
   return(NAmg::NApplication::application<NAmg::TApp>());
}
//
int main(int argc, char* argv[])
{
   NAmg::TApp app;
   NAmg::TStringsList arguments(argc, argv);
   return app.ConsoleMainLoop(arguments);
}
//