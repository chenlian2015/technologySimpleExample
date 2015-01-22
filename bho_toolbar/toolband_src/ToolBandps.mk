
ToolBandps.dll: dlldata.obj ToolBand_p.obj ToolBand_i.obj
	link /dll /out:ToolBandps.dll /def:ToolBandps.def /entry:DllMain dlldata.obj ToolBand_p.obj ToolBand_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ToolBandps.dll
	@del ToolBandps.lib
	@del ToolBandps.exp
	@del dlldata.obj
	@del ToolBand_p.obj
	@del ToolBand_i.obj
