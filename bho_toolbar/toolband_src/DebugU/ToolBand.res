        ��  ��                  �      �� ��     0 	        �4   V S _ V E R S I O N _ I N F O     ���               ?                        �   S t r i n g F i l e I n f o   �   0 4 0 9 0 4 B 0   H   F i l e D e s c r i p t i o n     T o o l B a n d   M o d u l e   6   F i l e V e r s i o n     1 ,   0 ,   0 ,   1     2 	  I n t e r n a l N a m e   T o o l B a n d     B   L e g a l C o p y r i g h t   C o p y r i g h t   2 0 0 1     B   O r i g i n a l F i l e n a m e   T o o l B a n d . D L L     @   P r o d u c t N a m e     T o o l B a n d   M o d u l e   :   P r o d u c t V e r s i o n   1 ,   0 ,   0 ,   1     D    V a r F i l e I n f o     $    T r a n s l a t i o n     	�;  0   R E G I S T R Y   ��e       0 	        HKCR
{
	ToolBand.ToolBandObj.1 = s 'ToolBand Sample'
	{
		CLSID = s '{0E1230F8-EA50-42A9-983C-D22ABC2EED3B}'
	}
	ToolBand.ToolBandObj = s 'ToolBand Sample'
	{
		CLSID = s '{0E1230F8-EA50-42A9-983C-D22ABC2EED3B}'
		CurVer = s 'ToolBand.ToolBandObj.1'
	}
	NoRemove CLSID
	{
		ForceRemove {0E1230F8-EA50-42A9-983C-D22ABC2EED3B} = s 'ToolBand Sample'
		{
			ProgID = s 'ToolBand.ToolBandObj.1'
			VersionIndependentProgID = s 'ToolBand.ToolBandObj'
			ForceRemove 'Programmable'
			InprocServer32 = s '%MODULE%'
			{
				val ThreadingModel = s 'Apartment'
			}
			'TypeLib' = s '{5297E905-1DFB-4A9C-9871-A4F95FD58945}'
		}
	}
}

HKCU
{
    NoRemove Software
    {
        NoRemove Microsoft
        {
            NoRemove 'Internet Explorer'
            {
                NoRemove MenuExt
                {
                    ForceRemove '&Sample Toolband Serach' = s 'res://%MODULE%/MENUSEARCH.HTM'
                    {
                        val Contexts = b '10'
                    }
                }
            }
        }
    }
}
        ��� ���     0 	            *     X      �� ���     0 	        (                �                       �  �   �� �   � � ��  ��� ���   �  �   �� �   � � ��  ��� wwwwwwwwwwwwwwwww������wwwwwwwwww�    wwwwwwwwww�    wwwwwwwwww�    wwwwwwwwww�    wwwwwwwwww�    wwwwwwwwww�    wwwwwwwwww�    wwwwwwwwww�    wwwwwwwwww�    wwwwwwwwww�    wwwwwwwwww�    wwwwwwwwww������wwwwwwwwwwwwwwwwwwwwwwwww|       �� ���     0	            � M e n u P o p u p     	�& C o n f i g u r a t i o n s . . .     �O p t i o n & 2 . . .   � �O p t i o n & 3 . . .   4       �� ���     0	            � M o r e     
�& L i n k   1   � �L i n k 2   �      �� ���     0	        � Ȁ         *�     C o n f i g u r a t i o n s    M S   S a n s   S e r i f     	 P      �  � ��� N o   T e x t     	  P     , �  � ��� T e x t   o n   t h e   R i g h t     	  P     < �  � ��� T e x t   U n d e r        P    	  N ����� T o o l B a r   B u t t o n   T y p e      P    H t 2   ��� O K         P    | t 2   ��� C a n c e l         P    � t 2   ��� A p p l y       <   �� M E N U S E A R C H . H T M         0 	        <script language="JavaScript">
var parentwin = external.menuArguments;
var rng = parentwin.document.selection.createRange();
var str = new String(rng.text);
var re = new RegExp(" ","g");

    str = str.replace ( re, "+" );
    parentwin.navigate("http://"+ str);
</script>
  �  ,   T Y P E L I B   ��     0 	        MSFT      	      A                            "       ����       �             d   L  �   ����   d     ����   p     ����   T     ����     �   ����   �  �   ����   �     ����   �  H   ����   �  H   ����   ����    ����   ����    ����     T   ����   p  $   ����   ����    ����   ����    ����   %"  �                                     `                        ����                  ����4" �                                     �   @  0       0           ����              �������������   ���������������������   ������������������������������������x   ��������������������H      ��������������������0   �R��J�q��_ՉE��������e�w�|Q���  �w<���������c�w�|Q���  �w<�����    d�w�|Q���  �w<����������0P�B�<�*�.�;    ����0     �      F   ����      �      F   �����-��}�J��C;M��d   `   d      ��������      �   x          - stdole2.tlbWWW������������������������������������������������������������   ����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������0   ����������������    �������� �TOOLBANDLibW    ����8��ToolBandObjWd   ����8z�IToolBandObj ToolBand 1.0 Type LibraryW ToolBandObj ClassW IToolBandObj Interface >   Created by MIDL version 8.00.0595 at Tue Jan 27 11:32:35 2015
 ��TWW S WW       ����0   D       H   L      B       �� ���    0	         B l a c k  R e d  G r e e n            B l u e                 �       �� ��    0	               C o n f i g u r a t i o n s 
 C o n f i g u r a t i o n s  O p t i o n 2 
 O p t i o n 2  O p t i o n 3 
 O p t i o n 3        C o n f i g u r a t i o n s 
 C o n f i g u r a t i o n s  L i n k 1 
 L i n k 1  L i n k 2 
 L i n k 2         0       �� ��     0	                 T o o l B a n d                       0       �� ��     0	           E d i t   B o x                             