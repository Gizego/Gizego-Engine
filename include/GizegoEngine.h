/*
	Build this with at least C++11.
	Some functions won't work if you aren't on windows while compiling.
	Most of them are in the Hardware  

	Credits to some libraries:

		- GLAD (glad.dav1d.de)
		- GLFW (www.glfw.org)
		- OpenAL-soft (github.com/kcat/openal-soft)
		- STB public domain libraries (github.com/nothings/stb)

	A special thanks to Karl Wimble (ThinMatrix), for the OpenGL tutorials that made
	the creation of this game engine possible.

	LICENSE:
		This game framework is public domain, but I thank you for appreciation.
*/

#define GLEW_STATIC
#ifndef GIZEGOENGINE_H //#IMPORTANT - Requires C++11
#define GIZEGOENGINE_H
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
//SYSTEM
#include <wchar.h> 
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <locale>
#include <fstream>
#include <string>
#include <codecvt>
#include <chrono>
#include <vector>
#include <atomic>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <future>

//WINDOWS
#ifdef _WIN32
#include <Windows.h>
#endif

//OPENGL
#include "glad/glad.h"
#include "glfw/glfw3.h"
#define VERTEX_TYPE GL_FLOAT
#define HALF_VERT float

//AUDIO
#include "al/al.h"
#include "al/alc.h"

//STB
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_TRUETYPE_IMPLEMENTATION 
#include "stb/stb_truetype.h"
#include "stb/tinydir.h"

namespace GizegoEngine
{
	//Represents the gravity of an exception
	enum ExceptionGravity : char
	{
		Error, Warning
	};

	//DO NOT USE.
	void(*ExHandler)(std::wstring, ExceptionGravity);

	//Throws an GizegoEngine exception. You can use it but it is recommended you don't.
	inline void ThrowException(std::wstring description, ExceptionGravity gravity = ExceptionGravity::Error)
	{
		if (ExHandler)
			ExHandler(description, gravity);
	}

	//RGBA color.
	class Color
	{
	public:
		Color() {}
		//Creates a color.
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
			R = r;
			G = g;
			B = b;
			A = a;
		}

		//Returns an OpenGL color. Don't use this unless you are working with OpenGL directly but if so, delete the pointer after using it.
		inline float* ToGL()
		{
			return new float[4]{ R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f };
		}

		~Color() {}

