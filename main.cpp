#include "main.h"


void APIENTRY DllMain (HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved) {
    if(fdwReason == DLL_PROCESS_ATTACH) {
        //we need this to check if current dll function has been loaded
        std::map<std::string,HINSTANCE> cachedDlls;

        //load dll from config file
        std::ifstream file (".\\dll\\dll.list");
        std::string line,dllName,funcName;

        if(file.is_open ()) {
            while(getline (file,line)) {
                //if contains "->", call specific function
                size_t pos = line.find ("->");
                dllName = line;

                if(pos != std::string::npos) {
                    dllName = line.substr (0,pos);
                    funcName = line.substr (pos + 2);
                    // MessageBoxA (nullptr,dllName.c_str (),"test",MB_OK | MB_ICONINFORMATION);
                    // MessageBoxA (nullptr,funcName.c_str (),"test",MB_OK | MB_ICONINFORMATION);

                    //if dll has been loaded, call function only
                    if(cachedDlls.count (dllName) > 0) {
                        // MessageBoxA (nullptr,"call function only","test",MB_OK | MB_ICONINFORMATION);
                        callFunction(cachedDlls[dllName],funcName);
                    }
                    else {
                        // MessageBoxA (nullptr,"load dll and call function","test",MB_OK | MB_ICONINFORMATION);
                        cachedDlls.insert(std::pair<std::string,HINSTANCE>(dllName,loadDll(dllName)));
                        callFunction(cachedDlls[dllName],funcName);
                    }
                }
                //else we simply load dll to ram
                else {
                    cachedDlls.insert(std::pair<std::string,HINSTANCE>(dllName,loadDll(dllName)));
                }

                line.clear ();
            }
            file.close ();
        }
    }
}

HINSTANCE loadDll (std::string dllName) {
    HINSTANCE singleDLL;
    //if only dll name is given, append default path to it
    size_t pos = dllName.find ("\\");
    if(pos != std::string::npos){
        singleDLL = LoadLibrary (dllName.c_str ());
    }
    else{
        dllName = ".\\dll\\" + dllName;
        singleDLL = LoadLibrary (dllName.c_str ());
    }

    // dllName="try load dll: "+dllName;
    // MessageBoxA(nullptr, dllName.c_str(), "test", MB_OK | MB_ICONINFORMATION);

    return singleDLL;
}

void callFunction(HINSTANCE dll,std::string funcName){
    // MessageBoxA(nullptr, ("try call function: "+funcName).c_str(), "test", MB_OK | MB_ICONINFORMATION);
    void(*func)()=(void (*)())GetProcAddress(dll,funcName.c_str ());
    func();
}

int LowLevelKeyboardProc () {
    return 0;
}

void SetHookID () {
    return;
}
/*
wrong solution

dll.def

EXPORTS
   ?LowLevelKeyboardProc@@YGJHIJ@Z=W2kDTS.?LowLevelKeyboardProc@@YGJHIJ@Z
   ?SetHookID@@YAXPAUHHOOK__@@@Z=W2kDTS.?SetHookID@@YAXPAUHHOOK__@@@Z

*/
