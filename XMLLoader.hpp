#include <iostream>

#include "../tsl/ordered_map.h"
#include "../Databases/TagDepthTree.h"
#include "../Databases/ArgStringList.h"
#include "../Databases/ArgList.h"

#include <iostream>
#include <fstream>

namespace XMLViewer
{
    using TagDepthTree = BlazesRusCode::TagDepthTree;
    class OtherTagMember
    {
        std::string TagName;
        std::string Desc;
        /// <summary>
        /// The argument fields of the xml tag
        /// </summary>
        ArgList ArgFields;
        /// <summary>
        /// Vector of index references in InnerTag
        /// </summary>
        std::vector<int> ChildTags;
    };

    /// <summary>
    /// Store entry level tags with this
    /// </summary>
    class XMLEntryTag
    {
		std::string TagName;
        std::string Desc;
        /// <summary>
        /// The argument fields of the xml tag
        /// </summary>
        ArgList ArgFields;
        /// <summary>
        /// The primary tags
        /// </summary>
        std::vector<OtherTagMember> PrimaryTags;
        /// <summary>
        /// Child tags of PrimaryTags and other InnerTags
        /// </summary>
        std::vector<OtherTagMember> InnerTags;
    };

    /// <summary>
    /// Class XMLLoader.
    /// </summary>
    class XMLLoader
    {
    protected:
        /// <summary>
        /// Equal to std::pair(std::string, int)
        /// </summary>
        class DataOrderInfo
        {
        public:
            std::string EntryTagName;
            int Index;
            DataOrderInfo(std::string entryTagName, int indexPosition)
            {
                EntryTagName = entryTagName;
                Index = indexPosition;
            }
        };
    public:
        /// <summary>
        /// The other data
        /// </summary>
        std::vector<XMLEntryTag> xmlData;

        /// <summary>
        /// Generate XML files based on loaded content 
        /// </summary>
        void GenerateXMLsFromContent()
        {

        }