		unsigned char R, G, B, A;
	};

	//A component with an x and y.
	struct vec2
	{
		int x, y;
		vec2() { x = 0; y = 0; }
		vec2(int x, int y) { this->x = x; this->y = y; }
	
	};
	//Like a vec2 but with floats and used for object scaling
	struct vecf
	{
		float x, y;
		vecf() { x = 0; y = 0; }
		vecf(float x, float y) { this->x = x; this->y = y; }
	};

	//A component with width and height.
	struct Size
	{
		unsigned int width, height;
		Size() { width = 0; height = 0; }
		Size(unsigned int width, unsigned int height) { this->width = width; this->height = height; }
	};

	//The random number generator
	namespace Random
	{
		// Sets the seed of the random number generator
		inline void SetSeed(unsigned int seed)
		{
			srand(seed);
		}

		// Creates a random seed based in time(0)
		inline void RandomSeed()
		{
			srand((unsigned int)time(0));
			srand(rand());
		}

		// Returns the next integer that the random number generator creates
		inline int NextInteger()
		{
			return rand();
		}

		// Returns the next integer that the random number generator creates between min and max
		inline int NextInteger(int min, int max)
		{
			return (rand() % (max + 1)) + min;
		}

		// Returns the next double between 0 and 1 that the random number generator creates
		inline double NextDouble()
		{
			return NextInteger(0, 10000) / 10000.00;
		}
	}

	//All function inside this namespace are case sensitive
	namespace StringTools
	{
		// Returns GetPart(string, index, index + compare.size()) == compare but using another algorithm for better performance.
		bool ComparePart(std::wstring& string, int index, std::wstring compare)
		{
			if (compare.length() <= string.length())
			{
				for (unsigned int ostart = index; ostart - index < compare.length(); ++ostart)
				{
					if (string[ostart] != compare[ostart - index])
						return false;
				}
				return true;
			}
			return false;
		}

		//Returns the first index of a character in a string. Returns -1 if the character is not found.
		int FirstIndexOf(std::wstring& string, wchar_t ch)
		{
			for (unsigned int i = 0; i < string.length(); ++i)
			{
				if (ch == string[i])
					return i;
			}
			return -1;
		}

		//Returns the first index of a character in a string starting from int start. Returns -1 if the character is not found.
		int FirstIndexOf(std::wstring& string, wchar_t ch, int start)
		{
			for (unsigned int i = start; i < string.length(); ++i)
			{
				if (ch == string[i])
					return i;
			}
			return -1;
		}

		//Returns the first index of string tofind in a string. Returns -1 if tofind is not found.
		int FirstIndexOf(std::wstring& string, std::wstring tofind)
		{
			for (unsigned int i = 0; i < string.length(); ++i)
			{
				if (ComparePart(string, i, tofind))
					return i;
			}
			return -1;
		}

		//Returns the first index of string tofind in a string starting from int start. Returns -1 if tofind is not found.
		int FirstIndexOf(std::wstring& string, std::wstring tofind, int start)
		{
			for (int i = start; i < string.length(); ++i)
			{
				if (ComparePart(string, i, tofind))
					return i;
			}
			return -1;
		}

		//Returns the last index of a character in a string. Returns -1 if the character is not found.
		int LastIndexOf(std::wstring& string, wchar_t ch)
		{
			for (int i = string.length() - 1; i > -1; i--)
			{
				if (ch == string[i])
					return i;
			}
			return -1;
		}

		//Returns the last index of a character in a string starting from not the end of the string but int start . Returns -1 if the character is not found.
		int LastIndexOf(std::wstring& string, wchar_t ch, int start)
		{
			for (unsigned int i = start; i > -1; i--)
			{
				if (ch == string[i])
					return i;
			}
			return -1;
		}

		//Returns the last index of string tofind in a string. Returns -1 if tofind is not found.
		int LastIndexOf(std::wstring& string, std::wstring tofind)
		{
			for (size_t i = string.length() - 1; i > -1; i--)
			{
				if (ComparePart(string, i, tofind))
					return i;
			}
			return -1;
		}

		//Returns the last index of string tofind in a string startinf from int start. Returns -1 if tofind is not found.
		int LastIndexOf(std::wstring& string, std::wstring tofind, int start)
		{
			for (size_t i = start - 1; i > -1; i--)
			{
				if (ComparePart(string, i, tofind))
					return i;
			}
			return -1;
		}

		//Returns a part of a string from the index start to index end.
		std::wstring GetPart(std::wstring& string, unsigned int start, unsigned int end)
		{
			if (start <= string.length() && end <= string.length() && start <= end)
			{
				std::wstring res;
				while (start < end)
				{
					res += string[start];
					start++;
				}
				return res;
			}
			else
				ThrowException(L"Invalid GetPart Parameters");
			return L"";
		}

		//Checks is a character is a number.
		bool IsNumeric(wchar_t ch)
		{
			switch (ch)
			{
			case L'0':
				return true;
			case L'1':
				return true;
			case L'2':
				return true;
			case L'3':
				return true;
			case L'4':
				return true;
			case L'5':
				return true;
			case L'6':
				return true;
			case L'7':
				return true;
			case L'8':
				return true;
			case L'9':
				return true;
			}
			return false;
		}

		// Converts a wstring to a string
		std::string ToString(std::wstring string)
		{
			return std::string(string.begin(), string.end());
		}

		// Converts a string to a wstring. NEEDS WINDOWS .H
		std::wstring ToWstring(std::string string)

		{
#ifdef _WINDOWS_
			std::wstring ret(string.length(), L' ');
			MultiByteToWideChar(CP_NONE, 0, string.c_str(), -1, (LPWSTR)ret.data(), (int)ret.size());
			return ret;
#else
#warning StringTools::ToWString will not work with characters > 128
			ThrowException(L"Won't be able to convert std::string chars > 127 while converting to wstring. If you wanst all the characters to work #include <Windows.h>", ExceptionGravity::Warning);
			return std::wstring(string.begin(), string.end());
#endif			
		}
	}

	//A namespace to create, delete and open files and directories.
	namespace IO
	{
		enum FileFormat : char
		{
			ANSIorUTF8, UTF8BOM, UCS2BIG, UCS2LITTLE
		};

		// To open and to save text files
		class TextFile
		{
		private:
			std::wstring m_text;
			FileFormat m_format;

		public:
			TextFile() {}
			//Loads a file which text can be with TextFile::GetText(). Supports ANSI, UTF8, UTF8 BOM, UCS2, UCS2 BOM. Only supports accented characters in UCS2, UCS2 BOM and ANSI
			TextFile(std::wstring filepath) {
				std::wstring tmp;
				std::wfstream stream = std::wfstream(StringTools::ToString(filepath).c_str());

				if (!stream.is_open())
				{
					ThrowException(L"Error opening text file " + filepath);
					return;
				}

				while (!stream.eof())
				{
					std::getline(stream, tmp);

					for (unsigned int i = 0; i < tmp.size(); i++)
						if (tmp[i] != 0)
							m_text += tmp[i];

					m_text += L'\n';
				}

				//Header Removing
				if (m_text[0] == L'\xff' && m_text.c_str()[1] == L'\xfe')
				{
					//LITTLE ENDIAN
					for (unsigned int i = 2; i < m_text.length(); i++)
						m_text[i - 2] = m_text[i];
					m_format = FileFormat::UCS2LITTLE;
				}
				else if (m_text[1] == L'\xff' && m_text[0] == L'\xfe')
				{
					// BIG ENDIAN
					for (unsigned int i = 2; i < m_text.length(); i++)
						m_text[i - 2] = m_text[i];
					m_format = FileFormat::UCS2BIG;
				}
				else if (m_text[0] == L'\xef' && m_text[1] == L'\xbb' && m_text[2] == L'\xbf')
				{
					// UTF8 BOM
					for (unsigned int i = 3; i < m_text.length(); i++)
						m_text[i - 3] = m_text[i];
					m_format = FileFormat::UTF8BOM;
				}
				else
				{
					//ANSI
					m_format = FileFormat::ANSIorUTF8;
				}


				stream.close();
			}
			~TextFile() {}

			//Returns the text loaded by the constructor
			inline std::wstring& GetText() { return m_text; }
			// Returns the format of the text file load by the constructor
			inline FileFormat GetFormat() { return m_format; }

			//Saves a text file
			static void Save(std::wstring filepath, std::wstring text)
			{
				std::wfstream stream = std::wfstream(StringTools::ToString(filepath).c_str(), std::ios::out);
				if (!stream.is_open())
				{
					ThrowException(L"Error saving text file " + filepath);
					return;
				}
				stream << text;
				stream.close();
			}

			//Deletes a file
			static inline void Delete(std::wstring filepath)
			{
				if (std::remove(StringTools::ToString(filepath).c_str()))
					ThrowException(L"Error deleting text file " + filepath);
			}

			//Returns the size of a file in bytes
			unsigned long long GetSize(std::wstring filepath)
			{
				std::wifstream str(StringTools::ToString(filepath));

				if (!str.is_open())
				{
					ThrowException(L"Error opening text file for size evaluation " + filepath);
					return 0;
				}

				unsigned long long size = str.tellg();
				str.close();
				return size;
			}
		};

		class BinaryFile
		{
		public:
			BinaryFile() {}
			~BinaryFile() {
				if (m_data)
					delete[] m_data;
			}

			//Loads a binary file. Maximum file size is 4gb
			BinaryFile(std::wstring filepath)
			{
				FILE* file = _wfopen(filepath.c_str(), L"rb");
				if (file == NULL)
				{
					ThrowException(L"Error opening binary file " + filepath);
					return;
				}
				else
				{
					fseek(file, 0, SEEK_END);
					m_length = (unsigned long)ftell(file);
					fseek(file, 0, 0);
					m_data = new unsigned char[m_length];
					fread(m_data, m_length, 1, file);
					fclose(file);
				}
			}

			//Saves a binary file
			static void Save(std::wstring filepath, std::vector<unsigned char> data)
			{
				FILE* file = _wfopen(filepath.c_str(), L"w");
				if (file == NULL)
					ThrowException(L"Error saving binary file " + filepath);
				else
				{
					for (unsigned int i = 0; i < data.size(); i++)
						fputc(data[i], file);
				}
			}

			//Deletes a file.
			inline static void Delete(std::wstring filepath)
			{
				if (std::remove(StringTools::ToString(filepath).c_str()))
					ThrowException(L"Error deleting binary file " + filepath);
			}

			//Returns the data loaded by the constructor
			inline unsigned char* GetData() { return m_data; }

			//Returns the size of a file in bytes
			inline unsigned long long GetSize()
			{
				return m_length;
			}

			//Returns the size of a file in bytes
			unsigned long long GetSize(std::wstring filepath)
			{
				FILE* file = _wfopen(filepath.c_str(), L"r");
				if (file == NULL)
				{
					ThrowException(L"Error opening binary file for size evaluation " + filepath);
					return 0;
				}
				fseek(file, 0, SEEK_END);
				unsigned long length = (unsigned long)ftell(file);
				fclose(file);
				return length;
			}
		private:
			unsigned char* m_data;
			unsigned long long m_length;
		};

		//Returns the list of files inside a directory.
		std::vector<std::wstring> GetFilesInDirectory(std::wstring directory)
		{
			std::vector<std::wstring> ret;
			tinydir_dir dir;
			tinydir_open(&dir, StringTools::ToString(directory).c_str());

			while (dir.has_next)
			{
				tinydir_file file;
				tinydir_readfile(&dir, &file);

				if (!file.is_dir)
				{
					ret.push_back(StringTools::ToWstring(file.name));
				}
				tinydir_next(&dir);
			}

			tinydir_close(&dir);
			return ret;
		}

		//Returns the list of directories inside a directory.
		std::vector<std::wstring> GetDirectoriesInDirectory(std::wstring directory)
		{
			std::vector<std::wstring> ret;
			tinydir_dir dir;
			tinydir_open(&dir, StringTools::ToString(directory).c_str());

			while (dir.has_next)
			{
				tinydir_file file;
				tinydir_readfile(&dir, &file);

				if (file.is_dir)
				{
					ret.push_back(StringTools::ToWstring(file.name));
				}
				tinydir_next(&dir);
			}

			tinydir_close(&dir);
			return ret;
		}

		//Creates a directory in a filepath.
		inline void CreateFolder(std::wstring path)
		{
			system((std::string("mkdir ") + StringTools::ToString(path)).c_str());
		}
	}

	//Transforms objects into byte arrays and vice versa
	namespace BinaryConverter
	{
		//Converts an object to bytes. Do not try to convert objects with pointers like lists and vectors.
		template<typename T>
		std::vector<unsigned char> ToBytes(T object)
		{
			unsigned int length = sizeof(object);
			std::vector<unsigned char> ret;
			ret.resize(length);
			ret.reserve(length);
			for (int i = 0; i < length; i++)
				ret.data()[i] = ((unsigned char*)&object)[i];
			return ret;
		}

		//Converts an array of bytes to a type you specified.
		template<typename T>
		T ToType(std::vector<unsigned char> data)
		{
			if (data.size() == sizeof(T))
			{
				T ret = T();
				for (int i = 0; i < data.size(); ++i)
					((unsigned char*)&ret)[i] = data[i];
				return ret;
			}
			else
			{
				ThrowException(L"Incompatible sizes while converting byte array to object");
			}
			return T();
		}

	}

	//General time related functions
	namespace Time
	{
		//Returns the milliseconds passed
		inline long long TimeInMilliseconds()
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		}

		//Returns the seconds passed
		inline long long TimeInSeconds()
		{
			return time(0);
		}
	}

	//Used to count the time between Start() and Stop()
	class Stopwatch
	{
	public:
		Stopwatch() {}
		~Stopwatch() {}

		//Starts counting the time
		inline void Start()
		{
			m_beg = Time::TimeInMilliseconds();
		}

		//Stops counting the time
		inline void Stop()
		{
			m_end = Time::TimeInMilliseconds();
		}

		//Resets the stopwatch
		inline void Reset() {
			m_beg = 0;
			m_end = 0;
		}

		//Gets the time between Start() and End() in milliseconds
		inline long long GetResultMilliseconds()
		{
			return m_end - m_beg;
		}

	private:
		long long m_beg, m_end;
	};

	//Calls a function from x in x milliseconds
	class Timer
	{
	public:
		Timer() {}
		//Creates the timer with an interval in milliseconds. Timers can be singlethreaded if you run Thread::UpdateAll() every frame in your game loop
		Timer(unsigned int interval, void(*OnTick)())
		{
			m_interval = interval;
			m_ontick = OnTick;
		}

		~Timer() {}

		//Starts the Timer
		inline void Start()
		{
			m_active = true;
			m_last = Time::TimeInMilliseconds();
		}

		//Stops the timer
		inline void Stop()
		{
			m_active = false;
		}

		//Call this every frame
		void Update()
		{
			if (Time::TimeInMilliseconds() >= (m_last + m_interval))
			{
				m_ontick();
				m_last = Time::TimeInMilliseconds();
			}
		}

	private:
		unsigned int m_interval;
		bool m_active;
		void(*m_ontick)();
		long long m_last;
	};

	//Represents an opengl version.
	class GLVersion
	{
	public:
		GLVersion() {}
		//Creates an OpenGL version.
		inline GLVersion(unsigned int Major, unsigned int Minor)
		{
			this->Major = Major;
			this->Minor = Minor;
		}

		~GLVersion() {}

		unsigned int Major, Minor;
	};

	

	//Namespace to get information about the system hardware.
	namespace Hardware
	{
		//Returns the number of threads in the CPU.
		inline unsigned int GetThreadCount()
		{
#ifdef _WINDOWS_
			SYSTEM_INFO sysinfo;
			GetSystemInfo(&sysinfo);
			return sysinfo.dwNumberOfProcessors;
#else
			return std::thread::hardware_concurrency();
#endif
		}

		//Returns the amount of RAM in your system in bytes. Needs windows.h
		inline unsigned long long GetMemoryAmount()
		{
#ifdef _WINDOWS_
			MEMORYSTATUSEX memorydata;
			GlobalMemoryStatusEx(&memorydata);
			return memorydata.ullTotalPhys;
#else
#warning Hardware::GetGetMemoryAmount will not work. include windows.h
			ThrowException(L"Windows is not included in this window build so you can't call this function");
			return 0;
#endif			
		}

		//Returns the size of the page file in bytes. Needs windows.h
		inline unsigned long long GetPageFileSize()
		{
#ifdef _WINDOWS_

			MEMORYSTATUSEX memorydata;
			GlobalMemoryStatusEx(&memorydata);
			return memorydata.ullTotalPageFile;
#else
			#warning Hardware::GetPageFileSize will not work.include windows.h
			ThrowException(L"Windows is not included in this window build so you can't call this function");
			return 0;
#endif
			
		}

		//Returns the frequency of the CPU in MHz. Needs windows.h
		unsigned long long GetCPUFrequency()
		{
#ifdef _WINDOWS_
			DWORD BufSize = _MAX_PATH;
			DWORD dwMHz = _MAX_PATH;
			HKEY hKey;

			long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);

			if (lError != ERROR_SUCCESS)
			{
				ThrowException(L"Can't open registry to get CPU frequency");
				return 0;
			}
			RegQueryValueEx(hKey, "~MHz", NULL, NULL, (LPBYTE)&dwMHz, &BufSize);
			return dwMHz;
#else
			#warning Hardware::GetCPUFrequency will not work.include windows.h
			ThrowException(L"Windows is not included in this window build so you can't call this function");
			return 0;
#endif
			
		}
	}

	//Represents a screen. Can be used to get your screen resolution and run your window at fullscreen-
	class Display
	{
	public:
		//Gets the width and the height of the primary display.
		Display()
		{
			m_monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);
			m_width = mode->width;
			m_height = mode->height;
			m_refresh = mode->refreshRate;
			m_red = mode->redBits;
			m_green = mode->greenBits;
			m_blue = mode->blueBits;
			int x, y;
			glfwGetMonitorPos(m_monitor, &x, &y);
			m_x = x;
			m_y = y;
		}

		~Display() {}

		//Returns the width of a screen.
		inline unsigned int GetWidth() { return m_width; }
		//Returns the height of a screen.
		inline unsigned int GetHeight() { return m_height; }
		//Returns the refresh rate of a screen.
		inline unsigned int GetRefreshRate() { return m_refresh; }
		//DO NOT USE unless you are working directly with GLFW.
		inline unsigned int GetX() { return m_x; }
		//DO NOT USE unless you are working directly with GLFW.
		inline unsigned int GetY() { return m_y; }
		//DO NOT USE unless you are working directly with GLFW.
		inline GLFWmonitor* GetMonitor() { return m_monitor; }
		//DO NOT USE unless you are working directly with GLFW.
		inline unsigned int GetRedBits() { return m_red; }
		//DO NOT USE unless you are working directly with GLFW.
		inline unsigned int GetGreenBits() { return m_green; }
		//DO NOT USE unless you are working directly with GLFW.
		inline unsigned int GetBlueBits() { return m_blue; }

	private:
		unsigned int m_width, m_height, m_refresh, m_x, m_y, m_red, m_green, m_blue;
		GLFWmonitor* m_monitor;
	};

	//Represents a windows DLL 
	class GameMod
	{
	public:
		GameMod() {}

		//Loads a windows DLL and you can call functions. Mods must be the same build target(x86 or x64) as the application.
		GameMod(std::wstring filepath)
		{
#ifdef _WINDOWS_
			m_dll = LoadLibraryW(filepath.c_str());
			if (m_dll)
			{
				return;
			}
			ThrowException(L"Unable to load mod.");
			return;
#else
#warning GameMods will not work. include windows.h
			ThrowException(L"Unable to load mod because this feature is windows exclusive.");
#endif			
		}

		//Gets a function from the DLL
		template<typename T>
		T GetFunction(std::string FunctionName)
		{
#ifdef _WINDOWS_
			if (m_dll)
			{
				return (T)GetProcAddress((HMODULE)m_dll, FunctionName.c_str());
			}
			ThrowException(L"Unable to load mod function.");
			return T();
#else
				ThrowException(L"Unable to load mod because this feature is windows exclusive.");
return T();
#endif			
		}

		~GameMod()
		{
#ifdef _WINDOWS_
		if (m_dll)
				FreeLibrary((HMODULE) m_dll);
#endif		
		}

	private:
		void* m_dll = NULL;
	};

	//Namespace used to get information about mouse position, mouse scroll, keyboard keys
	namespace Input
	{
		//Can be a key on your keyboard or a mouse button
		class KeyBind
		{
		public:
			KeyBind() {}
			//Creates a new Keybind. Set key as GLFW_KEY_something
			KeyBind(int Key)
			{
				m_key = Key;
				m_mouse = false;
			}
			//Creates a new Keybind. The mouse parameter sets if the key is a mouse button (GLFW_MOUSE_BUTTON_something, true) or a keyboard key(GLFW_KEY_something, false)
			KeyBind(int key, bool mouse)
			{
				m_key = key;
				m_mouse = mouse;
			}

			//Returns the key number. It is a GLFW_KEY
			inline int GetKeyNumber()
			{
				return m_key;
			}

			//Returns if the key is a mouse button.
			inline bool IsMouseButton()
			{
				return m_mouse;
			}

			//Returns the name of the key. The names are from an US keyboard.
			inline std::wstring GetKeyName()
			{
				if (!m_mouse)
				{
					if (m_key < GLFW_KEY_LAST && m_key > -1)
						return m_keynames[m_key];
					else
						ThrowException(L"Unknown key getting key name");
				}
				else
				{
					if (m_key == 0)
						return L"Left button";
					else if (m_key == 1)
						return L"Right button";
					else if (m_key == 2)
						return L"Middle button";
					else
						return L"Mouse button " + std::to_wstring(m_key);
				}
				return L"";
			}

			//DO NOT USE. THIS GENERATES THE NAMES OF THE KEYS. glfwGetKeyName() crashes with certain keys so this was the only way.
			static void Initialize()
			{
				m_keynames = new std::wstring[349];
				m_keynames[GLFW_KEY_SPACE] = L"Space";
				m_keynames[GLFW_KEY_APOSTROPHE] = L"Apostrophe";
				m_keynames[GLFW_KEY_COMMA] = L"Comma";
				m_keynames[GLFW_KEY_MINUS] = L"Minus";
				m_keynames[GLFW_KEY_PERIOD] = L"Period";
				m_keynames[GLFW_KEY_SLASH] = L"Slash";
				m_keynames[GLFW_KEY_0] = L"0";
				m_keynames[GLFW_KEY_1] = L"1";
				m_keynames[GLFW_KEY_2] = L"2";
				m_keynames[GLFW_KEY_3] = L"3";
				m_keynames[GLFW_KEY_4] = L"4";
				m_keynames[GLFW_KEY_5] = L"5";
				m_keynames[GLFW_KEY_6] = L"6";
				m_keynames[GLFW_KEY_7] = L"7";
				m_keynames[GLFW_KEY_8] = L"8";
				m_keynames[GLFW_KEY_9] = L"9";
				m_keynames[GLFW_KEY_SEMICOLON] = L"Semicolon";
				m_keynames[GLFW_KEY_EQUAL] = L"Equal";
				m_keynames[GLFW_KEY_A] = L"A";
				m_keynames[GLFW_KEY_B] = L"B";
				m_keynames[GLFW_KEY_C] = L"C";
				m_keynames[GLFW_KEY_D] = L"D";
				m_keynames[GLFW_KEY_E] = L"E";
				m_keynames[GLFW_KEY_F] = L"F";
				m_keynames[GLFW_KEY_G] = L"G";
				m_keynames[GLFW_KEY_H] = L"H";
				m_keynames[GLFW_KEY_I] = L"I";
				m_keynames[GLFW_KEY_J] = L"J";
				m_keynames[GLFW_KEY_K] = L"K";
				m_keynames[GLFW_KEY_L] = L"L";
				m_keynames[GLFW_KEY_M] = L"M";
				m_keynames[GLFW_KEY_N] = L"N";
				m_keynames[GLFW_KEY_O] = L"O";
				m_keynames[GLFW_KEY_P] = L"P";
				m_keynames[GLFW_KEY_Q] = L"Q";
				m_keynames[GLFW_KEY_R] = L"R";
				m_keynames[GLFW_KEY_S] = L"S";
				m_keynames[GLFW_KEY_T] = L"T";
				m_keynames[GLFW_KEY_U] = L"U";
				m_keynames[GLFW_KEY_V] = L"V";
				m_keynames[GLFW_KEY_W] = L"W";
				m_keynames[GLFW_KEY_X] = L"X";
				m_keynames[GLFW_KEY_Y] = L"Y";
				m_keynames[GLFW_KEY_Z] = L"Z";
				m_keynames[GLFW_KEY_LEFT_BRACKET] = L"Left Bracket";
				m_keynames[GLFW_KEY_BACKSLASH] = L"Backslash";
				m_keynames[GLFW_KEY_RIGHT_BRACKET] = L"Right Bracket";
				m_keynames[GLFW_KEY_GRAVE_ACCENT] = L"Grave accent";
				m_keynames[GLFW_KEY_WORLD_1] = L"World Key 1";
				m_keynames[GLFW_KEY_WORLD_2] = L"World Key 2";
				m_keynames[GLFW_KEY_ESCAPE] = L"Escape";
				m_keynames[GLFW_KEY_ENTER] = L"Enter";
				m_keynames[GLFW_KEY_TAB] = L"Tab";
				m_keynames[GLFW_KEY_BACKSPACE] = L"Backspace";
				m_keynames[GLFW_KEY_INSERT] = L"Insert";
				m_keynames[GLFW_KEY_DELETE] = L"Delete";
				m_keynames[GLFW_KEY_RIGHT] = L"Right key";
				m_keynames[GLFW_KEY_LEFT] = L"Left key";
				m_keynames[GLFW_KEY_DOWN] = L"Down Key";
				m_keynames[GLFW_KEY_UP] = L"Up key";
				m_keynames[GLFW_KEY_PAGE_UP] = L"Page up";
				m_keynames[GLFW_KEY_PAGE_DOWN] = L"Page down";
				m_keynames[GLFW_KEY_HOME] = L"Home key";
				m_keynames[GLFW_KEY_END] = L"End key";
				m_keynames[GLFW_KEY_CAPS_LOCK] = L"Caps lock";
				m_keynames[GLFW_KEY_SCROLL_LOCK] = L"Scroll lock";
				m_keynames[GLFW_KEY_NUM_LOCK] = L"Num lock";
				m_keynames[GLFW_KEY_PRINT_SCREEN] = L"Printscreen";
				m_keynames[GLFW_KEY_PAUSE] = L"Pause key";
				m_keynames[GLFW_KEY_F1] = L"F1";
				m_keynames[GLFW_KEY_F2] = L"F2";
				m_keynames[GLFW_KEY_F3] = L"F3";
				m_keynames[GLFW_KEY_F4] = L"F4";
				m_keynames[GLFW_KEY_F5] = L"F5";
				m_keynames[GLFW_KEY_F6] = L"F6";
				m_keynames[GLFW_KEY_F7] = L"F7";
				m_keynames[GLFW_KEY_F8] = L"F8";
				m_keynames[GLFW_KEY_F9] = L"F9";
				m_keynames[GLFW_KEY_F10] = L"F10";
				m_keynames[GLFW_KEY_F11] = L"F11";
				m_keynames[GLFW_KEY_F12] = L"F12";
				m_keynames[GLFW_KEY_F13] = L"F13";
				m_keynames[GLFW_KEY_F14] = L"F14";
				m_keynames[GLFW_KEY_F15] = L"F15";
				m_keynames[GLFW_KEY_F16] = L"F16";
				m_keynames[GLFW_KEY_F17] = L"F17";
				m_keynames[GLFW_KEY_F18] = L"F18";
				m_keynames[GLFW_KEY_F19] = L"F19";
				m_keynames[GLFW_KEY_F20] = L"F20";
				m_keynames[GLFW_KEY_F21] = L"F21";
				m_keynames[GLFW_KEY_F22] = L"F22";
				m_keynames[GLFW_KEY_F23] = L"F23";
				m_keynames[GLFW_KEY_F24] = L"F24";
				m_keynames[GLFW_KEY_F25] = L"F25";
				m_keynames[GLFW_KEY_KP_0] = L"Numpad 0";
				m_keynames[GLFW_KEY_KP_1] = L"Numpad 1";
				m_keynames[GLFW_KEY_KP_2] = L"Numpad 2";
				m_keynames[GLFW_KEY_KP_3] = L"Numpad 3";
				m_keynames[GLFW_KEY_KP_4] = L"Numpad 4";
				m_keynames[GLFW_KEY_KP_5] = L"Numpad 5";
				m_keynames[GLFW_KEY_KP_6] = L"Numpad 6";
				m_keynames[GLFW_KEY_KP_7] = L"Numpad 7";
				m_keynames[GLFW_KEY_KP_8] = L"Numpad 8";
				m_keynames[GLFW_KEY_KP_9] = L"Numpad 9";
				m_keynames[GLFW_KEY_KP_DECIMAL] = L"Numpad period";
				m_keynames[GLFW_KEY_KP_DIVIDE] = L"Numpad slash";
				m_keynames[GLFW_KEY_KP_MULTIPLY] = L"Numpad asterisk";
				m_keynames[GLFW_KEY_KP_SUBTRACT] = L"Numpad minus";
				m_keynames[GLFW_KEY_KP_ADD] = L"Numpad plus";
				m_keynames[GLFW_KEY_KP_ENTER] = L"Numpad enter";
				m_keynames[GLFW_KEY_KP_EQUAL] = L"Numpad equal";
				m_keynames[GLFW_KEY_KP_EQUAL] = L"Numpad equal";
				m_keynames[GLFW_KEY_LEFT_SHIFT] = L"Left shift";
				m_keynames[GLFW_KEY_LEFT_CONTROL] = L"Left control";
				m_keynames[GLFW_KEY_LEFT_ALT] = L"Left alt";
				m_keynames[GLFW_KEY_LEFT_SUPER] = L"Windows key";
				m_keynames[GLFW_KEY_RIGHT_SHIFT] = L"Right shift";
				m_keynames[GLFW_KEY_RIGHT_CONTROL] = L"Right control";
				m_keynames[GLFW_KEY_RIGHT_ALT] = L"Right alt";
				m_keynames[GLFW_KEY_RIGHT_SUPER] = L"Right super";
				m_keynames[GLFW_KEY_MENU] = L"Menu key";
			}

			//DO NOT USE UNLESS YOU ARE WORKING DIRECTLY WITH THE ENGINE.
			inline static void Clean()
			{
				delete[] m_keynames;
			}

			~KeyBind() {}

		private:
			static std::wstring* m_keynames;
			int m_key;
			bool m_mouse;
		};

		//DO NOT USE.
		int m_x, m_y, m_sx, m_sy;
		//DO NOT USE.
		std::wstring m_textinput;
		//DO NOT USE.
		std::vector<KeyBind> last_frame, this_frame;

		//DO NOT USE.
		void ScrollResponse(GLFWwindow*, double x, double y)
		{
			m_sx -= (int)x;
			m_sy -= (int)y;
		}

		//DO NOT USE.
		void TextResponse(GLFWwindow*, unsigned int character)
		{
			m_textinput += character;
		}

		//DO NOT USE.
		void KeyResponse(GLFWwindow* window, int key, int sc, int action, int mods)
		{
			if (key == GLFW_KEY_BACKSPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
			{
					m_textinput += (wchar_t) 8;
			}
			if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
			{
				m_textinput += (wchar_t) 1;
			}
			if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
			{
				m_textinput += (wchar_t) 2;
			}
		}
	}

	//DO NOT USE.
	namespace AudioFramework
	{
		ALCdevice* m_device;
		ALCcontext* m_context;

		//DO NOT USE.
		void Initialize()
		{
			m_device = alcOpenDevice(NULL);
			if (!m_device)
			{
				ThrowException(L"Unable to load OpenAL for audio playback");
				return;
			}
			m_context = alcCreateContext(m_device, NULL);
			if (!m_context)
			{
				ThrowException(L"Unable to load OpenAL for audio playback");
				return;
			}
			alcMakeContextCurrent(m_context);
		}

		//DO NOT USE.
		void Finalize()
		{
			alcDestroyContext(m_context);
			alcCloseDevice(m_device);
		}

		//DO NOT USE.
		bool isBigEndian()
		{
			int a = 1;
			return !((char*)&a)[0];
		}

		//DO NOT USE.
		int convertToInt(char* buffer, int len)
		{
			int a = 0;
			if (!isBigEndian())
				for (int i = 0; i < len; ++i)
					((char*)&a)[i] = buffer[i];
			else
				for (int i = 0; i < len; ++i)
					((char*)&a)[3 - i] = buffer[i];
			return a;
		}

		//DO NOT USE.
		bool LoadToBuffer(std::wstring filepath, int ALbuffer)
		{
			int chan, samplerate, bps, size;

			char buffer[4];
			std::ifstream in(StringTools::ToString(filepath), std::ios::binary);

			if (!in.is_open())
			{
				ThrowException(L"Wave file does not exist or you have no permission to open it: " + filepath);
				return false;
			}

			in.read(buffer, 4);
			if (strncmp(buffer, "RIFF", 4) != 0)
			{
				ThrowException(L"Wave file is invalid: " + filepath);
				return false;
			}
			in.read(buffer, 4);
			in.read(buffer, 4);      //WAVE
			in.read(buffer, 4);      //fmt
			in.read(buffer, 4);      //16
			in.read(buffer, 2);      //1
			in.read(buffer, 2);
			chan = convertToInt(buffer, 2);
			in.read(buffer, 4);
			samplerate = convertToInt(buffer, 4);
			in.read(buffer, 4);
			in.read(buffer, 2);
			in.read(buffer, 2);
			bps = convertToInt(buffer, 2);
			in.read(buffer, 4);      //data
			in.read(buffer, 4);
			size = convertToInt(buffer, 4);
			char* data = new char[size];
			in.read(data, size);
			in.close();

			unsigned int format;
			if (chan == 1)
			{
				if (bps == 8)
				{
					format = AL_FORMAT_MONO8;
				}
				else {
					format = AL_FORMAT_MONO16;
				}
			}
			else {
				if (bps == 8)
				{
					format = AL_FORMAT_STEREO8;
				}
				else {
					format = AL_FORMAT_STEREO16;
				}
			}

			alBufferData(ALbuffer, format, data, size, samplerate);
			return true;
		}


	}

	//Represents an audio file.
	class AudioSample
	{
	public:
		AudioSample() { m_buffer = 0; }
		//Creates an audio from a WAV file.
		AudioSample(std::wstring filepath)
		{
			alGenBuffers(1, &m_buffer);
			if (!AudioFramework::LoadToBuffer(filepath, m_buffer))
				return;
			else
			{
				alGetBufferi(m_buffer, AL_SIZE, &m_bytes);
				alGetBufferi(m_buffer, AL_CHANNELS, &m_channels);
				alGetBufferi(m_buffer, AL_BITS, &m_bits);
				alGetBufferi(m_buffer, AL_FREQUENCY, &m_freq);
			}
		}

		//Returns the OpenAL buffer. This is not needed unless you are using OpenAL directly.
		inline unsigned int GetBuffer()
		{
			return m_buffer;
		}

		//Returns the lenght of the sample in milliseconds
		inline unsigned long long GetLenght()
		{
			return (unsigned long long)((float)(m_bytes * 18 / (m_channels * m_bits)) / (float)(m_freq * 1000ULL));
		}

		//Returns the number of channels in the sample 1 is mono, 2 is strereo, etc.
		inline unsigned int GetChannels()
		{
			return m_channels;
		}

		~AudioSample()
		{
			if (m_buffer)
				alDeleteBuffers(1, &m_buffer);
		}

	private:
		unsigned int m_buffer;
		int m_bytes = 0, m_channels = 0, m_bits = 0, m_freq = 0;
	};

	//Playing, paused or stopped
	enum AudioPlayingState : char
	{
		Playing, Paused, Stopped
	};

	//An object that can play audio
	class AudioSource
	{
	public:

		//Creates an audio source used to play one audio at once.
		AudioSource()
		{
			alGenSources(1, &m_source);
			alSourcef(m_source, AL_GAIN, m_volume);
			alSourcei(m_source, AL_SOURCE_RELATIVE, AL_TRUE);
			alSource3f(m_source, AL_POSITION, -1, 0, 0);
		}

		~AudioSource() {
			if (m_source)
			{
				alSourceStop(m_source);
				alDeleteSources(1, &m_source);
			}
		}

		//Plays a sample. Do not delete the audio file pointer.
		inline void PlaySample(AudioSample* sample)
		{
			m_sample = sample;
			alSourcei(m_source, AL_BUFFER, sample->GetBuffer());
			alSourcePlay(m_source);
		}

		//Returns the OpenAL source. This is not needed unless you are using OpenAL directly.
		inline unsigned int GetSource()
		{
			return m_source;
		}

		//Sets the volume of the audio being played. 0 is none and 100 is 100%. Higher values will do nothing. The volume will be maxed.
		inline void SetVolume(unsigned char volume)
		{
			if (m_volume != volume)
			{
				if (m_source)
					alSourcef(m_source, AL_GAIN, volume / 100.0f);
				else ThrowException(L"Audio source hasn't been created yet");
				m_volume = volume;
			}
		}

		//Sets the audio panning. -1 is extreme left, 0 is middle and 1 is extreme right. The audio must be mono (1 audio channel).
		inline void SetPanning(float panning)
		{
			panning += 0.01f;
			if (m_panning != panning)
			{
				if (m_source)
					alSource3f(m_source, AL_POSITION, panning, 0, 1);
				else ThrowException(L"Audio source hasn't been created yet");
				m_panning = panning;
			}
		}

		//Gets the volume that the audio is playing at
		inline unsigned char GetVolume()
		{
			return m_volume;
		}

		//Gets the panning that audio is playing at
		inline float GetPanning()
		{
			return m_panning;
		}

		//Stops the audio being played
		inline void Stop()
		{
			if (m_source)
				alSourceStop(m_source);
			else ThrowException(L"Audio source hasn't been created yet");
		}

		//Pauses the audio being played. Use Unpause() to get the audio playing again.
		inline void Pause()
		{
			if (m_source)
				alSourcePause(m_source);
			else ThrowException(L"Audio source hasn't been created yet");
		}

		//Gets the audio playing after being paused.
		inline void UnPause()
		{
			if (m_source)
				alSourcePlay(m_source);
			else ThrowException(L"Audio source hasn't been created yet");
		}

		//Goes to a millisecond in the audio.
		inline void JumpToMillisecond(unsigned long long millisecond)
		{
			if (m_source)
				alSourcef(m_source, AL_SEC_OFFSET, millisecond / 1000.0f);
			else ThrowException(L"Audio source hasn't been created yet");
		}

		//Sets the playback speed. As consequence, it also changes the pitch of the sound.
		inline void SetSpeed(float multiplier)
		{
			if (m_source)
				alSourcef(m_source, AL_PITCH, multiplier);
			else ThrowException(L"Audio source hasn't been created yet");
		}

		//Fades the audio from the current volume to the desired one during a set period of time
		void Fade(unsigned char volume, unsigned long long milliseconds)
		{
				m_dif = (volume - m_volume);
				m_interval = (unsigned long long) (milliseconds / std::abs(m_dif));
				m_last = Time::TimeInMilliseconds();
				m_targetvol = volume;
				m_milliseconds = milliseconds;
		}

		//Returns if the audio is playing, paused or stopped.
		AudioPlayingState GetPlayingState()
		{
			int playing, paused, stopped;
			if (m_source)
			{
				alGetSourcei(m_source, AL_PLAYING, &playing);
				alGetSourcei(m_source, AL_PAUSED, &paused);
				alGetSourcei(m_source, AL_STOPPED, &stopped);

				if (playing)
					return AudioPlayingState::Playing;
				else if (paused)
					return AudioPlayingState::Paused;
				else
					return AudioPlayingState::Stopped;
			}
			else ThrowException(L"Audio source hasn't been created yet");
			return AudioPlayingState::Stopped;
		}
		
		//Call this every frame if you're fading
		void Update()
		{
			if (m_volume != m_targetvol)
			{
				if (Time::TimeInMilliseconds() - m_last >= m_interval)
				{
						if (m_dif > 0)
							SetVolume(m_volume + 1);
						else
							SetVolume(m_volume - 1);
						m_last = Time::TimeInMilliseconds();
				}
			}
		}
		
	private:
		unsigned int m_source;
		unsigned char m_volume = 100;
		float m_panning = 0;
		AudioSample* m_sample;
		
		//Fade variables
		unsigned char m_dif, m_targetvol;
		unsigned long long m_interval, m_last, m_milliseconds;
		bool m_fading;
		
	};

	//Represents an audio source with a position.
	class AudioEntity
	{
	public:
		AudioEntity()
		{
			m_source = new AudioSource();
		}

		void PlaySample(AudioSample* sample)
		{
			m_source->PlaySample(sample);
		}

		//Sets the position and the range of the audio-emmiting entity. Range is the radius of the circumference.
		void SetParameters(vec2 pos, unsigned int range)
		{
			m_x = pos.x;
			m_y = pos.y;
			m_range = range;
		}

		//Call this every time the listener(player) moves and feed this function it's position.
		void Update(vec2 pos)
		{
			if (pos.x > m_x)
				m_source->SetPanning(-0.9);
			else if (pos.x < m_y)
				m_source->SetPanning(0.9);
			else
				m_source->SetPanning(0);

			double distance = MeasureDistance(pos.x, pos.y);
			if (distance < m_range)
			{
				if (pos.x != m_x && pos.y != m_y)
					m_source->SetVolume((m_range / distance) * 10);
				else
					m_source->SetVolume(100);
			}
			else
			{
				m_source->SetVolume(0);
			}
		}

		~AudioEntity()
		{
			delete m_source;
		}

		//DO NOT USE. This is OpenAL directly related.
		inline AudioSource* GetSource()
		{
			return m_source;
		}
	private:
		AudioSource* m_source;
		int m_x, m_y, m_range;

		double MeasureDistance(unsigned int x, unsigned int y)
		{
			return sqrt(pow(abs((double)m_x - x), 2) + pow(abs((double)m_y - y), 2));
		}
	};

	//DO NOT USE. This is directly related to OpenGL and this is automatically used by the renderer.
	class ShaderVariable
	{
	public:
		ShaderVariable(unsigned int VAOindex, std::string name) {
			m_VAOindex = VAOindex;
			m_name = name;
		}
		~ShaderVariable() {}

		inline unsigned int GetIndex() { return m_VAOindex; }
		inline const char* GetName() { return m_name.c_str(); }
	private:
		unsigned int m_VAOindex;
		std::string m_name;
	};

	//DO NOT USE. This is an OpenGL shader and this is automatically used by the renderer.
	class Shader
	{
	public:
		Shader() {}
		~Shader() 
		{
			if (m_id)
				glDeleteProgram(m_id);
			if (m_vid)
				glDeleteShader(m_vid);
			if (m_fid)
				glDeleteShader(m_fid);
		}

		void Create(std::string vertex_shader, std::string fragment_shader, std::vector<ShaderVariable> variables, void(*uniforms)(int))
		{
			m_vid = Compile(vertex_shader.c_str(), GL_VERTEX_SHADER);
			m_fid = Compile(fragment_shader.c_str(), GL_FRAGMENT_SHADER);
			m_id = glCreateProgram();
			glAttachShader(m_id, m_vid);
			glAttachShader(m_id, m_fid);

			for (unsigned int i = 0; i < variables.size(); i++)
				glBindAttribLocation(m_id, variables[i].GetIndex(), variables[i].GetName());

			glLinkProgram(m_id);
			glValidateProgram(m_id);

			uniforms(m_id);
			glUseProgram(m_id);

		}

	private:
		unsigned int m_vid, m_fid, m_id;
		static int Compile(const char* code, GLenum type)
		{
			unsigned int shaderID = glCreateShader(type);
			glShaderSource(shaderID, 1, &code, 0);
			glCompileShader(shaderID);

			//int isCompiled = 0;
			//glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
			return shaderID;
		}
	};
	
	//DO NOT USE.
	enum ShaderType : char {Textured, ColorOnly};

	//DO NOT USE. This is directly related to OpenGL and this is automatically used by the renderer.
	class TextureShader : Shader
	{
	public:
		TextureShader() {}

		inline void CreateUpLayer()
		{
			Create(VertexShader, FragShader, { ShaderVariable(0, "pos"), ShaderVariable(1, "tc") }, Uniforms);
			SetPosition(vec2());
			SetScale(vecf(1, 1));
			SetColor(Color(255, 255, 255));
			SetShaderType(ShaderType::Textured);
			
		}

		~TextureShader() {}

		inline void SetMatrix(float* matrix)
		{
			glUniformMatrix4fv(m_ortho, 1, 0, matrix);
		}

		inline void SetPosition(vec2 pos)
		{
			if (m_x != pos.x || m_y != pos.y)
			{ 
				glUniform2f(m_position, (float)pos.x, (float)pos.y);
				m_x = pos.x;
				m_y = pos.y;
			}
		}

		inline void SetScale(vecf scale)
		{
			if (scale.x != l_scale.x || scale.y != l_scale.y)
			{ 
				glUniform2f(m_scale, scale.x, scale.y);
				l_scale = scale;
			}
		}

		inline void SetColor(Color color)
		{
			if (l_color.R != color.R || l_color.G != color.G || l_color.B != color.B || l_color.A != color.A)
			{
				float* ptr = color.ToGL();
				glUniform4f(m_color, ptr[0], ptr[1], ptr[2], ptr[3]);
				delete[] ptr;
				l_color = Color(color.R, color.G, color.B, color.A);
			}
		}

		inline void SetShaderType(ShaderType type)
		{
			if (l_geo != (int) type)
			{ 
				glUniform1f(m_geo, type);
				l_geo = type;
			}
		}

	private:
		static const char *VertexShader, *FragShader;
		static int m_ortho, m_position, m_scale, m_color, m_geo;
		int m_x, m_y; vecf l_scale; Color l_color; int l_geo;

		static void Uniforms(int id)
		{
			m_ortho = glGetUniformLocation(id, "ortho");
			m_position = glGetUniformLocation(id, "position");
			m_scale = glGetUniformLocation(id, "scale");
			m_color = glGetUniformLocation(id, "incolor");
			m_geo = glGetUniformLocation(id, "geometric");
		}
	};
	const char* TextureShader::VertexShader = "#version 130\nin vec2 pos;\nin vec2 tc;\nout vec4 fragcolor;\nout vec2 outtc;\nout float geo;\nuniform mat4 ortho;\nuniform vec2 position;\nuniform vec2 scale;\nuniform vec4 incolor;\nuniform float geometric;\n"
		"void main(void) {\n gl_Position = ortho * vec4(vec2(pos.x * scale.x, pos.y * scale.y) + position, 0.0, 1.0);\nfragcolor = incolor;\nouttc = tc;geo = geometric;} "; 
	const char* TextureShader::FragShader = "#version 130\nin vec4 fragcolor;\nin vec2 outtc;\nin float geo;\nuniform sampler2D txt;\nvoid main(void) {\n if (geo == 0.0f)\n\tgl_FragColor = texture2D(txt, outtc) * fragcolor;\nelse\n\tgl_FragColor = fragcolor; }";
	int TextureShader::m_ortho, TextureShader::m_position, TextureShader::m_scale, TextureShader::m_color, TextureShader::m_geo;
	
	//DO NOT USE. This is a list of all the shaders the engine uses.
	namespace Shaders
	{
		TextureShader* ts;
	}
	
	//DO NOT USE. This is OpenGL directly related.
	namespace Bindings
	{
		//DO NOT USE. This is OpenGL directly related.
		unsigned int s_vao = 0, s_tex = 0, s_fbo = 0;

		//DO NOT USE. This is OpenGL directly related.
		inline void BindTexture(unsigned int tex)
		{
			if (tex != s_tex)
			{
				s_tex = tex;
				glBindTexture(GL_TEXTURE_2D, tex);
			}
		}

		//DO NOT USE. This is OpenGL directly related.
		inline void BindVAO(unsigned int vao)
		{
			if (vao != s_vao)
			{
				s_vao = vao;
				glDisableVertexAttribArray(0);
				glDisableVertexAttribArray(1);
				glBindVertexArray(vao);
				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
			}
		}

		//DO NOT USE. This is OpenGL directly related.
		inline void BindFramebuffer(unsigned int fbo, unsigned int width, unsigned int height)
		{
			if (fbo != s_fbo)
			{
				s_fbo = fbo;
				glBindFramebuffer(GL_FRAMEBUFFER, fbo);
				glViewport(0, 0, width, height);
				float matrix[16] = {
					(float)  2.0f / width, 0, 0, 0,
					0, (float)  2.0f / -((float)height), 0, 0,
					0, 0, 2, 0,
					-1, 1, 0, 1
				};
				Shaders::ts->SetMatrix(matrix);
			}
		}
	}

	//BMP, PNG, JPEG or TGA
	enum ImageFormat : char
	{
		BMP, PNG, JPEG, TGA
	};

	//An image that can be modified
	class Image
	{
	public:
		Image() {}

		//Creates a new image with a width, a height.
		Image(Size size)
		{
			m_width = size.width;
			m_height = size.height;
			m_data = (unsigned char*) malloc(m_width * m_height * 4);
			Fill(Color(0, 0, 0, 0));
		}

		//Creates a image from a file.
		Image(std::wstring filepath)
		{
			int form;
			unsigned char* data = stbi_load(StringTools::ToString(filepath).c_str(), (int*)&m_width, (int*)&m_height, &form, 0);
			if (!data)
			{
				ThrowException(L"Error loading image " + filepath);
				return;
			}

			Color t;

			switch (form)
			{
			case 1:
				m_data = (unsigned char*)malloc(m_width * m_height * 4);
				for (int x = 0; x < m_width; x++)
				{
					for (int y = 0; y < m_height; y++)
					{
						SetPixel(vec2(x, y), Color(data[*(data + ((x + y * m_width)))], data[*(data + ((x + y * m_width)))], data[*(data + ((x + y * m_width)))]));
					}
				}
				free(data);
				break;
			case 2: 
				m_data = (unsigned char*)malloc(m_width * m_height * 4);
				
				for (int x = 0; x < m_width; x++)
				{
					for (int y = 0; y < m_height; y++)
					{
						SetPixel(vec2(x, y), Color(*((data + ((x + y * m_width) * 2))), *((data + ((x + y * m_width) * 2))), *((data + ((x + y * m_width) * 2))), *((data + ((x + y * m_width) * 2)) + 1)));
					}
				}
				free(data);
				break;
			case 3: 
				m_data = (unsigned char*)malloc(m_width * m_height * 4);
				for (int x = 0; x < m_width; x++)
				{
					for (int y = 0; y < m_height; y++)
					{
						SetPixel(vec2(x, y), Color(*((data + ((x + y * m_width) * 3))), *((data + ((x + y * m_width) * 3)) + 1), *((data + ((x + y * m_width) * 3)) + 2), 255));
					}
				}
				free(data);
				break;
			case 4: 
				m_data = data;
				return;

			default:
				ThrowException(L"Unrecognized image format with file " + filepath);
				break;
			}
		}

		//Returns the width of the image
		inline unsigned int GetWidth()
		{
			return m_width;
		}

		//Returns the height of the image
		inline unsigned int GetHeight()
		{
			return m_height;
		}

		//DO NOT USE. Returns the pixels of the image.
		inline unsigned char* GetData()
		{
			return m_data;
		}

		//Returns the color of a pixel in the image
		inline Color GetPixel(vec2 pos)
		{
			Color c;
			for (unsigned char f = 0; f < 4; f++)
			{
				((unsigned char*)&c)[f] = *((m_data + ((pos.x + pos.y * m_width) * 4)) + f);
			}
			return c;
		}

		//Sets the color of a pixel.
		inline void SetPixel(vec2 pos, Color color)
		{
			int offset = (pos.x + pos.y *m_width) * 4;
			for (unsigned char i = 0; i <  4; i++)
				m_data[offset + i] = *((unsigned char*)&color + i);
		}
		
		//Fills the image with one color.
		void Fill(Color color)
		{
			int k = m_width * m_height * 4;
			for (int i = 0; i < k; i += 4)
			{
				for (int f = 0; f < 4; f++)
				{
					m_data[i + f] = *((unsigned char*)&color + f);
				}
			}
		}
		
		//Saves the image in a format. If that format is JPEG quality must be set between 1% and 100%. This parameter doesn't matter in other image formats. 
		void Save(std::wstring filepath, ImageFormat format, int quality)
		{
			switch (format)
			{
			case GizegoEngine::BMP:
				if (stbi_write_bmp(StringTools::ToString(filepath).c_str(), m_width, m_height, 4, m_data) == 0)
					ThrowException(L"Unable to save image " + filepath);
				break;
			case GizegoEngine::PNG:
				if (stbi_write_png(StringTools::ToString(filepath).c_str(), m_width, m_height, 4, m_data, 0) == 0)
					ThrowException(L"Unable to save image " + filepath);
				break;
			case GizegoEngine::JPEG:
				if (stbi_write_jpg(StringTools::ToString(filepath).c_str(), m_width, m_height, 4, m_data, quality) == 0)
					ThrowException(L"Unable to save image " + filepath);
				break;
			case GizegoEngine::TGA:
				if (stbi_write_tga(StringTools::ToString(filepath).c_str(), m_width, m_height, 4, m_data) == 0)
					ThrowException(L"Unable to save image " + filepath);
				break;
			}
		}

		//An image must be finalized before it can be rendered. This must be done after the window creation. Trying to edit an image or set it as an icon after finalization will crash the program.
		void Finalize()
		{
			HALF_VERT vecs[12] = 
			{
				0, 0,
				(int)m_width, (int)m_height,
				(int)m_width, 0,
				0, 0,
				(int)m_width, (int)m_height,
				0, (int) m_height
			};

			float tcs[12] = { 
				0, 0,
				1, 1,
				1, 0,
				0, 0,
				1, 1,
				0, 1
			};

			//VAO
			glGenVertexArrays(1, &m_vao);
			Bind();

			//VBO
			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vecs), vecs, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, VERTEX_TYPE, GL_FALSE, sizeof(int) * 2, NULL);		

			//TBO
			glGenBuffers(1, &m_tbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(tcs), tcs, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);

			//TEXTURE
			glGenTextures(1 ,&m_tex);
			Bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);

			//DELETING OLD IMAGE
			stbi_image_free(m_data);
			m_data = NULL;
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		//Renders the image to the current framebuffer. DO NOT USE.
		inline void Render()
		{
			Bind();
			glDrawArrays(GL_TRIANGLES, 0, 12);
		}

		~Image() {
			if (m_data)
				free(m_data);
			if (m_vao)
				glDeleteVertexArrays(1, &m_vao);
			if (m_vbo)
				glDeleteBuffers(1, &m_vbo);
			if (m_tbo)
				glDeleteBuffers(1, &m_tbo);
			if (m_tex)
				glDeleteTextures(1, &m_tex);
		}

	private:
		unsigned char* m_data;
		unsigned int m_width, m_height;
		unsigned int m_vao = 0, m_vbo = 0, m_tbo = 0, m_tex = 0;	

		inline void Bind()
		{
			Bindings::BindTexture(m_tex);
			Bindings::BindVAO(m_vao);
		}
	};
	
	//FillTriangles (Fill) or Lines (outline)
	enum GeometryRenderingMode : char
	{
		FillTriangles, Lines
	};

	//A 2D mesh
	class GeometryMesh
	{
	public:
		GeometryMesh() { 
			m_vao = 0; m_vbo = 0; m_count = 0; 
		}

		//Only call this after the window creation
		GeometryMesh(std::vector<HALF_VERT> vecs)
		{
			m_count = vecs.size();
			glGenVertexArrays(1, &m_vao);
			Bindings::BindVAO(m_vao);

			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, vecs.size() * 4, vecs.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, VERTEX_TYPE, GL_FALSE, sizeof(int) * 2, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		//Creates a regular polygon with n sides r radius. Only call this after the window creation. Render with FillTriangles.
		GeometryMesh(unsigned int n, unsigned int r)
		{
			std::vector<HALF_VERT> vecs;
			double dif = 6.28318530718 / n;			

			vecs.push_back(r);
			vecs.push_back(0);
			for (int i = 0; i < n; ++i)
			{
				vecs.push_back(cos(i * dif) * r);
				vecs.push_back(sin(i * dif) * r);
				vecs.push_back(0);
				vecs.push_back(0);
			}

			vecs.push_back(r);
			vecs.push_back(0);

			m_count = vecs.size();
			glGenVertexArrays(1, &m_vao);
			Bindings::BindVAO(m_vao);

			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, vecs.size() * 4, vecs.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		//DO NOT USE.
		void Render(GeometryRenderingMode mode, int offsetx, int offsety)
		{
			Shaders::ts->SetPosition(vec2(offsetx, offsety));
			Bindings::BindVAO(m_vao);
			switch (mode)
			{
			case GeometryRenderingMode::FillTriangles:
				glDrawArrays(GL_TRIANGLE_STRIP, 0, m_count);
				break;
			case GeometryRenderingMode::Lines:
				glDrawArrays(GL_LINES, 0, m_count);
				break;
			} 		
		}	

		~GeometryMesh()
		{
			if (m_vao)
				glDeleteVertexArrays(1 ,&m_vao);
			if (m_vbo)
				glDeleteBuffers(1, &m_vbo);
		}

	private:
		unsigned int m_vao = 0, m_vbo = 0, m_count = 0, m_rad;
	};
	
	class Font
	{
		struct Glyph
		{
			unsigned int x, y, w, h, adv, vao, vbo, tbo;

			void Destroy()
			{
				if (vao)
				{
					glDeleteVertexArrays(1, &vao);
					glDeleteBuffers(1, &vbo);
					glDeleteBuffers(1, &tbo);
				}
			}
		};
	public:
		Font() {}
		//Only call this constructor after Window::Create().
		Font(std::wstring filepath, unsigned int size, int range)
		{
			m_size = size;
			m_file = new IO::BinaryFile(filepath);

			stbtt_fontinfo info;
			if (!stbtt_InitFont(&info, m_file->GetData(), 0))
			{
				ThrowException(L"Failed to load font " + filepath);
				return;
			}
			float scale = stbtt_ScaleForPixelHeight(&info, size);
			int ascent, descent, lineGap;
			stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
			ascent *= scale;
			descent *= scale;			

			int tw = 0, th = 0;
			for (int i = 0; i < range; i++)
			{
				int x, y, w, h, adv;
				stbtt_GetCodepointBitmapBox(&info, i, scale, scale, &x, &y, &w, &h);
				stbtt_GetCodepointHMetrics(&info, i, &adv, NULL);
				tw += (w - x);
				if (th < (h - y))
					th = (h - y);
				Glyph g;
				g.x = x;
				g.y = y;
				g.w = (w - x);
				g.h = (h - y);
				g.adv = adv * scale;
				m_glyphs.push_back(g);
			}
			
			m_max = th;

			Image* atlas = new Image(Size(tw, th));
			int xoff = 0;
			for (int i = 0; i < range; i++)
			{								
				unsigned char* img = new unsigned char[m_glyphs[i].w * m_glyphs[i].h];
				stbtt_MakeCodepointBitmap(&info, img, m_glyphs[i].w, m_glyphs[i].h, m_glyphs[i].w, scale, scale, i);
				for (int b = 0; b < m_glyphs[i].w * m_glyphs[i].h; b++)
					atlas->SetPixel(vec2(b % m_glyphs[i].w + xoff, b / m_glyphs[i].w), Color(img[b], img[b], img[b], img[b]));

				delete[] img;
				
				glGenVertexArrays(1, &m_glyphs[i].vao);
				Bindings::BindVAO(m_glyphs[i].vao);

				HALF_VERT vecs[] = {
					0, 0,
					m_glyphs[i].w, m_glyphs[i].h,
					m_glyphs[i].w, 0,
					0, 0,
					m_glyphs[i].w, m_glyphs[i].h,
					0, m_glyphs[i].h,
				};

				float tcs[] = {
					(float)xoff / tw, 0,
					(float)(xoff + m_glyphs[i].w) / tw, (float)m_glyphs[i].h / th,
					(float)(xoff + m_glyphs[i].w) / tw, 0,
					(float)xoff / tw, 0,
					(float)(xoff + m_glyphs[i].w) / tw, (float)m_glyphs[i].h / th,
					(float)xoff / tw, (float)m_glyphs[i].h / th
				};

				glGenBuffers(1, &m_glyphs[i].vbo);
				glBindBuffer(GL_ARRAY_BUFFER, m_glyphs[i].vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vecs), vecs, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 2, VERTEX_TYPE, GL_FALSE, sizeof(int) * 2, NULL);

				glGenBuffers(1, &m_glyphs[i].tbo);
				glBindBuffer(GL_ARRAY_BUFFER, m_glyphs[i].tbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(tcs), tcs, GL_STATIC_DRAW);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				xoff += m_glyphs[i].w;
			}		
			glGenTextures(1, &m_tex);
			Bindings::BindTexture(m_tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas->GetWidth(), atlas->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas->GetData());
			delete atlas;
		}

		//DO NOT USE.
		vec2 RenderGlyph(wchar_t character, int x, int y)
		{
			if (character == L'\n')
				return vec2(-x, m_max);

			if (character == L'\t')
				return vec2(m_glyphs[9].adv, 0);

			Bindings::BindTexture(m_tex);
			Shaders::ts->SetPosition(vec2(x + m_glyphs[character].x, m_max + m_glyphs[character].y + y));
			Bindings::BindVAO(m_glyphs[character].vao);
			glDrawArrays(GL_TRIANGLES, 0, 12);
			return vec2(m_glyphs[character].adv, 0);
		}

		//DO NOT USE.
		vec2 MeasureString(std::wstring str)
		{
			vec2 ret;
			vec2 res;
			ret.y = m_max;
			for (int i = 0; i < str.size(); i++)
			{
				if (str[i] == L'\n')
				{
					if (res.x < ret.x)
						res.x = ret.x;
					ret.x = 0;
					ret.y += m_max;
				}

				if (str[i] == L'\t')
				{
					ret.x += m_glyphs[9].adv;
				}

				ret.x += m_glyphs[str[i]].adv;

				if (res.x < ret.x)
					res.x = ret.x;
			}
			return vec2(res.x, ret.y);
		}

		~Font() {
			for (int i = 0; i < m_glyphs.size(); i++)
				m_glyphs[i].Destroy();
			if (m_file)
				delete m_file;
			if (m_tex)
				glDeleteTextures(1, &m_tex);
		}

		//DO NOT USE.
		inline unsigned int GetMMax() { return m_max; }

	private:
		std::vector<Glyph> m_glyphs;
		IO::BinaryFile* m_file;
		unsigned int m_tex, m_size, m_max;
	};
	
	//Represents a square particle. It can be instanced with a ParticleInstance.
	class ParticleCore
	{
	public:
		ParticleCore() {}

		//Only call after Window::Create
		ParticleCore(Image* img) 
		{
			m_textured = true;
			CreateBuffers(img->GetWidth(), img->GetHeight());
			CreateTexture(img);
		}

		//Only call after Window::Create
		ParticleCore(unsigned int width, unsigned int height) 
		{
			m_textured = false;
			CreateBuffers(width, height);
		}

		~ParticleCore() 
		{
			if (m_vao)
				glDeleteVertexArrays(1, &m_vao);
			if (m_vbo)
				glDeleteBuffers(1, &m_vbo);
			if (m_tbo)
				glDeleteBuffers(1, &m_tbo);
			if (m_tex)
				glDeleteTextures(1, &m_tex);
		}

		//DO NOT USE.
		void RenderCore()
		{
			Bindings::BindVAO(m_vao);
			if (m_textured)
			{ 
				Shaders::ts->SetShaderType(ShaderType::Textured);
				Bindings::BindTexture(m_tex);
			}
			else
				Shaders::ts->SetShaderType(ShaderType::ColorOnly);
		}

	private:

		void CreateBuffers(unsigned int width, unsigned int height)
		{
			HALF_VERT vecs[12] =
			{
				0, 0,
				(int)width, (int)height,
				(int)width, 0,
				0, 0,
				(int)width, (int)height,
				0, (int)height
			};

			//VAO
			glGenVertexArrays(1, &m_vao);
			Bindings::BindVAO(m_vao);

			//VBO
			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vecs), vecs, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, VERTEX_TYPE, GL_FALSE, sizeof(int) * 2, NULL);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void CreateTexture(Image* img)
		{
			float tcs[12] = {
				0, 0,
				1, 1,
				1, 0,
				0, 0,
				1, 1,
				0, 1
			};

			//TBO
			glGenBuffers(1, &m_tbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(tcs), tcs, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			//TEXTURE
			glGenTextures(1, &m_tex);
			Bindings::BindTexture(m_tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->GetWidth(), img->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img->GetData());
		}

		unsigned int m_vao = 0, m_vbo = 0, m_tbo = 0, m_tex = 0;
		
		bool m_textured;	
	};

	//Represents an instance of a particle.
	class ParticleInstance
	{
	public:
		ParticleInstance() {}

		//Creates a particle. lifetime is in milliseconds.
		ParticleInstance(void(*Update)(unsigned long long ellapsed, vec2* pos, vec2 initpos, vecf* scale, Color* color), unsigned long long lifetime, vec2 position)
		{
			m_upd = Update;
			m_lifetime = lifetime;
			m_start = Time::TimeInMilliseconds();
			m_ivec = position;
			m_vec = vec2();
		}
		~ParticleInstance() {}

		//DO NOT USE if you are using ParticleInstancing. It will return true if the particle gets destroyed.
		bool Update()
		{
			if (Time::TimeInMilliseconds() >= m_start + m_lifetime)
			{ 
				delete this;
				return true;
			}
			m_upd(Time::TimeInMilliseconds() - m_start, &m_vec, m_ivec, &m_scale, &m_color);
			return false;
		}			

		//DO NOT USE.
		void Render()
		{
			Shaders::ts->SetPosition(m_vec);
			Shaders::ts->SetScale(m_scale);
			Shaders::ts->SetColor(m_color);
		}

	private:
		unsigned long long m_start, m_lifetime;
		void(*m_upd)(unsigned long long ellapsed, vec2* pos, vec2 initpos, vecf* scale, Color* color);
		vec2 m_vec, m_ivec;
		vecf m_scale = vecf(1, 1);
		Color m_color;
	};
	
	//Can be used to instanciate particles.
	class ParticleInstanciator
	{
	public:
		ParticleInstanciator() {}
		~ParticleInstanciator() { Clear(); }

		//Instances a particle.
		void InstantiateParticle(ParticleCore* core, void(*Update)(unsigned long long ellapsed, vec2* pos, vec2 initpos, vecf* scale, Color* color), unsigned long long lifetime, vec2 position)
		{
			m_cores.push_back(core);
			m_instances.push_back(new ParticleInstance(Update, lifetime, position));
			m_canrender.push_back(true);
		}

		//Call every frame
		void Update()
		{
			for (int i = 0; i < m_instances.size(); i++)
				if (m_canrender[i])
					m_canrender[i] = !m_instances[i]->Update();
		}

		//DO NOT USE.
		void Render()
		{
			for (int i = 0; i < m_instances.size(); ++i)
			{
				if (m_canrender[i])
				{ 
					m_cores[i]->RenderCore();
					m_instances[i]->Render();
					glDrawArrays(GL_TRIANGLES, 0, 12);
				}
			}
		}

		//Removes all particles from the list.
		void Clear()
		{
			for (int i = 0; i < m_cores.size(); i++)
				delete m_instances[i];
			m_cores.clear();
			m_instances.clear();
			m_canrender.clear();
		}

	private:
		std::vector<ParticleCore*> m_cores;
		std::vector<ParticleInstance*> m_instances;
		std::vector<bool> m_canrender;
	};
	
	//DO NOT USE.
	class SpriteAtlas
	{
	public:
		SpriteAtlas(Size size) 
		{
			m_atlas = new Image(size);
		}
		~SpriteAtlas() {
			delete m_atlas;
		}

		//DO NOT USE.
		unsigned int AddImage(Image* img)
		{
			unsigned int last = m_lastx;
			for (int x = 0; x < img->GetWidth(); x++)
			{
				for (int y = 0; y < img->GetHeight(); y++)
				{
					m_atlas->SetPixel(vec2(m_lastx + x, y), img->GetPixel(vec2(x, y)));
				}
			}
			m_lastx += img->GetWidth();
			return m_lastx;
		}

		//DO NOT USE.
		unsigned int Finalize()
		{
			unsigned int m_tex;
			glGenTextures(1, &m_tex);
			Bindings::BindTexture(m_tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_atlas->GetWidth(), m_atlas->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_atlas->GetData());
			return m_tex;
		}

	private:
		Image* m_atlas;
		unsigned int m_lastx = 0;
	};
	
	class Sprite
	{
	public:
		Sprite() {}
		//All images must be the same size. Different size images will bug when rendered. Only call after Window::Create().
		Sprite(std::vector<Image*> img) 
		{
			unsigned int width = 0, height;
			height = img[0]->GetHeight();
			for (int i = 0; i < img.size(); i++)
				width += img[i]->GetWidth();
			SpriteAtlas* atlas = new SpriteAtlas(Size(width, height));

			GenerateVBO(img[0]->GetWidth(), img[0]->GetHeight());

			for (int i = 0; i < img.size(); i++)
			{
				atlas->AddImage(img[i]);
				
				m_vao.push_back(0);
				glGenVertexArrays(1, &m_vao[i]);
				glBindVertexArray(m_vao[i]);

				m_tbo.push_back(GenerateTBO(i * img[0]->GetWidth(), img[i]->GetWidth(), width));
			}

			m_tex = atlas->Finalize();
			delete atlas;
		}
		~Sprite() {
			if (m_tex)
			{ 
				glDeleteBuffers(m_tbo.size(), m_tbo.data());
				glDeleteVertexArrays(m_vao.size(), m_vao.data());
				glDeleteTextures(1, &m_tex);
				glDeleteBuffers(1, &m_vbo);
			}
		}

		//DO NOT USE.
		inline void Render()
		{
			Bindings::BindTexture(m_tex);
			Bindings::BindVAO(m_vao[m_state]);
			glDrawArrays(GL_TRIANGLES, 0, 12);
		}

		//Sets the image to render.
		inline void SetAnimationState(unsigned int state)
		{
			m_state = state;
		}

	private:	
		std::vector<unsigned int> m_tbo;
		std::vector<unsigned int> m_vao;
		unsigned int m_vbo, m_tex, m_state = 0;

		unsigned int GenerateTBO(unsigned int x, unsigned int sx, unsigned int t)
		{
			unsigned int ret;
			float tcs[12] = 
			{
				(float) x / t, (float) 0,
				(float) (x + sx) / t, (float) 1,
				(float)(x + sx) / t, (float) 0,
				(float)x / t, (float)0,
				(float)(x + sx) / t, (float)1,
				(float)x / t, (float)1
			};

			glGenBuffers(1, &ret);
			glBindBuffer(GL_ARRAY_BUFFER, ret);
			glBufferData(GL_ARRAY_BUFFER, sizeof(tcs), tcs, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glVertexAttribPointer(0, 2, VERTEX_TYPE, GL_FALSE, sizeof(int) * 2, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			return ret;
		}

		void GenerateVBO(unsigned int width, unsigned int height)
		{
			HALF_VERT vecs[12] =
			{
				0, 0,
				(int)width, (int)height,
				(int)width, 0,
				0, 0,
				(int)width, (int)height,
				0, (int)height
			};

			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vecs), vecs, GL_STATIC_DRAW);			
		}
	};
	
	//The base component for tilemapping. It is an image or an image array that can be animated and will be drawn to render terrains.
	class Tile
	{
	public:
		Tile() {}

		//Creates an unanimated tile from an image;
		inline Tile(Image* image, bool hasalpha, bool hascollision)
		{
			m_anim = std::vector<Image*>();
			m_tbo = std::vector<unsigned int>(1);
			m_vao = std::vector<unsigned int>(1);
			m_x = std::vector<unsigned int>(1);
			m_y = std::vector<unsigned int>(1);
			m_interval = 0;
			m_alpha = hasalpha;
			m_col = hascollision;
			m_anim.push_back(image);
		}

		//Creates an animated tile from an image list. interval is the interval in milliseconds that it takes a tile to get to the next frame.
		inline Tile(std::vector<Image*> images, bool hasalpha, bool hascollision, unsigned int interval)
		{
			m_anim = images;
			m_vao = std::vector<unsigned int>(images.size());
			m_tbo = std::vector<unsigned int>(images.size());
			m_x = std::vector<unsigned int>(images.size());
			m_y = std::vector<unsigned int>(images.size());
			m_alpha = hasalpha;
			m_col = hascollision;
			m_interval = interval;
		}

		//DO NOT USE. This is only used when a tile is placed in an atlas;
		inline void SubmitLocation(unsigned int x, unsigned int y, unsigned int index)
		{
			m_x[index] = x;
			m_y[index] = y;
		}

		//Returns an image from the tile.
		inline Image* GetImage(unsigned int index)
		{
			return m_anim[index];
		}

		//Returns the length of the animation of the tile
		inline unsigned int GetFrameCount()
		{
			return m_anim.size();
		}

		//DO NOT USE BY YOURSELF. This is gonna be called in the TileAtlas.
		inline void Finalize(unsigned int t, unsigned int w, unsigned int h, unsigned int vbo)
		{
			for (unsigned int i = 0; i < m_anim.size(); i++)
			{ 
				glGenVertexArrays(1, &m_vao[i]);
				Bind(i);
				glVertexAttribPointer(0, 2, VERTEX_TYPE, GL_FALSE, sizeof(int) * 2, NULL);

				float tcs[12] =
				{
					(float)m_x[i] * t / w, (float)m_y[i] * t / h,
					(float)(t + m_x[i] * t) / w, (float)(t + m_y[i] * t) / h,
					(float)(t + m_x[i] * t) / w, (float)m_y[i] * t / h,
					(float)m_x[i] * t / w, (float)m_y[i] * t / h,
					(float)(t + m_x[i] * t) / w, (float)(t + m_y[i] * t) / h,
					(float)m_x[i] * t / w, (float)(t + m_y[i] * t) / h
				};

				glGenBuffers(1, &m_tbo[i]);
				glBindBuffer(GL_ARRAY_BUFFER, m_tbo[i]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(tcs), tcs, GL_STATIC_DRAW);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
			}
		}

		//DO NOT USE. This is called when rendering a terrain.
		void Render(unsigned int x, unsigned int y)
		{			
			Shaders::ts->SetPosition(vec2(x, y));	
			Bind(m_animstate);
			glDrawArrays(GL_TRIANGLES, 0, 12);
		}

		//Advances to the next animation state
		inline void NextState()
		{
			if (m_animstate != m_anim.size() - 1)
			{ 
				m_animstate++;
			}
			else
				m_animstate = 0;
		}

		//Returns the animation interval
		inline unsigned int GetInterval()
		{
			return m_interval;
		}

		//Returns if a tile has alpha or not
		inline bool HasAlpha()
		{
			return m_alpha;
		}

		//Returns if the tile has collision
		inline bool HasCollision()
		{
			return m_col;
		}

		~Tile() {
			glDeleteVertexArrays(m_vao.size(), m_vao.data());
			glDeleteBuffers(m_tbo.size(), m_tbo.data());
		}

	private:
		std::vector<Image*> m_anim;
		bool m_alpha = false, m_col = false;
		unsigned int m_animstate = 0, m_interval;
		std::vector<unsigned int> m_vao, m_tbo, m_x, m_y;
		inline void Bind(unsigned int state)
		{			
			Bindings::BindVAO(m_vao[state]);
		}
	};
	
	class TileAtlas
	{
	public:
		TileAtlas() {}
		//Creates a tile atlas width * height tiles. The tiles must be square and have a side length of tilesize.
		TileAtlas(Size size, unsigned int tilesize) 
		{
			m_width = size.width;
			m_height = size.height;
			m_ts = tilesize;
			m_ocp = new bool*[m_width];
			for (unsigned int i = 0; i < m_width; i++)
				m_ocp[i] = new bool[m_height];

			for (unsigned int x = 0; x < size.width; x++)
				for (unsigned int y = 0; y < size.height; y++)
					m_ocp[x][y] = false;
			m_atlas = new Image(Size(size.width * tilesize, size.height * tilesize));
			m_list =  std::vector<Tile*>();
			m_list.clear();
		}
		
		~TileAtlas() {
			if (m_atlas)
				delete m_atlas;

			if (m_vbo)
				glDeleteBuffers(GL_ARRAY_BUFFER, &m_vbo);

			if (m_tex)
				glDeleteTextures(GL_TEXTURE_2D, &m_tex);

			for (unsigned int i = 0; i < m_width; i++)
				delete[] m_ocp[i];
			delete[] m_ocp;
		}

		//Adds a tile to the atlas.
		void AddTile(Tile* tile)
		{	
			m_list.push_back(tile);
			for (int i = 0; i < tile->GetFrameCount(); i++)
			{
				
				for (unsigned int y = 0; y < m_height; y++)
				{
					for (unsigned int x = 0; x < m_width; x++)
					{						
						if (m_ocp[x][y] == false)
						{
							tile->SubmitLocation(x, y, i);
														
							Image* img = tile->GetImage(i);
							
							for (int a = 0; a < m_ts; a++)
								for (int b = 0; b < m_ts; b++)
									m_atlas->SetPixel(vec2(a + x * 32, b + y * 32), img->GetPixel(vec2(a, b)));

							m_ocp[x][y] = true;
							goto LABEL;
						}
					}
				}
				ThrowException(L"Unable to add tile to the tile atlas because it was full");
				return;
			LABEL:;
			}			
		}

		//Returns a tile. It's id is generated by the order it was placed in the atlas.
		inline Tile* GetTile(unsigned int id)
		{
			if (id < m_list.size())
				return m_list[id];
		}

		//You should call this function before using this to render terrains.
		void Finalize()
		{
			//Create Texture
			glGenTextures(1, &m_tex);
			Bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_atlas->GetWidth(), m_atlas->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_atlas->GetData());

			//Create general VBO
			HALF_VERT vecs[12] =
			{
				0, 0,
				(int)m_ts, (int)m_ts,
				(int)m_ts, 0,
				0, 0,
				(int)m_ts, (int)m_ts,
				0, (int)m_ts
			};

			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vecs), vecs, GL_STATIC_DRAW);
			
			for (int i = 0; i < m_list.size(); i++)
			{
				m_list[i]->Finalize(m_ts, m_atlas->GetWidth(), m_atlas->GetHeight(), m_vbo);
				m_last.push_back(Time::TimeInMilliseconds());
				glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		//Must be called every frame
		void Update()
		{
			for (int i = 0; i < m_list.size(); i++)
				if (m_list[i]->GetInterval() > 0 && Time::TimeInMilliseconds() >= m_last[i] + m_list[i]->GetInterval())
				{ 
					m_list[i]->NextState();
					m_last[i] = Time::TimeInMilliseconds();
				}
		}

		//DO NOT USE. This is OpenGL related.
		inline unsigned int GetTextureID()
		{
			return m_tex;
		}

		//Returns the size of a tile in pixels.
		inline unsigned int GetTilesize()
		{
			return m_ts;
		}

	private:
		Image* m_atlas;
		bool** m_ocp;
	
		unsigned int m_width, m_height, m_ts, m_tex, m_vbo;
		std::vector<Tile*> m_list;
		std::vector<unsigned long long> m_last;

		inline void Bind()
		{
			Bindings::BindTexture(m_tex);
		}
	};
	
	class Camera
	{
	public:
		Camera() 
		{
			m_x = 0;
			m_y = 0;
			m_width = 0;
			m_height = 0;
		}
		~Camera() {}

		//Sets the size of the camera grid.
		inline void SetSize(Size size)
		{
			m_width = size.width;
			m_height = size.height;
		}

		//Gets the width of the camera frid.
		inline unsigned int GetWidth()
		{
			return m_width;
		}

		//Gets the height of the camera frid.
		inline unsigned int GetHeight()
		{
			return m_height;
		}

		//Sets the camera position.
		inline void SetPosition(int x,int y)
		{
			m_x = x;
			m_y = y;
		}

		//Gets the x position.
		inline int GetX()
		{
			return m_x;
		}

		//Gets the y position.
		inline int GetY()
		{
			return m_y;
		}

		//Adds x and y to the current position.
		inline void AddPosition(vec2 pos)
		{
			m_x += pos.x;
			m_y += pos.y;
		}

	private:
		int m_x, m_y;
		unsigned int m_width, m_height;
	};
	
	//DO NOT USE. This is integrated in the terrain class.
	class LightMap
	{
		struct LightTile
		{
			Color c;
			unsigned short averages;
			unsigned int id;
		};

	public:
		LightMap() {}

		//DO NOT USE. This is integrated in the terrain class.
		LightMap(Size size, Color color) 
		{
			m_width = size.width;
			m_height = size.height;

			m_data = new LightTile*[size.width];
			for (unsigned int x = 0; x < size.width; x++)
			{
				m_data[x] = new LightTile[size.height];
			}

			for (int x = 0; x < m_width; x++)
				for (int y = 0; y < m_height; y++)
				{ 
					LightTile tile;
					tile.c = color;
					tile.averages = 1;
					tile.id = 0;
					m_data[x][y] = tile;
				}
		}

		//DO NOT USE. This is integrated in the terrain class.
		unsigned int AddLight(unsigned int range, Color color, int xpos, int ypos, bool avg)
		{
			unsigned int id = m_cid;
			m_cid++;
			
			SetTileColor(color, xpos, ypos, id, avg);
	
			for (int r = 0; r < range; r++)
			{ 
				for (int x = xpos - range + 1; x < xpos + range; x++)
					for (int y = ypos - range + 1; y < ypos + range; y++)
					{ 
						SetTileColor(color, x - 1, y, id, avg);
						SetTileColor(color, x + 1, y, id, avg);
						SetTileColor(color, x, y + 1, id, avg);
						SetTileColor(color, x, y - 1, id, avg);
					}
			}

			return id;
		}

		//DO NOT USE. This is integrated in the terrain class.
		inline void SetTileColor(Color c, int x, int y, unsigned int id, bool avg)
		{
			if (x < m_width && y < m_height)
			{
				if (avg)
				{ 
					if (m_data[x][y].id != id)
					{ 
					unsigned int r = m_data[x][y].c.R * m_data[x][y].averages + c.R, g = m_data[x][y].c.G * m_data[x][y].averages + c.G,
						b = m_data[x][y].c.B * m_data[x][y].averages + c.B, a = m_data[x][y].c.A * m_data[x][y].averages + c.A;
					m_data[x][y].averages++;
					m_data[x][y].c = Color(r / m_data[x][y].averages, g / m_data[x][y].averages, b / m_data[x][y].averages, a / m_data[x][y].averages);					
					m_data[x][y].id = id;
					}
				}
				else
				{ 
					m_data[x][y].c = c;
					m_data[x][y].id = id;
				}
			}
		}

		//DO NOT USE. This is integrated in the terrain class.
		inline Color GetTileColor(int x, int y)
		{
			if (x < m_width && y < m_height)
				return m_data[x][y].c;
			return Color(255, 255, 255, 255);
		}

		~LightMap() 
		{
			for (unsigned int x = 0; x < m_width; x++)
			{
				delete[] m_data[x];
			}
			delete[] m_data;
		}

	private:
		LightTile** m_data;
		unsigned int m_width, m_height, m_cid = 1;
	};
	
	//A tilemap terrain
	class Terrain
	{
	public:
		Terrain() {}
		Terrain(Size size, unsigned int layers = 1, Color LightmapBackcolor = Color(255, 255, 255))
		{
			m_light = new LightMap(size, LightmapBackcolor);
			data = new unsigned int**[layers];

			for (int l = 0; l < layers; l++)
			{ 
				data[l] = new unsigned int*[size.width];
				for (unsigned int x = 0; x < size.width; x++)
				{ 
					data[l][x] = new unsigned int[size.height];
				}
			}
			m_width = size.width;
			m_height = size.height;
			m_layers = layers;

			for (int i = 0; i < layers; i++)
			{
				FillLayer(i, 0);
			}
		}

		Terrain(std::wstring filepath, Color LightmapBackcolor = Color(255, 255, 255))
		{
			IO::BinaryFile file = IO::BinaryFile(filepath);
			std::vector<unsigned char> buffer = std::vector<unsigned char>();
			unsigned char* filedata = file.GetData();
			for (int i = 0; i < file.GetSize(); i++)
				buffer.push_back(filedata[i]);
			std::vector<unsigned char> tmp;
			tmp = { buffer[0], buffer[1], buffer[2], buffer[3] };
			m_width = BinaryConverter::ToType<unsigned int>(tmp);
			tmp = { buffer[4], buffer[5], buffer[6], buffer[7] };
			m_height = BinaryConverter::ToType<unsigned int>(tmp);
			tmp = { buffer[8], buffer[9], buffer[10], buffer[11] };
			m_layers = BinaryConverter::ToType<unsigned int>(tmp);

			m_light = new LightMap(Size(m_width, m_height), LightmapBackcolor);
			data = new unsigned int**[m_layers];

			for (int l = 0; l < m_layers; l++)
			{
				data[l] = new unsigned int*[m_width];
				for (unsigned int x = 0; x < m_width; x++)
				{
					data[l][x] = new unsigned int[m_height];
				}
			}

			int s = 12;

			for (int l = 0; l < m_layers; l++)
			{
				for (int x = 0; x < m_width; x++)
				{
					for (int y = 0; y < m_height; y++)
					{
						tmp = { buffer[s], buffer[s + 1], buffer[s + 2], buffer[s + 3] };
						data[l][x][y] = BinaryConverter::ToType<unsigned int>(tmp);
						s += 4;
					}
				}
			}
		}

		//Gets the tile id of a place in the terrain.
		inline unsigned int GetTile(unsigned int layer, unsigned int x, unsigned int y)
		{
			if (x < m_width && y < m_height && layer < m_layers)
				return data[layer][x][y];
			else
				return 0xffffffff;
		}

		//Sets the tile id of a place in the terrain.
		inline void SetTile(unsigned int layer, unsigned int x, unsigned int y, unsigned int value)
		{
			if (x < m_width && y < m_height && layer < m_layers)
				data[layer][x][y] = value;
		}

		//Fills a layer with a tile.
		inline void FillLayer(unsigned int layer, unsigned int value)
		{
			for (int x = 0; x < m_width; x++)
			{
				for (int y = 0; y < m_height; y++)
				{
					data[layer][x][y] = value;
				}
			}
		}

		//Returns the width of the terrain.
		inline unsigned int GetWidth()
		{
			return m_width;
		}

		//Returns the height of the terrain.
		inline unsigned int GetHeight()
		{
			return m_height;
		}

		//Returns the number of layers in the terrain.
		inline unsigned int GetLayerCount()
		{
			return m_layers;
		}

		//Adds a light to the terrain lightmap. Wall layer will be the layer of light obstacles (every non-transparent tile)
		inline void AddLight(unsigned int range, Color color, int x, int y, bool mixlights)
		{
			m_light->AddLight(range, color, x, y, mixlights);
		}

		//DO NOT USE. 
		inline LightMap* GetLightMap()
		{
			return m_light;
		}

		//Removes all lights and fills the light map with a color.
		inline void ResetLights(Color BackColor)
		{
			if (m_light)
				delete m_light;
			m_light = new LightMap(Size(m_width, m_height), BackColor);
		}

		//Saves a terrain to a file. Lights will not be saved.
		void SaveToFile(std::wstring filepath)
		{
			std::vector<unsigned char> data;
			std::vector<unsigned char> tmp = BinaryConverter::ToBytes(m_width);

			for (int i = 0; i < tmp.size(); ++i)
				data.push_back(tmp[i]);

			tmp = BinaryConverter::ToBytes(m_height);
			for (int i = 0; i < tmp.size(); ++i)
				data.push_back(tmp[i]);

			tmp = BinaryConverter::ToBytes(m_layers);
			for (int i = 0; i < tmp.size(); ++i)
				data.push_back(tmp[i]);

			for (int l = 0; l < m_layers; ++l)
			{
				for (int x = 0; x < m_width; ++x)
				{
					for (int y = 0; y < m_height; ++y)
					{
						tmp = BinaryConverter::ToBytes(this->data[l][x][y]);
						for (int i = 0; i < tmp.size(); ++i)
							data.push_back(tmp[i]);
					}
				}
			}

			IO::BinaryFile::Save(filepath, data);
		}


		~Terrain()
		{
			if (data)
			{ 
				for (int l = 0; l < m_layers; l++)
				{			
					for (unsigned int x = 0; x < m_width; x++)
					{
						delete[] data[l][x];
					}
					delete[] data[l];
				}
			}
			delete m_light;
			delete[] data;
		}

	private:
		unsigned int*** data;
		LightMap* m_light;
		unsigned int m_width, m_height, m_layers;
	};
	
	//Used to check collisions.
	class TilemapEntity
	{
	public:
		TilemapEntity() {}
		TilemapEntity(unsigned int width, unsigned int height) 
		{
			m_width = width;
			m_height = height;
		}

		//Sets the entity size
		inline void SetSize(unsigned int width, unsigned int height)
		{
			m_width += width;
			m_height += height;
		}

		//Adds x and y to the entity position checking for collision. layer is the layer where to find walls.
		bool Move(vec2 pos, int tilesize, int layer, Terrain* ter, TileAtlas* atlas)
		{
			if ((m_x + m_width) % tilesize == 0)
			{
				if (pos.x > 0)
				{
					unsigned int t1 = ter->GetTile(layer, (m_x + m_width) / tilesize, std::floor(m_y / (double)tilesize));
					unsigned int t2 = ter->GetTile(layer, (m_x + m_width) / tilesize, std::ceil(m_y / (double)tilesize));
					bool tile1 = false, tile2 = false;
					if (t1 != 0xffffffff)
						tile1 = atlas->GetTile(t1)->HasCollision();
					if (t2 != 0xffffffff)
						tile2 = atlas->GetTile(t2)->HasCollision();

					if (!(tile1 || tile2))
					{
						m_x += pos.x;
						return true;
					}
					else
						return false;
				}
				else if (pos.x < 0)
				{
					unsigned int t1 = ter->GetTile(layer, (m_x + m_width) / tilesize - 2, std::floor(m_y / (double)tilesize));
					unsigned int t2 = ter->GetTile(layer, (m_x + m_width) / tilesize - 2, std::ceil(m_y / (double)tilesize));
					bool tile1 = false, tile2 = false;
					if (t1 != 0xffffffff)
						tile1 = atlas->GetTile(t1)->HasCollision();
					if (t2 != 0xffffffff)
						tile2 = atlas->GetTile(t2)->HasCollision();

					if (!(tile1 || tile2))
					{
						m_x += pos.x;
						return true;
					}
					else
						return false;
				}
			}

			if ((m_y + m_height) % tilesize == 0)
			{
				if (pos.y > 0)
				{
					unsigned int t1 = ter->GetTile(layer, std::floor(m_x / (double)tilesize), (m_y + m_height) / tilesize);
					unsigned int t2 = ter->GetTile(layer, std::ceil(m_x / (double)tilesize), (m_y + m_height) / tilesize);
					bool tile1 = false, tile2 = false;
					if (t1 != 0xffffffff)
						tile1 = atlas->GetTile(t1)->HasCollision();
					if (t2 != 0xffffffff)
						tile2 = atlas->GetTile(t2)->HasCollision();

					if (!(tile1 || tile2))
					{
						m_y += pos.y;
						return true;
					}
					else
						return false;
				}
				else if (pos.y < 0)
				{
					unsigned int t1 = ter->GetTile(layer, std::floor(m_x / (double)tilesize), (m_y + m_height) / tilesize - 2);
					unsigned int t2 = ter->GetTile(layer, std::ceil(m_x / (double)tilesize), (m_y + m_height) / tilesize - 2);
					bool tile1 = false, tile2 = false;
					if (t1 != 0xffffffff)
						tile1 = atlas->GetTile(t1)->HasCollision();
					if (t2 != 0xffffffff)
						tile2 = atlas->GetTile(t2)->HasCollision();

					if (!(tile1 || tile2))
					{
						m_y += pos.y;
						return true;
					}
					else
						return false;
				}
			}

			m_x += pos.x;
			m_y += pos.y;
			return true;
		}

		//Sets the entity position without checking for collision
		inline void SetPosition(int x, int y)
		{
			m_x = x;
			m_y = y;
		}

		//Returns the entity X position
		inline int GetX()
		{
			return m_x;
		}

		//Returns the entity Y position
		inline int GetY()
		{
			return m_y;
		}

		~TilemapEntity() {}

	private:
		int m_x = 0, m_y = 0;
		unsigned int m_width, m_height;
	};
	
	class Renderer;

	//An UI element.
	class UIElement
	{
	public:

		//Checks if the mouse in clicking the element.
		virtual bool IsClicked() = 0;
		//DO NOT USE.
		virtual void Render(Renderer*) = 0;
		//Call this every frame.
		virtual void Update() = 0;


		//Sets the component poition.
		inline void SetPosition(vec2 pos)
		{
			this->pos = pos;
		}
		
		//Gets the poition of the element.
		inline vec2 GetPos()
		{
			return pos;
		}

	private:
		vec2 pos;
	};
	
	//Represents a framebuffer.
	class Renderer
	{
	public:
		Renderer() {}
		//Only call after Window::Create()
		Renderer(Size size, bool IsWindow)
		{
			if (IsWindow)
			{
				m_width = size.width;
				m_height = size.height;
				s_wind = this;
				m_frame = 0;
				m_tex = 0;
			}
			else
			{
				m_width = size.width;
				m_height = size.height;
				glGenFramebuffers(1, &m_frame);
				Bind();
				glGenTextures(1, &m_tex);
				glBindTexture(GL_TEXTURE_2D, m_tex);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0);

				//VAO
				glGenVertexArrays(1, &m_vao);
				Bindings::BindVAO(m_vao);

				HALF_VERT vecs[12] =
				{
					0, 0,
					(int)m_width, (int)m_height,
					(int)m_width, 0,
					0, 0,
					(int)m_width, (int)m_height,
					0, (int)m_height
				};

				float tcs[12] = {
					0, 1,
					1, 0,
					1, 1,
					0, 1,
					1, 0,
					0, 0
				};

				//VBO
				glGenBuffers(1, &m_vbo);
				glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vecs), vecs, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 2, VERTEX_TYPE, GL_FALSE, sizeof(int) * 2, NULL);

				glGenBuffers(1, &m_tbo);
				glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(tcs), tcs, GL_STATIC_DRAW);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				const GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
				glDrawBuffers(1, DrawBuffers);
				Fill(Color(0, 0, 0));
			}
		}

		//DO NOT USE. This function is called when the window is resized;
		static void UpdateWindowFramebuffer(Size size)
		{
			if (s_wind)
			{
				s_wind->m_width = size.width;
				s_wind->m_height = size.height;
			}
			else
				ThrowException(L"Unable to resize window renderer");
		}

		//Fills the framebuffer with a color
		inline void Fill(Color color)
		{
			Bind();
			if (m_frame == 0)
			{
				float* gl = color.ToGL();
				glClearColor(gl[0], gl[1], gl[2], gl[3]);
				delete[] gl;
			}
			else
			{
				Shaders::ts->SetShaderType(ShaderType::ColorOnly);
				Bindings::BindVAO(m_vao);
				Shaders::ts->SetColor(color);
				glDrawArrays(GL_TRIANGLES, 0, 12);
			}
		}

		//Renders a image to this renderer output.
		inline void Render(Image* img, vec2 pos, vecf scale = vecf(1, 1), Color backcolor = Color(255, 255, 255))
		{
			Bind();
			Shaders::ts->SetShaderType(ShaderType::Textured);
			Shaders::ts->SetColor(backcolor);
			Shaders::ts->SetPosition(pos);
			Shaders::ts->SetScale(scale);
			img->Render();
		}

		//Retreives the content of this renderer. This function takes some time to be processed and must be called when you're done rendering.
		inline Image* GetContent()
		{
			Bind();
			Image* img = new Image(Size(m_width, m_height));
			glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, img->GetData());
			return img;
		}

		//Renders a terrain depending on a camera.
		void Render(Terrain* ter, Camera* cam, TileAtlas* atlas)
		{
			Bindings::BindTexture(atlas->GetTextureID());
			Shaders::ts->SetShaderType(ShaderType::Textured);
			Shaders::ts->SetScale(vecf(1, 1));

			unsigned int size = atlas->GetTilesize();
			int start_x = cam->GetX();
			int start_y = cam->GetY();

			int end_x = std::floor(start_x / (double) size) + cam->GetWidth() + 1;
			int end_y = std::floor(start_y / (double)size) + cam->GetHeight() + 1;

			for (int x = std::floor(start_x / (double)size) - 1; x < end_x ; x++)
			{
				for (int y = std::floor(start_y / (double)size) - 1; y < end_y; y++)
				{
					for (int l = 0; l < ter->GetLayerCount(); l++)
					{
						
						unsigned int val;

						bool alphaup = false;
						for (int a = l; a < ter->GetLayerCount(); a++)
						{ 
							val = ter->GetTile(a, x, y);
							if (val != 0xffffffff)
								if (atlas->GetTile(val)->HasAlpha())
									alphaup = true;
						}

						if (l == ter->GetLayerCount() - 1 || alphaup)
						{ 
							val = ter->GetTile(l, x, y);
							if (val != 0xffffffff)
							{ 
								Shaders::ts->SetColor(ter->GetLightMap()->GetTileColor(x, y));
								atlas->GetTile(val)->Render(x * size - cam->GetX() , y  * size - cam->GetY());
							}
						}
					}
				}
			}
		}

		//Renders a geometry mesh. DO NOT USE isprogress.
		inline void Render(GeometryMesh* mesh, Color color, GeometryRenderingMode mode, vec2 pos, vecf scale = vecf(1, 1))
		{
			Bind();
			Shaders::ts->SetShaderType(ShaderType::ColorOnly);
			Shaders::ts->SetColor(color);
			//DONT SET POS
			Shaders::ts->SetScale(scale);
			mesh->Render(mode, pos.x, pos.y);
		}

		//Renders the content of another renderer.
		inline void Render(Renderer* renderer, Color color, vec2 pos, vecf scale = vecf(1, 1))
		{
			Bind();
			Shaders::ts->SetShaderType(ShaderType::Textured);
			renderer->BindForRendering();
			Shaders::ts->SetColor(color);
			Shaders::ts->SetPosition(pos);
			Shaders::ts->SetScale(scale);
			glDrawArrays(GL_TRIANGLES, 0, 12);
		}

		//Renders a string with a font.
		inline void Render(std::wstring string, Font* font, vec2 pos, Color color = Color(255, 255, 255))
		{
			Bind();
			Shaders::ts->SetShaderType(ShaderType::Textured);
			Shaders::ts->SetColor(color);
			Shaders::ts->SetScale(vecf(1, 1));
			for (int i = 0; i < string.length(); i++)
			{
				vec2 vec = font->RenderGlyph(string[i], pos.x, pos.y);
				pos.x += vec.x;
				pos.y += vec.y;
				Shaders::ts->SetPosition(pos);
			}
		}

		//Renders a sprite.
		inline void Render(Sprite* sprite, vec2 pos, vecf scale = vecf(1, 1), Color color = Color(255, 255, 255))
		{
			Bind();
			Shaders::ts->SetShaderType(ShaderType::Textured);
			Shaders::ts->SetPosition(pos);
			Shaders::ts->SetColor(color);
			Shaders::ts->SetScale(scale);
			sprite->Render();
		}

		//Renders a particle. Can be used but ParticleInstancer may be more practicle.
		inline void Render(ParticleCore* core, ParticleInstance* particle)
		{
			Bind();
			core->RenderCore();
			particle->Render();
			glDrawArrays(GL_TRIANGLES, 0, 12);
		}

		//Renders all the particles in a instancer.
		inline void Render(ParticleInstanciator* instanciator)
		{
			Bind();
			instanciator->Render();
		}

		//Renders an UI element
		inline void Render(UIElement* element)
		{
			element->Render(this);
		}

		inline ~Renderer() {
			if (m_frame != 0)
				glDeleteFramebuffers(1, &m_frame);
			if (m_tex != 0)
				glDeleteTextures(1, &m_tex);
			if (m_vao)
			{ 
				glDeleteVertexArrays(1, &m_vao);
				glDeleteBuffers(1, &m_vbo);
				glDeleteBuffers(1, &m_tbo);
			}
		}

	private:
		unsigned int m_frame, m_tex, m_width, m_height, m_vao, m_vbo, m_tbo;
		static unsigned int m_bound;
		static Renderer* s_wind;

		inline void Bind()
		{		
			Bindings::BindFramebuffer(m_frame, m_width, m_height);
		}

		inline void BindForRendering()
		{
			Bindings::BindTexture(m_tex);
			Bindings::BindVAO(m_vao);
		}
	};
	unsigned int Renderer::m_bound;
	Renderer* Renderer::s_wind;
	
	class Window
	{
	public:
		Window() { m_window = NULL; }
		~Window() 
		{
			if (m_window)
				glfwDestroyWindow(m_window);
			if (icon)
				delete[] icon;
			if (m_renderer)
				delete m_renderer;
		}
		//Creates a window with this parameters. To show for the first time it use Create().
		Window(Size size, std::wstring title) {
			m_width = size.width;
			m_height = size.height;
			m_title = title;
		}

		//Creates the window. This should only be executed once in your program after using the constructor with width, height and title. Don´t delete the renderer.
		Renderer* Create(GLVersion version = GLVersion(3, 0))
		{	
			m_window = glfwCreateWindow(m_width, m_height, StringTools::ToString(m_title).c_str() , NULL, NULL);
			glfwMakeContextCurrent(m_window);

			if (!m_initialized)
			{
				if (!gladLoadGL())
				{ 
					ThrowException(L"GL version not supported");
					return NULL;
				}
				m_initialized = true;
				
				Shaders::ts = new TextureShader();
				
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				Shaders::ts->CreateUpLayer();

				m_renderer = new Renderer(Size(m_width, m_height), true);
			}

			int maj, min;
			glGetIntegerv(GL_MAJOR_VERSION, &maj);
			glGetIntegerv(GL_MINOR_VERSION, &min);
			if (maj < version.Major)
			{
				ThrowException(std::wstring(L"Your GPU does not support OpenGL ") + std::to_wstring(version.Major) + L" " + std::to_wstring(version.Minor));
				return NULL;
			}
			else
			{
				if (maj == version.Major)
				{
					if (min < version.Minor)
					{
						ThrowException(std::wstring(L"Your GPU does not support OpenGL ") + std::to_wstring(version.Major) + L" " + std::to_wstring(version.Minor));
						return NULL;
					}
				}
			}

			mainwindow = this;

			glfwSetFramebufferSizeCallback((GLFWwindow*)m_window, GLFWresize);
			glfwSetScrollCallback(m_window, Input::ScrollResponse);
			glfwSetCharCallback(m_window, Input::TextResponse);
			glfwSetKeyCallback(m_window, Input::KeyResponse);
			GLFWresize(m_window, m_width, m_height);
			return m_renderer;
		}

		//Returns if the window is open.
		inline bool IsOpen() { if (m_window) return !glfwWindowShouldClose(m_window); else ThrowException(L"Window hasn't been created yet"); return false; }
		
		//Call this in your game loop.
		void Update()
		{
			glfwPollEvents();
			double x, y;
			glfwGetCursorPos(Window::GetMainWindow()->GetHandle(), &x, &y);
			Input::m_x = (int)x;
			Input::m_y = (int)y;

			Input::last_frame.reserve(Input::this_frame.size());
			Input::last_frame.resize(Input::this_frame.size());

			for (unsigned int i = 0; i < Input::this_frame.size(); i++)
				Input::last_frame[i] = Input::this_frame[i];

			Input::this_frame.clear();
		}

		//Swaps the old frame for the new one you rendered. Call this every frame.		
		inline void SwapBuffers() {
			if (m_window) { glfwSwapBuffers(m_window);  glClear(GL_COLOR_BUFFER_BIT); }
			else ThrowException(L"Window hasn't been created yet");
		}
		//Gets the size of the window.
		inline Size GetSize() { return Size(m_width, m_height); }
		//Gets the title.
		inline std::wstring GetTitle() { return m_title; }
		//Sets the title.
		inline void SetTitle(std::wstring title) { if (m_window) glfwSetWindowTitle(m_window, StringTools::ToString(title).c_str()); else ThrowException(L"Window hasn't been created yet");}
		//Closes the window.
		inline void Close() { if (m_window) glfwSetWindowShouldClose(m_window, true); else ThrowException(L"Window hasn't been created yet");}
		//Hides the window.
		inline void Hide() { if (m_window) glfwHideWindow(m_window); else ThrowException(L"Window hasn't been created yet");}
		//Shows the window. Not needed after Create() but don't call this before calling that function.
		inline void Show() { if (m_window) glfwShowWindow(m_window); else ThrowException(L"Window hasn't been created yet");}
		//Maximizes the window
		inline void Maximize() { if (m_window) glfwMaximizeWindow(m_window); else ThrowException(L"Window hasn't been created yet");}
		//Minimizes the window
		inline void Minimize() { if (m_window) glfwIconifyWindow(m_window); else ThrowException(L"Window hasn't been created yet");}
		//Checks if the window is maximized
		inline bool IsMaximized() 
		{
			if (m_window)
			{
				if (glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED) == 1)
					return true;
			}
			else ThrowException(L"Window hasn't been created yet");
			return false;
		}
		//Checks if the window is minimized
		inline bool IsMinimized()
		{
			if (m_window)
			{ 
				if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) == 1)
					return true;
			}
			else ThrowException(L"Window hasn't been created yet");
			return false;
		}
		//Turns VSync on or off
		inline void SetVSyncState(bool state) 
		{
			m_vsync = state;
			if (state)
			{ 
				glfwSwapInterval(1);
			}
			else
			{ 
				glfwSwapInterval(0);			
			}
		}

		//Gets VSync state
		inline bool GetVSyncState() { return m_vsync; }

		//Puts the window in fullscreen
		inline void FullScreen(Display* display)
		{
			if (m_window)
				glfwSetWindowMonitor(m_window, display->GetMonitor(), display->GetX(), display->GetY(), display->GetWidth(), display->GetHeight(), display->GetRefreshRate());
			else ThrowException(L"Window hasn't been created yet");
		}

		//Puts the window in fullscreen with a costum resolution
		inline void FullScreen(Display* display, Size size)
		{
			if (m_window)
				glfwSetWindowMonitor(m_window, display->GetMonitor(), display->GetX(), display->GetY(), size.width, size.height, display->GetRefreshRate());
			else ThrowException(L"Window hasn't been created yet");
		}

		//Sets the windo icon.
		void SetIcon(Image* icon)
		{
			if (m_window)
			{
				if (!this->icon)
					this->icon = new GLFWimage[1];
				this->icon[0].pixels = icon->GetData();
				this->icon[0].width = icon->GetWidth();
				this->icon[0].height = icon->GetHeight();
				glfwSetWindowIcon(m_window, 1, (const GLFWimage*)this->icon);
			}
			else ThrowException(L"Window hasn't been created yet");
		}

		//Resizes the window.
		inline void Resize(Size size)
		{
			IncompleteResize(size.width, size.height);
			glfwSetWindowSize(m_window, size.width, size.height);
		}

		//DO NOT USE UNLESS YOU ARE WORKING WITH THE ENGINE DIRECTLY. Returns the glfwWindow*.
		inline GLFWwindow* GetHandle()
		{
			return m_window;
		}

		//Returns the last and only window that you created. 
		inline static Window* GetMainWindow()
		{
			return mainwindow;
		}

		//Returns the window position
		inline vec2 GetWindowPosition()
		{
			int xpos, ypos;
			glfwGetWindowPos(m_window, &xpos, &ypos);
			return vec2(xpos, ypos);
		}

		//Sets the window position
		inline void SetPosition(vec2 pos)
		{
			glfwSetWindowPos(m_window, pos.x, pos.y);
		}

	private:
		static bool m_initialized;
		unsigned int m_width, m_height;
		std::wstring m_title;
		GLFWwindow* m_window;
		bool m_vsync;
		GLFWimage* icon;
		Renderer* m_renderer;
		static Window* mainwindow;

		inline static void GLFWresize(GLFWwindow* window, int width, int height)
		{
			mainwindow->IncompleteResize(width, height);
		}

		void IncompleteResize(int width, int height)
		{
			m_width = width;
			m_height = height;
			Renderer::UpdateWindowFramebuffer(Size(width, height));
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, width, height);
			float matrix[16] = {
				(float)  2.0f / width, 0, 0, 0,
				0, (float)  2.0f / -((float)height), 0, 0,
				0, 0, 2, 0,
				-1, 1, 0, 1
			};
			Shaders::ts->SetMatrix(matrix);
		}
	};
	Window* Window::mainwindow;
	bool Window::m_initialized;
	
	std::wstring* Input::KeyBind::m_keynames;

	namespace Input
	{
		//Waits for a keypress and returns the key you pressed. Also works with mouse buttons. The window must be created before using this fuctions.
		KeyBind WaitForKeypress()
		{
			while (true)
			{ 		
				glfwPollEvents();
				for (int i = 32; i < GLFW_KEY_LAST; i++)
				{ 
					if (glfwGetKey(Window::GetMainWindow()->GetHandle(), i) == GLFW_PRESS)
					{
						return KeyBind(i);
					}					
				}
				for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
				{ 
					if (glfwGetMouseButton(Window::GetMainWindow()->GetHandle(), i) == GLFW_PRESS)
					{
						return KeyBind(i, true);
					}
				}
			}
		}

		//Checks if there is any key or mouse button pressed. It will return key or mouse button pressed at the moment. In case the of none of the keys or mouse buttons being pressed this will return -1.
		//In case of multiple keys or mouse buttons being pressed it will return the key with the lower values, first it process keys and then mouse buttons. Use GetKeysPressed() to get a list of all the keys pressed at the moment
		KeyBind GetKeyPressed()
		{
			glfwPollEvents();
			for (int i = 32; i < GLFW_KEY_LAST; i++)
				if (glfwGetKey(Window::GetMainWindow()->GetHandle(), i) == GLFW_PRESS)
				{
					return KeyBind(i);
				}
			for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
				if (glfwGetMouseButton(Window::GetMainWindow()->GetHandle(), i) == GLFW_PRESS)
				{
					return KeyBind(i, true);
				}

			return KeyBind(-1);
		}

		//Returns all the keys and mouse buttons pressed at the moment at the call.
		std::vector<KeyBind> GetKeysPressed()
		{
			std::vector<KeyBind> keybinds;
			glfwPollEvents();
			for (int i = 32; i < GLFW_KEY_LAST; i++)
				if (glfwGetKey(Window::GetMainWindow()->GetHandle(), i) == GLFW_PRESS)
				{
					keybinds.push_back(i);
				}
			for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
				if (glfwGetMouseButton(Window::GetMainWindow()->GetHandle(), i) == GLFW_PRESS)
				{
					keybinds.push_back(KeyBind(i, true));
				}
			return keybinds;
		}

		//Checks if the key is pressed at the moment of the function call
		bool IsKeyPressed(KeyBind kb)
		{
			if (kb.IsMouseButton())
			{ 
				if (glfwGetMouseButton(Window::GetMainWindow()->GetHandle(), kb.GetKeyNumber()) == GLFW_PRESS)
					return true;
			}
			else
				if (glfwGetKey(Window::GetMainWindow()->GetHandle(), kb.GetKeyNumber()) == GLFW_PRESS)
					return true;
			return false;
		}

		//Returns the X position of the mouse inside the window. Call Input::Update() every frame.
		inline int GetMouseX()
		{			
			return m_x;
		}

		//Returns the Y position of the mouse inside the window. Call Input::Update() every frame.
		inline int GetMouseY()
		{
			return m_y;
		}

		//Returns the state of the vertical mouse scroll.
		inline int GetMouseScrollY()
		{
			return m_sy;
		}

		//Returns the state of the horizontal mouse scroll. In most mouses the scroll is only vertical but in the apple mouse the scroll is both vertical and horizontal.
		inline unsigned int GetMouseScrollX()
		{
			return m_sx;
		}

		//Returns the text input sice the last time you called this function.
		inline std::wstring GetTextInput()
		{
			std::wstring temp;
			temp += m_textinput;
			m_textinput = L"";
			return temp;
		}

		//Checks is a key or mouse button is pressed down. It only returns true once when the is pushed down. If it is already down it will return false.
		bool KeyClicked(KeyBind kb)
		{
			if (IsKeyPressed(kb))
			{ 
				this_frame.push_back(kb);

				for (unsigned int i = 0; i < last_frame.size(); i++)
				{
					if (last_frame[i].GetKeyNumber() == kb.GetKeyNumber())
						return false;
				}

				return true;
			}
			return false;
		}
	}
	
	//Operating system default cursors.
	enum CursorType
	{
		Normal = GLFW_ARROW_CURSOR,
		Text = GLFW_IBEAM_CURSOR,
		Crosshair = GLFW_CROSSHAIR_CURSOR,
		VerticalResize = GLFW_VRESIZE_CURSOR,
		HorizontalResize = GLFW_HRESIZE_CURSOR
	};

	//A cursor (mouse pointer image)
	class Cursor
	{
	public:
		Cursor() {}

		//Creates a cursor from an image.
		Cursor(Image* img)
		{
			GLFWimage image;
			image.width = img->GetWidth();
			image.height = img->GetHeight();
			image.pixels = img->GetData();
			m_cursor = glfwCreateCursor(&image, 0, 0);
		}

		//Creates a system cursor
		Cursor(CursorType type)
		{
			m_cursor = glfwCreateStandardCursor(type);
		}

		//Hides the cursor
		inline static void HideCursor()
		{
			glfwSetInputMode(Window::GetMainWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		//Makes the cursor visible
		inline static void ShowCursor()
		{
			glfwSetInputMode(Window::GetMainWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		//Sets this as the window cursor
		inline void Use()
		{
			glfwSetCursor(Window::GetMainWindow()->GetHandle(), m_cursor);
		}

		~Cursor() 
		{
			if (m_cursor)
				glfwDestroyCursor(m_cursor);
		}

	private:
		GLFWcursor* m_cursor;
	};
	
	//An UI button. 
	class Button : public UIElement
	{
	public:
		Button() {}

		//Creates a button. Deletefont will dictate if the font pointer will be deleted in the deconstructor.
		Button(Font* font, Color fontcolor, Color backcolor,std::wstring text, bool deletefont)
		{
			m_font = font;
			m_fontcolor = fontcolor;
			m_backcolor = backcolor;
			m_deletefont = deletefont;
			m_text = text;

			vec2 size = font->MeasureString(text);
#define BS 6
			std::vector<HALF_VERT> vecs = 
			{
				0, 0,				
				size.x + BS, 0,
				size.x + BS, size.y + BS,
				0, 0,				
				0, size.y + BS,
				size.x + BS, size.y + BS,
			};
			width = size.x + BS;
			height = size.y + BS;
#undef BS			
			m_geo = new GeometryMesh(vecs);
			SetPosition(vec2());
		}

		~Button() 
		{
			if (m_font && m_deletefont)
				delete m_font;
			if (m_geo)
				delete m_geo;
		}

		//Call this every frame.
		inline void Update() override {}

		//Checks if the element is clicked.
		inline bool IsClicked() override
		{
			if (GizegoEngine::Input::KeyClicked(GizegoEngine::Input::KeyBind(GLFW_MOUSE_BUTTON_1, true)) && GizegoEngine::Input::GetMouseX() > GetPos().x && GizegoEngine::Input::GetMouseX() < width + GetPos().x && GizegoEngine::Input::GetMouseY() > GetPos().y && GizegoEngine::Input::GetMouseY() < height + GetPos().y)
				return true;
			return false;
		}

		//DO NOT USE.
		inline void Render(Renderer* renderer) override
		{
			renderer->Render(m_geo, m_backcolor, FillTriangles, GetPos());
			renderer->Render(m_text, m_font, vec2(GetPos().x + 3, GetPos().y + 3), m_fontcolor);
		}

	private:
		Font* m_font; 
		bool m_deletefont; 
		Color m_fontcolor, m_backcolor;
		GeometryMesh* m_geo;
		std::wstring m_text;
		unsigned int width, height;
	};
	
	//An UI label (text)
	class Label : public UIElement
	{
	public:
		Label() {}

		Label(Font* font, Color fontcolor, std::wstring text, bool deletefont)
		{
			m_font = font;
			m_deletefont = deletefont;
			m_fontcolor = fontcolor;
			m_text = text;
			SetPosition(vec2());
		}

		~Label() {
			if (m_deletefont && m_font)
				delete m_font;
		}

		//Sets the text of the label.
		inline void SetText(std::wstring text)
		{
			m_text = text;
		}

		//Checks if the element is clicked.
		inline bool IsClicked() override
		{
			vec2 size = m_font->MeasureString(m_text);
			if (GizegoEngine::Input::KeyClicked(GizegoEngine::Input::KeyBind(GLFW_MOUSE_BUTTON_1, true)) && GizegoEngine::Input::GetMouseX() > GetPos().x && GizegoEngine::Input::GetMouseX() < size.x + GetPos().x && GizegoEngine::Input::GetMouseY() > GetPos().y && GizegoEngine::Input::GetMouseY() < size.y + GetPos().y)
				return true;
			return false;
		}

		//Call this every frame.
		inline void Update() override {}

		//DO NOT USE.
		inline void Render(Renderer* renderer) override
		{
			renderer->Render(m_text, m_font, GetPos(), m_fontcolor);
		}
	private:
		Font* m_font;
		bool m_deletefont;
		Color m_fontcolor;
		std::wstring m_text;
	};

	//A UI picture.
	class PictureBox : public UIElement
	{
	public:
		PictureBox() {}

		//The image you throw in this function must be finalized. deleteimage dictates if the image is deleted when this UI compenent is deleted.
		PictureBox(Image* image, bool deleteimage)
		{
			m_img = image;
			m_deleteimage = deleteimage;
			SetPosition(vec2());
		}

		~PictureBox() {
			if (m_deleteimage)
				delete m_img;
		}

		//Checks if the element is clicked.
		inline bool IsClicked() override
		{
			if (GizegoEngine::Input::KeyClicked(GizegoEngine::Input::KeyBind(GLFW_MOUSE_BUTTON_1, true)) && GizegoEngine::Input::GetMouseX() > GetPos().x && GizegoEngine::Input::GetMouseX() < m_img->GetWidth() + GetPos().x && GizegoEngine::Input::GetMouseY() > GetPos().y && GizegoEngine::Input::GetMouseY() < m_img->GetHeight() + GetPos().y)
				return true;
			return false;
		}

		//Call this every frame.
		inline void Update() override {}

		//DO NOT USE.
		inline void Render(Renderer* renderer) override
		{
			renderer->Render(m_img, GetPos());
		}
	private:
		Image* m_img;
		bool m_deleteimage;
	};

	//An UI progress bar.
	class ProgressBar : public UIElement
	{
	public:
		ProgressBar() {}

		ProgressBar(unsigned int width, unsigned int height, Color backcolor, Color progresscolor) 
		{
			m_width = width;
			m_height = height;
			m_cback = backcolor;
			m_cfront = progresscolor;

			std::vector<HALF_VERT> vecs =
			{
				0, 0,
				width, 0,
				width, height,
				0, 0,
				0, height,
				width, height,
			};

			m_back = new GeometryMesh(vecs);
			m_front = new GeometryMesh(vecs);
			SetPosition(vec2());
		}

		~ProgressBar() 
		{
			if (m_back)
				delete m_back;
			if (m_front)
				delete m_front;
		}

		//Sets the value of the progressbar (0 to 1).
		inline void SetValue(float value)
		{
			m_value = value;
		}

		//Gets the value of the progress bar.
		inline float GetValue()
		{
			return m_value;
		}

		//Checks if the element is clicked.
		inline bool IsClicked() override
		{
			if (GizegoEngine::Input::KeyClicked(GizegoEngine::Input::KeyBind(GLFW_MOUSE_BUTTON_1, true)) && GizegoEngine::Input::GetMouseX() > GetPos().x && GizegoEngine::Input::GetMouseX() < m_width + GetPos().x && GizegoEngine::Input::GetMouseY() > GetPos().y && GizegoEngine::Input::GetMouseY() < m_height + GetPos().y)
				return true;
			return false;
		}

		//Call this every frame.
		inline void Update() override {}

		//DO NOT USE.
		inline void Render(Renderer* renderer) override
		{
			renderer->Render(m_back, m_cback, FillTriangles, GetPos());
			renderer->Render(m_front, m_cfront, FillTriangles, GetPos(), vecf(m_value, 1));
		}

	private:
		GeometryMesh* m_back, *m_front;
		Color m_cback, m_cfront;
		unsigned int m_width, m_height;
		float m_value = 0;
	};

	//An UI checkbox.
	class CheckBox : public UIElement
	{
	public:
		CheckBox() {}
		CheckBox(Font* font, Color fontcolor, std::wstring text, bool deletefont)
		{
			m_deletefont = deletefont;
			m_font = font;
			m_text = text;
			m_tcolor = fontcolor;
			SetPosition(vec2());
			m_clicked = false;

			std::vector<HALF_VERT> vertices =
			{
				0, 0,
				font->GetMMax(), 0,
				font->GetMMax(), font->GetMMax(),
				0, 0,
				0, font->GetMMax(),
				font->GetMMax(), font->GetMMax()
			};

			m_sqr = new GeometryMesh(vertices);

			vertices =
			{
				0, 0,
				font->GetMMax(), font->GetMMax(),
				font->GetMMax(), 0,
				0, font->GetMMax()
			};

			m_cross = new GeometryMesh(vertices);
		}

		//Returns the state of the checkbox.
		inline bool GetState()
		{
			return m_clicked;
		}

		//Sets the text of the check box.
		inline void SetText(std::wstring text)
		{
			m_text = text;
		}

		//DO NOT USE.
		inline void Render(Renderer* renderer) override
		{
			renderer->Render(m_sqr, Color(255, 255, 255), FillTriangles, GetPos());
			if (m_clicked)
				renderer->Render(m_cross, Color(0, 0, 0), Lines, GetPos());
			renderer->Render(m_text, m_font, vec2(m_font->GetMMax() + GetPos().x + 3, GetPos().y), m_tcolor);
		}

		//Call this every frame.
		inline void Update() override
		{
			IsClicked();
		}

		//Checks if the element is clicked.
		inline bool IsClicked() override
		{
			if (GizegoEngine::Input::KeyClicked(GizegoEngine::Input::KeyBind(GLFW_MOUSE_BUTTON_1, true)) && GizegoEngine::Input::GetMouseX() > GetPos().x && GizegoEngine::Input::GetMouseX() < m_font->GetMMax() + GetPos().x && GizegoEngine::Input::GetMouseY() > GetPos().y && GizegoEngine::Input::GetMouseY() < m_font->GetMMax() + GetPos().y)
			{
				m_clicked = !m_clicked;
				return true;
			}
			return false;
		}

		~CheckBox() 
		{
			if (m_deletefont)
				delete m_font;
			delete m_sqr;
			delete m_cross;
		}

	private:
		GeometryMesh* m_sqr, *m_cross;
		bool m_deletefont, m_clicked;
		Font* m_font;
		std::wstring m_text;
		Color m_tcolor;
	};

	class NumberMarking
	{
	public:
		NumberMarking() {}
		NumberMarking(double value, unsigned int height, bool showvalue) 
		{
			m_value = value;
			m_height = height;
			m_write = showvalue;
		}
		~NumberMarking() {}
	
		//DO NOT USE.
		double m_value;
		//DO NOT USE.
		unsigned int m_height;
		//DO NOT USE.
		bool m_write;
	};

	//An UI number slider.
	class NumberSlider : public UIElement
	{
	public:
		NumberSlider() {}

		NumberSlider(Color linecolor, Color cursorcolor, std::vector<NumberMarking> markings, Font* font, bool deletefont, unsigned int width, double minvalue, double maxvalue, double startval, bool cliptomarkings) 
		{
			SetPosition(vec2());
			
			double pp = (maxvalue - minvalue) / (double) width;
			m_cpos = (startval - minvalue) / pp;

			m_clip = cliptomarkings;
			m_width = width;
			m_min = minvalue;
			m_max = maxvalue;
			m_cline = linecolor;
			m_ccur = cursorcolor;
			m_marks = markings;
			m_font = font;
			m_deletefont = deletefont;

			std::vector<float> vecs = { 0, 0, width, 0 };
			m_line = new GeometryMesh(vecs);
			vecs = { 0, -1, 0, 1 };
			m_mark = new GeometryMesh(vecs);
			vecs =
			{
				-2, -5,
				2, -5,
				2, 5,
				-2, -5,
				-2, 5,
				2, 5
			};
			m_cur = new GeometryMesh(vecs);
		}

		//Gets the value of the slider.
		inline double GetValue()
		{
			double pp = (m_max - m_min) / (double)m_width;
			return (m_cpos * pp) + m_min;
		}

		//Sets the value of the slider.
		inline void SetValue(double value)
		{
			double pp = (m_max - m_min) / (double)m_width;
			m_cpos = (value - m_min) / pp;
		}

		//Checks if the element is clicked.
		inline bool IsClicked() override
		{
			if (GizegoEngine::Input::IsKeyPressed(GizegoEngine::Input::KeyBind(GLFW_MOUSE_BUTTON_1, true)) && GizegoEngine::Input::GetMouseX() > GetPos().x + m_cpos - 2 && GizegoEngine::Input::GetMouseX() < m_cpos + GetPos().x + 2)
			{
				if (GizegoEngine::Input::GetMouseY() > GetPos().y - 5 && GizegoEngine::Input::GetMouseY() < 5 + GetPos().y)
					return true;
			}
			return false;
		}

		//Call this every frame.
		void Update() override
		{
			if (GizegoEngine::Input::IsKeyPressed(GizegoEngine::Input::KeyBind(GLFW_MOUSE_BUTTON_1, true)) && GizegoEngine::Input::GetMouseX() > GetPos().x + m_cpos - 2 && GizegoEngine::Input::GetMouseX() < m_cpos + GetPos().x + 2)
			{	
				if (GizegoEngine::Input::GetMouseY() > GetPos().y - 5 && GizegoEngine::Input::GetMouseY() < 5 + GetPos().y)
					m_press = true;
			}
			else if (!GizegoEngine::Input::IsKeyPressed(GizegoEngine::Input::KeyBind(GLFW_MOUSE_BUTTON_1, true)))
			{
				m_press = false;
			}

			if (m_press)
			{
				if (m_clip)
					Clip(Input::GetMouseX() - GetPos().x);
				else
				{
					unsigned int val = Input::GetMouseX();
					if (val > GetPos().x && val < GetPos().x + m_width)
						m_cpos = val - GetPos().x;
					else if (val < GetPos().x)
						m_cpos = 0;
					else
						m_cpos = m_width;
				}
			}
		}

		//DO NOT USE.
		void Render(Renderer* renderer) override
		{
			renderer->Render(m_line, m_cline, Lines, GetPos());

			double pp = (m_max - m_min) / (double)m_width;
			for (int i = 0; i < m_marks.size(); i++)
			{
				renderer->Render(m_mark, m_cline, Lines, vec2(GetPos().x + ((m_marks[i].m_value - m_min) / pp), GetPos().y), vecf(1, m_marks[i].m_height));

				if (m_marks[i].m_write)
				{
					char buffer[32];
					snprintf(buffer, sizeof(buffer), "%g", m_marks[i].m_value);
					std::wstring text = StringTools::ToWstring(std::string(buffer));

					vec2 mes = m_font->MeasureString(text);
					renderer->Render(text, m_font, vec2(GetPos().x + ((m_marks[i].m_value - m_min) / pp) - mes.x / 2, GetPos().y + m_marks[i].m_height + 1));
				}
			}

			renderer->Render(m_cur, m_ccur, FillTriangles, vec2(GetPos().x + m_cpos, GetPos().y));		
		}

		~NumberSlider()
		{
			if (m_cur)
				delete m_cur;
			if (m_line)
				delete m_line;
			if (m_mark)
				delete m_mark;
			if (m_deletefont && m_font)
				delete m_font;
		}

	private:
		unsigned int m_width, m_cpos;
		double m_min, m_max;
		GeometryMesh* m_cur, *m_line, *m_mark;
		Color m_cline, m_ccur;
		std::vector<NumberMarking> m_marks;
		bool m_press, m_deletefont, m_clip;
		Font* m_font;

		void Clip(int mx)
		{
			double pp = (m_max - m_min) / (double)m_width;
			unsigned int first, last;
			for (int i = 0; i < m_marks.size(); i++)
			{
				if ((m_marks[i].m_value - m_min) / pp < mx)
					first = (m_marks[i].m_value - m_min) / pp;
			}

			for (int i = m_marks.size() - 1; i >= 0; i--)
			{
				if ((m_marks[i].m_value - m_min) / pp > mx)
					last = (m_marks[i].m_value - m_min) / pp;
			}

			unsigned int dfirst, dlast;
			dfirst = std::abs(mx - (int)first);
			dlast = std::abs(mx - (int)last);

			if (dfirst < dlast)
				m_cpos = first;
			else if (dlast < dfirst)
				m_cpos = last;
			else
				m_cpos = last;
		}
	};

	//An UI radio button.
	class RadioButton : public UIElement
	{
	public:
		RadioButton() {}
		RadioButton(Font* font, Color textcolor, bool deletefont, std::wstring text, unsigned int circledetail = 32) 
		{
			m_font = font;
			m_deletefont = deletefont;
			m_text = text;
			m_color = textcolor;
			SetPosition(vec2());

			m_circle = new GeometryMesh(circledetail, font->GetMMax() / 2);
		}
		~RadioButton() 
		{
			delete m_circle;
			if (m_deletefont)
				delete m_font;
		}

		//DO NOT USE.
		inline void Render(Renderer* renderer) override
		{
			renderer->Render(m_circle, Color(255, 255, 255), FillTriangles, vec2(GetPos().x + m_font->GetMMax() / 2, GetPos().y + m_font->GetMMax() / 2));
			if (m_selected)
				renderer->Render(m_circle, Color(0, 0, 0), FillTriangles, vec2(GetPos().x + m_font->GetMMax() / 2, GetPos().y + m_font->GetMMax() / 2), vecf(0.66, 0.66));
			renderer->Render(m_text, m_font, vec2(GetPos().x + 3 + m_font->GetMMax(), GetPos().y));
		}

		//Sets the text of the radio button.
		inline void SetText(std::wstring text) { m_text = text; }

		//Returns if the radio button on or off.
		inline bool GetState()
		{
			return m_selected;
		}

		//Call every frame.
		inline void Update() override
		{
			if (GizegoEngine::Input::KeyClicked(GizegoEngine::Input::KeyBind(GLFW_MOUSE_BUTTON_1, true)) && GizegoEngine::Input::GetMouseX() > GetPos().x && GizegoEngine::Input::GetMouseX() < m_font->GetMMax() + GetPos().x && GizegoEngine::Input::GetMouseY() > GetPos().y && GizegoEngine::Input::GetMouseY() < m_font->GetMMax() + GetPos().y)
			{
				ClickEvent(this);
			}
		}

		//Checks if the element is clicked.
		inline bool IsClicked() override
		{
			if (GizegoEngine::Input::KeyClicked(GizegoEngine::Input::KeyBind(GLFW_MOUSE_BUTTON_1, true)) && GizegoEngine::Input::GetMouseX() > GetPos().x && GizegoEngine::Input::GetMouseX() < m_font->GetMMax() + GetPos().x && GizegoEngine::Input::GetMouseY() > GetPos().y && GizegoEngine::Input::GetMouseY() < m_font->GetMMax() + GetPos().y)
			{
				return true;
			}
			return false;
		}
		
		//Adds a list of radio button relations.
		inline static unsigned int AddRelationList(std::vector<RadioButton*> relation)
		{
			relations.push_back(relation);
			return relations.size() - 1;
		}

		//DO NOT USE.
		void SetState(bool state)
		{
			m_selected = state;
		}

	private:
		Font* m_font;
		std::wstring m_text;
		bool m_deletefont, m_selected = false;
		Color m_color;
		GeometryMesh* m_circle;

		static std::vector<std::vector<RadioButton*>> relations;

		static void ClickEvent(RadioButton* src)
		{
			for (int i = 0; i < relations.size(); i++)
			{
				for (int j = 0; j < relations[i].size(); j++)
				{
					if (relations[i][j] == src)
					{
						for (int k = 0; k < relations[i].size(); k++)
							relations[i][k]->SetState(false);
						relations[i][j]->SetState(true);
						return;
					}
				}
			}
		}
	};
	std::vector<std::vector<RadioButton*>> RadioButton::relations;

	//An UI textbox.
	class Textbox : public UIElement
	{
	public:
		Textbox() {}

		Textbox(Font* font, Color backcolor, Color fontcolor, bool deletefont, unsigned int width)
		{
			SetPosition(vec2());
			m_font = font;
			m_backcolor = backcolor;
			m_deletefont = deletefont;
			m_fontcolor = fontcolor;
			m_width = width;
			m_textwidth = 0;

			std::vector<HALF_VERT> vecs =
			{
				0, 0,
				0, font->GetMMax() + 3,
				6 + width, font->GetMMax() + 3,
				0, 0,
				6 + width, 0,
				6 + width, font->GetMMax() + 3
			};
			m_geo = new GeometryMesh(vecs);

			vecs = { 3, 3, 3, font->GetMMax() };

			m_cur = new GeometryMesh(vecs);
		}

		//Checks if the element is clicked.
		inline bool IsClicked() override
		{
			if (GizegoEngine::Input::KeyClicked(GizegoEngine::Input::KeyBind(GLFW_MOUSE_BUTTON_1, true)) && GizegoEngine::Input::GetMouseX() > GetPos().x && GizegoEngine::Input::GetMouseX() < m_width + 3 + GetPos().x && GizegoEngine::Input::GetMouseY() > GetPos().y && GizegoEngine::Input::GetMouseY() < m_font->GetMMax() + GetPos().y + 3)
				return true;
			return false;
		}

		//Call this every frame
		void Update() override
		{
			if (m_clicked && Time::TimeInMilliseconds() >= m_lasttime + 500)
			{
				m_canshow = !m_canshow;
				m_lasttime = Time::TimeInMilliseconds();
			}

			if (GizegoEngine::Input::KeyClicked(GizegoEngine::Input::KeyBind(GLFW_MOUSE_BUTTON_1, true)))
			{
				if (GizegoEngine::Input::GetMouseY() > GetPos().y && GizegoEngine::Input::GetMouseY() < m_font->GetMMax() + GetPos().y + 3)
				{
					m_clicked = true;
					m_lasttime = Time::TimeInMilliseconds();
					m_canshow = true;
				}
				else if (GizegoEngine::Input::KeyClicked(GizegoEngine::Input::KeyBind(GLFW_MOUSE_BUTTON_1, true)))
				{
					m_clicked = false;
				}
			}

			if (m_clicked)
			{
				std::wstring str = Input::GetTextInput();
				for (int i = 0; i < str.length(); i++)
					if (str[i] == 8)
					{
						if (m_text.length() > 0)
						{							
								m_text = StringTools::GetPart(m_text, 0, m_cpos - 1) + StringTools::GetPart(m_text, m_cpos, m_text.length());
								m_cpos--;
								m_textwidth = m_font->MeasureString(StringTools::GetPart(m_text, 0, m_cpos)).x;
						}
					}
					else if (str[i] == 1)
					{
						if (m_cpos > 0)
						{
							m_cpos--;
							m_textwidth = m_font->MeasureString(StringTools::GetPart(m_text, 0, m_cpos)).x;
						}
					}
					else if (str[i] == 2)
					{
						if (m_cpos < m_text.length())
						{
							m_cpos++;
							m_textwidth = m_font->MeasureString(StringTools::GetPart(m_text, 0, m_cpos)).x;
						}
					}
					else
					{
						vec2 size = m_font->MeasureString(m_text + str[i]);
						if (size.x < m_width)
						{
							m_text = StringTools::GetPart(m_text, 0, m_cpos) + str[i] + StringTools::GetPart(m_text, m_cpos, m_text.length());
							m_cpos++;
							m_textwidth = m_font->MeasureString(StringTools::GetPart(m_text, 0, m_cpos)).x;
						}
					}
			}
			else
			{
				Input::GetTextInput();
			}
		}

		//Sets the cursor position.
		inline void SetCursorPos(unsigned int pos)
		{
			m_cpos = pos;
		}

		//Gets the cursor position.
		inline unsigned int GetCursorPos()
		{
			return m_cpos;
		}

		//Gets the content of the textbox
		inline std::wstring GetText()
		{
			return m_text;
		}

		//DO NOT USE.
		inline void Render(Renderer* renderer) override
		{
			renderer->Render(m_geo, m_backcolor, FillTriangles, GetPos());
			renderer->Render(m_text, m_font, vec2(GetPos().x + 3, GetPos().y), m_fontcolor);
			if (m_clicked && m_canshow)
				renderer->Render(m_cur, m_fontcolor, Lines, vec2(GetPos().x + 1 + m_textwidth, GetPos().y));
		}

		~Textbox() 
		{
			if (m_geo)
				delete m_geo;
			if (m_cur)
				delete m_cur;
			if (m_font && m_deletefont)
				delete m_font;
		}

	private:
		GeometryMesh* m_geo, *m_cur;
		Font* m_font;
		Color m_backcolor, m_fontcolor;
		std::wstring m_text;
		bool m_deletefont, m_clicked = false, m_canshow = true;
		unsigned int m_width, m_cpos = 0, m_textwidth;
		unsigned long long m_lasttime;
	};

	//An UI ComboBox.
	class ComboBox : public UIElement
	{
	public:
		ComboBox() {}
		ComboBox(Font* font, Color backcolor, Color fontcolor, Color clickcolor, bool deletefont, unsigned int width)
		{
			SetPosition(vec2());
			m_font = font;
			m_cback = backcolor;
			m_cfont = fontcolor;
			m_cclick = clickcolor;
			m_deletefont = deletefont;
			m_width = width;

			std::vector<HALF_VERT> vecs =
			{
				0, 0,
				0, font->GetMMax() + 3,
				width, font->GetMMax() + 3,
				0, 0,
				width, 0,
				width, font->GetMMax() + 3
			};
			m_box = new GeometryMesh(vecs);
			vecs =
			{
				0, 0,
				12, 0,				
				12, font->GetMMax() + 3,
				0, 0,
				0, font->GetMMax() + 3,
				12, font->GetMMax() + 3
			};
			m_blue = new GeometryMesh(vecs);
			vecs =
			{
				0, 0,
				6, 0,
				4, 6,				
			};
			m_arrow = new GeometryMesh(vecs);

			vecs =
			{
				0, 0,
				m_width + 12, 0,
				m_width + 12, 1,
				0, 0,
				0, 1,
				m_width + 12, 1
			};

			m_dropgeo = new GeometryMesh(vecs);
		}
		~ComboBox() 
		{
			//GeometryMesh* m_box, *m_arrow, *m_blue, *m_dropgeo;
			//Font* m_font;
			if (m_deletefont)
				delete m_font;
			if (m_box)
				delete m_box;
			if (m_arrow)
				delete m_arrow;
			if (m_blue)
				delete m_blue;
			if (m_dropgeo)
				delete m_dropgeo;
		}

		//DO NOT USE.
		void Render(Renderer* renderer) override 
		{
			renderer->Render(m_box, m_cback, FillTriangles, GetPos());			
			renderer->Render(m_blue, m_cclick, FillTriangles, vec2(GetPos().x + m_width, GetPos().y));
			renderer->Render(m_arrow, m_cfont, FillTriangles, vec2(GetPos().x + m_width + 2, GetPos().y + (m_font->GetMMax() / 2)));
			renderer->Render(m_text, m_font, vec2(GetPos().x + 3, GetPos().y), m_cfont);

			if (m_drop)
			{
				renderer->Render(m_dropgeo, m_cback, FillTriangles, vec2(GetPos().x, GetPos().y + m_font->GetMMax()), vecf(1, (m_font->GetMMax() + 3) * m_elements.size() + 3));
				
				for (int i = 0; i < m_elements.size(); i++)
				{
					renderer->Render(m_elements[i], m_font, vec2(GetPos().x + 3, GetPos().y + (m_font->GetMMax() + 3) * (i + 1)), m_cfont);
				}
			}
		}

		//Sets the list of elements.
		inline void SetElements(std::vector<std::wstring> elements)
		{
			m_elements = elements;
		}

		//Gets the value of the box.
		inline std::wstring GetValue()
		{
			return m_text;
		}

		//Checks if the element is clicked.
		bool IsClicked() override 
		{
			if (GizegoEngine::Input::KeyClicked(GizegoEngine::Input::KeyBind(GLFW_MOUSE_BUTTON_1, true)))
			{				
				if (GizegoEngine::Input::GetMouseX() > GetPos().x + m_width && GizegoEngine::Input::GetMouseX() < GetPos().x + m_width + 12 && GizegoEngine::Input::GetMouseY() > GetPos().y && GizegoEngine::Input::GetMouseY() < m_font->GetMMax() + GetPos().y + 3)
				{
					m_drop = !m_drop;
					return true;
				}
				

				if (m_drop)
				{
					if (GizegoEngine::Input::GetMouseX() > GetPos().x && GizegoEngine::Input::GetMouseX() < GetPos().x + m_width + 12)
					{
						if (GizegoEngine::Input::GetMouseY() > GetPos().y + m_font->GetMMax() && GizegoEngine::Input::GetMouseY() < ((m_font->GetMMax() + 3) * (m_elements.size() + 1)) + GetPos().y + 3)
						{
							m_text = m_elements[((GizegoEngine::Input::GetMouseY() - GetPos().y - 3)  * m_elements.size() / ((m_font->GetMMax() + 3) * (m_elements.size()))) - 1];
							m_drop = false;
							return false;
						}
					}
				}
				m_drop = false;
			}			
			return false;
		}

		//Call this every frame.
		void Update() override 
		{
			IsClicked();
		}

	private:
		GeometryMesh* m_box, *m_arrow, *m_blue, *m_dropgeo;
		Font* m_font;
		Color m_cback, m_cfont, m_cclick;
		unsigned int m_width;
		bool m_deletefont, m_drop = false;
		std::wstring m_text = L"";
		std::vector<std::wstring> m_elements = { L"" };
	};

	//Sets the function that hanles GizegoEngine created exceptions.
	inline void SetExceptionHandler(void(*function)(std::wstring, ExceptionGravity))
	{
		ExHandler = function;
	}
	
	//Call this in main() before doing anything else. function is the GizegoEngine exception handler. If it is set to NULL this exceptions will be left unhandled. IT SETS LC_CTYPE TO ""
	void InitializeEngine(void(*function)(std::wstring, ExceptionGravity))
	{		
		AudioFramework::Initialize();
		alDistanceModel(AL_LINEAR_DISTANCE);
		setlocale(2, "");

		SetExceptionHandler(function);		
		stbi_flip_vertically_on_write(1);

		if (!glfwInit())
		{
			ThrowException(L"Error initializing GLFW");
			return;
		}
		Input::KeyBind::Initialize();	
	}

	//Call this in the end of main()
	void CloseEngine()
	{
		AudioFramework::Finalize();
		Input::KeyBind::Clean();
		delete Shaders::ts;
		glfwTerminate();
	}
}
#endif