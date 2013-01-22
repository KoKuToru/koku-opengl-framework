#include "window.h"
#include <cmath>

/*
	Simple test programm

	compile with: g++ *.cpp -lSDL
*/

class test_window: public koku::opengl::windowCallback
{
	private:
		koku::opengl::window my_window;
		koku::opengl::buffer my_buffer;
		koku::opengl::shader my_shader;
		koku::opengl::shader_uniform my_camera_pos;
		koku::opengl::compute my_compute;
		koku::opengl::texture my_texture;
		koku::opengl::shader_uniform my_texture_pos;
		koku::opengl::shader_uniform my_eye_shift;
		koku::opengl::rendertarget my_target;
		koku::opengl::texture my_target_texture;

		bool run;

	protected:
		void onQuit()
		{
			//stop everything !
			run = false;
		}

	public:
		test_window():
			my_window(this, "test", 1024, 768, true),
			my_buffer(&my_window),
			my_shader(&my_window),
			my_camera_pos("camera_pos"),
			my_compute(&my_window),
			my_texture(&my_window),
			my_texture_pos("texture"),
			my_eye_shift("eye_shift"),
			my_target(&my_window),
			my_target_texture(&my_window),
			run(true)
		{
			const float vertex[] =
			{
				1*0.8,1*0.8,-1, //right top
				-1*0.8,1*0.8,-1, //left top
				-1*0.8,-1*0.8,-1, //left bottom
				1*0.8, -1*0.8, -1, //right bottom

				1*0.8,1*0.8,1, //right top
				-1*0.8,1*0.8,1, //left top
				-1*0.8,-1*0.8,1, //left bottom
				1*0.8, -1*0.8, 1 //right bottom
			};

			const float uv[] =
			{
				0,0,
				1,0,
				1,1,
				0,1,

				0,0,
				1,0,
				1,1,
				0,1
			};

			const float camera_matrix[] =
			{
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1
			};

			const unsigned short index[] =
			{
				0,1,3,2,0+4,1+4,3+4,2+4
			};

			/*  GIMP header image file format (RGB): /home/luca/Dokumente/Unbenannt.h  */

			static unsigned int width = 64;
			static unsigned int height = 64;

			/*  Call this macro repeatedly.  After each use, the pixel data can be extracted  */

			#define HEADER_PIXEL(data,pixel) {\
			pixel[0] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4)); \
			pixel[1] = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2)); \
			pixel[2] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33))); \
			data += 4; \
			}
			static char header_data[] =
				"`P\\``````P`^_0\\]^0X]`0``X.D6O\\+N[.G]X^#RZ>;V^/4#_?@$_/<!_O<$_?8#"
				"^_H%V=?NQ\\;GR<KIS]#CY.+L_/;_`?H%`OL(_/8%].W]Z^?WY>+TXN#TXN#TX>+W"
				"X^'UY.+VX=_SX-[RX^'UY.+XY.+XYN3Z[>K^XM_SW]SN[.G[Z^C\\T='HQ<;AT-3R"
				"]?0'^OD&^_D\"^?;`Z^?VY^3VX>+UY>CZR,CASM/^`P``^PXV_``X`````0L````\\"
				"_PTZ````^`@Z``\\``P```P`[P\\SLQ\\SCY^7YY.'UZ>;V]O,!_/D%_/<#^O4!^O+`"
				"_/L&V]GOR<;DWMOW^OH(_/L$]O/]^_4$\\.GZZ^?WZ.3TY.'SX^#RX^'UY.+VYN3Z"
				"Y^;WX^+SYN3XZ.;ZX^'UX-[RY.+VY>/YY^7YY.'UX^#RZN?Y\\N`#ZNC^UM;OR,GD"
				"U]7LW]WQ\\_'_\\N[]Y^/SY.'UY.+XX>+UYN?ZL+/;^P8[_0T^`P``_0@^``\\````\\"
				"`0```P\\_``T\\XNH7OL7PM[W>OL/8Z.KXY>/YY.+VZ.7W\\.W]^_@&_?H&_/<#_/<!"
				"Z.;TV-7IY^'Z_/@.^?8$]O/]]?0!Z>;XY.'SY.'SY.'SY.'UX^'UY.+VY.+VY>/W"
				"WMWNY>3UY>3UXN'RX^'UY>/WY.+VY>/WXN#TYN3XY^;WY>+TZ.7W[>O_YN3ZT]/J"
				"Q<3BS,GC]?,$]_0#ZN7ZX][UY.#WY.+V[.KXQ<CJTMH.``\\```\\``PX`_PP``@`]"
				"_`\\`Y?$?M+C<O;[9U=?MXN7YZ>K_X=_UXN#VY>+VX^#RY^3T]?'`_/D%_/D%`/L%"
				"Y^7S\\.O^^_4'`/@)^_8\"\\>WZY^;UX>+WY>/YY>/YY>/WY>/WY>/WY.+VX^'UX^'U"
				"YN7VY^;WY>3UX^+SY>3UY^;WY>3UX=_SXN/VXN/VYN7VX^+SW=SMY.'SZN?[Z.7Y"
				"U-/SR<;@\\.X!`/T,]_$#[>C]Y>+VYN/UZ.;TUM;QN;[GP\\OYX>H7```^`P``_```"
				"UN0/L;G>WN#XY^;W[^[]X=_SW]ORZ^7^YN/WZ.7WY.'SY>'Q\\.S[^/4#^?8\"_OD#"
				"^?8$_/D'__@'_/4$]>_\\Z.3SX=_SXN3ZX>+WXN/XXN/XY.+XY.+VY.+VY.+VY.+V"
				"Z>?[WMSPXN#TZ.;ZXN#TX>#QY>3UX^+SY.7ZW^#SX>+UY^;WY./TY>+TZ.7WZ.7W"
				"YN<!T,[DY^3T_?H&_/8%^_@'Y^3TX^'OZ./VZNC^W.#ZT-;QN\\+?N,'A[/@C`@X`"
				"JK3,X.7ZY^7[YN+YW=GQZ>7\\Y^+U[^CX\\^__\\^__\\>W]\\>W]]?'`^_4$^_8\"^_7`"
				"^_@&^_4$_O@']>[^X-SLW]SNYN3XX>+WX^'WX^'WX^'UX^'UX^'UY.+VY.+VY>/W"
				"X-[TZ>?[YN3XX-[RY>/WY.+VX=_SZ.;ZY.+VXN#TXN/VY>;YYN7VY>3UY>+TX^#R"
				"Z^G`Y.'S\\>WZ_?@\"^/+_^_@&Z.7UX^+PW]OSYN7VY.7VXN7WYN?XU-GML;?:Z_8K"
				"W^+VZ>?[Y.'UYN+YX^'XY>/Y]?'`^_7`_OD%_?@$_?<&_O@'_?<&_?<&_OD%^_8\""
				"^O0#__P*[^W[W][OYN3ZZ>?]X^'WY.+VYN/WY>/WY>/WY.+VY.+VX^'UX^'UX^'U"
				"Y>/YZ^G_YN3ZXN#VZ.;\\YN3XX-[RY>/WX^#TY>/WY>/WX>+UX>+UX^'UY.'UY>+T"
				"YN+Y\\.W]_/D%_OD#^?,\"\\>S_Y.'SYN7VZN;^X>#OY./TY.#W[>K^ZNGXS]/MK;'@"
				"Z^<%X]WVY>'QY^7QX.+OZ>OY^/8%`/H*_??`^O3__/<#_O@'^O0#_/4$_O<$^_0!"
				"`/H)\\.S[YN7TY.7XXN3ZX>'XX^'WYN/UY>+TY>+TY>+TY>+TY.+VY.+VX^'UX^'U"
				"YN3[O+K1S,KA[>P!X-[TW=OQZNC^XN#VY>+VY^3XXN#TX.'TY.7XY>/WXN#TYN/W"
				"X=WT[>GY^?/`^O4!`/H*[.?\\W]WSXN/XY>/YX^+QZ.;ZY-W]Y-OWX]_N[.X\"N[_E"
				"L;O5Y><#Z./ZY-[IYN3R\\.\\$^/8%__K__?@\"_/<!^_7`^_7`_/<#_/<#_/<#^_8\""
				"]O0#Z^CZXN#TXN/XX^3YX^'WX^#TYN/WWM[UZ^G_XM_OZ.;\\X>#`Y.;TXN+IZ.3^"
				"V]WSEY_/J+#8[>C]ZN#YX>+WV^'NY^3XYN3[W]WTY.+XY^7YY>/WX^'UX^'UXN#V"
				"X^+QY^3T^?8$_/D%\\N[]Z>;VX=_SY.7ZY>#UY.7XWN7VXNCYX^'UXM[U[.L!OL/:"
				"M;WEV-WTYN/SYN'VX^#T\\._]__T&^_0#^O3__/<!_?@\"_?@$_?@$_OD%_OD%_O@'"
				"\\>[^Z>;XXN#TX^'UY>/YY.+VY.'UYN/WZNL!W=SJY.#WZ^D$YN7TWN#NY.+Y[>K\\"
				"P<3DQ]+_M\\#JUM/MY]_UY.7XX>3TY>+VYN3ZX^'WYN3XX-[RX=_SZ^G]XN#TZ.;\\"
				"X^+SY>+R\\^_^]O,!ZN;UYN/UX=_SX^3YY.'SY.7ZX./WX.'TY.'QZ.7WX.#YMKO:"
				"Z?(AK[;4Y^C]Z.;]X=_U\\>_]^?8\"_/0'^_8\"_?@$_OD%_/<#^_8\"^_8\"^_4$^O0#"
				"ZN?YYN/UX^'UX^'UY.+VY.+VY>+VY>+VX^'UZ.;TY.#WZ.3\\X^'MYN7TYN/]Y^7Y"
				"I*O8\\`$LUN(.N[O>Z>;VX.'RY.3[X^+SX-[RYN3XY>/WXN'RX-_PY^7YW]WQYN3Z"
				"Y.+VY>+T[^O[]/#`Y>'QYN/UX^'WX^3YY>3SX>+WXN+[X=[RZN;SX-WMW>']M+SD"
				"`PXZT-@%PL;LYNG[Z.?VW]OR\\>T#_O@'^O<%_/D'^_@&^?8$^_4$_?<&_/8%^?,\""
				"Y>+TY.+VY.+VX^'UY.+VY>/WY>/WY.'UXM_OY^/ZY>3U\\._`Y>'YYN3RZNGWQ\\?J"
				"N,+X`@`[\\_`PJ[#9Y.?WX>#OY^3`XN'RX^+SYN7VX>#QY^;WY^7YX^'UY>/WY.+V"
				"Y.+VY.'UZ^CZ\\^\\!Y.'SZ.7YY>/YX>+WY.;SW]WSZ>7_Z./XX-SIZ^W[NL#AWN<8"
				"_PL[``\\`N<#MQLSEZ.G\\Y^/ZY>#W[NKZ[NO[[NO[[>GY[>GY]/#__/D'_/8%]N__"
				"XN#TY.+VY>/WY>+VY>+VY>/WY.+VX^'UY^3XX^'UX>3TXN+[YN'`Z.;TW^/PIZG8"
				"[OLS`P`\\_`T_ML'PQ\\[BZ^K[Y>'[X>+UY^7YX=_SX^'UY.+VY.+VX^'UZ>?[X^'U"
				"Y>+VX^#TYN/W[>K^Y>+VYN/WY>/YXN#VW>'MZNC^YN#YY-_TY^;UTM?LN</H``X`"
				"``X`_0DW_`TUNL+JT=+QZ^CXZ.3TX=WUY.'SX^#RX=[PX^#P[^O[^O<%]_0\"[^CX"
				"XN#VY.+VYN/WYN/UYN/UYN/UY.+VX^'UY-_\\ZNW]L[G2N[WBZ^G_YN7VLK;:RM8\""
				"`0```P\\]````WNP>JK/3[>O_XMWRY.3[X^'WX^'W[NT\"Y^7[W]WSY.+VX^'UX^+S"
				"Y^3XY.'UX^#TZ.7YZ>;ZY.'UY>/WY.+VWN'QZNC\\X=SQX]_VX^C_K;C7[?DE``\\`"
				"``\\``@`]`P`^Y^`=IZ?,[NO]X=_MY^?^Y>/WY>/WY.'SYN/U\\N[^_?H(^/4#[N?W"
				"Y.+XX^'WY>+VYN/UYN/UY>+TY.+VY.+VY^7[T-7SA(NVO,'?[.[\\Q\\;GL+;K`0`\\"
				"_0XZ`PT^``\\`^`P_K+?DW-CPY>'PX^7]Y>/ZZ.;]U=/JX^'XYN3[YN3ZXN#TY^;W"
				"Z>;XYN/WY.'UY^3X\\N`#X^'UY>3UY>3UY>;[W]SNY>+RY^?^N,'CUN0/`@`_``X`"
				"```Y_`T`[OXULKWIUMKXZ./`XN#VX>7QX^'WY.+VY.'UYN/U\\N___?H)]_0\"Z>7T"
				"XN/XXN#VX^#TYN/UYN/UY>+TY./TX^3WY.?MJ*OAE)O0U>'NT];FK*K:^`,Y_@`^"
				"`0`Y``\\`_`H[_P``Q<`%R,/@[^GTW>'[YN/]X-WWEY2NRL?AY^7\\X^'WY./TY./T"
				"Z./VY>+TY.'SZ.7Y^/@(X^+SY./TX^+SXN+YY>+RY^7SVM_TI[/:`0```P````\\]"
				"`PX```\\[S-/^Q,3E[>GVYN+OY>+\\WM_ZX.'RYN3R[>;W[^?]__D)_??`^/+_Y]WW"
				"YN7TY>/WX-ST[>H#Y.#XX=[RY^3XZ.7YL[[KBI7!E9[(SM@!H*G5[_LG````_`T`"
				"`PX\\``\\`````^`D[[/LJJ;/8Z.D!Y-WM[>GYRM#GI;'BL+KLY.?YY.'KY.#WY.7Z"
				"X^#PZ^?_Y^/[Y.+N]O7\\[.CXX][QXM[MXN7YX^'UX]WZ[>K\\P<+=R,P!``\\```T`"
				"_`T_```\\[ODJN+OGZ>K_YN/UX-[UXN/T\\.\\$W-[TRL[JQ<7FZN;]]?+X^_C`[NL!"
				"W=ORZ>?]Y.'SY.+PW][LZ.G\\W^#]N[O@E*#/H:S9FJ71DIW)X>T8``\\]````````"
				"``\\```X_`````P```P``K[C@SL_JY^+UZ.;ZKK73XN\\?LKWLS=+IZ^GWYN/UY>/Y"
				"ZN?WX]_WX]_WW][L^/D#Z^G_Z>;`Z^G]X./UY^;UY>'YYN;QWN/WM;[J```Y_0HU"
				"`P`^_``[_`T`T-@.O,#>Z.;\\YN3XZ.CQX=WUIJO,R]D$Q](#N[S;``$&^OG^Y^C["
				"X^'WY>+TYN3RZ>CV\\/(\"T]GTJ;'6K[KFFJ?:Z/4ER-4#WNH8```^_`PZ`@X^````"
				"``X_``\\`````_`T_`P``T=X&N[_=\\.\\$W=_WK;3A^`@ZW>D5N+W>Y^7YY^7SX-[U"
				"YN/SXM[VZ.;]Y^CYX>3VS='OR\\WRQ\\SJX>3XY^7YXM[XY.;SS-3HP<[Z_@\\X`0`\\"
				"```^`P`\\`0``[_HMKK3/Z>?[Z.7YW]SFZN+XL[S>^PX`\\/`YQ\\GN_/7^_/G_X^;Z"
				"Z>CVY.'QXN#TWMW[L++7IJW8R-<\"G;#:RM@,_`T_`P``[_PL]P,S`````````0T]"
				"````_PDZ`P\\_`P```0\\^\\OXIIJS/Z.H&M;O<SM8,`P``]@,NJJ_:W-SUY>/OY^/["
				"YN/UZ.3\\X=_VY^C[O,#:P,7NS]D$N<7LN;G4\\.\\%ZN8&V=?MMKO9`@L`_@X`_PX`"
				"``T`_`TX`P\\_N,'KU][QY>3U[NL$[.C_^/($MK_?W?(EZODUO+OC``$,]>[WX^'X"
				"X-WMZ.7WY^?`K:_4SM,!]P$VL,#TO]'`````_@H\\````PL\\!F*37^P<W````````"
				"`@X``P\\_``\\`````^PDX`@X\\NL+JU]O_GJ?/G*7?]0(V`P`[O,3TQ,?GY^;UY^/Z"
				"Z.7W[.C_W-KPZ^[`O\\3BX>H9``\\`TN$,S,WBX^#TY-W]Y.+VMKK8^P(Y``\\``P\\`"
				"``L````\\_`LWL+C=Y.K[W^#UP\\'EQ\\?LP\\/<LKO=^`P]]/`VP\\+J`_\\.]>WYZN3]"
				"YN#]Y.+YY.;\\N+_=\\OXIF:3:@(O\"L;SRH*O8J+/BHZ_@TM\\1J+7HX.T?`````0T["
				"`0X``````@T[_`P\\`P```P``S=D%C)2\\R=4#]/`U_0H^`P``[ODJJ['6Z>P!X-WQ"
				"[>C[\\>T!Y>+TZ>K[OL/BV-`4`@\\`N,CXT-;G\\/(\"X=WWZNGWQLOBU=X/`PX\\`P\\`"
				"``\\`_0HU```^O\\CTTMCSN\\#>V-`,T-P'TMH&^00O````_0<ZO\\/G]_<%\\N[[[.?^"
				"Z>#\\X^'XX^K^G*G(AY2`D9O/H:?8I:K3M;WEMK_IS]L'J[?HW.D;`P```0T]````"
				"`P```P\\_`PX\\`````@\\`]`@Y^P<WOL;TZO<IV>84P\\`\"RM@-XN\\=M\\#HRLWMZ^K["
				"Z>3W[^K_Y^3TY^;UO\\+BV-\\3``\\`X?,?JK74N+_<O;WBW>+WQ<SGS]H+_PPW`P\\]"
				"`0`^^PPU`P``]_\\SL+C=K;?<_@L]_@```0``_PP```\\```\\^NL/E[_8\"^?H\"Z>3W"
				"YN#MYN7TT-CPAI.\\E:+8HZWCQ<GOW=[ST-;[PLKPM+WIV.02`@X^`P``_`P\\`P\\_"
				"`P``_`P\\```^```^`0X``0```P``O,?V=H&X=H.L@HZ_C9G2I;/?DIW)IZO1[^[]"
				"]>\\!^?,%Z^?V[.OYPL/BV^(6``\\``0`\\]`4Z[/<HW^,8N;_BML#E[_LS`P```P``"
				"`@X^````_`P\\````[_PLEJ+3[/DI```````^_@HZ```````^Q<WUX^4!_/H)\\NWS"
				"X>#QY^?RT]KNK[GMN+_JVM?UY>0'U^7N^O4!^O3_\\?'ZIZS+Z?,N`@\\``@\\W``\\`"
				"`P`^``````\\``PXY`P\\_`P\\`_0D[IK+7M+K1YN/]W-CHP\\'EM+OPJKCHA(Z`LKS8"
				"S='M_?P']>T\"[.?ZO<#BV>,:`P`[_PP^`0T]````]`0TC)C)T-T-`@X^`P\\_`P\\_"
				"````_PL[`P\\_`0T]````^@8V`````P\\_```^_`P\\````````W^D2RL_N``<<Y>'P"
				"X^#^Z.3[W.'UFZ3,M+O8__T0K*;-I:W%]O#][N?W_/L$U-;NMK[P``\\`_0HT``\\`"
				"`0\\\\`@X``PT^```\\`P````\\`Q\\_]Q<SJYN8-R\\?I_/L$]O8$X.3`J[77G*;<G:?9"
				"N+G6^/<\"\\^K^\\>W]NKW=Y.\\D`P`\\`@\\``@X^````Z/4EP<W^_`P\\````````````"
				"`P\\_`````````````P\\_`````P\\_`@X^`PX\\````_@H\\````]/`LNK_FT=7SZ.D!"
				"Y>/WX]_VXN+]CI7\"Q<KI__P*Y][\\YNC^]O+_]>\\![NOU^OL%KK'9]`(W````_`P\\"
				"```^`P\\``0T_`P`^`0``_@@[KK'9[.X\"QL'HHJ'\"ZNKQ\\._T\\_']R<OAFZ+9K[_["
				"P\\3A``(._/4$\\^_^M+G7Z_@H`0\\\\`0X``````````0T]````````^`@X````_`P\\"
				"`````````0T]````````^P<W`````````@T[````````_`T_`0T]I*W9HJK0M[W@"
				"Z.OSY>#WY>+^F)O'SM+P]O#Y^.`#[NO]\\>_[\\NX$^?/^__\\\"OL+@Y.P@_PP^`P``"
				"```\\``\\``@\\`_@X]_``_[OHHO+S=_?@$\\_$\"Z.7_\\_+`\\NO\\`?<(Z^;Y@8BUA9O-"
				"TM+K`O\\-^_/_[>OYLKG6\\_`N`P```@T``P\\_`````````````````````@X^````"
				"````_PL[````_`P\\```````````````````^_`P\\_`P^`P``Y_4GHJ[=\\?TII[#<"
				"ZNO^Z>/^X]_OMKO2Y>O\\]O3T^>[`]NX)[^WY\\^\\%_?@\"_OX!P,3BWN8:`0X``P``"
				"`@TY``\\``P``_0\\]`0``WNH8P<#@^O+^]/+V]/(%[NO[]^X&__()].W`FZ/+<HFR"
				"W]WS_O@(_/7\\\\N__N;_@_@PW`P```@X^````_`P\\_`P\\_@HZ````````_PL[````"
				"`P\\_`P\\_````````_PL[````_PL[`P\\_`0L\\````````_`X]X>`?IK+C\\_`PHZ[="
				"W.#MY=_ZZN;SS-'HSM/Q]/+\\^O+^]_#`^?8\"_/8(^?;`^_P&L;3<Z_8K````_PL["
				"`PXZ``\\``P\\`_``]`0``X^\\=N+C;_/D'\\>[X].`\"^O3]_?,#_/4$ZNGXI*O6E:75"
				"[NO[`?L+`?L!\\>`%PLKP`P`\\_`T````^`````P\\_`````````P\\__0DY`````@X^"
				"````````_@HZ`````P\\_`@X^``````````\\`````_PL[`P``\\`(QF:76_PL[KKCJ"
				"U=SGZ.+[\\.GYNKW?CY+(\\?(,^_7`_/K\\_?@$^/(!__X'V-KRN<'S_`L`_PPV``\\`"
				"```\\``\\```X``P`\\`@`_^0,VMKGA[>\\#^_D(^_4'`/7Z^_'[``(/X.;WN<#MO\\?Y"
				"\\N[[^_,$__K`UM7ST]T(`P`^_PL````[_@HZ````````_@HZ````````````````"
				"`P\\_`````````````@X^`````````@X^````_@HZ````_`PZ```^R=8$OLK[ML#R"
				"O</>ZN0%[^GRPLCAHJC;L[/4Y>#S\\.S[]_'^_/<!X>'JIZS+Y_$L`P`````Y`PT`"
				"`P\\]``\\```\\````\\_@PY``\\`N<#KR,SF__\\,__<(`_T\"`_L)^/H,KKG.W.00N\\#K"
				"^?;^^_,$``$)Q<3E[?8G`0`^`0T````X````_@HZ`````````@X^`0T]````````"
				"`````````````````````````````````0T]`````````@X^`P\\__`P\\U>(2DY_0"
				"M+C>Z^K[[NCUS\\SJP<GWX.\\<K[OJL[GNKK?AKK?CL[OKZO4F``\\`_`L```\\``@T`"
				"`@\\`_PP^`P```@\\``P\\_```^^`<UK;CENKW?S]KOV.#NWMSRL+76P-/\\\\0,IN[WB"
				"`_?Z_?H(WN+`Q,WY`P\\_`````0T]`P``````````````````````````````````"
				"`````````````````````````````````````P\\_`0T]````````````````_PL["
				"M[_GW-SUXM[NXM_YIJ[4K;OIL;_O]?TQ```[^00P`@T[``\\``@X^````````````"
				"``X_`````````0L\\``\\````^`@T[_PHXW.(,PLS^OL;TOL#HWN82`0``UN,5O[[F"
				"``$(^_D,M+K=^P8R`0T_`P\\``````P``````````````````````````````````"
				"`````````````````````````````````P\\_````````````_`P\\`0T]^`@X````"
				"SMH%O\\'F\\N\\%Z.?XN\\#>HJ[=S=L/G:G:^0<T`P`^`P```P\\_`````P\\]_PLY\\?\\J"
				"Y?$?[?DGZO8D```^``\\]^`8W``\\`_`H[``\\`_@D`[_PVN,#NML#RY>`NN+OM[NO_"
				"``(5O\\#=X^T4`P\\_`````````P\\_`P`^````````````````````````````````"
				"`````````````````````````````````````````````@X^`@X^`````P\\_````"
				"`P``MKKHS\\[N\\?#_M;K1H:G7J+7KN,CX]@8W`0``^`D[`0X``P``_`P\\```^ZO@C"
				"FZ?8DY_0S=@)``\\``````````P```P``_PH_[_`SDJ/.U.(*KKGFE)C'WM[W`O[["
				"U-3URL_V```^`P```0T_`PT^```^`P`^````````````````````````````````"
				"`````````````````````````````````P\\_`@X^`P\\_````````````_PL[`P\\_"
				"Y?,GE9W/I:G-[?$$U-KSD)C&OLG_]08R`P```P```P````\\`_`P^`0T_````_`P\\"
				"`0``^`D[`P```0T]_@HZ`P``_P\\`_`\\````\\```WV^L4B);$H*C.Z^S_`?\\$\\>SR"
				"OL/L```\\^`D[`P```@X``@T[```^`P``````````````````````````````````"
				"`````````````````````````````````````P\\_`P\\_````_`P\\`P\\_````_`P\\"
				"E*'6<'RO?8:PH:?(T=?ZIZ[;KKCJF:?5J;+>S]D$W.@6]`(S``\\``````0T_`P\\`"
				"_P\\``0```0T]```^```\\^P<U_@PY_@P[Z/8>TMX)L;KTDYW<L;K<`@,%_/L$N[KC"
				"[?8C`````P``_0H\\``\\````^_`PZ`P``````````````````````````````````"
				"`````````````````````````````````@X^`P\\_`````````````````0T]P\\_`"
				"M+_UU.$/T-X)FZ;5BI+\"M+WI```[Q],!HZC)P,3HJJ_6GZ;1FJ+0HZ_>HJ[?I+'D"
				"G*G<I;'BJ[/AL[KEK[3=KK;<J+#8E9[&G:O9J;#ES-81]P,]P,KOZ>K_O;W>X.TE"
				"``\\`_PL]`P```@`_```^``\\\\```^`@\\`````````````````````````````````"
				"`````````````````````````````````````@X^`````@X^`P\\_`````0T]K;GJ"
				"Z?0I_`XY_@\\[^04]J;+LL[SHQ,WUL+CHT=/KRLOFU=CXN+[CE)W%U-`,X>`=WNT<"
				"Y_,ATML(DI>`M;G?R,OMR<SNT-7VJ:[/W.TF`0L^`@TX`0`YT-T%M[7EZ>`@^```"
				"_0<X````_`T_```````\\``PY```^`0``````````````````````````````````"
				"`````````````````````````````````@X^`````````0T]`P\\_````^`@X[/DI"
				"ML#T````_0DY`0\\^````R=4#EI[&RM#SR]'JQ<;CS,WLSM'QKK?7IK3:T-T-Y_,K"
				"T=@*H*?4M+K?TM;TQ\\GAR\\OBSL[GT-#KL+74XN<4``P```\\`_PT\\_0LV`P`^`PX`"
				"`P\\_`````P\\_`````@X^`````````P\\_````````````````````````````````"
				"`````````````````````````````````P\\_`````````P\\_````````````_@HZ"
				"SMD,P\\_`````_0LZ_`P^`0PZVN0-GJ;,S='MT=#PR<7FS<SLR]#NN,'?JK+7HZC1"
				"J[36O\\7FRL_MQ,CDS]#MR<KIR\\ONR\\OPR]/KK;7:Y_(C``\\``P`^`P`\\_`X]``\\`"
				"`````````````````````0T]`0T]````````````````````````````````````"
				"`````````````````````````````````````P\\_`P\\_`````````P\\_````````"
				"``\\`S]P,R-4%`P````````X`]O\\LHZK5R,KBV=GRW=CUS,?DT='LS]#MS<[KUM7S"
				"R<_JR,[IS-#LR<KGR\\SKR,CIS<WPQLGKS-3JMK_AM<#L````_@PY`@`[`0T]`0P`"
				"`P\\_`P\\_`0T]_`P\\`````````0T]````````````````````````````````````"
				"`````````````````````````````````````P\\_`````````````0T]`0T]````"
				"``X`````U^02NL;W_0@]``\\```\\`L;GGR]+FT];JZ.;]T]#JR,7CQ\\;FS,GGR\\7B"
				"R\\KHRLGGS,GES,GEV-7OW=WVR<OASM/HJ+'1HJS1H:S9L;WN_`X[`0\\\\````````"
				"`````````````P\\_`````````P\\_````````````````````````````````````"
				"`````````````````````````````````P\\_````````````````````````````"
				"`````@X^```^VN<7L;SRS-@-T-L.DIW,L+K>MKS=P\\3AU=3RQ<3DUM7S[^X$[.?Z"
				"X=SQY-_VZ^/[Z.#XZN7\\ZNC_UMCNP,7<IZ_?_`H[^04WM<'TF:76[OLI```^_PLY"
				"`0T]_`P\\`````````@X^`P\\_`````@X^````````````````````````````````"
				"`````````````````````````````````````````````@X^`P\\_`````````P\\_"
				"`@X````````^`P\\][_PNP,P!ML+UW^P:`@P`Y^`AM;G?T=7SQ\\CEW-[T^OH'^/7["
				"`/L%_?@\"_/4\"__D)].\\'K*O+L;7;J[#;[?DO]`,Y]`,X`0P`SML-EJ+3T-X)`@\\Z"
				"`````@X^_PL[`````P\\_`0T]````````````````````````````````````````"
				"`````````````````````````````````````````````P\\_`@X^`P\\_`P\\_`@X^"
				"````^P<W```\\```^`P\\_````````_`PZ_PL`UM\\.PL?HT-3NR<KET]/L[.G]^_@$"
				"]O0#]O,!\\>W\\XM_QW-SWP<7KX>H9FJ7;M+_U_PL]\\O\\O]@(R````V^@8?XNZP<[Z"
				"_@HZ`````P\\_`P\\_`````P\\_`````P\\_````````````````````````````````"
				"`````````````````````````````````@X^`P\\_````````````````````````"
				"`P\\``````0XY```^````_@HZ```^`@`[```^M\\#BT]CMQ\\K<S<WHS,OLSLCKVM'Q"
				"T=#PU=+NS,KAQL;=S]/MN,+D\\P$L_0``L+[LF:?3[_TG_0HT^04SYO,C?8FZ=H*Q"
				"Q=(\"`0T]````_`P\\`````0T]`````P\\_````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"`PT^```^```^`0\\\\`P`^`P``_`P\\````]@(TO,3JU=GUR,KBRLWMR,KOS,_OR<KE"
				"R<SVQLCMQ\\CERLSDS]7PLKSA^P@Z_````P`YTM\\-D9S2T]X3``\\][OPGJ[GIR-D,"
				"G*C9[?HJ`````@X^`````````P\\_`P\\_````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"``\\`_`P\\````_`X]`0\\^`P```P``````U=X+OL+FSL[ES,K@S,SGQL7ER<GDSLSB"
				"T-#KSL[EV]KKXN'P\\?,'U-CVY>T7``X__`\\`_`T_[/LFEJ//N<7X_@<^_`H]]/`N"
				"DY_0Y/$A`````0T]`@X^`@X^````````````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"``\\``````P\\_`P```P```0\\^_PL[_PL[P<7KVMGWW]SPW]KMX^#TZN;^[NO_[^O["
				"\\.[X]_3^`/L#_/<!`O\\-Z^?^Q,/AR,OK[/@P`P```P`\\]P4PK[SOE)W5G*/8EYW."
				"S=H*_0DY`````````````````````P\\_````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"``\\`^`@Z`P\\``P``^@@W_PLY```\\V.00L[;8Y>7`^/0(]_,%]/(%]O4(^_D*__P*"
				"_/H#`/X'_OD%^_4$_?<&`O\\.^O@)[.G[NL'?T=H!`0L\\`0X``P``Y_8C[ODJ_00Z"
				"`0T]`````@X^`@X^````````````````````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"`````````P\\``P\\_```^\\OXIL[SDKK;>R\\[NR\\SIR\\CBS<OBU=;KU]KNU]CMVMCL"
				"V]GOVMCOW-CPY>'YX=WTY>+T^_H'_/L&``,)T='HQ,?Q_00X`0X`^PPX_``]_``]"
				"`````@X^_PL[`````````0T]`P\\_````````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"_`P^````````_`LYX^T8J[/9PL;JS]+RR<[LQ<;ET=#NSLWKR\\_IQ<ODQ\\CCRLKE"
				"S<SLR,?HR,CIQ\\CGRLKER\\OBV]KK[^[]]_'Z__X)Z>K_NK[D\\OHN`P``_P`__@`W"
				"`P\\_`````````````````````````@X^````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"`P``^`@X```^Y>\\:I:W5T=;WS,WJS,SERL[HR<KISLWMQ\\CGQLKFRM#KS='MRLGI"
				"R,GHS<[MS,_ORL[LS='OSL_JQ,3=RLKAU]7KYN7V]O8$[.T%PL3K]/LK````^`TY"
				"`@X^`P\\_`````0T]`P\\_````````````````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"_PT\\````^@4SJ;+>S=/XQL?FT,WGSLO?S,[FSLWKSLKLSLWNR<WKQ\\OGS,WLSLWN"
				"R\\ODS]#KQ<GCRL[JQLKFR\\SKS,OKS\\[NR<WIQ<;CT,WGY.+XW^#SP<;D^0$R``T`"
				"`P\\_`````````````````````@X^````````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"_````0\\^R]+]PL3IR\\SIS,SER\\WEW^'YV]CRS\\SHT,WIR<GDR\\OFR<OCSM#FR,WB"
				"Q\\SCSM#HR\\WER<OCS<WHRLGGS\\[NR<CIRLCNTL_KS<OBRL?AU-;LRM+HN<7L`PX`"
				"`P\\_`P\\_````````````````````````````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"_0H\\```^NL'LQ\\OOS\\[LRLC?V-7I\\.W_V-COQL;?S,OIS,SMSL[QSL[OQ\\CES,WH"
				"S]#MRLOHR\\SKSL_NRLOJS<[MSL_LR\\SIQ\\OER\\WCU-'MS\\OOR<CIT=7OJK/1X.D6"
				"`P\\_`P\\_````````````````````````````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"_`LY``\\\\TMH$L+;;TM/RY.+X\\NO\\^O+`Q\\OERL[HR\\_KR\\SKQL?FR\\OLS<WPS,KN"
				"W]_ZS<SLQ\\;GR<GLS\\_RRLOJPL;@SM/HR]#GR\\[BWM[WSLKLS,CJR\\WER,SHR<ST"
				"`P\\_`P\\_````````````````````````````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"```^```^]`,OP,SUM[W@Z.H$`OX0XMKHQ<CJR\\[NQ<GCS<_ET-+HS,SEQ,#AV-+U"
				"^?4'U-#GT,WKT,_PQ<7HR<KITM3LR<S@R<CIUM3JZ^K[V-;MS,SET-7LPL;JPL7Y"
				"`P\\_`P\\_````````````````````````````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"`0T]`0X``0```P``]`4MO,+EYN<!W]OSR<WKS,_ORLWMP<7CR<KGU]7LS\\S>[^O["
				"^O7[W=GHR<7<TM'OU-/TTM+SS,WJS<WFS,?HT,WA_OX.UM?RP,7FM\\#BHZS6\\?LR"
				"`P\\_`P\\_````````````````````````````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"`P```P```0``_P\\``P``^P8QO\\?MM;G=S-+IN+W>IJK8HJ79I*;6TM+S`0,0___`"
				"``,._?T+X^/\\O+_?GZ/'LK;<Q,;KS=#RU=+NV-COSM+NP,3RR],'S-D'^@DR_PTZ"
				"`P\\_`P\\_````````````````````````````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"`````0T]`P```P``_PT\\```````^X^\\=E9_#J[?@W>@;]`$XU^(7L;CEN\\#?Q\\SA"
				"Q,CFPL?HO<7JFZ3.S-@#OL?SG*/0J*_:K[3=K[C8U=`$``\\```X``P`````\\_`TX"
				"`P\\_`P\\_````````````````````````````````````````````````````````"
				"````````````````````````````````````````````````````````````````"
				"```^```^```^``\\]``\\`````_0H^`P``IK+KS=L0`0``]04T_P`\\S]P,EZ'7\\/@O"
				"Z?(A\\OTN\\`(Q@Y/%Z_PM_```]P,S\\/TMEJ'@RMD&^`XT`0```0X`_@H\\``\\]``X`"
				"`P\\_`P\\_````````````````````````````````````````````````````````"
				"";

			char* texture = new char[width*height*3];
			char* header_ptr = header_data;
			for(int i = width*height*3-3; i >=0; i-=3)
			{
				HEADER_PIXEL(header_ptr, (texture+i));
			}

			my_buffer.upload(false,        vertex, 8*3, 3);
			my_buffer.upload(false,            uv, 8*2, 2);
			my_buffer.upload(false, camera_matrix, 4*4, 1); //gets calculated by compute_shader ;)
			my_buffer.upload(true,          index, 8*1, 1);
			my_texture.upload((unsigned char*)texture, width, height, 3);
			my_target_texture.upload((unsigned char*)0, 128, 128, 4); //empty texture

			delete[] texture;

			my_shader.uploadVertex("#version 400\n"
								   "layout(location = 0) in vec3 Position;\n"
								   "layout(location = 1) in vec2 UV;\n"
								   "out vec3 Position_tc;\n"
								   "out vec2 UV_tc;\n"
								   "void main()\n"
								   "{\n"
								   "	Position_tc = Position;\n" //pass position to tesslelation control shader
								   "	UV_tc = UV;\n"
								   "}\n");

			my_shader.uploadTessellationControl("#version 400\n"
												"layout(vertices = 4) out;"
												"in vec3 Position_tc[];\n"
												"in vec2 UV_tc[];\n"
												"out vec3 Position_te[];\n"
												"out vec2 UV_te[];\n"
												"#define ID gl_InvocationID\n"
												"void main()\n"
												"{\n"
												"	Position_te[ID] = Position_tc[ID];\n"
												"	UV_te[ID] = UV_tc[ID];\n"
												"	if (ID != 0)\n"
												"	{\n"
												"		gl_TessLevelInner[0] = 16;\n" //tes-level
												//"		gl_TessLevelInner[0] = int(int(gl_TessLevelInner[0])/2)*2;\n" //only 1,2,4,6, ...
												"		gl_TessLevelInner[1] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[0] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[1] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[2] = gl_TessLevelInner[0];\n"
												"		gl_TessLevelOuter[3] = gl_TessLevelInner[0];\n"
												"	}\n"
												"}\n");

			my_shader.uploadTessellationEval("#version 400\n"
											 "layout(quads, equal_spacing) in;\n"
											 "in vec3 Position_te[];\n"
											 "in vec2 UV_te[];\n"
											 "out vec2 UV_geo;\n"
											 "out vec3 Position_geo;\n"
											 "void main()\n"
											 "{\n"
											 "	float u = gl_TessCoord.x, v = gl_TessCoord.y;\n"
											 "	vec3 a = mix(Position_te[0], Position_te[1], u);\n"
											 "	vec3 b = mix(Position_te[2], Position_te[3], u);\n"
											 "	Position_geo = mix(a, b, v);\n" //pass to geometry shader
											 "	vec2 a2 = mix(UV_te[0], UV_te[1], u);\n"
											 "	vec2 b2 = mix(UV_te[2], UV_te[3], u);\n"
											 "	UV_geo = mix(a2, b2, v);\n"
											 "}\n");

			my_shader.uploadGeometry("#version 430\n"
									 //"layout(quads) in;\n"
									 "layout(triangles) in;\n" //can't use quads
									 "layout(triangle_strip, max_vertices=24) out;\n"
									 "uniform sampler2D texture;\n" //use same from fragment ?
									 "uniform float eye_shift;\n"
									 "in vec3 Position_geo[];\n"
									 "in vec2 UV_geo[];\n"
									 "out vec4 Vertex;\n"
									 "out vec2 UV_fr;\n"
									 "out vec3 Distance;\n"
									 "layout (binding=2) uniform result { mat4 CameraMatrix[]; }; \n" //gets data from compute shader
									 "void main()\n"
									 "{\n"
									 /* PROJECTION */
									 "	float h = 1.0/tan(45*0.0087266);\n" //FOV
									 "	float zNear = 0.1;\n"
									 "	float zFar  = 1000.0; \n"
									 "	float frustrumLength = zFar - zNear;\n"
									 "	mat4 ProjectionMatrix = mat4(vec4(h, 0, 0, 0),\n"
									 "								 vec4(0, h, 0, 0),\n"
									 "								 vec4(0, 0, -(zFar + zNear)/frustrumLength, -1),\n"
									 "								 vec4(0, 0, -2.0*(zNear * zFar)/frustrumLength, 0));\n"
									 /* PROJECTION END */
									 "	for(int i = 0; i < 3; i++)\n"
									 "	{\n"
									 "		Vertex = vec4(Position_geo[i], 1);\n"
									 "		gl_Position = ProjectionMatrix * (CameraMatrix[0] * Vertex + vec4(eye_shift, 0, 0, 1));\n" //again doesn't matter which ModelViewMatrix_geo (should be all the same)
									 "		UV_fr = UV_geo[i];\n"
									 "		if (i == 0) Distance = vec3(1,0,0);\n"
									 "		else if (i == 1) Distance = vec3(0,1,0);\n"
									 "		else Distance = vec3(0,0,1);\n"
									 "		EmitVertex();\n"
									 "	}\n"
									 "	EndPrimitive();\n"
									 //Dup it
									 "	for(int i = 0; i < 3; i++)\n"
									 "	{\n"
									 "		vec4 dv = textureLod(texture, UV_geo[i], 0);\n"
									 "		float df = dv.w;\n"
									 "		Vertex = vec4(Position_geo[i] + vec3(0,0,-0.5) - vec3(0,0,1)*df, 1);\n"
									 "		gl_Position = ProjectionMatrix * (CameraMatrix[0] * Vertex + vec4(eye_shift, 0, 0, 1));\n" //again doesn't matter which ModelViewMatrix_geo (should be all the same)
									 "		UV_fr = UV_geo[i];\n"
									 "		if (i == 0) Distance = vec3(1,0,0);\n"
									 "		else if (i == 1) Distance = vec3(0,1,0);\n"
									 "		else Distance = vec3(0,0,1);\n"
									 "		EmitVertex();\n"
									 "	}\n"
									 "	EndPrimitive();\n"
									 "}\n");

			my_shader.uploadFragment("#version 400\n"
									 "uniform sampler2D texture;\n"
									 "uniform float eye_shift;\n"
									 "layout(location = 0) out vec4 FragColor;\n"
									 "in vec4 Vertex;\n"
									 "in vec2 UV_fr;\n"
									 "in vec3 Distance;\n"
									 /* CUSTOM TEXTURE FITLER */
									 "vec4 texture2D_Filter(sampler2D texture_sampler, vec2 texture_uv, float width)\n"
									 "{\n"
									 "	ivec2 texture_size = textureSize(texture_sampler, int(textureQueryLod(texture_sampler, texture_uv).x));\n"
									 "	vec2 texel_size = vec2(1.0, 1.0)/texture_size;\n"
									 "	vec2 texture_fract = (2.0*fract(texture_uv * texture_size)) - vec2(1.0, 1.0);\n" // -1 .. 1

									 "float bw = 0.1*width;\n"
									 "vec4 c1 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2( 0, 0));\n"
									 "vec4 c2 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2(-1, 0));\n"
									 "vec4 c3 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2( 1, 0));\n"
									 "vec4 c4 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2( 0,-1));\n"
									 "vec4 c5 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2( 0, 1));\n"
									 "vec4 c6 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2(-1, 1));\n"
									 "vec4 c7 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2( 1, 1));\n"
									 "vec4 c8 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2(-1,-1));\n"
									 "vec4 c9 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2( 1,-1));\n"

									 "vec4 rc1 = (c1+c2+c3+c4+c5+c6+c7+c8+c9)/9;\n"

									 "bw = 0.25*width;\n"
									 "c6 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2(-1, 1));\n"
									 "c7 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2( 1, 1));\n"
									 "c8 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2(-1,-1));\n"
									 "c9 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2( 1,-1));\n"

									 "vec4 rc2 = (c6+c7+c8+c9)/4;\n"

									 "bw = 0.15*width;\n"
									 "c2 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2(-1, 0));\n"
									 "c3 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2( 1, 0));\n"
									 "c4 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2( 0,-1));\n"
									 "c5 = texture2D(texture_sampler, texture_uv + bw*texel_size*vec2( 0, 1));\n"

									 "vec4 rc3 = (c2+c3+c4+c5)/4;\n"

									 "vec4 mix_color = min(rc1, (rc2 + rc3)/2);"

									 //Search best color ;)
									 "bw = 0.5*width;\n"
									 "c1 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2( 0, 0))*texture_size), 0);\n"
									 "c2 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2(-1, 0))*texture_size), 0);\n"
									 "c3 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2( 1, 0))*texture_size), 0);\n"
									 "c4 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2( 0,-1))*texture_size), 0);\n"
									 "c5 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2( 0, 1))*texture_size), 0);\n"
									 "c6 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2(-1, 1))*texture_size), 0);\n"
									 "c7 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2( 1, 1))*texture_size), 0);\n"
									 "c8 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2(-1,-1))*texture_size), 0);\n"
									 "c9 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2( 1,-1))*texture_size), 0);\n"

									 "vec4 best_color = c1;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c2))) best_color = c2;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c3))) best_color = c3;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c4))) best_color = c4;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c5))) best_color = c5;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c6))) best_color = c6;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c7))) best_color = c7;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c8))) best_color = c8;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c9))) best_color = c9;\n"

									 /*"bw = 0.8;\n"
									 "c1 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2( 0, 0))*texture_size), 0);\n"
									 "c2 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2(-1, 0))*texture_size), 0);\n"
									 "c3 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2( 1, 0))*texture_size), 0);\n"
									 "c4 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2( 0,-1))*texture_size), 0);\n"
									 "c5 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2( 0, 1))*texture_size), 0);\n"
									 "c6 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2(-1, 1))*texture_size), 0);\n"
									 "c7 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2( 1, 1))*texture_size), 0);\n"
									 "c8 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2(-1,-1))*texture_size), 0);\n"
									 "c9 = texelFetch(texture_sampler, ivec2((texture_uv + bw*texel_size*vec2( 1,-1))*texture_size), 0);\n"

									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c1))) best_color = c1;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c2))) best_color = c2;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c3))) best_color = c3;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c4))) best_color = c4;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c5))) best_color = c5;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c6))) best_color = c6;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c7))) best_color = c7;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c8))) best_color = c8;\n"
									 "if (abs(length(mix_color-best_color)) > abs(length(mix_color-c9))) best_color = c9;\n"*/

									 "return best_color;//(mix_color+best_color)/2;\n"

									 "}\n"
									 /* END FILTER */
									 "void main()\n"
									 "{\n"
									 "	if (eye_shift != 0)\n"
									 "	{\n"
									 "		if (eye_shift > 0)\n"
									 "		{\n"
									 "			if (int(mod(gl_FragCoord.y, 2)) == 0) discard;\n"
									 "		}\n"
									 "		else\n"
									 "		{\n"
									 "			if (int(mod(gl_FragCoord.y, 2)) == 1) discard;\n"
									 "		}\n"
									 "	}\n"
									 "	float f = 250*fwidth(Vertex.xyz);\n"
									 "	float D = clamp(min(min(Distance.x, Distance.y), Distance.z) / (f/250*15), 0.9, 1.0);\n"
									 "	FragColor = vec4((vec4(D,D,D,1) * texture2D_Filter(texture, UV_fr, f)).xyz, gl_FragCoord.z);\n"
									 //"	FragColor = texture2D_Filter(texture, UV_fr, f);\n"
									 //"	FragColor = texture2D(texture, UV_fr);\n"
									 //"	FragColor = texelFetch(texture, ivec2(UV_fr*64), 0);"
									 "}\n");

			my_shader.compile();

			my_compute.upload("#version 430\n"
							  "uniform vec3 camera_pos;\n"
							  "uniform vec3 camera_look_at_pos;\n"
							  "layout (binding=2) buffer result { mat4 matrix[]; }; \n" //perverted, layout(binding=4) mat4 result[]; wont work
							  "layout (local_size_x = 1) in;\n"
							  "void main()\n"
							  "{\n"
							  //calculate look up at gpu
							  "	vec3 from = camera_pos;\n" //from to doesn't work right ? no idea why
							  "	vec3 to   = camera_look_at_pos;\n"
							  "	vec3 dir  = normalize(to - from);\n" //could be a "normal" for collisions detection
							  "	vec3 up   = vec3(0,1,0);\n"
							  "	vec3 right= cross(dir, up);\n"
							  "	up = cross(dir, right);\n"
							  "	mat4 look_at_matrix = mat4(vec4(right, 0),\n"
							  "							   vec4(up   , 0),\n"
							  "							   vec4(dir  , 0),\n"
							  "							   vec4(-from, 1));\n"
							  "	matrix[gl_GlobalInvocationID.x] = inverse(look_at_matrix);\n"
							  /* PROJECTION */
							  "	float h = 1.0/tan(45*0.0087266);\n" //FOV
							  "	float zNear = 0.1;\n"
							  "	float zFar  = 1000.0; \n"
							  "	float frustrumLength = zFar - zNear;\n"
							  "	mat4 ProjectionMatrix = mat4(vec4(h, 0, 0, 0),\n"
							  "								 vec4(0, h, 0, 0),\n"
							  "								 vec4(0, 0, -(zFar + zNear)/frustrumLength, -1),\n"
							  "								 vec4(0, 0, -2.0*(zNear * zFar)/frustrumLength, 0));\n"
							  /* PROJECTION END */
							  //"matrix[gl_GlobalInvocationID.x] = ProjectionMatrix * matrix[gl_GlobalInvocationID.x];" //done in geometry shader
							  "}\n");

			my_compute.compile();
		}

		bool update()
		{
			my_window.update();

			my_window.begin();

				glClearColor(1,1,1,1);
				glEnable(GL_DEPTH_TEST);

				static int frame = -1000;
				frame = frame + 1;

				//RENDER TO TEXTURE:
				{
					GLfloat pos[3] = {std::cos(frame/25.0f)*4.0f, 2, std::sin(frame/25.0f)*4.0f};
					my_compute.set(&my_camera_pos, 3, 1, pos);

					my_compute.begin();
						my_compute.bind(&my_buffer); //binds the buffer to storage buffer block binding=0..n
						my_compute.execute(1,1,1);
					my_compute.end();

					my_target.bind(&my_target_texture);
					my_target.begin();
						my_window.begin();
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						my_window.end();
						my_shader.set(&my_texture_pos, &my_texture);
						my_shader.begin();
							my_shader.bind(&my_buffer); //binds the buffers to uniform block binding=0..n
							//my_shader.set(&my_eye_shift, -0.02f);
							my_buffer.render(4, 8);
							//my_shader.set(&my_eye_shift,  0.02f);
							//my_buffer.render(4, 8);
						my_shader.end();
					my_target.end();
				}
				//RENDER TO SCREEN:
				{
					GLfloat pos[3] = {std::cos(frame/250.0f)*4.0f, 2, 4+std::cos(frame/100.0f)*2.0f + std::sin(frame/250.0f)*4.0f};
					my_compute.set(&my_camera_pos, 3, 1, pos);

					my_compute.begin();
						my_compute.bind(&my_buffer); //binds the buffer to storage buffer block binding=0..n
						my_compute.execute(1,1,1);
					my_compute.end();

					my_window.begin();
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					my_window.end();
					my_shader.set(&my_texture_pos, &my_target_texture);
					my_shader.begin();
						my_shader.bind(&my_buffer); //binds the buffers to uniform block binding=0..n
						//my_shader.set(&my_eye_shift, -0.02f);
						my_buffer.render(4, 8);
						//my_shader.set(&my_eye_shift,  0.02f);
						//my_buffer.render(4, 8);
					my_shader.end();
				}
				my_window.flip(1000/30); //max. 30Hz

			my_window.end();
			return run;
		}
};

int main()
{
	test_window test;

	while(test.update());

	return 0;
}

