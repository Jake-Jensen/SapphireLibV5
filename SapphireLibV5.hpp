#pragma once

// Small update for documentation reasons

#define _CRT_SECURE_NO_WARNINGS

// Some of these includes may be redundant or unused. Once the library is finished, I will prune the excess
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <random>
#include <fstream>
#include <ostream>
#include <istream>
#include <chrono>
#include <ctime>
#include <typeinfo>
#include <atomic>
#include <mutex>
#include <thread>
#include <exception>
#include <Lmcons.h>
#include <curl/curl.h>
#include <direct.h>

#ifdef WIN32
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Advapi32.lib")
#include <ws2bth.h>
#include <Windows.h>
#include <crtdbg.h>
#include <wininet.h>
#pragma comment(lib,"Wininet.lib")
#include <psapi.h>
#include <tchar.h>
#include <SetupAPI.h>
#pragma comment(lib, "SetupAPI.lib")
#include <wlanapi.h>
#include <Windot11.h>           // for DOT11_SSID struct
#include <objbase.h>
#include <wtypes.h>
// Need to link with Wlanapi.lib and Ole32.lib
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")
#endif

#include <sys/types.h>
#include <locale>
#include <codecvt>
#include <sys/stat.h>
#include <filesystem>
#include <optional>
#include <sstream>
#include <thread>
#include <algorithm>

