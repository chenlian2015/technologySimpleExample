[!newguid(InterfaceGUID)]
[!Dialog("Names98", "RBDeskBand")]

[!strcpy(UpperShortName, ShortName)]
[!toupper(UpperShortName)]

[!strcpy(RGSName, ShortName)]
[!AddStringToSymbol(RGSName, RGSName, ".rgs")]

[!AddStringToSymbol(HeaderTemplate, GalleryPath, "DeskBand.h")]
[!AddStringToSymbol(SourceTemplate, GalleryPath, "DeskBand.cpp")]
[!AddStringToSymbol(RGSTemplate, GalleryPath, "DeskBand.rgs")]
[!AddStringToSymbol(RGSToolbarTemplate, GalleryPath, "IEToolbar.rgs")]
[!AddStringToSymbol(IDLInterface, GalleryPath, "dbint.idl")]
[!AddStringToSymbol(IDLCoClass, GalleryPath, "dbco.idl")]
[!AddStringToSymbol(IDLProject, ProjectName, ".idl")]
[!AddSymbolToString(IDR_DESKBAND, "IDR_", UpperShortName)]
[!AddSymbolToString(InsertHeader, "\"", HeaderName)]
[!AddStringToSymbol(InsertHeader, InsertHeader, "\"")]
[!AddStringToSymbol(BandRegTemplate, GalleryPath, "bandreg.src")]

[!DeleteFile(HeaderName)]
[!DeleteFile(CPPName)]
[!DeleteFile(RGSName)]

[!target(HeaderName)]
[!include(HeaderTemplate)]
[!target()]

[!target(CPPName)]
[!include(SourceTemplate)]
[!target()]

[!target(RGSName)]
[!include(RGSTemplate)]
[!target()]

[!AddFileToProject(HeaderName)]
[!AddFileToProject(CPPName)]
[!AddFileToProject(RGSName)]

[!AddToObjectMap(CoClassName, ClassName)]

[!AddRegistryToRC(RGSName, IDR_DESKBAND)]

[!AddInterfaceToIDL(IDLProject, IDLInterface)]

[!AddCoClassToIDL(IDLProject, IDLCoClass)]

[!AddIncludeFile(ProjectNameCPP,  InsertHeader)]

[!if=(RBIETOOLBAR,"1")]
[!AddToMemberFunction(NULLSTR, "STDAPI DllRegisterServer(void)", BandRegTemplate, InsertInDllRegisterServer, BEFORESEARCH, "return _Module.RegisterServer(TRUE);")]
[!AddToMemberFunction(NULLSTR, "STDAPI DllUnregisterServer(void)", BandRegTemplate, InsertInDllUnRegisterServer, BEFORESEARCH, "return _Module.UnregisterServer(TRUE);")]
[!endif]
