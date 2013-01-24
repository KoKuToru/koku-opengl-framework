/*  GIMP header image file format (RGB): /source/koku-opengl-framework/example/texture.h  */

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
	"/U5K/U1P05-Z0U1_1E-\\1U5Y2%5V2%5X1E)[3%>#2U=\\1E5U2%=S2%AX2%F#45V6"
	"5UZM76*R76>E8G\"@6&J12%Z&4F>=77&U97?,9'7.7V_%2%N>1UJ#A)RIA)Z43FE9"
	"0UQ75FQ[1E>$3EZ025YZ1UYO1EUN26!Q1%EO3&!\\4V2/1EIT3692CZUXO-Z3H\\=\\"
	"6G]*16=02%]R0U=Y2EQS2%QF4F9F2EY@;GZ4>HJMG:W=UNHB\\P@`[@@`[0L[Y`DR"
	"/E1J/E1M0%)Y0%%[1%)X15-V1E-R1E-T15%X2E9_2E=Z1E5S1U=Q1UAU1UB\"35F2"
	"45FE5%RH6F2@:7>G8W:@46:45VVH8G6`:GS17W'&1UFB2F\"39WV6DJRL?9B(2F51"
	"3VAA25]K/D]Y3%V*3F1\\2&!M1EYK3F9S4VAZ1UMU/$UW3F)^67%BG+F*LM./M-B1"
	"J\\V9<I!W2V%O56F\"4F9P1UM;/5)*2%Q<KK[8YO8=]04V[`,Y]@P`[P@`[@D\\]P``"
	"/%-F/%)J/4]R/E!W04]S0E%Q1%)M1%)N0U!S1E-V1U1U151P155M1E=R1E5]1E.)"
	"5%ZE55^H4F&976^@6F^?46:>6VZY877*8W?-77\"[0%:&9'^3E;\"JH+VG>I=X36I-"
	"3VE<2%]E3%]^1%9Y1%II2%]E26%D1UYD1%IF2E]U6&>19G:6;H-]J,*>KLN4K,N0"
	"N-:CCJF)3F)D36!G25]18GA@5&I66VYLZOH9`P``^`P_\\P0W^P``]@T_[@<_[P@`"
	".U)C.U%G/$]P/$YS/DYN0$]K0E!I0U%J04YM0D]N15-O1%1L0U-I159P151Z/DM^"
	"35B;5&&D35Z52%R036*:6V^S;G_6;'_::G_1;(*_67.+AZ2?IL6GH+^4BJE^;8MF"
	"0U]).E-/4VA\\25UV2F%J2V-D2F)C15QB4VEU1EISO<OW\\P,EO]/3J<&FLLV?E+!V"
	"<8U37W9'4F9,DZ>-N=.EQ-ZMGK:5FJ^I]`8I\\/XS\\0$T^PT]\\P@U\\04W^```\\P8`"
	".E%B.E!F.TYO.TUP/D]L/E!G0%%D0%%D/TUH/DQH05%K0E)H05)E1U=O155X.$9V"
	"-41\\25F51%2..4R'3%ZE=XG:EZ@%CZ#[C*#HC*7/?)F=F;J>FKR+FKR!IL6-F;:'"
	"CZJ(5FY?3V-K36!P4&=M0UM<05A>3&-L/U1H?9\"Q[_XM^0@ON,O2:(!I5&Y!7'9!"
	"C:5:K\\=\\OM22ML^*MM1_O=R/QM^TT>;@]`@J\\/`U\\`0S^@PX[`(P\\P0X_PX`[_X`"
	".5!C.4]G.TYO.TUP/D]L/4]F/D]B/D]B/TUH.TEE/T]G05%G/U!C2%AP1E5[-$)N"
	",4%Q0%&&-DF\"-$6)4V2SB9GPK+P7H[4%G;+LH+G6FK6OI<2CFKB%H,\"\"L,R2J<20"
	"JL2=:8)K0UA:0%1@36-O3&)N1UUL1UQR8G66HK/>D)S5;'JF@)*=C*./CZIVN]>."
	"P]J(MLUTOM9]P-M_Q>6#J\\I]O-6TU>SRZ?LE[O`VY_HJ[?`N\\`4YZ?@TZ/8[[?H`"
	".$UC.4UG.DQO.TUR/DYN/4YH/$QB.TMA/DUI.$=E/4UG0%!H/DYD25ES259_,D!L"
	"-4=N-TEV.4F&4F*L=H3;CISTE*/SA9?8DJ7.H[K+I;RUJ<.IIK^8J\\64J+^*JL*/"
	"I;Z5D:F097MZ4&-S1EIS2%MZ1UM_1EF#:'FME*+?S-@8]`8UVNSWKL6JLLR,M-!\\"
	"NM!^N])YQ=]_L\\YPM]5`MM.>R^/BQ][_PM8,T^@CV.\\AW_,IW^\\RT^(IU>,HV.8J"
	"-DMA.$QF.DQQ.TUT/4UO/$UH.TMC.4EA/DUK-T9F/4QJ0%!J/DYF2EEU2E=`,T!L"
	"+3]D+#]I1U63A9/=JK<+EJ/S=('&5F:8A9:SH;7!H[6VH;6KJ[RJK\\*EH+*-M,:A"
	"IKV?IKRN=HJ27W\"+2%J!7&Z=G*WAV^PE\\P(`^0@`]@,`^@<Y^@P8K\\:IM].*N-5Z"
	"N])`M\\U[OM9_L<Q\\N=>:Q.#(T>H\"O=,(KL4!K\\D\"MLW`M\\T\"O<\\3V>LQS]`;A)7,"
	".$I?/4YI/DUS.DES-T=J.DIJ/$UH/$UH.TMM.DIM.$=G2%=S.TME35QZ3UN$.D=S"
	"-D5M*3IG3%B5?XS2I;'ZBYC=5F.81%)XBIBSD)^QDY^NC)BCA(V3=7]_='YT?HI_"
	":'ET5VIQ9'6/C9S&L<#XS-P;XO4S[`$`\\P0`\\/\\`]@,`^`D[_``@R^+'M-\"%O]Q`"
	"L\\QZP=B/N]23I;^.RN/,XOL#U>P=N],5E:_MBZ;>G;CDK<CTP-<0TN@?@YG!:'^?"
	"/5!@.4MB-T=I.4AN.DIM.4EI-TAC-D=D.4EL/4QR.TIJ,T)>1E9N<(\":?8FN.49Q"
	"-T1P,#UP.$1_6&.B97&N3%F/04YZ3EM^8VZ+E)ZXI:S)@8BC8VF\"869[86=V6F)P"
	"561X?H^LO\\[XW>TDSMXAQ=8?V^\\R\\P8`[`(`\\@,`\\@$`]04V]`H>SN72L]&,O=N\""
	"K<AXK,:&O-6NP-C'Y/X#ZP(BZ/\\[U>PUM,X1D:OEBZ?5GKGCQ-\\0?Y?(=HZQL<KG"
	"/$]6.4M8.DMF/DYN.TYM-4EE-DID/%!L.DIM.$AK/D]L.TMC6VQ]:7B,D)ZZ8&Z4"
	"56&42%6+,T!V4E^2=X.T?(FUA)\"YC9F`>8.H766*8F>.B8ZUBHVU4U9^,C5=/D-N"
	"55^1CYS2S=H6R=<9I+4\"L,(/W?$Q\\0<`[0(`]@D`[P$_]08Y]P<FW?+NN=2>M]:)"
	"L]*%N]BAWO7JZ/`.^@`P[P8R\\@@`X?D_QMTHP=D?EJ_FHKSM=8_!;XFZQ-T)NM/]"
	"1%E3/U-3.4Q;.$IA-DMA-4I@-TQB.4UF.4EI-$1D.TUD3%]N:'F\"2%EA>(B8A92R"
	"CIO.AY;.:WNO;'RL;GRH8&^945Z)6&./<'FEB9&_F)_-BI#!:VZ@24Q`.CUQ/4)["
	"7F>IC9?=J[C[D)[F?H_@M<D5[0,_Z0(XZ`(\\\\PD`ZO\\Z]PH`^@LT[@(+P-JTK,J-"
	"I\\:,T>[/\\@D;\\@<T\\08UZP$S[P0]YOH\\U^LXP=4AJ\\'\\67\"C;H:UM\\_`P-@*R^,7"
	"<HIS97MM36-B.DY8-DQ8/E1B/5-A-4M:.D]E/E-I-TI915I>5VIE,45!2UM::'F*"
	";X\"K87*I3V\"59G>K=86Y9G:H1U6%56.3>86V@H[!?HB^:G2J4UV304N!.T5[04J&"
	"5E^E;GO!>(?)7&VR77\"^H;@#W_DQY0$Q[@<`Y_`\\X?@T\\`@`]`D_]@<AR>#,I<&:"
	"O-K!\\PX3]PL^\\`4`[?`\\]@<[]08Y]P<`\\/\\`UN4N4&.>DZC8L\\KUF[/@KL7\\S.,="
	"BZ5_AI^!;85X2F!=-$Q--DY1.E%7.$]8,$95.$Y<059:7'-L:X!N7')>;X%RDZ>I"
	"K\\3D>8_!05:0=HK&I[GXEJGD6VRA3%V19W>I<8&U;GJS8FZI7VNF76FB4V\"32UB-"
	"04R+0E\"0/4V)-DJ(36\"MB*#IQ>$8X?TP[0<`T>LKT^LM[P(`]00`]@@NVO'FL,FR"
	"V?7Y]P`OZ/H`[OP`\\0(X\\`8L]P<F]@8GW.L8:':DFZ_+N]+CO]GFE+\"]E:_&O=CR"
	"=9%?>)-I;89O56U@/UE/.%%,.%).-U%1/%-:,TI05VUI:H)S7W9967%0:GYB?)*'"
	"LLO>FK/=3&&ABY[IQ-8DMLD3A);356F<6&J93F\"12UN77&NK:'BU56:=-4MS*S]E"
	",D)R,D)V+T%P/U6(;83+GK;_O]T5U/(IU?$TK<8.P=@CYOH`[O\\`\\`4Q[P3`P=G,"
	"\\0DC\\@<\\\\@,`]00`]04FX?/^[/_\\T.3<:GUZ;X-XH;J<N=6MHL*4F;J,GL*5I,F>"
	":(938G]27'I987UE9X)N87YK3FI</%A+/UA3+49!9GYO:H-L5W%-;89??Y=R:8)N"
	"E;&^J<3N4&BIC:+PRMTQQ-<GG;#Q1%F/0%2'-DI^.4N*4&&E6VVN1EF0+4-I*4!@"
	"*3QE-$=Q+$)K.4^\";X;-HKT'N=85Q.,?N-07B*/KK\\@2W?0_Z?L`[?\\S]@T3R>'B"
	"[P0CY/<E]P<VX?(1Q-C8D::4GK*677)0=HQGE*Q[FK=UHL%RG;YLG<!OE[QNEKQP"
	":HMA:8I@:(AB9X=C:(=F:(=G:8AH:HEK98%I6W=?57)56W968GY7:(-9:X9<;(AR"
	"C:J[K,CV9GZ]DZGSP=<DOM4@D*7G/%*-+$%[-4F%.$F.1UB?5VBL0%.,)#IC*3U?"
	"+T-I-$=Q+T-V.U&,5&JV<(G:E['\\K,H,G;OX=I+0G;C\\Q-TEV_([]0D`Y?X:WO8$"
	"Y/@)VNSYMLG&CZ61CZB!CZ9W5&M!?)-DG;5`F[5TFK=DG+M>GL!@G\\!FGKUPFKIR"
	"9HI=9HI=98E<98A>98A?9HE@:HID:XMG:(9E77M:5W527'I598%:98%99H-69X5L"
	"@9ZKF;7<8GNRB:'@KL,&K<(%A9O805:0*#UW+T%^/D^31E></DV-+T!W*CUF+$!B"
	".T]S,D5O*#MR*CZ\",$675&J_B:/QG+G^>9C/;X[#@9W;C:;HMLX0Y_\\UXOP:X?H\""
	"HK>QG[*=B)YXG[>$C*5L4FM!?)5=F+%XG;=VG+ALF[I=G+Q6GKY:H+]BG[QKGKIO"
	"8HI-8HE/8HE38HA69(==98AA9HAD9XEG77Q;5'-25')/7GQ7981<981;9X9=;(IO"
	"<Y\"4<HZE2&&(:8&PB9[4E:K@<XFY/E.!(C=E,4)V4&\"<3U^;*3AR'R]A+T%D+T)C"
	".TUR-DAW-DF$/4^83%VV>(WGDZK[<8S0.EB-4W&B97^Y<(K$L,G`WO@>N=7?G;JG"
	"E*^#D:IRC:9MA9YE67)!>9)9G[A]E*YMG;ENG+IEF[I=F[M7G+Q:G;Q=G[UDH+YI"
	"7H9)781*781.7X558X9=9(9B88-A7H!?4G%03FU,4W%.7WU89()=9()=:HAC<Y)R"
	"?)N*=Y2.:86)@)JGB:.QB:.Q97V(/%1AB)VO4V>!1E5]4V&10$V\")C-F*3AB-D5M"
	"5V613U^115622EJD7W'&>(SB<HC40%J8+$AY0U^-6'&J@9G6TNDC]`\\YT>OYK\\JZ"
	"CZMZE:]QF+%V5W!!@YQCE*YPE[)NF[=KF[EDG+M>F[M9F[M5F[M7G;U9H+]@HL%B"
	"6H%36']15WU36X%;8(-B7X)C6'M<471547-24W536WI988!?7WY=77Q<:(5F=9-N"
	"@:-R?)YJ?9YP?Z!T=Y=IA:5UCJY\\EK:(HKV=@9F*4V9N-$-A*C9E)3!I+#9T/4>%"
	"9W2I9W>K4F\"=2UJ<3EZH0U6<.$Z+,4M]+4EP/%>!8WFTI[G`X/(_]@<`\\`DXZO`1"
	"MLVYDJN\"4VU!@YYJE[-WD[%JJLEZE[A>F;M9FKU5F[]4G,!3G,!5G<!8G\\);H<1="
	"5GI=57E<57E<5WM>67Q=5WI;47133&].4G1267M988!?8']>6WI:7GU=;(EJ>)=N"
	"=Y9;?)U7BJMGC*UG@*%9@*!4@J-3BJI>C:QJL,J:FK\"877!N/4I<*SA7+#5=/4EP"
	">(>GC)VZ;'R<0%)W,4-R)#AK)3MK,$=R*$!G46B3BZ#:R]TCW.P[X.XZ]@8`\\P4G"
	"U.?N5&E9@9EXF;.\"I,%_F;EGC[!8J\\UOF;M9FKU6G,!5GL)5G\\-6G\\-8GL%9G<!9"
	"475L4W=J5GAH5'A?4'5337--3G)+3W1*4'1-67U677]=5WE75W=38X-=<I)J?)QN"
	">I=@>9=:>IA;?IQ=A:1?BJA?BZI;BZM9AZ99DJUGEJ]TI+R+I[N6@)5S<H5HDZ>-"
	"G;6<J\\2MAZ\"+3VE<+D5+(#9,)3E=)SUF1UV&DZC6QMH0Y/4QYO8U[/LW\\`0SS-P!"
	"9724RMSIO]>`DJ]XBZQ<I\\IMF;Q?DK5:EKABE[EAF;Q>F[]:G<)9FK]6E;E4D;52"
	"3G%N4W=N5WIG4G9926]'1FU!37)%5'E.4G916GU<6WQ>47-24W--9(59<Y5B>)EA"
	"@)UFA:!H@9UC@9Y?B*5DAJ1?B*19AJ)6B*18B:59B:-;E*UHH[QYGK=VFK)SI\\&#"
	"GKF!G[R%HL\"%G;N);H=P-$Q/*CU<,D1S97:JR]T0[O`S[/TN\\0$R[`$MQM;Y8'\"0"
	"P]+R[/\\+R^3&H\\&\"D+-8D;52E+=:E+5CD[-KD[-KE;9DF+M@F;U<EKI7CK%3AZI,"
	"57EL2W!=2V]21&I\"3G9#27%!4GA(4G9/4G586'IC5G9B0V1*/%Q!<Y5B@Z=F=YI6"
	"?YQ=?I=<?)98@)I<A)Y>A9]?AJ!BB*)DA9]AAJ!@B*%@BZ1CDJMJFK-RF[-TF+)R"
	"G+EZE+-PDK-EF+=RD*N!:']X+3U?.TJ\";'JWT-`9^PPXX_,8^PLLQ-7R569`N<S<"
	"XO7\\NM&`G+F$D[1DCK)/D;50E+9@D+!HE;)YC*ERG+MYEK9LD;->B*I2C[%9C:]7"
	"26]93W9:1&I$3W9*1V]!5'M+3'!+57=?4G%J46]N1&!@-%))57-:<Y1H>IY=A:EB"
	"?YQ=@)E>@9M;@YU=@YU=@IQ>@IM@A9YEB:)IB:)GB:-CB:-BC:9ECZEHCZ=HB:-E"
	"CZMOC*IEB:M5C*U=CZQW@IB*3EY^3%B3BI76V^@DX>`=]P<IM,7B4F-^EJ?\"Q-CD"
	"IKRNDJV#BJEDD[5=EKI7D[=4D;->D;%ICZURE+%X@J!AD;%I?I]1BJM9E+5CEK=E"
	"2F]/2W!-4'9.0FA!5'A31FA.3&Q@46YS67*)5&N+1EMW*T)14VYH<(]Q=Y=E=I5:"
	"@)UD@IYDA*%BAJ-A@Z%<?YQ:?YQ=@Y]C@)Q@@I]@@Z%<A*);BJ5?BZAFC*5JAJ%I"
	"?IEEAJ)FE+1BE[9GCJMT@YJ'5V:\"45R3C);5P\\`,U.,2B)>_2UI`9G6;M,/KIKG)"
	"B:*!D:YME;1HE[=CF;I@E+5;C:Y6C:U9BZI;AJ58F+=JC*M>BZI;D;!CBJE<E+-G"
	"069$2W!.069$37%42VQ84&]H3&AR9H\"887>=0UF\".4US)3M3-$U07WQI@J%Y@J!M"
	"AJ-JA:-DAJ5@AJ9<A*1:@:!;@9]@A*)G?IQA?YU>?YY9?Y]5@Z-7AZ=?AZ9DA:-F"
	"B*1JA*%?D+!;E[=CBZELB*\"%<X64@8ZWH*S?I;'B8G&735U];'V:DJ.^R=KUH[>W"
	"@IQLF[INF[INBJE:B*A6C*Q7BZQ4BZQ2E[A>C[!6?Z!(A:51G[YQC*IAEK1O?)E7"
	"3&Y44'):16945G9J265G2V5R1V!W8WJ:8'28,$1H,45A4F=Y2&!A0UU057%7?Y]Q"
	"BZMKAJA:@J52@:11@:-5?Z!:?IUC?YUJ@)YK@)]E?9Y:>9M.>9Q)>YY+?)Y1>IQ/"
	"@*%1=9=!?)]!A:=)@9]8@IMR9WIU;'N-<8&966F#6VQ_2UUJ<8:,P-;3EZ^@=(UL"
	"I\\.'NM:-M-\"'G[MPEK)FD:Y=CZU8E+)9BZI-@J%\"C:Q0D[%<A*!5D:MJ=8Y6?Y=D"
	"0V120&%10V);0%Y?.E5?,4M;/%5H2&%T5VY]@)BA@YN<<(I`:81R@)R$;XQO;XYE"
	"AZEK@*19>I].>)Y*=YQ-=9A4=)5=<Y5B<9%?<91;<911;Y5)<I=(=IQ.>IY7>IY7"
	"?J!3AZE4DK-9C[!8A:->?YEH56Q136%6:7YVA)F3>Y\"*A)J/G;6DBJ2&7GQ*GKQ]"
	"M=.*M=*#L<Y]KLMZH+UNB*56?9E-A*!4?YQ-C*E8BJ=8D*QA>))1<8I2A)MP>(YI"
	"1&)74&UG0U]A5&]U+TI46G5]87M[;(A[;(AP=))M<Y-C;8Y:=9-@=Y1E=)%FBJE^"
	">YMK=IEB<I=3<9=-;Y5+;9)-;)!3;))89HE2:(Y2;9).;95/<YA4=IQ@>YQN?)UQ"
	"=I-F?YQG?YUB=I15<XY6=(]96W5%:X55E;%`F+.'C*:\"E*V,;HIC7GQ)E;ENL-1]"
	"K,UUJ,=KK<UKM-1RK<QPG;IKCZEKB*!OD*=Z<8A;>Y-@9GY)?95DG+**HK:<=(=T"
	"05U=1V-E0EUC,TY6&C5!1F)B8W]R6WE>:HE>7G]'<I54;(],;HY0<(]7=Y1E;(E>"
	";HUD;8Y@;I%8;I)1;)%-:8Y*:X]2;))6<Y9==)I>>)Q;=9Q:=9E<<I=B<I-I;X]I"
	">I9O<8YC:(94;XU:?IQI?IQJ9()0@9]L:XE4H+Z+@I]P5G%#?)EDD*]LH,)JH,1C"
	"J,=KJ,5IK,MEI\\=AH+]BH+UNDJMR<XQA3V=&7'-6;X=FA9][CJ>(P-B_M,J\\:G]W"
	"05MH4&MU,DU7'3A!'SM!.EA/8X!M6WI<88%98X169H94:8I6:HI8;(Q<;XQ?<(U@"
	":HI::8I6:8I4:HQ1;8]4<))7<I-;<91;<9-8<919=)9:<I97;Y%5;)!3;Y!8<Y1>"
	"9890:XQ6?)]D>IUD?9]N7']6;Y!T985N2&5/1F5'77Q4B:=TIL.\"J<5YIL1MJ\\AM"
	"L,IRKLARL,IRJL=NK,IUE;%E8X!!2&5!3&E!5&]-5G)88'UJ7WMMIL&YS^SF5G-N"
	"4VU]0EQI*41,/%A:1V1>0F%26WE@9(-B985?989:9H989X=7:(A8:HI:;HM<;XU;"
	";Y!<;Y!:;H]9;H]9;H]9;H]9;H]7;H]7=99><I19;I!5:XU1:8M/:8M-;(Y0;Y%3"
	"=)9:?)]F?)]H=III889<2VY12&I:.%E/-E9,/%M,9(-CFK>*J<:'FK9KH[]LQ-Z&"
	"JL1LK,9PK<9TK<AXBJ=8G;QO@J):AZ=G>IIH9H9B+DU!-51-0F!?)T='-%1026IC"
	"05QF/5A@,DY216)=26=:/U])4G%36GI48H-98H139(519H=1:HM5;H]9<Y1>=99@"
	"=)5?=99@=99@<I-=;H]9:XQ6:HM5:HM3;H]7;(Y3:8M0:HQ1;8]4<9-7=)9:=)9:"
	"=YA@=IAE6WM34G13,E1!.%E',E1%*DQ!.%A,,5!!.E=$7GM<CZI^JL:*J\\=\\H;UG"
	"J<9KG+E>BJ93I\\)TI<)SH+]R=Y=+K,V)CJY\\AJ6%36MB*$5)+TQ3,$]3+U!)+%!#"
	"/%=?2&1H1&!@0V!:0V)31V=05W964W--7'U17'Y+7X!,9(5-:HM3<))7=)9;=YA@"
	"=)5?=I=A=YAB<Y1>;H]9:8I4:(E3:8I48X1.9XA0;(U5<)%9=)5==YA@=IA==)5?"
	"<I)B7W]9.5=!,5!!'3U!,5!),E-),U5(-5=*/%Q0-E1)-%!!5G-4CJM\\I\\2#GKUP"
	"EK1=J,9MJ,5RG[QKK,M\\E;9HA*99E[IY<)%C-E9!*$9%-E1?,U%<+$U3+5%(*D]!"
	"261J0U]C.U=71&%;3&M<3FY84W)44G),67I067M*7'Y+8(-,98A1:8Q3:XQ4:XQ4"
	":XY7;8];;Y%=;8];:8M798=398=39XE58H10:HU6<91=<I5><)-<;Y);;(]8:8M:"
	";(IG36I4-E)%,$U'+4M*,U%0+T]++$](+E%*.EI6/UU:-U1.,U!!1V9%=I1BG;M\\"
	"H<!SG[]KK,QXG;]JDK1?@Z95E;ER8H5,/5Q!,4]\",4Y3*TA5+DM8,U58+U)+,5=)"
	"/5E=.E98/%E60V%82FA;2VM53FQ147!/5G905G=-67I,7H!/9(95:(I7:HI8:8I6"
	"8H119(929HA59HA49(929(9298=39HA4:(I6;Y%=<Y5A;Y%>:HQ9:HQ9:HQ9:(A@"
	"6'-?-U)(,D]*,$U*-U56,5!3+$Y.*TU/+U%3*TU/+TY1-5-0,5%%,%!!1&-\"7GY0"
	"GKV\"E[=ODK-CG+]JBZ]8@J=8=)A;-UI!,%!!,$Y+,E!9+DY:,%%9*DQ.(TA\",%5,"
	"-U-30E]<2F=B1F1;1F173V];4G);4G-747-24')05'1.67E18(!89H9>:(A@9XA<"
	"7X!28()/88-28H118X5298=49HA59XE6:(I7;8]<;I!=:8M:9XE8:HQ;:XQ>:8AG"
	"/EE3*D-(,4Y++TQ',U%.,E)2+U!6,E-=+$Y9+U!:,5-5-%10,U1*-%1&-59$.%A!"
	"5W52BZEVF[QTE+IF@JA2D;=M6'E+,5%!,$Y/+4M6,4]:-%5;,%)2+U1.,%11)$A%"
	"0E]9/5M2/UU205]22&=84'!<36U636Y44G-74'%34&]/4W)16'=67'M:7GU<77U7"
	"88)688)48(%38H1398=69XE6:8M::HQ99HA7:8M::(I99(579(57:(E=9H==7WY@"
	"-4]/,4E20%Q</%M5-E5/-U=5+$U5+DY<+U!@*TU8*4M.*DU(+5%(+E)),5)+,E-+"
	",DY!/5Q!<9-8B:]E@:=9C;1T1FA',$](,$U:-5%E*4I4+$Y0+E--)TQ$*T].*T]0"
	"2&=62&A41F921F9206%+5G9?4'%75G=;4W184W165'575'955GA767M:6GQ;67M9"
	"6WY578%47H)57X-47X-48(1388568H957X-48H97:8U>8X=:6WY4:HUC:8QC3&U1"
	"+TM+,DU5-E)2-51-,U)++T]++U%4,5)<*DQ9*TU8*DU0*$Q)*$U'*U!**TU-*4M-"
	"-%!4,E!%,U-!8(5.EKQ`3G1$,E-!,E%4+TMA+TMB+DY:+E!3+%%+*U!(*DY-*DY/"
	"0&%'0F-)169,36Y4169,4W184'%536Y24W185'575'574W544W545'9557=65GA6"
	"6'M46GU47']57']57(!378%47H)57X-488567(!38(177H%76GU49HEB8H5>169*"
	"+TQ'.557.UA5,E%*+U!(,U1-+U%1)TE,+$U5*TY5*TY1*T]0*DY/*4U.*4U.*DQ/"
	"*$1.,$Q.-%1!5'=6@*1_26Q-,U1**TE0+TM?+TMA+4U;+4Y6*T].*T],*DY-*DY-"
	"36M03VU226I.4'%51VA,2VQ.3W!226I,3F]13W%04'-23W-037%.37%.4'1/4W=2"
	"5'A357E45WM45WM46'U36'U26G]46X!37H-66'U078)78H==8(5;7X-<3G)-+E%!"
	"*4I!,5%'-51-,5!*+U!),5)*+D]%)$A!*4Q')DA*)D=/*DM5*$I4)DE.)TM*+4]/"
	"+4M4,DY8*TE*+4Y&1VE<+E!#,E-,+$Q,+4Y6+4U9+$Q:+$Q:+$U7*TU0*TY+*TY)"
	"16)/2VA30F).2&A21V=11&1.26E32FI3169,1VA,26Q/2VY/2FU,2FY+37%,4G91"
	"47134G545'A55'A357E25GM16'U367Y36X!55WQ178)89(E?8(1=6'Q71VM(,51!"
	",E1!+D]!+T]#-%-,,5)**DM!*DQ!,5-$,U1**$M((T1,)T=5*$A6)$90)$A'*DU*"
	")4-*+$A4*D=4)D1/,$Y5)D5(,E).+$Q(+4]/+$Y1*TM7*TI;*TI;*TQ6*DQ.*4Q)"
	"5&YN4VUK36=G3&9D2VAE0%U8/EM6/EQ306%50&%10V102&I22&Q12&U-2F]-37)0"
	"3W)54'-647154G544G915'A15GM16'U26X!55WQ16'U36'U34W=047504W=44W95"
	"0&5%-UM!,51!+E!!*TU!*$I!,%%!.5M+0F-9,U-1*$9/*$A6*4E7)490(T5%)DE&"
	")41'*$9/*TA9*D9:+TQ=+TU60&!>1&5=+U!(+DY*+4M4+4I9+4E=+$E:*DA1*$=+"
	"<(:<2%YT25]U0UEO.5)E/%9F0EQJ.%-=0E]C/%I9.EE20&!216A51FE21FI12&Q1"
	"3&Y436]53G%43W)33W-047505'A15GM15WQ26'U26'U35'A14'1/4'1/5GI77X)A"
	"7H-A7H)E4W=<1&=016A15'=B7'UK6'AJ.UQ4+T]/)4-,)D1/*DA3*4I2)TE+)TE)"
	"+4Q/*DA/*$56)D)8*45;+4I7/EY<3&UC+U%$+D]%+4Q/+4M6+DI>*T=;)T15)$%0"
	"FZW>0E2%8'*A:7NH56N4=XVUCZ;):X.@0%IQ,TY>+$E0,U%2/5Y706-60F150V93"
	"2&I326M32FQ22VY13&].3G)/475.4WA.4G=-5WQ25GI35'A14W=24G914G935GE8"
	"57E<5WY>4WA626Y+47967X-J6'AJ0F%;*DA))T1)(T!%)$-'*$=+*TI.+$I1*TE0"
	"(T)%)T9**D=4+$E:+DM<+4M6*$A(,%%*+U%$+E!#+DY*+DU1+DM8*T==)4!<(#M9"
	"L\\/`.DJ'=H:`CZ#7:W^SF:W@K\\3T:7ZJ/%-V*T->(#I()T1)-E11/5Y40&)506-3"
	"16A316A11VE11VM026Q/2VY/3G)/4'114G914W=237%,26U*4'1157E657A75WQ<"
	"57E<4WI:57M56H!88XEC7H)G0F17)$1\")D-**$9/*D=.)T9))T9)*4A,*4=2)T50"
	"*$9-+$I1*$91(T!-*493+4M4)T9),5%-+U!)+D](+DY,,$U2,$Q8+$9=)#U='C9;"
	"FZWL/$^*9'>NBY_2=8:Z<(&U?(W\";7ZR872>46:$/U=D.555/EY2/5Y..UU-0V55"
	"061/06=/1FE21FQ326U42FY32V]43'!337%42V]22FY137)24G5657A95'=84796"
	"5'A;5'M;3'1.6X1;6H)<.V)&*T]\"(D1$)D1-)$5/)T5.)49,)D=-)TA.)T=3)T=3"
	"+$I5(#Y)*495*D=6+4M2)T9),%!0*TI-+4E3+4E6,T]<*T52*T52+$58'S-5)#=@"
	"<87!,$5]3F*566Z</U%`.$E]2UJ24%^58'&>6&N,2&!M/EM8/EY0/%U+/%Y.0V55"
	"/F1.0&9.0FA01&I216M31FQ426U42FY51VM21FI/1VM02FY13G%4471547154'-2"
	"479657U:4'A25GY82G%1*E!!(T9!)$9))$10)$10)$5-)49,)49,)D=/)T=3)T=5"
	"*4=2+4M6*TE4)4-,*4A++T]-,E).,%!.+DM/+4I/-5!6+DE/+T=2+D58'C%2'2Y8"
	"=XK%0E6,05:&,49R(C1A(3-D,T)X/DZ\"2UR)3V*#2%]N/%A8.EI..5I*.5M,/F!1"
	"/6)//V5/06=10FA21&=016A11FI11VM21&A-16E,2&M.2VY/37!/3W-04'114750"
	"3W5/5'I43G1.3G-30F50*$E!(D!')T13)4)3)4)1)4-*)D5()D5()T5,*$54*497"
	")$!*+DM0)$1$(4%!,%!.*TM')$5!,E1$,5-!-%=!.UM$-%!#*T)()3I.'2U/'RY6"
	"F:SE5FJ>.4YZ)#IC*#ME,T5T05&%/4V!,T1Q/U1T/EAF.%16-U=-.EI,.5M..EQ-"
	"/V110&9006=106=10F500F5.0V9/1&A/1&A-1FI-26Q/2VY/3&].37%.3G)/3W-."
	"3W5/3'),16I'1VM.1FA8-E92*4=2)D)6)4%7)4)3)4-*)41')41')D1+*$54*$54"
	"+4I1+$I+*TQ$.%E267A[,U)5*DI&3G%<:Y!C;)%<;Y!D6WE@.U)8)SA2(S-5*3A>"
	"AIG01%B++3]L(C5>*#MD-TEV1UB,.4I^,D1Q/E-S/5=E-E)4.5E//U]1/F!3/F!3"
	"0V550V930V930F520&-0/V)-0&-.0&-,0V5.16=/1VI-2&M,26Q+26U*2FQ*26Q+"
	"2W!02&Q/1FM+2&Q136]?06%=*DA3(CY4)4%8)4%7)$%.)$)))41()D1+)T11*$91"
	",4Y3)T5\",U-'06!:;HJ6.%-E-U-=7GYP>)UI=9Q:=YIA:XAI1%A@(#%.'\"I-(\"Y2"
	"25R3+4%T+D!M)CEB(31>*3MH/U\"$.$E]/4]\\15IZ/UEG-E)2.%A./EY0/V%40&)5"
	"06-306110611/V%1/5]//%Y./5]//6!-0V120V1016=01FA01VE/1VI-1VA,1F9/"
	"0F-31&963'!72V]62VY90&)5)D5)'SQ+)D)9)4%8)$%2)$%.)4-,)D1+*$9/*4=."
	"+$E-)D1#,5!)+DQ-3FB\"*$!C+$5B05U?=)=M<Y=:@*!NBZ2-8W:%,D!D*399*#58"
	"*SMU.DN\"/E!_*3QF)CAE)3=F-49Z-D=[-DMW/E5U/EAE.555.EI..UQ,.EI,/5U/"
	".UU-/%Y./5Y..UM-.EI..EI..UM//%Q20&!60&!60F!51&)51F171V971&-40F!7"
	"-E-7.UE:26I:2&I02FU.0V5+*4E!)$%&)T)6)D!:)3]7)4!4)D)/)T--*49+*D=,"
	",$M1+TI0,DU3+499+D-O)#=N)3IJ*$!98'QL:8=DB*2*H;BQ;GV9*S=@)#%4+#=6"
	"+CY[46&;1UB,)#9E+3]N*SUN+#UT+3YS)CMG-$MK/%9D/UM;/U]3.EM)-UA(.EI,"
	"-U=)-UE*.%A,.%A.-U9/.%=1.5=4.UE8.UE:.5=8.U=9/EI<0EY>0U]?0%U:/5A>"
	"+DE=,$M;/EU70&)*2FY+2V],,%%!*4A\"*$10)T)6)D!8)3]6)D%1)T--*49-*TA/"
	"+$93+$96+$5:-4QM(31K*3MZ)CEP*#Q@+41*26!7?I21H;.`9G.6'RA2)S%30$MG"
	",4\"$1524156/,T1Y+#UQ,$%V,41],D5\\+4)P-4QM.U5C.U=7.EA-.5A'-U9',E)$"
	".%A,.%A.-51-,5!*,U%0.59:/%=?.E5?/EAE-E!>,$I:+TE9.$]@15QM1%MJ-$UB"
	"+D9I,$EF/EM?2&E7/F%!2FY)3W)5)TA!*4=&(CY()D)/(CU-(3U*+$A5+TI:,DU?"
	"*T1;-DMI+#]H)SEJ,4%]+#QX(#)C(S58(C=+1%=GE*2\\B)6T0DMS)2U5*#%12U5O"
	"+CR\".TJ,.DJ&,$%X,D-X.4R#0%..15B32U^20UE_/%5H.%18-E1+-%)%-E1).EA-"
	",E%*-51.-U54.5=8/5IA0EYJ0EQL/E=L.5%J,$AC*T-@+$1A,TAH/5)P0%5S-TMM"
	"+T5N,DIM/5EE1&-</%Y&0V9)2FQ2+D]!'S]!(#Y!*DA)+$E.+DI4,DU=-4]G15Q\\"
	"2EZ\"0E5_-49]-D:\"0%\"*.$IY*SY?*S]8/4YH76R*CIJ_;'6?+S=?)2M0.#]<96V%"
	",3Z#,T\"#+#QX*3IQ+T)Y.$N$05.02EZ:2V\"6.U)[,DQC-E-:.555-5),-5)-.E=4"
	"-U-5.%18.%-;.5-@.U1G/%5L.%!K-$MK.T]S,$-L+D%J,41M,D5O-TIS/$]X.TYW"
	",TER.$]P/59I261J2F=B2FA=1V58,U)#(D%!,5!!/EU..UE005UA1V!U2%Z$6&V;"
	"8'*A6&>=4E^B4F\"@4&\"02%QV1EMA2E]A8G.$@(^MBI.]3U:#(RE,*2Y,1TUD>'^2"
	"+CQ\\*CAV(C%I(C-H*3UQ+4!Y-4F'1%B61%F3-4QY*4)?+4A8-5!8-U-7-E%7-U):"
	"-$]9,4M9+D=:+D=>,DIE-DUM-TUS-DQU/$Y[,4-R,4-R-DAW-$9U,D1S.$IW.TYX"
	"-TUU.U)S/%1M4&IZ7GB%5G%W1V-E-%%,+$I!1&1026E3.EE*16%C46J!2V\".4F6>"
	":7FM;7RT;'C!6VBK2ER#4&9T8WIO:X)W>HR7E:+!?(.P.3UK(29%,C=,35-B<7>$"
	")3%L'BUC&RM=(#)A)CIM)SIQ,$2\"1%>85FJH0UF,*T-H'SE3)C]4,TQ?/%)H/%)J"
	")T!7)#Q5(SM8*D%B.$QR0U9`2UV*3F\"1.4I^+T!T,4%U-T=Y-$1T,$!P,T1Q-DER"
	"-DUP.U)R.5%N2V-^6G*-46F\"1V!U/UEF16)?3FQ?26E5/%M,2F9H4FN\"1UR,3%^8"
	"9G:H<8\"V:WB^4V&A46\"&;8&+@)>&>(]^B9JCD9^[7V:3)BM6)2M&0$9576)I;G-V"
	"&RA;&2=7&\"E4(#-=)SEH)#AL+4%_0E686FVN25Z8,$=T(#A=)3Q<,TMH/%!R.DYR"
	")SY?)#M>)3MA+T5N/E!]1UF*35Z24&&6.TR!,$%V,4%U-D9X-$1T,4)M-$5O-$9M"
	",T=K.5!Q-TYN05AY1UZ!0UI]2F&\"3V=`57!Z2V=G1&)70V%83FIN3&5\\0%6#35Z3"
	"46*-7&R>6VBM7&JJ?8RTGK*^F*ZB>8^#F*BTA9\"O14EX)\"=3-3E36EYH>W]_>X!Z"
	"&2=3&\"=1&\"I/(C9:*SYH*CYQ,D:$15B<25VA/5.0,49\\+41Q,4=P-DQU.4MZ.$IY"
	"+4)N*#UI*3MJ,4-T.TQ`05*'0U2+15:-1E6-.DE_-45W-D9V-$5P-$9M-$9K,4-F"
	",D9J.4UQ/%!R2%Q`3&\"&3%^)56B25FJ03&%]1EUL2&%F15YA1U]J2F!V0E5^0U9`"
	"0%)U1E>!3%>88VZOCYW+H[3.EJJLAIJ:FZJ`;WB@+S)F*RU=1$AB9FMRBI\"'>']N"
	"&\"=/&\"=-&BQ/)CI<-$=P.$U]05634F6I1%B>-TR.,$5]-$I\\.$U[-4IX-TA].TR!"
	"*3YN(C=E(S1H*SQP-TA_0%&(156/25F34%^705\"&-T=Y,T1Q,$%K,D1I,D1G*SU@"
	"-4EM.DYR0E9Z5FJ.872=87.@8'*A46.01UM]56N#8'B#3VAN26%N6G\"&5&B,15E_"
	"0E5T0%)W/DF(4EV><7ZQ>(BH?Y*:F:RSCYJW5UZ)'1]8+S%A24IG869K?X5Z8&A2"
	"";