namespace Sapphire {

std::string readBuffer; // The buffer used by cURL (NetworkTool) for storing the downloaded information
char CurlError[1024];	// The buffer used by cURL (NetworkTool) for storing information exclusively related to errors

// The following consts are for changing the line color of terminal output, if UseVirtualTerminal() has been invoked
const int BLACK = 30;	
const int RED = 31;
const int GREEN = 32;
const int YELLOW = 33;
const int BLUE = 34;
const int MAGENTA = 35;
const int CYAN = 36;
const int WHITE = 37;

// Various error constants for switch[]ing returns
const int READFILE_BAD_SIZE = 52;
const int READFILE_MISSING = 53;
const int READFILE_EMPTY_FILE = 54;
const int WRITEFILE_BAD_SIZE = 52;
const int WRITEFILE_MISSING = 53;
const int WRITEFILE_EMPTY_FILE = 54;

// The number of logical cores the CPU reports to the OS
const int processor_count = std::thread::hardware_concurrency();

// Constants for the _LOG function
const int LOG_INFO = 0;
const int LOG_DEBUG = 1;
const int LOG_WARN = 2;
const int LOG_ERROR = 3;
const int LOG_UNRECOVERABLE = 4;

// Various error constants for switch[]ing returns
const std::string REGTOOLS_NO_FILE = "FAIL_NO_FILE";
const std::string REGTOOLS_CHECKED_OUT = "FAIL_CHECKED_OUT";
const std::string REGTOOLS_CORRUPT = "FAIL_CORRUPT";
const std::string REGTOOLS_UNKNOWN = "FAIL_UNKNOWN";
const std::string REGTOOLS_ERROR = "FAIL_ERROR";
const std::string REGTOOLS_WRITE_SUCCESS = "SUCCESS";
const std::string REGTOOLS_WRITE_FAIL = "FAIL";
const std::string REGTOOLS_WRITE_CLOSE_FAIL = "FAIL";

// Various hives for the ReadRegistry function. Not compatible with Windows' HKEY_LOCAL_MACHINE and etc macros
// due to internal switching
const int REGTOOLS_HKEY_CLASSES_ROOT = 0;
const int REGTOOLS_HKEY_CURRENT_USER = 1;
const int REGTOOLS_HKEY_LOCAL_MACHINE = 2;
const int REGTOOLS_HKEY_USERS = 3;
const int REGTOOLS_HKEY_CURRENT_CONFIG = 4;

/* Forward declarations */

// The struct used by NetworkTool and various others. Prevents needing multiple arguments to call the function
// Default constructor sets default values that should help most people. For most purposes, just setting the URL should
// be sufficient.
struct DownloadOptions {
	std::string IP; // The IP to target
	std::string URL; // OR the URL to target (Non-exclusive)
	std::string Referrer; // The referrer string to pass
	bool SkipVerification; // Skip verification
	bool SkipHostnameVerification; // Skip hostname verification
	bool DownloadToMemory; // Downloads to readBuffer
	bool UseCustomPort; // Use a different port, automatically set if Port != 80L
	long Port; // The port to use, defaults to 80L
	int LowerPort;	// The lowest port to start portscanning on
	int UpperPort;	// The highest port to start portscanning on
	bool UseTimeout; // Use a custom timeout value, automatically set if Timeout is set
	long Timeout;	// The timeout to wait for
	bool ShowOutput;	// Show debug and verbose output
	bool DownloadToStartup;	// Download to the program's startup directory
	std::string ErrorCode;	// The error code returned by cURL
	bool IsSinglePort;	// Port scanner use only, indicates only a single port will be scanned.
	DownloadOptions() : IP(""), URL(""), Referrer(""), SkipVerification(false), SkipHostnameVerification(false), DownloadToMemory(true), UseCustomPort(false), Port(80L), LowerPort(0L), UpperPort(65545L),
		UseTimeout(false), Timeout(0L), ShowOutput(false), DownloadToStartup(false), ErrorCode(""), IsSinglePort(false)
	{
	}
};

// Legacy, left over function for setting the default values. Was used before the struct had a constructor.
DownloadOptions ResetDownloadOptions(DownloadOptions DO)
{
	DO.IP = "";
	DO.URL = "";
	DO.Referrer = "";
	DO.SkipVerification = false;
	DO.SkipHostnameVerification = false;
	DO.DownloadToMemory = false;
	DO.UseCustomPort = false;
	DO.Port = 0L;
	DO.LowerPort = 0L;
	DO.UpperPort = 0L;
	DO.UseTimeout = false;
	DO.Timeout = 0L;
	DO.ShowOutput = false;
	DO.DownloadToStartup = false;
	DO.ErrorCode = "";
	return DO;
}

// Internal function I use before testing a functions for bugs.
void DebugOutput(std::string Message, bool NeedsTesting, std::string Reason = "Does not return, or can't fail")
{
	std::string NeedsTest;
	if (NeedsTesting) {
		NeedsTest = "[Needs testing] ";
	}
	else {
		NeedsTest = "[Does not need testing] ";
	}
	std::cout << "[DEBUG] " << Message << " " << NeedsTest << " " << "[" << Reason << "]\n";
}

void SetupTerminal();
inline bool CheckFileExistence(const std::string& name);
bool CreateDir(std::string DirPath);
int ReadFile(std::string File, std::vector<std::string> VectorToSaveTo);
int WriteFileVector(std::string File, std::vector<std::string> Vector, bool ForceNewline);
std::string ChangeLineColor(int ForegroundColor, std::string Message);
long GetFileSize(std::string filename);
std::string ConvertToString(char* a);
void UseVirtualTerminal();
static std::string GetTime();
void WriteFile(std::string Filename, std::string DataToWrite, bool Overwrite);
void _LOG(std::string Message, int ErrorLevel = 0, bool OverwriteLog = true);
void InitializeLog();
long long GetExecutionTime(void(*f)(void));
long long BenchmarkTime();
void CopyDataToClipboard(std::string Data);
void BenchmarkAndClipboard();
size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
int DownloadFile(std::string URL, bool ToMemory, std::string DownloadFile, std::string DownloadPath, bool CreatePath);
bool IsNumber(const std::string& str);
std::vector<std::string> Split(const std::string& str, char delim);
bool IsValidIP(std::string ip);
void PortScanner();
std::string GetWiFiSignalStrength();
int GetWiFiInformation();
std::string NetworkTool(DownloadOptions DO);
bool CheckNetworkConnection();
bool CheckConnectionToServer(LPCWSTR ServerURL);
std::wstring s2ws(const std::string& str);
std::string ws2s(const std::wstring& wstr);
int StartProcess(LPCSTR lpApplicationName, std::string cmdLine, bool isWait, LPDWORD pExitCode);
HWND GetWindowHWND(std::wstring ToFind);
std::string GetLastErrorAsString();
std::string ReadRegistry(int Hive, const std::wstring& regSubKey, const std::wstring& regValue, bool ShowOutput);
void ClearConsoleScreen(HANDLE hConsole);
void BenchmarkThread();
void Benchmark();
/* End forward declarations */

// Just calls UseVirtualTerminal. Does the same exact thing as just calling UseVirtualTerminal. Was used when 
// these functions were a class, and UseVirtualTerminal was a private member.
void SetupTerminal()
{
	UseVirtualTerminal();
}

// Taken from stack overflow, checks if a file is accessible. Will return false
// if the file is locked by another process, or the context is invalid.
inline bool CheckFileExistence(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

// Creates a directory, and if the directory already exists, will return true. 
bool CreateDir(std::string DirPath)
{
	if (CheckFileExistence(DirPath)) {
		return true;
	}

	bool MakeDirRet = _mkdir(DirPath.c_str());

	if (MakeDirRet == EEXIST) {
		_LOG("Cannot create directory, already exists.", Sapphire::LOG_INFO);
		return true;
	}
	if (MakeDirRet == ENOENT) {
		_LOG("Cannot create directory, path missing.", Sapphire::LOG_ERROR);
		return false;
	}

	// Failback
	if (CheckFileExistence(DirPath)) {
		return true;
	}
	else {
		return false;
	}

}

// Reads a file line by line into a vector of strings. Has a maximum file size of ~512MB
int ReadFile(std::string File, std::vector<std::string> VectorToSaveTo)
{
	VectorToSaveTo.clear();
	if (CheckFileExistence(File) == false) {
		_LOG("Function: ReadFile. Result: Error, the file was missing or inaccessible under the current user context.", Sapphire::LOG_ERROR);
		return READFILE_MISSING;
	}
	if (GetFileSize(File) > 512000000) {
		_LOG("Function: ReadFile. Result: Error, the file size exceeded 512MB.");
		return READFILE_BAD_SIZE;
	}
	if (GetFileSize(File) == 0) {
		_LOG("Function: ReadFile. Result: Error, the file was empty.");
		return READFILE_EMPTY_FILE;
	}
	std::ifstream input(File);
	for (std::string line; getline(input, line);)
	{
		VectorToSaveTo.push_back(line);
	}
	_LOG("Function: ReadFile. Result: Successfully read the file to the vector.", Sapphire::LOG_INFO);
}

// Writes to a file from a vector of strings, with a maximum vector size of ~512MB
int WriteFileVector(std::string File, std::vector<std::string> Vector, bool ForceNewline)
{
	if (sizeof Vector > 512000000) {
		_LOG("Function: WriteFile. Result: Error, the vector's total size was greater then 512MB.", Sapphire::LOG_ERROR);
		return WRITEFILE_BAD_SIZE;
	}
	if (sizeof Vector == 0) {
		_LOG("Function: WriteFile. Result: Error, the vector was empty or cleared.", Sapphire::LOG_ERROR);
		return WRITEFILE_EMPTY_FILE;
	}
	std::ofstream myfile;
	myfile.open(File);
	for (int i = 0; i < Vector.size(); i++) {
		if (ForceNewline) {
			if (Vector[i].empty() == false) {
				std::string FinalChar = Vector[i].substr(Vector[i].size() - 1, Vector[i].size());
				std::string NewXline = "\n";
				if (strcmp(FinalChar.c_str(), NewXline.c_str()) != 0) {
					Vector[i].append("\n");
				}
			}
		}
		myfile << Vector[i];
	}
	myfile.close();
	_LOG("Function: WriteFile. Result: Successfully written the contents of the vector to the file.", Sapphire::LOG_INFO);
	return 0;
}

// Returns a formatted string for use with cout and etc, for printing colored strings to a terminal.
std::string ChangeLineColor(int ForegroundColor, std::string Message) {
	std::string Formatter = "\033[0;";
	Formatter.append(std::to_string(ForegroundColor));
	Formatter.append("m");
	Formatter.append(Message);
	Formatter.append("\033[0m");
	return Formatter;
}

// Simply gets the file size of the given filename.
long GetFileSize(std::string filename)
{
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

// Converts a char array to a string. 
std::string ConvertToString(char* a)
{
	int size = sizeof a;
	int i;
	std::string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

// Gets the console handle, and sets it to enable virtual terminal processing (Mainly for color support)
void UseVirtualTerminal()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}

// Gets the current system time as string. Used mainly for the _LOG function to timestamp.
std::string GetTime()
{
	auto start = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(start);
#pragma warning(disable : 4996)
	std::string ts = std::ctime(&t);
#pragma warning(default : 4996)
	ts.resize(ts.size() - 1);
	return ts;
}

// Writes data to a file from a string, with the option of overwriting the contents or not.
void WriteFile(std::string Filename, std::string DataToWrite, bool Overwrite)
{
	if (Overwrite) {
		std::ofstream myfile;
		myfile.open(Filename, std::ios::trunc);
		myfile << DataToWrite << std::endl;
		myfile.close();
	}
	else {
		std::ofstream myfile;
		myfile.open(Filename, std::ios::app);
		myfile << DataToWrite << std::endl;
		myfile.close();
	}
}

// Prints formatted data to a Log.txt located in the program's startup folder. 
// Automatically gets the current system time and appends that as well.
void _LOG(std::string Message, int ErrorLevel, bool OverwriteLog) {
	std::string StringTime = GetTime();

	std::string ErrorString = "[";
	ErrorString.append(StringTime);
	ErrorString.append("] ");
	switch (ErrorLevel) {
	case 0:
		ErrorString.append("[INFO] " + Message);
		break;
	case 1:
		ErrorString.append("[DEBUG] " + Message);
		break;
	case 2:
		ErrorString.append("[WARN] " + Message);
		break;
	case 3:
		ErrorString.append("[ERROR] " + Message);
		break;
	case 4:
		ErrorString.append("[UNRECOVERABLE ERROR] " + Message);
		break;
	default:
		ErrorString.append("[INFO] " + Message);
		break;
	}

	if (CheckFileExistence("Log.txt")) {
		if (OverwriteLog) {
			WriteFile("Log.txt", ErrorString, true);
		}
		else {
			WriteFile("Log.txt", ErrorString, false);
		}
	}
	else {
		WriteFile("Log.txt", ErrorString, false);
	}
}

// Clears out the previous log contents, and sets it up for further usage.
void InitializeLog()
{
	std::string StringTime = GetTime();

	std::string ErrorString = "[";
	ErrorString.append(StringTime);
	ErrorString.append("] ");
	ErrorString.append("[INIT] Log has been initialized.");
	WriteFile("Log.txt", ErrorString, true);
}

// I forgot why I made this actually. Will most likely be removed in the future.
struct EasyVector {
	std::vector<int> IntVector;
	std::vector<int64_t> Int64Vector;
	std::vector<int32_t> Int32Vector;
	std::vector<float> FloatVector;
	std::vector<double> DoubleVector;
	std::vector<long> LongVector;
	std::vector<long long> LongLongVector;
	std::vector<long double> LongDoubleVector;
	std::vector<unsigned int> UIntVector;
	std::vector<unsigned long> ULongVector;
};

// Measures the execution time of a *void function. 
long long GetExecutionTime(void(*f)(void))
{
	auto t1 = std::chrono::high_resolution_clock::now();
	f();
	auto t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	return duration;
}

// Gets the time it takes to run the libary's benchmark.
long long BenchmarkTime()
{
	auto t1 = std::chrono::high_resolution_clock::now();
	Benchmark();
	auto t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	duration = duration / processor_count;
	return duration;
}

// Copies data below the 32bit address space (in size) from a string to the system clipboard.
// Also used by the benchmarking function.
void CopyDataToClipboard(std::string Data)
{

	if (Data.length() > 2147483647) {
		throw std::runtime_error("Clipboard data greater than 32bit space.");
		return;
	}

	const char* output = Data.c_str();
	const size_t len = strlen(output) + 1;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	if (sizeof hMem == 0) {
		throw std::bad_alloc();
		return;
	}
	else {
		memcpy(GlobalLock(hMem), output, len);
	}
	GlobalUnlock(hMem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
}

// The actual benchmark function. This used to be threaded, but with current optimizations made
// externally to Windows 10's scheduler, now runs them sequentially 100 times, then
// averages the time it takes the run all of the benchmarks.
void BenchmarkAndClipboard()
{
	long long TotalTime = 0;
	long long BTime;

	auto t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100; i++) {
		BTime = BenchmarkTime();
		TotalTime = TotalTime + BTime;
	}
	auto t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	long long BenchDuration = duration;

	std::string BenchmarkData = "System configuration\n";
	BenchmarkData.append("Number of logical processors: ");
	BenchmarkData.append(std::to_string(processor_count));
	BenchmarkData.append("\nTime to benchmark: ");
	BenchmarkData.append(std::to_string(duration / 1000));
	BenchmarkData.append("\n");
	BenchmarkData.append("Benchmark score (Lower is better): ");
	BenchmarkData.append(std::to_string(TotalTime / 100));
	BenchmarkData.append(" -- Sapphire Library Benchmark V0.1");
	CopyDataToClipboard(BenchmarkData);
	std::cout << "Benchmark score: " << TotalTime / 100 << std::endl;
	std::cout << "The required information has been copied to your clipboard.\n";
}

const int CURL_BAD_URL = 4;
const int CURL_BAD_TRANSPORT = 5;
const int CURL_BAD_NAME = 6;
const int CURL_BAD_PATH = 7;

// Internal function used by NetworkTool and DownloadFile, for writing the data
// via fwrite.
size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

// Internal function used by NetworkTool and Downloadfile
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	try
	{
		return size * nmemb;
	}
	catch (const std::exception& e)
	{
		throw e.what();
	}

}

// Downloads a file to the specified directory with the specified name.
// Use CreatePath = true to create the path if the DownloadPath doesn't exist.
int DownloadFile(std::string URL, bool ToMemory, std::string DownloadFile, std::string DownloadPath, bool CreatePath)
{

	// Error checking
	if (URL.substr(0, 4) != "http") {
		_LOG("Function: DownloadFile. Result: Error, the URL passed didn't contain HTTP or HTTPS at the start.", Sapphire::LOG_ERROR);
		return CURL_BAD_URL;
	}

	if (!ToMemory) {

		if (CheckFileExistence(DownloadPath) != true) {
			if (CreatePath == false) {
				_LOG("Function: DownloadFile. Result: Error, the directory to save to didn't exist, CreatePath was false.", Sapphire::LOG_ERROR);
				return CURL_BAD_PATH;
			}
			else {
				if (CreateDir(DownloadPath) != true) {
					_LOG("Function: DownloadFile. Result: Error, the directory to save to didn't exist, and couldn't be created.", Sapphire::LOG_ERROR);
					return CURL_BAD_PATH;
				}
			}
		}
		std::string EndOfPath = DownloadPath.substr(DownloadPath.size() - 1, DownloadPath.size());
		// Check if the dir supplied came with a trailing slash or not
		if (strcmp(EndOfPath.c_str(), "/") != 0) {
			if (strcmp(EndOfPath.c_str(), "\\") != 0) {
				DownloadPath.append("/");
			}
		}
	}

	int ReturnValue = 0;

	CURL* curl;
	FILE* fp;
#pragma warning(suppress: 26812)
	CURLcode res;

	std::string url = URL;
	std::string DownloadLocation = DownloadPath;
	DownloadLocation.append(DownloadFile);
	char CustomDownload[260];
	strcpy(CustomDownload, DownloadLocation.c_str());
	curl = curl_easy_init();
	if (curl) {
		if (ToMemory) {
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			_LOG(curl_easy_strerror(res), Sapphire::LOG_INFO);
		}
		else {
			fp = fopen(CustomDownload, "wb");
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			fclose(fp);

			bool IsFileBlocked = true;
			std::fstream filestr;
			filestr.open(DownloadLocation.c_str());

			while (IsFileBlocked) {

				if (filestr.is_open())
				{
					if (filestr.peek() == std::ifstream::traits_type::eof())
					{
						filestr.close();
						std::remove(DownloadLocation.c_str());
						return -1;
					}
					filestr.close();
					IsFileBlocked = false;
				}
				_LOG("Function: DownloadFile. Result: Successfully downloaded the file.", Sapphire::LOG_INFO);
				return ReturnValue;
			}
			return 0;
		}
	}
	return 0;
}

// Checks if the string provided only contains numbers.
bool IsNumber(const std::string& str)
{
	return !str.empty() &&
		(str.find_first_not_of("[0123456789]") == std::string::npos);
}

// Splits a string based on delimiter, and returns a vector containing the split strings.
// Isn't very efficient, and isn't a map, so unless the user knows how many times the
// string will split, is pretty useless. Will possibly be removed in the future.
std::vector<std::string> Split(const std::string& str, char delim)
{
	auto i = 0;
	std::vector<std::string> list;
	auto pos = str.find(delim);
	while (pos != std::string::npos) {
		list.push_back(str.substr(i, pos - i));
		i = ++pos;
		pos = str.find(delim, pos);
	}

	list.push_back(str.substr(i, str.length()));

	return list;
}

// Checks if the supplied string is a valid IPv4 address.
bool IsValidIP(std::string ip)
{
	std::vector<std::string> list = Split(ip, '.');
	if (list.size() != 4) {
		return false;
	}

	for (std::string str : list)
	{
		if (!IsNumber(str) || stoi(str) > 255 || stoi(str) < 0)
			return false;
	}

	return true;
}

// I made this a long time ago, and pretty sure it doesn't work anymore. 
// After testing, I will either fix it, or remove it.
void PortScanner()
{
	DownloadOptions DO;

	if (!IsValidIP(DO.IP)) {
		ChangeLineColor(RED, "Invalid IP.");
		return;
	}

	if (DO.LowerPort < 0 || DO.LowerPort > 65535) {
		ChangeLineColor(RED, "Invalid low port.");
		return;
	}

	if (DO.UpperPort < 0 || DO.UpperPort > 65535) {
		ChangeLineColor(RED, "Invalid low port.");
		return;
	}
#pragma warning(suppress: 6287)
	if (DO.LowerPort > DO.UpperPort || DO.UpperPort < DO.LowerPort) {
		ChangeLineColor(RED, "Invalid port range. Port range was inverted.");
		return;
	}

	if (DO.LowerPort == DO.UpperPort) {
		DO.IsSinglePort = true;
	}


	std::string IsOpen;
	int NumberOfClosedPorts = 0;
	int NumberOfOpenPorts = 0;

	if (DO.IsSinglePort) {
		IsOpen = NetworkTool(DO);
		if (IsOpen == "No error" || IsOpen == "Unsupported protocol") {
			std::string MessageBuilder = "Port ";
			MessageBuilder.append(std::to_string(DO.LowerPort));
			MessageBuilder.append(" is open on target ");
			MessageBuilder.append(DO.IP);
			MessageBuilder.append(".");
			ChangeLineColor(GREEN, MessageBuilder);
		}
		else {
			std::string MessageBuilder = "Port ";
			MessageBuilder.append(std::to_string(DO.LowerPort));
			MessageBuilder.append(" is closed/ filtered on target ");
			MessageBuilder.append(DO.IP);
			MessageBuilder.append(".");
			ChangeLineColor(RED, MessageBuilder);
		}
	}
	else {
		for (int i = DO.LowerPort; i < DO.UpperPort + 1; i++) {
			DO.Port = i;
			if (DO.ShowOutput) {
				ChangeLineColor(GREEN, "Scanning port: " + std::to_string(i));
			}
			IsOpen = NetworkTool(DO);
			if (IsOpen == "No error" || IsOpen == "Unsupported protocol") {
				std::string MessageBuilder = "Port ";
				MessageBuilder.append(std::to_string(i));
				MessageBuilder.append(" is open on target ");
				MessageBuilder.append(DO.IP);
				MessageBuilder.append(".");
				ChangeLineColor(GREEN, MessageBuilder);
				NumberOfOpenPorts++;
			}
			else {
				if (DO.ShowOutput) {
					std::string MessageBuilder = "Port ";
					MessageBuilder.append(std::to_string(i));
					MessageBuilder.append(" is closed on target ");
					MessageBuilder.append(DO.IP);
					MessageBuilder.append(".");
					MessageBuilder.append(" Error was: ");
					MessageBuilder.append(IsOpen);
					ChangeLineColor(RED, MessageBuilder);
				}
				NumberOfClosedPorts++;
			}
		}
	}

	std::string MessageBuilderX = "Number of open ports on target ";
	MessageBuilderX.append(DO.IP);
	MessageBuilderX.append(": ");
	MessageBuilderX.append(std::to_string(NumberOfOpenPorts));
	ChangeLineColor(WHITE, MessageBuilderX);
	MessageBuilderX = "Number of closed ports on target ";
	MessageBuilderX.append(DO.IP);
	MessageBuilderX.append(": ");
	MessageBuilderX.append(std::to_string(NumberOfClosedPorts));
	ChangeLineColor(WHITE, MessageBuilderX);
	ResetDownloadOptions(DO);
}

// Simply returns the current signal strength of the first connected adapter.
std::string GetWiFiSignalStrength()
{
	HANDLE hClient;
	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
	PWLAN_INTERFACE_INFO pIfConnInfo = NULL;
	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;

	PWLAN_BSS_LIST pBssList = NULL;
	PWLAN_BSS_ENTRY  pBssEntry = NULL;
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;

	DWORD dwResult = 0;
	DWORD dwMaxClient = 2;
	DWORD dwCurVersion = 0;
	DWORD connectInfoSize = sizeof(WLAN_CONNECTION_ATTRIBUTES);

	int i;

	// Initialise the Handle
	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS)
	{
		return "Failed handle open.";
	}

	// Get the Interface List
	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if (dwResult != ERROR_SUCCESS)
	{
		return "Failed IFaceList.";
	}

	//Loop through the List to find the connected Interface
	PWLAN_INTERFACE_INFO pIfInfo = NULL;
	for (i = 0; i < (int)pIfList->dwNumberOfItems; i++)
	{
		pIfInfo = (WLAN_INTERFACE_INFO*)&pIfList->InterfaceInfo[i];
		if (pIfInfo->isState == wlan_interface_state_connected)
		{
			pIfConnInfo = pIfInfo;
			break;
		}
	}

	if (pIfConnInfo == NULL)
		return "Failed ConnInfo.";

	// Query the Interface
	dwResult = WlanQueryInterface(hClient, &pIfConnInfo->InterfaceGuid, wlan_intf_opcode_current_connection, NULL, &connectInfoSize, (PVOID*)&pConnectInfo, &opCode);
	if (dwResult != ERROR_SUCCESS)
	{
		return "Failed query.";
	}

	// Scan the connected SSID
	dwResult = WlanScan(hClient, &pIfConnInfo->InterfaceGuid, &pConnectInfo->wlanAssociationAttributes.dot11Ssid, NULL, NULL);
	if (dwResult != ERROR_SUCCESS)
	{
		return "Failed scan.";
	}

	// Get the BSS Entry
	dwResult = WlanGetNetworkBssList(hClient, &pIfConnInfo->InterfaceGuid, &pConnectInfo->wlanAssociationAttributes.dot11Ssid, dot11_BSS_type_infrastructure, TRUE, NULL, &pBssList);

	if (dwResult != ERROR_SUCCESS)
	{
		return "Failed BSS list.";
	}

	// Get the RSSI value
	pBssEntry = &pBssList->wlanBssEntries[0];
	return std::to_string(pBssEntry->lRssi);
}

// Gets and prints information regarding the NIC and WiFi currently used.
int GetWiFiInformation()
{

	// Declare and initialize variables.

	HANDLE hClient = NULL;
	DWORD dwMaxClient = 2;      //    
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;
	DWORD dwRetVal = 0;
	int iRet = 0;

	WCHAR GuidString[39] = { 0 };

	unsigned int i, k;

	// variables used for WlanEnumInterfaces

	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
	PWLAN_INTERFACE_INFO pIfInfo = NULL;

	// variables used for WlanQueryInterfaces for opcode = wlan_intf_opcode_current_connection
	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
	DWORD connectInfoSize = sizeof(WLAN_CONNECTION_ATTRIBUTES);
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		return 1;
		// You can use FormatMessage here to find out why the function failed
	}

	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
		return 1;
		// You can use FormatMessage here to find out why the function failed
	}
	else {
		wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems);
		wprintf(L"Current Index: %lu\n", pIfList->dwIndex);
		for (i = 0; i < (int)pIfList->dwNumberOfItems; i++) {
			pIfInfo = (WLAN_INTERFACE_INFO*)&pIfList->InterfaceInfo[i];
			wprintf(L"  Interface Index[%u]:\t %lu\n", i, i);
			iRet =
				StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR)&GuidString,
					sizeof(GuidString) / sizeof(*GuidString));
			// For c rather than C++ source code, the above line needs to be
			// iRet = StringFromGUID2(&pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 
			//     sizeof(GuidString)/sizeof(*GuidString)); 
			if (iRet == 0)
				wprintf(L"StringFromGUID2 failed\n");
			else {
				wprintf(L"  InterfaceGUID[%d]:\t %ws\n", i, GuidString);
			}
			wprintf(L"  Interface Description[%d]: %ws", i, pIfInfo->strInterfaceDescription);
			wprintf(L"\n");
			wprintf(L"  Interface State[%d]:\t ", i);
			switch (pIfInfo->isState) {
			case wlan_interface_state_not_ready:
				wprintf(L"Not ready\n");
				break;
			case wlan_interface_state_connected:
				wprintf(L"Connected\n");
				break;
			case wlan_interface_state_ad_hoc_network_formed:
				wprintf(L"First node in a ad hoc network\n");
				break;
			case wlan_interface_state_disconnecting:
				wprintf(L"Disconnecting\n");
				break;
			case wlan_interface_state_disconnected:
				wprintf(L"Not connected\n");
				break;
			case wlan_interface_state_associating:
				wprintf(L"Attempting to associate with a network\n");
				break;
			case wlan_interface_state_discovering:
				wprintf(L"Auto configuration is discovering settings for the network\n");
				break;
			case wlan_interface_state_authenticating:
				wprintf(L"In process of authenticating\n");
				break;
			default:
				wprintf(L"Unknown state %ld\n", pIfInfo->isState);
				break;
			}
			wprintf(L"\n");

			// If interface state is connected, call WlanQueryInterface
			// to get current connection attributes
			if (pIfInfo->isState == wlan_interface_state_connected) {
				dwResult = WlanQueryInterface(hClient,
					&pIfInfo->InterfaceGuid,
					wlan_intf_opcode_current_connection,
					NULL,
					&connectInfoSize,
					(PVOID*)&pConnectInfo,
					&opCode);

				if (dwResult != ERROR_SUCCESS) {
					wprintf(L"WlanQueryInterface failed with error: %u\n", dwResult);
					dwRetVal = 1;
					// You can use FormatMessage to find out why the function failed
				}
				else {
					wprintf(L"  WLAN_CONNECTION_ATTRIBUTES for this interface\n");

					wprintf(L"  Interface State:\t ");
					switch (pConnectInfo->isState) {
					case wlan_interface_state_not_ready:
						wprintf(L"Not ready\n");
						break;
					case wlan_interface_state_connected:
						wprintf(L"Connected\n");
						break;
					case wlan_interface_state_ad_hoc_network_formed:
						wprintf(L"First node in a ad hoc network\n");
						break;
					case wlan_interface_state_disconnecting:
						wprintf(L"Disconnecting\n");
						break;
					case wlan_interface_state_disconnected:
						wprintf(L"Not connected\n");
						break;
					case wlan_interface_state_associating:
						wprintf(L"Attempting to associate with a network\n");
						break;
					case wlan_interface_state_discovering:
						wprintf
						(L"Auto configuration is discovering settings for the network\n");
						break;
					case wlan_interface_state_authenticating:
						wprintf(L"In process of authenticating\n");
						break;
					default:
						wprintf(L"Unknown state %ld\n", pIfInfo->isState);
						break;
					}

					wprintf(L"  Connection Mode:\t ");
					switch (pConnectInfo->wlanConnectionMode) {
					case wlan_connection_mode_profile:
						wprintf(L"A profile is used to make the connection\n");
						break;
					case wlan_connection_mode_temporary_profile:
						wprintf(L"A temporary profile is used to make the connection\n");
						break;
					case wlan_connection_mode_discovery_secure:
						wprintf(L"Secure discovery is used to make the connection\n");
						break;
					case wlan_connection_mode_discovery_unsecure:
						wprintf(L"Unsecure discovery is used to make the connection\n");
						break;
					case wlan_connection_mode_auto:
						wprintf
						(L"connection initiated by wireless service automatically using a persistent profile\n");
						break;
					case wlan_connection_mode_invalid:
						wprintf(L"Invalid connection mode\n");
						break;
					default:
						wprintf(L"Unknown connection mode %ld\n",
							pConnectInfo->wlanConnectionMode);
						break;
					}

					wprintf(L"  Profile name used:\t %ws\n", pConnectInfo->strProfileName);

					wprintf(L"  Association Attributes for this connection\n");
					wprintf(L"    SSID:\t\t ");
					if (pConnectInfo->wlanAssociationAttributes.dot11Ssid.uSSIDLength == 0)
						wprintf(L"\n");
					else {
						for (k = 0;
							k < pConnectInfo->wlanAssociationAttributes.dot11Ssid.uSSIDLength;
							k++) {
							wprintf(L"%c",
								(int)pConnectInfo->wlanAssociationAttributes.dot11Ssid.
								ucSSID[k]);
						}
						wprintf(L"\n");
					}

					wprintf(L"    BSS Network type:\t ");
					switch (pConnectInfo->wlanAssociationAttributes.dot11BssType) {
					case dot11_BSS_type_infrastructure:
						wprintf(L"Infrastructure\n");
						break;
					case dot11_BSS_type_independent:
						wprintf(L"Infrastructure\n");
						break;
					default:
						wprintf(L"Other = %lu\n",
							pConnectInfo->wlanAssociationAttributes.dot11BssType);
						break;
					}

					wprintf(L"    MAC address:\t ");
					for (k = 0; k < sizeof(pConnectInfo->wlanAssociationAttributes.dot11Bssid);
						k++) {
						if (k == 5)
							wprintf(L"%.2X\n",
								pConnectInfo->wlanAssociationAttributes.dot11Bssid[k]);
						else
							wprintf(L"%.2X-",
								pConnectInfo->wlanAssociationAttributes.dot11Bssid[k]);
					}

					wprintf(L"    PHY network type:\t ");
					switch (pConnectInfo->wlanAssociationAttributes.dot11PhyType) {
					case dot11_phy_type_fhss:
						wprintf(L"Frequency-hopping spread-spectrum (FHSS)\n");
						break;
					case dot11_phy_type_dsss:
						wprintf(L"Direct sequence spread spectrum (DSSS)\n");
						break;
					case dot11_phy_type_irbaseband:
						wprintf(L"Infrared (IR) baseband\n");
						break;
					case dot11_phy_type_ofdm:
						wprintf(L"Orthogonal frequency division multiplexing (OFDM)\n");
						break;
					case dot11_phy_type_hrdsss:
						wprintf(L"High-rate DSSS (HRDSSS) = \n");
						break;
					case dot11_phy_type_erp:
						wprintf(L"Extended rate PHY type\n");
						break;
					case dot11_phy_type_ht:
						wprintf(L"802.11n PHY type\n");
						break;
					default:
						wprintf(L"Unknown = %lu\n",
							pConnectInfo->wlanAssociationAttributes.dot11PhyType);
						break;
					}

					wprintf(L"    PHY index:\t\t %u\n",
						pConnectInfo->wlanAssociationAttributes.uDot11PhyIndex);

					wprintf(L"    Signal Quality:\t %d\n",
						pConnectInfo->wlanAssociationAttributes.wlanSignalQuality);

					wprintf(L"    Receiving Rate:\t %ld\n",
						pConnectInfo->wlanAssociationAttributes.ulRxRate);

					wprintf(L"    Transmission Rate:\t %ld\n",
						pConnectInfo->wlanAssociationAttributes.ulTxRate);
					wprintf(L"\n");

					wprintf(L"  Security Attributes for this connection\n");

					wprintf(L"    Security enabled:\t ");
					if (pConnectInfo->wlanSecurityAttributes.bSecurityEnabled == 0)
						wprintf(L"No\n");
					else
						wprintf(L"Yes\n");

					wprintf(L"    802.1X enabled:\t ");
					if (pConnectInfo->wlanSecurityAttributes.bOneXEnabled == 0)
						wprintf(L"No\n");
					else
						wprintf(L"Yes\n");

					wprintf(L"    Authentication Algorithm: ");
					switch (pConnectInfo->wlanSecurityAttributes.dot11AuthAlgorithm) {
					case DOT11_AUTH_ALGO_80211_OPEN:
						wprintf(L"802.11 Open\n");
						break;
					case DOT11_AUTH_ALGO_80211_SHARED_KEY:
						wprintf(L"802.11 Shared\n");
						break;
					case DOT11_AUTH_ALGO_WPA:
						wprintf(L"WPA\n");
						break;
					case DOT11_AUTH_ALGO_WPA_PSK:
						wprintf(L"WPA-PSK\n");
						break;
					case DOT11_AUTH_ALGO_WPA_NONE:
						wprintf(L"WPA-None\n");
						break;
					case DOT11_AUTH_ALGO_RSNA:
						wprintf(L"RSNA\n");
						break;
					case DOT11_AUTH_ALGO_RSNA_PSK:
						wprintf(L"RSNA with PSK\n");
						break;
					default:
						wprintf(L"Other (%lu)\n", pConnectInfo->wlanSecurityAttributes.dot11AuthAlgorithm);
						break;
					}

					wprintf(L"    Cipher Algorithm:\t ");
					switch (pConnectInfo->wlanSecurityAttributes.dot11CipherAlgorithm) {
					case DOT11_CIPHER_ALGO_NONE:
						wprintf(L"None\n");
						break;
					case DOT11_CIPHER_ALGO_WEP40:
						wprintf(L"WEP-40\n");
						break;
					case DOT11_CIPHER_ALGO_TKIP:
						wprintf(L"TKIP\n");
						break;
					case DOT11_CIPHER_ALGO_CCMP:
						wprintf(L"CCMP\n");
						break;
					case DOT11_CIPHER_ALGO_WEP104:
						wprintf(L"WEP-104\n");
						break;
					case DOT11_CIPHER_ALGO_WEP:
						wprintf(L"WEP\n");
						break;
					default:
						wprintf(L"Other (0x%x)\n", pConnectInfo->wlanSecurityAttributes.dot11CipherAlgorithm);
						break;
					}
					wprintf(L"\n");
				}
			}
		}

	}
	if (pConnectInfo != NULL) {
		WlanFreeMemory(pConnectInfo);
		pConnectInfo = NULL;
	}

	if (pIfList != NULL) {
		WlanFreeMemory(pIfList);
		pIfList = NULL;
	}

	return dwRetVal;
}

