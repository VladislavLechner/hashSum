#include <boost/filesystem.hpp>
#include <QDebug>
#include <iostream>
#include <fstream>

class Formats
{
    std::string m_zip = "4b50 0403 0014";
    std::string m_pdf = "5025 4644 312d";
    std::string m_run = "457f 464c 0102";
    std::string m_webp = "4952 4646 8cf6";
    std::string m_png = "5089 474e 0a0d";
    std::string m_rar = "6152 2172 071a";
    std::string m_mp3 = "4449 0333";
public:
    std::string currentFormat(const std::string& format)
    {
        if (format == "zip")
            return m_zip;
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
            return "0";
        }
        else
        {
            std::string exp("Undefined format");
            throw exp;
        }
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

        std::list<std::string> resultOfHash;

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

        boost::filesystem::recursive_directory_iterator endIt;
        std::string absolutPath(boost::filesystem::current_path().string());

        for (boost::filesystem::recursive_directory_iterator it(dir) ; it != endIt; ++it)
        {
            if (!boost::filesystem::is_directory(*it))
            {
                ++countOfAllFiles;

                std::string forHex("hexdump " + it->path().string() + " > hex.txt");
                system(forHex.c_str());
                std::ifstream hexFile(absolutPath + "/hex.txt");
                std::string temp;

                if (hexFile.is_open())
                {
                    getline(hexFile,temp);
                    hexFile.close();
                }
                else
                {
                    std::string exp("Hex file is not opened");
                    throw exp;
                }

                Formats formats;
                std::string currentFormat = formats.currentFormat(inputFormat);

                if (currentFormat == "0" || temp.find(currentFormat) < temp.size())
                {
                    ++countOfMatchingFiles;

                    std::string forMd5Sum("md5sum " + it->path().string() + " > md5sum.txt");
                    system(forMd5Sum.c_str());

                    std::ifstream md5SumFile(absolutPath + "/md5sum.txt");
                    if(md5SumFile.is_open())
                    {
                        std::string temp;
                        getline(md5SumFile, temp);
                        resultOfHash.push_back(temp);
                        md5SumFile.close();
                    }
                    else
                    {
                        std::string exp("Md5Sum file is not opened");
                        throw exp;
                    }
                }
            }
        }

        std::cout << "All files = " << countOfAllFiles << std::endl
        << "Matching files = " << countOfMatchingFiles << std::endl;

        for (auto &x: resultOfHash)
        {
            std::cout << x << std::endl;
        }

        return 0;

    }  catch (std::string exp) {
        std::cout << exp << std::endl;
        return -1;
    }
}
