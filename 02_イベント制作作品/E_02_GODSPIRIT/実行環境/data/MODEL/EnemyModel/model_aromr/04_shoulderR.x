xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 231;
 -9.39914;1.78307;-0.02742;,
 -7.05679;3.07706;-0.02742;,
 -6.77821;2.89905;-1.93678;,
 -9.07534;1.49000;-1.33791;,
 -6.08320;1.12999;-2.68819;,
 -7.95103;-0.06882;-2.63980;,
 -5.33716;0.00690;-2.07497;,
 -5.59754;0.32171;-2.15603;,
 -4.92626;1.09414;-1.12927;,
 -5.62611;0.70359;-1.12927;,
 -5.25957;-0.24079;-3.52730;,
 -6.26903;-1.60466;-3.51516;,
 -5.03394;-1.40714;-2.59943;,
 -5.23621;-0.16480;-3.03773;,
 -4.48374;-1.41713;-4.20239;,
 -5.03966;-2.55110;-3.59402;,
 -3.98410;-2.60938;-3.27492;,
 -4.37749;-1.59396;-3.71428;,
 -1.84649;-2.52334;-4.29234;,
 -1.96347;-3.75467;-3.83625;,
 -0.28631;-3.57425;-4.07891;,
 -0.28889;-3.57580;-4.07891;,
 -5.32449;0.48578;-2.15603;,
 -5.54033;0.76839;-1.12927;,
 -4.96316;-0.00073;-3.03773;,
 -4.10444;-1.42989;-3.71428;,
 -5.26727;0.93246;-1.12927;,
 -5.50776;1.37096;-0.02742;,
 -4.89369;1.69672;-0.02742;,
 -5.19408;0.40767;-2.14688;,
 -4.69010;0.16333;-3.03773;,
 -3.83138;-1.26583;-3.71428;,
 -2.86179;-2.76425;-3.52993;,
 -2.58871;-2.60019;-3.65352;,
 -0.40422;-3.57268;-3.70362;,
 -0.40163;-3.57113;-3.70362;,
 -4.99423;1.09653;-1.12927;,
 -5.23470;1.53503;-0.02742;,
 -4.87163;-0.02477;-2.15832;,
 -4.41706;0.32739;-3.03773;,
 -3.55834;-1.10176;-3.71428;,
 -2.79342;-2.43613;-3.50097;,
 -0.39904;-3.56956;-3.70362;,
 -4.72118;1.26058;-1.12927;,
 -4.96166;1.69910;-0.02742;,
 -4.35042;0.12567;-2.14688;,
 -4.14401;0.49146;-3.03773;,
 -3.28528;-0.93769;-3.71428;,
 -2.52037;-2.27205;-4.13958;,
 -0.39646;-3.56801;-3.70362;,
 -4.44813;1.42465;-1.12927;,
 -4.68860;1.86315;-0.02742;,
 -4.07736;0.28974;-2.14688;,
 -3.57338;0.04540;-3.03773;,
 -2.71466;-1.38376;-3.71428;,
 -2.24731;-2.10800;-4.13958;,
 -0.39385;-3.56646;-3.70362;,
 -4.17508;1.58871;-1.12927;,
 -4.41556;2.02722;-0.02742;,
 -3.80432;0.45381;-2.14688;,
 -3.30034;0.20947;-3.03773;,
 -2.44161;-1.21969;-3.71428;,
 -1.97427;-1.94393;-4.13958;,
 -0.39127;-3.56491;-3.70362;,
 -3.90203;1.75277;-1.12927;,
 -4.14250;2.19129;-0.02742;,
 -3.53126;0.61787;-2.14688;,
 -3.02728;0.37353;-3.03773;,
 -2.16856;-1.05563;-3.71428;,
 -1.70122;-1.77986;-4.13958;,
 -0.38867;-3.56334;-3.70362;,
 -3.62899;1.91684;-1.12927;,
 -3.86946;2.35535;-0.02742;,
 -3.25822;0.78193;-2.14688;,
 -2.75424;0.53759;-3.03773;,
 -1.89551;-0.89157;-3.71428;,
 -1.42817;-1.61581;-4.13958;,
 -0.38610;-3.56179;-3.70362;,
 -3.35594;2.08091;-1.12927;,
 -3.59642;2.51941;-0.02742;,
 -2.98517;0.94600;-2.14688;,
 -2.48119;0.70167;-3.03773;,
 -1.62247;-0.72750;-3.71428;,
 -1.15512;-1.45175;-4.13958;,
 -0.38350;-3.56025;-3.70362;,
 -3.08289;2.24498;-1.12927;,
 -3.32336;2.68348;-0.02742;,
 -2.71211;1.11007;-2.14688;,
 -2.20815;0.86573;-3.03773;,
 -1.34942;-0.56343;-3.71428;,
 -0.88207;-1.28767;-4.13958;,
 -0.38091;-3.55867;-3.70362;,
 -2.80984;2.40903;-1.12927;,
 -2.83373;1.69768;-0.02742;,
 -2.93503;2.29100;-2.14688;,
 -1.93511;1.02979;-3.03773;,
 -1.07636;-0.39937;-3.71428;,
 -0.60902;-1.12361;-4.13958;,
 -0.37833;-3.55713;-3.70362;,
 -3.67485;3.88693;-0.02742;,
 1.24777;3.75695;-0.02742;,
 1.26226;3.51509;-1.63274;,
 -3.39906;3.38965;-1.40474;,
 0.30538;1.57692;-1.20138;,
 0.50856;1.75678;-0.02742;,
 1.43028;2.88295;-3.17656;,
 -3.07967;2.93469;-2.68819;,
 0.25998;1.25963;-2.71302;,
 1.26374;1.44053;-4.51532;,
 -2.25602;1.56391;-3.52730;,
 0.22167;0.91824;-3.36024;,
 0.98069;0.20666;-4.73849;,
 -1.48020;0.38759;-4.20239;,
 0.22908;-0.33825;-3.72851;,
 0.56956;-0.75062;-4.53836;,
 -0.75671;-0.71865;-4.53836;,
 0.26532;-1.08783;-4.05406;,
 -0.25524;-3.55557;-4.07891;,
 -0.25783;-3.55713;-4.07891;,
 -0.37574;-3.55557;-3.70362;,
 -5.14272;1.33941;-0.02742;,
 -1.34779;-3.39080;-3.60905;,
 -0.40939;-3.57580;-3.70362;,
 -9.07534;1.49000;1.28788;,
 -6.77821;2.89905;1.88675;,
 -7.95103;-0.06882;2.58977;,
 -6.08320;1.12999;2.63816;,
 -5.33716;0.00690;2.02494;,
 -5.62611;0.70359;1.07924;,
 -4.92626;1.09414;1.07924;,
 -5.59754;0.32171;2.10600;,
 -6.26903;-1.60466;3.46513;,
 -5.25957;-0.24079;3.47726;,
 -5.03394;-1.40714;2.54940;,
 -5.23621;-0.16480;2.98770;,
 -5.03966;-2.55110;3.54398;,
 -4.48374;-1.41713;4.15236;,
 -3.98410;-2.60938;3.22489;,
 -4.37749;-1.59396;3.66425;,
 -1.96347;-3.75467;3.78622;,
 -1.84649;-2.52334;4.24230;,
 -1.34779;-3.39080;3.55902;,
 -3.61256;-2.92831;3.59738;,
 0.93524;-3.57580;4.02888;,
 0.93781;-3.57425;4.02888;,
 0.93524;-3.57580;3.65359;,
 0.93781;-3.57425;3.65359;,
 -5.22698;0.76839;1.07924;,
 -5.32449;0.48578;2.10600;,
 -4.96316;-0.00073;2.98770;,
 -4.10444;-1.42989;3.66425;,
 -2.86179;-2.76425;3.47990;,
 0.94041;-3.57268;3.65359;,
 -5.26727;0.93246;1.07924;,
 -5.19408;0.40767;2.09685;,
 -4.69010;0.16333;2.98770;,
 -3.83138;-1.26583;3.66425;,
 -2.58871;-2.60019;3.60349;,
 0.94299;-3.57113;3.65359;,
 -4.99423;1.09653;1.07924;,
 -4.87163;-0.02477;2.10829;,
 -4.41706;0.32739;2.98770;,
 -3.55834;-1.10176;3.66425;,
 -2.79342;-2.43613;3.45094;,
 0.94559;-3.56956;3.65359;,
 -4.72118;1.26058;1.07924;,
 -4.35042;0.12567;2.09685;,
 -4.14401;0.49146;2.98770;,
 -3.28528;-0.93769;3.66425;,
 -2.52037;-2.27205;4.08955;,
 0.94817;-3.56801;3.65359;,
 -4.44813;1.42465;1.07924;,
 -4.07736;0.28974;2.09685;,
 -3.57338;0.04540;2.98770;,
 -2.71466;-1.38376;3.66425;,
 -2.24731;-2.10800;4.08955;,
 0.95077;-3.56646;3.65359;,
 -4.17508;1.58871;1.07924;,
 -3.80432;0.45381;2.09685;,
 -3.30034;0.20947;2.98770;,
 -2.44161;-1.21969;3.66425;,
 -1.97427;-1.94393;4.08955;,
 0.95335;-3.56491;3.65359;,
 -3.90203;1.75277;1.07924;,
 -3.53126;0.61787;2.09685;,
 -3.02728;0.37353;2.98770;,
 -2.16856;-1.05563;3.66425;,
 -1.70122;-1.77986;4.08955;,
 0.95595;-3.56334;3.65359;,
 -3.62899;1.91684;1.07924;,
 -3.25822;0.78193;2.09685;,
 -2.75424;0.53759;2.98770;,
 -1.89551;-0.89157;3.66425;,
 -1.42817;-1.61581;4.08955;,
 0.95853;-3.56179;3.65359;,
 -3.35594;2.08091;1.07924;,
 -2.98517;0.94600;2.09685;,
 -2.48119;0.70167;2.98770;,
 -1.62247;-0.72750;3.66425;,
 -1.15512;-1.45175;4.08955;,
 0.96113;-3.56025;3.65359;,
 -3.08289;2.24498;1.07924;,
 -2.71211;1.11007;2.09685;,
 -2.20815;0.86573;2.98770;,
 -1.34942;-0.56343;3.66425;,
 -0.88207;-1.28767;4.08955;,
 0.96372;-3.55867;3.65359;,
 -2.80984;2.40903;1.07924;,
 -2.93503;2.29100;2.09685;,
 -1.93511;1.02979;2.98770;,
 -1.07636;-0.39937;3.66425;,
 -0.60902;-1.12361;4.08955;,
 0.96629;-3.55713;3.65359;,
 -3.39906;3.38965;1.35470;,
 0.67210;3.51509;1.35470;,
 0.12777;1.65434;1.15449;,
 -3.07967;2.93469;2.63816;,
 0.97290;3.04509;2.75486;,
 0.45176;1.61980;2.28737;,
 -2.25602;1.56391;3.47726;,
 1.50310;2.66053;3.88854;,
 0.29525;1.10238;2.84014;,
 -1.48020;0.38759;4.15236;,
 1.97428;1.40985;4.18014;,
 0.56583;-0.16384;3.43646;,
 -0.75671;-0.71865;4.48833;,
 2.47721;-0.55457;4.32702;,
 0.64881;-0.86208;3.61211;,
 0.96629;-3.55713;4.02888;,
 0.96889;-3.55557;4.02888;,
 0.96889;-3.55557;3.65359;;
 
 208;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;6,7,8,9;,
 4;5,4,10,11;,
 4;12,13,7,6;,
 4;11,10,14,15;,
 4;16,17,13,12;,
 4;15,14,18,19;,
 4;19,18,20,21;,
 4;7,22,23,8;,
 4;13,24,22,7;,
 4;17,25,24,13;,
 4;23,26,27,28;,
 4;22,29,26,23;,
 4;24,30,29,22;,
 4;25,31,30,24;,
 4;32,33,31,25;,
 4;34,35,33,32;,
 4;26,36,37,27;,
 4;29,38,36,26;,
 4;30,39,38,29;,
 4;31,40,39,30;,
 4;33,41,40,31;,
 4;35,42,41,33;,
 4;36,43,44,37;,
 4;38,45,43,36;,
 4;39,46,45,38;,
 4;40,47,46,39;,
 4;41,48,47,40;,
 4;42,49,48,41;,
 4;43,50,51,44;,
 4;45,52,50,43;,
 4;46,53,52,45;,
 4;47,54,53,46;,
 4;48,55,54,47;,
 4;49,56,55,48;,
 4;50,57,58,51;,
 4;52,59,57,50;,
 4;53,60,59,52;,
 4;54,61,60,53;,
 4;55,62,61,54;,
 4;56,63,62,55;,
 4;57,64,65,58;,
 4;59,66,64,57;,
 4;60,67,66,59;,
 4;61,68,67,60;,
 4;62,69,68,61;,
 4;63,70,69,62;,
 4;64,71,72,65;,
 4;66,73,71,64;,
 4;67,74,73,66;,
 4;68,75,74,67;,
 4;69,76,75,68;,
 4;70,77,76,69;,
 4;71,78,79,72;,
 4;73,80,78,71;,
 4;74,81,80,73;,
 4;75,82,81,74;,
 4;76,83,82,75;,
 4;77,84,83,76;,
 4;78,85,86,79;,
 4;80,87,85,78;,
 4;81,88,87,80;,
 4;82,89,88,81;,
 4;83,90,89,82;,
 4;84,91,90,83;,
 4;85,92,93,86;,
 4;87,94,92,85;,
 4;88,95,94,87;,
 4;89,96,95,88;,
 4;90,97,96,89;,
 4;91,98,97,90;,
 4;99,100,101,102;,
 4;92,103,104,93;,
 4;102,101,105,106;,
 4;94,107,103,92;,
 4;106,105,108,109;,
 4;95,110,107,94;,
 4;109,108,111,112;,
 4;96,113,110,95;,
 4;112,111,114,115;,
 4;97,116,113,96;,
 4;115,114,117,118;,
 4;98,119,116,97;,
 4;0,3,9,120;,
 4;100,104,103,101;,
 4;3,5,6,9;,
 4;101,103,107,105;,
 4;5,11,12,6;,
 4;105,107,110,108;,
 4;11,15,16,12;,
 4;108,110,113,111;,
 4;15,19,121,16;,
 4;111,113,116,114;,
 4;19,21,122,121;,
 4;102,2,1,99;,
 4;106,4,2,102;,
 4;109,10,4,106;,
 4;112,14,10,109;,
 4;115,18,14,112;,
 4;118,20,18,115;,
 4;91,21,114,116;,
 4;0,123,124,1;,
 4;123,125,126,124;,
 4;127,128,129,130;,
 4;125,131,132,126;,
 4;133,127,130,134;,
 4;131,135,136,132;,
 4;137,133,134,138;,
 4;135,139,140,136;,
 4;141,137,138,142;,
 4;139,143,144,140;,
 4;145,141,142,146;,
 4;130,129,147,148;,
 4;134,130,148,149;,
 4;138,134,149,150;,
 4;142,138,150,151;,
 4;146,142,151,152;,
 4;147,28,27,153;,
 4;148,147,153,154;,
 4;149,148,154,155;,
 4;150,149,155,156;,
 4;151,150,156,157;,
 4;152,151,157,158;,
 4;153,27,37,159;,
 4;154,153,159,160;,
 4;155,154,160,161;,
 4;156,155,161,162;,
 4;157,156,162,163;,
 4;158,157,163,164;,
 4;159,37,44,165;,
 4;160,159,165,166;,
 4;161,160,166,167;,
 4;162,161,167,168;,
 4;163,162,168,169;,
 4;164,163,169,170;,
 4;165,44,51,171;,
 4;166,165,171,172;,
 4;167,166,172,173;,
 4;168,167,173,174;,
 4;169,168,174,175;,
 4;170,169,175,176;,
 4;171,51,58,177;,
 4;172,171,177,178;,
 4;173,172,178,179;,
 4;174,173,179,180;,
 4;175,174,180,181;,
 4;176,175,181,182;,
 4;177,58,65,183;,
 4;178,177,183,184;,
 4;179,178,184,185;,
 4;180,179,185,186;,
 4;181,180,186,187;,
 4;182,181,187,188;,
 4;183,65,72,189;,
 4;184,183,189,190;,
 4;185,184,190,191;,
 4;186,185,191,192;,
 4;187,186,192,193;,
 4;188,187,193,194;,
 4;189,72,79,195;,
 4;190,189,195,196;,
 4;191,190,196,197;,
 4;192,191,197,198;,
 4;193,192,198,199;,
 4;194,193,199,200;,
 4;195,79,86,201;,
 4;196,195,201,202;,
 4;197,196,202,203;,
 4;198,197,203,204;,
 4;199,198,204,205;,
 4;200,199,205,206;,
 4;201,86,93,207;,
 4;202,201,207,208;,
 4;203,202,208,209;,
 4;204,203,209,210;,
 4;205,204,210,211;,
 4;206,205,211,212;,
 4;99,213,214,100;,
 4;207,93,104,215;,
 4;213,216,217,214;,
 4;208,207,215,218;,
 4;216,219,220,217;,
 4;209,208,218,221;,
 4;219,222,223,220;,
 4;210,209,221,224;,
 4;222,225,226,223;,
 4;211,210,224,227;,
 4;225,228,229,226;,
 4;212,211,227,230;,
 4;0,120,128,123;,
 4;100,214,215,104;,
 4;123,128,127,125;,
 4;214,217,218,215;,
 4;125,127,133,131;,
 4;217,220,221,218;,
 4;131,133,137,135;,
 4;220,223,224,221;,
 4;135,137,141,139;,
 4;223,226,227,224;,
 4;139,141,145,143;,
 4;213,99,1,124;,
 4;216,213,124,126;,
 4;219,216,126,132;,
 4;222,219,132,136;,
 4;225,222,136,140;,
 4;228,225,140,144;,
 4;206,227,226,143;;
 
 MeshMaterialList {
  1;
  208;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\armor.jpg";
   }
  }
 }
 MeshNormals {
  284;
  -0.519047;0.854745;-0.000361;,
  -0.468318;0.647324;-0.601373;,
  -0.316333;0.323660;-0.891727;,
  -0.323679;0.134138;-0.936610;,
  -0.281040;-0.197372;-0.939181;,
  -0.108057;-0.292183;-0.950238;,
  -0.018006;-0.224241;-0.974367;,
  -0.347003;0.937864;-0.000366;,
  -0.332966;0.760361;-0.557660;,
  -0.275535;0.433106;-0.858196;,
  -0.279591;0.262624;-0.923503;,
  -0.228002;0.011163;-0.973597;,
  -0.065168;-0.170072;-0.983274;,
  0.020619;-0.186927;-0.982157;,
  -0.082845;0.996549;0.005206;,
  -0.106600;0.908800;-0.403385;,
  -0.163006;0.672982;-0.721473;,
  -0.204848;0.414273;-0.886801;,
  -0.163705;0.177799;-0.970355;,
  -0.026197;-0.065716;-0.997494;,
  0.050377;-0.150822;-0.987277;,
  -0.003115;0.999937;0.010757;,
  -0.020504;0.952601;-0.303532;,
  -0.088618;0.790873;-0.605530;,
  -0.177953;0.443782;-0.878288;,
  -0.162881;0.136468;-0.977162;,
  -0.031340;-0.086660;-0.995745;,
  0.041566;-0.152569;-0.987418;,
  -0.172696;-0.984975;0.000629;,
  0.978252;-0.184041;0.095670;,
  0.990307;0.124940;0.060683;,
  0.882766;0.412874;0.224185;,
  0.821635;0.386417;0.419045;,
  -0.078381;-0.811968;0.578415;,
  -0.013780;-0.940583;0.339283;,
  0.975666;0.054164;-0.212464;,
  0.973108;0.187430;-0.133904;,
  0.851229;0.249523;-0.461679;,
  0.838425;0.266924;-0.475178;,
  0.825078;0.284152;-0.488369;,
  0.210703;-0.610948;0.763116;,
  0.168078;-0.157662;0.973084;,
  0.102106;0.183032;0.977790;,
  0.102605;0.180081;0.978286;,
  -0.092904;-0.948797;0.301915;,
  0.128406;-0.561953;0.817142;,
  -0.041597;-0.254628;0.966144;,
  0.007307;-0.060542;0.998139;,
  -0.044172;-0.192791;0.980245;,
  0.515041;-0.857165;0.000818;,
  0.275995;-0.779896;0.561773;,
  0.098456;-0.963058;0.250652;,
  0.113208;-0.962704;-0.245732;,
  0.058661;-0.240779;0.968806;,
  -0.424396;-0.509648;0.748430;,
  -0.183856;-0.534705;0.824796;,
  0.515027;-0.857174;0.000818;,
  0.392733;-0.700885;0.595416;,
  0.298461;-0.589135;0.750893;,
  0.167278;-0.958798;-0.229619;,
  0.206768;-0.178772;0.961919;,
  0.129911;0.157264;0.978975;,
  0.045155;0.383673;0.922364;,
  0.515032;-0.857170;0.000818;,
  0.413403;-0.688020;0.596429;,
  0.383681;-0.638529;0.667135;,
  0.217495;-0.966044;0.139478;,
  -0.029347;-0.456802;0.889084;,
  -0.011334;-0.129450;0.991521;,
  -0.099735;0.166487;0.980987;,
  0.515038;-0.857167;0.000818;,
  0.413402;-0.688020;0.596429;,
  0.404191;-0.672692;0.619770;,
  0.345697;-0.575336;0.741271;,
  0.210967;-0.351103;0.912261;,
  0.067581;-0.112577;0.991342;,
  -0.099993;0.166211;0.981007;,
  0.515041;-0.857165;0.000818;,
  0.413404;-0.688018;0.596430;,
  0.404188;-0.672695;0.619769;,
  0.345690;-0.575342;0.741270;,
  0.210960;-0.351107;0.912260;,
  0.067542;-0.112606;0.991342;,
  -0.100058;0.166164;0.981009;,
  0.515048;-0.857161;0.000818;,
  0.413413;-0.688014;0.596429;,
  0.404203;-0.672684;0.619771;,
  0.345705;-0.575330;0.741272;,
  0.210965;-0.351103;0.912261;,
  0.067742;-0.112512;0.991338;,
  -0.099678;0.166338;0.981018;,
  0.515044;-0.857163;0.000818;,
  0.413409;-0.688016;0.596428;,
  0.404201;-0.672686;0.619770;,
  0.345706;-0.575331;0.741270;,
  0.210970;-0.351098;0.912262;,
  0.067615;-0.112546;0.991343;,
  -0.099929;0.166267;0.981004;,
  0.242415;-0.959664;0.142411;,
  0.619408;-0.634472;0.462361;,
  0.640895;-0.513479;0.570608;,
  0.396346;-0.412798;0.820065;,
  0.210964;-0.351102;0.912262;,
  0.067535;-0.112588;0.991344;,
  -0.100075;0.166191;0.981002;,
  -0.229700;-0.973225;0.008371;,
  -0.239873;-0.961324;-0.135342;,
  0.606961;-0.521911;0.599340;,
  0.239156;-0.416651;0.877044;,
  0.118978;-0.370435;0.921207;,
  0.001750;-0.130379;0.991463;,
  -0.100526;0.166223;0.980951;,
  -0.114927;-0.993231;0.016879;,
  -0.191303;-0.981392;-0.016498;,
  -0.143651;-0.892679;0.427187;,
  0.471577;-0.521295;0.711243;,
  0.484412;-0.336866;0.807382;,
  -0.064071;-0.147584;0.986972;,
  -0.101435;0.166105;0.980877;,
  -0.468417;0.647520;0.601085;,
  -0.316331;0.323660;0.891728;,
  -0.323678;0.134137;0.936611;,
  -0.281040;-0.197372;0.939181;,
  -0.101765;-0.286400;0.952690;,
  -0.005290;-0.212146;0.977224;,
  -0.333050;0.760549;0.557354;,
  -0.275533;0.433106;0.858196;,
  -0.279591;0.262624;0.923503;,
  -0.228003;0.011163;0.973596;,
  -0.065402;-0.162697;0.984506;,
  0.020114;-0.172433;0.984816;,
  -0.104699;0.910163;0.400801;,
  -0.173026;0.709943;0.682674;,
  -0.198104;0.452562;0.869450;,
  -0.121312;0.224550;0.966882;,
  -0.002356;-0.007336;0.999970;,
  0.054326;-0.127167;0.990393;,
  -0.016774;0.954228;0.298610;,
  -0.106479;0.848801;0.517879;,
  -0.162789;0.519848;0.838604;,
  -0.078624;0.229760;0.970066;,
  0.023289;0.021621;0.999495;,
  0.063174;-0.121992;0.990519;,
  0.978252;-0.184041;-0.095670;,
  0.990307;0.124940;-0.060683;,
  0.882766;0.412874;-0.224185;,
  0.755223;0.631553;-0.175436;,
  0.595279;0.798474;0.089901;,
  0.042092;0.210859;-0.976610;,
  0.017628;-0.017220;-0.999696;,
  -0.718321;-0.439715;-0.539135;,
  -0.413559;-0.745141;-0.523195;,
  0.210703;-0.610948;-0.763116;,
  0.103508;-0.114400;-0.988028;,
  0.020483;0.045252;-0.998766;,
  0.034334;-0.012414;-0.999333;,
  0.316909;0.727044;0.609077;,
  0.070205;-0.955355;-0.287000;,
  0.128406;-0.561953;-0.817142;,
  -0.041596;-0.254627;-0.966144;,
  0.027999;-0.024461;-0.999309;,
  -0.012053;-0.136814;-0.990523;,
  0.276219;-0.780317;-0.561078;,
  0.098456;-0.963058;-0.250652;,
  0.113208;-0.962704;0.245732;,
  0.058661;-0.240779;-0.968806;,
  -0.384812;-0.485684;-0.784876;,
  -0.096977;-0.475372;-0.874424;,
  0.392981;-0.701305;-0.594758;,
  0.298461;-0.589135;-0.750893;,
  0.167278;-0.958798;0.229619;,
  0.206768;-0.178772;-0.961919;,
  0.119428;0.154116;-0.980808;,
  0.025275;0.378412;-0.925292;,
  0.413654;-0.688438;-0.595772;,
  0.383681;-0.638529;-0.667135;,
  0.217495;-0.966044;-0.139478;,
  -0.029347;-0.456802;-0.889084;,
  0.000900;-0.148920;-0.988849;,
  -0.076575;0.127702;-0.988852;,
  0.413653;-0.688438;-0.595772;,
  0.404191;-0.672692;-0.619770;,
  0.345697;-0.575336;-0.741271;,
  0.210967;-0.351103;-0.912261;,
  0.079290;-0.132038;-0.988068;,
  -0.076687;0.127470;-0.988873;,
  0.413655;-0.688436;-0.595773;,
  0.404188;-0.672695;-0.619769;,
  0.345690;-0.575342;-0.741270;,
  0.210960;-0.351107;-0.912261;,
  0.079287;-0.132041;-0.988068;,
  -0.076684;0.127473;-0.988873;,
  0.413664;-0.688432;-0.595772;,
  0.404203;-0.672684;-0.619771;,
  0.345705;-0.575330;-0.741272;,
  0.210965;-0.351103;-0.912261;,
  0.079405;-0.131912;-0.988076;,
  -0.076461;0.127720;-0.988859;,
  0.413660;-0.688434;-0.595771;,
  0.404201;-0.672686;-0.619770;,
  0.345706;-0.575331;-0.741270;,
  0.210970;-0.351098;-0.912262;,
  0.079311;-0.132004;-0.988071;,
  -0.076648;0.127533;-0.988868;,
  0.619558;-0.634706;-0.461840;,
  0.640895;-0.513479;-0.570608;,
  0.396346;-0.412798;-0.820065;,
  0.210964;-0.351102;-0.912262;,
  0.079236;-0.132079;-0.988067;,
  -0.076784;0.127390;-0.988876;,
  -0.222064;-0.957460;0.184275;,
  0.832880;-0.297978;-0.466391;,
  0.195636;-0.422557;-0.884970;,
  0.050179;-0.392741;-0.918279;,
  -0.060918;-0.161865;-0.984931;,
  -0.145426;0.082391;-0.985932;,
  -0.166986;-0.981672;0.091851;,
  -0.146124;-0.890828;-0.430201;,
  0.192727;-0.438058;-0.878044;,
  0.220225;-0.269539;-0.937470;,
  0.254327;-0.123783;-0.959164;,
  -0.213300;0.036621;-0.976300;,
  0.323125;-0.537754;0.778723;,
  0.133327;-0.871468;0.471983;,
  0.425108;-0.707476;0.564589;,
  0.193658;-0.322292;0.926620;,
  0.112202;-0.869420;0.481165;,
  -0.005421;-0.916860;0.399172;,
  0.515049;-0.857160;0.000818;,
  -0.173996;-0.955006;-0.240186;,
  0.193658;-0.322293;0.926620;,
  -0.637639;-0.451811;0.623925;,
  -0.144798;-0.493098;0.857839;,
  0.326044;0.430295;0.841749;,
  0.186650;0.573234;0.797850;,
  -0.065656;-0.974251;-0.215693;,
  -0.461757;-0.590699;0.661707;,
  -0.144796;-0.493096;0.857840;,
  -0.323147;-0.874637;-0.361368;,
  0.905078;-0.409965;0.112969;,
  -0.260058;-0.949797;0.173943;,
  -0.158776;-0.854997;0.493732;,
  0.921103;-0.362576;0.141800;,
  0.893049;-0.442394;0.082168;,
  -0.247667;-0.649230;0.719139;,
  0.827515;-0.498567;0.258164;,
  -0.336797;-0.565811;0.752613;,
  0.809003;-0.372634;0.454597;,
  -0.237923;-0.621908;0.746072;,
  0.833116;-0.211412;0.511099;,
  0.892916;-0.207467;0.399572;,
  0.920490;-0.208347;0.330590;,
  -0.032472;-0.519284;-0.853985;,
  -0.018253;-0.006108;-0.999815;,
  0.323125;-0.537754;-0.778723;,
  0.425108;-0.707476;-0.564589;,
  0.518554;-0.738632;-0.430725;,
  0.054172;-0.157320;-0.986061;,
  0.597676;-0.749386;-0.284964;,
  0.328934;-0.829741;-0.450923;,
  -0.173996;-0.955006;0.240186;,
  0.193658;-0.322293;-0.926620;,
  -0.637639;-0.451811;-0.623926;,
  -0.144798;-0.493098;-0.857839;,
  0.296720;0.447007;-0.843885;,
  0.125115;0.599763;-0.790335;,
  -0.065656;-0.974251;0.215693;,
  -0.461757;-0.590699;-0.661707;,
  -0.144796;-0.493096;-0.857840;,
  -0.322845;-0.875244;0.360165;,
  0.905078;-0.409965;-0.112969;,
  -0.158805;-0.855307;-0.493185;,
  0.956634;-0.291205;0.007187;,
  -0.247667;-0.649230;-0.719139;,
  0.902647;-0.292188;-0.315998;,
  -0.336796;-0.565809;-0.752614;,
  0.781336;-0.219273;-0.584323;,
  0.872348;-0.338400;-0.352837;,
  -0.237921;-0.621905;-0.746075;,
  0.524675;-0.077528;-0.847765;,
  -0.064124;-0.787354;-0.613158;,
  0.660330;-0.031805;-0.750302;,
  0.012385;-0.908427;-0.417859;,
  0.851908;0.009503;-0.523605;;
  208;
  4;0,7,8,1;,
  4;1,8,9,2;,
  4;30,36,35,29;,
  4;2,9,10,3;,
  4;31,31,36,30;,
  4;3,10,11,4;,
  4;32,32,31,31;,
  4;4,11,12,5;,
  4;5,12,13,6;,
  4;36,39,38,35;,
  4;222,40,223,224;,
  4;225,41,40,222;,
  4;38,37,37,37;,
  4;223,44,226,227;,
  4;40,45,44,223;,
  4;41,46,45,40;,
  4;42,47,46,41;,
  4;43,48,47,42;,
  4;226,50,49,228;,
  4;44,51,50,226;,
  4;229,52,51,44;,
  4;46,53,230,45;,
  4;231,54,53,46;,
  4;48,55,54,47;,
  4;50,57,56,49;,
  4;51,58,57,50;,
  4;52,59,59,51;,
  4;53,60,232,230;,
  4;233,61,60,53;,
  4;234,62,61,233;,
  4;57,64,63,56;,
  4;58,65,64,57;,
  4;59,66,235,59;,
  4;236,67,237,232;,
  4;61,68,67,60;,
  4;62,69,68,61;,
  4;64,71,70,63;,
  4;65,72,71,64;,
  4;66,73,72,65;,
  4;67,74,73,237;,
  4;68,75,74,67;,
  4;69,76,75,68;,
  4;71,78,77,70;,
  4;72,79,78,71;,
  4;73,80,79,72;,
  4;74,81,80,73;,
  4;75,82,81,74;,
  4;76,83,82,75;,
  4;78,85,84,77;,
  4;79,86,85,78;,
  4;80,87,86,79;,
  4;81,88,87,80;,
  4;82,89,88,81;,
  4;83,90,89,82;,
  4;85,92,91,84;,
  4;86,93,92,85;,
  4;87,94,93,86;,
  4;88,95,94,87;,
  4;89,96,95,88;,
  4;90,97,96,89;,
  4;92,99,98,91;,
  4;93,100,99,92;,
  4;94,101,100,93;,
  4;95,102,101,94;,
  4;96,103,102,95;,
  4;97,104,103,96;,
  4;238,106,105,238;,
  4;100,107,239,99;,
  4;101,108,107,100;,
  4;102,109,108,101;,
  4;103,110,109,102;,
  4;104,111,110,103;,
  4;14,21,22,15;,
  4;106,113,112,105;,
  4;15,22,23,16;,
  4;240,114,113,106;,
  4;16,23,24,17;,
  4;108,115,114,107;,
  4;17,24,25,18;,
  4;109,116,115,108;,
  4;18,25,26,19;,
  4;110,117,116,109;,
  4;19,26,27,20;,
  4;111,118,117,110;,
  4;28,241,241,28;,
  4;242,242,243,243;,
  4;241,244,244,241;,
  4;243,243,245,245;,
  4;244,246,246,244;,
  4;245,245,115,247;,
  4;246,248,248,246;,
  4;247,115,116,249;,
  4;248,33,33,248;,
  4;249,116,250,250;,
  4;33,34,34,33;,
  4;15,8,7,14;,
  4;16,9,8,15;,
  4;17,10,9,16;,
  4;18,11,10,17;,
  4;19,12,11,18;,
  4;20,13,12,19;,
  4;251,251,250,250;,
  4;0,119,125,7;,
  4;119,120,126,125;,
  4;144,143,143,144;,
  4;120,121,127,126;,
  4;145,144,144,145;,
  4;121,122,128,127;,
  4;146,145,145,146;,
  4;122,123,129,128;,
  4;147,146,146,147;,
  4;123,124,130,129;,
  4;148,252,253,149;,
  4;150,150,150,151;,
  4;254,255,256,152;,
  4;257,254,152,153;,
  4;253,257,153,154;,
  4;149,253,154,155;,
  4;156,156,156,156;,
  4;256,258,259,157;,
  4;152,151,157,158;,
  4;153,152,158,159;,
  4;154,153,159,160;,
  4;155,154,160,161;,
  4;259,228,49,162;,
  4;157,259,162,163;,
  4;260,157,163,164;,
  4;159,158,261,165;,
  4;262,159,165,166;,
  4;161,160,166,167;,
  4;162,49,56,168;,
  4;163,162,168,169;,
  4;164,163,170,170;,
  4;165,261,263,171;,
  4;264,165,171,172;,
  4;265,264,172,173;,
  4;168,56,63,174;,
  4;169,168,174,175;,
  4;170,170,266,176;,
  4;267,263,268,177;,
  4;172,171,177,178;,
  4;173,172,178,179;,
  4;174,63,70,180;,
  4;175,174,180,181;,
  4;176,175,181,182;,
  4;177,268,182,183;,
  4;178,177,183,184;,
  4;179,178,184,185;,
  4;180,70,77,186;,
  4;181,180,186,187;,
  4;182,181,187,188;,
  4;183,182,188,189;,
  4;184,183,189,190;,
  4;185,184,190,191;,
  4;186,77,84,192;,
  4;187,186,192,193;,
  4;188,187,193,194;,
  4;189,188,194,195;,
  4;190,189,195,196;,
  4;191,190,196,197;,
  4;192,84,91,198;,
  4;193,192,198,199;,
  4;194,193,199,200;,
  4;195,194,200,201;,
  4;196,195,201,202;,
  4;197,196,202,203;,
  4;198,91,98,204;,
  4;199,198,204,205;,
  4;200,199,205,206;,
  4;201,200,206,207;,
  4;202,201,207,208;,
  4;203,202,208,209;,
  4;269,98,105,210;,
  4;205,204,270,211;,
  4;206,205,211,212;,
  4;207,206,212,213;,
  4;208,207,213,214;,
  4;209,208,214,215;,
  4;14,131,137,21;,
  4;210,105,112,216;,
  4;131,132,138,137;,
  4;217,210,216,217;,
  4;132,133,139,138;,
  4;212,217,217,218;,
  4;133,134,140,139;,
  4;213,212,218,219;,
  4;134,135,141,140;,
  4;214,213,219,220;,
  4;135,136,142,141;,
  4;215,214,220,221;,
  4;28,28,271,271;,
  4;242,272,272,242;,
  4;271,271,273,273;,
  4;272,274,274,272;,
  4;273,273,275,275;,
  4;274,276,277,274;,
  4;275,275,278,278;,
  4;276,279,219,218;,
  4;278,278,252,280;,
  4;279,281,220,219;,
  4;280,252,282,282;,
  4;131,14,7,125;,
  4;132,131,125,126;,
  4;133,132,126,127;,
  4;134,133,127,128;,
  4;135,134,128,129;,
  4;136,135,129,130;,
  4;283,220,281,283;;
 }
 MeshTextureCoords {
  231;
  0.199276;1.457092;,
  0.347333;1.392392;,
  0.364941;1.401293;,
  0.219743;1.471745;,
  0.408872;1.489745;,
  0.290809;1.549686;,
  0.456028;1.545900;,
  0.439570;1.530159;,
  0.482001;1.491538;,
  0.437764;1.511065;,
  0.460933;1.558285;,
  0.397126;1.626478;,
  0.475194;1.616602;,
  0.462409;1.554485;,
  0.509972;1.617101;,
  0.474833;1.673800;,
  0.541553;1.676714;,
  0.516688;1.625943;,
  0.676669;1.672412;,
  0.669275;1.733979;,
  0.775286;1.724957;,
  0.775123;1.725035;,
  0.456829;1.521956;,
  0.443186;1.507825;,
  0.479668;1.546281;,
  0.533947;1.617739;,
  0.460446;1.499622;,
  0.445245;1.477697;,
  0.484059;1.461409;,
  0.465072;1.525862;,
  0.496928;1.538078;,
  0.551207;1.609537;,
  0.612493;1.684457;,
  0.629754;1.676255;,
  0.767833;1.724879;,
  0.767997;1.724801;,
  0.477704;1.491418;,
  0.462505;1.469494;,
  0.485454;1.547483;,
  0.514187;1.529876;,
  0.568465;1.601333;,
  0.616815;1.668051;,
  0.768161;1.724723;,
  0.494964;1.483216;,
  0.479763;1.461290;,
  0.518399;1.539961;,
  0.531446;1.521672;,
  0.585725;1.593130;,
  0.634074;1.659847;,
  0.768324;1.724645;,
  0.512223;1.475013;,
  0.497023;1.453088;,
  0.535659;1.531758;,
  0.567515;1.543975;,
  0.621793;1.615433;,
  0.651334;1.651645;,
  0.768489;1.724568;,
  0.529482;1.466810;,
  0.514281;1.444884;,
  0.552917;1.523554;,
  0.584773;1.535771;,
  0.639052;1.607229;,
  0.668592;1.643441;,
  0.768652;1.724491;,
  0.546741;1.458606;,
  0.531541;1.436681;,
  0.570177;1.515352;,
  0.602033;1.527568;,
  0.656311;1.599026;,
  0.685851;1.635238;,
  0.768816;1.724412;,
  0.563999;1.450403;,
  0.548800;1.428478;,
  0.587435;1.507149;,
  0.619291;1.519365;,
  0.673571;1.590824;,
  0.703111;1.627035;,
  0.768978;1.724334;,
  0.581259;1.442199;,
  0.566058;1.420274;,
  0.604695;1.498945;,
  0.636550;1.511161;,
  0.690829;1.582620;,
  0.720370;1.618832;,
  0.769143;1.724257;,
  0.598518;1.433996;,
  0.583318;1.412071;,
  0.621954;1.490741;,
  0.653809;1.502959;,
  0.708088;1.574417;,
  0.737629;1.610628;,
  0.769306;1.724178;,
  0.615777;1.425793;,
  0.614267;1.461361;,
  0.607864;1.431695;,
  0.671067;1.494756;,
  0.725348;1.566213;,
  0.754888;1.602425;,
  0.769470;1.724101;,
  0.561101;1.351898;,
  0.872253;1.358397;,
  0.873169;1.370491;,
  0.578533;1.376763;,
  0.812686;1.467399;,
  0.825529;1.458406;,
  0.883790;1.402097;,
  0.598721;1.399511;,
  0.809816;1.483263;,
  0.873263;1.474218;,
  0.650783;1.468050;,
  0.807395;1.500333;,
  0.855371;1.535912;,
  0.699822;1.526865;,
  0.807863;1.563157;,
  0.829385;1.583776;,
  0.745553;1.582178;,
  0.810154;1.600636;,
  0.777250;1.724023;,
  0.777086;1.724101;,
  0.769633;1.724023;,
  0.468319;1.479275;,
  0.708191;1.715785;,
  0.767506;1.725035;,
  0.219743;1.471745;,
  0.364941;1.401293;,
  0.290809;1.549686;,
  0.408872;1.489745;,
  0.456028;1.545900;,
  0.437764;1.511065;,
  0.482001;1.491538;,
  0.439570;1.530159;,
  0.397126;1.626478;,
  0.460933;1.558285;,
  0.475194;1.616602;,
  0.462409;1.554485;,
  0.474833;1.673800;,
  0.509972;1.617101;,
  0.541553;1.676714;,
  0.516688;1.625943;,
  0.669275;1.733979;,
  0.676669;1.672412;,
  0.708191;1.715785;,
  0.565038;1.692660;,
  0.852499;1.725035;,
  0.852661;1.724957;,
  0.852499;1.725035;,
  0.852661;1.724957;,
  0.462993;1.507825;,
  0.456829;1.521956;,
  0.479668;1.546281;,
  0.533947;1.617739;,
  0.612493;1.684457;,
  0.852825;1.724879;,
  0.460446;1.499622;,
  0.465072;1.525862;,
  0.496928;1.538078;,
  0.551207;1.609537;,
  0.629754;1.676255;,
  0.852989;1.724801;,
  0.477704;1.491418;,
  0.485454;1.547483;,
  0.514187;1.529876;,
  0.568465;1.601333;,
  0.616815;1.668051;,
  0.853153;1.724723;,
  0.494964;1.483216;,
  0.518399;1.539961;,
  0.531446;1.521672;,
  0.585725;1.593130;,
  0.634074;1.659847;,
  0.853316;1.724645;,
  0.512223;1.475013;,
  0.535659;1.531758;,
  0.567515;1.543975;,
  0.621793;1.615433;,
  0.651334;1.651645;,
  0.853480;1.724568;,
  0.529482;1.466810;,
  0.552917;1.523554;,
  0.584773;1.535771;,
  0.639052;1.607229;,
  0.668592;1.643441;,
  0.853643;1.724491;,
  0.546741;1.458606;,
  0.570177;1.515352;,
  0.602033;1.527568;,
  0.656311;1.599026;,
  0.685851;1.635238;,
  0.853808;1.724412;,
  0.563999;1.450403;,
  0.587435;1.507149;,
  0.619291;1.519365;,
  0.673571;1.590824;,
  0.703111;1.627035;,
  0.853971;1.724334;,
  0.581259;1.442199;,
  0.604695;1.498945;,
  0.636550;1.511161;,
  0.690829;1.582620;,
  0.720370;1.618832;,
  0.854135;1.724257;,
  0.598518;1.433996;,
  0.621954;1.490741;,
  0.653809;1.502959;,
  0.708088;1.574417;,
  0.737629;1.610628;,
  0.854299;1.724178;,
  0.615777;1.425793;,
  0.607864;1.431695;,
  0.671067;1.494756;,
  0.725348;1.566213;,
  0.754888;1.602425;,
  0.854461;1.724101;,
  0.578533;1.376763;,
  0.835866;1.370491;,
  0.801460;1.463528;,
  0.598721;1.399511;,
  0.854879;1.393991;,
  0.821939;1.465255;,
  0.650783;1.468050;,
  0.888392;1.413218;,
  0.812046;1.491126;,
  0.699822;1.526865;,
  0.918175;1.475752;,
  0.829149;1.554437;,
  0.745553;1.582178;,
  0.949965;1.573974;,
  0.834394;1.589349;,
  0.854461;1.724101;,
  0.854626;1.724023;,
  0.854626;1.724023;;
 }
}