// The best function ever made.
// Naw, it's a generic cURL function using my struct, which just downloads the page/ item, optionally to memory.
std::string NetworkTool(DownloadOptions DO)
{

	// DownloadOptions DO;

	CURL* curl;
	CURLcode res;
	if (DO.IP == "" && DO.URL == "") {
		std::cout << "No host selected." << std::endl;
	}
	if (DO.IP.length() > 0 && DO.URL == "") {
		DO.URL = DO.IP;
	}

	std::string url = DO.URL;
	std::string ref = DO.Referrer;

	curl = curl_easy_init();
	if (curl) {

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		if (DO.SkipVerification) {
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		}
		if (DO.SkipHostnameVerification) {
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		}
		if (DO.UseTimeout == true) {
			curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, DO.Timeout);
		}

		if (DO.UseCustomPort == true) {
			curl_easy_setopt(curl, CURLOPT_PORT, DO.Port);
		}

		if (std::strcmp(DO.Referrer.c_str(), "") != 0) {
			curl_easy_setopt(curl, CURLOPT_REFERER, ref.c_str());
		}

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		try
		{
			res = curl_easy_perform(curl);
		}
		catch (const std::bad_alloc&)
		{
			std::cout << ChangeLineColor(RED, "Critical error, couldn't allocate memory for operation.\n");
		}

		DO.ErrorCode = curl_easy_strerror(res);
		curl_easy_cleanup(curl);

	}
	// Assume the shit is finished, and print out the information for debugging purposes.
	if (DO.ShowOutput == true) {
		if (_CrtCheckMemory()) {
			std::cout << "Passed memory check." << std::endl;
		}
		else {
			std::cerr << "Failed memory check. Exiting immediately." << std::endl;
			exit(-99);
		}
	}

	return DO.ErrorCode;
}

