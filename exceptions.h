#include <windows.h>
#include <iostream>

#ifndef EXCEPTIONS
#define EXCEPTIONS

namespace objects {
	class Exception {
	public:
		Exception() {
			message = (char*)"";
		}
		Exception(char* str) {
			message = str;
		}
		void work() {
			LPCTSTR Caption = "Error";
			MessageBox(NULL,
				message,
				Caption,
				MB_ICONERROR);
			std::cout << '\a';
		};
	protected:
		char* message;
	};
}
#endif
