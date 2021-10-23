// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/XMLViewer under Boost Software License 1.0 
// (MainProject this is submodule to at https://github.com/BlazesRus/BlazesRusSharedCode)
// Other Required submodule at https://github.com/BlazesRus/MFCApp
// ***********************************************************************
#pragma once

#include "../MFCApp/MFCView.h"
//#include "../VariableLists/StringVectorList.h"

class XMLView : public MFCView
{
protected: // create from serialization only
    XMLView() noexcept;
    virtual ~XMLView()
    {

    }
    DECLARE_DYNCREATE(XMLView)

// Attributes
public:
    StringVectorList ContentList;
    MFCDoc* GetDocument() const;

protected:

// Operations
public:
    /// <summary>
    /// Resets the storage of this instance.
    /// </summary>
    void Reset()
    {

    }

    //-------File Loading Operations-------------------------------------------------------
    
    /// <summary>
    /// Loads the data from file.
    /// </summary>
    /// <param name="FilePath">The file path.</param>
    /// <returns>bool</returns>
    bool LoadDataFromFile(std::string FilePath);

    /// <summary>
    /// Saves the loaded data to file. (if / or \ is last character(Targeting Directory instead of file), will instead create/replace TextFile.xml)
    /// </summary>
    /// <param name="FilePath">The file path. or file name </param>
    void SaveDataToFile(std::string FilePath);
//--------------------------------------------------------------------------------------

// Overrides
public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
#ifndef BlazesMFCApp_DisablePrinter
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
#endif

// Implementation
public:
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in XMLView.cpp
inline MFCDoc* XMLView::GetDocument() const
   { return reinterpret_cast<MFCDoc*>(m_pDocument); }
#endif

