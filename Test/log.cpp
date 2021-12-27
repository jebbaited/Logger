#pragma warning(disable : 4996)
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>
#include <mutex>
#include "log.h"

using namespace std::chrono;

const std::string DEBUG = "DEBUG";
const std::string INFO = "INFO";
const std::string WARNING = "WARNING";
const std::string ERROR = "ERROR";

std::mutex mtx;
std::string timeInStr;
bool checkFirstCall;

std::string Logger::getCurrentTime()
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
	auto str = oss.str();
	timeInStr = str;

	return str;
}

Logger::Logger(const std::string& prefix, const std::string& level)
{
	this->prefix = prefix;

	chooseLogLevel(level);

	mtx.lock();
	ask();
	mtx.unlock();
}

void Logger::setToStdout()
{
	target = OutputTarget::STDOUT;
}

void Logger::setToFile(const std::string& fileName)
{
	target = OutputTarget::FILE;

	logFile.open(fileName, std::ios::app);

	if (!logFile.is_open())
	{
		std::cout << "Can't open the file " << fileName << std::endl;
	}
}

void Logger::setLogLevel(LogLevel logLevel)
{
	this->logLevel = logLevel;
}

void Logger::log(LogLevel logLevel, const std::string& text)
{
	threadID = std::this_thread::get_id();

	if (checkFirstCall == false)
	{
		switch (target)
		{
		case Logger::OutputTarget::STDOUT:
			std::cout << "\n" << getCurrentTime() << "; " << logLevel << prefix << "(" << threadID << ")" << "; " << text;
			break;
		case Logger::OutputTarget::FILE:
			std::cout << "\nSaved to the file\n";
			logFile << "\n" << getCurrentTime() << "; " << logLevel << prefix << "(" << threadID << ")" << "; " << text;
			logFile.close();
			break;
		default:
			break;
		}
	}
	else
	{
		switch (target)
		{
		case Logger::OutputTarget::STDOUT:
			std::cout << text;
			break;
		case Logger::OutputTarget::FILE:
			logFile << text;
			logFile.close();
			break;
		default:
			break;
		}
	}

	checkFirstCall = true;
}

void Logger::ask()
{
	std::cout << "\nChoose which output to use: \n1 - console log \n2 - file log \n0 - exit\n";
	do
	{
		std::cout << "Answer: ";
		std::cin >> response;
	} while (response != 0 && response != 1 && response != 2);

	if (response == 0)
	{
		exit(0);
	}
}

Logger& Logger::operator<<(const char* text)
{
	this->logText = text;

	chooseWhereToWrite();

	return *this;
}

Logger& Logger::operator<<(const int number)
{
	this->logText = std::to_string(number);

	chooseWhereToWrite();

	return *this;
}

void Logger::chooseWhereToWrite()
{
	switch (this->response)
	{
	case 1:
		setToStdout();

		mtx.lock();
		log(logLevel, logText);
		mtx.unlock();

		break;
	case 2:
		setToFile("Logs.txt");

		mtx.lock();
		log(logLevel, logText);
		mtx.unlock();

		break;
	default:
		break;
	}
}

void Logger::chooseLogLevel(const std::string& str)
{
	if (str == "DEBUG")
	{
		setLogLevel(LogLevel::DEBUG);
	}
	else if (str == "INFO")
	{
		setLogLevel(LogLevel::INFO);
	}
	else if (str == "WARNING")
	{
		setLogLevel(LogLevel::WARNING);
	}
	else if (str == "ERROR")
	{
		setLogLevel(LogLevel::ERROR);
	}
}

std::ostream& operator<<(std::ostream& os, const LogLevel& data)
{
	switch (data)
	{
	case LogLevel::DEBUG:
		os << "DEBUG; ";
		break;
	case LogLevel::INFO:
		os << "INFO; ";
		break;
	case LogLevel::WARNING:
		os << "WARNING; ";
		break;
	case LogLevel::ERROR:
		os << "ERROR; ";
		break;
	default:
		break;
	}

	return os;
}

std::function<Logger(const std::string)> getLogger(const std::string prefix)
{
	return [prefix](const std::string level)
	{
		Logger logger(prefix, level);
		return logger;
	};
}