// Checks if an internet connection is available
bool CheckNetworkConnection()
{
	LPCWSTR URLTest = L"http://google.com/";
	bool ConnectionTest = InternetCheckConnection(URLTest, FLAG_ICC_FORCE_CONNECTION, 0);
	if (ConnectionTest) {
		return true;
	}
	else {
		return false;
	}
}

// Checks if a specific server can be contacted.
bool CheckConnectionToServer(LPCWSTR ServerURL)
{
	bool ConnectionTest = InternetCheckConnection(ServerURL, FLAG_ICC_FORCE_CONNECTION, 0);
	if (ConnectionTest) {
		return true;
	}
	else {
		return false;
	}
}

// Converts a string to a wide string (std::string to std::wstring)
std::wstring s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	_LOG("Function: s2ws. Result: Success / No relative return.", Sapphire::LOG_INFO);

	return converterX.from_bytes(str);
}

// Converts a wstring to a string
std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	_LOG("Function: ws2s. Result: Success / No relative return.", Sapphire::LOG_INFO);

	return converterX.to_bytes(wstr);
}

// Starts a process.
int StartProcess(LPCSTR lpApplicationName, std::string cmdLine, bool isWait, LPDWORD pExitCode)
{

	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	::ZeroMemory(&si, sizeof(si));

	si.cb = sizeof(si);
	::ZeroMemory(&pi, sizeof(pi));

	// reset last error
	::SetLastError(0);
	// Start the child process. 
	BOOL bCreateProcess = ::CreateProcessA(lpApplicationName, // No module name (use command line)
		(LPSTR)cmdLine.c_str(),  // Command line 
		NULL,                     // Process handle not inheritable
		NULL,                     // Thread handle not inheritable
		FALSE,                    // Set handle inheritance to FALSE
		CREATE_NO_WINDOW,         // No creation flags
		NULL,                     // Use parent's environment block
		NULL,                     // Use parent's starting directory 
		&si,                      // Pointer to STARTUPINFO structure
		&pi);                     // Pointer to PROCESS_INFORMATION structure

	if (!bCreateProcess)
	{
		// create process failed,
		//Logger::trace(error, getClassName(), "createProcess", getFormattedStringA("create process failed with error:%d, 
		// Commad line:'%s',isWait:%d",GetLastError(), cmdLine.c_str(), isWait),"CreateProcess Failed");
		_LOG("Function: StartProcess. Result: Failure. Create process failed. Tracer unavailable.", Sapphire::LOG_ERROR);
		return 0;
	}

	//Logger::trace(info, getClassName(), "createProcess", getFormattedStringA("created process,Commad line:'%s',isWait:%d,Result:%d", cmdLine.c_str(), isWait,bCreateProcess),"Launched Process");
	// Wait until child process exits.
	if (isWait)
	{
		::WaitForSingleObject(pi.hProcess, INFINITE);
		if (pExitCode)
		{
			::GetExitCodeProcess(pi.hProcess, pExitCode);
		}
	}
	::CloseHandle(pi.hProcess);
	pi.hProcess = NULL;
	::CloseHandle(pi.hThread);
	pi.hThread = NULL;
	_LOG("Function: StartProcess. Result: Thread executed successfully.", Sapphire::LOG_INFO);
	return 1; // return non zero. function succeeded

}

