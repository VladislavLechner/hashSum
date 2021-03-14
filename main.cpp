#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>

// @brief нахождение формата файла
class Formats
{  
    std::string m_pdf = "PDF";
    std::string m_zip = "Zip";
    std::string m_rar = "RAR";
    std::string m_jpg = "JPEG";
    std::string m_doc = "Microsoft Word";
    std::string m_txt = "ASCII text";
    std::string m_mp3 = "Audio file with ID3";
    std::string m_png = "PNG";
    std::string m_run = "ELF";
    std::string m_webp = "RIFF";
    std::string m_xlsx = "Microsoft Excel";

public:
    std::string currentFormat(const std::string& format)
    {
        if (format == "zip")
            return m_zip;
        else if(format == "txt")
            return m_txt;
        else if(format == "jpg")
            return m_jpg;
        else if (format == "pdf")
            return m_pdf;
        else if (format == "run")
            return m_run;
        else if (format == "webp")
            return m_webp;
        else if (format == "png")
            return m_png;
        else if (format == "rar")
            return m_rar;
        else if (format == "mp3")
            return m_mp3;
        else if (format == "")
        {
            return "";
        }
        else
        {
            std::string exp("Undefined format");
            throw exp;
        }
    }

    bool matching(const std::string& path, const std::string& inputFormat)
    {
        std::string forHex("file -b " + path + " > format.txt");
        system(forHex.c_str());
        std::ifstream formatFile(boost::filesystem::current_path().string() + "/format.txt");
        std::string temp;

        if (formatFile.is_open())
        {
            getline(formatFile,temp);
            formatFile.close();
        }
        else
        {
            std::string exp("Hex file is not opened");
            throw exp;
        }
        size_t pos = temp.find(",");
        if (pos < temp.size())
            temp.erase(pos, temp.size());
        return (temp.find(currentFormat(inputFormat)) < temp.size());
    }
};




int main(int argc, char *argv[])
{
    try {
        std::string nameOfDir;
        std::string typeOfOperation;
        std::string inputFormat;

        int countOfAllFiles = 0;
        int countOfMatchingFiles = 0;

//        std::list<std::string> resultOfHash;

        if (argc > 4)
        {
            std::string exp("Too many argument");
            throw exp;
        }

        for (int i = 1; i < argc; ++i)
        {
            std::string arg(argv[i]);
            if (arg.find("/") < arg.size())
                nameOfDir = arg;
            else if (arg == "ctrlSum" )
                typeOfOperation = arg;
            else
                inputFormat = arg;
        }

        if (typeOfOperation != "ctrlSum")
        {
            std::string exp("Uncorrect operation");
            throw exp;
        }


        boost::filesystem::path dir(nameOfDir);
        if (!boost::filesystem::exists(dir))
        {
            std::string exp("Directory is not exist");
            throw exp;
        }

        if (!boost::filesystem::is_directory(dir) && inputFormat == "")
        {
            std::cout << "For only one file" << std::endl;
            system(dir.c_str());
            return 0;
        }
        else if (!boost::filesystem::is_directory(dir) && inputFormat != "")
        {
            std::string exp("Uncorrect operation");
            throw exp;
        }

        boost::filesystem::recursive_directory_iterator endIt;

        for (boost::filesystem::recursive_directory_iterator it(dir); it != endIt; ++it)
        {
            if (!boost::filesystem::is_directory(*it))
            {
                ++countOfAllFiles;
                Formats format;
                if (format.matching(it->path().string(), inputFormat))
                {
                    ++countOfMatchingFiles;
                    std::string temp("md5sum " + it->path().string());
                    system(temp.c_str());
                }
            }
        }

        std::cout << "All files = "      << countOfAllFiles      << std::endl
                  << "Matching files = " << countOfMatchingFiles << std::endl;

//        for (auto &x: resultOfHash)
//        {
//            std::cout << x << std::endl;
//        }
        return 0;

    }  catch (std::string exp) {
        std::cout << exp << std::endl;
        return -1;
    }
}
