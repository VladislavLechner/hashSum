#include <boost/filesystem.hpp>
#include <QDebug>
#include <iostream>
#include <fstream>

class Formats
{
    std::string m_format;
    std::string m_zip = "4b50 0403 0014";
    std::string m_pdf = "5025 4644 312d";
    std::string m_run = "457f 464c 0102";
    std::string m_webp = "4952 4646 8cf6";
    std::string m_png = "5089 474e 0a0d";
    std::string m_rar = "6152 2172 071a";
    std::string m_mp3 = "4449 0333";
    std::string m_doc = m_zip;
    std::string m_xlsx = m_doc;

public:
    std::string currentFormat(std::string format)
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
        else if (format == "doc")
            return m_doc;
        else if (format == "xlsx")
            return m_xlsx;
        else
            throw std::exception();
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

        if (argc != 4)
        {
            throw std::exception();
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
        std::cout << "Dir = " << nameOfDir << std::endl;
        std::cout << "Format = " << inputFormat << std::endl;
        std::cout << "Operation = " << typeOfOperation << std::endl;

        boost::filesystem::path dir(nameOfDir);
        boost::filesystem::recursive_directory_iterator it(dir), endIt;
        std::string absolutPath(boost::filesystem::current_path().string());

        for (it; it != endIt; ++it)
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
//                    std::cout << temp << std::endl;
                }
                hexFile.close();

                Formats formats;
                std::string currentFormat = formats.currentFormat(inputFormat);

                if (temp.find(currentFormat) < temp.size())
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
                    }
                    md5SumFile.close();
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

    }  catch (...) {
        std::cout << "Error!" << std::endl;
        return -1;
    }
}
