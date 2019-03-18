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
 114;
 -0.50660;1.60258;1.22999;,
 0.15656;1.62234;1.11595;,
 -0.27866;1.82996;0.54984;,
 -0.12444;-1.12928;2.06225;,
 -0.44141;-1.76888;1.55986;,
 0.13403;-1.78749;1.47770;,
 -0.48499;0.76879;2.04784;,
 0.44101;0.48595;2.03871;,
 -0.38756;1.13191;1.84343;,
 -0.47466;-2.15974;0.83270;,
 0.24394;-2.06984;0.86938;,
 -0.04791;-0.25941;2.28617;,
 0.61836;-0.78704;1.97996;,
 0.78484;0.00214;1.98955;,
 0.44920;1.09997;1.65701;,
 1.36854;-0.46706;1.44028;,
 0.52133;-1.48910;1.62442;,
 0.81269;-1.80338;0.89162;,
 1.08228;-1.14233;1.43908;,
 1.22904;0.43208;1.47432;,
 1.62148;0.22421;0.89895;,
 0.10781;1.79985;0.30360;,
 0.62405;1.61103;0.62900;,
 1.41154;-1.16888;0.83758;,
 0.92336;1.09997;1.27705;,
 1.25549;1.10569;0.60611;,
 1.12470;-1.64688;0.30071;,
 0.60984;-2.01548;0.14339;,
 1.71827;-0.39536;0.50526;,
 1.64461;0.39962;0.12723;,
 -0.11435;-2.21789;0.46501;,
 -0.05610;-2.19149;0.00657;,
 1.69917;-0.39702;-0.07281;,
 0.21270;1.69139;-0.25379;,
 1.49174;-1.15672;0.15081;,
 1.44281;-0.66689;-0.64222;,
 0.94744;-1.52384;-0.59804;,
 1.15445;1.13526;-0.21231;,
 1.42166;0.39962;-0.57812;,
 0.27013;-1.99970;-0.43029;,
 0.69181;1.50862;-0.18460;,
 0.23119;1.39970;-0.82968;,
 0.84239;0.06201;-1.31790;,
 1.02473;0.81266;-0.81970;,
 -0.42705;1.76510;-0.22997;,
 1.08937;-0.78965;-1.01547;,
 0.45504;-0.40449;-1.51650;,
 0.47380;-1.20059;-1.22386;,
 0.56784;0.75440;-1.21516;,
 -0.09268;-1.79205;-0.90759;,
 -0.10717;0.78094;-1.40976;,
 -0.12007;-1.15672;-1.41614;,
 -0.54804;-1.63742;-1.05368;,
 -0.60382;-2.07382;-0.38680;,
 -0.14177;0.20440;-1.60548;,
 -0.28352;-0.38994;-1.65853;,
 -0.78158;-0.81876;-1.50405;,
 -1.03898;1.42583;-0.69179;,
 -1.05579;-1.23027;-1.22923;,
 -1.03018;-1.79305;-0.69045;,
 -0.66544;-2.18473;0.12849;,
 -1.01563;0.39325;-1.45832;,
 -1.27740;-0.37836;-1.41388;,
 -0.77030;1.17583;-1.08897;,
 -1.44143;0.76879;-1.03349;,
 -1.85780;0.18881;-0.89998;,
 -0.70474;1.79925;0.39312;,
 -1.68732;-1.14233;-0.80739;,
 -1.71745;-1.48678;-0.35538;,
 -1.97767;0.70818;-0.42620;,
 -2.25212;0.02392;-0.23831;,
 -1.88417;-0.41185;-0.91677;,
 -2.17672;-0.77238;-0.24790;,
 -1.82357;1.09934;-0.09453;,
 -1.15543;1.63128;-0.02780;,
 -2.07232;-1.20059;0.21231;,
 -2.07807;0.78994;0.63838;,
 -1.06731;1.61781;0.86821;,
 -1.80324;-1.48727;0.79087;,
 -2.26569;-0.65403;0.71372;,
 -1.57976;-1.78373;0.15348;,
 -1.16738;-2.01417;-0.01262;,
 -0.95848;-2.02528;0.94639;,
 -2.32300;0.02918;0.67489;,
 -2.09860;-0.39454;1.24187;,
 -1.59043;1.38095;0.44999;,
 -1.39097;1.27879;1.19958;,
 -1.36454;-1.78352;1.01583;,
 -1.70892;-1.18920;1.37736;,
 -1.69948;0.76879;1.41595;,
 -1.55186;-0.80325;1.76217;,
 -1.81443;0.04019;1.62322;,
 -1.06141;-1.53248;1.60820;,
 -1.15729;0.76879;1.84855;,
 -1.23214;0.00645;2.06514;,
 -0.88393;-0.59318;2.16047;,
 -0.83130;-1.15672;1.98018;,
 -0.46834;0.00645;2.27560;,
 -0.97211;2.47182;-2.04762;,
 -10.78065;3.31308;4.11928;,
 -0.79353;2.93397;3.30783;,
 -11.29297;0.66821;-3.39155;,
 -11.52298;2.69070;-2.53901;,
 -1.05325;1.92512;3.48796;,
 -10.87441;2.07982;4.34322;,
 -0.78711;0.84509;-2.73336;,
 -10.72538;-1.28736;-3.52320;,
 -10.35101;-0.09274;4.74931;,
 -1.07951;0.13657;3.81459;,
 -9.31010;-2.49454;4.74871;,
 -0.80413;-2.52841;3.81396;,
 -0.60248;-0.75284;-2.83925;,
 -9.73993;-2.75947;-3.50897;,
 -0.56702;-2.43287;-2.82391;;
 
 218;
 3;0,1,2;,
 3;3,4,5;,
 3;6,7,8;,
 3;5,9,10;,
 3;11,12,13;,
 3;1,8,14;,
 3;13,12,15;,
 3;16,5,17;,
 3;18,16,17;,
 3;19,15,20;,
 3;21,1,22;,
 3;15,18,23;,
 3;22,24,25;,
 3;17,26,23;,
 3;17,10,27;,
 3;20,15,28;,
 3;17,27,26;,
 3;25,20,29;,
 3;30,31,27;,
 3;29,28,32;,
 3;21,22,33;,
 3;32,34,35;,
 3;34,36,35;,
 3;34,26,36;,
 3;37,29,38;,
 3;27,39,36;,
 3;38,32,35;,
 3;40,37,41;,
 3;38,42,43;,
 3;38,35,42;,
 3;21,44,2;,
 3;45,46,42;,
 3;33,40,41;,
 3;36,47,45;,
 3;43,48,41;,
 3;46,45,47;,
 3;47,36,49;,
 3;41,48,50;,
 3;51,49,52;,
 3;49,39,53;,
 3;54,46,55;,
 3;55,51,56;,
 3;44,41,57;,
 3;51,58,56;,
 3;52,49,59;,
 3;31,60,53;,
 3;61,55,62;,
 3;30,60,31;,
 3;62,56,58;,
 3;58,52,59;,
 3;63,61,64;,
 3;61,65,64;,
 3;44,66,2;,
 3;67,59,68;,
 3;64,69,57;,
 3;65,70,69;,
 3;65,71,70;,
 3;71,67,72;,
 3;67,68,72;,
 3;57,73,74;,
 3;68,75,72;,
 3;73,69,76;,
 3;74,77,66;,
 3;75,78,79;,
 3;75,80,78;,
 3;81,82,80;,
 3;82,60,9;,
 3;83,79,84;,
 3;30,9,60;,
 3;85,86,77;,
 3;78,80,87;,
 3;84,79,88;,
 3;76,89,86;,
 3;84,90,91;,
 3;84,88,90;,
 3;88,92,90;,
 3;88,87,92;,
 3;89,93,86;,
 3;94,90,95;,
 3;96,92,4;,
 3;93,6,8;,
 3;92,82,4;,
 3;97,95,11;,
 3;8,1,0;,
 3;4,9,5;,
 4;24,22,1,14;,
 3;9,30,10;,
 3;11,13,7;,
 3;3,12,11;,
 3;3,16,12;,
 3;5,16,3;,
 3;8,7,14;,
 3;7,13,19;,
 3;2,1,21;,
 3;12,18,15;,
 3;16,18,12;,
 4;19,24,14,7;,
 4;20,25,24,19;,
 3;5,10,17;,
 3;15,19,13;,
 3;17,23,18;,
 4;34,32,28,23;,
 3;30,27,10;,
 3;15,23,28;,
 3;20,28,29;,
 4;37,40,22,25;,
 3;23,26,34;,
 4;7,6,97,11;,
 3;25,29,37;,
 4;50,48,42,54;,
 3;40,33,22;,
 3;26,27,36;,
 3;29,32,38;,
 3;37,38,43;,
 3;35,36,45;,
 3;37,43,41;,
 3;31,39,27;,
 3;35,45,42;,
 3;42,48,43;,
 3;21,33,44;,
 3;39,49,36;,
 4;51,55,46,47;,
 3;42,46,54;,
 4;65,61,62,71;,
 3;47,49,51;,
 3;41,44,33;,
 3;41,50,63;,
 3;31,53,39;,
 3;54,61,50;,
 3;54,55,61;,
 3;63,57,41;,
 3;51,52,58;,
 3;50,61,63;,
 3;53,59,49;,
 3;56,62,55;,
 3;58,59,67;,
 3;64,57,63;,
 4;67,71,62,58;,
 4;76,69,70,83;,
 3;44,57,74;,
 3;44,74,66;,
 3;59,81,68;,
 3;53,81,59;,
 3;65,69,64;,
 3;53,60,81;,
 3;69,73,57;,
 3;68,81,80;,
 3;71,72,70;,
 3;68,80,75;,
 4;83,70,72,79;,
 4;89,76,83,91;,
 3;72,75,79;,
 3;73,85,74;,
 3;73,76,85;,
 3;74,85,77;,
 3;81,60,82;,
 3;82,87,80;,
 3;85,76,86;,
 3;78,88,79;,
 3;78,87,88;,
 4;93,89,91,94;,
 3;83,84,91;,
 3;82,92,87;,
 4;2,66,77,0;,
 3;91,90,94;,
 4;0,77,86,8;,
 3;90,96,95;,
 3;92,96,90;,
 4;6,93,94,97;,
 3;93,8,86;,
 3;82,9,4;,
 4;3,11,95,96;,
 3;94,95,97;,
 3;96,4,3;,
 3;98,99,100;,
 3;98,101,102;,
 3;103,99,104;,
 3;105,106,101;,
 3;103,107,108;,
 3;108,109,110;,
 3;111,112,106;,
 3;109,113,110;,
 3;111,105,108;,
 3;98,102,99;,
 3;98,105,101;,
 3;103,100,99;,
 3;105,111,106;,
 3;103,104,107;,
 3;108,107,109;,
 3;111,113,112;,
 3;109,112,113;,
 3;100,103,98;,
 3;108,110,113;,
 3;113,111,108;,
 3;105,98,103;,
 3;103,108,105;,
 3;100,99,98;,
 3;102,101,98;,
 3;104,99,103;,
 3;101,106,105;,
 3;108,107,103;,
 3;110,109,108;,
 3;106,112,111;,
 3;110,113,109;,
 3;108,105,111;,
 3;99,102,98;,
 3;101,105,98;,
 3;99,100,103;,
 3;106,111,105;,
 3;107,104,103;,
 3;109,107,108;,
 3;112,113,111;,
 3;113,112,109;,
 3;98,103,100;,
 3;113,110,108;,
 3;108,111,113;,
 3;103,98,105;,
 3;105,108,103;;
 
 MeshMaterialList {
  1;
  218;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.034400;0.000000;0.138400;1.000000;;
   0.500000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  158;
  -0.038657;0.669777;0.741556;,
  0.141856;-0.063446;0.987852;,
  0.112188;-0.487936;0.865640;,
  -0.040377;-0.950859;0.306981;,
  0.200348;-0.791261;0.577726;,
  0.194536;0.889303;0.413879;,
  0.350037;0.643496;0.680726;,
  0.323067;0.364870;0.873211;,
  0.454679;-0.284887;0.843864;,
  0.261493;-0.924461;0.277478;,
  0.528517;0.062127;0.846646;,
  0.660947;-0.476436;0.579792;,
  0.406666;-0.604330;0.685133;,
  0.613383;0.627186;0.479998;,
  0.714673;0.296173;0.633660;,
  0.811378;-0.127087;0.570538;,
  0.929014;0.216260;0.300273;,
  0.843731;-0.469136;0.260824;,
  0.546787;-0.768060;0.333330;,
  0.111590;-0.991663;0.064431;,
  0.782092;0.607006;0.140984;,
  0.985206;-0.082561;0.150175;,
  0.458033;0.880016;0.125609;,
  0.951336;0.295468;-0.087511;,
  0.976258;-0.075678;-0.202963;,
  0.867193;-0.475762;-0.147062;,
  0.706769;-0.707216;-0.017957;,
  0.194800;0.980821;0.006638;,
  0.714875;0.626172;-0.311226;,
  0.848543;0.248312;-0.467243;,
  0.841158;-0.205597;-0.500183;,
  0.429860;-0.900435;-0.066610;,
  0.487729;0.831204;-0.266871;,
  0.623516;0.497943;-0.602727;,
  0.272812;0.929288;-0.248994;,
  0.626425;-0.283841;-0.725965;,
  0.603350;-0.651453;-0.459976;,
  0.408761;0.468985;-0.782922;,
  0.584908;0.140260;-0.798880;,
  0.375258;-0.487877;-0.788136;,
  0.361547;-0.106435;-0.926259;,
  0.029719;-0.483873;-0.874633;,
  0.113998;-0.766024;-0.632623;,
  0.258513;-0.883976;-0.389562;,
  0.252139;0.769093;-0.587301;,
  0.069999;0.478494;-0.875296;,
  0.112589;0.188009;-0.975693;,
  -0.040401;-0.083460;-0.995692;,
  -0.210290;-0.296776;-0.931505;,
  0.108228;-0.978632;-0.174833;,
  0.010915;0.991826;0.127128;,
  -0.320198;0.259828;-0.911023;,
  -0.129592;-0.679113;-0.722503;,
  -0.193562;0.632078;-0.750341;,
  -0.428635;-0.110417;-0.896705;,
  -0.352932;-0.494216;-0.794474;,
  -0.051247;0.962751;-0.265487;,
  -0.382534;0.774592;-0.503661;,
  -0.552576;0.479003;-0.682067;,
  -0.357641;-0.767660;-0.531780;,
  -0.110395;-0.923297;-0.367880;,
  -0.741223;0.200524;-0.640608;,
  -0.776441;-0.106930;-0.621051;,
  -0.663844;-0.473044;-0.579258;,
  -0.683980;-0.643135;-0.344309;,
  -0.419380;-0.888326;-0.187075;,
  -0.709670;0.670130;-0.217470;,
  -0.803549;0.455883;-0.382728;,
  -0.941880;0.087662;-0.324311;,
  -0.907974;-0.292174;-0.300362;,
  -0.621776;-0.783117;-0.011051;,
  -0.426395;0.892947;-0.144338;,
  -0.854653;-0.516122;-0.056447;,
  -0.631954;0.769583;0.091518;,
  -0.171036;-0.984085;-0.048208;,
  -0.846116;0.511145;0.151055;,
  -0.966437;0.128673;0.222359;,
  -0.936601;-0.288564;0.198771;,
  -0.760811;-0.605821;0.232696;,
  -0.850437;-0.128562;0.510126;,
  -0.705637;-0.480566;0.520704;,
  -0.688550;0.449450;0.569116;,
  -0.753090;0.115412;0.647716;,
  -0.543708;-0.757276;0.361820;,
  -0.206080;0.978279;0.022393;,
  -0.382683;0.890459;0.246245;,
  -0.534002;0.700028;0.474135;,
  -0.579671;-0.272382;0.767978;,
  -0.394483;0.485562;0.780136;,
  -0.452192;0.077097;0.888582;,
  -0.395063;-0.647543;0.651623;,
  -0.338148;-0.897625;0.282711;,
  -0.270947;-0.173533;0.946823;,
  -0.245846;-0.470044;0.847714;,
  -0.088906;0.889038;0.449118;,
  -0.063400;0.390606;0.918372;,
  -0.133318;0.108176;0.985152;,
  -0.088126;-0.753983;0.650956;,
  0.089129;0.176567;0.980245;,
  0.101205;0.077304;0.991858;,
  0.030733;0.995738;-0.086952;,
  0.027920;0.995424;-0.091384;,
  0.059082;0.185757;-0.980818;,
  0.067934;0.033353;-0.997132;,
  0.088147;0.187509;0.978300;,
  0.094124;0.141602;0.985439;,
  0.050913;0.392838;-0.918197;,
  0.054854;0.293413;-0.954411;,
  0.063292;0.064067;-0.995936;,
  0.109273;0.011056;0.993950;,
  0.104236;0.026897;0.994189;,
  0.013866;-0.999873;0.007866;,
  0.070171;0.013366;-0.997445;,
  0.050912;0.392844;-0.918195;,
  0.086986;0.162579;0.982854;,
  -0.005585;-0.999878;-0.014591;,
  0.995835;0.066364;0.062523;,
  0.996739;0.069083;0.041703;,
  0.997030;0.057240;0.051519;,
  0.025106;0.995083;-0.095814;,
  0.085815;0.153670;0.984389;,
  0.074740;-0.007527;-0.997175;,
  0.033305;-0.998985;0.030317;,
  0.967910;-0.251074;-0.010609;,
  0.998441;-0.052279;0.019555;,
  0.997127;-0.075410;0.007168;,
  0.994024;0.102707;0.036986;,
  0.996660;0.062252;0.052847;,
  -0.030733;-0.995738;0.086952;,
  -0.027920;-0.995424;0.091384;,
  -0.050913;-0.392838;0.918197;,
  -0.054854;-0.293413;0.954411;,
  -0.050912;-0.392844;0.918195;,
  -0.088147;-0.187509;-0.978300;,
  -0.086986;-0.162579;-0.982854;,
  -0.089129;-0.176567;-0.980245;,
  -0.063292;-0.064067;0.995936;,
  -0.059082;-0.185757;0.980818;,
  -0.101205;-0.077304;-0.991858;,
  -0.094124;-0.141602;-0.985439;,
  -0.109273;-0.011056;-0.993950;,
  -0.104236;-0.026897;-0.994189;,
  -0.070171;-0.013366;0.997445;,
  -0.067934;-0.033353;0.997132;,
  0.005585;0.999878;0.014591;,
  -0.013866;0.999873;-0.007866;,
  -0.997030;-0.057240;-0.051519;,
  -0.996739;-0.069083;-0.041703;,
  -0.995835;-0.066364;-0.062523;,
  -0.025106;-0.995083;0.095814;,
  -0.085815;-0.153670;-0.984389;,
  -0.074740;0.007527;0.997175;,
  -0.033305;0.998985;-0.030317;,
  -0.997127;0.075410;-0.007168;,
  -0.998441;0.052279;-0.019555;,
  -0.967910;0.251074;0.010609;,
  -0.996660;-0.062252;-0.052847;,
  -0.994024;-0.102707;-0.036986;;
  218;
  3;94,5,50;,
  3;2,97,4;,
  3;95,7,0;,
  3;4,3,9;,
  3;1,8,10;,
  3;5,0,6;,
  3;10,8,15;,
  3;12,4,18;,
  3;11,12,18;,
  3;14,15,16;,
  3;27,5,22;,
  3;15,11,17;,
  3;22,13,20;,
  3;18,26,17;,
  3;18,9,31;,
  3;16,15,21;,
  3;18,31,26;,
  3;20,16,23;,
  3;19,49,31;,
  3;23,21,24;,
  3;27,22,34;,
  3;24,25,30;,
  3;25,36,30;,
  3;25,26,36;,
  3;28,23,29;,
  3;31,43,36;,
  3;29,24,30;,
  3;32,28,44;,
  3;29,38,33;,
  3;29,30,38;,
  3;27,56,50;,
  3;35,40,38;,
  3;34,32,44;,
  3;36,39,35;,
  3;33,37,44;,
  3;40,35,39;,
  3;39,36,42;,
  3;44,37,45;,
  3;41,42,52;,
  3;42,43,60;,
  3;46,40,47;,
  3;47,41,48;,
  3;56,44,57;,
  3;41,55,48;,
  3;52,42,59;,
  3;49,74,60;,
  3;51,47,54;,
  3;19,74,49;,
  3;54,48,55;,
  3;55,52,59;,
  3;53,51,58;,
  3;51,61,58;,
  3;56,84,50;,
  3;63,59,64;,
  3;58,67,57;,
  3;61,68,67;,
  3;61,62,68;,
  3;62,63,69;,
  3;63,64,69;,
  3;57,66,71;,
  3;64,72,69;,
  3;66,67,75;,
  3;71,85,84;,
  3;72,78,77;,
  3;72,70,78;,
  3;65,91,70;,
  3;91,74,3;,
  3;76,77,79;,
  3;19,3,74;,
  3;73,86,85;,
  3;78,70,83;,
  3;79,77,80;,
  3;75,81,86;,
  3;79,87,82;,
  3;79,80,87;,
  3;80,90,87;,
  3;80,83,90;,
  3;81,88,86;,
  3;89,87,92;,
  3;93,90,97;,
  3;88,95,0;,
  3;90,91,97;,
  3;96,92,1;,
  3;0,5,94;,
  3;97,3,4;,
  4;13,22,5,6;,
  3;3,19,9;,
  3;1,10,7;,
  3;2,8,1;,
  3;2,12,8;,
  3;4,12,2;,
  3;0,7,6;,
  3;7,10,14;,
  3;50,5,27;,
  3;8,11,15;,
  3;12,11,8;,
  4;14,13,6,7;,
  4;16,20,13,14;,
  3;4,9,18;,
  3;15,14,10;,
  3;18,17,11;,
  4;25,24,21,17;,
  3;19,31,9;,
  3;15,17,21;,
  3;16,21,23;,
  4;28,32,22,20;,
  3;17,26,25;,
  4;7,95,96,1;,
  3;20,23,28;,
  4;45,37,38,46;,
  3;32,34,22;,
  3;26,31,36;,
  3;23,24,29;,
  3;28,29,33;,
  3;30,36,35;,
  3;28,33,44;,
  3;49,43,31;,
  3;30,35,38;,
  3;38,37,33;,
  3;27,34,56;,
  3;43,42,36;,
  4;41,47,40,39;,
  3;38,40,46;,
  4;61,51,54,62;,
  3;39,42,41;,
  3;44,56,34;,
  3;44,45,53;,
  3;49,60,43;,
  3;46,51,45;,
  3;46,47,51;,
  3;53,57,44;,
  3;41,52,55;,
  3;45,51,53;,
  3;60,59,42;,
  3;48,54,47;,
  3;55,59,63;,
  3;58,57,53;,
  4;63,62,54,55;,
  4;75,67,68,76;,
  3;56,57,71;,
  3;56,71,84;,
  3;59,65,64;,
  3;60,65,59;,
  3;61,67,58;,
  3;60,74,65;,
  3;67,66,57;,
  3;64,65,70;,
  3;62,69,68;,
  3;64,70,72;,
  4;76,68,69,77;,
  4;81,75,76,82;,
  3;69,72,77;,
  3;66,73,71;,
  3;66,75,73;,
  3;71,73,85;,
  3;65,74,91;,
  3;91,83,70;,
  3;73,75,86;,
  3;78,80,77;,
  3;78,83,80;,
  4;88,81,82,89;,
  3;76,79,82;,
  3;91,90,83;,
  4;50,84,85,94;,
  3;82,87,89;,
  4;94,85,86,0;,
  3;87,93,92;,
  3;90,93,87;,
  4;95,88,89,96;,
  3;88,0,86;,
  3;91,3,97;,
  4;2,1,92,93;,
  3;89,92,96;,
  3;93,97,2;,
  3;101,101,100;,
  3;113,107,106;,
  3;98,114,104;,
  3;102,108,107;,
  3;98,105,99;,
  3;99,110,109;,
  3;103,112,108;,
  3;111,111,115;,
  3;116,117,118;,
  3;101,119,101;,
  3;113,102,107;,
  3;98,120,114;,
  3;102,103,108;,
  3;98,104,105;,
  3;99,105,110;,
  3;103,121,112;,
  3;111,122,111;,
  3;123,124,125;,
  3;118,126,127;,
  3;127,116,118;,
  3;117,125,124;,
  3;124,118,117;,
  3;128,129,129;,
  3;130,131,132;,
  3;133,134,135;,
  3;131,136,137;,
  3;138,139,135;,
  3;140,141,138;,
  3;136,142,143;,
  3;144,145,145;,
  3;146,147,148;,
  3;129,149,129;,
  3;131,137,132;,
  3;134,150,135;,
  3;136,143,137;,
  3;139,133,135;,
  3;141,139,138;,
  3;142,151,143;,
  3;145,152,145;,
  3;153,154,155;,
  3;156,157,146;,
  3;146,148,156;,
  3;154,153,147;,
  3;147,146,154;;
 }
 MeshTextureCoords {
  114;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
