// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/XMLViewer under Boost Software License 1.0 
// (MainProject this is submodule to at https://github.com/BlazesRus/BlazesRusSharedCode)
// Other Required submodule at https://github.com/BlazesRus/MFCApp
// ***********************************************************************
#pragma once

#include "../MFCApp/MFCApp.h"
#include "TextView.h"

#include "../OtherFunctions/MFCMacrosV3.h"
#include "../OtherFunctions/MFCBaseTwoMacros.h"

#include "AppSettings.h"

class AppProcessor : public MFCApp<TextView, AppSettings>
{
    MFC_RuntimeExtPart01Base02(AppProcessor, MFCApp, TextView, AppSettings)
    //Any message map messages here
    MFC_RuntimeExtPart02()
    MFC_RuntimeExtClassName(AppProcessor)
};

MFC_RuntimeImplimentation(AppProcessor)
MFC_RuntimeClassImplimentation(AppProcessor)

extern AppProcessor theApp;