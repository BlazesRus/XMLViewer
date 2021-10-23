// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/XMLViewer under Boost Software License 1.0 
// (MainProject this is submodule to at https://github.com/BlazesRus/BlazesRusSharedCode)
// Other Required submodule at https://github.com/BlazesRus/MFCApp
// ***********************************************************************
#include "AppPch.h"
#include "XMLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// XMLView
IMPLEMENT_DYNCREATE(XMLView, MFCView)

BEGIN_MESSAGE_MAP(XMLView, MFCView)
#ifndef BlazesMFCApp_DisablePrinter
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, &MFCView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &MFCView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &MFCView::OnFilePrintPreview)
#endif
END_MESSAGE_MAP()

// XMLView construction/destruction

XMLView::XMLView() noexcept
{

}

BOOL XMLView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    return MFCView::PreCreateWindow(cs);
}

#pragma region File Loading And Saving
bool XMLView::LoadDataFromFile(std::string FilePath)
{
    Reset();
    char LineChar;
    std::string LineString="";
    std::ifstream inFile;
    inFile.open(FilePath);
    if (!inFile)
    {
        return false;
    }
    while (inFile >> LineChar)
    {
        if (LineChar != '\n')
        {
            ContentList.Add(LineString); LineString.clear();
        }
        else
            LineString += LineChar;
    }
    if (!LineString.empty())
        ContentList.Add(LineString);
    return true;
}

void XMLView::SaveDataToFile(std::string FilePath)
{
    if (FilePath.back() == '/' || FilePath.back() == '\\') { FilePath += "TextFile.txt"; }
    //size_t StringLength;
    //char StringChar;
    //std::string LineString;
    //std::fstream LoadedFileStream;
    ////Fix for non-existing file
    //CreateFileIfDoesntExist(FilePath);
    //LoadedFileStream.open(FilePath.c_str(), std::fstream::out | std::fstream::trunc);
    //if (LoadedFileStream.is_open())
    //{
    //	if (LoadedFileStream.good())
    //	{//Saving to file now
    //		//for (ContentList::iterator ArgElement = ContentList.begin(), EndElement = ContentList.end(); ArgElement != EndElement; ++ArgElement)
    //		//{
    //		//	LoadedFileStream << ArgElement << "\n";
    //		//}
    //	}
    //	else
    //	{
    //		if (LoadedFileStream.bad()) { std::cout << "Failed Read/Write operation Error!\n"; }
    //		else if (LoadedFileStream.fail()) { std::cout << "Failed format based Error!\n"; }
    //		else if (LoadedFileStream.bad()) { std::cout << "Failed Read/Write operation Error!\n"; }
    //		else if (LoadedFileStream.eof()) {/*Send debug message of reaching end of file?*/ }
    //	}
    //	LoadedFileStream.close();
    //}
    //else
    //{
    //	std::cout << "Failed to open " << FilePath << ".\n";
    //}
    ContentList.SaveDataToFileV3(FilePath, true);
}
#pragma endregion File Loading And Saving

// XMLView drawing

void XMLView::OnDraw(CDC* /*pDC*/)
{
    MFCDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // TODO: add draw code for native data here
}

#pragma region Printer Features
#ifndef BlazesMFCApp_DisablePrinter
// XMLView printing
BOOL XMLView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default preparation
    return DoPreparePrinting(pInfo);
}

void XMLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add extra initialization before printing
}

void XMLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add cleanup after printing
}
#endif
#pragma endregion Printer Features

#pragma region XMLView diagnostics
#ifdef _DEBUG
void XMLView::AssertValid() const
{
    MFCView::AssertValid();
}

void XMLView::Dump(CDumpContext& dc) const
{
    MFCView::Dump(dc);
}

MFCDoc* XMLView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(MFCDoc)));
    return (MFCDoc*)m_pDocument;
}
#endif //_DEBUG
#pragma endregion XMLView diagnostics

// XMLView message handlers