// Gets the HWND of a window
HWND GetWindowHWND(std::wstring ToFind)
{
	HWND Find = ::FindWindowExW(0, 0, ToFind.c_str(), 0);
	return Find;
}

// Gets the last reported Windows error, and returns it as a string.
std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

// Reads a specific registry entry, and returns the value as a string
std::string ReadRegistry(int Hive, const std::wstring& regSubKey, const std::wstring& regValue, bool ShowOutput)
{
	size_t bufferSize = 0xFFF; // If too small, will be resized down below.
	std::wstring valueBuf; // Contiguous buffer since C++11.
	valueBuf.resize(bufferSize);
	auto cbData = static_cast<DWORD>(bufferSize * sizeof(wchar_t));
	auto rc = RegGetValueW(HKEY_CLASSES_ROOT, regSubKey.c_str(), regValue.c_str(), RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);
	switch (Hive)
	{
	case 0: {
		rc = RegGetValueW(HKEY_CLASSES_ROOT, regSubKey.c_str(), regValue.c_str(), RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);
		break;
	}
	case 1: {
		rc = RegGetValueW(HKEY_CURRENT_USER, regSubKey.c_str(), regValue.c_str(), RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);
		break;
	}
	case 2: {
		rc = RegGetValueW(HKEY_LOCAL_MACHINE, regSubKey.c_str(), regValue.c_str(), RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);
		break;
	}
	case 3: {
		rc = RegGetValueW(HKEY_USERS, regSubKey.c_str(), regValue.c_str(), RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);
		break;
	}
	case 4: {
		rc = RegGetValueW(HKEY_CURRENT_CONFIG, regSubKey.c_str(), regValue.c_str(), RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);
		break;
	}
	default:
		return "FAIL_NO_HIVE";
		break;
	}

	while (rc == ERROR_MORE_DATA)
	{
		// Get a buffer that is big enough.
		cbData /= sizeof(wchar_t);
		if (cbData > static_cast<DWORD>(bufferSize))
		{
			bufferSize = static_cast<size_t>(cbData);
		}
		else
		{
			bufferSize *= 2;
			cbData = static_cast<DWORD>(bufferSize * sizeof(wchar_t));
		}
		valueBuf.resize(bufferSize);
		switch (Hive)
		{
		case 0: {
			rc = RegGetValueW(HKEY_CLASSES_ROOT, regSubKey.c_str(), regValue.c_str(), RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);
			break;
		}
		case 1: {
			rc = RegGetValueW(HKEY_CURRENT_USER, regSubKey.c_str(), regValue.c_str(), RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);
			break;
		}
		case 2: {
			rc = RegGetValueW(HKEY_LOCAL_MACHINE, regSubKey.c_str(), regValue.c_str(), RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);
			break;
		}
		case 3: {
			rc = RegGetValueW(HKEY_USERS, regSubKey.c_str(), regValue.c_str(), RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);
			break;
		}
		case 4: {
			rc = RegGetValueW(HKEY_CURRENT_CONFIG, regSubKey.c_str(), regValue.c_str(), RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);
			break;
		}
		default:
			return "FAIL_NO_HIVE";
			break;
		}
	}

	switch (rc)
	{
	case ERROR_SUCCESS:
		cbData /= sizeof(wchar_t);
		valueBuf.resize(static_cast<size_t>(cbData - 1)); // remove end null character
		return ws2s(valueBuf);
		break;
	case ERROR_FILE_NOT_FOUND:
		if (ShowOutput == true) {
			std::cerr << "[RegistryTools] [Read] Registry key not found" << std::endl;
		}
		return REGTOOLS_NO_FILE;
		break;
	case ERROR_FILE_CHECKED_OUT:
		if (ShowOutput) {
			std::cerr << "[RegistryTools] [Read] Registry key checked out. What?" << std::endl;
		}
		return REGTOOLS_CHECKED_OUT;
		break;
	case ERROR_FILE_CORRUPT:
		if (ShowOutput) {
			std::cerr << "[RegistryTools] [Read] Registry key corrupt. This should never happen." << std::endl;
		}
		return REGTOOLS_CORRUPT;
		break;
	default:
		break;
	}
	throw std::runtime_error("[RegistryTools] [Read] Unknown error. Code: " + std::to_string(rc));
	return REGTOOLS_UNKNOWN;
}