        /// <summary>
        /// Loads the XML in read-only format(deleting a tag could break inner tag references in this format).
        /// </summary>
        /// <param name="FilePath">The file path.</param>
        /// <returns>bool</returns>
        bool LoadXML(std::string FilePath)
        {
            char LineChar;
            bool InsideXMLComment = false;
            //If false, then inside tag-content types instead of tags
            bool InsideTag = false;
            std::string ScanBuffer = "";

            //First name inside tag becomes CurrentTag
            std::string CurrentTag = "";
            std::string CurrentNodeName = "";
            unsigned int CurrentNodeIndex = 0;
            //0=NormalTag; 1:SelfContainedTag; 2:TagIsClosing; 3:XMLVersionTag
            int TagType = 0;

            bool ScanningArgData = false;
            std::string ArgElement;
            ArgStringList LastArg;
            ArgList ArgBuffer;

            bool PotentialComment = false;
            bool InsideParenthesis = false;
            //0 = Not Scanning TagContent Yet: 1 = Potential SingleLine TagContent: 2 Multi-line target content
            short TagContentStage = 0;

            //Current state of code loading for certain sections of code
            size_t Stage = 0;

            TagDepthTree TagDepth;

            int EntryNodeIndex = -1;
            std::string EntryTagName = "";
            bool InsideClosingTag = false;

            bool StartedTagRead = false;
            bool SkipCurrentTag = false;
            std::ifstream inFile;
            inFile.open(FilePath);
            if (!inFile)
            {
                return false;
            }
            while (inFile >> LineChar)
            {
                if (PotentialComment)
                {
                    ScanBuffer += LineChar;
                    if (ScanBuffer == "--")
                    {
                        InsideXMLComment = true;
                        PotentialComment = false;
                        ScanBuffer = "";
                    }
                    else if (ScanBuffer.size() >= 2)//Detecting non-normal format TagName?
                    {
                        PotentialComment = false;
                        ScanBuffer = "!" + ScanBuffer;
                    }
                }
                else if (InsideXMLComment)//Ignoring all xml inside xml formatted comment
                {
                    Stage = ScanBuffer.size();
                    if (Stage == 0)
                    {
                        if (LineChar == '-')
                        {
                            ScanBuffer = "-";
                        }
                    }
                    else if (Stage == 1)
                    {
                        if (LineChar == '-')
                        {
                            ScanBuffer = "--";
                        }
                        else
                        {
                            ScanBuffer = "";
                        }
                    }
                    else
                    {
                        if (LineChar == '>')
                        {
                            InsideXMLComment = false;
                        }
                        ScanBuffer = "";
                    }
                }
                else if (ScanningArgData)
                {
                    if (Stage == 0)
                    {
                        if (LineChar == '=')
                        {
                            Stage = 1;
                            ArgElement = "";
                        }
                        else if (LineChar != ' ' && LineChar != '\t' && LineChar != '\n')//Skip Whitespace
                        {
                            ScanBuffer += LineChar;
                        }
                    }
                    else if (Stage == 1)
                    {
                        if (LineChar == '\"')
                        {
                            Stage = 2;
                        }
                    }
                    else if (Stage == 2)
                    {
                        if (LineChar == '\"') { Stage = 3; }
                    }
                    else if (Stage == 3)
                    {
                        if (LineChar == ',')
                        {
                            LastArg.Add(ArgElement);
                        }
                        else if (LineChar == '\"')
                        {
                            ArgBuffer.Add(ScanBuffer, LastArg);
                            ScanningArgData = false;
                        }
                        else
                        {
                            ArgElement += LineChar;
                        }
                    }
                }
                else if(StartedTagRead)//Only start saving scans once enter certain depth of xml file
                {
                    if(InsideClosingTag)
                    {
                        if(LineChar=='>')
                        {
                            if (CurrentTag == EntryTagName)//Exiting entry tag
                            {
                                EntryTagName.clear();
                            }
                            else//Exiting inner tag 
                            {

                            }
                            CurrentTag = "";//Reset it to clear buffer so next tag has fresh storage
                            TagContentStage = 0;
                            InsideClosingTag = false; InsideTag = false;
                        }
                    }
                    else if (InsideTag)
                    {
                        if (LineChar == '>')
                        {
                            if (EntryTagName.empty())
                            {
                                EntryTagName = CurrentTag;
                            }
                            else
                            { 
                            }
                        }
                        else if (CurrentTag.empty())
                        {
                            if (ScanBuffer.empty())
                            {
                                if (LineChar == '!')//Detecting potential Commented Out Parts
                                    PotentialComment = true;
                                else if(LineChar=='/')
                                {

                                }
                                else if (LineChar != ' ' && LineChar != '	' && LineChar != '\n')
                                    ScanBuffer += LineChar;
                            }
                            else if (LineChar == '/')//Closed Tag without any arguments
                            {
                                CurrentTag = ScanBuffer;
                                ScanBuffer = "/";
                            }
                            else if (LineChar == ' ' || LineChar == '	' || LineChar == '\n')
                            {
                                CurrentTag = ScanBuffer;
                                ScanBuffer.clear();
                                //if (LineChar != '\\')
                                //{
                                //    ScanningArgData = true; Stage = 0;
                                //}
                            }
                            else if (LineChar != ' ' && LineChar != '	' && LineChar != '\n')
                            {
                                ScanBuffer += LineChar;
                            }
                        }
                        //------------------Scanning Argument Field/Values-------------------------------
                        else
                        {
                            if (ScanBuffer.empty())
                            {
                                if (LineChar != ' ' && LineChar != '	' && LineChar != '\n')
                                {
                                    ScanBuffer += LineChar;
                                }
                            }
                            else if (LineChar == ' ' || LineChar == '	' || LineChar == '\n')
                            {
                                //CurrentTag = ScanBuffer;
                                ScanBuffer.clear();
                                //if (LineChar != '\\')
                                //{
                                //    ScanningArgData = true; Stage = 0;
                                //}
                            }
                            else if (LineChar != ' ' && LineChar != '	' && LineChar != '\n')
                            {
                                ScanBuffer += LineChar;
                            }
                        }
                    }
                    else
                    {
                        if (LineChar == '<')
                        {
                            //Send Description field into tag target

                            InsideTag = true; ScanBuffer.clear();
                        }
                        else//If description value is empty, add data to description field buffer
                        {
                            ScanBuffer += LineChar;
                        }
                    }
                }
                else
                {
                    if (InsideTag)
                    {
                        if(SkipCurrentTag)
                        {
                            if(LineChar=='>')
                            {
                                SkipCurrentTag = false; ScanBuffer.clear(); InsideTag = false;
                            }
                        }
                        else
                        {
                            if (ScanBuffer.empty())
                            {
                                if (LineChar == '!')//Detecting potential Commented Out Parts
                                {
                                    PotentialComment = true;
                                }
#ifdef XMlViewer_AllowSpecialTags
                                else if (LineChar == '?')
                                    SkipCurrentTag = true;
#endif
                                else if (LineChar != ' ' && LineChar != '	')
                                {
                                    ScanBuffer += LineChar;
                                }
                            }
                            else if (LineChar == ' ' || LineChar == '	')
                            {
								CurrentTag = ScanBuffer;
								ScanBuffer.clear();
								StartedTagRead = true;
                            }
                            else
                            {
                                ScanBuffer += LineChar;
                            }
                        }
                    }
                    else
                    {
                        if (LineChar == '<')
                        {
                            InsideTag = true;
                        }
                    }
                }
            }
            return true;
        }
        void GenerateDebugOutput()
        {

        }
        void GenerateFiles()
        {

        }
    };
}

//int main()
//{
//    XMLViewer::XMLLoader XmlData;
//    XmlData.LoadXML();
//    //XmlData.GenerateFiles();
//}