#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class LogFileException {
public:
    LogFileException(const string& message) : message_(message) {}
    string getMessage() const { return message_; }
private:
    string message_;
};

class FileNotFoundException : public LogFileException {
public:
    FileNotFoundException(const string& filePath) : LogFileException("File not found: " + filePath) {}
};

class InvalidFilePathException : public LogFileException {
public:
    InvalidFilePathException(const string& filePath) : LogFileException("Invalid file path: " + filePath) {}
};

class EmptyFileException : public LogFileException {
public:
    EmptyFileException(const string& filePath) : LogFileException("File is empty: " + filePath) {}
};

class Log {
public:
    Log(const string& filePath) : filePath_(filePath) {
        if (!isValidFilePath(filePath_)) {
            throw InvalidFilePathException(filePath_);
        }
    }

    void setFilePath(const string& filePath) {
        if (!isValidFilePath(filePath)) {
            throw InvalidFilePathException(filePath);
        }
        filePath_ = filePath;
    }

    void writeLog(const string& code, const string& message, const string& type) {
        ofstream file(filePath_, ios::app);
        if (!file) {
            throw FileNotFoundException(filePath_);
        }
        file << code << ", " << message << ", " << type << endl;
        file.close();
    }

    vector<string> readLogs() {
        ifstream file(filePath_);
        if (!file) {
            throw FileNotFoundException(filePath_);
        }
        vector<string> logs;
        string line;
        while (getline(file, line)) {
            logs.push_back(line);
        }
        file.close();
        return logs;
    }

    void copyLogs(const string& destinationFilePath) {
        ifstream file(filePath_);
        if (!file) {
            throw FileNotFoundException(filePath_);
        }
        ofstream destinationFile(destinationFilePath);
        if (!destinationFile) {
            throw FileNotFoundException(destinationFilePath);
        }
        string line;
        while (getline(file, line)) {
            destinationFile << line << endl;
        }
        file.close();
        destinationFile.close();
    }

    void deleteLog(const string& code) {
        ifstream file(filePath_);
        if (!file) {
            throw FileNotFoundException(filePath_);
        }
        ofstream tempFile("temp.txt");
        string line;
        bool found = false;
        while (getline(file, line)) {
            if (line.find(code) == string::npos) {
                tempFile << line << endl;
            }
            else {
                found = true;
            }
        }
        file.close();
        tempFile.close();
        if (!found) {
            throw EmptyFileException(filePath_);
        }
        remove(filePath_.c_str());
        rename("temp.txt", filePath_.c_str());
    }

    void deleteAllLogs() {
        remove(filePath_.c_str());
    }

private:
    bool isValidFilePath(const string& filePath) {
        return filePath.find(".txt") != string::npos;
    }

    string filePath_;
};