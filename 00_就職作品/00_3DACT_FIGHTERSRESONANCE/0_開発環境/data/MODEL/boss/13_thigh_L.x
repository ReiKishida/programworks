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
 112;
 -3.17051;-3.67672;-3.85557;,
 -3.10420;-4.84854;-4.01480;,
 -3.26394;-4.93052;-1.45114;,
 -3.23533;-3.75734;-1.44525;,
 3.98680;0.91460;-1.24126;,
 4.83851;-2.39884;-1.23783;,
 4.12796;-2.51839;-3.33636;,
 3.31519;0.80160;-3.22483;,
 -3.26929;-0.30631;-3.71560;,
 -3.64475;-0.36948;-1.43731;,
 2.60409;-2.77480;-5.42800;,
 1.87479;0.55924;-5.20189;,
 -2.83812;-3.69050;0.96507;,
 -3.26929;-0.30631;0.84097;,
 -0.13832;0.22051;-5.68853;,
 0.47431;-3.13316;-5.94284;,
 -1.26268;-3.42542;-6.26589;,
 -1.46359;-0.00248;-5.88327;,
 -1.78016;-0.05575;3.16690;,
 -1.26268;-3.42542;3.42580;,
 0.47430;-3.13316;3.24836;,
 -0.13833;0.22051;2.99917;,
 -2.37056;-3.61183;3.37540;,
 -2.82735;-0.23195;3.11925;,
 2.60409;-2.77480;2.96613;,
 1.87479;0.55923;2.73241;,
 -2.37056;-3.61183;-6.26589;,
 -2.55905;-0.18680;-5.17118;,
 4.12796;-2.51839;0.86070;,
 3.31518;0.80160;0.74232;,
 -1.16580;-4.57749;3.72978;,
 -0.56671;-8.14790;3.73500;,
 1.28268;-7.83673;3.54608;,
 0.68169;-4.26663;3.54105;,
 4.56778;-3.61276;-3.46256;,
 2.94696;-3.88548;-5.68726;,
 -2.84146;-4.85944;1.11251;,
 0.68169;-4.26663;-6.23486;,
 -1.16579;-4.57749;-6.57846;,
 -2.34415;-4.77576;3.67617;,
 2.94696;-3.88548;3.24087;,
 -2.34415;-4.77576;-6.57846;,
 4.56778;-3.61276;1.00150;,
 5.32353;-3.48560;-1.23053;,
 2.17142;-13.10184;3.59586;,
 4.46207;-12.71642;3.29232;,
 3.55028;-7.45518;3.24559;,
 -2.24409;-8.43014;1.11505;,
 -1.74627;-8.34638;3.68134;,
 -0.56670;-8.14790;-6.58383;,
 -1.74627;-8.34638;-6.58383;,
 3.55028;-7.45518;-5.69171;,
 1.28268;-7.83673;-6.23987;,
 5.17276;-7.18218;1.00392;,
 5.92928;-7.05489;-1.23040;,
 -2.63545;-8.41392;-4.01754;,
 5.17276;-7.18218;-3.46473;,
 -2.66700;-8.50130;-1.45125;,
 -0.88831;-13.61668;3.73250;,
 -0.33194;-16.38961;3.10374;,
 0.71228;-16.21391;3.15125;,
 0.30324;-13.41618;3.78671;,
 0.30325;-13.41618;-6.63699;,
 -0.88831;-13.61667;-6.63699;,
 4.46207;-12.71642;-5.73581;,
 2.17143;-13.10184;-6.28954;,
 6.10104;-12.44065;1.02787;,
 6.86525;-12.31206;-1.22916;,
 -1.81288;-13.68381;-4.04462;,
 6.10104;-12.44065;-3.48620;,
 -1.81840;-13.77317;-1.45225;,
 -1.39119;-13.70129;1.14012;,
 4.35688;-15.60067;-5.19384;,
 4.02338;-16.61859;-4.44988;,
 2.40476;-16.89094;-4.84116;,
 2.34947;-15.93844;-5.67910;,
 4.35688;-15.60067;2.71799;,
 5.79319;-15.35900;0.73353;,
 6.34058;-15.26689;-1.24442;,
 0.21658;-16.29732;-5.98359;,
 -0.77264;-16.46376;-3.71176;,
 5.79320;-15.35900;-3.22238;,
 -1.14703;-16.52676;-1.43993;,
 -0.77264;-16.46376;0.83191;,
 0.71228;-16.21391;-5.98359;,
 2.34947;-15.93844;2.98400;,
 5.18152;-16.42372;0.32950;,
 4.49969;-17.17063;-0.11888;,
 4.82341;-17.11616;-1.28862;,
 5.62289;-16.34946;-1.26537;,
 0.24267;-17.25473;2.24066;,
 1.08465;-17.11306;2.27897;,
 4.02338;-16.61859;1.92962;,
 0.68495;-17.18031;-5.08668;,
 -0.11268;-17.31452;-3.25484;,
 5.18152;-16.42372;-2.86025;,
 -0.41456;-17.36532;-1.42301;,
 -0.11268;-17.31452;0.40882;,
 1.08465;-17.11306;-5.08668;,
 2.40475;-16.89094;2.14411;,
 0.39534;-17.86122;-1.40424;,
 4.49970;-17.17063;-2.45836;,
 1.20176;-17.72554;-4.09130;,
 0.61675;-17.82397;-2.74777;,
 3.65028;-17.31355;-3.62425;,
 0.61675;-17.82397;-0.06071;,
 2.46312;-17.51330;-3.91123;,
 1.49491;-17.67621;-4.09130;,
 1.49491;-17.67621;1.31092;,
 2.46312;-17.51330;1.21201;,
 0.87737;-17.78012;1.28283;,
 3.65027;-17.31355;1.05470;;
 
 122;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,0,3,9;,
 4;7,6,10,11;,
 4;9,3,12,13;,
 4;14,15,16,17;,
 4;18,19,20,21;,
 4;13,12,22,23;,
 4;21,20,24,25;,
 4;17,16,26,27;,
 4;11,10,15,14;,
 4;23,22,19,18;,
 4;25,24,28,29;,
 4;29,28,5,4;,
 4;27,26,0,8;,
 4;30,31,32,33;,
 4;6,34,35,10;,
 4;3,2,36,12;,
 4;15,37,38,16;,
 4;19,30,33,20;,
 4;12,36,39,22;,
 4;20,33,40,24;,
 4;16,38,41,26;,
 4;10,35,37,15;,
 4;22,39,30,19;,
 4;24,40,42,28;,
 4;28,42,43,5;,
 4;26,41,1,0;,
 4;5,43,34,6;,
 4;32,44,45,46;,
 4;36,47,48,39;,
 4;33,32,46,40;,
 4;38,49,50,41;,
 4;35,51,52,37;,
 4;39,48,31,30;,
 4;40,46,53,42;,
 4;42,53,54,43;,
 4;41,50,55,1;,
 4;43,54,56,34;,
 4;1,55,57,2;,
 4;34,56,51,35;,
 4;2,57,47,36;,
 4;37,52,49,38;,
 4;58,59,60,61;,
 4;49,62,63,50;,
 4;51,64,65,52;,
 4;48,58,61,31;,
 4;46,45,66,53;,
 4;53,66,67,54;,
 4;50,63,68,55;,
 4;54,67,69,56;,
 4;55,68,70,57;,
 4;56,69,64,51;,
 4;57,70,71,47;,
 4;52,65,62,49;,
 4;31,61,44,32;,
 4;47,71,58,48;,
 4;72,73,74,75;,
 4;45,76,77,66;,
 4;66,77,78,67;,
 4;63,79,80,68;,
 4;67,78,81,69;,
 4;68,80,82,70;,
 4;69,81,72,64;,
 4;70,82,83,71;,
 4;65,75,84,62;,
 4;61,60,85,44;,
 4;71,83,59,58;,
 4;44,85,76,45;,
 4;62,84,79,63;,
 4;64,72,75,65;,
 3;21,29,13;,
 4;86,87,88,89;,
 4;59,90,91,60;,
 4;76,92,86,77;,
 4;77,86,89,78;,
 4;79,93,94,80;,
 4;78,89,95,81;,
 4;80,94,96,82;,
 4;81,95,73,72;,
 4;82,96,97,83;,
 4;75,74,98,84;,
 4;60,91,99,85;,
 4;83,97,90,59;,
 4;85,99,92,76;,
 4;84,98,93,79;,
 3;87,100,101;,
 4;93,102,103,94;,
 4;89,88,101,95;,
 4;94,103,100,96;,
 4;95,101,104,73;,
 4;96,100,105,97;,
 4;74,106,107,98;,
 4;91,108,109,99;,
 4;97,105,110,90;,
 4;99,109,111,92;,
 4;98,107,102,93;,
 4;73,104,106,74;,
 4;90,110,108,91;,
 4;92,111,87,86;,
 3;18,13,23;,
 3;13,29,9;,
 3;9,14,8;,
 3;29,7,9;,
 3;8,14,27;,
 3;27,14,17;,
 3;14,7,11;,
 3;7,29,4;,
 3;29,21,25;,
 3;9,7,14;,
 3;21,13,18;,
 3;105,108,110;,
 3;108,105,109;,
 3;109,87,111;,
 3;105,87,109;,
 3;87,101,88;,
 3;105,100,87;,
 3;101,106,104;,
 3;100,103,106;,
 3;106,103,107;,
 3;107,103,102;,
 3;106,101,100;;
 
 MeshMaterialList {
  1;
  122;
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
   0.537000;0.537000;0.537000;1.000000;;
   0.030000;
   1.000000;1.000000;1.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\character\\boss_ex.png";
   }
  }
 }
 MeshNormals {
  136;
  -0.683416;0.115145;-0.720891;,
  -0.974231;-0.117055;0.192802;,
  0.968516;0.248952;-0.000008;,
  0.857005;0.225373;-0.463410;,
  -0.086688;0.153050;-0.984409;,
  0.014827;0.078863;0.996775;,
  0.100731;0.092009;0.990650;,
  0.528486;0.171983;-0.831339;,
  0.180523;0.110868;-0.977302;,
  0.486063;0.166466;0.857923;,
  0.857502;0.226402;0.461988;,
  -0.970423;-0.025912;-0.240014;,
  -0.976805;-0.117898;0.178749;,
  -0.994817;-0.095830;0.034003;,
  -0.982981;-0.046579;0.177703;,
  -0.980416;-0.045291;0.191661;,
  -0.020400;0.210538;-0.977373;,
  -0.649667;0.141918;-0.746855;,
  -0.000264;0.167059;0.985947;,
  0.944144;0.329532;-0.000083;,
  0.835215;0.305401;0.457325;,
  0.835478;0.302203;-0.458967;,
  0.508423;0.256062;-0.822155;,
  0.169276;0.192721;-0.966542;,
  0.085185;0.177302;0.980463;,
  0.465167;0.255199;0.847640;,
  -0.978324;-0.010419;-0.206817;,
  -0.997905;-0.044972;0.046511;,
  -0.981591;-0.068924;0.178128;,
  -0.979023;-0.067834;0.192129;,
  0.067305;0.141925;-0.987587;,
  -0.022036;0.130962;-0.991142;,
  0.005950;0.131019;0.991362;,
  0.957049;0.289927;-0.000080;,
  0.846731;0.267362;0.459961;,
  0.846760;0.264560;-0.461525;,
  0.518276;0.217417;-0.827115;,
  0.181346;0.155552;-0.971039;,
  0.091741;0.141924;0.985617;,
  0.475082;0.215873;0.853051;,
  -0.982055;-0.070242;-0.175025;,
  -0.995321;-0.075292;0.060551;,
  -0.970884;-0.159270;0.178936;,
  -0.968261;-0.158800;0.193011;,
  0.088396;0.020489;-0.995875;,
  -0.000949;0.005640;-0.999984;,
  0.026480;0.010045;0.999599;,
  0.985219;0.171302;0.000000;,
  0.872923;0.152235;0.463498;,
  0.872225;0.151992;-0.464888;,
  0.543014;0.096955;-0.834107;,
  0.202475;0.039433;-0.978493;,
  0.112433;0.024348;0.993361;,
  0.500784;0.090103;0.860870;,
  -0.973199;-0.149237;-0.174962;,
  -0.985238;-0.156596;0.069169;,
  -0.964306;-0.195270;0.178842;,
  -0.026549;-0.110837;0.993484;,
  0.106873;-0.092577;-0.989953;,
  0.017985;-0.106883;-0.994109;,
  0.044791;-0.099766;0.994002;,
  0.999945;0.010447;-0.000069;,
  0.891568;0.004663;0.452862;,
  0.890766;0.006696;-0.454413;,
  0.559954;-0.036152;-0.827735;,
  0.220305;-0.074025;-0.972618;,
  0.130387;-0.086311;0.987699;,
  0.517946;-0.045946;0.854179;,
  -0.946556;-0.252572;-0.200597;,
  -0.975619;-0.211879;0.057225;,
  -0.873637;-0.455756;0.170428;,
  0.038138;-0.465726;0.884106;,
  0.156988;-0.447493;-0.880401;,
  -0.523035;-0.663807;-0.534597;,
  0.101279;-0.451993;0.886254;,
  0.931764;-0.363066;-0.000383;,
  0.847941;-0.363306;0.386011;,
  0.849436;-0.356414;-0.389136;,
  0.554233;-0.405987;-0.726636;,
  0.256850;-0.422517;-0.869199;,
  0.176744;-0.434806;0.883009;,
  0.514897;-0.425093;0.744431;,
  -0.820591;-0.520566;-0.235885;,
  -0.880453;-0.473735;0.019442;,
  -0.636739;-0.758799;0.137067;,
  -0.296123;-0.848531;0.438527;,
  0.185537;-0.743864;-0.642061;,
  -0.225216;-0.835026;-0.502006;,
  0.144833;-0.748763;0.646821;,
  0.767208;-0.641398;-0.000682;,
  0.704777;-0.649423;0.285553;,
  0.709977;-0.641540;-0.290447;,
  0.476949;-0.698284;-0.533778;,
  0.258888;-0.718450;-0.645605;,
  0.199564;-0.731376;0.652122;,
  0.443160;-0.717172;0.537842;,
  -0.595056;-0.775274;-0.211799;,
  -0.644841;-0.764317;0.000230;,
  -0.065203;-0.996998;0.041767;,
  -0.083053;-0.967235;0.239915;,
  0.185110;-0.941357;-0.282100;,
  -0.044336;-0.959277;-0.278966;,
  0.167239;-0.944099;0.284091;,
  0.536489;-0.843907;-0.000517;,
  0.311429;-0.947478;0.072790;,
  0.338378;-0.936969;-0.087118;,
  0.359784;-0.879115;-0.312589;,
  0.202354;-0.960609;-0.190481;,
  0.187704;-0.954771;0.230606;,
  0.337661;-0.888256;0.311426;,
  -0.095613;-0.992481;-0.076413;,
  -0.066246;-0.997803;0.000095;,
  -0.056731;0.067821;0.996083;,
  -0.071442;0.158314;0.984801;,
  -0.065543;0.121755;0.990394;,
  -0.950923;-0.059480;-0.303657;,
  -0.045131;-0.001879;0.998979;,
  -0.936909;-0.143152;-0.318918;,
  -0.897645;-0.273010;-0.345976;,
  -0.961548;-0.195482;0.192905;,
  -0.870016;-0.457521;0.183703;,
  0.037369;-0.222084;-0.974311;,
  -0.165927;0.986138;0.000003;,
  -0.165925;0.986138;0.000002;,
  -0.165929;0.986138;0.000001;,
  -0.165929;0.986138;0.000001;,
  -0.165927;0.986138;0.000000;,
  -0.165929;0.986138;-0.000000;,
  -0.165928;0.986138;0.000000;,
  -0.165928;0.986138;0.000001;,
  -0.165926;0.986138;0.000000;,
  -0.165927;0.986138;0.000001;,
  -0.165927;0.986138;-0.000002;,
  -0.165930;0.986138;0.000003;,
  -0.165919;0.986139;-0.000000;,
  -0.165923;0.986139;0.000008;;
  122;
  4;26,40,41,27;,
  4;2,19,21,3;,
  4;11,26,27,13;,
  4;3,21,22,7;,
  4;13,27,14,12;,
  4;8,23,16,4;,
  4;5,18,24,6;,
  4;12,14,15,1;,
  4;6,24,25,9;,
  4;4,16,17,0;,
  4;7,22,23,8;,
  4;112,113,18,5;,
  4;9,25,20,10;,
  4;10,20,19,2;,
  4;0,17,26,11;,
  4;32,46,52,38;,
  4;21,35,36,22;,
  4;27,41,28,14;,
  4;23,37,30,16;,
  4;18,32,38,24;,
  4;14,28,29,15;,
  4;24,38,39,25;,
  4;16,30,31,17;,
  4;22,36,37,23;,
  4;113,114,32,18;,
  4;25,39,34,20;,
  4;20,34,33,19;,
  4;17,115,40,26;,
  4;19,33,35,21;,
  4;52,66,67,53;,
  4;28,42,43,29;,
  4;38,52,53,39;,
  4;30,44,45,31;,
  4;36,50,51,37;,
  4;114,116,46,32;,
  4;39,53,48,34;,
  4;34,48,47,33;,
  4;115,117,54,40;,
  4;33,47,49,35;,
  4;40,54,55,41;,
  4;35,49,50,36;,
  4;41,55,42,28;,
  4;37,51,44,30;,
  4;57,71,74,60;,
  4;44,58,59,45;,
  4;50,64,65,51;,
  4;116,57,60,46;,
  4;53,67,62,48;,
  4;48,62,61,47;,
  4;117,118,68,54;,
  4;47,61,63,49;,
  4;54,68,69,55;,
  4;49,63,64,50;,
  4;55,69,56,42;,
  4;51,65,58,44;,
  4;46,60,66,52;,
  4;42,56,119,43;,
  4;78,92,93,79;,
  4;67,81,76,62;,
  4;62,76,75,61;,
  4;118,73,82,68;,
  4;61,75,77,63;,
  4;68,82,83,69;,
  4;63,77,78,64;,
  4;69,83,70,56;,
  4;65,79,72,58;,
  4;60,74,80,66;,
  4;56,70,120,119;,
  4;66,80,81,67;,
  4;58,72,121,59;,
  4;64,78,79,65;,
  3;122,123,124;,
  4;90,104,103,89;,
  4;71,85,88,74;,
  4;81,95,90,76;,
  4;76,90,89,75;,
  4;73,87,96,82;,
  4;75,89,91,77;,
  4;82,96,97,83;,
  4;77,91,92,78;,
  4;83,97,84,70;,
  4;79,93,86,72;,
  4;74,88,94,80;,
  4;70,84,85,120;,
  4;80,94,95,81;,
  4;72,86,87,73;,
  3;104,111,105;,
  4;87,101,110,96;,
  4;89,103,105,91;,
  4;96,110,111,97;,
  4;91,105,106,92;,
  4;97,111,98,84;,
  4;93,107,100,86;,
  4;88,102,108,94;,
  4;84,98,99,85;,
  4;94,108,109,95;,
  4;86,100,101,87;,
  4;92,106,107,93;,
  4;85,99,102,88;,
  4;95,109,104,90;,
  3;125,124,126;,
  3;124,123,127;,
  3;127,128,129;,
  3;123,130,127;,
  3;129,128,131;,
  3;131,128,132;,
  3;128,130,133;,
  3;130,123,134;,
  3;123,122,135;,
  3;127,130,128;,
  3;122,124,125;,
  3;98,102,99;,
  3;102,98,108;,
  3;108,104,109;,
  3;98,104,108;,
  3;104,105,103;,
  3;98,111,104;,
  3;105,107,106;,
  3;111,110,107;,
  3;107,110,100;,
  3;100,110,101;,
  3;107,105,111;;
 }
 MeshTextureCoords {
  112;
  0.538760;0.119980;,
  0.538760;0.119980;,
  0.621010;0.252790;,
  0.621010;0.252790;,
  0.529200;0.126160;,
  0.529200;0.126160;,
  0.533340;0.128490;,
  0.533340;0.128490;,
  0.538760;0.119980;,
  0.621010;0.252790;,
  0.535760;0.128660;,
  0.535760;0.128660;,
  0.518460;0.126270;,
  0.518460;0.126270;,
  0.537940;0.129120;,
  0.537940;0.129120;,
  0.539490;0.128980;,
  0.539490;0.128980;,
  0.531350;0.133170;,
  0.531350;0.133170;,
  0.531820;0.131320;,
  0.531820;0.131320;,
  0.531210;0.134730;,
  0.531210;0.134730;,
  0.531420;0.130570;,
  0.531420;0.130570;,
  0.540890;0.129150;,
  0.540890;0.129150;,
  0.530410;0.128880;,
  0.530410;0.128880;,
  0.531350;0.133170;,
  0.531350;0.133170;,
  0.531820;0.131320;,
  0.531820;0.131320;,
  0.533340;0.128490;,
  0.535760;0.128660;,
  0.518460;0.126270;,
  0.537940;0.129120;,
  0.539490;0.128980;,
  0.531210;0.134730;,
  0.531420;0.130570;,
  0.540890;0.129150;,
  0.530410;0.128880;,
  0.529200;0.126160;,
  0.531820;0.131320;,
  0.531420;0.130570;,
  0.531420;0.130570;,
  0.518460;0.126270;,
  0.531210;0.134730;,
  0.539490;0.128980;,
  0.540890;0.129150;,
  0.535760;0.128660;,
  0.537940;0.129120;,
  0.530410;0.128880;,
  0.529200;0.126160;,
  0.538760;0.119980;,
  0.533340;0.128490;,
  0.621010;0.252790;,
  0.531210;0.134730;,
  0.531210;0.134730;,
  0.531350;0.133170;,
  0.531350;0.133170;,
  0.539490;0.128980;,
  0.540890;0.129150;,
  0.535760;0.128660;,
  0.537940;0.129120;,
  0.530410;0.128880;,
  0.529200;0.126160;,
  0.538760;0.119980;,
  0.533340;0.128490;,
  0.621010;0.252790;,
  0.518460;0.126270;,
  0.535760;0.128660;,
  0.535760;0.128660;,
  0.537940;0.129120;,
  0.537940;0.129120;,
  0.531420;0.130570;,
  0.530410;0.128880;,
  0.529200;0.126160;,
  0.540890;0.129150;,
  0.538760;0.119980;,
  0.533340;0.128490;,
  0.621010;0.252790;,
  0.518460;0.126270;,
  0.539490;0.128980;,
  0.531820;0.131320;,
  0.530410;0.128880;,
  0.530410;0.128880;,
  0.529200;0.126160;,
  0.529200;0.126160;,
  0.531210;0.134730;,
  0.531350;0.133170;,
  0.531420;0.130570;,
  0.540890;0.129150;,
  0.538760;0.119980;,
  0.533340;0.128490;,
  0.621010;0.252790;,
  0.518460;0.126270;,
  0.539490;0.128980;,
  0.531820;0.131320;,
  0.621010;0.252790;,
  0.533340;0.128490;,
  0.540890;0.129150;,
  0.538760;0.119980;,
  0.535760;0.128660;,
  0.518460;0.126270;,
  0.537940;0.129120;,
  0.539490;0.128980;,
  0.531350;0.133170;,
  0.531820;0.131320;,
  0.531210;0.134730;,
  0.531420;0.130570;;
 }
}