// Clears the terminal
void ClearConsoleScreen(HANDLE hConsole)
{
	COORD coordScreen = { 0, 0 };    /* here's where we'll home the
										cursor */
	BOOL bSuccess;
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
	DWORD dwConSize;                 /* number of character cells in
										the current buffer */

										/* get the number of character cells in the current buffer */

	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
	// PERR(bSuccess, "GetConsoleScreenBufferInfo");
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	/* fill the entire screen with blanks */

	bSuccess = FillConsoleOutputCharacter(hConsole, (TCHAR)' ',
		dwConSize, coordScreen, &cCharsWritten);
	// PERR(bSuccess, "FillConsoleOutputCharacter");

	/* get the current text attribute */

	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
	// PERR(bSuccess, "ConsoleScreenBufferInfo");

	/* now set the buffer's attributes accordingly */

	bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
		dwConSize, coordScreen, &cCharsWritten);
	// PERR(bSuccess, "FillConsoleOutputAttribute");

	/* put the cursor at (0, 0) */

	bSuccess = SetConsoleCursorPosition(hConsole, coordScreen);
	// PERR(bSuccess, "SetConsoleCursorPosition");
	return;
}

int FinishedThreads = 0;
bool AllThreadsFinished = false;
bool DoneSpawning = false;

void BenchmarkThread()
{

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 64000);

	std::vector<int> VectToSort;
	for (int i = 0; i < 16300; i++) {
		VectToSort.push_back(dist6(rng));
	}
	std::sort(VectToSort.begin(), VectToSort.end());
	FinishedThreads++;
}

void Benchmark()
{
	for (int i = 0; i < processor_count; i++) {
		std::thread Benchmarker(&BenchmarkThread);
		Benchmarker.join();
	}
}

};