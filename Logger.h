#pragma once
#include <fstream>
#include <mutex>
#include <string>

class Logger {
private:
	std::ifstream logfileIn;
	std::ofstream logfileOut;
	std::mutex mtx;
public:
	Logger(const std::string& filename)
	{
		logfileOut.open(filename, std::ios::app);
		if (!logfileOut.is_open())
		{
			throw std::runtime_error("ошибка открытия файла для записи");
		}
		logfileIn.open(filename);
		if (!logfileIn.is_open()) {
			throw std::runtime_error("Ошибка открытия файла для чтения");
		}
	}
	~Logger()
	{
		if (logfileOut.is_open())
		{
			logfileOut.close();
		}
		if (logfileIn.is_open()) {
			logfileIn.close();
		}
	}

	void logMessage(const std::string& message)
	{
		std::lock_guard<std::mutex> lock(mtx);
		logfileOut << message << std::endl;
	}
	std::string readLog()
	{
		std::lock_guard<std::mutex> lock(mtx);
		std::string line;
		if (std::getline(logfileIn, line)) {
			return line;
		}
		return "";
	}
};